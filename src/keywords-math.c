/* ------------------------------------------------------------------------- */

#include "./keywords.h"
#include "./inter_kw.h"
#include "./math.h"
#include "./memory.h"

#include <string.h>

#ifndef OPTION_MATH
#error
#endif

/* ------------------------------------------------------------------------- */

/* это всё нужно убрать !!! */

#define UPWARD  0
#define FORWARD 1

#define snode_move_strict(P, V) snode_move_strict_(pcontext, P, V)


static struct snode* snode_move_strict_(struct inter *pcontext,
                                        struct snode *ppoint, uintp vi) {

  if (TYPEGET(ppoint->s[vi].vtype) != TYPE_SEXP) {

    ERROR(pcontext->perr, WRONG_ARGUMENT);
  }

  return ppoint->s[vi].u.psexp;
}

static int snode_move_(struct snode **ppoint, uintp vi) {

  if (TYPEGET((*ppoint)->s[vi].vtype) != TYPE_SEXP) {

    return 1;
  }

  *ppoint = (*ppoint)->s[vi].u.psexp;
  return 0;
}

#define snode_check_completion(P, V)            \
  snode_check_completion_(pcontext, P, V)

static void snode_check_completion_(struct inter *pcontext,
                                    struct snode *ppoint, uintp vi) {

  if (TYPEGET(ppoint->s[vi].vtype) != TYPE_NIL) {

    ERROR(pcontext->perr, INCORRECT_COMPLETION);
  }

  return;
}

#define MIN(A, B) (((A)<(B))?(A):(B))
#define MAX(A, B) (((A)>(B))?(A):(B))

/* ------------------------------------------------------------------------- */

static void number_init_one(struct inter *pcontext,
                            struct string *pa, uintp vzeropadd) {

  stralloc(pcontext->pm, pa, 1 + vzeropadd);

  pa->pstr[0] = '1';

  memset(&pa->pstr[1], '0', vzeropadd);

  return;
}

static char number_iszero(struct string *pa) {

  return (pa->vlen == 1 && pa->pstr[0] == '0');
}

static void number_conversion(struct inter *pcontext,
                              struct scell *pa, uintp vpoint) {

  uintp voldlen, vdiff;


  if (pa->vpoint < vpoint) {

    /* расширение дроби вправо, с добавлением нулей */

    voldlen = pa->u.sobject.vlen;
    vdiff = vpoint - pa->vpoint;

    strrealloc_r(pcontext->pm, &pa->u.sobject, voldlen + vdiff);
    memset(&pa->u.sobject.pstr[voldlen], '0', vdiff);

    pa->vpoint += vdiff;

  } else {

    /* сужение дроби влево */

    vdiff = pa->vpoint - vpoint;

    if (vdiff >= pa->u.sobject.vlen - (pa->u.sobject.pstr[0] == '-')) {

      /* сужение до нуля */

      strrealloc_r(pcontext->pm, &pa->u.sobject, 1);
      pa->u.sobject.pstr[0] = '0';
      pa->vpoint -= vdiff;

      return;
    }

    strrealloc_r(pcontext->pm, &pa->u.sobject, pa->u.sobject.vlen - vdiff);
    pa->vpoint -= vdiff;
  }

  return;
}

static void numbers_equalization(struct inter *pcontext,
                                 struct scell *pa, struct scell *pb) {

  if (pa->vpoint > pb->vpoint) {

    number_conversion(pcontext, pb, pa->vpoint);

  } else {

    number_conversion(pcontext, pa, pb->vpoint);
  }

  return;
}

/* ------------------------------------------------------------------------- */

/*
  аргументы обрабатываются в один проход. исполняются, потом производится
  операция - над первым аргументом каждым последующим (с конвертацией перед
  этим). сами аргументы при этом не сокращаются.

  число аргументов от одного.

  x = {+ - * / %}

  (x o ...)
  ^  ^ <----+ копируется scell
  *> *>     | и проставляется nil
  ^         | перед возвриатом
  |         ret
  clear
*/

