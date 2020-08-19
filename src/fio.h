/* ------------------------------------------------------------------------- */

#ifndef FIO_H
#define FIO_H

#include "./lib/bits.h"
#include "./lib/list.h"

#include <stdio.h>


struct point {

  struct part
          *pcur;
  uint    vmode;
  bitptr_t
          sp; /* pcur->sadrl+1 => sp >= pcur->sadrf */
};

struct part;

struct file {

  uintp   vparts;

  bitptr_t
          sreal_adrf, /* указатель на использумый бит */
          sreal_adrl; /* указатель на неиспользуемый бит */

  struct dlist_head
          slistparts; /* struct part */
};

struct fio {

  FILE    *pfd;
  uint    vmode;

  struct file sfile;
  struct point spoint;
};


#define FIOM_READ     1  /* парава на чтение файла */
#define FIOM_WRITE    2  /* парава на изменение файла */
#define FIOM_CREATE   4 /* создать если не существует */
#define FIOM_REXTEND  8 /* расширять файл чтением. новые биты будут установлены в 0 */
#define FIOM_WEXTEND 16 /* расширять файл записью */


/*
  открыть файл если он существует.

  возврат != 0 - ошибка
 */
int fio_open(struct fio *ph, const char *ppath, uint vmode, uint vbmode);

/*
  если используются после функций с префиксом b, возврат неопределён.
*/
uintp fio_read(struct fio *ph, char *pbuf, uintp vbytes);
uintp fio_write(struct fio *ph, const char *pbuf, uintp vbytes);

/*
  читает файл пока не упирётся в конец
  начальная позиция указатля зависит от его ориентации

  возврат - прочитанное число бит.
*/
uintp fio_readb(struct fio *ph,
                bitptr_t sbufpos, uint vposmode, uintp vbits);

/*
  пишет в файл и если доходит до конца, расширяет его
  начальная позиция указатля зависит от его ориентации

  возврат - записанное число бит.
*/
uintp fio_writeb(struct fio *ph,
                 bitptr_t sbufpos, uint vposmode, uintp vbits);

/*
  закрыть файл
*/
int fio_close(struct fio *ph);


void fio_pmode(struct fio *ph, uint vmode);


#define FIO_PEND  0
#define FIO_PBEG  1

void fio_pseek(struct fio *ph, intp voff);
intp fio_ptell(struct fio *ph, int vto);

void fio_pseekb(struct fio *ph, intp voff);
intp fio_ptellb(struct fio *ph, int vto);

#endif /* FIO_H */

/* ------------------------------------------------------------------------- */
