/* ------------------------------------------------------------------------- */

#include "./string.h"
#include "./memory.h"

#include <string.h> /* memcmp(), memcpy() */

int string_eq(const struct string *pa, const struct string *pb) {

  if (pa->vlen == pb->vlen &&
      memcmp(pa->pstr, pb->pstr, pa->vlen) == 0) {

    return 1;
  }

  return 0;
}

#ifdef OPTION_NUMBERS
#ifdef OPTION_MATH

int string_isnumber(const struct string *pa,
                    uintp *ppointpos, uintp *pfirstnonz) {

  struct string sa = *pa;
  const char *ppoint = NULL, *pnonz = NULL;

  assert(sa.vlen != 0);


  if (*sa.pstr == '-') {

    sa.pstr++, sa.vlen--;
  }

  if (sa.vlen == 0) {

    return 0;
  }

  for ( ; ; sa.pstr++) {

    if (*sa.pstr == '.') {

      if (ppoint != NULL) {

        return 0;
      }

      ppoint = sa.pstr;

    } else {

      if (*sa.pstr < '0' || *sa.pstr > '9') {

        return 0;
      }

      if (pnonz == NULL && *sa.pstr != '0') {

        pnonz = sa.pstr;
      }
    }

    if (--sa.vlen == 0) {

      if (ppoint == NULL) {

        *ppointpos = pa->vlen - 1;

      } else {

        *ppointpos = ppoint - pa->pstr;
      }

      if (pnonz == NULL) {

        *pfirstnonz = pa->vlen - 1;

      } else {

        *pfirstnonz = pnonz - pa->pstr;
      }

      return 1;
    }
  }
}

#else

int string_isnumber(const struct string *pa) {

  struct string sa = *pa;

  assert(sa.vlen != 0);


  if (*sa.pstr == '-') {

    sa.pstr++, sa.vlen--;
  }

  if (sa.vlen == 0) {

    return 0;
  }

  for ( ; ; sa.pstr++) {

    if (*sa.pstr < '0' || *sa.pstr > '9') {

      return 0;
    }
    if (--sa.vlen == 0) {

      return 1;
    }
  }
}

#endif

#endif

char* string_to_cstr(struct memory *pm, const struct string *pobj) {

  char *pstr;


  pstr = memalloc(pm, pobj->vlen + 1);
  memcpy(pstr, pobj->pstr, pobj->vlen);
  *(pstr + pobj->vlen) = '\0';

  return pstr;
}

void string_from_uintp(struct memory *pm, struct string *pobj, uintp vnum) {

  uintp va, vb;


  for (va = vnum, vb = 0; ++vb, va /= 10; );

  stralloc(pm, pobj, vb);


  for ( ; vb; pobj->pstr[--vb] = '0' + vnum % 10, vnum /= 10);

  return;
}

#define T uintp
#define SL INTPTR_MAX
#define UL UINTPTR_MAX
#include "./string-str2int.inc"

void string_scan(const struct string *pstr, uintp *pstrp,
                 const struct string *pset, uintp *psetp) {

  uintp vstrp = *pstrp, vsetp;


  for ( ; vstrp <  pstr->vlen; vstrp++) {

    for (vsetp = 0; vsetp != pset->vlen != '\0'; vsetp++) {

      if (pstr->pstr[vstrp] == pset->pstr[vsetp]) {

        *pstrp = vstrp, *psetp = vsetp;
        return;
      }
    }
  }

  *pstrp = vstrp, *psetp = vsetp;
  return;
}

/* ------------------------------------------------------------------------- */
