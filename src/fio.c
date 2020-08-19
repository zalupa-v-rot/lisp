/* ------------------------------------------------------------------------- *
 * !!! простенькая реализация - и так сойдёт. принцип такой: грузим всё в    *
 * память, а дальше линкуем куски. на закрытии меняем файл.                  *
 * ------------------------------------------------------------------------- */

#include "./fio.h"
#include "./fio_part.h"
#include "./fio_file.h"

#include "./lib/bits.h"
#include "./lib/assert.h"

/* ------------------------------------------------------------------------- */

#ifndef FIO_PART_SIZE
#define FIO_PART_SIZE (4*1024) /* !!! а вот мне так хочется */
#endif

#define RW_READ      0
#define RW_WRITE     1
#define RW_READ_ZERO 2
#define MEMCPY_MODE  3

#define FIO_FILECHANGED 128

#ifndef NDEBUG

#include <errno.h>
#include <string.h>

#define FIO_ERROR                                                        \
  do {                                                                  \
    fprintf(stderr, "fio error: line: %i, msg: %s\n", __LINE__, strerror(errno)); \
    return 1;                                                           \
  } while(0)
#else
#define FIO_ERROR \
  return 1
#endif

/* ------------------------------------------------------------------------- */

static void point_init(struct point *point, uint vpmode);
static void point_to_new_part(struct point *point, struct part *pnew);

static uintp reqchunk(struct fio *ph, int *pstatus, uintp vreq);

static void memprocess(struct fio *ph, int vrw,
                       bitptr_t *pbuf, uint vposmode, uintp *pbits);

static int init(struct fio *ph);


static void point_init(struct point *point, uint vpmode) {

  point->pcur = NULL;
  point->vmode = vpmode;
  return;
}

static void point_to_new_part(struct point *point, struct part *pnew) {

  point->pcur = pnew;
  point->sp = file_get_first(point->pcur);
  return;
}

static uintp reqchunk(struct fio *ph, int *pstatus, uintp vreq) {

  uintp vlen2last, vlen2real;
  int vrw = *pstatus;
  struct file *pfile = &ph->sfile;
  struct point *point = &ph->spoint;


  do {

    vlen2last = 0;

    if (point->pcur != NULL) {

      vlen2real = bitptr_diff(file_get_lastreal(pfile, point->pcur),
                              point->sp, point->vmode);
      vlen2last = bitptr_diff(file_get_last(point->pcur),
                              point->sp, point->vmode);

      printf("<< real %i, last %i\n", (int)vlen2real, (int)vlen2last);

      if (vlen2real != 0) {

        return (vlen2real>vreq)?vreq:vlen2real;
      }

      if (vlen2real == vlen2last) {

        struct part *pnext = file_next_part(pfile, point->pcur);

        if (pnext != NULL) {

          /* движение дальше по файлу */

          point_to_new_part(point, pnext);
          continue;
        }
      }
    }

    /* указатель на краю */

    if (!(ph->vmode & FIOM_WRITE) ||
        (vrw == RW_READ && !(ph->vmode & FIOM_REXTEND)) ||
        (vrw == RW_WRITE && !(ph->vmode & FIOM_WEXTEND))) {

      /* у этого режима нет прав расширять файл */

      return 0;
    }


    if (vrw == RW_READ) {

      /* дальше чтение нулями */

      *pstatus = *pstatus & ~MEMCPY_MODE | RW_READ_ZERO;
    }

    if (vlen2last) {

      vlen2last = (vlen2last>vreq)?vreq:vlen2last;
      file_set_lastreal(pfile, bitptr_add(point->sp, point->vmode, vlen2last));
      return vlen2last;
    }

    {
      struct part *pnew;

      if ((pnew = file_lengthen(pfile, point->vmode, FIO_PART_SIZE)) == NULL) {

        return 0;
      }

      point_to_new_part(point, pnew);

      vlen2last = FIO_PART_SIZE * CHAR_BIT;
      vlen2last = (vlen2last>vreq)?vreq:vlen2last;
      file_set_lastreal(pfile, bitptr_add(point->sp, point->vmode, vlen2last));
      return vlen2last;
    }

  } while(1);
}

