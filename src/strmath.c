/* ------------------------------------------------------------------------- */

#include "./strmath.h"

#include "./lib/assert.h"

#include <string.h> /* memcpy() */

/* ------------------------------------------------------------------------- */

#ifndef NDEBUG

void sm_assert(const struct string *pa);

#endif

#include "./strmath/misc.c"

/*
  pms* - pointer to most significant digit
  pls* - pointer to less significant digit
  *len - длины строк, ни в одной функции не могут быть 0
*/


/*
  сложить pls_a и pls_b с результатом в pls_a. valen >= vblen. pls_a и pls_b
  могут указывать на один участок памяти - тогда операция будет иметь вид: pls_a
  * 2

  возврат - перенос в новый разряд (0 или 1).
*/
static uchar add(char *pls_a, uintp valen, const char *pls_b, uintp vblen);


static uintp sub_from_eqlarger(const char *pls_max, const char *pls_min,
                               uintp vminlen, char *pls_res);

/*
  сократить лидирующие нули.

  pms  - старший разряд числа
  plen - длина числа. на возврате меняется к длине числа без нулей (может стать
  0 - если весь pms состоит из нулей)
*/
static void reduce_lead_zeros(const char *pms, uintp *plen);

/*
  умножить pls_b на vdigit ('0'...'9') и сложить с pls_a. pls_a и pls_b могут
  указывать на один участок памяти - тогда операция будет иметь вид: pls_a *
  (vdigit + 1)

  pls_a - должен быть способен вместить результат (vblen+1 ?)
*/
static void muladd_by_digit(char *pls_a, const char *pls_b, uintp vblen,
                            uchar vdigit);

/*
  умножить pls_a и pls_b (которые могут указывать на один участок памяти), и
  сложить с pls_c. естественно, pls_c не может пересекаться с остальными
  указателями.

  pls_c - должен быть способен вместить результат (valen + vblen ?)
*/
static void muladd(const char *pls_a, uintp valen, const char *pls_b,
                   uintp vblen, char *pls_c);

/*
  разделить pms_a на vdigit ('0'...'9') и поместить результат в pms_a

  возврат - остаток
*/
static uchar div_by_digit(char *pms_a, uintp valen, uchar vdigit);

static void div(char *pas, uintp *palen, const char *pbe, uintp vblen,
                char *pce, uintp *pclen);

static schar cmp(const char *pae, uintp valen, const char *pbe, uintp vblen);

static void zeros(char *pae, uintp valen);

#include "./strmath/lowlevel.c"


static int add_u(struct sm_head *ph,
                 struct string *pa, uintp vsigna,
                 const struct string *pb, uintp vsignb);

static int sub_u(struct sm_head *ph,
                 struct string *pa, uintp vsigna,
                 const struct string *pb, uintp vsignb);

static int mul_u(struct sm_head *ph,
                 const struct string *pa, uintp vsigna,
                 const struct string *pb, uintp vsignb,
                 struct string *pc, uintp vsignc);

static int div_u(struct sm_head *ph,
                 struct string *pa, uintp vsigna,
                 const struct string *pb, uintp vsignb,
                 struct string *pc, uintp vsignc);

#include "./strmath/unsigned.c"

/* ------------------------------------------------------------------------- */


void sm_init(struct sm_head *ph, void *pmh,
             int (*stralloc)(void*, struct string*, uintp),
             int (*strrealloc_r)(void*, struct string*, uintp),
             int (*strrealloc_l)(void*, struct string*, uintp),
             void (*strfree)(void*, struct string*)) {

  assert(ph != NULL);
  assert(stralloc != NULL);
  assert(strrealloc_r != NULL);
  assert(strrealloc_l != NULL);
  assert(strfree != NULL);


  ph->pmh = pmh;
  ph->stralloc = stralloc;
  ph->strrealloc_r = strrealloc_r;
  ph->strrealloc_l = strrealloc_l;
  ph->strfree = strfree;

  return;
}


int sm_add(struct sm_head *ph, struct string *pa, const struct string *pb) {

  assert(ph != NULL);
  sm_assert(pa);
  sm_assert(pb);


  switch ((pa->pstr[0] == '-') | (pb->pstr[0] == '-') << 1) {

  case 0:
    return add_u(ph, pa, 0, pb, 0);

  case 1:
    return sub_u(ph, pa, 1, pb, 0);

  case 2:
    return sub_u(ph, pa, 0, pb, 1);
  default:
  case 3:
    return add_u(ph, pa, 1, pb, 1);
  }
}

int sm_sub(struct sm_head *ph, struct string *pa, const struct string *pb) {

  assert(ph != NULL);
  sm_assert(pa);
  sm_assert(pb);


  switch ((pa->pstr[0] == '-') | (pb->pstr[0] == '-') << 1) {

  case 0:
    return sub_u(ph, pa, 0, pb, 0);

  case 1:
    return add_u(ph, pa, 1, pb, 0);

  case 2:
    return add_u(ph, pa, 0, pb, 1);
  default:
  case 3:
    return sub_u(ph, pa, 1, pb, 1);
  }
}

