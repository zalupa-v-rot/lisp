/*---------------------------------------------------------------------------*/

#ifndef TYPES_H_
#define TYPES_H_

#include <limits.h>
#include <stddef.h>

#if ((defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
     defined(__GNUC__))

#include <stdint.h>

#endif


#if ((!defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L) \
     && !defined(inline))

#if defined(__GNUC__)
#define inline __inline__
#else
#define inline /* none */
#endif

#endif

/*---------------------------------------------------------------------------*/

/*               char              char  ;*/
typedef   signed char              schar ;
typedef unsigned char              uchar ;
typedef unsigned short             usint ;
typedef   signed short             sint  ;
typedef unsigned int               uint  ;
/*        signed int               int   ;*/
typedef unsigned long              ulong ;
/*        signed long              long  ;*/
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
typedef unsigned long long         ullong;
typedef   signed long long         llong ;
#endif

#ifdef UINT8_MAX
typedef  uint8_t                   uint8 ;
typedef   int8_t                   int8  ;
#endif

#ifdef UINT16_MAX
typedef uint16_t                   uint16;
typedef  int16_t                   int16 ;
#endif

#ifdef UINT32_MAX
typedef uint32_t                   uint32;
typedef  int32_t                   int32 ;
#endif

#ifdef UINT64_MAX
typedef uint64_t                   uint64;
typedef  int64_t                   int64 ;
#endif

#if defined(UINTPTR_MAX)
typedef uintptr_t                  uintp ;
typedef intptr_t                   intp  ;
#else
/* такого быть не должно*/
#error
#endif

/*---------------------------------------------------------------------------*/

#if defined(__GNUC__)

#if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN
#endif

#elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)

#ifndef BIG_ENDIAN
#define BIG_ENDIAN
#endif

#elif (__BYTE_ORDER__ == __ORDER_PDP_ENDIAN__)

#ifndef PDP_ENDIAN
#define PDP_ENDIAN
#endif

#endif

#endif

#if    (!defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN) && !defined(PDP_ENDIAN))
#define LITTLE_ENDIAN
#endif

/*---------------------------------------------------------------------------*/

#define container_of(P, T, M) ((T*)((char*)(P) - offsetof(T, M)))

#ifndef ALIGNOF
#define ALIGNOF(T) offsetof(struct { char c; T member; }, member)
#endif


#ifndef ALIGN
#define ALIGN(P, A) (void*)( (uintp)(P) + ((A)-1) & ~(uintp)((A)-1) )
#endif


#ifndef BITSOF
#define BITSOF(X) (sizeof(X) * CHAR_BIT)
#endif

#ifndef UNUSED
#define UNUSED(X) ((void)(X))
#endif

#define NORETURN(F) F
#define NONREENTRANT(F) F



#endif /* TYPES_H_ */

/*---------------------------------------------------------------------------*/
