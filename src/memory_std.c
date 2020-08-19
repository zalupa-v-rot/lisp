/* ------------------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h> /* memcpy() */

#include "./memory_std.h"
#include "./lib/ofchk.h"

/* ------------------------------------------------------------------------- */

void* memalloc_std(void *ph, uintp vlen) {

  void *ptr;

  assert(ph != NULL);
  assert(vlen > 0);
  UNUSED(ph);


  if ((ptr = malloc(vlen)) == NULL) {

    return NULL;
  }

  return ptr;
}

void memfree_std(void *ph, void *ptr) {

  assert(ph != NULL);
  assert(ptr != NULL);
  UNUSED(ph);


  free(ptr);
  return;
}

struct snode* sexpalloc_std(void *ph) {

  struct snode *psexp;
  struct memory_std *pms = container_of(ph, struct memory_std, sm);

  assert(ph != NULL);


  if (pms->vsexp == pms->vsexpmax) {

    return NULL;
  }

  if ((psexp = malloc(sizeof(struct snode))) == NULL) {

    return NULL;
  }

  pms->vsexp++;
  return psexp;
}

void sexpfree_std(void *ph, struct snode *psexp) {

  struct memory_std *pms = container_of(ph, struct memory_std, sm);

  assert(ph != NULL);
  assert(psexp != NULL);
  assert(pms->vsexp > 0);


  pms->vsexp--;
  free(psexp);
  return;
}

int stralloc_std(void *ph, struct string *pstr, uintp vlen);

int strdeclare_std(void *ph, struct string *pstr, const char *ptr, uintp vlen) {

  assert(ph != NULL);
  assert(pstr != NULL);
  assert(ptr != NULL);
  assert(vlen > 0);


  if (stralloc_std(ph, pstr, vlen)) {

    return 1;
  }

  memcpy(pstr->pstr, ptr, vlen);

  return 0;
}

int stralloc_std(void *ph, struct string *pstr, uintp vlen) {

  struct memory_std *pms = container_of(ph, struct memory_std, sm);
  void *p;

  assert(ph != NULL);
  assert(pstr != NULL);
  assert(vlen > 0);


  if (pms->vstr == pms->vstrmax) {

    return 1;
  }

  if (ofchk_add_uintp(pms->vstrtotal, vlen) ||
      pms->vstrtotal + vlen > pms->vstrtotalmax) {

    return 1;
  }

  if ((p = malloc(vlen)) == NULL) {

    return 1;
  }


  pms->vstr++;
  pms->vstrtotal += vlen;

  pstr->pstr = p;
  pstr->vlen = vlen;

  return 0;
}

int strrealloc_r_std(void *ph, struct string *pstr, uintp vnewlen) {

  struct memory_std *pms = container_of(ph, struct memory_std, sm);
  void *p;

  assert(ph != NULL);
  assert(pstr != NULL);
  assert(vnewlen > 0);
  assert(pstr->vlen <= pms->vstrtotal);


  if (ofchk_add_uintp(pms->vstrtotal - pstr->vlen, vnewlen) ||
      pms->vstrtotal - pstr->vlen + vnewlen > pms->vstrtotalmax) {

    return 1;
  }

  if ((p = realloc(pstr->pstr, vnewlen)) == NULL) {

    return 1;
  }

  pms->vstrtotal = pms->vstrtotal - pstr->vlen + vnewlen;

  pstr->pstr = p;
  pstr->vlen = vnewlen;

  return 0;
}

int strrealloc_l_std(void *ph, struct string *pstr, uintp vnewlen) {

  struct memory_std *pms = container_of(ph, struct memory_std, sm);
  char *pnew;

  assert(ph != NULL);
  assert(pstr != NULL);
  assert(vnewlen > 0);
  assert(pstr->vlen <= pms->vstrtotal);


  if (ofchk_add_uintp(pms->vstrtotal - pstr->vlen, vnewlen) ||
      pms->vstrtotal - pstr->vlen + vnewlen > pms->vstrtotalmax) {

    return 1;
  }

  if ((pnew = malloc(vnewlen)) == NULL) {

    return 1;
  }

  if (vnewlen > pstr->vlen) {

    memcpy(pnew+vnewlen-pstr->vlen, pstr->pstr, pstr->vlen);

  } else {

    memcpy(pnew, pstr->pstr+pstr->vlen-vnewlen, vnewlen);
  }

  pms->vstrtotal = pms->vstrtotal - pstr->vlen + vnewlen;

  free(pstr->pstr);
  pstr->pstr = pnew;
  pstr->vlen = vnewlen;

  return 0;
}

