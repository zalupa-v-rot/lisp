/*------------------------------------------------------------------------------*/

#include "./tests.h"

#include <string.h>

int test0(void) {

  struct mpool sh, * const ph = &sh;

#define T char
#include "./test0.inc"

#define T sint
#include "./test0.inc"

#define T int
#include "./test0.inc"

#define T long
#include "./test0.inc"

#define T void*
#include "./test0.inc"

#define T intp
#include "./test0.inc"


#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define T llong
#include "./test0.inc"
#endif
#ifdef UINT8_MAX
#define T int8
#include "./test0.inc"
#endif
#ifdef UINT16_MAX
#define T int16
#include "./test0.inc"
#endif
#ifdef UINT32_MAX
#define T int32
#include "./test0.inc"
#endif
#ifdef UINT64_MAX
#define T int64
#include "./test0.inc"
#endif


#define T float
#include "./test0.inc"

#define T double
#include "./test0.inc"

#define T long double
#include "./test0.inc"


  return TRUE;
}

#include "./test1.inc"
#define N 13
#define X 7
static_assert(X/N*N != X, "");

static int testmemcmp(void *p, uint vsize, int vset) {

  void *p1;

  assert_ret((p1 = malloc(vsize)) != NULL, FALSE);
  memset(p1, vset, vsize);
  assert_ret(memcmp(p, p1, vsize) == 0, FALSE);
  free(p1);

  return TRUE;
}

int test1(void) {

  struct mpool sh, * const ph = &sh;


  assert_ret(mpinit_amount(ph, TESTTYPES_N) == 0, FALSE);

  {
    uint va;

    for (va = 0; va != TESTTYPES_N;
         mpinit_type(ph, va, gl_sizes[va], N), va++);
  }

  assert_ret(mpconstruct(ph) == 0, FALSE);


  {
    uint va, vb, vc; void *p;


    for (va = 0; va != TESTTYPES_N; va++) {

      for (vb = 0; vb != N; vb++) {

        assert_ret(mpleft(ph, va) == N-vb, FALSE);
        assert_ret((p = mpnew(ph, va)) != NULL, FALSE);
        assert_ret(((uintp)p & gl_align[va]-1) == 0, FALSE);
        memset(p, ((vb&1)?0xAA:0x55), gl_sizes[va]);

        assert_ret(testmemcmp(p, gl_sizes[va], ((vb&1)?0xAA:0x55)) == TRUE, FALSE);
      }

      assert_ret(mpleft(ph, va) == 0, FALSE);
    }

    for (va = 0; va != TESTTYPES_N; va++) {

      for (vb = 0; vb != N; vb++) {

        assert_ret(testmemcmp(mpinit_ptrof(ph, va, vb),
                              gl_sizes[va], ((vb&1)?0xAA:0x55)) == TRUE, FALSE);
      }

      for (vb = X % N, vc = 0; vc != N/2; vb = (vb + X) % N, vc++) {

        mpdel(ph, va, mpinit_ptrof(ph, va, vb));
        assert_ret(mpleft(ph, va) == 1+vc, FALSE);
      }

      for (vb = X % N, vc = 0; vc != N/2; vb = (vb + X) % N, vc++) {

        assert_ret((p = mpnew(ph, va)) != NULL, FALSE);
        assert_ret(mpleft(ph, va) == N/2-1-vc, FALSE);
        assert_ret(((uintp)p & gl_align[va]-1) == 0, FALSE);
        memset(p, ((vb&1)?0xAA:0x55), gl_sizes[va]);
      }

      for (vb = 0; vb != N; vb++) {

        assert_ret(testmemcmp(mpinit_ptrof(ph, va, vb),
                              gl_sizes[va], ((vb&1)?0xAA:0x55)) == TRUE, FALSE);
      }
    }

    for (va = 0; va != TESTTYPES_N; va++) {

      for (vb = 0; vb != N; vb++) {

        mpdel(ph, va, mpinit_ptrof(ph, va, vb));
        assert_ret(mpleft(ph, va) == vb+1, FALSE);
      }
    }
  }

  mpdestruct(ph);

  return TRUE;
}

/*------------------------------------------------------------------------------*/
