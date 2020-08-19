/* ------------------------------------------------------------------------- */

#include "./bits.h"
#include "./assert.h"
#ifndef NDEBUG
#include "./ofchk.h"
#endif

bitptr_t bitptr(void *pbyte, uint vmode) {

  bitptr_t sp;

  sp.pbyte = pbyte;
  sp.vbit = 0;

  if ((vmode & BM_BORDER) == BM_MSF) {

    sp.vbit = CHAR_BIT-1;
  }

  return sp;
}

void* bitptr_getbyte(bitptr_t sp) {

  return sp.pbyte;
}

uchar bitptr_getbit(bitptr_t sp) {

  return sp.vbit;
}

bitptr_t bitptr_floor(bitptr_t sp, uint vmode) {


  if ((vmode & BM_BORDER) == BM_LSF) {

    sp.vbit = 0;

  } else {

    sp.vbit = CHAR_BIT-1;
  }

  return sp;
}

bitptr_t bitptr_ceil(bitptr_t sp, uint vmode) {

  intp vbysig = ((vmode & BM_ORIENT)==BM_IO)?+1:-1;


  if ((vmode & BM_BORDER) == BM_LSF) {

    if (sp.vbit) { sp.pbyte += vbysig; }
    sp.vbit = 0;

  } else {

    if (sp.vbit < CHAR_BIT-1) { sp.pbyte += vbysig; }
    sp.vbit = CHAR_BIT-1;
  }

  return sp;
}

bitptr_t bitptr_add(bitptr_t sp, uint vmode, intp vbits) {

  intp vbysig = ((vmode & BM_ORIENT)==BM_IO)?+1:-1,
    vbisig = ((vmode & BM_BORDER)==BM_LSF)?+1:-1;


  sp.pbyte += vbits / CHAR_BIT * vbysig;
  sp.vbit += vbits % CHAR_BIT * vbisig;

  if (sp.vbit >= CHAR_BIT) { sp.vbit -= CHAR_BIT, sp.pbyte += vbysig * vbisig;
  } else if (sp.vbit < 0) {  sp.vbit += CHAR_BIT, sp.pbyte -= vbysig * vbisig;
  }

  return sp;
}

bitptr_t bitptr_sub(bitptr_t sp, uint vmode, intp vbits) {

  assert(!ofchk_mul_intp(vbits, -1));


  return bitptr_add(sp, vmode, -vbits);
}

intp bitptr_diff(bitptr_t sa, bitptr_t sb, uint vmode) {

  intp  vbytediff = sa.pbyte - sb.pbyte,
    vbitdiff = sa.vbit - sb.vbit, vsig;


  if ((vmode & BM_ORIENT) == BM_DO) {

    assert(!ofchk_mul_intp(vbytediff, -1));
    vbytediff = -vbytediff;
  }
  if ((vmode & BM_BORDER) == BM_MSF) {

    vbitdiff = -vbitdiff;
  }

  /* чтобы остаться в пределах INTPTR_MIN ... INTPTR_MAX */
  vsig = (vbytediff < 0)?-1:+1;
  vbytediff -= vsig;

  assert(!ofchk_mul_intp(vbytediff, CHAR_BIT));
  assert(!ofchk_add_intp(vbytediff * CHAR_BIT, vbitdiff));
  assert(!ofchk_add_intp(vbytediff * CHAR_BIT + vbitdiff, CHAR_BIT*vsig));

  return vbytediff * CHAR_BIT + vbitdiff + CHAR_BIT*vsig;
}

/* ------------------------------------------------------------------------- */

void bitmemcpy(bitptr_t sdst, uint vmode_dst,
               bitptr_t ssrc, uint vmode_src, uintp vbits) {

  uint vchunk;

  for (vchunk = 0; vbits; ) {

    if ((vchunk = BITSOF(int)) > vbits) {

      vchunk = vbits;
    }

    bitseti(sdst, vmode_dst, vchunk, bitgeti(ssrc, vmode_src, vchunk));
    sdst = bitptr_add(sdst, vmode_dst, vchunk);
    ssrc = bitptr_add(ssrc, vmode_src, vchunk);
    vbits -= vchunk;
  }

  return;
}

void bitmemset(bitptr_t sdst, uint vmode_dst, uint val, uintp vbits) {

  uint vchunk;

  for (vchunk = 0; vbits; ) {

    if ((vchunk = BITSOF(int)) > vbits) {

      vchunk = vbits;
    }

    bitseti(sdst, vmode_dst, vchunk, val);
    sdst = bitptr_add(sdst, vmode_dst, vchunk);
    vbits -= vchunk;
  }

  return;
}

