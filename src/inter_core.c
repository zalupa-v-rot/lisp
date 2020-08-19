/* ------------------------------------------------------------------------- */

#include "./inter_core.h"
#include "./clear.h"
#include "./memory.h"

#include <string.h> /* strlen() */

/* ------------------------------------------------------------------------- *
 *                                                                           *
 *                                                                           *
 * ------------------------------------------------------------------------- */

int inter_init(struct inter *pcontext, struct memory *pmh,
               struct error_stack *perr) {

  assert(pcontext != NULL);
  assert(pmh != NULL);


  pcontext->pm = pmh;
  pcontext->pm->perr = perr;
  pcontext->perr = perr;

#ifdef OPTION_HOOK_INTER_RET
  pcontext->pf_hook_inter_ret = NULL;
#endif

  dlist_init(&pcontext->sfunctions);
  dlist_init(&pcontext->sglobalvars);
  dlist_init(&pcontext->sfiles);
  pcontext->vfilesd = 0;
  dlist_init(&pcontext->svariables);
#ifdef OPTION_MATH
  sm_init(&pcontext->strmh, pmh,
          pmh->stralloc,
          pmh->strrealloc_r,
          pmh->strrealloc_l,
          pmh->strfree);
#endif
  return 0;
}

int inter_declarefunction(struct inter *pcontext,
                          const struct decfunction *pdecfun, uintp vfunn) {

  assert(pcontext != NULL);
  assert(vfunn == 0 || pdecfun != NULL);


  for ( ; vfunn != 0; vfunn--, pdecfun++) {

    if (defun_extern(pcontext, pdecfun->pname, pdecfun->pfunction)) {

      return 1;
    }
  }

  return 0;
}

#ifdef OPTION_HOOK_INTER_RET
void inter_sethook_inter_ret(struct inter *pcontext,
                             void *ptr,
                             void (*callback)(struct inter *pcontext,
                                              const char *pfilename,
                                              struct scell *proot,
                                              void *ptr)) {

  pcontext->pf_hook_inter_ret = callback;
  pcontext->ptr = ptr;

  return;
}
#endif

void inter_free(struct inter *pcontext) {

  struct dlist_node *pn;

  assert(pcontext != NULL);


  for ( ; (pn = dlist_findf(&pcontext->sfunctions)) != NULL;
        del_function(pcontext, container_of(pn, struct function, sn)));

  for ( ; (pn = dlist_findf(&pcontext->sglobalvars)) != NULL;
        del_variable(pcontext,
                     &pcontext->sglobalvars,
                     container_of(pn, struct variable, sn)));

  return;
}

int inter(struct inter *pcontext, struct scell *proot) {

  struct error_node senode;

  assert(pcontext != NULL);
  assert(proot != NULL);


  if (ERROR_CATCH(pcontext->perr, &senode)) {

    return 1;
  }

  pcontext->pcurspace = NULL;
  pcontext->vscope = 0;

  evalsexp(&pcontext->perr, proot);

  ERROR_LEAVE(pcontext->perr);
  return 0;
}

/* ------------------------------------------------------------------------- *
 *                                                                           *
 *                                                                           *
 * ------------------------------------------------------------------------- */

