/*------------------------------------------------------------------------------*/

#ifndef TYPES_H
#define TYPES_H

#include "./lib/types.h"
#include "./lib/assert.h"
#include "./lib/list.h"

#include "./options.h"
#include "./error.h"
#include "./string.h"
#ifdef OPTION_MATH
#include "./strmath.h"
#endif

#ifndef NDEBUG
#include <stdio.h>
#endif

/*------------------------------------------------------------------------------*/

#define TYPEGET(V)    ((V) >> (TYPE_P) & (TYPE_M))
#define TYPESET(V,T)  ((V) = (V) & ~((TYPE_M)<<(TYPE_P)) | (T)<<(TYPE_P))

#define MARKGET(V)    ((V) >> (MARK_P) & (MARK_M))
#define MARKSET(V,T)  ((V) = (V) & ~((MARK_M)<<(MARK_P)) | (T)<<(MARK_P))

#define ERRORFLAG_GET(V)  ((V) >> (ERROR_P) & (ERROR_M))
#define ERRORFLAG_ON(V)   ((V) = (V) & ~((ERROR_M)<<(ERROR_P)) | (ERROR_ON)<<(ERROR_P))
#define ERRORFLAG_OFF(V)  ((V) = (V) & ~((ERROR_M)<<(ERROR_P)))

#define BITTAIL_GET(V)    ((V) >> (BITTAIL_P) & (BITTAIL_M))
#define BITTAIL_SET(V,T)  ((V) = (V) & ~((BITTAIL_M)<<(BITTAIL_P)) | (T)<<(BITTAIL_P))

/* тип */
#define TYPE_M         3
#define TYPE_P         0
#define TYPE_SEXP      0
#define TYPE_OBJECT    1
#define TYPE_NIL       2

/* маркировка */
#define MARK_M         1
#define MARK_P         2
#define MARK_NON       0
#ifdef OPTION_NUMBERS
#define MARK_NUMBER    1
#endif

#define BITTAIL_M 7
#define BITTAIL_P 3
static_assert(BITTAIL_M + 1 >= CHAR_BIT, "");

/* точка ошибки */
#define ERROR_M        1
#define ERROR_P        3
#define ERROR_OFF      0
#define ERROR_ON       1

/* свободные биты в последнем байте */
/* #define BFREE_M        3 */
/* #define BFREE_P        8 */

#define INITNIL(V)  ((V) = (TYPE_NIL<<TYPE_P))

/*------------------------------------------------------------------------------*/

struct snode {

  struct scell {

    union {

      struct string
              sobject;

      struct snode
              *psexp;
    } u; /* если TYPEGET(vtype) == TYPE_NIL, зачения полей не определено */

    uint vtype;

#ifdef OPTION_MATH
    uint vpoint; /* число знаков после запятой */
#endif

  } s[2];
};

/* ------------------------------------------------------------------------- */

#define FUNTYPE_EXTERN 0
#define FUNTYPE_INTERN 1

struct function {

  struct scell sname;
  uint  vtype;

  union {

    struct function_extern {

      void (*pfun)(struct error_stack **pcnterr,
                   struct snode *pargs, struct scell *pret);

    } sextern;

    struct function_intern {

      struct scell
              sargs,
              sbody;

    } sintern;

  } u;

  struct dlist_node sn;
};

struct variable {

  struct scell
          sname,
          svalue;

  struct dlist_node sn;
};

/* ------------------------------------------------------------------------- */

#include "./lib/io.h"

struct filedesc {

  IOFILE  sd;
  uint    vmode;
  uintp   vid;
  struct dlist_node
          sn;
};

struct memory;

struct inter {

  struct error_stack
          *perr;

  struct memory
          *pm;

#ifdef OPTION_MATH
  struct sm_head
          strmh;
#endif

  struct dlist_head
          sfunctions, /* struct function */
          sfiles; /* struct filedesc */

#ifdef OPTION_HOOK_INTER_RET
  void *ptr;
  void (*pf_hook_inter_ret)(struct inter *pcontext,
                            const char *pfilename,
                            struct scell *proot, void *ptr);
#endif

  uintp   vscope, vfilesd;

  struct dlist_node
          *pcurspace; /* граница текущего простарнства имён, переменные выше - в
                         других пространствах. */

  struct dlist_head /* struct variable */
          sglobalvars,
          svariables; /*
                        от первого к последнему, новые - последние. с
                        группировкой по пространству, по локальным (которых нет,
                        но например let). с какого момента начинается конкретное
                        пространство определяется местом их задания - на стеке.

                        поиск должен проходить от последних к первым, реализуя
                        затенение локальными переменными, до граници своего
                        (текущего) пространстава имён.
                      */
};

#endif /* TYPES_H */

/* ------------------------------------------------------------------------- */
