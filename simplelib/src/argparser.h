/*---------------------------------------------------------------------------*/

#ifndef ARGPARSER_H
#define ARGPARSER_H

#include "./types.h"

/*---------------------------------------------------------------------------*/
/* !!! мысль такая есть, чтобы все аргументы перевести в указатели

   !!! в следующий раз описывай проблему чётче. я считаю что это полное говно и
       всё нужно выкинуть нахуй

   !!! или можно перевести все аргументы на байты - конвертировать внутри
       макросов, а птом в функциях собирать из байтов обратно в типы.
*/

#define OPTION_CALLBACK(NAMAE, PTR, FUNC)                   \
  { { NAMAE, sizeof(NAMAE)-1 }, {{ (void*)(PTR) }}, FUNC }

#define OPTION_FLAG_UINTP(NAMAE, PUINTP, MASK, FLAG)                    \
  { { NAMAE, sizeof(NAMAE)-1 }, {{ (void*)(PUINTP), (void*)(MASK), (void*)(FLAG) }}, arg_flag_uintp }

#define OPTION_STRING(NAMAE, PPUCHAR)                                 \
  { { NAMAE, sizeof(NAMAE)-1 }, {{ (void*)(PPUCHAR) }}, arg_string }

#define OPTION_ENUM_UINTP(NAMAE, PUINTP, N, PPUCHAR)                    \
  { { NAMAE, sizeof(NAMAE)-1 }, {{ (void*)(PUINTP), (void*)(N), (void*)(PPUCHAR) }}, arg_enum_uintp }

#define OPTION_INIT_UINTP(NAMAE, PUINTP)                                \
  { { NAMAE, sizeof(NAMAE)-1 }, {{ (void*)(PUINTP) }}, arg_init_uintp }

#define OPTION_INIT_PAIR_UINTP(NAMAE, PUINTPX, PUINTPY)                 \
  { { NAMAE, sizeof(NAMAE)-1 }, {{ (void*)(PUINTPX), (void*)(PUINTPY) }}, arg_init_pair_uintp }

struct option;
struct argerror;


/*
  число опций
  структура опций
  адресная база
  входящих аргументов
  началать с аргумента номер
  аргументов всего
  аргументы
*/
int argparser(uint vopt, const struct option *popt, void *pbase,
              int *pargi, int vargc, char **pargv,
              struct argerror *perr);

/*
  с проверкой на дубликацию опций

  !!! плохая функция. не учитываются сокращения и тогда это просто бред.  сделать
  бы просто, чтобы могло вызываться столько опций из набора с одним именем,
  сколько есть, и там навешивать проверку...
*/
int argparser_nd(uint vopt, const struct option *popt, void *pbase,
                 int *pargi, int vargc, char **pargv,
                 struct argerror *perr);


/*
  вывод ошибки в stderr
*/
void argparser_printerr(struct argerror *perr);

/*---------------------------------------------------------------------------*/

struct option {

  struct {
    char   *p;
    uintp   v;
  } snme;

  struct {
    void    *p[3];
  } u;

  int   (*pfun)(void * const *popt, void *pbase, int *pargi, int vargc, char **pargv,
                struct argerror *perr);
};


/* устанавливается если не NULL и только при возникновении ошибки
   vargi значения всегда валидные - не выходят за границы
*/
struct argerror {

  int     vargi,
          vcod;
  uintp   vpos;
};

#define ARGPARSER_CODES_N            11
#define ARGPARSER_NOT_A_ERROR         0
#define ARGPARSER_UNEXPECTED_SYMBOL   1
#define ARGPARSER_UINTP_OVERFLOW      2
#define ARGPARSER_UNRECOGNIZED_OPTION 3
#define ARGPARSER_UNEXPECTED_END      4
#define ARGPARSER_ZEROLENGTH_ARGUMENT 5
#define ARGPARSER_EMPTY_ENUM          6
#define ARGPARSER_WRONG_ENUM_KEYWORD  7
#define ARGPARSER_INCORRECT_PAIR      8
#define ARGPARSER_DUPLICATED_OPTION   9
#define ARGPARSER_UNDEFINED          10

#ifndef ARGPARSER_C

#define ARGP_ERRRET(PERR, ARG, POS, CODE)       \
  do {                                          \
    if ((PERR) != NULL) {                       \
      (PERR)->vargi = (ARG);                    \
      (PERR)->vpos = (POS);                     \
      (PERR)->vcod = (CODE);                    \
    }                                           \
    return 1;                                   \
  } while(0)


int arg_init_uintp(void * const *popt, void *pbase, int *pargi, int vargc, char **pargv,
                   struct argerror *perr);

int arg_flag_uintp(void * const *popt, void *pbase, int *pargi, int vargc, char **pargv,
                   struct argerror *perr);

int arg_string(void * const *popt, void *pbase, int *pargi, int vargc, char **pargv,
               struct argerror *perr);

int arg_enum_uintp(void * const *popt, void *pbase, int *pargi, int vargc, char **pargv,
                   struct argerror *perr);

int arg_init_pair_uintp(void * const *popt, void *pbase, int *pargi, int vargc, char **pargv,
                        struct argerror *perr);

#endif /* ARGPARSER_C */

#endif /* ARGPARSER_H */

/*---------------------------------------------------------------------------*/
