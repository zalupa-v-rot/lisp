/*------------------------------------------------------------------------------*/

#include "./tests.h"

int test0(uint vmode) {

  bitptr_t sa;

  /* чтобы не дрочить санитайзер */
  sa = bitptr((void*)1, vmode);

  assert_ret(bitptr_getbyte(sa) == (void*)1, FALSE);
  assert_ret(bitptr_getbit(sa) == (CHAR_BIT-1) * ((vmode&BM_BORDER) == BM_MSF), FALSE);

  assert_ret(teq(sa, bitptr_add(sa, vmode, 0), TEQ_BYTE|TEQ_BIT), FALSE);
  assert_ret(teq(sa, bitptr_sub(sa, vmode, 0), TEQ_BYTE|TEQ_BIT), FALSE);

  assert_ret(teq(sa, bitptr_add(sa, vmode, CHAR_BIT-1), TEQ_BYTE), FALSE);
  assert_ret(teq(sa, bitptr_add(sa, vmode, CHAR_BIT), TEQ_BIT), FALSE);

  assert_ret(teq(sa, bitptr_sub(sa, vmode, 1), 0), FALSE);
  assert_ret(teq(sa, bitptr_sub(sa, vmode, CHAR_BIT), TEQ_BIT), FALSE);

  assert_ret(teq(sa, bitptr_floor(sa, vmode), TEQ_BYTE|TEQ_BIT), FALSE);
  assert_ret(teq(sa, bitptr_floor(bitptr_add(sa, vmode, CHAR_BIT-1), vmode), TEQ_BYTE|TEQ_BIT), FALSE);

  assert_ret(teq(sa, bitptr_ceil(sa, vmode), TEQ_BYTE|TEQ_BIT), FALSE);
  assert_ret(teq(sa, bitptr_ceil(bitptr_add(sa, vmode, 1), vmode), TEQ_BIT), FALSE);

  /* чтобы не дрочить санитайзер */
  sa = bitptr((void*)2, vmode);
  assert_ret(bitptr_diff(sa, sa, vmode) == 0, FALSE);

  assert_ret(bitptr_diff(bitptr_add(sa, vmode, CHAR_BIT-1),
                            bitptr_add(sa, vmode, CHAR_BIT-1), vmode) == 0, FALSE);
  assert_ret(bitptr_diff(bitptr_add(sa, vmode, CHAR_BIT*2-1),
                            bitptr_add(sa, vmode, CHAR_BIT*2-1), vmode) == 0, FALSE);
  assert_ret(bitptr_diff(bitptr_sub(sa, vmode, CHAR_BIT-1),
                            bitptr_sub(sa, vmode, CHAR_BIT-1), vmode) == 0, FALSE);

  assert_ret(bitptr_diff(bitptr_add(sa, vmode, -CHAR_BIT+1),
                            bitptr_add(sa, vmode, -CHAR_BIT+1), vmode) == 0, FALSE);
  assert_ret(bitptr_diff(bitptr_add(sa, vmode, -CHAR_BIT*2+1),
                            bitptr_add(sa, vmode, -CHAR_BIT*2+1), vmode) == 0, FALSE);
  assert_ret(bitptr_diff(bitptr_sub(sa, vmode, -CHAR_BIT+1),
                            bitptr_sub(sa, vmode, -CHAR_BIT+1), vmode) == 0, FALSE);


  assert_ret(bitptr_diff(bitptr_add(sa, vmode, CHAR_BIT-1), sa, vmode) == +CHAR_BIT-1, FALSE);
  assert_ret(bitptr_diff(sa, bitptr_add(sa, vmode, CHAR_BIT-1), vmode) == -CHAR_BIT+1, FALSE);

  assert_ret(bitptr_diff(bitptr_sub(sa, vmode, CHAR_BIT-1), sa, vmode) == -CHAR_BIT+1, FALSE);
  assert_ret(bitptr_diff(sa, bitptr_sub(sa, vmode, CHAR_BIT-1), vmode) == +CHAR_BIT-1, FALSE);

  assert_ret(bitptr_diff(bitptr_add(sa, vmode, -CHAR_BIT+1), sa, vmode) == -CHAR_BIT+1, FALSE);
  assert_ret(bitptr_diff(sa, bitptr_add(sa, vmode, -CHAR_BIT+1), vmode) == +CHAR_BIT-1, FALSE);

  assert_ret(bitptr_diff(bitptr_sub(sa, vmode, -CHAR_BIT+1), sa, vmode) == +CHAR_BIT-1, FALSE);
  assert_ret(bitptr_diff(sa, bitptr_sub(sa, vmode, -CHAR_BIT+1), vmode) == -CHAR_BIT+1, FALSE);


  /* чтобы не дрочить санитайзер */
  sa = ((vmode & BM_BYTE) == BM_IO)? bitptr((void*)0, vmode) :
    bitptr((void*)(INTPTR_MAX/CHAR_BIT), vmode);

  assert_ret(bitptr_diff(bitptr_add(sa, vmode, INTPTR_MAX), sa, vmode) == +INTPTR_MAX, FALSE);
  assert_ret(bitptr_diff(sa, bitptr_add(sa, vmode, INTPTR_MAX), vmode) == -INTPTR_MAX, FALSE);

  assert_ret(bitptr_diff(bitptr_sub(sa, vmode, -INTPTR_MAX), sa, vmode) == INTPTR_MAX, FALSE);
  assert_ret(bitptr_diff(sa, bitptr_sub(sa, vmode, -INTPTR_MAX), vmode) == -INTPTR_MAX, FALSE);


  /* чтобы не дрочить санитайзер */
  sa = ((vmode & BM_BYTE) == BM_IO)? bitptr((void*)(UINTPTR_MAX/CHAR_BIT), vmode) :
    bitptr((void*)0, vmode);

  assert_ret(bitptr_diff(bitptr_sub(sa, vmode, INTPTR_MAX), sa, vmode) == -INTPTR_MAX, FALSE);
  assert_ret(bitptr_diff(sa, bitptr_sub(sa, vmode, INTPTR_MAX), vmode) == +INTPTR_MAX, FALSE);

  assert_ret(bitptr_diff(bitptr_add(sa, vmode, INTPTR_MIN), sa, vmode) == INTPTR_MIN, FALSE);
  assert_ret(bitptr_diff(sa, bitptr_add(sa, vmode, -INTPTR_MAX), vmode) == INTPTR_MAX, FALSE);


  return TRUE;
}

