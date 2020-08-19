/*------------------------------------------------------------------------------*/

#include "./tests.h"

int every_modes(int (*t)(uint vmode)) {


  assert_ret(t(BM_IO|BM_LSF) == TRUE, FALSE);
  assert_ret(t(BM_DO|BM_LSF) == TRUE, FALSE);
  assert_ret(t(BM_IO|BM_MSF) == TRUE, FALSE);
  assert_ret(t(BM_DO|BM_MSF) == TRUE, FALSE);

  return TRUE;
}

int every_modes2(int (*t)(uint vbmode, uint vamode)) {


  assert_ret(t(BM_IO|BM_LSF, BM_IO|BM_LSF) == TRUE, FALSE);
  assert_ret(t(BM_DO|BM_LSF, BM_IO|BM_LSF) == TRUE, FALSE);
  assert_ret(t(BM_IO|BM_MSF, BM_IO|BM_LSF) == TRUE, FALSE);
  assert_ret(t(BM_DO|BM_MSF, BM_IO|BM_LSF) == TRUE, FALSE);

  assert_ret(t(BM_IO|BM_LSF, BM_DO|BM_LSF) == TRUE, FALSE);
  assert_ret(t(BM_DO|BM_LSF, BM_DO|BM_LSF) == TRUE, FALSE);
  assert_ret(t(BM_IO|BM_MSF, BM_DO|BM_LSF) == TRUE, FALSE);
  assert_ret(t(BM_DO|BM_MSF, BM_DO|BM_LSF) == TRUE, FALSE);

  assert_ret(t(BM_IO|BM_LSF, BM_IO|BM_MSF) == TRUE, FALSE);
  assert_ret(t(BM_DO|BM_LSF, BM_IO|BM_MSF) == TRUE, FALSE);
  assert_ret(t(BM_IO|BM_MSF, BM_IO|BM_MSF) == TRUE, FALSE);
  assert_ret(t(BM_DO|BM_MSF, BM_IO|BM_MSF) == TRUE, FALSE);

  assert_ret(t(BM_IO|BM_LSF, BM_DO|BM_MSF) == TRUE, FALSE);
  assert_ret(t(BM_DO|BM_LSF, BM_DO|BM_MSF) == TRUE, FALSE);
  assert_ret(t(BM_IO|BM_MSF, BM_DO|BM_MSF) == TRUE, FALSE);
  assert_ret(t(BM_DO|BM_MSF, BM_DO|BM_MSF) == TRUE, FALSE);

  return TRUE;
}

int main(void) {


  assert(every_modes(test0) == TRUE);
  assert(every_modes2(test1) == TRUE);
  assert(test2() == TRUE);
  assert(every_modes(test3) == TRUE);

  assert(every_modes2(test5) == TRUE);

  return TRUE;
}

/*------------------------------------------------------------------------------*/