int bitmemcmp(bitptr_t sa, uint vmode_a,
              bitptr_t sb, uint vmode_b, uintp vbits) {

  uint vchunk, va, vb;

  for (vchunk = 0; vbits; ) {

    if ((vchunk = BITSOF(int)) > vbits) {

      vchunk = vbits;
    }

    va = bitgeti(sa, vmode_a, vchunk), vb = bitgeti(sb, vmode_b, vchunk);

    if (va < vb) { return -1; }
    if (va > vb) { return +1; }

    sa = bitptr_add(sa, vmode_a, vchunk), sb = bitptr_add(sb, vmode_b, vchunk);
    vbits -= vchunk;
  }

  return 0;
}

static void bitmemshiftl_shf(bitptr_t sa, uint vmode, uintp vshift,
                             uintp vbits) {

  uintp vchunk, vfield; bitptr_t se;

  assert(vshift < vbits);


  vfield = vbits - vshift;
  vchunk = vfield % BITSOF(int);
  if ((vchunk = vfield % BITSOF(int)) == 0) {

    vchunk = BITSOF(int);
  }

  se = bitptr_add(sa, vmode, vbits);
  sa = bitptr_add(sa, vmode, vfield);


  for ( ; ; vchunk = BITSOF(int)) {

    se = bitptr_sub(se, vmode, vchunk);
    sa = bitptr_sub(sa, vmode, vchunk);
    bitseti(se, vmode, vchunk, bitgeti(sa, vmode, vchunk));

    if ((vfield -= vchunk) == 0) { break; }
  }

  return;
}

void bitmemshiftl(bitptr_t sa, uint vmode, uintp vshift, uintp vbits) {

  assert(vbits <= INTPTR_MAX);

  if (!vshift) {

    return;
  }

  if (vshift < vbits) {

    bitmemshiftl_shf(sa, vmode, vshift, vbits);

  } else {

    vshift = vbits;
  }

  bitmemset(sa, vmode, 0, vshift);
  return;
}

static void bitmemshiftr_shf(bitptr_t sa, uint vmode, uintp vshift,
                             uintp vbits) {

  uintp vchunk, vfield; bitptr_t se;

  assert(vshift < vbits);


  vfield = vbits - vshift;
  vchunk = vfield % BITSOF(int);
  if ((vchunk = vfield % BITSOF(int)) == 0) {

    vchunk = BITSOF(int);
  }

  se = sa;
  sa = bitptr_add(sa, vmode, vbits-vfield);


  for ( ; ; vchunk = BITSOF(int)) {

    bitseti(se, vmode, vchunk, bitgeti(sa, vmode, vchunk));
    se = bitptr_add(se, vmode, vchunk);
    sa = bitptr_add(sa, vmode, vchunk);

    if ((vfield -= vchunk) == 0) { break; }
  }

  return;
}

void bitmemshiftr(bitptr_t sa, uint vmode, uintp vshift, uintp vbits) {

  assert(vbits <= INTPTR_MAX);

  if (!vshift) {

    return;
  }

  if (vshift < vbits) {

    bitmemshiftr_shf(sa, vmode, vshift, vbits);

  } else {

    vshift = vbits;
  }

  bitmemset(bitptr_add(sa, vmode, vbits-vshift), vmode, 0, vshift);
  return;
}

void bitmemrev(bitptr_t sa, uint vmode, uintp vbits) {

  bitptr_t se; uint va, vb;


  se = bitptr_add(sa, vmode, vbits);

  for ( ; ; ) {

    if (vbits >= BITSOF(int)*2) {

      se = bitptr_sub(se, vmode, BITSOF(int));

      va = bitrev(bitget(se, vmode, BITSOF(int)));
      vb = bitrev(bitget(sa, vmode, BITSOF(int)));

      bitset(se, vmode, BITSOF(int), vb);
      bitset(sa, vmode, BITSOF(int), va);

      vbits -= BITSOF(int)*2;
      sa = bitptr_add(sa, vmode, BITSOF(int));

    } else {

      if ((vbits /= 2) == 0) { return; }

      se = bitptr_sub(se, vmode, vbits);

      va = bitrev(bitget(se, vmode, vbits)) >> BITSOF(int) - vbits;
      vb = bitrev(bitget(sa, vmode, vbits)) >> BITSOF(int) - vbits;

      bitset(se, vmode, vbits, vb);
      bitset(sa, vmode, vbits, va);

      return;
    }
  }
}

