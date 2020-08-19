/* ------------------------------------------------------------------------- */

#ifndef MEMORY_H
#define MEMORY_H

#include "./types.h"

/* вызывает исключение */
void* memalloc(struct memory *ph, uintp vsize);

#define memfree(PM, PTR)                        \
  (PM)->memfree((PM), (PTR))

/* вызывает исключение */
struct snode* sexpalloc(struct memory *ph);

#define sexpfree(PM, PSEXP)                     \
  (PM)->sexpfree((PM), PSEXP)

/* вызывают исключение */
void strdeclare(struct memory *ph, struct string *pstr, const char *ptr,
                uintp vlen);
void stralloc(struct memory *ph, struct string *pstr, uintp vlen);
void strrealloc_r(struct memory *ph, struct string *pstr, uintp vlen);
void strrealloc_l(struct memory *ph, struct string *pstr, uintp vlen);

#define strfree(PM, PSTR)                       \
  (PM)->strfree((PM), PSTR)

/* вызывает исключение */
struct function* funalloc(struct memory *ph);

#define funfree(PM, PFUN)                       \
  (PM)->funfree((PM), PFUN)

/* вызывает исключение */
struct variable* varalloc(struct memory *ph);

#define varfree(PM, PVAR)                       \
  (PM)->varfree((PM), PVAR)

/* вызывает исключение */
struct filedesc* fdalloc(struct memory *ph);

#define fdfree(PM, PFD)                         \
  (PM)->fdfree((PM), (PFD))

/* ------------------------------------------------------------------------- */

struct memory {

  struct error_stack *perr;

  void* (*memalloc)(void *ph, uintp vlen);
  void (*memfree)(void *ph, void *ptr);

  struct snode* (*sexpalloc)(void *ph);
  void (*sexpfree)(void *ph, struct snode *psexp);

  /* обьявление атомов при парсинге (на случай переиспользования памяти
     исходного текста) */
  int (*strdeclare)(void *ph, struct string *pstr, const char *ptr, uintp vlen);
  /* память атомов */
  int (*stralloc)(void *ph, struct string *pstr, uintp vlen);
  int (*strrealloc_r)(void *ph, struct string *pstr, uintp vlen);
  int (*strrealloc_l)(void *ph, struct string *pstr, uintp vlen);
  void (*strfree)(void *ph, struct string *pstr);

  struct function* (*funalloc)(void *ph);
  void (*funfree)(void *ph, struct function *pfun);

  struct variable* (*varalloc)(void *ph);
  void (*varfree)(void *ph, struct variable *pvar);

  struct filedesc* (*fdalloc)(void *ph);
  void (*fdfree)(void *ph, struct filedesc *pfd);
};

#endif /* MEMORY_H */

/* ------------------------------------------------------------------------- */
