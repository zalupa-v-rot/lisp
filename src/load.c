/* ------------------------------------------------------------------------- */
/*
  в принципе, можно вынести read_file() в отдельный модуль, но это ничего не
  поменяет. потому как функция load_file() не относится ни к lisp.c ни к inter.c
  ни к parse. а точнее к последним двум вместе.

  поэтому функция такой урод.
*/

#include "./load.h"
#include "./parse.h"
#include "./inter.h"
#include "./clear.h"
#include "./memory.h"

/* ------------------------------------------------------------------------- */

void load_file(struct error_stack **pcnterr, const struct scell *pname) {

  struct inter *pcontext = container_of(pcnterr, struct inter, perr);
  char * volatile pfdata = NULL, * volatile pfname = NULL;
  IOFILE fd; uintp vfsize, vrealsize;
  struct error_node senode;


  io_nullinit(&fd);

  if (ERROR_CATCH(pcontext->perr, &senode)) {

    if (pfdata != NULL) {

      memfree(pcontext->pm, pfdata);
    }
    if (pfname != NULL) {

      memfree(pcontext->pm, pfname);
    }

    io_close(&fd);

    ERROR_SILENT(pcontext->perr);
    return;
  }


  pfname = string_to_cstr(pcontext->pm, &pname->u.sobject);

  if (io_open(&fd, pfname, "rb") || io_sizeb(&fd, &vrealsize)) {

    ERROR(pcontext->perr, FAILED_TO_OPEN_FILE);
  }

  pfdata = memalloc(pcontext->pm, vrealsize/CHAR_BIT);

  if (vrealsize == 0 ||
      (vfsize = io_readb(&fd, bitptr(pfdata, BM_IO|BM_LSF), BM_IO|BM_LSF,
                         vrealsize)) != vrealsize) {

    ERROR(pcontext->perr, FAILED_TO_READ_FILE);
  }

  if (io_close(&fd)) {

    ERROR(pcontext->perr, FAILED_TO_CLOSE_FILE);
  }

  if (load_data(pcontext, pfdata, vfsize/CHAR_BIT, pfname)) {

    ERROR(pcontext->perr, TRACE);
  }

  memfree(pcontext->pm, pfdata);
  memfree(pcontext->pm, pfname);
  ERROR_LEAVE(pcontext->perr);
  return;
}

int load_data(struct inter *pcontext,
              const char *pfdata, uintp vfsize, const char *pfname) {

  int vcode;

  assert(pfdata != NULL);
  assert(vfsize != 0);
  assert(pfname != NULL);



  {
    uintp vparsepos, vparseline, vparsecolumn;
    struct parse spar;
    struct scell sroot;

    parse_init(&spar, pcontext->pm, pcontext->perr);


    for (vparsepos = vparseline = vparsecolumn = 0; ; ) {

      vcode = parse(&spar, pfdata+vparsepos, vfsize-vparsepos, &sroot);


      if (parse_line(&spar) == 0) {

        vparsecolumn += parse_column(&spar);

      } else {

        vparsecolumn = parse_column(&spar);
        vparseline += parse_line(&spar);
      }


      if (vcode) {

        fprintf(stderr, "%s: parsing error: %u \"%s\" line: %u"
                " column: %u %s:%u:\n",

                pfname, error_code(pcontext->perr),
                error_str(pcontext->perr), (uint)vparseline,
                (uint)vparsecolumn,
                error_srcfile(pcontext->perr), error_srcline(pcontext->perr));

        break;
      }



      if (TYPEGET(sroot.vtype) != TYPE_NIL) {

        /* { дебажное !!! */
        /*   struct mstat_std smstat; */

        /*   mstat_std(pcontext->pm->phead, &smstat); */

        /*   printf("_memory usage: sexp %"PRIuPTR", str %"PRIuPTR",
             strtotal %"PRIuPTR", func %"PRIuPTR", var %"PRIuPTR"\n", */
        /*          smstat.vsexp, */
        /*          smstat.vstr, */
        /*          smstat.vstrtotal, */
        /*          smstat.vfunc, */
        /*          smstat.vvar); */
        /* } */

        if (vcode = inter(pcontext, &sroot)) {

          fprintf(stderr, "%s: interpretation error: %u \"%s\" %s:%u:\n",
                  pfname, error_code(pcontext->perr),
                  error_str(pcontext->perr),
                  error_srcfile(pcontext->perr), error_srcline(pcontext->perr));
        }


#ifdef OPTION_HOOK_INTER_RET
        if (pcontext->pf_hook_inter_ret != NULL) {

          pcontext->pf_hook_inter_ret(pcontext,
                                      pfname, &sroot, pcontext->ptr);
        }
#endif

        clear_scell(pcontext->pm, &sroot);


        if (vcode) { break; }
      }

      if ((vparsepos += parse_ending(&spar)) == vfsize) {

        break;
      }
    }
  }

  return vcode;
}

/* ------------------------------------------------------------------------- */