/* ------------------------------------------------------------------------- */

#undef bitset
#undef bitget
#undef bitrev

#if (defined(INT64_MAX))
#define T 64
#include "./bits_fx.inc"
#endif

#if (defined(INT32_MAX))
#define T 32
#include "./bits_fx.inc"
#endif

#if (defined(INT16_MAX))
#define T 16
#include "./bits_fx.inc"
#endif


#define D uint
#define X i
#if   (defined(INT16_MAX) && INT_MAX == INT16_MAX)
#define T 16
#elif (defined(INT32_MAX) && INT_MAX == INT32_MAX)
#define T 32
#elif (defined(INT64_MAX) && INT_MAX == INT64_MAX)
#define T 64
#else
#error
#endif
#include "./bits_sg.inc"

#define D uintp
#define X p
#if   (defined(INT16_MAX) && INTPTR_MAX == INT16_MAX)
#define T 16
#elif (defined(INT32_MAX) && INTPTR_MAX == INT32_MAX)
#define T 32
#elif (defined(INT64_MAX) && INTPTR_MAX == INT64_MAX)
#define T 64
#else
#error
#endif
#include "./bits_sg.inc"

static_assert(sizeof(intp) == sizeof(uintp), "");

#define D ulong
#define X l
#if   (defined(INT32_MAX) && LONG_MAX == INT32_MAX)
#define T 32
#elif (defined(INT64_MAX) && LONG_MAX == INT64_MAX)
#define T 64
#else
#error
#endif
#include "./bits_sg.inc"

/* ------------------------------------------------------------------------- */

#ifdef UINT8_MAX
uint8 bitrev8(uint8 vval) {

  vval = (vval & 0xF0U) >> 4 | (vval & 0x0FU) << 4;
  vval = (vval & 0xCCU) >> 2 | (vval & 0x33U) << 2;
  vval = (vval & 0xAAU) >> 1 | (vval & 0x55U) << 1;
  return vval;
}
#endif

#ifdef UINT16_MAX
uint16 bitrev16(uint16 vval) {

  vval = (vval & 0xFF00U) >> 8  | (vval & 0x00FFU) << 8;
  vval = (vval & 0xF0F0U) >> 4  | (vval & 0x0F0FU) << 4;
  vval = (vval & 0xCCCCU) >> 2  | (vval & 0x3333U) << 2;
  vval = (vval & 0xAAAAU) >> 1  | (vval & 0x5555U) << 1;
  return vval;
}
#endif

#ifdef UINT32_MAX
uint32 bitrev32(uint32 vval) {

  vval = (vval & 0xFFFF0000UL) >> 16 | (vval & 0x0000FFFFUL) << 16;
  vval = (vval & 0xFF00FF00UL) >> 8  | (vval & 0x00FF00FFUL) << 8;
  vval = (vval & 0xF0F0F0F0UL) >> 4  | (vval & 0x0F0F0F0FUL) << 4;
  vval = (vval & 0xCCCCCCCCUL) >> 2  | (vval & 0x33333333UL) << 2;
  vval = (vval & 0xAAAAAAAAUL) >> 1  | (vval & 0x55555555UL) << 1;
  return vval;
}
#endif

#ifdef UINT64_MAX
uint64 bitrev64(uint64 vval) {

  vval = (vval & (uint64)0xFFFFFFFF00000000) >> 32
    | (vval & (uint64)0x00000000FFFFFFFF) << 32;
  vval = (vval & (uint64)0xFFFF0000FFFF0000) >> 16
    | (vval & (uint64)0x0000FFFF0000FFFF) << 16;
  vval = (vval & (uint64)0xFF00FF00FF00FF00) >> 8
    | (vval & (uint64)0x00FF00FF00FF00FF) << 8;
  vval = (vval & (uint64)0xF0F0F0F0F0F0F0F0) >> 4
    | (vval & (uint64)0x0F0F0F0F0F0F0F0F) << 4;
  vval = (vval & (uint64)0xCCCCCCCCCCCCCCCC) >> 2
    | (vval & (uint64)0x3333333333333333) << 2;
  vval = (vval & (uint64)0xAAAAAAAAAAAAAAAA) >> 1
    | (vval & (uint64)0x5555555555555555) << 1;
  return vval;
}
#endif

/* ------------------------------------------------------------------------- */
