/* ------------------------------------------------------------------------- */

#ifndef STRING_H
#define STRING_H

#include "./lib/types.h"


#ifndef STRING_T
#define STRING_T
struct string {

  char *pstr;
  uintp vlen;
};
#endif

int string_eq(const struct string *pa, const struct string *pb);

#ifdef OPTION_NUMBERS
#ifdef OPTION_MATH

int string_isnumber(const struct string *pa,
                    uintp *ppointpos, uintp *pfirstnonz);

#else

int string_isnumber(const struct string *pa);

#endif
#endif

struct memory;

/*
  при нехватке памяти вызывает искючение
*/
char* string_to_cstr(struct memory *pm, const struct string *pobj);


/*
  при нехватке памяти вызывает искючение
*/
void string_from_uintp(struct memory *pm, struct string *pobj, uintp vnum);

/* врзврат != 0 - ошибка переполнения */
int string_to_uintp(const struct string *pobj, uintp *pnum);


/*
  pstrp - входящее и возвратное
  psetp - возвратное
*/
void string_scan(const struct string *pstr, uintp *pstrp,
                 const struct string *pset, uintp *psetp);

#endif /* STRING_H */


/* ------------------------------------------------------------------------- */
