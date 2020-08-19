/* ------------------------------------------------------------------------- */

#include "./lisp/lib/assert.h"
#include "./lisp/strmath.h"

#define TRUE  EXIT_SUCCESS
#define FALSE EXIT_FAILURE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------------- */

struct memstat {

  uintp   vallocated_chunks,
          vallocated_memory;
};

void memstat_init(struct memstat *pm);
int memstat_isclear(struct memstat *pm);

int some_stralloc(void *ptr, struct string *ps, uintp vsize);
int some_strrealloc_l(void *ptr, struct string *ps, uintp vnewlen);
int some_strrealloc_r(void *ptr, struct string *ps, uintp vnewlen);
void some_strfree(void *ptr, struct string *p);

int make_string(void *ptr, struct string *p, const char *pstr);

/* ------------------------------------------------------------------------- */

struct test_head {

  struct memstat smh;
  struct sm_head strmh;
};

void test_init_(struct test_head *ph);

int test_sum_(struct test_head *ph, const char *psa, const char *psb,
              const char *psc, int (*sm_x)(struct sm_head*,
                                           struct string*,
                                           const struct string*));

int test_mul_(struct test_head *ph, const char *psa, const char *psb,
              const char *psc);

int test_div_(struct test_head *ph, const char *psa, const char *psb,
              const char *psc, const char *psd);

int test_cmp_(struct test_head *ph, const char *psa, const char *psb,
              schar vc);

int test_not_(struct test_head *ph, const char *psa, const char *psb);

int test_sqrt_(struct test_head *ph, const char *psin, const char *psret,
               const char *psrem);

/* ------------------------------------------------------------------------- */