#define BI(M)     ((((M) & BM_BIT) == BM_LSF)?1:-1)
#define BIOFF(M,O)  ((BI(M)==-1)?CHAR_BIT-1-(O):(O))
#define BY(M)     ((((M) & BM_BYTE) == BM_IO)?1:-1)
#define BYOFF(O)  (CHAR_BIT * (O))

int test1(uint vbm, uint vam) {

  bitptr_t sa, sb;


  sa = ptrbitoff(0, vam, 0);
  sb = ptrbitoff(0, vbm, 0);
  assert_ret(bitptr_diff(sb, sa, vam) ==
             (BIOFF(vbm,0) - BIOFF(vam,0)) * BI(vam), FALSE);

  sa = ptrbitoff(0, vam, 1);
  sb = ptrbitoff(0, vbm, 1);
  assert_ret(bitptr_diff(sb, sa, vam) ==
             (BIOFF(vbm,1) - BIOFF(vam,1)) * BI(vam), FALSE);

  sa = ptrbitoff(0, vam, 0);
  sb = ptrbitoff(0, vbm, 1);
  assert_ret(bitptr_diff(sb, sa, vam) ==
             (BIOFF(vbm,1) - BIOFF(vam,0)) * BI(vam), FALSE);

  sa = ptrbitoff(0, vam, 1);
  sb = ptrbitoff(0, vbm, 0);
  assert_ret(bitptr_diff(sb, sa, vam) ==
             (BIOFF(vbm,0) - BIOFF(vam,1)) * BI(vam), FALSE);


  sa = ptrbitoff(0, vam, 0);
  sb = ptrbitoff(1, vbm, 0);
  assert_ret(bitptr_diff(sb, sa, vam) == (BYOFF(1) - BYOFF(0)) * BY(vam)
             + (BIOFF(vbm,0) - BIOFF(vam,0)) * BI(vam), FALSE);

  sa = ptrbitoff(0, vam, 1);
  sb = ptrbitoff(1, vbm, 1);
  assert_ret(bitptr_diff(sb, sa, vam) == (BYOFF(1) - BYOFF(0)) * BY(vam)
             + (BIOFF(vbm,1) - BIOFF(vam,1)) * BI(vam), FALSE);

  sa = ptrbitoff(0, vam, 0);
  sb = ptrbitoff(1, vbm, 1);
  assert_ret(bitptr_diff(sb, sa, vam) == (BYOFF(1) - BYOFF(0)) * BY(vam)
             + (BIOFF(vbm,1) - BIOFF(vam,0)) * BI(vam), FALSE);

  sa = ptrbitoff(0, vam, 1);
  sb = ptrbitoff(1, vbm, 0);
  assert_ret(bitptr_diff(sb, sa, vam) == (BYOFF(1) - BYOFF(0)) * BY(vam)
             + (BIOFF(vbm,0) - BIOFF(vam,1)) * BI(vam), FALSE);


  sa = ptrbitoff(1, vam, 0);
  sb = ptrbitoff(0, vbm, 0);
  assert_ret(bitptr_diff(sb, sa, vam) == (BYOFF(0) - BYOFF(1)) * BY(vam)
             + (BIOFF(vbm,0) - BIOFF(vam,0)) * BI(vam), FALSE);

  sa = ptrbitoff(1, vam, 1);
  sb = ptrbitoff(0, vbm, 1);
  assert_ret(bitptr_diff(sb, sa, vam) == (BYOFF(0) - BYOFF(1)) * BY(vam)
             + (BIOFF(vbm,1) - BIOFF(vam,1)) * BI(vam), FALSE);

  sa = ptrbitoff(1, vam, 0);
  sb = ptrbitoff(0, vbm, 1);
  assert_ret(bitptr_diff(sb, sa, vam) == (BYOFF(0) - BYOFF(1)) * BY(vam)
             + (BIOFF(vbm,1) - BIOFF(vam,0)) * BI(vam), FALSE);

  sa = ptrbitoff(1, vam, 1);
  sb = ptrbitoff(0, vbm, 0);
  assert_ret(bitptr_diff(sb, sa, vam) == (BYOFF(0) - BYOFF(1)) * BY(vam)
             + (BIOFF(vbm,0) - BIOFF(vam,1)) * BI(vam), FALSE);


  sa = ptrbitoff(1, vam, 0);
  sb = ptrbitoff(1, vbm, 0);
  assert_ret(bitptr_diff(sb, sa, vam) ==
             (BIOFF(vbm,0) - BIOFF(vam,0)) * BI(vam), FALSE);

  sa = ptrbitoff(1, vam, 1);
  sb = ptrbitoff(1, vbm, 1);
  assert_ret(bitptr_diff(sb, sa, vam) ==
             (BIOFF(vbm,1) - BIOFF(vam,1)) * BI(vam), FALSE);

  sa = ptrbitoff(1, vam, 0);
  sb = ptrbitoff(1, vbm, 1);
  assert_ret(bitptr_diff(sb, sa, vam) ==
             (BIOFF(vbm,1) - BIOFF(vam,0)) * BI(vam), FALSE);

  sa = ptrbitoff(1, vam, 1);
  sb = ptrbitoff(1, vbm, 0);
  assert_ret(bitptr_diff(sb, sa, vam) ==
             (BIOFF(vbm,0) - BIOFF(vam,1)) * BI(vam), FALSE);


  return TRUE;
}

