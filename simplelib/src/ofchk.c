/*---------------------------------------------------------------------------*/

#include "./ofchk.h"

static const OFCHK_TS gl_smax_s[] = {

  SCHAR_MAX,
  SHRT_MAX,
  INT_MAX,
  LONG_MAX,
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
  LLONG_MAX,
#endif
#ifdef INT8_MAX
  INT8_MAX,
#endif
#ifdef INT16_MAX
  INT16_MAX,
#endif
#ifdef INT32_MAX
  INT32_MAX,
#endif
#ifdef INT64_MAX
  INT64_MAX,
#endif
  INTPTR_MAX
};

static const OFCHK_TS gl_smin_s[] = {

  SCHAR_MIN,
  SHRT_MIN,
  INT_MIN,
  LONG_MIN,
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
  LLONG_MIN,
#endif
#ifdef INT8_MIN
  INT8_MIN,
#endif
#ifdef INT16_MIN
  INT16_MIN,
#endif
#ifdef INT32_MIN
  INT32_MIN,
#endif
#ifdef INT64_MIN
  INT64_MIN,
#endif
  INTPTR_MIN
};

static const OFCHK_TU gl_smax_u[] = {

  UCHAR_MAX,
  USHRT_MAX,
  UINT_MAX,
  ULONG_MAX,
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
  ULLONG_MAX,
#endif
#ifdef INT8_MAX
  UINT8_MAX,
#endif
#ifdef INT16_MAX
  UINT16_MAX,
#endif
#ifdef INT32_MAX
  UINT32_MAX,
#endif
#ifdef INT64_MAX
  UINT64_MAX,
#endif
  UINTPTR_MAX
};

/*---------------------------------------------------------------------------*/

int ofchk_add_s(OFCHK_TS va, OFCHK_TS vb, int vindex) {

  OFCHK_TS vmax = gl_smax_s[vindex], vmin = gl_smin_s[vindex];

  return (va >= 0 && vb > vmax - va) || (va < 0 && vb < vmin - va);
}

int ofchk_sub_s(OFCHK_TS va, OFCHK_TS vb, int vindex) {

  OFCHK_TS vmax = gl_smax_s[vindex], vmin = gl_smin_s[vindex];


#if   (INT_MIN + INT_MAX == -1) /* [-2^n, (2^n - 1)] */
  return  (va >= 0 && vb < -(vmax - va)) || (va < 0 && vb > -(vmin - va));
#elif (INT_MIN + INT_MAX == 0)  /* [-(2^n - 1), (2^n - 1)] */
  return  (va > 0 && vb < -(vmax - va)) || (va < 0 && vb > -(vmin - va));
#endif
}

int ofchk_mul_s(OFCHK_TS va, OFCHK_TS vb, int vindex) {

  OFCHK_TS vr, vmax = gl_smax_s[vindex], vmin = gl_smin_s[vindex];


  if (va == 0 || vb == 0) {

    return 0;
  }
#if   (INT_MIN + INT_MAX == -1) /* [-2^n, (2^n - 1)] */
  if (vb == -1) {

    return (va == vmin);
  }
#endif

  vr = (((va > 0) == (vb > 0))? vmax : vmin) / vb;

  return (va >= 0 && va > vr) || (va < 0 && va < vr);
}

int ofchk_div_s(OFCHK_TS va, OFCHK_TS vb, int vindex) {

  if (vb == 0) {

    return 1;
  }
#if   (INT_MIN + INT_MAX == -1) /* [-2^n, (2^n - 1)] */
  if (vb == -1) {

    return (va == gl_smin_s[vindex]);
  }
#else
  UNUSED(va), UNUSED(vindex);
#endif

  return 0;
}

int ofchk_add_u(OFCHK_TU va, OFCHK_TU vb, int vindex) {

  OFCHK_TU vt = va + vb;

  return vt < va || vt > gl_smax_u[vindex];
}

int ofchk_sub_u(OFCHK_TU va, OFCHK_TU vb, int vindex) {

  UNUSED(vindex);

  return (vb > va);
}

int ofchk_mul_u(OFCHK_TU va, OFCHK_TU vb, int vindex) {

  return (vb != 0) && ((OFCHK_TU)(va*vb) > gl_smax_u[vindex] || (OFCHK_TU)(va*vb) / vb != va);
}

int ofchk_div_u(OFCHK_TU va, OFCHK_TU vb, int vindex) {

  UNUSED(va), UNUSED(vindex);

  return (vb == 0);
}

/*---------------------------------------------------------------------------*/
