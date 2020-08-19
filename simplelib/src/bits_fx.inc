/* ------------------------------------------------------------------------- *
 * шаблон функций для фикс. типов                                            *
 *                                                                           *
 * bitset<T>_lsf bitget<T>_lsf                                               *
 * bitset<T>_msf bitget<T>_msf                                               *
 * ------------------------------------------------------------------------- */

#define TYPE             TYPE_(uint, T)
#define TYPE_(TYPE, X)   TYPE__(TYPE, X)
#define TYPE__(TYPE, X)  TYPE ## X

#define FNAME(A, X)   FNAME_(A, X)
#define FNAME_(A, X)  FNAME__(A, X)
#define FNAME__(A, X) A ## X


void FNAME(FNAME(bitset, T), _lsf)(bitptr_t sp, intp vsig,
                                   uint vbits, TYPE vval) {

  uint vmask, vleft, vchunk;

  assert(vbits > 0 && vbits <= BITSOF(TYPE));


  for (vleft = vbits; ; ) {

    if ((vchunk = CHAR_BIT - sp.vbit) > vleft) {

      vchunk = vleft;
    }

    vmask = (1 << vchunk) - 1;

    *sp.pbyte &= ~(vmask << sp.vbit);
    *sp.pbyte |= (vval & vmask) << sp.vbit;

    if ((vleft -= vchunk) == 0) { break; }

    vval >>= vchunk;
    sp.pbyte += vsig, sp.vbit = 0;
  }

  return;
}

TYPE FNAME(FNAME(bitget, T), _lsf)(bitptr_t sp, intp vsig, uint vbits) {

  TYPE vval;
  uint vloaded;

  assert(vbits > 0 && vbits <= BITSOF(TYPE));


  for (vval = vloaded = 0; ; ) {

    vval |= (TYPE)(*sp.pbyte >> sp.vbit) << vloaded;

    if ((vloaded += CHAR_BIT - sp.vbit) >= vbits) { break; }

    sp.pbyte += vsig, sp.vbit = 0;
  }

  /* чистка от лишних старших разрядов */
  return vval & (TYPE)-1 >> BITSOF(TYPE)-vbits;
}

void FNAME(FNAME(bitset, T), _msf)(bitptr_t sp, intp vsig,
                                   uint vbits, TYPE vval) {

  uint vmask, vleft, vchunk, voff;

  assert(vbits > 0 && vbits <= BITSOF(TYPE));


  for (vleft = vbits, voff = 0; ; ) {

    if ((vchunk = sp.vbit + 1) > vleft) {

      voff = vchunk - vleft;
      vchunk = vleft;
    }

    vmask = (1 << vchunk) - 1;

    *sp.pbyte &= ~(vmask << voff);
    *sp.pbyte |= bitrevc(vval & vmask) >> BITSOF(char) - vchunk << voff;

    if ((vleft -= vchunk) == 0) { break; }

    vval >>= vchunk;
    sp.pbyte += vsig, sp.vbit = CHAR_BIT-1;
  }

  return;
}

TYPE FNAME(FNAME(bitget, T), _msf)(bitptr_t sp, intp vsig, uint vbits) {

  TYPE vval;
  uint vleft, vchunk, voff;

  assert(vbits > 0 && vbits <= BITSOF(TYPE));


  for (vval = voff = 0, vleft = vbits; ; ) {

    if ((vchunk = sp.vbit + 1) > vleft) {

      voff = vchunk - vleft;
      vchunk = vleft;
    }

    vval |= (TYPE)(*sp.pbyte >> voff) << (vleft -= vchunk);

    if (vleft == 0) { break; }

    sp.pbyte += vsig, sp.vbit = CHAR_BIT-1;
  }

  /* лишние 'старшие' разряды очистятся */
  return FNAME(bitrev, T)(vval) >> BITSOF(TYPE) - vbits;
}

#undef T

#undef TYPE
#undef TYPE_
#undef TYPE__

#undef FNAME
#undef FNAME_
#undef FNAME__

