/* ------------------------------------------------------------------------- */

#include "./keywords.h"
#include "./inter_kw.h"

/* ------------------------------------------------------------------------- */

#define snode_check_completion(PCNXERR, PNODE, DIR) \
  do {                                              \
                                                    \
    if (snode_get_type(PNODE, DIR) != TYPE_NIL) {   \
                                                    \
      ERROR(*(PCNXERR), INCORRECT_COMPLETION);      \
    }                                               \
                                                    \
  } while(0)

/*
  (list  s ...)
  ^      ^
  *>     *> ...
  ^ |
  | '-----v
  clear  ret
*/
void kw_list(struct error_stack **pcnterr,
             struct snode *phead, struct scell *pret) {

  struct snode *ppoint = phead;


  if (snode_move(&ppoint, FORWARD)) {

    snode_check_completion(pcnterr, ppoint, FORWARD);
    snode_set_nil((struct snode*)pret, 0);
    return;
  }

  {
    struct snode *plist;


    for (plist = ppoint; ; ) {

      evalsexp(pcnterr, snode_get_scell(plist, UPWARD));

      if (snode_move(&plist, FORWARD)) {

        snode_check_completion(pcnterr, plist, FORWARD);
        break;
      }
    }
  }

  snode_transfer((struct snode*)pret, 0, phead, FORWARD);
  return;
}


/*
  .         .-----+ модифицируется первый список
  .         |     | линкуются списки, проставляются nil в аргументах
  .         v     v
  (append   s     s ...)
  ^      .--^  .> ^
  *>     |  *> |  *> ...
  ^      |     |
  |      v    nil
  clear  ret
*/
void kw_append(struct error_stack **pcnterr,
               struct snode *phead, struct scell *pret) {

  struct snode *plist, *pend, *ppoint = phead;


  if (snode_move(&ppoint, FORWARD)) {

    snode_check_completion(pcnterr, ppoint, FORWARD);
    snode_set_nil((struct snode*)pret, 0);
    return;
  }


  plist = ppoint;
  evalsexp(pcnterr, snode_get_scell(plist, UPWARD));

  if (!snode_move((pend = plist, &pend), UPWARD)) {

    for ( ; !snode_move(&pend, FORWARD); );
  }

  for ( ; ; ) {

    if (snode_move(&ppoint, FORWARD)) {

      snode_check_completion(pcnterr, ppoint, FORWARD);
      break;
    }

    evalsexp(pcnterr, snode_get_scell(ppoint, UPWARD));

    if (snode_get_type(pend,  FORWARD) != TYPE_NIL) {

      ERROR(*pcnterr, WRONG_ARGUMENT);
    }

    snode_transfer(pend, FORWARD, ppoint, UPWARD);

    for ( ; !snode_move(&pend, FORWARD); );
  }

  snode_transfer((struct snode*)pret, 0, plist, UPWARD);
  return;
}


