/*---------------------------------------------------------------------------*/

#include "./types.h"
#include "./ofchk.intern.h"

/*  */

int ofchk_add_s(OFCHK_TS va, OFCHK_TS vb, int vindex);
int ofchk_sub_s(OFCHK_TS va, OFCHK_TS vb, int vindex);
int ofchk_mul_s(OFCHK_TS va, OFCHK_TS vb, int vindex);
int ofchk_div_s(OFCHK_TS va, OFCHK_TS vb, int vindex);
int ofchk_add_u(OFCHK_TU va, OFCHK_TU vb, int vindex);
int ofchk_sub_u(OFCHK_TU va, OFCHK_TU vb, int vindex);
int ofchk_mul_u(OFCHK_TU va, OFCHK_TU vb, int vindex);
int ofchk_div_u(OFCHK_TU va, OFCHK_TU vb, int vindex);

/*  */

#define ofchk_add_schar(A, B) \
  ofchk_add_s((schar)(A), (schar)(B), OFCHK_CHAR_INDEX)

#define ofchk_sub_schar(A, B) \
  ofchk_sub_s((schar)(A), (schar)(B), OFCHK_CHAR_INDEX)

#define ofchk_mul_schar(A, B) \
  ofchk_mul_s((schar)(A), (schar)(B), OFCHK_CHAR_INDEX)

#define ofchk_div_schar(A, B) \
  ofchk_div_s((schar)(A), (schar)(B), OFCHK_CHAR_INDEX)

#define ofchk_add_uchar(A, B) \
  ofchk_add_u((uchar)(A), (uchar)(B), OFCHK_CHAR_INDEX)

#define ofchk_sub_uchar(A, B) \
  ofchk_sub_u((uchar)(A), (uchar)(B), OFCHK_CHAR_INDEX)

#define ofchk_mul_uchar(A, B) \
  ofchk_mul_u((uchar)(A), (uchar)(B), OFCHK_CHAR_INDEX)

#define ofchk_div_uchar(A, B) \
  ofchk_div_u((uchar)(A), (uchar)(B), OFCHK_CHAR_INDEX)

/*  */

#define ofchk_add_sint(A, B) \
  ofchk_add_s((sint)(A), (sint)(B), OFCHK_SHRT_INDEX)

#define ofchk_sub_sint(A, B) \
  ofchk_sub_s((sint)(A), (sint)(B), OFCHK_SHRT_INDEX)

#define ofchk_mul_sint(A, B) \
  ofchk_mul_s((sint)(A), (sint)(B), OFCHK_SHRT_INDEX)

#define ofchk_div_sint(A, B) \
  ofchk_div_s((sint)(A), (sint)(B), OFCHK_SHRT_INDEX)

#define ofchk_add_usint(A, B) \
  ofchk_add_u((usint)(A), (usint)(B), OFCHK_SHRT_INDEX)

#define ofchk_sub_usint(A, B) \
  ofchk_sub_u((usint)(A), (usint)(B), OFCHK_SHRT_INDEX)

#define ofchk_mul_usint(A, B) \
  ofchk_mul_u((usint)(A), (usint)(B), OFCHK_SHRT_INDEX)

#define ofchk_div_usint(A, B) \
  ofchk_div_u((usint)(A), (usint)(B), OFCHK_SHRT_INDEX)

/*  */

#define ofchk_add_int(A, B) \
  ofchk_add_s((int)(A), (int)(B), OFCHK_INT_INDEX)

#define ofchk_sub_int(A, B) \
  ofchk_sub_s((int)(A), (int)(B), OFCHK_INT_INDEX)

#define ofchk_mul_int(A, B) \
  ofchk_mul_s((int)(A), (int)(B), OFCHK_INT_INDEX)

#define ofchk_div_int(A, B) \
  ofchk_div_s((int)(A), (int)(B), OFCHK_INT_INDEX)

#define ofchk_add_uint(A, B) \
  ofchk_add_u((uint)(A), (uint)(B), OFCHK_INT_INDEX)

#define ofchk_sub_uint(A, B) \
  ofchk_sub_u((uint)(A), (uint)(B), OFCHK_INT_INDEX)

#define ofchk_mul_uint(A, B) \
  ofchk_mul_u((uint)(A), (uint)(B), OFCHK_INT_INDEX)

#define ofchk_div_uint(A, B) \
  ofchk_div_u((uint)(A), (uint)(B), OFCHK_INT_INDEX)

