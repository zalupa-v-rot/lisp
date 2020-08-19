/* ------------------------------------------------------------------------- */

#ifndef BITS_H
#define BITS_H

#include "./types.h"

typedef struct {

  uchar   *pbyte;
  schar    vbit;

} bitptr_t;


/*
  orientation:
  IO - increase oriented
  DO - decrease oriented
  bit order:
  LSF - least significant bit first
  MSF - most significant bit first
*/
#define BM_ORIENT 1
#define BM_BORDER 2

#define BM_BYTE 1
#define BM_BIT  2

#define BM_IO  0
#define BM_DO  1
#define BM_LSF 0
#define BM_MSF 2

#define BM_DEFAULT (BM_IO|BM_LSF)
#define BM_TRADITIONAL BM_DEFAULT

bitptr_t bitptr(void *pbyte, uint vmode);

void* bitptr_getbyte(bitptr_t sp);
uchar bitptr_getbit(bitptr_t sp);

bitptr_t bitptr_floor(bitptr_t sp, uint vmode);
bitptr_t bitptr_ceil(bitptr_t sp, uint vmode);

/*
  sp + vbits
  sp - vbits
*/
bitptr_t bitptr_add(bitptr_t sp, uint vmode, intp vbits);
bitptr_t bitptr_sub(bitptr_t sp, uint vmode, intp vbits);

/*
  a - b

  bitptr_diff(bitptr_add(sb, bitptr_diff(sa, sb)), sa) == 0

  то есть, это должно читаться так:
  расстояние от sb, по mode, до указателя в sa.
*/
intp bitptr_diff(bitptr_t sa, bitptr_t sb, uint vmode);

#define bitsetc(P, M, B, V) bitseti(P, M, B, (uchar)(V))
#define bitgetc(P, M, B)    (uchar)bitgeti(P, M, B)

#define bitsets(P, M, B, V) bitseti(P, M, B, (usint)(V))
#define bitgets(P, M, B)    (usint)bitgeti(P, M, B)

#define bitset bitseti
#define bitget bitgeti

void bitseti(bitptr_t sp, uint vmode, uint vbits, uint vval);
uint bitgeti(bitptr_t sp, uint vmode, uint vbits);

void bitsetl(bitptr_t sp, uint vmode, uint vbits, ulong vval);
ulong bitgetl(bitptr_t sp, uint vmode, uint vbits);

void bitsetp(bitptr_t sp, uint vmode, uint vbits, uintp vval);
uintp bitgetp(bitptr_t sp, uint vmode, uint vbits);

/* ------------------------------------------------------------------------- */

/*
  копировать vbits из ssrc в sdst
  ssrc и sdst могут пересекаться только,
  если у них один режим и bitptr_diff(ssrc, sdst, vmode_dst) >= 0
  (!!! сомнительная гарантия, нужно её убрать)
*/
void bitmemcpy(bitptr_t sdst, uint vmode_dst,
               bitptr_t ssrc, uint vmode_src, uintp vbits);


/*
  установить значение val на по всему размеру vbits

  !!! сомнительная функция. с другой стороны, если указывать размер val, то
  зачем ограничиваться размером int? так проще использовать, а правильный
  функционал можно назвать: bitmemcpy_cycle()
*/
void bitmemset(bitptr_t sdst, uint vmode_dst, uint val, uintp vbits);


/*
  сравнить vbits в sa и sb, возвращая результат первого отличающегося бита.
*/
int bitmemcmp(bitptr_t sa, uint vmode_a,
              bitptr_t sb, uint vmode_b, uintp vbits);


/*
  сместить биты по направлению к старшему. на месте смещённых остаются нули.
*/
void bitmemshiftl(bitptr_t sa, uint vmode, uintp vshift, uintp vbits);

/*
  сместить биты по направлению к младшему. на месте смещённых остаются нули.
*/
void bitmemshiftr(bitptr_t sa, uint vmode, uintp vshift, uintp vbits);


/*
  поменять биты местами.
*/
void bitmemrev(bitptr_t sa, uint vmode, uintp vbits);

/* ------------------------------------------------------------------------- */

#ifdef UINT8_MAX
uint8 bitrev8(uint8 vval);
#endif
#ifdef UINT16_MAX
uint16 bitrev16(uint16 vval);
#endif
#ifdef UINT32_MAX
uint32 bitrev32(uint32 vval);
#endif
#ifdef UINT64_MAX
uint64 bitrev64(uint64 vval);
#endif

#if (UCHAR_MAX == UINT8_MAX)
#define bitrevc (uchar)bitrev8
#else
#error
#endif

#if   (defined(UINT16_MAX) && UINT_MAX == UINT16_MAX)
#define bitrev (uint)bitrev16
#elif (defined(UINT32_MAX) && UINT_MAX == UINT32_MAX)
#define bitrev (uint)bitrev32
#elif (defined(UINT64_MAX) && UINT_MAX == UINT64_MAX)
#define bitrev (uint)bitrev64
#else
#error
#endif

#if   (defined(UINT16_MAX) && UINTPTR_MAX == UINT16_MAX)
#define bitrevp (uintp)bitrev16
#elif (defined(UINT32_MAX) && UINTPTR_MAX == UINT32_MAX)
#define bitrevp (uintp)bitrev32
#elif (defined(UINT64_MAX) && UINTPTR_MAX == UINT64_MAX)
#define bitrevp (uintp)bitrev64
#else
#error
#endif

#endif /* BITS_H */

/* ------------------------------------------------------------------------- */
