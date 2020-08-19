/*---------------------------------------------------------------------------*/

#include "./types.h"


struct mp_type {

  uintp   vsize,
          vlimit,
          vleft;

  void    *pbase,
          **ptr_stack_top;
};

struct mpool {

  uint    vtypes;

  struct mp_type
          *ptypes;
};

/*
  иницализировать всего типов
  vtypes > 0
  возврат != 0 - не успех
*/
int mpinit_amount(struct mpool *ph, uint vtypes);

/*
  иницализировать тип
  vsize > 0, vlimit > 0
*/
void mpinit_type(struct mpool *ph, uint vtype, uintp vsize, uintp vlimit);

/*
  иницализировать одной функцией
  vsize > 0, vlimit > 0
  возврат != 0 - не успех
*/
int mpinit(struct mpool *ph, uint vtypes, uintp vsize, uintp vlimit, ...);

/*
  построить
*/
int mpconstruct(struct mpool *ph);

/*
  разрушить
*/
void mpdestruct(struct mpool *ph);

/*
  указатель конкртной ячейки
*/
void* mpinit_ptrof(struct mpool *ph, uint vtype, uintp vindex);

/*
  выделение. принцип выделения-освобождения fifo. первоначально стек заполнен от
  старших до младших - то есть младшие первые на выделение.

  возврат == NULL - память кончилась
*/

void* mpnew(struct mpool *ph, uint vtype);

/*
  освобождение.
 */
void mpdel(struct mpool *ph, uint vtype, void *ptr);

/*
  осталось слотов
*/
uintp mpleft(struct mpool *ph, uint vtype);

/*---------------------------------------------------------------------------*/
