/* ------------------------------------------------------------------------- */

#include "./io.h"
#include "./ofchk.h"

#include <errno.h>

#define ERRORLIB(PH) (PH)->verror = IOE_LIBERROR, (PH)->verrno = errno,  \
    (PH)->vline = __LINE__

#define ERROR(PH, CODE) (PH)->verror = CODE, (PH)->vline = __LINE__

#define LA_NONE  0
#define LA_READ  1
#define LA_WRITE 2

/* ------------------------------------------------------------------------- */

IOFILE* io_nullinit(IOFILE *ph) {

  ph->pf = NULL;
  return ph;
}

static void io_clearerror(IOFILE *ph) {

  ph->verror = IOE_SUCCESS;
  return;
}

int io_size(IOFILE *ph, fsize_t *psize) {

  fsize_t vsize;

  io_clearerror(ph);


  if (FSEEK(ph->pf, 0, SEEK_END)) {

    ERRORLIB(ph);
    return 1;
  }

  if ((vsize = FTELL(ph->pf)) == -1) {

    ERRORLIB(ph);
    return 1;
  }

  if (FSEEK(ph->pf, 0, SEEK_SET)) {

    ERRORLIB(ph);
    return 1;
  }

  *psize = vsize;
  return 0;
}

int io_sizeb(IOFILE *ph, uintp *pbits) {

  fsize_t vsize;


  if (io_size(ph, &vsize)) {

    return 1;
  }

  if (ofchk_mul_uintp(vsize, CHAR_BIT) ||
      vsize < 0 || (intp)vsize != vsize) {

    ERROR(ph, IOE_OVERFLOW);
    return 1;
  }

  *pbits = vsize * CHAR_BIT;
  return 0;
}

int io_open(IOFILE *ph, const char *path, const char *mode) {

  io_clearerror(ph);


  if ((ph->pf = fopen(path, mode)) == NULL) {

    ERRORLIB(ph);
    return 1;
  }

  ph->vwsiz = ph->vrsiz = 0;
  return 0;
}

int io_attach(IOFILE *ph, FILE *pf) {

  io_clearerror(ph);


  ph->pf = pf;
  ph->vwsiz = ph->vrsiz = 0;
  return 0;
}


int io_close(IOFILE *ph) {

  if (ph->pf == NULL) {

    return 0;
  }

  if (ph->vwsiz) {

    uchar va = ph->vwrem & (1<<ph->vwsiz)-1;

    if (fwrite(&va, 1, 1, ph->pf) != 1) {

      ERRORLIB(ph);
      return 1;
    }
  }

  if (fclose(ph->pf)) {

    ph->pf = NULL;
    ERRORLIB(ph);
    return 1;
  }

  ph->pf = NULL;
  return 0;
}

/* ------------------------------------------------------------------------- */

static void mreverse(void *p, uintp vsize) {

  uchar *pa = p, *pb = (uchar*)p + (vsize-1);


  for ( ; vsize > 1; vsize -= 2) {

    *pa++ = *pb--;
  }

  return;
}

static void mbitreverse(void *p, uintp vsize) {

  uchar *pa = p;


  for ( ; vsize; *pa = bitrevc(*pa),  vsize--);

  return;
}

#define T (BM_IO|BM_LSF)

