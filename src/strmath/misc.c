/* ------------------------------------------------------------------------- */

#define MIN(A, B) (((A)<(B))?(A):(B))

#ifndef NDEBUG

void sm_assert(const struct string *pa) {

  uintp va = 0;

  assert(pa != NULL);
  assert(pa->vlen > 0);

  if (pa->pstr[0] == '-') {

    assert(pa->vlen > 1 && pa->pstr[1] != '0');
    va++;

  } else {

    assert(pa->vlen == 1 || pa->pstr[0] != '0');
  }

  {
    for ( ; va != pa->vlen;
          assert(pa->pstr[va] <= '9' && pa->pstr[va] >= '0'), va++);
  }

  return;
}

#endif

/* ------------------------------------------------------------------------- */
