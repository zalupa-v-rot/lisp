/* ------------------------------------------------------------------------- *
 * внешний - лицевой функционал                                              *
 * ------------------------------------------------------------------------- */

#ifndef INTER_H
#define INTER_H

#include "./types.h"

/* ------------------------------------------------------------------------- */

struct decfunction {

  void (*pfunction)(struct error_stack **pcnterr,
                    struct snode *pargs, struct scell *pret);
  const char *pname;
};


/*
  если произошла ошибка, значит не все функции и/или переменные определились.
  но структура полностью валидная (например, её можно освободить).

  возврат != 0 - ошибка
*/
int inter_init(struct inter *pcontext, struct memory *pmh,
               struct error_stack *perr);

/*
  pname используется как есть. !!! функции с рдинаковым именем не заменяются -
  будет всегда использоваться первое.

  возврат != 0 - ошибка.
*/
int inter_declarefunction(struct inter *pcontext,
                          const struct decfunction *pdecfun, uintp vfunn);


#ifdef OPTION_HOOK_INTER_RET
void inter_sethook_inter_ret(struct inter *pcontext,
                             void *ptr,
                             void (*callback)(struct inter *pcontext,
                                              const char *pfilename,
                                              struct scell *proot,
                                              void *ptr));
#endif

/*
  освободить всю память (функции и переменные) занимаемую контектстом.

  после возврата использовать контекст нельзя. только inter_init().
*/
void inter_free(struct inter *pcontext);

/*
  интерпретировать.

  в случае ошибки, память никуда не утекает - все обьекты динамической памяти
  или доступны в дереве или освобождены. (утверждение требует теста !!!)

  возврат != 0 - ошибка
*/
int inter(struct inter *pcontext, struct scell *proot);

#endif /* INTER_H */

/* ------------------------------------------------------------------------- */
