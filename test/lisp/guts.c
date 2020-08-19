/* ------------------------------------------------------------------------- */

#include "./guts.h"
#include "./lisp/parse.h"

#include <stdio.h> /* printf() */
#include <stdlib.h> /* malloc(), free() */
#include <string.h> /* strlen() */
#include <assert.h>
#include <inttypes.h>

/* ------------------------------------------------------------------------- */

#ifdef OPTION_MATH
#define FUNCTIONS (11+5+5)
#else
#define FUNCTIONS 11
#endif

void init_(struct head *ph) {


  ERROR_INIT(&ph->serr);

  if (lisp_init(&ph->scontext,
                memory_std_init(&ph->smemory,
                                512, 512, 1024, 32, 32, 1),
                &ph->serr)) {

    assert(0);
  }

  memory_clusage_(ph);
  return;
}

static int memory_check(struct head *ph);
static void memory_on_exit(struct head *ph);

void free_(struct head *ph) {

  inter_free(&ph->scontext);
  memory_on_exit(ph);
  memory_check(ph);
  return;
}

static int memory_check(struct head *ph) {

  struct mstat_std smstat;


  mstat_std(&ph->smemory, &smstat);



  if ((uintp)ph->vsexp != smstat.vsexp ||
      (uintp)ph->vstr != smstat.vstr ||
      (uintp)ph->vstrtotal != smstat.vstrtotal ||
      (uintp)ph->vfunc != smstat.vfunc ||
      (uintp)ph->vvar != smstat.vvar) {

    printf("wrong memory usage: real != expected: {vsexp:%"PRIuPTR
           " vstr:%"PRIuPTR" vstrtotal:%"PRIuPTR
           " vfunc:%"PRIuPTR" vvar:%"PRIuPTR"} != "
           "{vsexp:%"PRIuPTR
           " vstr:%"PRIuPTR" vstrtotal:%"PRIuPTR
           " vfunc:%"PRIuPTR" vvar:%"PRIuPTR"}\n",

           smstat.vsexp, smstat.vstr, smstat.vstrtotal,
           smstat.vfunc, smstat.vvar,

           ph->vsexp, ph->vstr, ph->vstrtotal,
           ph->vfunc, ph->vvar);

    return FALSE;
  }

  return TRUE;
}

void memory_chusage_(struct head *ph,
                     intp vsexp,
                     intp vstr, intp vstrtotal, intp vfunc, intp vvar) {

  ph->vsexp += vsexp;
  ph->vstr += vstr;
  ph->vstrtotal += vstrtotal;
  ph->vfunc += vfunc;
  ph->vvar += vvar;

  if (ph->vsexp < 0) { ph->vsexp = 0; }
  if (ph->vstr < 0) { ph->vstr = 0; }
  if (ph->vstrtotal < 0) { ph->vstrtotal = 0; }
  if (ph->vfunc < 0) { ph->vfunc = 0; }
  if (ph->vvar < 0) { ph->vvar = 0; }
  return;
}

void memory_clusage_(struct head *ph) {

  ph->vsexp = 0, ph->vstr = 3, ph->vstrtotal = 5,
          ph->vfunc = FUNCTIONS, ph->vvar = 2;
  return;
}

static void memory_on_exit(struct head *ph) {

  ph->vsexp = 0, ph->vstr = 0, ph->vstrtotal = 0,
          ph->vfunc = 0, ph->vvar = 0;
  return;
}

/* ------------------------------------------------------------------------- */

struct verification_tree {

  const char *pfilename;
  uintp vnamelen;

  const char *panswer;
  uintp vanslen;

  struct verification_tree
          *pnext;
};

struct verification {

  int vpassed;

  struct verification_tree
          *pfirst,
          *pcurrent;
};

static void hook_inter_ret(struct inter *pcontext,
                           const char *pfilename,
                           struct scell *proot, void *ptr);

static struct verification*
        create_verification_info(const char *panswers,
                                 const char *pfilenames,
                                 uintp vtrees);

static void free_verification(struct verification *pverif);

int enterline(struct head *ph,
              const char *pinput,
              const char *panswers,
              const char *pfilenames, uintp vtrees, int vresult) {

  struct verification *pverif;


  pverif = create_verification_info(panswers, pfilenames, vtrees);

  inter_sethook_inter_ret(&ph->scontext, (void*)pverif, hook_inter_ret);

  if (vresult == ((lisp_enter_str(&ph->scontext, pinput))? FALSE : TRUE) &&
      pverif->vpassed == TRUE) {

    intern_assert(memory_check(ph) == TRUE);
    free_verification(pverif);
    return TRUE;
  }

  free_verification(pverif);
  return FALSE;
}

/* ------------------------------------------------------------------------- */

static void hook_inter_ret(struct inter *pcontext,
                           const char *pfilename,
                           struct scell *proot, void *ptr) {

  struct verification *pverif = ptr;
  struct verification_tree *ptree;


  UNUSED(pcontext);
  assert(pverif->pcurrent != NULL);

  ptree = pverif->pcurrent;


  {
    uintp vlen = strlen(pfilename);

    if (vlen != ptree->vnamelen ||
        memcmp(pfilename, ptree->pfilename, vlen) != 0) {

      pverif->vpassed = FALSE;

      printf(":: wrong file name of tree \"%.*s\" != \"%.*s\"\n",
             (uint)vlen, pfilename, (uint)ptree->vnamelen, ptree->pfilename);
    }
  }

  {
    static char sanswer[512];
    uintp vlen, verrpos;

    vlen = print_tree(proot, sanswer, 512, &verrpos);

    assert(ptree->vanslen <= 512);

    if (vlen != ptree->vanslen ||
        memcmp(sanswer, ptree->panswer, vlen) != 0) {

      pverif->vpassed = FALSE;

      printf(":%"PRIuPTR": \"%.*s\" != \"%.*s\"\n", verrpos,
             (uint)vlen, sanswer, (uint)ptree->vanslen, ptree->panswer);
    }
  }

  pverif->pcurrent = ptree->pnext;
  return;
}

/* ------------------------------------------------------------------------- */

static void create_verification_tree(struct verification_tree *ptree,
                                     const char **ppanswers,
                                     const char **ppfilenames) {

  ptree->pfilename = *ppfilenames;
  ptree->vnamelen = strlen(*ppfilenames);

  ptree->panswer = *ppanswers;
  ptree->vanslen = strlen(*ppanswers);

  *ppfilenames += ptree->vnamelen + 1;
  *ppanswers += ptree->vanslen + 1;

  return;
}

static struct verification*
        create_verification_info(const char *panswers,
                                 const char *pfilenames,
                                 uintp vtrees) {

  struct verification *pverif;

  assert(panswers != NULL);
  assert(pfilenames != NULL);
  assert(vtrees != 0);


  if ((pverif = malloc(sizeof(struct verification)
                       + sizeof(struct verification_tree) * vtrees)) == NULL) {

    assert(0);
  }

  pverif->pcurrent = pverif->pfirst = (void*)(pverif+1);


  for ( ; ; ) {

    pverif->pcurrent->pnext = NULL;

    create_verification_tree(pverif->pcurrent, &panswers, &pfilenames);

    if (--vtrees == 0) {

      break;
    }

    pverif->pcurrent->pnext = pverif->pcurrent+1;
    pverif->pcurrent += 1;
  }

  pverif->pcurrent = pverif->pfirst;
  pverif->vpassed = TRUE;

  return pverif;
}

static void free_verification(struct verification *pverif) {

  free(pverif);
  return;
}

/* ------------------------------------------------------------------------- */
