/* ------------------------------------------------------------------------- *
 * интерфейс для keywords                                                    *
 * ------------------------------------------------------------------------- */

#ifndef INTER_CORE_H
#define INTER_CORE_H

#include "./types.h"
#include "./snode_wrapper.h"

/* ------------------------------------------------------------------------- */

/*
  добавить внутр. функцию.

  значения копируются.

  при неудаче выбрасывает ошибку. совпадающая функция (если таковая найдётся) не
  будет затёрта.
*/
void defun_intern(struct error_stack **pcnterr,
                 const struct scell *pname,
                 const struct scell *pargs, const struct scell *pbody);

/*
  изменить переменную (в текущ. пространстве или глобальном) или добавить
  (в текущ. пространство или глобальное)

  значения копируются.

  при неудаче выбрасывает ошибку. если переменная должна была быть создана, она
  может оказаться не полной, но с nil на листьях. если должна была быть
  переопределена, то старое значение сохранится.
*/
void setvar(struct error_stack **pcnterr,
            const struct scell *pname, const struct scell *pvalue);

/*
  добавить переменную (в текущ. пространство или глобальное)

  значения копируются.

  при неудаче выбрасывает ошибку. и часть переменной может быть не созданно, но
  с nil на листьях.
*/
void defvar(struct error_stack **pcnterr,
            const struct scell *pname, const struct scell *pvalue);

/*
  область переменных. войти-выйти.
  возврат первого подаётся на аргумент последнего.
*/
void* varscope_enter(struct error_stack **pcnterr);
void varscope_exit(struct error_stack **pcnterr, void*);


/*
  исполнить дерево

  при неудаче выбрасывает ошибку.
*/
void evalsexp(struct error_stack **pcnterr, struct scell *pexp);


/*
  открыть, прочитать из, записать в, закрыть файл.

  при неудаче выбрасывают ошибку.
*/
void open_file(struct error_stack **pcnterr,
               const struct scell *path,
               const struct scell *pmode,
               struct scell *pret);

void read_file(struct error_stack **pcnterr,
               const struct scell *pfd,
               const struct scell *psize,
               struct scell *pret);

void write_file(struct error_stack **pcnterr,
                const struct scell *pfd,
                const struct scell *pdata,
                struct scell *pret);

void close_file(struct error_stack **pcnterr,
                const struct scell *pfd,
                struct scell *pret);

#endif /* INTER_CORE_H */

/* ------------------------------------------------------------------------- */