/*
  (car (s ...))   (car nil)
  ^     ^ --.      ^    ^
  |     *>  |      *>   *>nil
  |     ^   |      ^
  *>    *>nil      |       nil
  ^         v      |        v
  clear   ret      clear   ret
*/
void kw_car(struct error_stack **pcnterr,
            struct snode *phead, struct scell *pret) {

  struct snode *ppoint = phead;


  if (snode_move(&ppoint, FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  snode_check_completion(pcnterr, ppoint, FORWARD);
  evalsexp(pcnterr, snode_get_scell(ppoint, UPWARD));

  if (snode_get_type(ppoint, UPWARD) == TYPE_NIL) {

    snode_set_nil((struct snode*)pret, 0);
    return;
  }

  if (snode_move(&ppoint, UPWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  snode_transfer((struct snode*)pret, 0, ppoint, UPWARD);
  return;
}


/*
  (cdr (s ...))   (cdr nil)
  ^     ^          ^    ^
  |     *>         *>   *>nil
  |     ^ |        ^
  *>    *>nil      |       nil
  ^       v        |        v
  clear   ret      clear   ret
*/
void kw_cdr(struct error_stack **pcnterr,
            struct snode *phead, struct scell *pret) {

  struct snode *ppoint = phead;


  if (snode_move(&ppoint, FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  snode_check_completion(pcnterr, ppoint, FORWARD);
  evalsexp(pcnterr, snode_get_scell(ppoint, UPWARD));

  if (snode_get_type(ppoint, UPWARD) == TYPE_NIL) {

    snode_set_nil((struct snode*)pret, 0);
    return;
  }

  if (snode_move(&ppoint, UPWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  snode_transfer((struct snode*)pret, 0, ppoint, FORWARD);
  return;
}


/*
  (quote !s)
  ^      ^ <+
  *>     *>nil
  ^         |
  |         |
  clear    ret
*/
void kw_quote(struct error_stack **pcnterr,
              struct snode *phead, struct scell *pret) {

  struct snode *ppoint = phead;


  if (snode_move(&ppoint, FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  snode_check_completion(pcnterr, ppoint, FORWARD);
  snode_transfer((struct snode*)pret, 0, ppoint, UPWARD);
  return;
}


/*
  (cond (s ... s )   ... )
  ^      ^   .-^
  |      *>  | *> nil
  |      ^   |        ^
  *>     *>  |        *>
  ^          |
  |          v
  clear     ret
*/
void kw_cond(struct error_stack **pcnterr,
             struct snode *phead, struct scell *pret) {

  struct snode *ppoint = phead;


  if (snode_move(&ppoint, FORWARD)) {

    snode_check_completion(pcnterr, ppoint, FORWARD);
    snode_set_nil((struct snode*)pret, 0);
    return;
  }


  for ( ; ; ) {

    struct snode *pclause;


    if (snode_move((pclause = ppoint, &pclause), UPWARD)) {

      ERROR(*pcnterr, WRONG_ARGUMENT);
    }

    evalsexp(pcnterr, snode_get_scell(pclause, UPWARD));

    if (snode_get_type(pclause, UPWARD) == TYPE_NIL) {

      if (snode_move(&ppoint, FORWARD)) {

        snode_check_completion(pcnterr, ppoint, FORWARD);
        snode_set_nil((struct snode*)pret, 0);
        return;
      }

      continue;
    }

    for ( ; ; ) {

      if (snode_move(&pclause, FORWARD)) {

        snode_check_completion(pcnterr, pclause, FORWARD);
        snode_transfer((struct snode*)pret, 0, pclause, UPWARD);
        return;
      }

      evalsexp(pcnterr, snode_get_scell(pclause, UPWARD));
    }
  }
}


/*
  (defun !name (!args ...) !sexp ...)
  ^     .-^     ^           ^
  *>    | *>    *>          *> ...
  ^     |
  |     v
  clear ret
*/
void kw_defun(struct error_stack **pcnterr,
              struct snode *phead, struct scell *pret) {

  struct snode *pname, *pargs;


  if (snode_move((pname = phead, &pname), FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  /* имя функции */

  if (snode_get_type(pname, UPWARD) != TYPE_OBJECT) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  /* аргументы */

  if (snode_move((pargs = pname, &pargs), FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  /* проверка аргументов на то, что они - список объектов или nil */

  switch(snode_get_type(pargs, UPWARD)) {

  case TYPE_OBJECT: ERROR(*pcnterr, WRONG_ARGUMENT);

  case TYPE_NIL: break;

  case TYPE_SEXP:
    {
      struct snode *ppoint;

      snode_move((ppoint = pargs, &ppoint), UPWARD);

      for ( ; ; ) {

        if (snode_get_type(ppoint, UPWARD) != TYPE_OBJECT) {

          ERROR(*pcnterr, WRONG_ARGUMENT);
        }

        if (snode_move(&ppoint, FORWARD)) {

          snode_check_completion(pcnterr, ppoint, FORWARD);
          break;
        }
      }
    }
    break;
  }

  /* остальное - тело функции. проверка на завершающий nil. */
  {
    struct snode *ppoint;

    for (ppoint = pargs; !snode_move(&ppoint, FORWARD); );
    snode_check_completion(pcnterr, ppoint, FORWARD);
  }

  defun_intern(pcnterr,
               snode_get_scell(pname, UPWARD),
               snode_get_scell(pargs, UPWARD),
               snode_get_scell(pargs, FORWARD));

  /* возврат - имя функции. */

  snode_transfer((struct snode*)pret, 0, pname, UPWARD);
  return;
}


/*
  (eval s)
  ^     ^-.
  *>    *>nil
  ^       |
  |       v
  clear ret
*/
void kw_eval(struct error_stack **pcnterr,
             struct snode *phead, struct scell *pret) {


  if (snode_move(&phead, FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  snode_check_completion(pcnterr, phead, FORWARD);


  evalsexp(pcnterr, snode_get_scell(phead, UPWARD));
  evalsexp(pcnterr, snode_get_scell(phead, UPWARD));

  snode_transfer((struct snode*)pret, 0, phead, UPWARD);
  return;
}


/*
  (set o     val)
  ^    ^--.  ^--.
  *>   *> |  *> nil
  ^       |     |
  |       |     v
  |       v   pvar->svalue -. copy
  |       pvar->sname       v
  clear                    ret
*/
void kw_set(struct error_stack **pcnterr,
            struct snode *phead, struct scell *pret) {

  struct snode *pname, *pvalue;


  if (snode_move((pname = phead, &pname), FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  evalsexp(pcnterr, snode_get_scell(pname, UPWARD));

  if (snode_get_type(pname, UPWARD) != TYPE_OBJECT) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  if (!snode_move((pvalue = pname, &pvalue), FORWARD)) {

    evalsexp(pcnterr, snode_get_scell(pvalue, UPWARD));
  }

  snode_check_completion(pcnterr, pvalue, FORWARD);

  setvar(pcnterr, snode_get_scell(pname, UPWARD),
         snode_get_scell(pvalue, (pvalue == pname)?FORWARD:UPWARD));

  /* возврат - значение. */

  if (pvalue == pname) { /* значения не было */

    snode_set_nil((struct snode*)pret, 0);

  } else {

    snode_transfer((struct snode*)pret, 0, pvalue, UPWARD);
  }

  return;
}


/*
  (let ((!o s) ... ) s ... )
  ^    ^    ^        ^
  |    *>   *> nil   |
  |    ^             |
  |    *> nil        |
  |    ^   .---------|
  *>   *>  |         *> nil
  ^        |
  |        v
  clear  ret
*/
void kw_let(struct error_stack **pcnterr,
            struct snode *phead, struct scell *pret) {

  struct snode *ppoint = phead;
  void *psepar;


  if (snode_move((ppoint = phead, &ppoint), FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  psepar = varscope_enter(pcnterr);

  /* переменные */

  {
    struct snode *pname, *pvalue, *pargs;


    if (snode_move((pargs = ppoint, &pargs), UPWARD)) {

      ERROR(*pcnterr, WRONG_ARGUMENT);
    }

    for ( ; ; ) {

      if (snode_move((pname = pargs, &pname), UPWARD)) {

        ERROR(*pcnterr, WRONG_ARGUMENT);
      }

      if (snode_get_type(pname, UPWARD) != TYPE_OBJECT) {

        ERROR(*pcnterr, WRONG_ARGUMENT);
      }

      if (!snode_move((pvalue = pname, &pvalue), FORWARD)) {

        evalsexp(pcnterr, snode_get_scell(pvalue, UPWARD));
      }

      snode_check_completion(pcnterr, pvalue, FORWARD);

      defvar(pcnterr, snode_get_scell(pname, UPWARD),
             snode_get_scell(pvalue, (pvalue == pname)?FORWARD:UPWARD));

      if (snode_move(&pargs, FORWARD)) {

        snode_check_completion(pcnterr, pargs, FORWARD);
        break;
      }
    }
  }


  {
    struct snode *pprog;

    /* исполнение */

    for (pprog = ppoint; ; ) {

      if (snode_move(&pprog, FORWARD)) {

        snode_check_completion(pcnterr, pprog, FORWARD);
        break;
      }

      evalsexp(pcnterr, snode_get_scell(pprog, UPWARD));
    }

    /* освобождение локальных переменных */

    varscope_exit(pcnterr, psepar);

    /* возврат */

    if (pprog == ppoint) {

      snode_set_nil((struct snode*)pret, 0);

    } else {

      snode_transfer((struct snode*)pret, 0, pprog, UPWARD);
    }
  }

  return;
}


/*
  (load s)
  ^     ^--.
  *>    *>nil
  ^        |
  |        v
  clear   ret
*/

#include "./load.h"

void kw_load(struct error_stack **pcnterr,
             struct snode *phead, struct scell *pret) {

  struct snode *pfile;


  if (snode_move((pfile = phead, &pfile), FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  snode_check_completion(pcnterr, pfile, FORWARD);

  evalsexp(pcnterr, snode_get_scell(pfile, UPWARD));

  if (snode_get_type(pfile, UPWARD) != TYPE_OBJECT) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }


  load_file(pcnterr, snode_get_scell(pfile, UPWARD));

  /* возврат - имя файла. */

  snode_transfer((struct snode*)pret, 0, pfile, UPWARD);
  return;
}

#ifdef OPTION_MATH

/*
  (open path mode)
  ^
  *>      fd/nil
  ^         |
  |         v
  clear    ret
*/
void kw_open(struct error_stack **pcnterr,
             struct snode *phead, struct scell *pret) {

  struct snode *path, *pmode;


  if (snode_move((path = phead, &path), FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  evalsexp(pcnterr, snode_get_scell(path, UPWARD));

  if (snode_get_type(path, UPWARD) != TYPE_OBJECT) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  if (snode_move((pmode = path, &pmode), FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  evalsexp(pcnterr, snode_get_scell(pmode, UPWARD));

  if (snode_get_type(pmode, UPWARD) != TYPE_OBJECT) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  snode_check_completion(pcnterr, pmode, FORWARD);


  open_file(pcnterr,
            snode_get_scell(path, UPWARD),
            snode_get_scell(pmode, UPWARD),
            pret);

  return;
}

/*
  (read fd size)
  ^
  *>     data/nil
  ^         |
  |         v
  clear    ret
*/
void kw_read(struct error_stack **pcnterr,
             struct snode *phead, struct scell *pret) {

  struct snode *pfd, *psize;


  if (snode_move((pfd = phead, &pfd), FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  evalsexp(pcnterr, snode_get_scell(pfd, UPWARD));

  if (snode_get_type(pfd, UPWARD) != TYPE_OBJECT ||
      snode_get_mark(pfd, UPWARD) != MARK_NUMBER) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  if (snode_move((psize = pfd, &psize), FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  evalsexp(pcnterr, snode_get_scell(psize, UPWARD));

  if (snode_get_type(psize, UPWARD) != TYPE_OBJECT ||
      snode_get_mark(psize, UPWARD) != MARK_NUMBER) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  snode_check_completion(pcnterr, psize, FORWARD);


  read_file(pcnterr,
            snode_get_scell(pfd, UPWARD),
            snode_get_scell(psize, UPWARD),
            pret);

  return;
}

/*
  (write fd data)
  ^
  *>    written bytes/bits
  ^         |
  |         v
  clear    ret
*/
void kw_write(struct error_stack **pcnterr,
              struct snode *phead, struct scell *pret) {

  struct snode *pfd, *pdata;


  if (snode_move((pfd = phead, &pfd), FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  evalsexp(pcnterr, snode_get_scell(pfd, UPWARD));

  if (snode_get_type(pfd, UPWARD) != TYPE_OBJECT ||
      snode_get_mark(pfd, UPWARD) != MARK_NUMBER) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  if (snode_move((pdata = pfd, &pdata), FORWARD)) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  evalsexp(pcnterr, snode_get_scell(pdata, UPWARD));

  if (snode_get_type(pdata, UPWARD) != TYPE_OBJECT) {

    ERROR(*pcnterr, WRONG_ARGUMENT);
  }

  snode_check_completion(pcnterr, pdata, FORWARD);


  write_file(pcnterr,
             snode_get_scell(pfd, UPWARD),
             snode_get_scell(pdata, UPWARD),
             pret);

  return;
}

#endif /* OPTION_MATH */

/* ------------------------------------------------------------------------- */
