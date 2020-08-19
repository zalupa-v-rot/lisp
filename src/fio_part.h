/* ------------------------------------------------------------------------- */

#ifndef FIO_PART_H
#define FIO_PART_H

#include "./fio.h"


struct part {

  bitptr_t
          sadrf, /* указатель на использумый бит */
          sadrl; /* указатель на неиспользуемый бит */

  struct dlist_node
          snode;
};

#define LSPART 0
#define MSPART 1

/*
  добавить новую часть файла в один из концов с размером vsize.

  возврат - новая часть или NULL
*/
struct part* part_new_last(struct dlist_head *ph, uint vpmode, uintp vsize);

/*
  удалить часть файла из одного из концов

  возврат - следующая часть или NULL
*/
struct part* part_free_first(struct dlist_head *ph, uint vpmode);

void part_free(struct dlist_head *ph, uint vpmode, struct part *pcur);


struct part* part_get_first(struct dlist_head *ph);
struct part* part_get_last(struct dlist_head *ph);

void* part_get_ptrorig(uint vpmode, struct part *pcur);
void* part_pop_ls(struct dlist_head *ph, uint vpmode, uintp *psize);


struct part* part_next(struct dlist_head *ph, struct part *pcur);

#endif /* FIO_PART_H */

/* ------------------------------------------------------------------------- */
