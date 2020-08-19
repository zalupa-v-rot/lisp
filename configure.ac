AC_INIT([lisp], [0])
AM_INIT_AUTOMAKE([-Wall foreign nostdinc subdir-objects])

AC_PROG_CC
AM_PROG_AR
AC_PROG_RANLIB
#AC_CHECK_LIB([])

AC_ARG_WITH([math],
  [AS_HELP_STRING([--without-math],
    [description])], [
  option_math=false
], [option_math=true])

AC_ARG_WITH([numbers],
  [AS_HELP_STRING([--without-numbers],
    [description])], [
  option_numbers=false
], [option_numbers=true])

AC_ARG_WITH([fio],
  [AS_HELP_STRING([--without-fio],
    [description])], [
  option_fio=false
], [option_fio=true])

AC_ARG_WITH([repl],
  [AS_HELP_STRING([--with-repl],
    [description])], [
  with_repl=true
], [with_repl=false])


AS_IF([test $option_numbers = true], [
  CFLAGS="$CFLAGS -DOPTION_NUMBERS"],
      [test $option_numbers = false], [
  option_math=false
])

AS_IF([test $option_math = true], [
  CFLAGS="$CFLAGS -DOPTION_MATH"
])

AS_IF([test $option_fio = true], [
  CFLAGS="$CFLAGS -DOPTION_FIO"
])

#  -DOPTION_EXTERN_ERRORS !!!! (переделать на устанавлевоемую функцию в рантайме)
CFLAGS="$CFLAGS -DOPTION_HOOK_INTER_RET"


AM_CONDITIONAL([OPTION_NUMBERS], [test $option_numbers = true])
AM_CONDITIONAL([OPTION_MATH], [test $option_math = true])
AM_CONDITIONAL([OPTION_FIO], [test $option_fio = true])
AM_CONDITIONAL([WITH_REPL], [test $with_repl = true])


AC_CONFIG_FILES([Makefile src/Makefile test/Makefile])
AC_CONFIG_SUBDIRS([simplelib])
AC_OUTPUT