uintp io_readb(IOFILE *ph, bitptr_t sp, uint vmode, uintp vbits) {

  uintp vretbits = 0;

  io_clearerror(ph);


  if (!vbits) {

    return 0;
  }
  if (ph->vwsiz) {

    ERROR(ph, IOE_CONSISTENCY);
    return 0;
  }

  if (ph->vrsiz) {

    uintp vchunk = (vbits < ph->vrsiz)?vbits:ph->vrsiz;

    bitmemcpy(sp, vmode, bitptr(&ph->vrrem, BM_IO|BM_LSF), BM_IO|BM_LSF, vchunk);

    sp = bitptr_add(sp, vmode, vchunk);
    ph->vrsiz -= vchunk;
    ph->vrrem >>= vchunk;

    vbits -= vchunk, vretbits += vchunk;
  }

  if (vbits == 0) {

    return vretbits;
  }

  if (vbits > INTPTR_MAX-(CHAR_BIT-1)*2) { /* вероятно переполнение. хотя
                                              недостающие биты остаются
                                              нетронутыми. */
    ERROR(ph, IOE_OVERFLOW);
    return vretbits;
  }

  {
    uchar vbegbuf, vendbuf, *pb; uintp vbytes;


    vbytes = (vbits + (CHAR_BIT-1)) / CHAR_BIT;

    if ((vmode & BM_ORIENT) == BM_IO) {

      pb = bitptr_getbyte(sp);
      vbegbuf = *pb;
      vendbuf = *(pb + (vbytes-1));

    } else {

      pb = bitptr_getbyte(sp);
      vbegbuf = *pb;
      vendbuf = *(pb - (vbytes-1));

      pb -= vbytes-1;
    }


    {
      uintp va;

      /* данные кладутся выравненно к байту */

      if ((va = fread(pb, 1, vbytes, ph->pf)) < vbytes) {

        if (ferror(ph->pf)) {

          ERRORLIB(ph);
        }
        /* if (feof(ph->pf)) {} */

        if (va == 0) { return 0; }
      }

      vbytes = va;

      /* сохранение остатка */

      if (vbytes * CHAR_BIT > vbits) {

        vretbits += vbits;
        ph->vrsiz = vbytes * CHAR_BIT - vbits;
        ph->vrrem = *(pb + (vbytes-1)) >> CHAR_BIT - ph->vrsiz;

      } else {

        vretbits += vbytes * CHAR_BIT;
      }
      /* правильный порядок байт, бит */

      if ((vmode & BM_ORIENT) == BM_DO) {

        mreverse(pb, vbytes);
      }

      if ((vmode & BM_BORDER) == BM_MSF) {

        mbitreverse(pb, vbytes);
      }
    }

    {
      /* смещение данных на правильные битовые места */

      uchar vshift, vrem;

      vshift = bitptr_getbit(sp); /* восстановка начальных битов */
      vrem = bitptr_getbit(bitptr_add(sp, vmode, vbits)); /* восстановка
                                                             конечных битов */

      if ((vmode & BM_BORDER) == BM_MSF) {

        vshift = (CHAR_BIT-1) - vshift;
        vrem += 1;

      } else {

        vrem = CHAR_BIT - vrem;
      }

      vrem = vrem % CHAR_BIT;

      {
        uintp vallbits = (vshift + vbits + (CHAR_BIT-1)) / CHAR_BIT * CHAR_BIT;

        bitmemshiftl(bitptr_floor(sp, vmode), vmode, vshift, vallbits);
      }

      /* восстановление пострадавших (вероятно) битов */

      bitmemcpy(bitptr_floor(sp, vmode), vmode,
                bitptr(&vbegbuf, T), T, vshift);

      bitmemcpy(bitptr_add(sp, vmode, vbits), vmode,
                bitptr_add(bitptr(&vendbuf, T), T, CHAR_BIT - vrem),
                vmode, vrem);
    }

    return vretbits;
  }
}

uintp io_writeb(IOFILE *ph, bitptr_t sp, uint vmode, uintp vbits) {

  io_clearerror(ph);


  if (!vbits) {

    return 0;
  }
  if (ph->vrsiz) {

    ERROR(ph, IOE_CONSISTENCY);
    return 0;
  }

  {
    uchar sbuf[sizeof(int)+1]; uintp vchunk, vbytes, vremchunk = 0, vretbits = 0;
    bitptr_t spb = bitptr(sbuf, T);

    if (ph->vwsiz) {

      vremchunk = ph->vwsiz, ph->vwsiz = 0;

      sbuf[0] = ph->vwrem;
      spb = bitptr_add(spb, T, vremchunk);
    }

    /* запись порциями по sizeof(int) */

    for ( ; ; vremchunk = 0, spb = bitptr(sbuf, T)) {

      vchunk = (vbits-vretbits < BITSOF(int)-vremchunk)?
        vbits-vretbits:BITSOF(int)-vremchunk;
      vbytes = (vchunk + vremchunk) / CHAR_BIT;

      bitmemcpy(spb, T, sp, vmode, vchunk);
      sp = bitptr_add(sp, vmode, vchunk);

      if (vbytes) {

        uintp vb;

        if ((vb = fwrite(sbuf, 1, vbytes, ph->pf)) != vbytes) {

          if (ferror(ph->pf)) {

            ERRORLIB(ph);
          }

          if (vb == 0) {

            /* если так, то и vremchunk не мог быть записан */
            ph->vwsiz = vremchunk;
            return vretbits;
          }

          /* vremchunk записан в любом случае. остатка нет, потому что
             кратно. */
          return vretbits + (vb * CHAR_BIT - vremchunk);
        }
      }

      if ((vretbits += vchunk) == vbits) {

        /* остаток */
        vchunk += vremchunk;
        ph->vwsiz = vchunk % CHAR_BIT;
        ph->vwrem = sbuf[vchunk/CHAR_BIT]; /* поэтому +1 в буфере */

        return vretbits;
      }
    }
  }

}

int io_error(IOFILE *ph) {

  return ph->verror;
}

#undef T

/* ------------------------------------------------------------------------- */