#undef BI
#undef BIOFF
#undef BY
#undef BYOFF

int test2(void) {


  assert_ret(bitrevc(0) == 0, FALSE);
  assert_ret(bitrevc((uchar)-1) == (uchar)-1, FALSE);
  assert_ret(bitrevc(0x55) == (uchar)0xAA << BITSOF(uchar)-8, FALSE);
  assert_ret(bitrevc(0xAA) == (uchar)0x55 << BITSOF(uchar)-8, FALSE);

  assert_ret(bitrev(0) == 0, FALSE);
  assert_ret(bitrev((uint)-1) == (uint)-1, FALSE);
  assert_ret(bitrev(0x5555) == (uint)0xAAAA << BITSOF(int)-16, FALSE);
  assert_ret(bitrev(0xAAAA) == (uint)0x5555 << BITSOF(int)-16, FALSE);

  assert_ret(bitrevp(0) == 0, FALSE);
  assert_ret(bitrevp((uintp)-1) == (uintp)-1, FALSE);
  assert_ret(bitrevp(0x5555) == (uintp)0xAAAA << BITSOF(uintp)-16, FALSE);
  assert_ret(bitrevp(0xAAAA) == (uintp)0x5555 << BITSOF(uintp)-16, FALSE);

#ifdef UINT8_MAX
  assert_ret(bitrev8(0) == 0, FALSE);
  assert_ret(bitrev8((uint8)-1) == (uint8)-1, FALSE);
  assert_ret(bitrev8(0x55) == 0xAAU, FALSE);
  assert_ret(bitrev8(0xAA) == 0x55U, FALSE);
#endif
#ifdef UINT16_MAX
  assert_ret(bitrev16(0) == 0, FALSE);
  assert_ret(bitrev16((uint16)-1) == (uint16)-1, FALSE);
  assert_ret(bitrev16(0x5555) == 0xAAAAU, FALSE);
  assert_ret(bitrev16(0xAAAA) == 0x5555U, FALSE);
#endif
#ifdef UINT32_MAX
  assert_ret(bitrev32(0) == 0, FALSE);
  assert_ret(bitrev32((uint32)-1) == (uint32)-1, FALSE);
  assert_ret(bitrev32(0x55555555L) == 0xAAAAAAAAUL, FALSE);
  assert_ret(bitrev32(0xAAAAAAAAL) == 0x55555555UL, FALSE);
#endif
#ifdef UINT64_MAX
  assert_ret(bitrev64(0) == 0, FALSE);
  assert_ret(bitrev64((uint64)-1) == (uint64)-1, FALSE);
  assert_ret(bitrev64((uint64)0x5555555555555555) == (uint64)0xAAAAAAAAAAAAAAAA, FALSE);
  assert_ret(bitrev64((uint64)0xAAAAAAAAAAAAAAAA) == (uint64)0x5555555555555555, FALSE);
#endif

  return TRUE;
}

