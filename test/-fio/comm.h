/* ------------------------------------------------------------------------- */

#include <string.h>

#include "./lisp/fio.h"
#include "./lib/rand.h"

#include "./chekers.h"
#include "./string.h"
#include "./assert.h"

#define success() \
  printf("success\n"); \
  return 0

#define MAGIC     4133
#define BUFBIT1   (BUFSIZE1 * CHAR_BIT)
#define BUFSIZE1  (BUFSIZE0*21)
#define BUFSIZE0  1024

char    sbuf[BUFSIZE1],
        sbuf2[BUFSIZE1];
struct rand sr;

const char snull2[1];
#define NULL2 ((void*)(snull2))

#define CODES_COMB 32

int mode_code(int vi) {

  return!!(vi & 1)*FIOM_READ | !!(vi & 2)*FIOM_WRITE |
    !!(vi & 4)*FIOM_REXTEND | !!(vi & 8)*FIOM_WEXTEND |
    !!(vi & 16)*FIOM_CREATE;
}

#define STR2(X) # X
#define STR(X) STR2(X)
#define T STR(TMP_DIR)

/* ------------------------------------------------------------------------- */
