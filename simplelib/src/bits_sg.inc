/* ------------------------------------------------------------------------- *
 * шаблон функций для типов <D>                                              *
 *                                                                           *
 * bitset<X> bitget<X>                                                       *
 *                                                                           *
 * использующих                                                              *
 *                                                                           *
 * bitset<T>_lsf bitget<T>_lsf                                               *
 * bitset<T>_msf bitget<T>_msf                                               *
 * ------------------------------------------------------------------------- */

#define FNAME(A, X)   FNAME_(A, X)
#define FNAME_(A, X)  FNAME__(A, X)
#define FNAME__(A, X) A ## X

void FNAME(bitset, X)(bitptr_t sp, uint vmode, uint vbits, D vval) {

  intp vsig = ((vmode & BM_ORIENT)==BM_IO)?+1:-1;

  if ((vmode & BM_BORDER) == BM_LSF) {
    FNAME(FNAME(bitset, T), _lsf)(sp, vsig, vbits, vval);
  } else {
    FNAME(FNAME(bitset, T), _msf)(sp, vsig, vbits, vval);
  }

  return;
}

D FNAME(bitget, X)(bitptr_t sp, uint vmode, uint vbits) {

  intp vsig = ((vmode & BM_ORIENT)==BM_IO)?+1:-1;

  if ((vmode & BM_BORDER) == BM_LSF) {
    return FNAME(FNAME(bitget, T), _lsf)(sp, vsig, vbits);
  } else {
    return FNAME(FNAME(bitget, T), _msf)(sp, vsig, vbits);
  }
}

#undef D
#undef X
#undef T

#undef FNAME
#undef FNAME_
#undef FNAME__