static void math_function(struct inter *pcontext,
                          struct snode *phead,
                          struct scell *pret,
                          void (*pf_one)(struct inter *pcontext,
                                         struct scell *pone),
                          void (*pf_next)(struct inter *pcontext,
                                          struct scell *pfirst,
                                          struct scell *pnext)) {

  struct snode *pargs, *pfirst;


  if (TYPEGET(phead->s[FORWARD].vtype) != TYPE_SEXP) {
    /* нет аргументов */
    ERROR(pcontext->perr, WRONG_ARGUMENT);
  }


  /* первый аргумент */

  pfirst = phead->s[FORWARD].u.psexp;

  evalsexp(&pcontext->perr, &pfirst->s[UPWARD]);


  if (MARKGET(pfirst->s[UPWARD].vtype) != MARK_NUMBER) {

    ERROR(pcontext->perr, WRONG_ARGUMENT);
  }

  if (TYPEGET(pfirst->s[FORWARD].vtype) != TYPE_SEXP) {

    snode_check_completion(pfirst, FORWARD);

    /* и последний */

    pf_one(pcontext, &pfirst->s[UPWARD]);

    *pret = pfirst->s[UPWARD];
    TYPESET(pfirst->s[UPWARD].vtype, TYPE_NIL);
    return;
  }


  pargs = snode_move_strict(pfirst, FORWARD);


  for ( ; ; ) {

    evalsexp(&pcontext->perr, &pargs->s[UPWARD]);

    if (MARKGET(pargs->s[UPWARD].vtype) != MARK_NUMBER) {

      ERROR(pcontext->perr, WRONG_ARGUMENT);
    }

    pf_next(pcontext, &pfirst->s[UPWARD], &pargs->s[UPWARD]);

    if (snode_move_(&pargs, FORWARD)) {

      snode_check_completion(pargs, FORWARD);
      break;
    }
  }

  *pret = pfirst->s[UPWARD];
  TYPESET(pfirst->s[UPWARD].vtype, TYPE_NIL);
  return;
}

static void nothing_one(struct inter *pcontext, struct scell *pone) {

  UNUSED(pcontext);
  UNUSED(pone);
  return;
}

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

void addition_next(struct inter *pcontext,
                   struct scell *pfirst, struct scell *pnext) {

  numbers_equalization(pcontext, pfirst, pnext);

  if (sm_add(&pcontext->strmh,
             &pfirst->u.sobject, &pnext->u.sobject)) {

    ERROR(pcontext->perr, OUT_OF_MEMORY);
  }

  return;
}

void kw_addition(struct error_stack **pcnterr,
                 struct snode *phead, struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);


  math_function(pcontext, phead, pret, nothing_one, addition_next);
  return;
}


static void subtraction_one(struct inter *pcontext, struct scell *pone) {

  if (sm_not(&pcontext->strmh, &pone->u.sobject)) {

    ERROR(pcontext->perr, OUT_OF_MEMORY);
  }
  return;
}

void subtraction_next(struct inter *pcontext,
                      struct scell *pfirst, struct scell *pnext) {

  numbers_equalization(pcontext, pfirst, pnext);

  if (sm_sub(&pcontext->strmh,
             &pfirst->u.sobject, &pnext->u.sobject)) {

    ERROR(pcontext->perr, OUT_OF_MEMORY);
  }

  return;
}

void kw_subtraction(struct error_stack **pcnterr,
                    struct snode *phead, struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);


  math_function(pcontext, phead, pret, subtraction_one, subtraction_next);
  return;
}


/*
  floor(a * b / min(a.point, b.point)) * 0.1^max(a.point, b.point)
*/
void multiplication_next(struct inter *pcontext,
                         struct scell *pfirst, struct scell *pnext) {

  struct string sresult;


  if (sm_mul(&pcontext->strmh, &pfirst->u.sobject,
             &pnext->u.sobject, &sresult)) {

    ERROR(pcontext->perr, OUT_OF_MEMORY);
  }

  strfree(pcontext->pm, &pfirst->u.sobject);
  pfirst->u.sobject = sresult;

  {
    uintp vmaxpoint = MAX(pfirst->vpoint, pnext->vpoint);

    pfirst->vpoint += pnext->vpoint;
    number_conversion(pcontext, pfirst, vmaxpoint);
  }

  return;
}

void kw_multiplication(struct error_stack **pcnterr,
                       struct snode *phead, struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);


  math_function(pcontext, phead, pret, nothing_one, multiplication_next);
  return;
}


