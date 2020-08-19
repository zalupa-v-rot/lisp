/* ------------------------------------------------------------------------- */

#if (!defined(NDEBUG))

#include <stdio.h>

void assert_(const char *pfile, int vline, const char *pexpr);

#undef assert
#define assert(EXP) \
  ((EXP) == 0 ? assert_(__FILE__, __LINE__, #EXP) : (void)0)


void assert_info_(const char *pfile, int vline,
                  const char *pexpr, const char *pinfo);

#define assert_info(EXP, INFO)                                          \
  ((EXP) == 0 ? assert_info_(__FILE__, __LINE__, #EXP, (INFO)) : (void)0)


#define assert_ret(EXP, CODE)                                           \
  do { if (!(EXP)) { printf("assert_ret: line %i\n", __LINE__); return (CODE); } } while(0)

#if ((!defined(__STDC_VERSION__) || __STDC_VERSION__ + 0 < 201112L) && !defined(static_assert))

#ifdef __COUNTER__
#define static_assert(CD, ST) static_assert2(CD, __COUNTER__)
#define static_assert2(CD, L) static_assert3(CD, L)
#define static_assert3(CD, L) typedef char static_assertion_##L[(CD)?1:-1]
#endif

#endif

#else

#ifndef assert
#define assert(EXP) ((void)(EXP))
#endif

#ifndef assert_info
#define assert_info(EXP, INFO) ((void)(EXP),(void)INFO)
#endif

#ifndef assert_ret
#define assert_ret(EXP, CODE) ((void)(EXP),(void)CODE)
#endif

#ifndef static_assert
#define static_assert(CD, ST) (void)
#endif

#endif

/* ------------------------------------------------------------------------- */