void defun_intern(struct error_stack **pcnterr,
                  const struct scell *pname,
                  const struct scell *pargs, const struct scell *pbody) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);
  struct error_node senode;
  struct function *pnew = NULL, *pold;


  assert(pcontext != NULL);
  assert(pname != NULL);
  assert(pargs != NULL);
  assert(pbody != NULL);


  if (ERROR_CATCH(pcontext->perr, &senode)) {

    if (pnew != NULL) {

      clear_scell(pcontext->pm, &pnew->sname);
      clear_scell(pcontext->pm, &pnew->u.sintern.sargs);
      clear_scell(pcontext->pm, &pnew->u.sintern.sbody);
      funfree(pcontext->pm, pnew);
    }

    ERROR_SILENT(*pcnterr);
    return;
  }

  {
    pnew = funalloc(pcontext->pm);
    pnew->vtype = FUNTYPE_INTERN;

    INITNIL(pnew->sname.vtype);
    INITNIL(pnew->u.sintern.sargs.vtype);
    INITNIL(pnew->u.sintern.sbody.vtype);
    copysexp(pcontext, &pnew->sname, pname);
    copysexp(pcontext, &pnew->u.sintern.sargs, pargs);
    copysexp(pcontext, &pnew->u.sintern.sbody, pbody);

    pold = find_function(pcontext, &pnew->sname.u.sobject);

    if (pold != NULL) {

      /* удаление старой только после успешного выделения новой. */

      del_function(pcontext, pold);
    }

    dlist_addl(&pcontext->sfunctions, &pnew->sn);
  }

  ERROR_LEAVE(pcontext->perr);
  return;
}

void setvar(struct error_stack **pcnterr,
            const struct scell *pname, const struct scell *pvalue) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);
  struct error_node senode;
  struct variable *pold;
  struct scell snewval;

  assert(pcontext != NULL);
  assert(pname != NULL);
  assert(pvalue != NULL);


  if ((pold = find_variable(pcontext, &pname->u.sobject)) == NULL) {

    defvar(&pcontext->perr, pname, pvalue);
    return;
  }

  INITNIL(snewval.vtype);

  if (ERROR_CATCH(pcontext->perr, &senode)) {

    clear_scell(pcontext->pm, &snewval);
    ERROR_SILENT(pcontext->perr);
    return;
  }

  copysexp(pcontext, &snewval, pvalue);
  clear_scell(pcontext->pm, &pold->svalue);
  pold->svalue = snewval;

  ERROR_LEAVE(pcontext->perr);
  return;
}

void defvar(struct error_stack **pcnterr,
            const struct scell *pname, const struct scell *pvalue) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);
  struct variable *pnew = NULL;

  assert(pcontext != NULL);
  assert(pname != NULL);
  assert(pvalue != NULL);


  pnew = varalloc(pcontext->pm);
  INITNIL(pnew->sname.vtype);
  INITNIL(pnew->svalue.vtype);
  copysexp(pcontext, &pnew->sname, pname);
  copysexp(pcontext, &pnew->svalue, pvalue);

  if (pcontext->vscope == 0) {

    dlist_addl(&pcontext->sglobalvars, &pnew->sn);

  } else {

    dlist_addl(&pcontext->svariables, &pnew->sn);
  }

  return;
}

void* varscope_enter(struct error_stack **pcnterr) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);


  pcontext->vscope++;
  return dlist_findl(&pcontext->svariables);
}

void varscope_exit(struct error_stack **pcnterr, void *psepar) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);


  pcontext->vscope--;
  del_variables_upto(pcontext, psepar);
  return;
}

