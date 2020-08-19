/*------------------------------------------------------------------------------*/

#include "./parse.h"
#include "./clear.h"
#include "./memory.h"

/*------------------------------------------------------------------------------*/

static void skip_symbol(struct parse *ppar) {

  ppar->vcolumn++, ppar->pt++;
  return;
}

static int skip_space(struct parse *ppar) {

  for ( ; ; ppar->vcolumn++, ppar->pt++) {

    if (ppar->pt == ppar->pte) {
      return 1;
    }

    if (*ppar->pt == ' ') { continue; }
    if (*ppar->pt == '\n') { ppar->vline++, ppar->vcolumn = -1; continue; }

    return 0;
  }
}

static int anyof(char vch, const char *ps) {

  for ( ; ; ) {

    if (vch == *ps) {

      return 1;
    }

    if (*++ps == '\0') { return 0; }
  }
}

static int skip_object(struct parse *ppar) {

  for ( ; ; ppar->vcolumn++, ppar->pt++) {

    if (ppar->pt == ppar->pte) {
      return 1;
    }

    if (anyof(*ppar->pt, ")( \n")) {
      return 0;
    }
  }
}


static void parse_pair(struct parse *ppar, struct scell *ptr);
static void parse_list(struct parse *ppar, struct scell *ptr);

static void parse_pair(struct parse *ppar, struct scell *ptr) {


  if (skip_space(ppar)) {

    ERROR(ppar->perr, UNEXPECTED_END);
  }

  if (*ppar->pt == ')') {

    ERROR(ppar->perr, UNEXPECTED_CLOSING_BRACKET);
  }

  if (*ppar->pt == '.') {

    ERROR(ppar->perr, UNEXPECTED_DOT);
  }

  {
    if (*ppar->pt == '(') {

      skip_symbol(ppar);
      ppar->vscope++;

      parse_list(ppar, ptr);

      /* if (TYPEGET(ptr->vtype) == TYPE_NIL) { */

      /*   ERROR(ppar->perr,  EMPTY_LIST); */
      /* } */

    } else {

      {
        char *ptmp = (char*)ppar->pt;

        skip_object(ppar);

        strdeclare(ppar->pm, &ptr->u.sobject, ptmp, ppar->pt - ptmp);
        TYPESET(ptr->vtype, TYPE_OBJECT);
      }
    }
  }

  if (skip_space(ppar)) {

    ERROR(ppar->perr, UNEXPECTED_END);
  }

  if (*ppar->pt != ')') {

    ERROR(ppar->perr, CLOSING_BRACKET_EXPECTED);
  }

  skip_symbol(ppar);
  ppar->vscope--;
  return;
}

static void parse_list(struct parse *ppar, struct scell *ptr) {


  if (skip_space(ppar)) {

    ERROR(ppar->perr, UNEXPECTED_END);
  }

  if (*ppar->pt == ')') {

    skip_symbol(ppar);
    ppar->vscope--;
    /* TYPESET(ptr->vtype, TYPE_NIL); */
    return;
  }

  if (*ppar->pt == '.') {

    if (ppar->pte != ppar->pt && *(ppar->pt+1) == ' ') {

      skip_symbol(ppar);
      parse_pair(ppar, ptr);
      return;
    }
  }

  {
    struct snode *psexp;

    ptr->u.psexp = psexp = sexpalloc(ppar->pm);
    TYPESET(ptr->vtype, TYPE_SEXP);
    INITNIL(psexp->s[0].vtype);
    INITNIL(psexp->s[1].vtype);

    if (*ppar->pt == '(') {

      skip_symbol(ppar);
      ppar->vscope++;

      parse_list(ppar, &psexp->s[0]);

      /* if (TYPEGET(psexp->s[0].vtype) == TYPE_NIL) { */

      /*   ERROR(ppar->perr, EMPTY_LIST); */
      /* } */

    } else {

      ptr = &psexp->s[0];

      {
        char *ptmp = (char*)ppar->pt;

        skip_object(ppar);

        strdeclare(ppar->pm, &ptr->u.sobject, ptmp, ppar->pt - ptmp);
        TYPESET(ptr->vtype, TYPE_OBJECT);
      }
    }

    parse_list(ppar, &psexp->s[1]);
  }

  return;
}

void parse_init(struct parse *ppar,
                struct memory *pm, struct error_stack *perr) {

  assert(ppar != NULL);
  assert(pm != NULL);
  assert(perr != NULL);


  ppar->pm = pm;
  ppar->perr = perr;
  return;
}

int parse(struct parse *ppar,
          const char *pstr, uintp vlen, struct scell *proot) {

  struct error_node senode;

  assert(ppar != NULL);
  assert(pstr != NULL);
  assert(proot != NULL);


  ppar->vcolumn = 0;
  ppar->vline = 0;

  ppar->vscope = 0;
  ppar->pt = ppar->pts = pstr;
  ppar->pte = pstr + vlen;

  /* заглушка на случай ошибки */
  INITNIL(proot->vtype);


  if (ERROR_CATCH(ppar->perr, &senode)) {

    clear_scell(ppar->pm, proot);
    INITNIL(proot->vtype);
    return 1;
  }

  do {

    if (skip_space(ppar)) {

      break;
    }

    if (*ppar->pt != '(') {

      ERROR(ppar->perr, OPERNING_BRACKET_EXPECTED);
    }

    skip_symbol(ppar);
    ppar->vscope++;
    parse_list(ppar, proot);

  } while(0);

  ERROR_LEAVE(ppar->perr);
  return 0;
}


