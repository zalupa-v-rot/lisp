/*---------------------------------------------------------------------------*/

#include <assert.h> /* assert(), static_assert() */


#if (!defined(NDEBUG))

#include <stdio.h>

void assert_(const char *pfile, int vline, const char *pexpr);

#undef assert
#define assert(EXP)                                           \
  ((EXP) == 0 ? assert_(__FILE__, __LINE__, #EXP) : (void)0)


void assert_ret_(const char *pfile, int vline, const char *pexpr);

#define assert_ret(EXP, CODE)                   \
  do {                                          \
    if ((EXP) == 0) {                           \
      assert_ret_(__FILE__, __LINE__, #EXP);    \
      return (CODE);                            \
    }                                           \
  } while(0)

#if ((!defined(__STDC_VERSION__) || __STDC_VERSION__ < 201112L) \
     && !defined(static_assert))

#ifdef __COUNTER__
#define static_assert(CD, ST) static_assert2(CD, __COUNTER__)
#define static_assert2(CD, L) static_assert3(CD, L)
#define static_assert3(CD, L) typedef char static_assertion_##L[(CD)?1:-1]
#endif

#endif

#else

#ifndef static_assert
#define static_assert(CD, ST) void static_assertion_no()
#endif

#ifndef assert_ret
#define assert_ret(EXP, CODE) ((void)(EXP),(void)CODE)
#endif

#endif

/*---------------------------------------------------------------------------*/
