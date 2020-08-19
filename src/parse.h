/* ------------------------------------------------------------------------- */

#ifndef PARSE_H
#define PARSE_H

#include "./types.h"

/* ------------------------------------------------------------------------- */

struct parse {

  uintp   vcolumn,
          vline;

  int  vscope; /* !!! убрать? depth */

  struct error_stack *perr;
  struct memory *pm;

  const char
          *pts, /* начало */
          *pt,  /* текущая позиция */
          *pte; /* конец */
};


/*
  просто чтобы меньше аргументов передавать дальше.
*/
void parse_init(struct parse *ppar,
                struct memory *pm, struct error_stack *perr);

/*
  парсинг и создание sexp - дерева

  ключевые символы '(' ')' '.'
  всё остальное - атомы, объекты

  proot - по адресу должна быть валидная память. в случае ошибки, структура
  будет проинизиализированна как TYPE_NIL. также, в случае ошибки, память
  выделенная на построение дерева освобождается.

  возврат != 0 - ошибка (расшифровка в pedesc)
  peret указатели у ppar и pm восстанавливаются при выходе.
*/
int parse(struct parse *ppar,
          const char *pstr, uintp vlen, struct scell *proot);


uintp parse_line(struct parse *ppar);
uintp parse_column(struct parse *ppar);
uintp parse_ending(struct parse *ppar);


/*
  pstr - памяь для печати. может быть NULL, тогда возвращается только размер.
  vlen - памяти под столько символов.

  возврат - реально прописалось символов.
*/
uintp print_tree(struct scell *proot, char *pstr, uintp vlen, uintp *perrpos);

#endif /* PARSE_H */

/* ------------------------------------------------------------------------- */
