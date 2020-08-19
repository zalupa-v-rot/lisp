/* ------------------------------------------------------------------------- */

#include "./inter_kw.h"
#include "./memory.h"
#include "./lib/ofchk.h"
#include <stdio.h>

static uchar cmp_freeid(void *p, struct dlist_node *pc) {

  uintp *pid = p;
  struct filedesc *pcur
          = container_of(pc, struct filedesc, sn);

  if (pcur->vid - *pid > 1) {

    return 1;
  }

  *pid = pcur->vid;
  return 0;
}

static uchar cmp_equal(void *p, struct dlist_node *pc) {

  uintp vid = (uintp)p;
  struct filedesc *pcur
          = container_of(pc, struct filedesc, sn);

  if (pcur->vid == vid) {

    return 1;
  }

  return 0;
}

#define INTIO_READ   1
#define INTIO_WRITE  2
#define INTIO_CREATE 4
#define INTIO_BIT    8
#define INTIO_ACCESS (INTIO_READ|INTIO_WRITE)

const char * mode_std(const char *pfname, uint vmode) {


  if (!(vmode & INTIO_ACCESS)) {

    return "";
  }

  if (vmode & INTIO_CREATE) {

    FILE *pf;

    if ((pf = fopen(pfname, "r")) == NULL) {

      return "ab+";

    } else {

      fclose(pf);
      return "rb+";
    }

  } else {

    /* 1 R  r
       2 W  a
       3 RW r+ */
    if ((vmode&3) == INTIO_READ) {

      return "rb";

    } else {

      return "rb+";
    }
  }
}

#define INTIO_MODCHARS  "rwcb"

void open_file(struct error_stack **pcnterr,
               const struct scell *path,
               const struct scell *pmode,
               struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);
  struct error_node senode;
  struct filedesc * volatile pfdesc = NULL;


  if (ERROR_CATCH(pcontext->perr, &senode)) {

    if (pfdesc != NULL) {

      fdfree(pcontext->pm, pfdesc);
    }

    ERROR_SILENT(pcontext->perr);
    return;
  }

  pfdesc = fdalloc(pcontext->pm);

  {
    static const int smods[4] = { INTIO_READ, INTIO_WRITE, INTIO_CREATE,
                                  INTIO_BIT };
    struct string set = { INTIO_MODCHARS, sizeof(INTIO_MODCHARS)-1 };
    uintp vp, vt, vn;


    for (vn = vp = pfdesc->vmode = 0; ; vn++) {

      string_scan(&pmode->u.sobject, &vp, &set, (vt = 0, &vt));

      if (vp++ >= pmode->u.sobject.vlen) {

        break;
      }

      if (pfdesc->vmode & smods[vt]) {

        ERROR(pcontext->perr, INCORRECT_FILE_MODE);
      }

      pfdesc->vmode |= smods[vp];
    }

    if (vn != pmode->u.sobject.vlen) {

      ERROR(pcontext->perr, INCORRECT_FILE_MODE);
    }
  }

  {
    char *pfname = string_to_cstr(pcontext->pm, &path->u.sobject);


    if (io_open(&pfdesc->sd, pfname, mode_std(pfname, pfdesc->vmode))) {

      memfree(pcontext->pm, pfname);
      ERROR(pcontext->perr, FAILED_TO_OPEN_FILE);
    }

    memfree(pcontext->pm, pfname);
  }


  {
    struct dlist_node *pcur;
    struct scell sfd;


    pfdesc->vid = 0;

    if ((pcur = dlist_search(&pcontext->sfiles,
                             &pfdesc->vid, cmp_freeid)) == NULL) {

      pfdesc->vid = pcontext->vfilesd;
    }


    INITNIL(sfd.vtype);
    TYPESET(sfd.vtype, TYPE_OBJECT);
    string_from_uintp(pcontext->pm, &sfd.u.sobject, pfdesc->vid);

    if (pcur == NULL) {

      dlist_addl(&pcontext->sfiles, &pfdesc->sn);

    } else {

      dlist_addbefore(&pcontext->sfiles, pcur, &pfdesc->sn);
    }

    pcontext->vfilesd++;
    *pret = sfd;
  }


  ERROR_LEAVE(pcontext->perr);
  return;
}

static struct filedesc* find_fd(struct inter *pcontext,
                                const struct string *ps) {

  uintp vid;
  struct dlist_node *pcur;


  if (string_to_uintp(ps, &vid)) {

    ERROR(pcontext->perr, UINTP_OVERFLOW);
  }

  if ((pcur = dlist_search(&pcontext->sfiles,
                           (void*)vid, cmp_equal)) == NULL) {

    ERROR(pcontext->perr, INVALID_FILE_DESCRIPTOR);
  }

  return container_of(pcur, struct filedesc, sn);
}