/*  */

#define ofchk_add_long(A, B) \
  ofchk_add_s((long)(A), (long)(B), OFCHK_LONG_INDEX)

#define ofchk_sub_long(A, B) \
  ofchk_sub_s((long)(A), (long)(B), OFCHK_LONG_INDEX)

#define ofchk_mul_long(A, B) \
  ofchk_mul_s((long)(A), (long)(B), OFCHK_LONG_INDEX)

#define ofchk_div_long(A, B) \
  ofchk_div_s((long)(A), (long)(B), OFCHK_LONG_INDEX)

#define ofchk_add_ulong(A, B) \
  ofchk_add_u((ulong)(A), (ulong)(B), OFCHK_LONG_INDEX)

#define ofchk_sub_ulong(A, B) \
  ofchk_sub_u((ulong)(A), (ulong)(B), OFCHK_LONG_INDEX)

#define ofchk_mul_ulong(A, B) \
  ofchk_mul_u((ulong)(A), (ulong)(B), OFCHK_LONG_INDEX)

#define ofchk_div_ulong(A, B) \
  ofchk_div_u((ulong)(A), (ulong)(B), OFCHK_LONG_INDEX)

/*  */
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define ofchk_add_llong(A, B) \
  ofchk_add_s((llong)(A), (llong)(B), OFCHK_LLONG_INDEX)

#define ofchk_sub_llong(A, B) \
  ofchk_sub_s((llong)(A), (llong)(B), OFCHK_LLONG_INDEX)

#define ofchk_mul_llong(A, B) \
  ofchk_mul_s((llong)(A), (llong)(B), OFCHK_LLONG_INDEX)

#define ofchk_div_llong(A, B) \
  ofchk_div_s((llong)(A), (llong)(B), OFCHK_LLONG_INDEX)

#define ofchk_add_ullong(A, B) \
  ofchk_add_u((ullong)(A), (ullong)(B), OFCHK_LLONG_INDEX)

#define ofchk_sub_ullong(A, B) \
  ofchk_sub_u((ullong)(A), (ullong)(B), OFCHK_LLONG_INDEX)

#define ofchk_mul_ullong(A, B) \
  ofchk_mul_u((ullong)(A), (ullong)(B), OFCHK_LLONG_INDEX)

#define ofchk_div_ullong(A, B) \
  ofchk_div_u((ullong)(A), (ullong)(B), OFCHK_LLONG_INDEX)
#endif

/*  */
#ifdef INT8_MAX
#define ofchk_add_int8(A, B) \
  ofchk_add_s((int8)(A), (int8)(B), OFCHK_INT8_INDEX)

#define ofchk_sub_int8(A, B) \
  ofchk_sub_s((int8)(A), (int8)(B), OFCHK_INT8_INDEX)

#define ofchk_mul_int8(A, B) \
  ofchk_mul_s((int8)(A), (int8)(B), OFCHK_INT8_INDEX)

#define ofchk_div_int8(A, B) \
  ofchk_div_s((int8)(A), (int8)(B), OFCHK_INT8_INDEX)

#define ofchk_add_uint8(A, B) \
  ofchk_add_u((uint8)(A), (uint8)(B), OFCHK_INT8_INDEX)

#define ofchk_sub_uint8(A, B) \
  ofchk_sub_u((uint8)(A), (uint8)(B), OFCHK_INT8_INDEX)

#define ofchk_mul_uint8(A, B) \
  ofchk_mul_u((uint8)(A), (uint8)(B), OFCHK_INT8_INDEX)

#define ofchk_div_uint8(A, B) \
  ofchk_div_u((uint8)(A), (uint8)(B), OFCHK_INT8_INDEX)
#endif
/*  */
#ifdef INT16_MAX
#define ofchk_add_int16(A, B) \
  ofchk_add_s((int16)(A), (int16)(B), OFCHK_INT16_INDEX)

#define ofchk_sub_int16(A, B) \
  ofchk_sub_s((int16)(A), (int16)(B), OFCHK_INT16_INDEX)

#define ofchk_mul_int16(A, B) \
  ofchk_mul_s((int16)(A), (int16)(B), OFCHK_INT16_INDEX)

#define ofchk_div_int16(A, B) \
  ofchk_div_s((int16)(A), (int16)(B), OFCHK_INT16_INDEX)

