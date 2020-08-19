/*---------------------------------------------------------------------------*/

#include "./mpool.h"
#include "./ofchk.h"
#include "./assert.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/*---------------------------------------------------------------------------*/

int mpinit_amount(struct mpool *ph, uint vtypes) {

  assert(ph != NULL);
  assert(vtypes > 0);


  ph->vtypes = vtypes;

  if ((ph->ptypes = malloc(vtypes * sizeof(struct mp_type))) == NULL) {

    return 1;
  }

#ifndef NDEBUG
  memset(ph->ptypes, 0, vtypes * sizeof(struct mp_type));
#endif

  return 0;
}

void mpinit_type(struct mpool *ph, uint vtype, uintp vsize, uintp vlimit) {

  struct mp_type *pt = &ph->ptypes[vtype];

  assert(ph != NULL);
  assert(vtype < ph->vtypes);
  assert(vsize > 0);
  assert(vlimit > 0);

  pt->vsize = vsize;
  pt->vlimit = vlimit;
  return;
}

int mpinit(struct mpool *ph, uint vtypes, uintp vsize, uintp vlimit, ...) {

  va_list args;


  if (mpinit_amount(ph, vtypes)) {

    return 1;
  }

  va_start(args, vlimit);

  {
    uintp va;

    for (va = 0; ; ) {

      mpinit_type(ph, va, vsize, vlimit);

      if (++va == vtypes) { break; }

      vsize = va_arg(args, uintp);
      vlimit = va_arg(args, uintp);
    }
  }

  va_end(args);
  return 0;
}

int mpconstruct(struct mpool *ph) {

  uint vt; struct mp_type *pt;

  assert(ph != NULL);


  for (pt = ph->ptypes, vt = ph->vtypes; ; pt++) {

    assert(pt->vsize != 0); /* тип неинициализирован */

    {
      uintp vtotal;

      if (ofchk_add_uintp(pt->vsize, sizeof(void*)) ||
          ofchk_mul_uintp(pt->vlimit, vtotal = pt->vsize + sizeof(void*)) ||
          ofchk_add_uintp(vtotal *= pt->vlimit, sizeof(void*)-1) ||
          (pt->pbase = malloc(vtotal + (sizeof(void*)-1))) == NULL) {

        return 1;
      }
    }

    pt->vleft = pt->vlimit;
    pt->ptr_stack_top = (void**)((uintp)pt->pbase + pt->vlimit * pt->vsize
                                 + (sizeof(void*)-1) & ~(sizeof(void*)-1));

    {
      uintp va = pt->vlimit;
      char *pbuf = (char*)pt->pbase + pt->vsize * va;

      for ( ; *pt->ptr_stack_top++ = (pbuf -= pt->vsize), --va; );
    }

    if (--vt == 0) { return 0; }
  }
}

void mpdestruct(struct mpool *ph) {

  uint vt; struct mp_type *pt;

  assert(ph != NULL);


  for (pt = ph->ptypes, vt = ph->vtypes; free(pt->pbase), pt++, --vt; );
  free(ph->ptypes);
  return;
}

void* mpinit_ptrof(struct mpool *ph, uint vtype, uintp vindex) {

  struct mp_type *pt = &ph->ptypes[vtype];

  assert(ph != NULL);
  assert(vtype < ph->vtypes);
  assert(vindex < pt->vlimit);


  return (void*)((char*)pt->pbase + pt->vsize * vindex);
}

void* mpnew(struct mpool *ph, uint vtype) {

  struct mp_type *pt = &ph->ptypes[vtype];

  assert(ph != NULL);
  assert(vtype < ph->vtypes);


  if (!pt->vleft) {

    return NULL;
  }

  pt->vleft--;
  return *--pt->ptr_stack_top;
}

void mpdel(struct mpool *ph, uint vtype, void *ptr) {

  struct mp_type *pt = &ph->ptypes[vtype];

  assert(ph != NULL);
  assert(ptr != NULL);
  assert(vtype < ph->vtypes);


  pt->vleft++;
  *pt->ptr_stack_top++ = ptr;
  return;
}

uintp mpleft(struct mpool *ph, uint vtype) {

  struct mp_type *pt = &ph->ptypes[vtype];

  assert(ph != NULL);
  assert(vtype < ph->vtypes);


  return pt->vleft;
}

/*---------------------------------------------------------------------------*/
