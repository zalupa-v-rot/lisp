/* ------------------------------------------------------------------------- */

#include "./assert.h"
#include <stdlib.h>


void assert_(const char *pfile, int vline, const char *pexpr) {

  fprintf(stderr, "%s:%i: assertion `%s` failed.\n",
          pfile, vline, pexpr);
  exit(1);
}

void assert_info_(const char *pfile, int vline,
                  const char *pexpr, const char *pinfo) {

  fprintf(stderr, "%s:%i: assertion `%s` failed. info `%s`\n",
          pfile, vline, pexpr, pinfo);
  exit(1);
}

/* ------------------------------------------------------------------------- */