void evalsexp(struct error_stack **pcnterr, struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);

  assert(pcontext != NULL);
  assert(pret != NULL);


  switch(TYPEGET(pret->vtype)) {

  case TYPE_SEXP:
    {
      struct function *pcalleefunc;
      struct snode *pname = pret->u.psexp;

      /* это должна быть функция - первый атом. второй - аргументы */

      if (TYPEGET(pname->s[0].vtype) != TYPE_OBJECT) {

        ERRORFLAG_ON(pname->s[0].vtype);
        ERROR(pcontext->perr, NOT_A_FUNCTION);
      }

      if ((pcalleefunc = find_function(pcontext,
                                       &pname->s[0].u.sobject)) == NULL) {

        ERRORFLAG_ON(pname->s[0].vtype);
        ERROR(pcontext->perr, UNDEFINED_FUNCTION);
      }

      if (TYPEGET(pcalleefunc->vtype) == FUNTYPE_EXTERN) {

        pcalleefunc->u.sextern.pfun(&pcontext->perr, pname, pret);

      } else {

        intern_call(pcontext, &pcalleefunc->u.sintern, pname, pret);
      }

      clear_snode(pcontext->pm, pname);
    }
    break;

  case TYPE_NIL:
    break;

  case TYPE_OBJECT:
    {
      struct variable *pvar;

#ifdef OPTION_NUMBERS
      if (MARKGET(pret->vtype) == MARK_NUMBER) {

        break;
      }

#ifdef OPTION_MATH
      {
        uintp vpoint, vfirstnonz, vnewlen;
        struct string *pnum = &pret->u.sobject;


        if (string_isnumber(pnum, &vpoint, &vfirstnonz)) {


          MARKSET(pret->vtype, MARK_NUMBER);

          pret->vpoint = pnum->vlen - vpoint - 1;
          /* pret->vpoint = (pret->vpoint == -1)?0:pret->vpoint; */
          vnewlen = pnum->vlen;

          if (vpoint > vfirstnonz && pnum->pstr[vpoint] == '.') {

            /* точка внутри числа без лид нулей
               смещение дроби на точку */

            memmove(pnum->pstr + vpoint, pnum->pstr + vpoint + 1,
                    pnum->vlen - vpoint - 1);
            vnewlen --;
          }

          /* сокращение нулей */

          if (pnum->pstr[vfirstnonz] == '0') {

            /* число нулевое. */

            memmove(pnum->pstr, pnum->pstr + vfirstnonz,
                    vnewlen - vfirstnonz);
            vnewlen -= vfirstnonz;

          } else {

            memmove(pnum->pstr + (*pnum->pstr == '-'), pnum->pstr + vfirstnonz,
                    vnewlen - vfirstnonz);
            vnewlen -= vfirstnonz - (*pnum->pstr == '-');
          }

          strrealloc_r(pcontext->pm, pnum, vnewlen);

          break;
        }
      }
#else
      if (string_isnumber(&pret->u.sobject)) {

        MARKSET(pret->vtype, MARK_NUMBER);
        break;
      }
#endif

#endif

      if ((pvar = find_variable(pcontext, &pret->u.sobject)) == NULL) {

        ERRORFLAG_ON(pret->vtype);
        ERROR(pcontext->perr, UNDEFINED_VARIABLE);
      }

      strfree(pcontext->pm, &pret->u.sobject);

      copysexp(pcontext, pret, &pvar->svalue);
    }
    break;
  }

  return;
}

/* ------------------------------------------------------------------------- *
 *                                                                           *
 *                                                                           *
 * ------------------------------------------------------------------------- */

void del_function(struct inter *pcontext, struct function *pfunc) {


  if (pfunc->vtype == FUNTYPE_INTERN) {

    clear_scell(pcontext->pm, &pfunc->sname);
    clear_scell(pcontext->pm, &pfunc->u.sintern.sargs);
    clear_scell(pcontext->pm, &pfunc->u.sintern.sbody);
  }

  dlist_del(&pcontext->sfunctions, &pfunc->sn);
  funfree(pcontext->pm, pfunc);
  return;
}

struct function* find_function(struct inter *pcontext, struct string *pname) {

  struct dlist_node *pn;
  struct function *pfun = NULL;
  struct dlist_head *plist = &pcontext->sfunctions;


  for (pn = dlist_findf(plist); ; pn = dlist_tol(pn)) {

    if (pn == NULL) { return NULL; }

    pfun = container_of(pn, struct function, sn);

    if (string_eq(&pfun->sname.u.sobject, pname)) { return pfun; }
  }
}

void* varspace_enter(struct inter *pcontext) {

  struct dlist_node *pold = pcontext->pcurspace;

  pcontext->vscope++;
  pcontext->pcurspace = dlist_findl(&pcontext->svariables);
  return pold;
}

void varspace_exit(struct inter *pcontext, void *psepar) {


  pcontext->vscope--;
  del_variables_upto(pcontext, pcontext->pcurspace);
  pcontext->pcurspace = psepar;
  return;
}