uintp parse_line(struct parse *ppar) {

  return ppar->vline;
}

uintp parse_column(struct parse *ppar) {

  return ppar->vcolumn;
}

uintp parse_ending(struct parse *ppar) {

  return ppar->pt - ppar->pts;
}

/*------------------------------------------------------------------------------*/

struct print {

  char  *pstr;
  uintp  vleft, verrorpos;
};

static void putst(struct print *ppri, const char *pstr, uintp vlen) {

  if (vlen > ppri->vleft) {

    vlen = ppri->vleft;
  }

  ppri->vleft -= vlen;

  if (ppri->pstr != NULL) {

    for ( ; vlen != 0; --vlen, *ppri->pstr++ = *pstr++);
  }

  return;
}


#ifdef OPTION_MATH

static void put_zeros(struct print *ppri, uintp vlen) {

  if (vlen > ppri->vleft) {

    vlen = ppri->vleft;
  }

  ppri->vleft -= vlen;

  if (ppri->pstr != NULL) {

    for ( ; vlen != 0; --vlen, *ppri->pstr++ = '0');
  }

  return;
}

static void put_number(struct print *ppri, const struct string *ps, uintp vpoint) {

  struct string ss = *ps;


  if (ss.pstr[0] == '-') {

    putst(ppri, "-", 1);
    ss.pstr++, ss.vlen--;
  }

  if (vpoint >= ss.vlen) {

    /* целое */
    putst(ppri, "0.", 2);

    /* дополнение нулями */

    put_zeros(ppri, vpoint - ss.vlen);

    /* дробь */

    putst(ppri, ss.pstr, ss.vlen);

  } else {

    putst(ppri, ss.pstr, ss.vlen - vpoint);

    if (vpoint) {

      putst(ppri, ".", 1);
      putst(ppri, &ss.pstr[ss.vlen - vpoint], vpoint);
    }
  }

  return;
}

#endif

static void print_scell(struct scell *proot, struct print *ppri);
static void print_snode(struct snode *proot, struct print *ppri);


static void print_scell(struct scell *proot, struct print *ppri) {

#ifndef NDEBUG
  if (proot == NULL) {
    return;
  }
#endif

  if (ERRORFLAG_GET(proot->vtype) == ERROR_ON) {

    assert(ppri->verrorpos == (uintp)-1);
    ppri->verrorpos = ppri->vleft;
  }

  switch(TYPEGET(proot->vtype)) {

  case TYPE_SEXP:
    putst(ppri, "(", 1);
    print_snode(proot->u.psexp, ppri);
    putst(ppri, ")", 1);
    break;

  case TYPE_OBJECT:
#ifdef OPTION_MATH
    if (MARKGET(proot->vtype) == MARK_NUMBER) {

      put_number(ppri, &proot->u.sobject, proot->vpoint);
      break;
    }
#endif
    putst(ppri, proot->u.sobject.pstr, proot->u.sobject.vlen);
    break;

  case TYPE_NIL:
    putst(ppri, "nil", 3);
    break;
  }

  return;
}

static void print_snode(struct snode *proot, struct print *ppri) {

  struct scell *ptr;

#ifndef NDEBUG
  if (proot == NULL) {
    return;
  }
#endif

  print_scell(&proot->s[0], ppri);

  ptr = &proot->s[1];

  if (ERRORFLAG_GET(ptr->vtype) == ERROR_ON) {

    assert(ppri->verrorpos == (uintp)-1);
    ppri->verrorpos = ppri->vleft;
  }

  switch(TYPEGET(ptr->vtype)) {

  case TYPE_SEXP:
    putst(ppri, " ", 1);
    print_snode(ptr->u.psexp, ppri);
    break;

  case TYPE_OBJECT:
    putst(ppri, " . ", 3);
    putst(ppri, ptr->u.sobject.pstr, ptr->u.sobject.vlen);
    break;

  case TYPE_NIL:
    break;
  }

  return;
}

uintp print_tree(struct scell *proot, char *pstr, uintp vlen, uintp *perrpos) {

  struct print spri;

  assert(proot != NULL);


  spri.verrorpos = -1;

  spri.pstr = pstr;
  spri.vleft = vlen;

  print_scell(proot, &spri);


  if (spri.verrorpos == (uintp)-1) {

    if (perrpos != NULL) { *perrpos = vlen-spri.vleft; }

  } else {

    if (perrpos != NULL) { *perrpos = vlen-spri.verrorpos; }
  }

  return vlen - spri.vleft;
}

/*------------------------------------------------------------------------------*/
