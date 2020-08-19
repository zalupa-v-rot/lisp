/*
  char, sint, int, long, void*, intp, float, double, long double
  + llong, int8, int16, int32, int64
*/
#define TESTTYPES_N (9+TLLONG+TINT8+TINT16+TINT32+TINT64)

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define TLLONG 1
#else
#define TLLONG 0
#endif
#ifdef UINT8_MAX
#define TINT8 1
#else
#define TINT8 0
#endif
#ifdef UINT16_MAX
#define TINT16 1
#else
#define TINT16 0
#endif
#ifdef UINT32_MAX
#define TINT32 1
#else
#define TINT32 0
#endif
#ifdef UINT64_MAX
#define TINT64 1
#else
#define TINT64 0
#endif

uintp gl_sizes[TESTTYPES_N] = {

  sizeof(char),
  sizeof(sint),
  sizeof(int),
  sizeof(long),
  sizeof(void*),
  sizeof(intp),

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
  sizeof(llong),
#endif
#ifdef UINT8_MAX
  sizeof(int8),
#endif
#ifdef UINT16_MAX
  sizeof(int16),
#endif
#ifdef UINT32_MAX
  sizeof(int32),
#endif
#ifdef UINT64_MAX
  sizeof(int64),
#endif
  sizeof(float),
  sizeof(double),
  sizeof(long double)
};

uintp gl_align[TESTTYPES_N] = {

  ALIGNOF(char),
  ALIGNOF(sint),
  ALIGNOF(int),
  ALIGNOF(long),
  ALIGNOF(void*),
  ALIGNOF(intp),

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
  ALIGNOF(llong),
#endif
#ifdef UINT8_MAX
  ALIGNOF(int8),
#endif
#ifdef UINT16_MAX
  ALIGNOF(int16),
#endif
#ifdef UINT32_MAX
  ALIGNOF(int32),
#endif
#ifdef UINT64_MAX
  ALIGNOF(int64),
#endif
  ALIGNOF(float),
  ALIGNOF(double),
  ALIGNOF(long double)
};
