/* ------------------------------------------------------------------------- */

#include "./fio_part.h"

#include "./lib/assert.h"

#include <stdlib.h>

struct part* part_new_last(struct dlist_head *ph, uint vpmode, uintp vsize) {

  struct part *pnew;
  char *pdata;


  if (vsize > (INTPTR_MAX - CHAR_BIT + 1) / CHAR_BIT ||
      (pdata = malloc(vsize + (ALIGNOF(struct part) - 1)
                      + sizeof(struct part))) == NULL) {

    return NULL;
  }


  pnew = (void*)((uintp)pdata + vsize + (ALIGNOF(struct part) - 1)
                 & ~(ALIGNOF(struct part) - 1));

  if ((vpmode & BM_ORIENT) == BM_IO) {

    pnew->sadrf = bitptr(pdata, vpmode);

  } else {

    pnew->sadrf = bitptr(pdata + (vsize-1), vpmode);
  }

  pnew->sadrl = bitptr_add(pnew->sadrf, vpmode, vsize * CHAR_BIT);

  dlist_addl(ph, &pnew->snode);

  return pnew;
}

struct part* part_free_first(struct dlist_head *ph, uint vpmode) {

  struct part *pcur, *pnext;

  assert(dlist_last(ph) != NULL);


  pcur = container_of((void*)dlist_first(ph), struct part, snode);
  pnext = (void*)dlist_tol(ph, &pcur->snode);
  dlist_del(ph, &pcur->snode);

  if ((vpmode & BM_ORIENT) == BM_IO) {

    free(bitptr_getbyte(pcur->sadrf));

  } else {

    free(bitptr_getbyte(bitptr_sub(pcur->sadrl, vpmode, 1)));
  }

  if (pnext == NULL) {

    return NULL;

  } else {

    return container_of(pnext, struct part, snode);
  }
}

void part_free(struct dlist_head *ph, uint vpmode, struct part *pcur) {


  dlist_del(ph, &pcur->snode);

  if ((vpmode & BM_ORIENT) == BM_IO) {

    free(bitptr_getbyte(pcur->sadrf));

  } else {

    free(bitptr_getbyte(bitptr_sub(pcur->sadrl, vpmode, 1)));
  }

  return;
}

struct part* part_get_first(struct dlist_head *ph) {

  struct part *pcur = (void*)dlist_first(ph);

  if (pcur == NULL) {

    return NULL;

  } else {

    return container_of(pcur, struct part, snode);
  }
}

struct part* part_get_last(struct dlist_head *ph) {

  struct part *pcur = (void*)dlist_last(ph);

  if (pcur == NULL) {

    return NULL;

  } else {

    return container_of(pcur, struct part, snode);
  }
}

void* part_get_ptrorig(uint vpmode, struct part *pcur) {


  if ((vpmode & BM_ORIENT) == BM_IO) {

    return bitptr_getbyte(pcur->sadrf);

  } else {

    return bitptr_getbyte(bitptr_sub(pcur->sadrl, vpmode, 1));
  }
}

struct part* part_next(struct dlist_head *ph, struct part *pcur) {

  struct part *pnext;

  assert(ph != NULL);
  assert(pcur != NULL);


  pnext = (void*)dlist_tol(ph, &pcur->snode);

  if (pnext == NULL) {

    return NULL;
  }

  return container_of(pnext, struct part, snode);
}

/* ------------------------------------------------------------------------- */
