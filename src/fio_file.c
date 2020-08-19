/* ------------------------------------------------------------------------- */

#include "./fio_file.h"

#include "./lib/assert.h"


void file_init(struct file *ph) {


  dlist_init(&ph->slistparts);

  ph->vparts = 0;
  return;
}

struct part* file_lengthen(struct file *ph, uint vpmode, uintp vsize) {

  struct part *pnew;

  assert(ph != NULL);

  printf("+ %i\n", (int)vsize);
  if ((pnew = part_new_last(&ph->slistparts, vpmode, vsize)) == NULL) {

    return NULL;
  }

  if (++ph->vparts == 1) {

    ph->sreal_adrf = pnew->sadrf;
    ph->sreal_adrl = pnew->sadrl;

  } else {

    ph->sreal_adrl = pnew->sadrf;
  }

  return pnew;
}

int file_shorten(struct file *ph, uint vpmode) {

  struct part *pnext;

  assert(ph != NULL);


  if (ph->vparts == 0) {

    return 1;
  }

  ph->vparts--;

  if ((pnext = part_free_first(&ph->slistparts, vpmode)) == NULL) {

    return 1;
  }

  ph->sreal_adrf = pnext->sadrf;
  return 0;
}

int file_close(struct file *ph, uint vpmode,
               int (*pwrite)(struct file *ph, void *pdata, uintp vsize)) {

  char *pdatls, *pdatms; uintp vsize;
  struct part *pcur;


  for (vsize = 0; ; ) {

    if ((vpmode & BM_ORIENT) == BM_IO) {

      pcur = part_get_first(&ph->slistparts);

    } else {

      pcur = part_get_last(&ph->slistparts);
    }

    if (pcur == NULL) {

      return 0;
    }

    if (vsize) {

      if ((vpmode & BM_ORIENT) == BM_IO) {

        ph->sreal_adrf = pcur->sadrf;

      } else {

        ph->sreal_adrl = pcur->sadrl;
      }
    }

    if ((vpmode & BM_ORIENT) == BM_IO) {

      /* ls = floor(first), ms = ceil(last) */

      pdatls = bitptr_getbyte(ph->sreal_adrf);
      pdatms = bitptr_getbyte(bitptr_ceil(file_get_lastreal(ph, pcur), vpmode));

    } else {

      /* ls = floor(last-1), ms = floor(first)+1 */

      pdatls = bitptr_getbyte(bitptr_sub(ph->sreal_adrl, vpmode, 1));
      pdatms = (char*)bitptr_getbyte(file_get_firstreal(ph, pcur)) + 1;
    }

    vsize = pdatms - pdatls;

    if (pwrite(ph, pdatls, vsize)) {

      return 1;
    }

    part_free(&ph->slistparts, vpmode, pcur);
  }
}

void* file_get_ptrorig(uint vpmode, struct part *pcur) {

  return part_get_ptrorig(vpmode, pcur);
}

bitptr_t file_get_lastreal(struct file *ph, struct part *pcur) {

  assert(ph != NULL);
  assert(pcur != NULL);


  if (dlist_tol(&ph->slistparts, &pcur->snode) == NULL) {

    return ph->sreal_adrl;
  }

  return pcur->sadrl;
}

bitptr_t file_get_firstreal(struct file *ph, struct part *pcur) {

  assert(ph != NULL);
  assert(pcur != NULL);


  if (dlist_tof(&ph->slistparts, &pcur->snode) == NULL) {

    return ph->sreal_adrf;
  }

  return pcur->sadrf;
}

bitptr_t file_get_last(struct part *pcur) {

  assert(pcur != NULL);

  return pcur->sadrl;
}

bitptr_t file_get_first(struct part *pcur) {

  assert(pcur != NULL);

  return pcur->sadrf;
}

void file_set_lastreal(struct file *ph, bitptr_t sp) {

  assert(ph != NULL);

  ph->sreal_adrl = sp;
  return;
}

struct part* file_next_part(struct file *ph, struct part *pcur) {

  assert(ph != NULL);
  assert(pcur != NULL);


  return part_next(&ph->slistparts, pcur);
}

/* ------------------------------------------------------------------------- */