void strfree_std(void *ph, struct string *pstr) {

  struct memory_std *pms = container_of(ph, struct memory_std, sm);

  assert(ph != NULL);
  assert(pstr != NULL);
  assert(pstr->pstr != NULL);
  assert(pstr->vlen > 0);
  assert(pms->vstr > 0);
  assert(pms->vstrtotal >= pstr->vlen);

  pms->vstr--;
  pms->vstrtotal -= pstr->vlen;

  free(pstr->pstr);
#ifndef NDEBUG
  pstr->pstr = NULL;
#endif

  return;
}

struct function* funalloc_std(void *ph) {

  struct function *pfun;
  struct memory_std *pms = container_of(ph, struct memory_std, sm);

  assert(ph != NULL);


  if (pms->vfunc == pms->vfuncmax) {

    return NULL;
  }

  if ((pfun = malloc(sizeof(struct function))) == NULL) {

    return NULL;
  }

  pms->vfunc++;
  return pfun;
}

void funfree_std(void *ph, struct function *pfun) {

  struct memory_std *pms = container_of(ph, struct memory_std, sm);

  assert(ph != NULL);
  assert(pfun != NULL);
  assert(pms->vfunc > 0);

  pms->vfunc--;
  free(pfun);
  return;
}

struct variable* varalloc_std(void *ph) {

  struct variable *pvar;
  struct memory_std *pms = container_of(ph, struct memory_std, sm);

  assert(ph != NULL);


  if (pms->vvar == pms->vvarmax) {

    return NULL;
  }

  if ((pvar = malloc(sizeof(struct variable))) == NULL) {

    return NULL;
  }

  pms->vvar++;
  return pvar;
}

void varfree_std(void *ph, struct variable *pvar) {

  struct memory_std *pms = container_of(ph, struct memory_std, sm);

  assert(ph != NULL);
  assert(pvar != NULL);
  assert(pms->vvar > 0);

  pms->vvar--;
  free(pvar);
  return;
}

struct filedesc* fdalloc_std(void *ph) {

  struct filedesc *pfd;
  struct memory_std *pms = container_of(ph, struct memory_std, sm);

  assert(ph != NULL);


  if (pms->vfd == pms->vfdmax) {

    return NULL;
  }

  if ((pfd = malloc(sizeof(struct filedesc))) == NULL) {

    return NULL;
  }

  pms->vfd++;
  return pfd;
}


void fdfree_std(void *ph, struct filedesc *pfd) {

  struct memory_std *pms = container_of(ph, struct memory_std, sm);

  assert(ph != NULL);
  assert(pfd != NULL);
  assert(pms->vfd > 0);

  pms->vfd--;
  free(pfd);
  return;
}

/* ------------------------------------------------------------------------- */

struct memory* memory_std_init(struct memory_std *pms,
                               uintp vsexpmax,
                               uintp vstrmax,
                               uintp vstrtotalmax,
                               uintp vfuncmax,
                               uintp vvarmax,
                               uintp vfdmax) {

  assert(pms != NULL);

  pms->vsexp = 0;
  pms->vstr = 0;
  pms->vstrtotal = 0;
  pms->vfunc = 0;
  pms->vvar = 0;
  pms->vfd = 0;

  pms->vsexpmax = vsexpmax;
  pms->vstrmax = vstrmax;
  pms->vstrtotalmax = vstrtotalmax;
  pms->vfuncmax = vfuncmax;
  pms->vvarmax = vvarmax;
  pms->vfdmax = vfdmax;


  pms->sm.memalloc = memalloc_std;
  pms->sm.memfree = memfree_std;

  pms->sm.sexpalloc = sexpalloc_std;
  pms->sm.sexpfree = sexpfree_std;

  pms->sm.strdeclare = strdeclare_std;
  pms->sm.stralloc = stralloc_std;
  pms->sm.strrealloc_r = strrealloc_r_std;
  pms->sm.strrealloc_l = strrealloc_l_std;
  pms->sm.strfree = strfree_std;

  pms->sm.funalloc = funalloc_std;
  pms->sm.funfree = funfree_std;

  pms->sm.varalloc = varalloc_std;
  pms->sm.varfree = varfree_std;

  pms->sm.fdalloc = fdalloc_std;
  pms->sm.fdfree = fdfree_std;

  return &pms->sm;
}

void mstat_std(struct memory_std *pms,
               struct mstat_std *pstat) {

  pstat->vsexp = pms->vsexp;
  pstat->vstr = pms->vstr;
  pstat->vstrtotal = pms->vstrtotal;
  pstat->vfunc = pms->vfunc;
  pstat->vvar = pms->vvar;
  return;
}

/* ------------------------------------------------------------------------- */