/*
  floor(1 * 10^(a.point*2) / a) * 0.1^a.point
*/
static void division_one(struct inter *pcontext, struct scell *pone) {

  struct string stmp, sresult;


  if (number_iszero(&pone->u.sobject)) {

    ERROR(pcontext->perr, DIVISION_BY_ZERO);
  }

  number_init_one(pcontext, &stmp, pone->vpoint * 2);

  if (sm_div(&pcontext->strmh, &stmp, &pone->u.sobject, &sresult)) {

    strfree(pcontext->pm, &stmp);
    ERROR(pcontext->perr, OUT_OF_MEMORY);
  }

  strfree(pcontext->pm, &pone->u.sobject);
  strfree(pcontext->pm, &stmp);

  pone->u.sobject = sresult;

  return;
}

/*
  conv(a, max(a.point, b.point))
  floor(a * 10^b.point / b) * 0.1^a.point
*/
void division_next(struct inter *pcontext,
                   struct scell *pfirst, struct scell *pnext) {

  {
    uintp vmaxpoint = MAX(pfirst->vpoint, pnext->vpoint);

    number_conversion(pcontext, pfirst, vmaxpoint + pnext->vpoint);
  }

  {
    struct string sresult;


    if (number_iszero(&pnext->u.sobject)) {

      ERROR(pcontext->perr, DIVISION_BY_ZERO);
    }

    if (sm_div(&pcontext->strmh, &pfirst->u.sobject,
               &pnext->u.sobject, &sresult)) {

      ERROR(pcontext->perr, OUT_OF_MEMORY);
    }

    strfree(pcontext->pm, &pfirst->u.sobject);

    /* по факту дробь уже сама и ужалась */
    pfirst->vpoint -= pnext->vpoint;
    pfirst->u.sobject = sresult;
  }
  return;
}

void kw_division(struct error_stack **pcnterr,
                 struct snode *phead, struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);


  math_function(pcontext, phead, pret, division_one, division_next);
  return;
}

/*
  1 * 10^a.point % a * 0.1^a.point
*/
static void divremainder_one(struct inter *pcontext, struct scell *pone) {

  struct string stmp, sresult;


  if (number_iszero(&pone->u.sobject)) {

    ERROR(pcontext->perr, DIVISION_BY_ZERO);
  }

  number_init_one(pcontext, &stmp, pone->vpoint);

  if (sm_div(&pcontext->strmh, &stmp, &pone->u.sobject, &sresult)) {

    strfree(pcontext->pm, &sresult);
    ERROR(pcontext->perr, OUT_OF_MEMORY);
  }

  strfree(pcontext->pm, &pone->u.sobject);
  strfree(pcontext->pm, &sresult);

  pone->u.sobject = stmp;

  return;
}

/*
  conv(a, max(a.point, b.point))
  conv(b, max(a.point, b.point))
  floor(a % b) * 0.1^a.point
*/
void divremainder_next(struct inter *pcontext,
                       struct scell *pfirst, struct scell *pnext) {


  numbers_equalization(pcontext, pfirst, pnext);

  {
    struct string sresult;


    if (number_iszero(&pnext->u.sobject)) {

      ERROR(pcontext->perr, DIVISION_BY_ZERO);
    }

    if (sm_div(&pcontext->strmh, &pfirst->u.sobject, &pnext->u.sobject,
               &sresult)) {

      strfree(pcontext->pm, &sresult);
      ERROR(pcontext->perr, OUT_OF_MEMORY);
    }

    strfree(pcontext->pm, &sresult);
  }
  return;
}

void kw_divremainder(struct error_stack **pcnterr,
                     struct snode *phead, struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);


  math_function(pcontext, phead, pret, divremainder_one, divremainder_next);
  return;
}


/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . *
 *                                                                           *
 *                                                                           *
 * . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