void del_variable(struct inter *pcontext,
                  struct dlist_head *plist, struct variable *pvar) {


  dlist_del(plist, &pvar->sn);
  clear_scell(pcontext->pm, &pvar->sname);
  clear_scell(pcontext->pm, &pvar->svalue);
  varfree(pcontext->pm, pvar);
  return;
}

void del_variables_upto(struct inter *pcontext, struct dlist_node *psepar) {

  struct dlist_node *pn;


  for ( ; ; ) {

    if ((pn = dlist_findl(&pcontext->svariables)) == psepar) { return; }
    assert(pn != NULL);

    del_variable(pcontext,
                 &pcontext->svariables, container_of(pn, struct variable, sn));
  }
}

struct variable* find_variable2(struct dlist_head *plist,
                                struct dlist_node *psepar,
                                const struct string *pname) {

  struct dlist_node *pn;
  struct variable *pvar = NULL;


  for (pn = dlist_findl(plist); ;
       pn = dlist_tof(pn)) {

    if (pn == psepar) { return NULL; }
    assert(pn != NULL);

    pvar = container_of(pn, struct variable, sn);

    if (string_eq(&pvar->sname.u.sobject, pname)) { return pvar; }
  }
}

struct variable* find_variable(struct inter *pcontext,
                               const struct string *pname) {

  struct variable *pvar;


  if ((pvar = find_variable2(&pcontext->svariables,
                             pcontext->pcurspace, pname)) != NULL) {

    return pvar;
  }

  return find_variable2(&pcontext->sglobalvars, NULL, pname);
}

void copysexp2(struct inter *pcontext,
               struct scell *pdstroot, const struct snode *psexpsrc) {

  struct scell *pdst;
  const struct scell *psrc;
  struct snode *psexpdst;
  uint va;


  INITNIL(pdstroot->vtype);

  pdstroot->u.psexp = psexpdst = sexpalloc(pcontext->pm);
  TYPESET(pdstroot->vtype, TYPE_SEXP);
  INITNIL(psexpdst->s[0].vtype);
  INITNIL(psexpdst->s[1].vtype);

  for (va = 0; va != 2; va++) {

    pdst = &psexpdst->s[va], psrc = &psexpsrc->s[va];

    switch(TYPEGET(psrc->vtype)) {

    case TYPE_SEXP:
      copysexp2(pcontext, pdst, psrc->u.psexp);
      break;

    case TYPE_OBJECT:
      stralloc(pcontext->pm, &pdst->u.sobject, psrc->u.sobject.vlen);
      memcpy(pdst->u.sobject.pstr, psrc->u.sobject.pstr, psrc->u.sobject.vlen);
      pdst->vtype = psrc->vtype;
#ifdef OPTION_MATH
      pdst->vpoint = psrc->vpoint;
#endif
      break;

    default:
    case TYPE_NIL:
      break;
    }
  }

  return;
}

void copysexp(struct inter *pcontext,
              struct scell *pdst, const struct scell *psrc) {


  assert(pdst != NULL);
  assert(psrc != NULL);

  INITNIL(pdst->vtype);


  switch(TYPEGET(psrc->vtype)) {

  case TYPE_SEXP:
    copysexp2(pcontext, pdst, psrc->u.psexp);
    break;

  case TYPE_OBJECT:
    stralloc(pcontext->pm, &pdst->u.sobject, psrc->u.sobject.vlen);
    memcpy(pdst->u.sobject.pstr, psrc->u.sobject.pstr, psrc->u.sobject.vlen);
    pdst->vtype = psrc->vtype;
#ifdef OPTION_MATH
    pdst->vpoint = psrc->vpoint;
#endif
    break;

  default:
  case TYPE_NIL:
    break;
  }

  return;
}

