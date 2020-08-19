/* ------------------------------------------------------------------------- */

#ifndef ERROR_H
#define ERROR_H

#include <setjmp.h>

#include "./lib/types.h"
#include "./lib/assert.h"

struct error_stack {

  struct error_node
          *ptop;

  uint    vcode;
  const char
          *psrcfile;
  uint    vsrcline;
};

struct error_node {

  jmp_buf sjmp;
  struct error_node *ptodown;

#ifndef NDEBUG
  const char *psrcfile;
#endif
};

#ifndef NDEBUG
void error_init(struct error_stack *pstack);
#endif

#ifndef NDEBUG
void error_set(struct error_stack *pstack,
               struct error_node *pnew, const char *psrcfile);
void error_unset(struct error_stack *pstack,
                 const char *psrcfile);
#else
void error_set(struct error_stack *pstack, struct error_node *pnew);
void error_unset(struct error_stack *pstack);
#endif

void error_silent(struct error_stack *pstack);

void error_trow(struct error_stack *pstack,
                uint vcode, const char *psrcfile, uint vsrcline);

uint error_code(struct error_stack *pstack);
const char* error_str(struct error_stack *pstack);
uint error_srcline(struct error_stack *pstack);
const char* error_srcfile(struct error_stack *pstack);

/* ------------------------------------------------------------------------- */

#ifndef NDEBUG

#define ERROR_INIT(PSTACK)                      \
  error_init(PSTACK)

#define ERROR_CATCH(PSTACK, NEWNODE)                                    \
  (error_set((PSTACK), (NEWNODE), __FILE__), setjmp((PSTACK)->ptop->sjmp))

#define ERROR_LEAVE(PSTACK) error_unset(PSTACK, __FILE__)

#else

#define ERROR_INIT(PSTACK)

#define ERROR_CATCH(PSTACK, NEWNODE)                              \
  (error_set((PSTACK), (NEWNODE)), setjmp((PSTACK)->ptop->sjmp))

#define ERROR_LEAVE(PSTACK) error_unset(PSTACK)

#endif

#define ERROR_SILENT(PSTACK) error_silent(PSTACK)

#define ERROR(PSTACK, CODE) ERROR_(PSTACK, CODE)
#define ERROR_(PSTACK, CODE) error_trow((PSTACK), (CODE), __FILE__, __LINE__)

/* ------------------------------------------------------------------------- */

/* парсинг */
#define NOT_A_FUNCTION              1
#define UNDEFINED_FUNCTION          2
#define ARGUMENT_TYPE_NOT_A_LIST    3
#define UNDEFINED_VARIABLE          4
#define WRONG_NUMBER_OF_ARGUMENTS   6
#define WRONG_ARGUMENT              7
#define INCORRECT_COMPLETION        8

/* интерпретация */
#define UNEXPECTED_END             11
#define UNEXPECTED_CLOSING_BRACKET 12
#define UNEXPECTED_DOT             13
#define CLOSING_BRACKET_EXPECTED   14
#define EMPTY_LIST                 16
#define OPERNING_BRACKET_EXPECTED  17

/* память */
#define OUT_OF_MEMORY              21

/*  */
#define FAILED_TO_OPEN_FILE        31
#define FAILED_TO_READ_FILE        32
#define FAILED_TO_WRITE_FILE       33
#define FAILED_TO_CLOSE_FILE       34
#define INVALID_FILE_DESCRIPTOR    35
#define INCORRECT_FILE_MODE        36

/* выход из файла с ошибкой */
#define TRACE                      41
/* #define NO_APPROPRIATE_CODE        42 */


/* математика */
#ifdef OPTION_MATH
#define DIVISION_BY_ZERO           51
#define UINTP_OVERFLOW             52
#endif


#define STANDARD_ERRORS_SPACE       1
#define EXTERN_ERRORS_SPACE        52

#ifdef OPTION_EXTERN_ERRORS
extern const char* extern_error_str(uint vcode);
#endif

#endif /* ERROR_H */

/* ------------------------------------------------------------------------- */
