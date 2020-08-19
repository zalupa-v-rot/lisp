/* ------------------------------------------------------------------------- */

#include "./lisp/lisp.h"
#include "./lisp/memory_std.h"
#include "./lisp/error.h"

struct head {

  struct inter scontext;
  struct memory_std smemory;
  struct error_stack serr;

  intp    vsexp,
          vstr,
          vstrtotal,
          vfunc,
          vvar;
};

#define TRUE  0
#define FALSE 1

#define intern_assert(EXP)                          \
                                                    \
  do {                                              \
    if (!(EXP)) {                                   \
      printf("intern_assert: line %i\n", __LINE__); \
      return FALSE;                                 \
    }                                               \
  } while(0)


void init_(struct head *ph);

void free_(struct head *ph);

void memory_chusage_(struct head *ph,
                     intp vsexp,
                     intp vstr, intp vstrtotal, intp vfunc, intp vvar);

void memory_clusage_(struct head *ph);

int enterline(struct head *ph,
              const char *pinput,
              const char *panswers,
              const char *pfilenames, uintp vtrees, int vresult);

/* ------------------------------------------------------------------------- */
