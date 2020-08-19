/* ------------------------------------------------------------------------- *
 * весь внутренний (и весь другой) функционал который хоть как-то осмысленен *
 * отдельно                                                                  *
 * ------------------------------------------------------------------------- */

#include "./inter.h"
#include "./inter_kw.h"


/*
  удалить функцию.
*/
void del_function(struct inter *pcontext, struct function *pfunc);

/*
  найти функцию по имени.

  возврат == NULL - не нашлось
*/
struct function* find_function(struct inter *pcontext, struct string *pname);

/*
  пространство переменных. войти-выйти.
  возврат первого подаётся на аргумент последнего.
*/
void* varspace_enter(struct inter *pcontext);
void varspace_exit(struct inter *pcontext, void *psepar);


/*
  удалить конкретную переменную
*/
void del_variable(struct inter *pcontext,
                  struct dlist_head *plist, struct variable *pvar);

/*
  удалить все переменные до конкретного разделителя (исключающего) который может
  быть NULL
*/
void del_variables_upto(struct inter *pcontext, struct dlist_node *psepar);

/*
  найти переменную в конкретном списке до конкретного разделителя (исключающего)
  который может быть NULL

  возврат == NULL - не нашлось
*/
struct variable* find_variable2(struct dlist_head *plist,
                                struct dlist_node *psepar,
                                const struct string *pname);

/*
  найти переменную в текущем пространстве или в случае ненахода, в глобальном.

  возврат == NULL - не нашлось
*/
struct variable* find_variable(struct inter *pcontext,
                               const struct string *pname);

/*
  копировать дерево psexpsrc в pdstroot. pdstroot становится типа TYPE_SEXP.

  при неудаче выбрасывает ошибку. целевое дерево останется правильным с nil на
  листьях.
*/
void copysexp2(struct inter *pcontext,
               struct scell *pdstroot, const struct snode *psexpsrc);

/*
  копировать дерево psrc в pdst.

  при неудаче выбрасывает ошибку. целевое дерево останется правильным с nil на
  листьях.
*/
void copysexp(struct inter *pcontext,
              struct scell *pdst, const struct scell *psrc);

/*
  определить внешнюю - не интерпретируемую (keyword) функцию.

  возврат != 0 - успешно. при неудаче фукция не создаётся.
*/
int defun_extern(struct inter *pcontext,
                 const char *pname,
                 void (*pfun)(struct error_stack **pcnterr,
                              struct snode *pargs, struct scell *pret));

/*
  исполнение внешней-интерпретируемой функциит.


  при неудаче выбрасывает ошибку. часть переменных может быть созданна в новом
  пространсте. одна из переменных может быть созданна не полностью, но с nil на
  листьях. вся остальная сопутствующая память будет присоединена в дерево - тело
  в phead->s[1].
*/
void intern_call(struct inter *pcontext,
                 struct function_intern *pintern,
                 struct snode *phead, struct scell *pret);

/* ------------------------------------------------------------------------- */