int sm_mul(struct sm_head *ph, const struct string *pa,
           const struct string *pb, struct string *pc) {

  assert(ph != NULL);
  sm_assert(pa);
  sm_assert(pb);


  switch ((pa->pstr[0] == '-') | (pb->pstr[0] == '-') << 1) {

  case 0:
    return mul_u(ph, pa, 0, pb, 0, pc, 0);

  case 1:
    return mul_u(ph, pa, 1, pb, 0, pc, 1);

  case 2:
    return mul_u(ph, pa, 0, pb, 1, pc, 1);
  default:
  case 3:
    return mul_u(ph, pa, 1, pb, 1, pc, 0);
  }
}

int sm_div(struct sm_head *ph, struct string *pa, const struct string *pb,
           struct string *pc) {

  assert(ph != NULL);
  sm_assert(pa);
  sm_assert(pb);
  assert(pb->vlen > 1 || *pb->pstr != '0');


  switch ((pa->pstr[0] == '-') | (pb->pstr[0] == '-') << 1) {

  case 0:
    return div_u(ph, pa, 0, pb, 0, pc, 0);

  case 1:
    return div_u(ph, pa, 1, pb, 0, pc, 1);

  case 2:
    return div_u(ph, pa, 0, pb, 1, pc, 1);
  default:
  case 3:
    return div_u(ph, pa, 1, pb, 1, pc, 0);
  }
}

schar sm_cmp(struct sm_head *ph, const struct string *pa,
             const struct string *pb) {

  assert(ph != NULL);
  sm_assert(pa);
  sm_assert(pb);
  UNUSED(ph);


  switch ((pa->pstr[0] == '-') | (pb->pstr[0] == '-') << 1) {

  case 0:
    return cmp(pa->pstr, pa->vlen, pb->pstr, pb->vlen);

  case 1:
    return -1;

  case 2:
    return +1;

  default:
  case 3:
    return -cmp(pa->pstr+1, pa->vlen-1, pb->pstr+1, pb->vlen-1);
  }
}

int sm_not(struct sm_head *ph, struct string *pa) {

  assert(ph != NULL);
  sm_assert(pa);


  if (pa->vlen == 1 && pa->pstr[0] == '0') {

    return 0;
  }

  if (pa->pstr[0] == '-') {

    if (ph->strrealloc_l(ph->pmh, pa, pa->vlen-1)) {

      return 1;
    }

  } else {

    if (ph->strrealloc_l(ph->pmh, pa, pa->vlen+1)) {

      return 1;
    }
    pa->pstr[0] = '-';
  }

  return 0;
}

/*
  e^(log(x)/2) = sqrt(x) = y^(log(x)/log(y)/2)

  (10x+y)^2 = (10x)^2 + 10xy*2 + y^2 = (10x)^2 + (10x*2 + y)y

  100     10*10      10^2
  10000   100*100    10^4
  1000000 1000*1000  10^6


  поиск квадрата
  y^2 = a(y) = a(y-1)*2+2

  (x10 + y)y ~= (x10 + 1 + x10 + 3 + x10 + 5 ... y)


  5,67,89 | 238
  1
  3
  ---
  1,67
    41
    43
    45
    --
    38,89
     4,61
     4,63
     4,65
     ....
     ----
     1,45,xx
*/

