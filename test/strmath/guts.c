/* ------------------------------------------------------------------------- */

#include "./common.h"


static int strcmp_special(const char *pa, const struct string *pb) {

  if (pb->vlen != strlen(pa) || memcmp(pb->pstr, pa, pb->vlen) != 0) {

    return 1;
  }

  return 0;
}

void test_init_(struct test_head *ph) {

  memstat_init(&ph->smh);

  sm_init(&ph->strmh, &ph->smh,
          some_stralloc,
          some_strrealloc_r,
          some_strrealloc_l,
          some_strfree);

  return;
}

int test_sum_(struct test_head *ph, const char *psa, const char *psb,
              const char *psc, int (*sm_x)(struct sm_head*,
                                           struct string*,
                                           const struct string*)) {

  struct string sa, sb;

  assert_ret(make_string(&ph->smh, &sa, psa) == 0, FALSE);
  assert_ret(make_string(&ph->smh, &sb, psb) == 0, FALSE);

  assert_ret(sm_x(&ph->strmh, &sa, &sb) == 0, FALSE);

  assert_ret(strcmp_special(psb, &sb) == 0, FALSE);
  assert_ret(strcmp_special(psc, &sa) == 0, FALSE);

  some_strfree(&ph->smh, &sa);
  some_strfree(&ph->smh, &sb);

  assert_ret(memstat_isclear(&ph->smh) != 0, FALSE);

  return TRUE;
}

int test_mul_(struct test_head *ph, const char *psa, const char *psb,
              const char *psc) {

  struct string sa, sb, sc;

  assert_ret(make_string(&ph->smh, &sa, psa) == 0, FALSE);
  assert_ret(make_string(&ph->smh, &sb, psb) == 0, FALSE);

  assert_ret(sm_mul(&ph->strmh, &sa, &sb, &sc) == 0, FALSE);

  assert_ret(strcmp_special(psa, &sa) == 0, FALSE);
  assert_ret(strcmp_special(psb, &sb) == 0, FALSE);
  assert_ret(strcmp_special(psc, &sc) == 0, FALSE);

  some_strfree(&ph->smh, &sa);
  some_strfree(&ph->smh, &sb);
  some_strfree(&ph->smh, &sc);

  assert_ret(memstat_isclear(&ph->smh) != 0, FALSE);

  return TRUE;
}

int test_div_(struct test_head *ph, const char *psa, const char *psb,
              const char *psc, const char *psd) {

  struct string sa, sb, sc;

  assert_ret(make_string(&ph->smh, &sa, psa) == 0, FALSE);
  assert_ret(make_string(&ph->smh, &sb, psb) == 0, FALSE);

  assert_ret(sm_div(&ph->strmh, &sa, &sb, &sc) == 0, FALSE);

  assert_ret(strcmp_special(psd, &sa) == 0, FALSE);
  assert_ret(strcmp_special(psb, &sb) == 0, FALSE);
  assert_ret(strcmp_special(psc, &sc) == 0, FALSE);

  some_strfree(&ph->smh, &sa);
  some_strfree(&ph->smh, &sb);
  some_strfree(&ph->smh, &sc);

  assert_ret(memstat_isclear(&ph->smh) != 0, FALSE);

  return TRUE;
}

int test_cmp_(struct test_head *ph, const char *psa, const char *psb,
              schar vc) {

  struct string sa, sb;

  assert_ret(make_string(&ph->smh, &sa, psa) == 0, FALSE);
  assert_ret(make_string(&ph->smh, &sb, psb) == 0, FALSE);

  assert_ret(sm_cmp(&ph->strmh, &sa, &sb) == vc, FALSE);

  some_strfree(&ph->smh, &sa);
  some_strfree(&ph->smh, &sb);

  assert_ret(memstat_isclear(&ph->smh) != 0, FALSE);

  return TRUE;
}

int test_not_(struct test_head *ph, const char *psa, const char *psb) {

  struct string sa;

  assert_ret(make_string(&ph->smh, &sa, psa) == 0, FALSE);

  assert_ret(sm_not(&ph->strmh, &sa) == 0, FALSE);

  assert_ret(strcmp_special(psb, &sa) == 0, FALSE);

  some_strfree(&ph->smh, &sa);

  assert_ret(memstat_isclear(&ph->smh) != 0, FALSE);

  return TRUE;
}

int test_sqrt_(struct test_head *ph, const char *psin, const char *psret,
               const char *psrem) {

  struct string sa, sr;

  assert_ret(make_string(&ph->smh, &sa, psin) == 0, FALSE);
  assert_ret(sm_sqrt(&ph->strmh, &sa, &sr) == 0, FALSE);

  assert_ret(strcmp_special(psret, &sr) == 0, FALSE);
  assert_ret(strcmp_special(psrem, &sa) == 0, FALSE);

  some_strfree(&ph->smh, &sa);
  some_strfree(&ph->smh, &sr);

  assert_ret(memstat_isclear(&ph->smh) != 0, FALSE);

  return TRUE;
}


/* ------------------------------------------------------------------------- */
