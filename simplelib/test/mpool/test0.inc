/* ------------------------------------------------------------------------- *
 * шаблон теста типа <T> в пуле минимальной конфигурации                     *
 * ------------------------------------------------------------------------- */

{
  assert_ret(mpinit(ph, 1, sizeof(T), 1) == 0, FALSE);
  assert_ret(mpconstruct(ph) == 0, FALSE);
  assert_ret(mpleft(ph, 0) == 1, FALSE);


  {
    void *p, *p1;

    assert_ret((p = mpnew(ph, 0)) != NULL, FALSE);
    assert_ret(mpleft(ph, 0) == 0, FALSE);
    assert_ret(((uintp)p & ALIGNOF(T)-1) == 0, FALSE);
    memset(p, 0xAA, sizeof(T));

    assert_ret((p1 = malloc(sizeof(T))) != NULL, FALSE);
    memset(p1, 0xAA, sizeof(T));
    assert_ret(memcmp(p, p1, sizeof(T)) == 0, FALSE);

    free(p1);
    mpdel(ph, 0, p);
  }

  assert_ret(mpleft(ph, 0) == 1, FALSE);
  mpdestruct(ph);
}

#undef T