static void memprocess(struct fio *ph, int vrw,
                       bitptr_t *pbuf, uint vposmode, uintp *pbits) {

  int vstatus;
  uintp vchunk, vbitsleft;
  bitptr_t sbufpos = *pbuf;


  for (vbitsleft = *pbits, vchunk = 0; ; ) {

    if ((vbitsleft -= vchunk) == 0) {

      break;
    }

    if ((vchunk = reqchunk(ph, (vstatus = vrw, &vstatus), vbitsleft)) == 0) {

      break;
    }


    switch(vstatus & MEMCPY_MODE) {

    case RW_READ:
      bitmemcpy(sbufpos, vposmode, ph->spoint.sp, ph->spoint.vmode, vchunk);
      break;

    case RW_WRITE:

      ph->vmode |= FIO_FILECHANGED;
      bitmemcpy(ph->spoint.sp, ph->spoint.vmode, sbufpos, vposmode, vchunk);
      break;

    default:
    case RW_READ_ZERO:
      ph->vmode |= FIO_FILECHANGED;
      bitmemset(ph->spoint.sp, ph->spoint.vmode, 0, vchunk);
      bitmemset(sbufpos, vposmode, 0, vchunk);
      break;
    }

    sbufpos = bitptr_add(sbufpos, vposmode, vchunk);
    ph->spoint.sp = bitptr_add(ph->spoint.sp, ph->spoint.vmode, vchunk);
  }

  *pbuf = sbufpos;
  *pbits = *pbits - vbitsleft;
  return;
}

static int init(struct fio *ph) {

  struct point *point = &ph->spoint;
  long  vsize;


  if (fseek(ph->pfd, 0, SEEK_END)) {

    FIO_ERROR;
  }
  if ((vsize = ftell(ph->pfd)) == -1) {

    FIO_ERROR;
  }
  if (fseek(ph->pfd, 0, SEEK_SET)) {

    FIO_ERROR;
  }

  if (vsize == 0) {

    return 0;
  }

  {
    char *ptr;
    struct part *pnew;

    if ((pnew = file_lengthen(&ph->sfile, point->vmode, vsize)) == NULL) {

      return 1;
    }

    ptr = file_get_ptrorig(point->vmode, pnew);

    point_to_new_part(point, pnew);

    printf("addr %p\n", ptr);

    if (fread(ptr, 1, vsize, ph->pfd) != (size_t)vsize) {

      file_shorten(&ph->sfile, point->vmode);
      FIO_ERROR;
    }
  }

  return 0;
}

/* ------------------------------------------------------------------------- */

int fio_open(struct fio *ph, const char *ppath, uint vmode, uint vpmode) {

  const char *pmode;


  ph->pfd = NULL;

  if (!(vmode & 3)) {

    return 1;
  }

  if (vmode & FIOM_CREATE) {

    if ((ph->pfd = fopen(ppath, "r")) == NULL) {

      pmode = "ab+";

    } else {

      fclose(ph->pfd);
      pmode = "rb+";
    }

  } else {

    /* 1 R  r
       2 W  a
       3 RW r+ */
    if ((vmode&3) == FIOM_READ) {
      pmode = "rb";
    } else {
      pmode = "rb+";
    }
  }

  point_init(&ph->spoint, vpmode);
  file_init(&ph->sfile);

  ph->vmode = vmode;

  if ((ph->pfd = fopen(ppath, pmode)) == NULL) {

    FIO_ERROR;
  }

  if (init(ph)) { return 1; }

  return 0;
}

uintp fio_read(struct fio *ph, char *pbuf, uintp vbytes) {

  assert(vbytes <= INTPTR_MAX / CHAR_BIT);

  return  fio_readb(ph, bitptr(pbuf, 0), BM_DEFAULT, vbytes * CHAR_BIT)
    / CHAR_BIT;
}

uintp fio_write(struct fio *ph, const char *pbuf, uintp vbytes) {

  assert(vbytes <= INTPTR_MAX / CHAR_BIT);

  return fio_writeb(ph, bitptr((char*)pbuf, 0), BM_DEFAULT, vbytes * CHAR_BIT)
    / CHAR_BIT;
}

uintp fio_readb(struct fio *ph,
                bitptr_t sbufpos, uint vposmode, uintp vbits) {


  if (!(ph->vmode & FIOM_READ)) {

    return 0;
  }

  memprocess(ph, RW_READ, &sbufpos, vposmode, &vbits);
  return vbits;
}

uintp fio_writeb(struct fio *ph,
                 bitptr_t sbufpos, uint vposmode, uintp vbits) {


  if (!(ph->vmode & FIOM_WRITE)) {

    return 0;
  }

  memprocess(ph, RW_WRITE, &sbufpos, vposmode, &vbits);
  return vbits;
}

int cb_pwrite(struct file *pfile, void *pdata, uintp vsize) {

  struct fio *ph = container_of(pfile, struct fio, sfile);


  printf("<< %i\n", (int)vsize);

  if (fwrite(pdata, 1, vsize, ph->pfd) != vsize) {

    /* !!! и что делать дальше? */
    FIO_ERROR;
  }

  return 0;
}

int cb_none(struct file *pfile, void *pdata, uintp vsize) {

  UNUSED(pfile), UNUSED(pdata), UNUSED(vsize);

  return 0;
}

