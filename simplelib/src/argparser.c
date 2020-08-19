/*----------------------------------------------------------------------------*/

#define ARGPARSER_C
#include "./argparser.h"

#include <stdio.h>
#include <string.h>
#include "./assert.h"

/*----------------------------------------------------------------------------*/

#define SUCCESS()                               \
  do {                                          \
    if (perr != NULL) {                         \
      perr->vargi = 0;                          \
      perr->vpos = 0;                           \
      perr->vcod = ARGPARSER_NOT_A_ERROR;       \
    }                                           \
    return 0;                                   \
  } while(0)

#define ERRRET(ARG, POS, CODE)                  \
  do {                                          \
    if (perr != NULL) {                         \
      perr->vargi = (ARG);                      \
      perr->vpos = (POS);                       \
      perr->vcod = (CODE);                      \
    }                                           \
    return 1;                                   \
  } while(0)


static_assert(sizeof(void*) == sizeof(uintp), "");

/*----------------------------------------------------------------------------*/

#include <inttypes.h>

#define PERR(CODE) \
  case CODE: fprintf(stderr, "ERROR: %s, argi: %i, pos: %"PRIuPTR"\n", # CODE, perr->vargi, perr->vpos); break

#define PDEF(CODE) \
  default: fprintf(stderr, "ERROR: %s, argi: %i, pos: %"PRIuPTR"\n", # CODE, perr->vargi, perr->vpos); break

void argparser_printerr(struct argerror *perr) {

  if (perr == NULL) { return; }

  switch(perr->vcod) {
    PERR(ARGPARSER_UNEXPECTED_SYMBOL);
    PERR(ARGPARSER_UINTP_OVERFLOW);
    PERR(ARGPARSER_UNRECOGNIZED_OPTION);
    PERR(ARGPARSER_UNEXPECTED_END);
    PERR(ARGPARSER_ZEROLENGTH_ARGUMENT);
    PERR(ARGPARSER_EMPTY_ENUM);
    PERR(ARGPARSER_WRONG_ENUM_KEYWORD);
    PERR(ARGPARSER_INCORRECT_PAIR);
    PERR(ARGPARSER_DUPLICATED_OPTION);

    PDEF(ARGPARSER_UNDEFINED);
  }

  return;
}

#undef PERR
#undef PDEF

/*----------------------------------------------------------------------------*/

struct init_uintp {

  uintp   *pout;
};

int arg_init_uintp(const struct init_uintp *popt, void *pbase,
                   int *pargi, int vargc, char **pargv,
                   struct argerror *perr) {

  uintp vp, vn, va;
  int   vargi;

  vargi = *pargi + 1;

  if (vargi == vargc || (pargv[vargi])[0] == '\0') {

    ERRRET(*pargi, 0, ARGPARSER_UNEXPECTED_END);
  }

  for (vp = vn = 0; ; ) {

    if ((uchar)(pargv[vargi])[vp] > '9' || (uchar)(pargv[vargi])[vp] < '0') {

      ERRRET(vargi, vp, ARGPARSER_UNEXPECTED_SYMBOL);
    }

    va = vn * 10 + (pargv[vargi])[vp] - '0';

    if (va / 10 != vn) {

      ERRRET(vargi, vp, ARGPARSER_UINTP_OVERFLOW);
    }

    vn = va;

    if ((pargv[vargi])[++vp] == '\0') {

      *(uintp*)((char*)pbase+(uintp)popt->pout) = vn;
      *pargi = vargi + 1;
      return 0;
    }
  }
}

struct flag_uintp {

  uintp   *pout, vmask, vflag;
};

int arg_flag_uintp(const struct flag_uintp *popt, void *pbase,
                   int *pargi, int vargc, char **pargv,
                   struct argerror *perr) {

  UNUSED(vargc);
  UNUSED(pargv);
  UNUSED(perr);

  *(uintp*)((char*)pbase+(uintp)popt->pout) &= ~popt->vmask;
  *(uintp*)((char*)pbase+(uintp)popt->pout) |= popt->vflag;
  *pargi += 1;
  return 0;
}


struct string {

  char   **ppout;
};

int arg_string(const struct string *popt, void *pbase,
               int *pargi, int vargc, char **pargv,
               struct argerror *perr) {

  int   vargi;

  vargi = *pargi + 1;

  if (vargi == vargc || (pargv[vargi])[0] == '\0') {

    ERRRET(*pargi, 0, ARGPARSER_UNEXPECTED_END);
  }

  *(char**)((char*)pbase+(uintp)popt->ppout) = pargv[vargi];
  *pargi = vargi + 1;
  return 0;
}



struct enum_uintp {

  uintp  *pout;
  uintp   vn; /* !!! */
  const uchar **ps;
};

int arg_enum_uintp(const struct enum_uintp *popt, void *pbase,
                   int *pargi, int vargc, char **pargv,
                   struct argerror *perr) {

  uintp varglen, venamelen, venum;
  int   vargi;


  vargi = *pargi + 1;

  if (vargi == vargc || (pargv[vargi])[0] == '\0') {

    ERRRET(*pargi, 0, ARGPARSER_UNEXPECTED_END);
  }

  if (popt->vn == 0) {

    ERRRET(*pargi, 0, ARGPARSER_EMPTY_ENUM);
  }

  for (varglen = 0; (pargv[vargi])[++varglen] != '\0'; );


  for (venum = 0; ; ) {


    for (venamelen = 0; (popt->ps[venum])[venamelen] != '\0'; venamelen++);

    if (venamelen == 0) {

      ERRRET(*pargi, 0, ARGPARSER_ZEROLENGTH_ARGUMENT);
    }

    if (varglen == venamelen &&
        memcmp(pargv[vargi], popt->ps[venum], varglen) == 0) {

      *(uintp*)((char*)pbase+(uintp)popt->pout) = venum;
      break;
    }

    if (++venum == popt->vn) {

      ERRRET(*pargi, 0, ARGPARSER_WRONG_ENUM_KEYWORD);
    }
  }

  *pargi = vargi + 1;
  return 0;
}



struct init_pair_uintp {

  uintp   *poutx,
          *pouty;
};

int arg_init_pair_uintp(const struct init_pair_uintp *popt, void *pbase,
                        int *pargi, int vargc, char **pargv,
                        struct argerror *perr) {

  uintp vp, vn, va;
  int   vargi;

  vargi = *pargi + 1;

  if (vargi == vargc || (pargv[vargi])[0] == '\0') {

    ERRRET(*pargi, 0, ARGPARSER_UNEXPECTED_END);
  }

  for (vp = vn = 0; ; vp++) {

    if ((uchar)(pargv[vargi])[vp] > '9' || (uchar)(pargv[vargi])[vp] < '0') {

      break;
    }

    va = vn * 10 + (pargv[vargi])[vp] - '0';

    if (va / 10 != vn) {

      ERRRET(vargi, vp, ARGPARSER_UINTP_OVERFLOW);
    }

    vn = va;
  }

  if (vp == 0 || (pargv[vargi])[vp] == '\0') {

    ERRRET(vargi, vp, ARGPARSER_INCORRECT_PAIR);
  }

  *(uintp*)((char*)pbase+(uintp)popt->poutx) = vn;

  for (vp++, vn = 0; ;) {

    if ((uchar)(pargv[vargi])[vp] > '9' || (uchar)(pargv[vargi])[vp] < '0') {

      ERRRET(vargi, vp, ARGPARSER_UNEXPECTED_END);
    }

    va = vn * 10 + (pargv[vargi])[vp] - '0';

    if (va / 10 != vn) {

      ERRRET(vargi, vp, ARGPARSER_UINTP_OVERFLOW);
    }

    vn = va;

    if ((pargv[vargi])[++vp] == '\0') {

      *(uintp*)((char*)pbase+(uintp)popt->pouty) = vn;
      *pargi = vargi + 1;
      return 0;
    }
  }
}

/*----------------------------------------------------------------------------*/

int argparser(uint vopt, const struct option *popt, void *pbase,
              int *pargi, int vargc, char **pargv,
              struct argerror *perr) {

  uintp vlen, vopti;


  for ( ; *pargi < vargc; ) {

    for (vlen = 0; (pargv[*pargi])[vlen] != '\0'; vlen++);

    for (vopti = 0; ; vopti++) {

      if (vopti == vopt) {

        ERRRET(*pargi, 0, ARGPARSER_UNRECOGNIZED_OPTION);
      }

      if (vlen == popt[vopti].snme.v &&
          memcmp(pargv[*pargi], popt[vopti].snme.p, vlen) == 0) {

        if (popt[vopti].pfun((void* const *)&popt[vopti].u, pbase, pargi, vargc, pargv, perr)) {

          return 1;
        }

        break;
      }
    }
  }

  SUCCESS();
}

static int argparser_finddub(const struct option *popt,
                             int vargi, int vargc, char **pargv) {

  uintp vlen;

  for ( ; vargi < vargc; vargi++) {

    for (vlen = 0; (pargv[vargi])[vlen] != '\0'; vlen++);

    if (vlen == popt->snme.v &&
        memcmp(pargv[vargi], popt->snme.p, vlen) == 0) {

      return 1;
    }
  }

  return 0;
}

static const struct option*
        argparser_findopt(uint vopt, const struct option *popt,
                          char *pargv) {

  uintp vlen, vi;

  for (vlen = 0; pargv[vlen] != '\0'; vlen++);

  for (vi = 0; vi != vopt; vi++) {

    if (vlen == popt[vi].snme.v &&
        memcmp(pargv, popt[vi].snme.p, vlen) == 0) {

      return &popt[vi];
    }
  }

  return NULL;
}

int argparser_nd(uint vopt, const struct option *popt, void *pbase,
                 int *pargi, int vargc, char **pargv,
                 struct argerror *perr) {

  int vargsi_s = *pargi;
  const struct option *pcur;

  for ( ; *pargi < vargc; ) {

    if ((pcur = argparser_findopt(vopt, popt, pargv[*pargi])) == NULL) {

      ERRRET(*pargi, 0, ARGPARSER_UNRECOGNIZED_OPTION);
    }

    if (argparser_finddub(pcur, vargsi_s, *pargi, pargv)) {

      ERRRET(*pargi, 0, ARGPARSER_DUPLICATED_OPTION);
    }

    if (pcur->pfun((void* const *)&pcur->u, pbase, pargi, vargc, pargv, perr)) {

      return 1;
    }
  }

  SUCCESS();
}

/*----------------------------------------------------------------------------*/

#undef ERRRET

/*----------------------------------------------------------------------------*/
