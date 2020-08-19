/* ------------------------------------------------------------------------- */

#ifndef FIO_FILE_H
#define FIO_FILE_H

#include "./fio.h"
#include "./fio_part.h"


void file_init(struct file *ph);

struct part* file_lengthen(struct file *ph, uint vpmode, uintp vsize);
int file_shorten(struct file *ph, uint vpmode);

int file_close(struct file *ph, uint vpmode,
               int (*pwrite)(struct file *ph, void *pdata, uintp vsize));

void* file_get_ptrorig(uint vpmode, struct part *pcur);


bitptr_t file_get_lastreal(struct file *ph, struct part *pcur);
bitptr_t file_get_firstreal(struct file *ph, struct part *pcur);
bitptr_t file_get_last(struct part *pcur);
bitptr_t file_get_first(struct part *pcur);

void file_set_lastreal(struct file *ph, bitptr_t sp);

struct part* file_next_part(struct file *ph, struct part *pcur);

#endif /* FIO_FILE_H */

/* ------------------------------------------------------------------------- */