int sm_sqrt(struct sm_head *ph, struct string *pa, struct string *pr) {

  uintp valen, vsigna;
  struct string ss, * const ps = &ss;

  assert(ph != NULL);
  sm_assert(pa);


  vsigna = (pa->pstr[0] == '-');
  valen = pa->vlen - vsigna;
  pa->pstr += vsigna;


  /* результат. временно размером с входящим. растёт слева направо. */
  if (ph->stralloc(ph->pmh, pr, valen)) {

    return 1;
  }

  /* вычитатель. размером с входящим. растёт справа налево. */
  if (ph->stralloc(ph->pmh, ps, valen)) {

    ph->strfree(ph->pmh, pr);
    return 1;
  }

  {
    uintp   vpoint = valen - 1 & ~(uintp)1, /* позиция во входящем для
                                               вычитателя */
            vremlen = valen - vpoint, /* тукущий размер вычитаемого */
            vreslen = 0, /* длина результата */
            vsublen;

    for ( ; ; ) {

      /* подготовка вычитателя для поиска следующей цифры: s = 10*r*2 s растёт
         справа на лево */

      memcpy(&ps->pstr[valen-vreslen-1], &pr->pstr[0], vreslen);
      ps->pstr[valen-1] = '0';
      vsublen = vreslen+1;

      if (add(&ps->pstr[valen-1], vsublen, &ps->pstr[valen-1], vsublen)) {

        /* будет срабатывать каждый раз если, если первый разряд результата
           окажется >= 5 */

        ps->pstr[valen - ++vsublen] = '1';
      }

      ps->pstr[valen-1] = '1';


      /* поиск следующей цифры
         (s10 + dig)dig ~= (s10 + 1 + s10 + 3 + s10 + 5 ... dig) */

      {
        uchar vdig;

        for (vdig = 0; ; vdig++) {

          /* сравнение -> вычитание */

          if (cmp(&pa->pstr[valen-vpoint-vremlen], vremlen,
                  &ps->pstr[valen-vsublen], vsublen) < 0) {

            break;
          }

          sub_from_eqlarger(&pa->pstr[valen-1-vpoint],
                            &ps->pstr[valen-1], vsublen,
                            &pa->pstr[valen-1-vpoint]);
          reduce_lead_zeros(&pa->pstr[valen-vpoint-vremlen], &vremlen);

          if (add(&ps->pstr[valen-1], vsublen, "2", 1)) {

            /* может стработать только на первом разряде */

            ps->pstr[valen - ++vsublen] = '1';
          }
        }

        /* расширение результата  */
        pr->pstr[vreslen++] = '0' + vdig;
      }


      if (vpoint == 0) {

        if (vremlen == 0) { vremlen = 1; } /* в случае ровного ответа a=0 */

        pa->pstr -= vsigna;
        ph->strfree(ph->pmh, ps);

        if (ph->strrealloc_l(ph->pmh, pa, vremlen)) {

          ph->strfree(ph->pmh, pr);
          return 1;
        }
        if (ph->strrealloc_r(ph->pmh, pr, vreslen)) {

          ph->strfree(ph->pmh, pr);
          return 1;
        }

        return 0;
      }

      vremlen += 2;
      vpoint -= 2;

    }
  }
}

/*

*/

void sm_root(void) {

  return;
}

#if 0
/*
  a - основание
  b - вводная степень (на выходе равняется или 0 или 1)
  c - остаточная степень

  r - результат
  t - временный


  r = a
  c = 0
  p = 1

  l0:
  если b < 2, то выход

  t = r * r; r = t
  c += (b % 2) * p
  b /= 2
  p *= 2
  goto l0
*/

/*
  pls_a - основание
  pms_b - степень (на выходе равняется или 0 или 1)
  pls_c - остаточная степень. формируется сложением с вводным числом. должна
  мочь вместить в себя, соотвецтвенно остаток (vblen ?)

  pls_r - результат. должен быть способен вместить (floor(log10(pls_a)+1) *
  pms_b ?)
  pls_t - буфер. должен быть размером с pls_r. на возврате значение не
  определено
*/
void pow_sqrt(const char *pls_a, uintp valen, char *pms_b, uintp vblen,

              char *pls_c
              char *pls_r, char *pls_t) {


  muladd(t, tlen, t, tlen, t2);
  t=t2;

  div_by_digit(char *pms_a, uintp valen, uchar vdigit);

  if (div_by_digit(b, blen, 2)) {

    add(c, clen, p, plen);
  }

  add(p, plen, p, plen);
}

void pow_sqrt(const char *pls_a, const char *pls_min,
              uintp vminlen, char *pls_res) {

}

void sm_pow(struct sm_head *ph, const struct string *pa, struct string *pb,
            struct string *pr) {

  struct string sm, * const pm = &sm;

  assert(ph != NULL);
  sm_assert(pa);
  sm_assert(pb);



  ph->stralloc(ph->pmh, pr, 1);
  pr->pstr[0] = '1';


  if (pb->pstr[0] == '-') {



  } else {

    /*  */

    pow_sqrt(pa, pb, pc);

    pow_line();


    uintp vreslenmax;

    /* максимальный (!!! или больше) размер результата */

    if (string_to_uintp(pb, &vreslenmax) ||
        ofchk_mul_uintp(vreslenmax, pa->vlen)) {

      /* детализация не имеет значения. всё одно - out of memory. как не
         крути. */
      return 1;
    }

    vreslenmax *= pa->vlen;

    ph->strrealloc_l(ph->pmh, pa, vremlen);



    ph->stralloc(ph->pmh, pr, pa->vlen );
    ph->stralloc(ph->pmh, pm, 1);


    mul(&pm->pstr[pm->vlen-1], pm->vlen, &pa->pstr[pa->vlen-1], pa->vlen,
        &pr->pstr[0], &vclen);

    for () {

      mul_u(ph, pm, 0, pa, 0, pr, 0);

      sub_from_eqlarger(const char *pmax, const char *pmin, uintp vminlen,
                              char *pas, char *pae, uintp *palen);
    }
  }

  return;
}
#endif

/* ------------------------------------------------------------------------- */