int test3(uint vmode) {


#define FSET bitsetc
#define FGET bitgetc
#define T    uchar
#include "./test3.inc"

#define FSET bitsets
#define FGET bitgets
#define T    usint
#include "./test3.inc"

#define FSET bitset
#define FGET bitget
#define T    uint
#include "./test3.inc"

#define FSET bitsetl
#define FGET bitgetl
#define T    ulong
#include "./test3.inc"

#define FSET bitsetp
#define FGET bitgetp
#define T    uintp
#include "./test3.inc"

  return TRUE;
}

/* int test4(uint vbm, uint vam) { */



/* } */

#define N 64
#include <string.h>

int test5(uint vbm, uint vam) {

  uchar *pabuf, *pa, *pbbuf, *pb;
  uintp vbits, vbytes, voff;


  for (voff = 0; voff != CHAR_BIT; voff++) {


    vbits = N * CHAR_BIT;
    vbytes = (vbits + voff + (CHAR_BIT-1)) / CHAR_BIT;

    {
      assert_ret((pa = pabuf = malloc(vbytes)) != NULL, FALSE);

      if ((vam & BM_BYTE) == BM_DO) {

        pa += vbytes - 1;
      }
    }

    {
      assert_ret((pb = pbbuf = malloc(vbytes)) != NULL, FALSE);

      if ((vbm & BM_BYTE) == BM_DO) {

        pb += vbytes - 1;
      }
    }

    {
      bitptr_t sa, sb;

      sa = bitptr_add(bitptr(pa, vam), vam, voff);
      sb = bitptr_add(bitptr(pb, vbm), vbm, voff);

      memset(pabuf, 0, sizeof(vbytes)), memset(pbbuf, 0, sizeof(vbytes));

      bitmemset(sb, vbm, 0x55, vbits);
      bitmemcpy(sa, vam, sb, vbm, vbits);

      assert_ret(bitmemcmp(sa, vam, sb, vbm, vbits) == 0, FALSE);
    }

    free(pabuf), free(pbbuf);
  }

  return TRUE;
}

#undef N

/*------------------------------------------------------------------------------*/
