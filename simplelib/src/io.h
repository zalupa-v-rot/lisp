/* ------------------------------------------------------------------------- */
/*
  модуль для работы с файламы/потоками. предпологается его совместить с fio -
  ему работу с regular file, остальное этому модулю.


  - regular file
  если есть права только на запись, то остаток на запись может не записаться в
  конечный файл, потому что потребуется чтение байта для совмещения данных.

  если же это конец файла, тогда остаток на запись запишется с дополнением
  нулями.

  - socket/pipe
  остаток на запись запишется с дополнением нулями.
*/

#include "./types.h"
#include "./bits.h"

#include <stdio.h>

#if (defined(_POSIX_VERSION) && _POSIX_VERSION >= 200112L)

#define fsize_t off_t
#define FSEEK fseeko

#define FTELL ftello

#else

#define fsize_t long
#define FSEEK fseek
#define FTELL ftell

#endif

#define IOE_SUCCESS     0
#define IOE_LIBERROR    1
#define IOE_OVERFLOW    2
/*
  операция может нарушить согласованность данных. запись прервёт
  последовательность чтения, чтение испортит запись в файле.

  ошибка может произойти если программа пытается совместить чтение-запись
  некратными байту порциями данных.

  чтобы этого избежать, нужно перед переходом на противоположный вызов, нужно
  дополнить доступ.

  или скомпилировать программу в posix стандарте. (!!! но мне пока это
  реализовывать лень)
*/
#define IOE_CONSISTENCY 3

typedef struct {

  FILE    *pf;

  /*
    остатки в чтении/записи.
    если pf - socket/pipe, то используется всё
    если pf - regular file, то используется vw*
  */
  uchar   vrrem, vrsiz,
          vwrem, vwsiz;


  int     verror,
          verrno,
          vline;

} IOFILE;


IOFILE *io_nullinit(IOFILE *ph);

/*
  размер файла в байтах

  возврат != 0 - ошибка.
*/
int io_size(IOFILE *ph, fsize_t *psize);

/*
  размер файла в битах.

  возврат != 0 - ошибка.
*/
int io_sizeb(IOFILE *ph, uintp *pbits);

/*
  открыть файл

  возврат != 0 - ошибка.
*/
int io_open(IOFILE *ph, const char *path, const char *mode);

/*
  присоединить открытый файл.

  возврат != 0 - ошибка.
*/
int io_attach(IOFILE *ph, FILE *pf);

/*
  закрыть файл

  возврат != 0 - ошибка.
*/
int io_close(IOFILE *ph);

/*
  возврат - число прочитанных бит.

  если возврат != vbits и возврат != 0, значение недостающих битов не
  определено.
*/
uintp io_readb(IOFILE *ph, bitptr_t sp, uint vmode, uintp vbits);

/*
  возврат - число записанных бит.
*/
uintp io_writeb(IOFILE *ph, bitptr_t sp, uint vmode, uintp vbits);



int io_error(IOFILE *ph);

/* ------------------------------------------------------------------------- */
