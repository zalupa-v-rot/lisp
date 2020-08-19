/* ------------------------------------------------------------------------- */

#include "./inter.h"


#ifdef OPTION_MATH
#define LISP_FUNCTIONS (11+5+5)
#else
#define LISP_FUNCTIONS 11
#endif


extern const struct decfunction
        slispfun[LISP_FUNCTIONS];

int lisp_init(struct inter *pcontext,
              struct memory *pm, struct error_stack *perr);

#define lisp_free(PH) inter_free(PH)

int lisp_enter_str(struct inter *pcontext, const char *pstring);

#ifdef OPTION_HOOK_INTER_RET
#define lisp_sethook_inter_ret(PH, PTR, CALLBACK) \
  inter_sethook_inter_ret((PH), (PTR), (CALLBACK))
#endif

#define lisp_declarefunction(PH, PDECFUN, VFUNN)  \
  inter_declarefunction((PH), (PDECFUN), (VFUNN))

/* ------------------------------------------------------------------------- */