void read_file(struct error_stack **pcnterr,
               const struct scell *pfd,
               const struct scell *psize,
               struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);
  struct error_node senode;
  struct scell sdata;
  struct filedesc *pfdesc;
  uintp vbits, vbytes;


  if (ERROR_CATCH(pcontext->perr, &senode)) {

    if (TYPEGET(sdata.vtype) != TYPE_NIL) {

      strfree(pcontext->pm, &sdata.u.sobject);
    }

    ERROR_SILENT(pcontext->perr);
    return;
  }

  {
    if (string_to_uintp(&psize->u.sobject, &vbits)) {

      ERROR(pcontext->perr, UINTP_OVERFLOW);
    }

    if (!(pfdesc->vmode & INTIO_BIT)) {

      if (ofchk_mul_uintp(vbits, CHAR_BIT)) {

        ERROR(pcontext->perr, UINTP_OVERFLOW);
      }

      vbits *= CHAR_BIT;
    }

    if (ofchk_add_uintp(vbits, CHAR_BIT-1)) {

      ERROR(pcontext->perr, UINTP_OVERFLOW);
    }

    vbytes = (vbits + (CHAR_BIT-1)) / CHAR_BIT;
  }

  INITNIL(sdata.vtype);

  if (vbits) {

    stralloc(pcontext->pm, &sdata.u.sobject, vbytes);
    TYPESET(sdata.vtype, TYPE_OBJECT);
    sdata.u.sobject.pstr[vbytes-1] = 0; /* на случай некратности CHAR_BIT */

    pfdesc = find_fd(pcontext, &pfd->u.sobject);

    if ((vbits = io_readb(&pfdesc->sd,
                          bitptr(sdata.u.sobject.pstr, BM_IO|BM_LSF),
                          BM_IO|BM_LSF, vbits)) == 0) {

      if (io_error(&pfdesc->sd)) {

        ERROR(pcontext->perr, FAILED_TO_READ_FILE);
      }

      strfree(pcontext->pm, &sdata.u.sobject);
      INITNIL(sdata.vtype);

    } else {

      vbytes = (vbits + (CHAR_BIT-1)) / CHAR_BIT;
      strrealloc_r(pcontext->pm, &sdata.u.sobject, vbytes);
    }

    BITTAIL_SET(sdata.vtype, vbits % CHAR_BIT);
  }

  *pret = sdata;
  ERROR_LEAVE(pcontext->perr);
  return;
}

void write_file(struct error_stack **pcnterr,
                const struct scell *pfd,
                const struct scell *pdata,
                struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);
  struct error_node senode;
  struct filedesc *pfdesc;
  uintp vbits;


  if (ERROR_CATCH(pcontext->perr, &senode)) {

    ERROR_SILENT(pcontext->perr);
    return;
  }

  {
    if (ofchk_mul_uintp(pdata->u.sobject.vlen, CHAR_BIT)) {

      ERROR(pcontext->perr, UINTP_OVERFLOW);
    }

    vbits = (pdata->u.sobject.vlen-1) * CHAR_BIT + BITTAIL_GET(pdata->vtype);
  }

  if (vbits) {

    pfdesc = find_fd(pcontext, &pfd->u.sobject);

    vbits = io_writeb(&pfdesc->sd,
                      bitptr(pdata->u.sobject.pstr, BM_IO|BM_LSF),
                      BM_IO|BM_LSF, vbits);

    if (io_error(&pfdesc->sd)) {

      ERROR(pcontext->perr, FAILED_TO_WRITE_FILE);
    }
  }

  if (!(pfdesc->vmode & INTIO_BIT)) {

    vbits /= CHAR_BIT;
  }

  {
    struct scell swritten;

    INITNIL(swritten.vtype);
    TYPESET(swritten.vtype, TYPE_OBJECT);
    MARKSET(swritten.vtype, MARK_NUMBER);
#ifdef OPTION_MATH
    swritten.vpoint = 0;
#endif
    string_from_uintp(pcontext->pm, &swritten.u.sobject, vbits);

    *pret = swritten;
    ERROR_LEAVE(pcontext->perr);
    return;
  }
}

void close_file(struct error_stack **pcnterr,
                const struct scell *pfd,
                struct scell *pret) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);
  struct error_node senode;
  struct filedesc *pfdesc;


  if (ERROR_CATCH(pcontext->perr, &senode)) {

    ERROR_SILENT(pcontext->perr);
    return;
  }

  pfdesc = find_fd(pcontext, &pfd->u.sobject);

  if (io_close(&pfdesc->sd)) {

    ERROR(pcontext->perr, FAILED_TO_CLOSE_FILE);
  }

  dlist_del(&pcontext->sfiles, &pfdesc->sn);
  fdfree(pcontext->pm, pfdesc);
  INITNIL(pret->vtype);
  ERROR_LEAVE(pcontext->perr);
  return;
}

/* ------------------------------------------------------------------------- */