int defun_extern(struct inter *pcontext,
                 const char *pname,
                 void (*pfun)(struct error_stack **pcnterr,
                              struct snode *pargs, struct scell *pret)) {

  struct error_node senode;
  struct function *pnew = NULL;


  if (ERROR_CATCH(pcontext->perr, &senode)) {

    if (pnew != NULL) {

      funfree(pcontext->pm, pnew);
    }

    return 1;
  }

  {
    pnew = funalloc(pcontext->pm);
    pnew->vtype = FUNTYPE_EXTERN;

    /* внешние функции при переопределении не освобождают имя */
    pnew->sname.u.sobject.pstr = (char*)pname;
    pnew->sname.u.sobject.vlen = strlen(pname);
    pnew->u.sextern.pfun = pfun;

    dlist_addl(&pcontext->sfunctions, &pnew->sn);
  }

  ERROR_LEAVE(pcontext->perr);
  return 0;
}

/*
  (name args ...)
  |
  |               function body
  v
  (name args ... s ... s   last s)
  ^      ^       |     ^      ^ <-+ копируется scell
  *>     *>      *>    *>     *>nil и проставляется nil
  ^                               | перед возвриатом
  |                               |
  clear                           |
  .                               v
  .                              ret
*/
void intern_call(struct inter *pcontext,
                 struct function_intern *pintern,
                 struct snode *phead, struct scell *pret) {

  struct dlist_node *poldspace;


  /* первый phead->s[0] - имя функции */

  if (TYPEGET(pintern->sargs.vtype) != TYPE_NIL) {

    struct snode *pdescr, *pargs;

    /* исполнение аргументов в старом пространстве локальных переменных */

    for (pdescr = pintern->sargs.u.psexp, pargs = phead; ; ) {

      if (TYPEGET(pargs->s[1].vtype) != TYPE_SEXP) {

        ERRORFLAG_ON(phead->s[1].vtype);
        ERROR(pcontext->perr, WRONG_NUMBER_OF_ARGUMENTS);
      }

      pargs = pargs->s[1].u.psexp;

      evalsexp(&pcontext->perr, &pargs->s[0]);


      if (TYPEGET(pdescr->s[1].vtype) == TYPE_NIL) {

        if (TYPEGET(pargs->s[1].vtype) != TYPE_NIL) {

          ERRORFLAG_ON(pargs->s[1].vtype);
          ERROR(pcontext->perr, WRONG_NUMBER_OF_ARGUMENTS);
        }

        break;
      }

      pdescr = pdescr->s[1].u.psexp;
    }

    /* добавление переменных в новое пространство */

    poldspace = varspace_enter(pcontext);

    for (pdescr = pintern->sargs.u.psexp, pargs = phead; ; ) {

      pargs = pargs->s[1].u.psexp;

      defvar(&pcontext->perr, &pdescr->s[0], &pargs->s[0]);

      if (TYPEGET(pdescr->s[1].vtype) == TYPE_NIL) {
        break;
      }

      pdescr = pdescr->s[1].u.psexp;
    }

    phead = pargs;


  } else {

    if (TYPEGET(phead->s[1].vtype) != TYPE_NIL) {

      ERRORFLAG_ON(phead->s[1].vtype);
      ERROR(pcontext->perr, WRONG_NUMBER_OF_ARGUMENTS);
    }

    poldspace = varspace_enter(pcontext);
  }

  /* выполнение */

  {
    struct scell *plast = &phead->s[1];


    copysexp(pcontext, &phead->s[1], &pintern->sbody);


    for ( ; ; ) {

      if (TYPEGET(phead->s[1].vtype) == TYPE_NIL) {

        break;
      }

      assert(TYPEGET(phead->s[1].vtype) == TYPE_SEXP);

      phead = phead->s[1].u.psexp;
      plast = &phead->s[0];

      evalsexp(&pcontext->perr, plast);
    }

    *pret = *plast;
    TYPESET(plast->vtype, TYPE_NIL);

    varspace_exit(pcontext, poldspace);
  }

  return;
}

/* ------------------------------------------------------------------------- */
