/* ------------------------------------------------------------------------- */

#include "./error.h"

#include <string.h> /* strcmp() */

/* ------------------------------------------------------------------------- */

#ifndef NDEBUG

void error_init(struct error_stack *pstack) {

  assert(pstack != NULL);


  pstack->ptop = NULL;
  return;
}

#endif

#ifndef NDEBUG

void error_set(struct error_stack *pstack,
               struct error_node *pnew, const char *psrcfile) {

  assert(pstack != NULL);
  assert(pnew != NULL);


  pnew->ptodown = pstack->ptop;
  pstack->ptop = pnew;

  pnew->psrcfile = psrcfile;

  return;
}

void error_unset(struct error_stack *pstack,
                 const char *psrcfile) {

  struct error_node *pcur;

  assert(pstack != NULL);
  assert(pstack->ptop != NULL);


  pcur = pstack->ptop;
  pstack->ptop = pstack->ptop->ptodown;

  assert(strcmp(pcur->psrcfile, psrcfile) == 0);
  return;
}

#else

void error_set(struct error_stack *pstack, struct error_node *pnew) {


  pnew->ptodown = pstack->ptop;
  pstack->ptop = pnew;
  return;
}

void error_unset(struct error_stack *pstack) {

  assert(pstack != NULL);
  assert(pstack->ptop != NULL);


  pstack->ptop = pstack->ptop->ptodown;
  return;
}

#endif

void error_silent(struct error_stack *pstack) {

  struct error_node *pcur;

  assert(pstack != NULL);
  assert(pstack->ptop != NULL);


  pcur = pstack->ptop;
  pstack->ptop = pstack->ptop->ptodown;
  longjmp(pcur->sjmp, 1);
  return;
}

void error_trow(struct error_stack *pstack,
                uint vcode, const char *psrcfile, uint vsrcline) {

  struct error_node *pcur;

  assert(pstack != NULL);
  assert(pstack->ptop != NULL);


  pstack->vcode = vcode;
  pstack->psrcfile = psrcfile;
  pstack->vsrcline = vsrcline;

  pcur = pstack->ptop;
  pstack->ptop = pstack->ptop->ptodown;
  longjmp(pcur->sjmp, 1);
  return;
}

/* ------------------------------------------------------------------------- */

uint error_code(struct error_stack *pstack) {

  assert(pstack != NULL);


  return pstack->vcode;
}

#ifndef NDEBUG
uint error_srcline(struct error_stack *pstack) {

  assert(pstack != NULL);


  return pstack->vsrcline;
}

const char* error_srcfile(struct error_stack *pstack) {

  assert(pstack != NULL);


  return pstack->psrcfile;
}
#endif

#define PERR(CODE)                              \
  case CODE: return # CODE; break
#define PDEF(CODE)                              \
  default: return # CODE; break

const char* error_str(struct error_stack *pstack) {

#ifdef OPTION_EXTERN_ERRORS
    if (pstack->vcode >= EXTERN_ERRORS_SPACE) {

      return extern_error_str(pstack->vcode);
    }
#endif

#ifndef NDEBUG
  if (pstack->vcode < STANDARD_ERRORS_SPACE) {

    assert(0);
    /* switch(pstack->vcode) { */
    /*   PDEF(NOT_A_ERROR) */
    /* } */
  }
#endif

  switch(pstack->vcode) {

    PERR(NOT_A_FUNCTION);
    PERR(UNDEFINED_FUNCTION);
    PERR(ARGUMENT_TYPE_NOT_A_LIST);
    PERR(UNDEFINED_VARIABLE);
    PERR(WRONG_NUMBER_OF_ARGUMENTS);
    PERR(WRONG_ARGUMENT);
    PERR(INCORRECT_COMPLETION);

    PERR(UNEXPECTED_END);
    PERR(UNEXPECTED_CLOSING_BRACKET);
    PERR(UNEXPECTED_DOT);
    PERR(CLOSING_BRACKET_EXPECTED);
    PERR(EMPTY_LIST);
    PERR(OPERNING_BRACKET_EXPECTED);

    PERR(OUT_OF_MEMORY);

    PERR(FAILED_TO_OPEN_FILE);

    PERR(TRACE);

#ifdef OPTION_MATH
    PERR(DIVISION_BY_ZERO);
#endif

    PDEF(UNDEFINED_ERROR);
  }
}

#undef PERR
#undef PDEF

/* ------------------------------------------------------------------------- */
