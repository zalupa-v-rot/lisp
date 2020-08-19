/* ------------------------------------------------------------------------- *
 * шаблон теста функций FSET FGET типа T                                     *
 * ------------------------------------------------------------------------- */

{
  uchar *pbuf, *p;

  assert_ret((p = pbuf = malloc(sizeof(T) * (BITSOF(T)-1))) != NULL,
             FALSE);


  if ((vmode & BM_BYTE) == BM_DO) {

    p += sizeof(T) * (BITSOF(T)-1) - 1;
  }

  {
    T vn0, vn1, vn2; int va;


    for (vn1 = vn0 = 0, va = 0; va != BITSOF(T)/2; va++) {

      vn0 |= (T)1 << va*2;
      vn1 |= (T)1 << va*2+1;
    }

    {
      bitptr_t sp, st;

      sp = bitptr(p, vmode);

      for (st = sp, va = 0; va != BITSOF(T); va++) {

        FSET(st, vmode, BITSOF(T)-1, ((va)?vn0:vn1));
        st = bitptr_add(st, vmode, BITSOF(T)-1);
      }

      for (st = sp, va = 0; va != BITSOF(T); va++) {

        vn2 = FGET(st, vmode, BITSOF(T)-1);
        st = bitptr_add(st, vmode, BITSOF(T)-1);

        assert_ret(vn2 == (((va)?vn0:vn1) & ~((T)1<<BITSOF(T)-1)), FALSE);
      }
    }
  }

  free(pbuf);
}

#undef FSET
#undef FGET
#undef T