static void compare_function(struct inter *pcontext,
                             struct snode *phead, struct scell *pret,
                             int (*pf_next)(struct inter *pcontext,
                                            struct scell *pcurr,
                                            struct scell *pnext)) {

  struct snode *pfirst, *pnext;


  if (TYPEGET(phead->s[FORWARD].vtype) != TYPE_SEXP) {
    /* нет аргументов */
    ERROR(pcontext->perr, WRONG_ARGUMENT);
  }


  /* первый аргумент */

  pnext = pfirst = phead->s[FORWARD].u.psexp;

  evalsexp(&pcontext->perr, &pfirst->s[UPWARD]);


  if (MARKGET(pfirst->s[UPWARD].vtype) != MARK_NUMBER) {

    ERROR(pcontext->perr, WRONG_ARGUMENT);
  }

  if (TYPEGET(pnext->s[FORWARD].vtype) == TYPE_SEXP) {

    struct snode *pcurr;

    pcurr = pnext;
    pnext = snode_move_strict(pnext, FORWARD);

    for ( ; ; ) {

      evalsexp(&pcontext->perr, &pnext->s[UPWARD]);

      if (MARKGET(pnext->s[UPWARD].vtype) != MARK_NUMBER) {

        ERROR(pcontext->perr, WRONG_ARGUMENT);
      }

      if (pf_next(pcontext, &pcurr->s[UPWARD], &pnext->s[UPWARD])) {

        TYPESET(pret->vtype, TYPE_NIL);
        return;
      }

      if (pcurr = pnext, snode_move_(&pnext, FORWARD)) {

        break;
      }
    }
  }

  snode_check_completion(pnext, FORWARD);

  strrealloc_l(pcontext->pm, &pfirst->s[UPWARD].u.sobject, 1);
  pfirst->s[UPWARD].u.sobject.pstr[0] = 't';
  MARKSET(pfirst->s[UPWARD].vtype, MARK_NON);

  *pret = pfirst->s[UPWARD];
  TYPESET(pfirst->s[UPWARD].vtype, TYPE_NIL);
  return;
}

int equality_next(struct inter *pcontext,
                  struct scell *pfirst, struct scell *pnext) {


  numbers_equalization(pcontext, pfirst, pnext);

  if (sm_cmp(&pcontext->strmh, &pfirst->u.sobject, &pnext->u.sobject) == 0) {

    return 0;
  }

  return 1;
}

void kw_equality(struct error_stack **pcnterr,
                 struct snode *phead, struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);


  compare_function(pcontext, phead, pret, equality_next);
  return;
}

int increase_next(struct inter *pcontext,
                  struct scell *pfirst, struct scell *pnext) {


  numbers_equalization(pcontext, pfirst, pnext);

  if (sm_cmp(&pcontext->strmh, &pfirst->u.sobject, &pnext->u.sobject) < 0) {

    return 0;
  }

  return 1;
}

void kw_increase(struct error_stack **pcnterr,
                 struct snode *phead, struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);


  compare_function(pcontext, phead, pret, increase_next);
  return;
}

int increase_oreq_next(struct inter *pcontext,
                       struct scell *pfirst, struct scell *pnext) {


  numbers_equalization(pcontext, pfirst, pnext);

  if (sm_cmp(&pcontext->strmh, &pfirst->u.sobject, &pnext->u.sobject) <= 0) {

    return 0;
  }

  return 1;
}

void kw_increase_oreq(struct error_stack **pcnterr,
                      struct snode *phead, struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);


  compare_function(pcontext, phead, pret, increase_oreq_next);
  return;
}

int decrease_next(struct inter *pcontext,
                  struct scell *pfirst, struct scell *pnext) {


  numbers_equalization(pcontext, pfirst, pnext);

  if (sm_cmp(&pcontext->strmh, &pfirst->u.sobject, &pnext->u.sobject) > 0) {

    return 0;
  }

  return 1;
}

void kw_decrease(struct error_stack **pcnterr,
                 struct snode *phead, struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);


  compare_function(pcontext, phead, pret, decrease_next);
  return;
}

int decrease_oreq_next(struct inter *pcontext,
                       struct scell *pfirst, struct scell *pnext) {


  numbers_equalization(pcontext, pfirst, pnext);

  if (sm_cmp(&pcontext->strmh, &pfirst->u.sobject, &pnext->u.sobject) >= 0) {

    return 0;
  }

  return 1;
}

void kw_decrease_oreq(struct error_stack **pcnterr,
                      struct snode *phead, struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);


  compare_function(pcontext, phead, pret, decrease_oreq_next);
  return;
}

/* ------------------------------------------------------------------------- */
