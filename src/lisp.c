/* ------------------------------------------------------------------------- */

#include "./lisp.h"
#include "./keywords.h"
#include "./load.h"

#include <string.h> /* strlen() */
#include <stdio.h>

/* ------------------------------------------------------------------------- */

const struct decfunction
        slispfun[LISP_FUNCTIONS] =
  { { kw_list, "list" }, { kw_append, "append" }, { kw_car, "car" },
    { kw_cdr, "cdr" }, { kw_quote, "quote" }, { kw_cond, "cond" },
    { kw_defun, "defun" }, { kw_eval, "eval" }, { kw_set, "set" },
    { kw_let, "let" }, { kw_load, "load" },
#ifdef OPTION_MATH
    { kw_addition, "+" }, { kw_subtraction, "-" }, { kw_multiplication, "*" },
    { kw_division, "/" }, { kw_divremainder, "%" },

    { kw_equality, "=" }, { kw_increase, "<" }, { kw_increase_oreq, "<=" },
    { kw_decrease, ">" }, { kw_decrease_oreq, ">=" }
#endif
  };

int lisp_init(struct inter *pcontext,
              struct memory *pm, struct error_stack *perr) {


  if (inter_init(pcontext, pm, perr) ||
      inter_declarefunction(pcontext, slispfun, LISP_FUNCTIONS)) {

#ifndef NDEBUG
    fprintf(stderr, "interpreter init error: %i \"%s\" (source file:%s:%i:)\n",
            error_code(perr), error_str(perr),
            error_srcfile(perr), error_srcline(perr));
#else
    fprintf(stderr, "interpreter init error: %i \"%s\"\n",
            error_code(perr), error_str(perr));
#endif

    return 1;
  }

  if (lisp_enter_str(pcontext, "(set (quote nil) ())") ||
      lisp_enter_str(pcontext, "(set (quote t) (quote t))")) {

    return 1;
  }

  return 0;
}

int lisp_enter_str(struct inter *pcontext, const char *pstring) {

  return load_data(pcontext, pstring, strlen(pstring), "");
}

/* ------------------------------------------------------------------------- */
