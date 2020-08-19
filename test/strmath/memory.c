/* ------------------------------------------------------------------------- */

#include "./common.h"

void memstat_init(struct memstat *pm) {

  pm->vallocated_chunks = 0;
  pm->vallocated_memory = 0;
  return;
}

int memstat_isclear(struct memstat *pm) {

  return (pm->vallocated_chunks == 0 && pm->vallocated_memory == 0);
}

int some_stralloc(void *ptr, struct string *ps, uintp vsize) {

  struct memstat *pm = ptr;

  if ((ps->pstr = malloc(vsize)) == NULL) {

    return 1;
  }

  memset(ps->pstr, '#', vsize);
  ps->vlen = vsize;

  pm->vallocated_chunks += 1;
  pm->vallocated_memory += vsize;
  return 0;
}

int some_strrealloc_l(void *ptr, struct string *ps, uintp vnewlen) {

  char *pnew;
  struct memstat *pm = ptr;


  if ((pnew = malloc(vnewlen)) == NULL) {

    return 1;
  }

  if (vnewlen > ps->vlen) {

    memcpy(pnew+vnewlen-ps->vlen, ps->pstr, ps->vlen);

  } else {

    memcpy(pnew, ps->pstr+ps->vlen-vnewlen, vnewlen);
  }

  pm->vallocated_memory -= ps->vlen;

  free(ps->pstr);
  ps->pstr = pnew;
  ps->vlen = vnewlen;

  pm->vallocated_memory += ps->vlen;
  return 0;
}

int some_strrealloc_r(void *ptr, struct string *ps, uintp vnewlen) {

  char *pnew;
  struct memstat *pm = ptr;


  if ((pnew = realloc(ps->pstr, vnewlen)) == NULL) {

    return 1;
  }

  pm->vallocated_memory -= ps->vlen;

  ps->pstr = pnew;
  ps->vlen = vnewlen;

  pm->vallocated_memory += ps->vlen;
  return 0;
}

void some_strfree(void *ptr, struct string *p) {

  struct memstat *pm = ptr;

  free(p->pstr);
  p->pstr = NULL;

  pm->vallocated_chunks -= 1;
  pm->vallocated_memory -= p->vlen;
}

int make_string(void *ptr, struct string *p, const char *pstr) {

  if (some_stralloc(ptr, p, strlen(pstr))) {

    return 1;
  }

  memcpy(p->pstr, pstr, p->vlen);
  return 0;
}

/* ------------------------------------------------------------------------- */
