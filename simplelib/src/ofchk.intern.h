/*---------------------------------------------------------------------------*/

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)

#if (ULLONG_MAX >= UINTPTR_MAX)
#define OFCHK_TS llong
#define OFCHK_TU ullong
#else
#define OFCHK_TS intp
#define OFCHK_TU uintp
#endif

#elif (defined(UINT64_MAX))

#if (UINT64_MAX >= UINTPTR_MAX)
#define OFCHK_TS int64
#define OFCHK_TU uint64
#else
#define OFCHK_TS intp
#define OFCHK_TU uintp
#endif

#else

#if (ULONG_MAX >= UINTPTR_MAX)
#define OFCHK_TS long
#define OFCHK_TU ulong
#else
#define OFCHK_TS intp
#define OFCHK_TU uintp
#endif

#endif

#define OFCHK_CHAR_INDEX 0
#define OFCHK_SHRT_INDEX 1
#define OFCHK_INT_INDEX  2
#define OFCHK_LONG_INDEX 3
#define OFCHK_BASE_INDEX OFCHK_LONG_INDEX

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define OFCHK_LLONG_INDEX (OFCHK_BASE_INDEX+1)
#else
#define OFCHK_LLONG_INDEX  OFCHK_BASE_INDEX
#endif

#ifdef INT8_MAX
#define OFCHK_INT8_INDEX  (OFCHK_LLONG_INDEX + 1)
#else
#define OFCHK_INT8_INDEX  (OFCHK_LLONG_INDEX)
#endif

#ifdef INT16_MAX
#define OFCHK_INT16_INDEX  (OFCHK_INT8_INDEX + 1)
#else
#define OFCHK_INT16_INDEX  (OFCHK_INT8_INDEX)
#endif

#ifdef INT32_MAX
#define OFCHK_INT32_INDEX  (OFCHK_INT16_INDEX + 1)
#else
#define OFCHK_INT32_INDEX  (OFCHK_INT16_INDEX)
#endif

#ifdef INT64_MAX
#define OFCHK_INT64_INDEX  (OFCHK_INT32_INDEX + 1)
#else
#define OFCHK_INT64_INDEX  (OFCHK_INT32_INDEX)
#endif

#define OFCHK_INTPTR_INDEX  (OFCHK_INT64_INDEX + 1)

/*---------------------------------------------------------------------------*/