#define ofchk_add_uint16(A, B) \
  ofchk_add_u((uint16)(A), (uint16)(B), OFCHK_INT16_INDEX)

#define ofchk_sub_uint16(A, B) \
  ofchk_sub_u((uint16)(A), (uint16)(B), OFCHK_INT16_INDEX)

#define ofchk_mul_uint16(A, B) \
  ofchk_mul_u((uint16)(A), (uint16)(B), OFCHK_INT16_INDEX)

#define ofchk_div_uint16(A, B) \
  ofchk_div_u((uint16)(A), (uint16)(B), OFCHK_INT16_INDEX)
#endif
/*  */
#ifdef INT32_MAX
#define ofchk_add_int32(A, B) \
  ofchk_add_s((int32)(A), (int32)(B), OFCHK_INT32_INDEX)

#define ofchk_sub_int32(A, B) \
  ofchk_sub_s((int32)(A), (int32)(B), OFCHK_INT32_INDEX)

#define ofchk_mul_int32(A, B) \
  ofchk_mul_s((int32)(A), (int32)(B), OFCHK_INT32_INDEX)

#define ofchk_div_int32(A, B) \
  ofchk_div_s((int32)(A), (int32)(B), OFCHK_INT32_INDEX)

#define ofchk_add_uint32(A, B) \
  ofchk_add_u((uint32)(A), (uint32)(B), OFCHK_INT32_INDEX)

#define ofchk_sub_uint32(A, B) \
  ofchk_sub_u((uint32)(A), (uint32)(B), OFCHK_INT32_INDEX)

#define ofchk_mul_uint32(A, B) \
  ofchk_mul_u((uint32)(A), (uint32)(B), OFCHK_INT32_INDEX)

#define ofchk_div_uint32(A, B) \
  ofchk_div_u((uint32)(A), (uint32)(B), OFCHK_INT32_INDEX)
#endif
/*  */
#ifdef INT64_MAX
#define ofchk_add_int64(A, B) \
  ofchk_add_s((int64)(A), (int64)(B), OFCHK_INT64_INDEX)

#define ofchk_sub_int64(A, B) \
  ofchk_sub_s((int64)(A), (int64)(B), OFCHK_INT64_INDEX)

#define ofchk_mul_int64(A, B) \
  ofchk_mul_s((int64)(A), (int64)(B), OFCHK_INT64_INDEX)

#define ofchk_div_int64(A, B) \
  ofchk_div_s((int64)(A), (int64)(B), OFCHK_INT64_INDEX)

#define ofchk_add_uint64(A, B) \
  ofchk_add_u((uint64)(A), (uint64)(B), OFCHK_INT64_INDEX)

#define ofchk_sub_uint64(A, B) \
  ofchk_sub_u((uint64)(A), (uint64)(B), OFCHK_INT64_INDEX)

#define ofchk_mul_uint64(A, B) \
  ofchk_mul_u((uint64)(A), (uint64)(B), OFCHK_INT64_INDEX)

#define ofchk_div_uint64(A, B) \
  ofchk_div_u((uint64)(A), (uint64)(B), OFCHK_INT64_INDEX)
#endif
/*  */

#define ofchk_add_intp(A, B) \
  ofchk_add_s((intp)(A), (intp)(B), OFCHK_INTPTR_INDEX)

#define ofchk_sub_intp(A, B) \
  ofchk_sub_s((intp)(A), (intp)(B), OFCHK_INTPTR_INDEX)

#define ofchk_mul_intp(A, B) \
  ofchk_mul_s((intp)(A), (intp)(B), OFCHK_INTPTR_INDEX)

#define ofchk_div_intp(A, B) \
  ofchk_div_s((intp)(A), (intp)(B), OFCHK_INTPTR_INDEX)

#define ofchk_add_uintp(A, B) \
  ofchk_add_u((uintp)(A), (uintp)(B), OFCHK_INTPTR_INDEX)

#define ofchk_sub_uintp(A, B) \
  ofchk_sub_u((uintp)(A), (uintp)(B), OFCHK_INTPTR_INDEX)

#define ofchk_mul_uintp(A, B) \
  ofchk_mul_u((uintp)(A), (uintp)(B), OFCHK_INTPTR_INDEX)

#define ofchk_div_uintp(A, B) \
  ofchk_div_u((uintp)(A), (uintp)(B), OFCHK_INTPTR_INDEX)

/*---------------------------------------------------------------------------*/