int fio_close(struct fio *ph) {


  if (ph->pfd == NULL) { return 1; }

  if (!(ph->vmode & FIO_FILECHANGED)) {


    if (file_close(&ph->sfile, ph->spoint.vmode, cb_none)) {

      fclose(ph->pfd);
      return 1;
    }

    if (fclose(ph->pfd)) {

      FIO_ERROR;
    }

    return 0;
  }

  if (fseek(ph->pfd, 0, SEEK_SET)) {

    FIO_ERROR;
  }


  if (file_close(&ph->sfile, ph->spoint.vmode, cb_pwrite)) {

    fclose(ph->pfd);
    return 1;
  }

  if (fclose(ph->pfd)) {

    FIO_ERROR;
  }

  return 0;
}

/* void fio_pmode(struct fio *ph, uint vpmode) { */

/*   struct file *pfile = &ph->sfile; */
/*   struct point *point = &ph->spoint; */


/*   /\* зануление _новых_ битов на концах файла *\/ */

/*   if (point->pcur != NULL && */
/*       (vpmode & BM_BORDER) != (point->vmode & BM_BORDER)) { */


/*     if (bitptr_getbit(pfile->sreal_adrl)) { */

/*       bitset(pfile->sreal_adrl, point->vmode, */
/*              CHAR_BIT - bitptr_getbit(pfile->sreal_adrl), 0); */
/*     } */

/*     if (bitptr_getbit(pfile->sreal_adrm)) { */

/*       bitset(pfile->sreal_adrm, point->vmode, */
/*              CHAR_BIT - bitptr_getbit(pfile->sreal_adrm), 0); */
/*     } */
/*   } */

/*   point->vmode = vpmode; */
/*   return; */
/* } */

/* void fio_pseek(struct fio *ph, intp voff) { */

/*   fio_pseekb(ph, voff * CHAR_BIT); */
/*   return; */
/* } */

/* intp fio_ptell(struct fio *ph, int vto) { */

/*   return fio_ptellb(ph, vto) / CHAR_BIT; */
/* } */

/* #define ABS(A) (((A)<0)?-(A):(A)) */

/* void fio_pseekb(struct fio *ph, intp voff) { */

/*   struct part *pnext; */
/*   struct file *pfile = &ph->sfile; */
/*   struct point *point = &ph->spoint; */
/*   uint vdir = ((point->vmode & BM_ORIENT) == BM_IO)?MSPART:LSPART; */
/*   intp vchunk = 0; */


/*   if (voff < 0) { */

/*     vchunk = CHAR_BIT; /\* ошибка указателя в первой части *\/ */
/*     vdir = !vdir; */
/*   } */

/*   if (point->pcur == NULL) { */

/*     return; */
/*   } */

/*   for ( ; ; vchunk = 0) { */

/*     vchunk = bitptr_diff(file_get_lastreal(pfile, point->pcur), */
/*                          point->sp, point->vmode); */

/*     if (ABS(vchunk) > ABS(voff)) { */

/*       vchunk = voff; */
/*     } */

/*     point->sp = bitptr_add(point->sp, point->vmode, vchunk); */

/*     if ((voff -= vchunk) == 0) { */

/*       break; */
/*     } */

/*     if ((pnext = file_next_part(pfile, point->pcur)) != NULL) { */

/*       point_to_new_part(point, pnext); */

/*     } else { */

/*       break; */
/*     } */
/*   } */

/*   return; */
/* } */

/* intp fio_ptellb(struct fio *ph, int vto) { */

/*   struct part *pnext; */
/*   struct file *pfile = &ph->sfile; */
/*   struct point spoint = ph->spoint, * const point = &spoint; */
/*   intp vlen2real; */
/*   uint vdir; */


/*   if (point->pcur == NULL) { */

/*     return 0; */
/*   } */

/*   if (vto == FIO_PBEG) { */

/*     vdir = ((point->vmode & BM_ORIENT) == BM_IO)?LSPART:MSPART; */
/*     vlen2real = CHAR_BIT; /\* ошибка указателя в первой части *\/ */
/*     /\* printf("_%i %i\n", (int)pfile->vparts, (int)vlen2real); *\/ */

/*   } else { */

/*     vdir = ((point->vmode & BM_ORIENT) == BM_IO)?MSPART:LSPART; */
/*     vlen2real = 0; */
/*   } */

/*   for ( ; ; ) { */

/*     vlen2real += bitptr_diff(file_get_lastreal(pfile, point->pcur), */
/*                              point->sp, point->vmode); */

/*     if ((pnext = file_next_part(pfile, point->pcur)) != NULL) { */

/*       point_to_new_part(point, pnext); */

/*     } else { */

/*       return vlen2real; */
/*     } */
/*   } */
/* } */

/* ------------------------------------------------------------------------- */
