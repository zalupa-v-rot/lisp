/* ------------------------------------------------------------------------- */

#include <assert.h>

#include "./guts.h"

#define init()                                  \
  struct head sh;                               \
  init_(&sh)

#define memory_chusage(SEXP, STR, STRTOTAL, FUNC, VAR)  \
  memory_chusage_(&sh, SEXP, STR, STRTOTAL, FUNC, VAR)

#define memory_clusage() \
  memory_clusage_(&sh)

#define test(IN, ANSWER, RES) \
  assert(enterline(&sh, (IN), (ANSWER), "", 1, (RES)) == TRUE)

#define test_m(IN, ANSWERS, FNAMES, TREES, RES) \
  assert(enterline(&sh, (IN), (ANSWERS), (FNAMES), (TREES), (RES)) == TRUE)

#define success()      \
  free_(&sh);          \
  printf("success\n"); \
  return 0

#define S(T) T"\0"

/* ------------------------------------------------------------------------- */

int main(void) {

  init();

#ifndef OPTION_NUMBERS
  printf("warning: no OPTION_NUMBERS\n");
#endif
#ifndef OPTION_MATH
  printf("warning: no OPTION_MATH\n");
#endif

  test("(quote a)", "a", TRUE);
  test("(quote a . t)", "(quote a . t)", FALSE);
  test("(quote a b)", "(quote a b)", FALSE);
  test("(quote (some . thing))", "(some . thing)", TRUE);

  test("(list)", "nil", TRUE);
  test("(list . t)", "(list . t)", FALSE);
  test("(list ())", "(nil)", TRUE);
  test("(list . ())", "nil", TRUE);
  test("(list . nil)", "(list . nil)", FALSE); /* потому что не успевает
                                                  исполнится */
  test("(list t . nil)", "(list t . nil)", FALSE);
#ifdef OPTION_NUMBERS
  test("(list 1 2)", "(1 2)", TRUE);
  test("(list (list 1) 2)", "((1) 2)", TRUE);
  test("(list (list 1) (list 2))", "((1) (2))", TRUE);
#endif
  test("(list . (nil))", "(nil)", TRUE);
  test("(list . ((list)))", "(nil)", TRUE);
  test("(list . ((list . ((list)))))", "((nil))", TRUE);

  test("(list (list) (list (list)) (list (list) (list (list))))",
       "(nil (nil) (nil (nil)))", TRUE);
#ifdef OPTION_NUMBERS
  test("(list 0 (list 0 1) (list 0 1 2 (list 0 1 2 3)) (list 0 1 2 3 4 "
       "(list 0 1 2 3 4 5) (list 0 1 2 3 4 5 6 (list 0 1 2 3 4 5 6 7))))",
       "(0 (0 1) (0 1 2 (0 1 2 3)) (0 1 2 3 4 (0 1 2 3 4 5) "
       "(0 1 2 3 4 5 6 (0 1 2 3 4 5 6 7))))", TRUE);
#endif


#ifdef OPTION_NUMBERS
  test("(append)", "nil", TRUE);
  test("(append 1)", "1", TRUE);
  test("(append . 1)", "(append . 1)", FALSE);
  test("(append 1 . 2)", "(append 1 . 2)", FALSE);
  test("(append 1 2)", "(append 1 2)", FALSE);
  test("(append (quote (1 . 2)))", "(1 . 2)", TRUE);
  test("(append (list 1) 2)", "(1 . 2)", TRUE);
  test("(append (list 1) (list 2))", "(1 2)", TRUE);
  test("(append (list 1) (list 2) 3 (list 4) (list 5))",
       "(append (1 2 . 3) nil nil (4) (list 5))", FALSE);
  test("(append (list 1) (list 2) 3 4 (list 5))",
       "(append (1 2 . 3) nil nil 4 (list 5))", FALSE);
#endif

  test("(car nil)", "nil", TRUE);
  test("(car nil . nil)", "(car nil . nil)", FALSE);
  test("(car nil nil)", "(car nil nil)", FALSE);
  test("(car (list (quote high)))", "high", TRUE);
  test("(car (list (quote high) (quote load)))", "high", TRUE);


  test("(cdr nil)", "nil", TRUE);
  test("(cdr nil . nil)", "(cdr nil . nil)", FALSE);
  test("(cdr nil nil)", "(cdr nil nil)", FALSE);
  test("(cdr (list (quote high)))", "nil", TRUE);
  test("(cdr (list (quote high) (quote load)))", "(load)", TRUE);


  test("(cond nil)", "(cond nil)", FALSE); /* потому что он не исполняет в
                                              этой плоскости */
  test("(cond)", "nil", TRUE);
  test("(cond (t (quote a) . t))", "(cond (t a . t))", FALSE);
  test("(cond (nil a . c) ((quote d) (quote e) (quote f)) g)", "f", TRUE);
  test("(cond (nil a . c) (t (quote e) (quote f)) g)", "f", TRUE);
  test("(cond (nil a . c) ((quote d) (quote e)) g)", "e", TRUE);
  test("(cond (nil a . c) ((list) e . f) g)",
       "(cond (nil a . c) (nil e . f) g)", FALSE);
  test("(cond ((car (list nil)) (quote a)) ((list) (quote b)) ((quote c)))",
       "c", TRUE);


  test("(defun some-fun-1 nil some)", "(defun some-fun-1 nil some)",
       FALSE); /* (!!!*2) */
  test("(defun some-fun-2 (a) a . a)", "(defun some-fun-2 (a) a . a)", FALSE);
  test("(defun some-fun-3 (a . b))", "(defun some-fun-3 (a . b))", FALSE);
  test("(defun some-fun-1 () . t)", "(defun some-fun-1 nil . t)", FALSE);
  test("(defun some-fun-1 nil t)", "(defun some-fun-1 nil t)", FALSE);

  memory_chusage(+1, +2, +11, +1, +0);
  test("(defun some-fun-1 () t)", "some-fun-1", TRUE);
  test("(some-fun-1 t)", "(some-fun-1 t)", FALSE);
  test("(some-fun-1 . t)", "(some-fun-1 . t)", FALSE);
  test("(some-fun-1)", "t", TRUE);
  memory_chusage(-1, -1, -1, +0, +0);
  test("(defun some-fun-1 ())", "some-fun-1", TRUE);
  test("(some-fun-1 t)", "(some-fun-1 t)", FALSE);
  test("(some-fun-1 . t)", "(some-fun-1 . t)", FALSE);
  test("(some-fun-1)", "nil", TRUE);
  memory_chusage(+1, +1, +1, +0, +0);
  test("(defun some-fun-1 (a))", "some-fun-1", TRUE);
  test("(some-fun-1)", "(some-fun-1)", FALSE);
  test("(some-fun-1 t t)", "(some-fun-1 t t)", FALSE);
  test("(some-fun-1 t . t)", "(some-fun-1 t . t)", FALSE);
  test("(some-fun-1 t)", "nil", TRUE);
  memory_chusage(+1, +1, +1, +0, +0);
  test("(defun some-fun-1 (a) a)", "some-fun-1", TRUE);
  test("(some-fun-1)", "(some-fun-1)", FALSE);
  test("(some-fun-1 t t)", "(some-fun-1 t t)", FALSE);
  test("(some-fun-1 t . t)", "(some-fun-1 t . t)", FALSE);
  test("(some-fun-1 t)", "t", TRUE);
  memory_chusage(+2, +2, +2, +0, +0);
  test("(defun some-fun-1 (a b) b a)", "some-fun-1", TRUE);
  test("(some-fun-1 nil t)", "nil", TRUE);
  test("(some-fun-1 t nil)", "t", TRUE);
  memory_chusage(-1, -1, -1, +0, +0);
  test("(defun some-fun-1 (a a) a)", "some-fun-1", TRUE); /* (!!!*1) и так
                                                             сойдёт. как фича. */
  test("(some-fun-1 nil t)", "t", TRUE); /* (!!!*1) */
  memory_chusage(+3, +2, +5, +0, +0);
  test("(defun some-fun-1 (a b) (list b a))", "some-fun-1", TRUE);
  test("(some-fun-1 nil t)", "(t nil)", TRUE);
  memory_chusage(-2, -1, -4, +0, +0);
  test("(defun some-fun-1 (a b) b a)", "some-fun-1", TRUE);
  memory_chusage(+4, +5, +14, +1, +0);
  test("(defun some-fun-2 (b a) b a)", "some-fun-2", TRUE);
  test("(some-fun-1 nil t)", "nil", TRUE);
  test("(some-fun-2 t nil)", "nil", TRUE);

#ifdef OPTION_NUMBERS
  memory_chusage(+4, +3, +6, +0, +0);
  test("(defun some-fun-1 (1 2 3) (list 1 2 3))", "some-fun-1",
       TRUE); /* (!!!*0) мда. */
  /* (!!!*0) ну а что, объекты определяются числами и не ищутся в переменных */
  test("(some-fun-1 (quote one) (quote two) (quote three))", "(1 2 3)", TRUE);
  memory_chusage(-4, -3, -6, +0, +0);
  test("(defun some-fun-1 (a b) b a)", "some-fun-1", TRUE);
#endif

  test("(eval nil)", "nil", TRUE);
  test("(eval . nil)", "(eval . nil)", FALSE);
  test("(eval nil . nil)", "(eval nil . nil)", FALSE);
  test("(eval (list (quote list) (quote one) (quote two)))",
       "(eval (list one two))", FALSE);
  test("(eval (list (quote list) (list (quote quote) (quote one)) "
       "(list (quote quote) (quote two))))",
       "(one two)", TRUE);


  test("(set a a)", "(set a a)", FALSE);
  test("(set (quote a) a)", "(set a a)", FALSE);
  test("(set (quote a) (quote a) . t)", "(set a a . t)", FALSE);
  test("(set (list t) (quote a))", "(set (t) (quote a))", FALSE);
  memory_chusage(+0, +2, +2, +0, +1);
  test("(set (quote a) (quote b))", "b", TRUE);
  test("(list a)", "(b)", TRUE); /* глобальный */
  memory_chusage(+0, +2, +2, +0, +1);
  test("(list (set (quote a) (quote b)) (set a a) a b "
       "(set (quote a) (quote c)) (set (quote b) b) a b)",
       "(b b b b c b c b)", TRUE); /* определение и переопределение */
  memory_chusage(+4, +1, +1, +0, +0);
  /* переопределение во вложениях */
  test("(list (set (quote a) (list (set (quote a) (cdr (list "
       "(set (quote a) (quote c)) a))) a)) a)",
       "(((c) (c)) ((c) (c)))", TRUE);
  memory_chusage(-4, -1, -1, +0, +0);
  /* пометка глобальных */
  test("(list (set (quote a) t) (set (quote b) t))", "(t t)", TRUE);


  test("(let (nil))", "(let (nil))", FALSE);
  test("(let ((a)))", "nil", TRUE);
  test("(let ((a)) a)", "nil", TRUE);
  test("(let ((a t)))", "nil", TRUE);
  test("(let ((a (list t))) a)", "(t)", TRUE);
  test("(list a (let ((a (list t)) (a (list nil))) a) a)",
       "(t (nil) t)", TRUE); /* перекрытие */
  test("(let ((a nil) (b t)) b a)", "nil", TRUE);
  test("(let ((a nil) (b (let ((a t)) a))) (list b a))", "(t nil)", TRUE);
  test("(let ((c t)))", "nil", TRUE);
  test("(list c)", "(list c)", FALSE); /* не глобальный */
  test("(let ((c t)) (set (quote c) nil))", "nil", TRUE);
  test("(let ((c t)) (set (quote x) nil))", "nil", TRUE);

  /* выполнение аргументов */
  test("(list (set (quote a) (quote b)) (list a . a))",
       "(list b (list b . a))", FALSE);
  test("(list (set (quote a) (quote b)) (car a))", "(list b (car b))", FALSE);
  test("(list (set (quote a) (quote b)) (cdr a))", "(list b (cdr b))", FALSE);


  /* test("(defun some-fun-1 (a b) b a)", "some-fun-1", TRUE); */
  memory_chusage(-5, -4, -4, +0, +0);
  memory_chusage(+22, +12, +35, +0, +0);
  /* рекурсия */
  test("(defun some-fun-1 (a) (cond ((cdr a) (list (car a) "
       "(some-fun-1 (cdr a)))) ((car a))))", "some-fun-1", TRUE);
  test("(some-fun-1 (list t nil (quote a) (quote b)))", "(t (nil (a b)))",
       TRUE);

  /* мульти-деревьевые выражения.  */

  test_m("(list (quote a)) (list (quote b)) (list (quote c))",
         "(a)\0(b)\0(c)", "\0\0", 3, TRUE);
  test_m("(list (quote a))\n (list (quote b))\n\n (list (quote c)) \n",
         "(a)\0(b)\0(c)", "\0\0", 3, TRUE);
  memory_chusage(+0, +0, +2, +0, +0);
  test_m("(set (quote a) (quote abs)) (list a)", "abs\0(abs)", "\0", 2, TRUE);
  test_m("(let ((c (quote abc))) c) (list c)", "abc\0(list c)", "\0",
         2, FALSE);

  /* (set (quote a) (list (quote z)))
     (load (quote multi01.lisp)) -> (set (quote a) (append a (list (quote c))))
     (append a (list (quote x))) */
  memory_chusage(+2, +1, -1, +0, +0);
  test_m("(load (quote multi00.lisp))",
         S("(z)")S("(z c)")S("multi01.lisp")S("(z c x)")
         S("multi00.lisp"),
         S("multi00.lisp")S("multi01.lisp")
         S("multi00.lisp")S("multi00.lisp")S(""), 5, TRUE);

  memory_clusage();
  memory_chusage(+2, +8, +26, +2, +2);
  test_m("(set (quote a) t) (set (quote b) t) (defun some-fun-1 (a)) "
         "(defun some-fun-2 (b))",
         S("t")S("t")S("some-fun-1")S("some-fun-2"), S("")S("")S("")S(""),
         4, TRUE);

#ifdef OPTION_MATH

  test("(+ 1)", "1", TRUE);
  test("(- 1)", "-1", TRUE);
  test("(+ -1)", "-1", TRUE);
  test("(- -1)", "1", TRUE);
  test("(+ 1 1)", "2", TRUE);
  test("(- 1 1)", "0", TRUE);
  test("(+ -1 1)", "0", TRUE);
  test("(- -1 1)", "-2", TRUE);
  test("(+ 1 -1)", "0", TRUE);
  test("(- 1 -1)", "2", TRUE);
  test("(+ -1 -1)", "-2", TRUE);
  test("(- -1 -1)", "0", TRUE);

  test("(+ (+ 1) (- 2) (+ 3) (- 4) (+ 5) (- 6) (+ 7) (- 8) (+ 9) (- 10))",
       "-5", TRUE);
  test("(- (+ 1) (- 2) (+ 3) (- 4) (+ 5) (- 6) (+ 7) (- 8) (+ 9) (- 10))",
       "7", TRUE);

  test("(* 0)", "0", TRUE);
  test("(* 123456789)", "123456789", TRUE);
  test("(* 1 2 3 4 5 6)", "720", TRUE);
  test("(* 1 1)", "1", TRUE);
  test("(* -1 1)", "-1", TRUE);
  test("(* 1 -1)", "-1", TRUE);
  test("(* -1 -1)", "1", TRUE);

  test("(/ 2)", "0", TRUE);
  test("(/ 1)", "1", TRUE);
  test("(/ 0)", "(/ 0)", FALSE);
  test("(/ 123456789 2 3 4 5)", "1028806", TRUE);
  test("(/ 1 1)", "1", TRUE);
  test("(/ -1 1)", "-1", TRUE);
  test("(/ 1 -1)", "-1", TRUE);
  test("(/ -1 -1)", "1", TRUE);

  test("(% 2)", "1", TRUE);
  test("(% 1)", "0", TRUE);
  test("(% 0)", "(% 0)", FALSE);
  test("(% 123456789 5 4 3 2)", "0", TRUE);
  test("(% 1 1)", "0", TRUE);
  test("(% -1 1)", "0", TRUE);
  test("(% 1 -1)", "0", TRUE);
  test("(% -1 -1)", "0", TRUE);

  test("(/ 123456789 123)", "1003713", TRUE);
  test("(/ -123456789 123)", "-1003713", TRUE);
  test("(/ 123456789 -123)", "-1003713", TRUE);
  test("(/ -123456789 -123)", "1003713", TRUE);

  test("(% 123456789 123)", "90", TRUE);
  test("(% -123456789 123)", "-90", TRUE);
  test("(% 123456789 -123)", "90", TRUE);
  test("(% -123456789 -123)", "-90", TRUE);

  /* дроби  */

  test("(+ 1 0.0010)", "1.0010", TRUE);
  test("(+ -00000)", "0", TRUE);
  test("(+ 001)", "1", TRUE);
  test("(+ -001)", "-1", TRUE);
  test("(+ .1)", "0.1", TRUE);
  test("(+ 1.)", "1", TRUE);
  test("(+ -1.)", "-1", TRUE);
  test("(+ 0.1)", "0.1", TRUE);
  test("(+ -.1)", "-0.1", TRUE);
  test("(+ -0.1)", "-0.1", TRUE);
  test("(+ 1.0)", "1.0", TRUE);
  test("(+ 1234.56789)", "1234.56789", TRUE);
  test("(+ -1234.56789)", "-1234.56789", TRUE);
  test("(+ 0000.0000123456789)", "0.0000123456789", TRUE);
  test("(+ -0000.0000123456789)", "-0.0000123456789", TRUE);
  test("(+ 0000.0)", "0.0", TRUE);
  test("(+ -0000.0)", "0.0", TRUE);

  test("(- 1 0.0010)", "0.9990", TRUE);
  test("(- -00000)", "0", TRUE);
  test("(- 001)", "-1", TRUE);
  test("(- -001)", "1", TRUE);
  test("(- .1)", "-0.1", TRUE);
  test("(- 1.)", "-1", TRUE);
  test("(- -1.)", "1", TRUE);
  test("(- 0.1)", "-0.1", TRUE);
  test("(- -.1)", "0.1", TRUE);
  test("(- -0.1)", "0.1", TRUE);
  test("(- 1.0)", "-1.0", TRUE);
  test("(- 1234.56789)", "-1234.56789", TRUE);
  test("(- -1234.56789)", "1234.56789", TRUE);
  test("(- 0000.0000123456789)", "-0.0000123456789", TRUE);
  test("(- -0000.0000123456789)", "0.0000123456789", TRUE);
  test("(- 0000.0)", "0.0", TRUE);
  test("(- -0000.0)", "0.0", TRUE);

  test("(* 0.1 1)", "0.1", TRUE);
  test("(* 1 0.1)", "0.1", TRUE);
  test("(* 0.1 0.1)", "0.0", TRUE);
  test("(* 0.1 0.10)", "0.01", TRUE);
  test("(* 0.10 0.1)", "0.01", TRUE);
  test("(* -0.1 1)", "-0.1", TRUE);
  test("(* -1 0.1)", "-0.1", TRUE);
  test("(* -0.1 0.1)", "0.0", TRUE);
  test("(* -0.1 0.10)", "-0.01", TRUE);
  test("(* -0.10 0.1)", "-0.01", TRUE);
  test("(* 0.1 -1)", "-0.1", TRUE);
  test("(* 1 -0.1)", "-0.1", TRUE);
  test("(* 0.1 -0.1)", "0.0", TRUE);
  test("(* 0.1 -0.10)", "-0.01", TRUE);
  test("(* 0.10 -0.1)", "-0.01", TRUE);
  test("(* -0.1 -1)", "0.1", TRUE);
  test("(* -1 -0.1)", "0.1", TRUE);
  test("(* -0.1 -0.1)", "0.0", TRUE);
  test("(* -0.1 -0.10)", "0.01", TRUE);
  test("(* -0.10 -0.1)", "0.01", TRUE);

  test("(/ 0.1 1)", "0.1", TRUE);
  test("(/ 1 0.1)", "10.0", TRUE);
  test("(/ 0.1 0.1)", "1.0", TRUE);
  test("(/ 0.1 0.10)", "1.00", TRUE);
  test("(/ 0.10 0.1)", "1.00", TRUE);
  test("(/ 0.1 2)", "0.0", TRUE);
  test("(/ 0.1 2.0)", "0.0", TRUE);
  test("(/ -0.1 1)", "-0.1", TRUE);
  test("(/ -1 0.1)", "-10.0", TRUE);
  test("(/ -0.1 0.1)", "-1.0", TRUE);
  test("(/ -0.1 0.10)", "-1.00", TRUE);
  test("(/ -0.10 0.1)", "-1.00", TRUE);
  test("(/ -0.1 2)", "0.0", TRUE);
  test("(/ -0.1 2.0)", "0.0", TRUE);
  test("(/ 0.1 -1)", "-0.1", TRUE);
  test("(/ 1 -0.1)", "-10.0", TRUE);
  test("(/ 0.1 -0.1)", "-1.0", TRUE);
  test("(/ 0.1 -0.10)", "-1.00", TRUE);
  test("(/ 0.10 -0.1)", "-1.00", TRUE);
  test("(/ 0.1 -2)", "0.0", TRUE);
  test("(/ 0.1 -2.0)", "0.0", TRUE);
  test("(/ -0.1 -1)", "0.1", TRUE);
  test("(/ -1 -0.1)", "10.0", TRUE);
  test("(/ -0.1 -0.1)", "1.0", TRUE);
  test("(/ -0.1 -0.10)", "1.00", TRUE);
  test("(/ -0.10 -0.1)", "1.00", TRUE);
  test("(/ -0.1 -2)", "0.0", TRUE);
  test("(/ -0.1 -2.0)", "0.0", TRUE);

  test("(% 0.1 1)", "0.1", TRUE);
  test("(% 1 0.1)", "0.0", TRUE);
  test("(% 0.1 0.1)", "0.0", TRUE);
  test("(% 0.1 0.10)", "0.00", TRUE);
  test("(% 0.10 0.1)", "0.00", TRUE);
  test("(% 0.1 2)", "0.1", TRUE);
  test("(% 0.1 2.0)", "0.1", TRUE);
  test("(% -0.1 1)", "-0.1", TRUE);
  test("(% -1 0.1)", "0.0", TRUE);
  test("(% -0.1 0.1)", "0.0", TRUE);
  test("(% -0.1 0.10)", "0.00", TRUE);
  test("(% -0.10 0.1)", "0.00", TRUE);
  test("(% -0.1 2)", "-0.1", TRUE);
  test("(% -0.1 2.0)", "-0.1", TRUE);
  test("(% 0.1 -1)", "0.1", TRUE);
  test("(% 1 -0.1)", "0.0", TRUE);
  test("(% 0.1 -0.1)", "0.0", TRUE);
  test("(% 0.1 -0.10)", "0.00", TRUE);
  test("(% 0.10 -0.1)", "0.00", TRUE);
  test("(% 0.1 -2)", "0.1", TRUE);
  test("(% 0.1 -2.0)", "0.1", TRUE);
  test("(% -0.1 -1)", "-0.1", TRUE);
  test("(% -1 -0.1)", "0.0", TRUE);
  test("(% -0.1 -0.1)", "0.0", TRUE);
  test("(% -0.1 -0.10)", "0.00", TRUE);
  test("(% -0.10 -0.1)", "0.00", TRUE);
  test("(% -0.1 -2)", "-0.1", TRUE);
  test("(% -0.1 -2.0)", "-0.1", TRUE);


  test("(/ 1234.00000)", "0.00081", TRUE);
  test("(% 1234.00000)", "1.00000", TRUE);
  test("(/ 0.1234)", "8.1037", TRUE);
  test("(% 0.1234)", "0.0128", TRUE);
  test("(/ -1234.00000)", "-0.00081", TRUE);
  test("(% -1234.00000)", "1.00000", TRUE);
  test("(/ -0.1234)", "-8.1037", TRUE);
  test("(% -0.1234)", "0.0128", TRUE);


  test("(* 0.123456789 0.987654321)", "0.121932631", TRUE);
  test("(/ 0.123456789 0.987654321)", "0.124999998", TRUE);
  test("(% 0.123456789 0.987654321)", "0.123456789", TRUE);

  test("(* 12.3456789 987.654321)", "12193.2631112", TRUE);
  test("(/ 12.3456789 987.654321)", "0.0124999", TRUE);
  test("(% 12.3456789 987.654321)", "12.3456789", TRUE);

  test("(* 123.456789 98.7654321)", "12193.2631112", TRUE);
  test("(/ 123.456789 98.7654321)", "1.2499999", TRUE);
  test("(% 123.456789 98.7654321)", "24.6913569", TRUE);

  test("(+ 1 . 0)", "(+ 1 . 0)", FALSE);
  test("(- 1 . 0)", "(- 1 . 0)", FALSE);
  test("(* 1 . 0)", "(* 1 . 0)", FALSE);
  test("(/ 1 . 0)", "(/ 1 . 0)", FALSE);
  test("(% 1 . 0)", "(% 1 . 0)", FALSE);



  test("(=)", "(=)", FALSE);
  test("(= 0)", "t", TRUE);
  test("(= 0 0)", "t", TRUE);
  test("(= 0 1)", "nil", TRUE);
  test("(= 0 1 (something))", "nil", TRUE);

  test("(<)", "(<)", FALSE);
  test("(< 0)", "t", TRUE);
  test("(< 0 1)", "t", TRUE);
  test("(< 0 0)", "nil", TRUE);
  test("(< 0 0 (something))", "nil", TRUE);

  test("(>)", "(>)", FALSE);
  test("(> 1)", "t", TRUE);
  test("(> 1 0)", "t", TRUE);
  test("(> 1 1)", "nil", TRUE);
  test("(> 1 1 (something))", "nil", TRUE);

  test("(<=)", "(<=)", FALSE);
  test("(<= 0)", "t", TRUE);
  test("(<= 0 0)", "t", TRUE);
  test("(<= 0 0 1)", "t", TRUE);
  test("(<= 0 0 1 0)", "nil", TRUE);
  test("(<= 0 0 1 0 (something))", "nil", TRUE);

  test("(>=)", "(>=)", FALSE);
  test("(>= 1)", "t", TRUE);
  test("(>= 1 0)", "t", TRUE);
  test("(>= 1 0 0)", "t", TRUE);
  test("(>= 1 0 0 1)", "nil", TRUE);
  test("(>= 1 0 0 1 (something))", "nil", TRUE);

  test("(= 1 . 0)", "(= 1 . 0)", FALSE);
  test("(< 1 . 0)", "(< 1 . 0)", FALSE);
  test("(> 1 . 0)", "(> 1 . 0)", FALSE);
  test("(<= 1 . 0)", "(<= 1 . 0)", FALSE);
  test("(>= 1 . 0)", "(>= 1 . 0)", FALSE);

  memory_chusage(+46+25, +46, +227, +2, +0);
  test_m("(load (quote list-sort.lisp))",
         S("plist-add")S("list-sort-i")S("list-sort.lisp"),
         S("list-sort.lisp")S("list-sort.lisp")S(""),
         3, TRUE);

  test("(list-sort-i nil (list 0 1 2 3 4))", "(0 1 2 3 4)", TRUE);
  test("(list-sort-i nil (list 4 3 2 1 0))", "(0 1 2 3 4)", TRUE);
  test("(list-sort-i nil (list 2 3 2 1 0))", "(0 1 2 2 3)", TRUE);
  test("(list-sort-i nil (list 44 11 21 1 34))", "(1 11 21 34 44)", TRUE);

  test_m("(load (quote eval-5-or-6.lisp))",
         S("5")S("eval-5-or-6.lisp"),
         S("eval-5-or-6.lisp")S(""), 2, TRUE);
#endif

  success();
}

/* ------------------------------------------------------------------------- */
