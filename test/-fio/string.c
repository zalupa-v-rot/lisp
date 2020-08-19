/* ------------------------------------------------------------------------- */

#include "./lisp/fio.h"

#include <stdarg.h>
#include <stdio.h>

#define RD "FIOM_READ"
#define WD "FIOM_WRITE"
#define RE "FIOM_REXTEND"
#define WE "FIOM_WEXTEND"
#define CR "FIOM_CREATE"

const char* gl_smode[32] = {
  /**/ "(none)",
  /**/ RD,
  /**/        WD,
  /**/ RD "|" WD,
  /**/               RE,
  /**/ RD        "|" RE,
  /**/        WD "|" RE,
  /**/ RD "|" WD "|" RE,
  /**/                      WE,
  /**/ RD               "|" WE,
  /**/        WD        "|" WE,
  /**/ RD "|" WD        "|" WE,
  /**/               RE "|" WE,
  /**/ RD        "|" RE "|" WE,
  /**/        WD "|" RE "|" WE,
  /**/ RD "|" WD "|" RE "|" WE "|" CR,
  /**/ "(none)"                "|" CR,
  /**/ RD                      "|" CR,
  /**/        WD               "|" CR,
  /**/ RD "|" WD               "|" CR,
  /**/               RE        "|" CR,
  /**/ RD        "|" RE        "|" CR,
  /**/        WD "|" RE        "|" CR,
  /**/ RD "|" WD "|" RE        "|" CR,
  /**/                      WE "|" CR,
  /**/ RD               "|" WE "|" CR,
  /**/        WD        "|" WE "|" CR,
  /**/ RD "|" WD        "|" WE "|" CR,
  /**/               RE "|" WE "|" CR,
  /**/ RD        "|" RE "|" WE "|" CR,
  /**/        WD "|" RE "|" WE "|" CR,
  /**/ RD "|" WD "|" RE "|" WE "|" CR,
};

const char* strmode(int vmode) {

  return gl_smode[!!(vmode & FIOM_READ) | !!(vmode & FIOM_WRITE)*2 |
                  !!(vmode & FIOM_REXTEND)*4 | !!(vmode & FIOM_WEXTEND)*8
                  | !!(vmode & FIOM_CREATE)*16];
}

const char* gl_sbmode[4] = {

  "BM_IO|BM_LSF", "BM_DO|BM_LSF", "BM_IO|BM_MSF", "BM_DO|BM_MSF"
};

const char* strbmode(int vmode) {

  return gl_sbmode[((vmode & BM_ORIENT)==BM_DO) | ((vmode & BM_BORDER)==BM_MSF)*2];
}

char gl_sbuf[1024];

const char* sprintf_b(const char *pformat, ...) {

  va_list args;

  va_start(args, pformat);

  vsprintf(gl_sbuf, pformat, args);

  va_end(args);

  return gl_sbuf;
}

void strmagic(char *pstr, uintp vsize) {

  uchar vb, vc;

  for (vc = 1; ; vc++) {
    for (vb = 0; vb != vc; vb++) {

      if (vsize == 0) { return; }; vsize--;
      *pstr++ = vc;
    }
  }
}

/* ------------------------------------------------------------------------- */
