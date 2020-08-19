/*---------------------------------------------------------------------------*/

#include "./assert.h"

#include <stdio.h>
#include <stdlib.h>

void assert_(const char *pfile, int vline, const char *pexpr) {

  fprintf(stderr, "%s:%i: assert `%s` failed.\n",
          pfile, vline, pexpr);

  exit(EXIT_FAILURE);
}

void assert_ret_(const char *pfile, int vline, const char *pexpr) {

  fprintf(stderr, "%s:%i: assert_ret `%s` failed.\n",
          pfile, vline, pexpr);

  return;
}

/*---------------------------------------------------------------------------*/
