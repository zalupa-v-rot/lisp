/* ------------------------------------------------------------------------- */

static uchar add(char *pls_a, uintp valen, const char *pls_b, uintp vblen) {

  uchar vn, vc;

  assert(valen >= vblen);


  for (vc = 0; ; ) {

    vn = (uchar)*pls_a - '0' + (uchar)*pls_b - '0' + vc;
    vc = 0;

    if (vn >= 10) {

      vn -= 10, vc = 1;
    }

    *pls_a = vn + '0';

    pls_a--, pls_b--;

    if (--valen, --vblen == 0) { break; }
  }

  /* добивка переноса в случае если a больше b по разрядам */
  for ( ; valen != 0 && vc != 0; valen--) {

    vn = (uchar)*pls_a - '0' + vc;
    vc = 0;

    if (vn >= 10) {

      vn -= 10, vc = 1;
    }

    *pls_a-- = vn + '0';
  }

  return vc;
}

/*
  вычитание из большего или равного

  pls_max - младший разряд большего
  pls_min - младший разряд меньшего
  vminlen - длина младшего
  pls_res - младший разряд возвратного

  возврат - длина возвратной строки (может содержать одни нули)
*/
static uintp sub_from_eqlarger(const char *pls_max, const char *pls_min,
                               uintp vminlen, char *pls_res) {

  schar vn, vb;
  char *pls_res_ori = pls_res;


  for (vb = 0; ; ) {

    vn = ((uchar)*pls_max - '0') - ((uchar)*pls_min - '0') - vb;
    vb = 0;

    if (vn < 0) {

      vn += 10, vb = 1;
    }

    *pls_res-- = vn + '0';

    pls_max--, pls_min--;

    if (--vminlen == 0) { break; }
  }

  /* добивка заимствования в случае если a == max */
  for ( ; vb != 0; ) {

    vn = (uchar)*pls_max-- - '0' - vb;
    vb = 0;

    if (vn < 0) {

      vn += 10, vb = 1;
    }

    *pls_res-- = vn + '0';
  }

  return pls_res - pls_res_ori;
}

static void reduce_lead_zeros(const char *pms, uintp *plen) {

  uintp vlen = *plen;


  for ( ; *pms++ == '0' && --vlen > 0; );

  *plen = vlen;
  return;
}

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

static uchar mul_digit_by_digit(uchar va, uchar vb) {

  uchar vr = 0;

  if (vb & 1) { vr += va; }
  if (vb & 2) { vr += va<<1; }
  if (vb & 4) { vr += va<<2; }
  if (vb & 8) { vr += va<<3; }

  return vr;
}

/*
  с учётом что va <= 99.

  старшие 4 бита - результат
  младшие 4 бита - остаток
*/
static uchar div_by_10(uchar va) {

  uchar vr = 0;

  if (va >= 80) { va -= 80; vr += 8<<4; }
  if (va >= 40) { va -= 40; vr += 4<<4; }
  if (va >= 20) { va -= 20; vr += 2<<4; }
  if (va >= 10) { va -= 10; vr += 1<<4; }

  return vr | va;
}

static void muladd_by_digit(char *pls_a, const char *pls_b, uintp vblen,
                            uchar vdigit) {

  uchar vn, vcm, vca;

  assert((vdigit & 0x0f) < 10);


  /* умножение b и тутиже сложение с a */
  for (vcm = vca = 0; ; ) {

    vn = mul_digit_by_digit((uchar)*pls_b-- - '0', vdigit) + vcm;
    vcm = 0;

    vn = div_by_10(vn);
    vcm = vn >> 4, vn &= 0x0F;


    vn = ((uchar)*pls_a - '0') + vn + vca;
    vca = 0;

    if (vn >= 10) {

      vn -= 10, vca = 1;
    }

    *pls_a-- = vn + '0';

    if (--vblen == 0) { break; }
  }

  /* добивка переноса - a всегда избыточен по размеру */
  for (vca += vcm ; vca != 0; ) {

    vn = (uchar)*pls_a - '0' + vca;
    vca = 0;

    if (vn >= 10) {

      vn -= 10, vca = 1;
    }

    *pls_a-- = vn + '0';
  }

  return;
}

/* . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . */

static void muladd(const char *pls_a, uintp valen, const char *pls_b,
                   uintp vblen, char *pls_c) {

  for ( ; ; ) {

    muladd_by_digit(pls_c--, pls_a, valen, *pls_b--);

    if (--vblen == 0) {

      break;
    }
  }

  return;
}

static uchar div_by_digit(char *pms_a, uintp valen, uchar vdigit) {

  uchar vr;


  for (vdigit -= '0', vr = 0; ; ) {

    vc = (uchar)*pma_a - '0' + vr;
    *pma_a++ = '0' + vc / vdigit;
    vr = vc % vdigit * 10;

    if (--valen == 0) { break; }
  }

  return vr;
}

/*
  pas - делимое на вводе, на возврате остаток.
  palen - вводное и возвратное.

  pbe - делитель. не проверяется на равность 0. если 0, то это бесконечный цикл.

  pce - возврат результата. размер должен быть max(*palen + 1 - vblen, 1)
  pclen - только возвратное. результат: max(*palen + 1 - vblen, 0). 0 - если результат 0.
*/

static void div(char *pas, uintp *palen, const char *pbe, uintp vblen,
                char *pce, uintp *pclen) {

  uintp vshift, valen, vclen;
  char *pce2 = pce;


  if ((valen = *palen) < vblen) {

    *pce = '0';
    *pclen = 0;
    return;
  }

  vshift = valen - vblen;
  vclen = vshift + 1;

  {
    uchar vdig;
    const char *pbs = pbe + vblen - 1;

    for ( ; ; ) {

      /* это смещение или нижне на 1 */

      if (cmp(pas - valen + 1, valen-vshift, pbe, vblen) < 0) {

        *pce++ = '0';

        if (vshift == 0) { break; }
        vshift--;
      }

      /* определение цифры делителя вычитанием */

      for (vdig = 0; ; ) {

        sub_from_eqlarger(pas - vshift, pbs, vblen, pas - vshift);
        reduce_lead_zeros(pas - valen + 1, &valen);

        ++vdig;

        if (valen < vblen + vshift || /* разряд вычитаемого ужался */
            cmp(pas - valen + 1, valen-vshift, pbe, vblen) < 0) { /* значение стало меньше */
          break;
        }
      }

      *pce++ = '0' + vdig;

      if (vshift == 0) { break; }
      vshift--;


     /* отступ нулями до нового смещения */
      if (valen < vblen + vshift) {

        uintp va = vblen + vshift - valen;

        if (va > vshift) {

          for ( ; *pce++ = '0', vshift != 0; vshift--);
          break;

        } else {

          for (vshift -= va; va != 0; *pce++ = '0', va--);
        }
      }
    }
  }

  /* сокращение лидирующего нуля */
  if (*pce2 == '0') {

    vclen--;
  }

  *pclen = vclen;
  *palen = valen;
  return;
}


/*
  -1 a меньше b
  +1 a больше b
  0  a равно b
*/
static schar cmp(const char *pae, uintp valen, const char *pbe, uintp vblen) {

  if (valen < vblen) {

    return -1;

  } else
  if (valen > vblen) {

    return +1;

  } else {

    for ( ; ; pae++, pbe++) {

      if (*pae < *pbe) { return -1;
      } else
      if (*pae > *pbe) { return +1;
      }

      if (--valen == 0) {

        return 0;
      }
    }
  }
}


static void zeros(char *pae, uintp valen) {

  for ( ; valen != 0; *pae++ = '0', valen--);
  return;
}

/* ------------------------------------------------------------------------- */
