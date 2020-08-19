/* ------------------------------------------------------------------------- */

#include "./lib/types.h"

#ifndef STRING_T
#define STRING_T
struct string {

  char *pstr;
  uintp vlen;
};
#endif

#ifndef NDEBUG

void sm_assert(const struct string *pa);

#else

#define sm_assert(ARG) ((void)ARG)

#endif

struct sm_head {

  void *pmh;
  int (*stralloc)(void*, struct string*, uintp);
  int (*strrealloc_r)(void*, struct string*, uintp);
  int (*strrealloc_l)(void*, struct string*, uintp);
  void (*strfree)(void*, struct string*);
};

void sm_init(struct sm_head *ph, void *pmh,
             int (*stralloc)(void*, struct string*, uintp),
             int (*strrealloc_r)(void*, struct string*, uintp),
             int (*strrealloc_l)(void*, struct string*, uintp),
             void (*strfree)(void*, struct string*));


/*
  так как интерфейс функций исходит из оптимальности реализации. sm_add, sm_sub
  - не нужен промежуточный буфер, поэтому возврат - модификация одиного из
  аргументов - pa.

  при неуспешном возарате, значение pa не определено.

  возврат 0 - успешно
*/

int sm_add(struct sm_head *ph, struct string *pa, const struct string *pb);

int sm_sub(struct sm_head *ph, struct string *pa, const struct string *pb);


/*
  pc = pa * pb
  pc - при успешном возврате, результат. структура инициализируется функцией.

  возврат 0 - успешно
*/
int sm_mul(struct sm_head *ph, const struct string *pa,
           const struct string *pb, struct string *pc);


/*
  pc = pa / pb
  pa = pa % pb

  целочисленное деление. округление вниз. проверка на 0 не производится.

  pa - возвращается остаток такой, чтобы pa/pb*pb + pa%pb = pa. при неуспешном
  .    возарате, значение pa не определено.
  pb - делитель.
  pc - при успешном возврате, результат. структура инициализируется функцией.

  возврат 0 - успешно
*/
int sm_div(struct sm_head *ph, struct string *pa, const struct string *pb,
           struct string *pc);


/*
  -1 a меньше b
  +1 a больше b
  0  a равно b
*/
schar sm_cmp(struct sm_head *ph, const struct string *pa,
             const struct string *pb);

/*
  -a

  при неуспешном возарате, значение pa не определено.

  возврат 0 - успешно
*/
int sm_not(struct sm_head *ph, struct string *pa);


#if 0 /* функций нет */
/* real numbers with fixed pointer */

/*  a * b / d = c  */

void sm_realmul(const struct string *pa, const struct string *pb, struct string *pc,
                const struct string *pd,
                void (*stralloc)(void*, struct string*, uintp),
                void (*strrealloc_l)(void*, struct string*, uintp), void *ptr);

/*  a * d / b = c
    a * d % b = a  */

void sm_realdiv(struct string *pa, const struct string *pb, struct string *pc,
                const struct string *pd,
                void (*stralloc)(void*, struct string*, uintp),
                void (*strrealloc_l)(void*, struct string*, uintp), void *ptr);

#endif

/*
  .        pr   pa
  sqrt(0) = 0   0
  sqrt(1) = 1   0 = sqrt(-1)
  sqrt(2) = 1   1 = sqrt(-2)

  pa - входящее. на возврате остаток. при неуспешном возарате, значение не
  .    определено.
  pr - при успешном возврате, результат. структура инициализируется функцией.

  возврат 0 - успешно
*/
int sm_sqrt(struct sm_head *ph, struct string *pa, struct string *pr);


#if 0 /* функции нет */
/*
  pow(x, y) = 1*x*x ...
  pow(x,-y) = 1/x/x ...
  pow(x,0) = 1
  pow(0,x) = 0

  pa = pa^pb
*/
void sm_pow(struct sm_head *ph, const struct string *pa, struct string *pb,
            struct string *pr);

#endif

/* ------------------------------------------------------------------------- */
