/* ------------------------------------------------------------------------- */

#include "./memory.h"

void* memalloc(struct memory *ph, uintp vsize) {

  void *ptr;


  if ((ptr = ph->memalloc(ph, vsize)) == NULL) {

    ERROR(ph->perr, OUT_OF_MEMORY);
  }

  return ptr;
}

struct snode* sexpalloc(struct memory *ph) {

  void *ptr;


  if ((ptr = ph->sexpalloc(ph)) == NULL) {

    ERROR(ph->perr, OUT_OF_MEMORY);
  }

  return ptr;
}

void strdeclare(struct memory *ph, struct string *pstr, const char *ptr,
                uintp vlen) {


  if (ph->strdeclare(ph, pstr, ptr, vlen)) {

    ERROR(ph->perr, OUT_OF_MEMORY);
  }

  return;
}

void stralloc(struct memory *ph, struct string *pstr, uintp vlen) {


  if (ph->stralloc(ph, pstr, vlen)) {

    ERROR(ph->perr, OUT_OF_MEMORY);
  }

  return;
}

void strrealloc_r(struct memory *ph, struct string *pstr, uintp vlen) {


  if (ph->strrealloc_r(ph, pstr, vlen)) {

    ERROR(ph->perr, OUT_OF_MEMORY);
  }

  return;
}

void strrealloc_l(struct memory *ph, struct string *pstr, uintp vlen) {


  if (ph->strrealloc_l(ph, pstr, vlen)) {

    ERROR(ph->perr, OUT_OF_MEMORY);
  }

  return;
}

struct function* funalloc(struct memory *ph) {

  void *ptr;


  if ((ptr = ph->funalloc(ph)) == NULL) {

    ERROR(ph->perr, OUT_OF_MEMORY);
  }

  return ptr;
}

struct variable* varalloc(struct memory *ph) {

  void *ptr;


  if ((ptr = ph->varalloc(ph)) == NULL) {

    ERROR(ph->perr, OUT_OF_MEMORY);
  }

  return ptr;
}

struct filedesc* fdalloc(struct memory *ph) {

  void *ptr;


  if ((ptr = ph->fdalloc(ph)) == NULL) {

    ERROR(ph->perr, OUT_OF_MEMORY);
  }

  return ptr;
}

/* ------------------------------------------------------------------------- */
