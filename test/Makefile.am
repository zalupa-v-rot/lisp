bin_PROGRAMS = strmath0 lisp0
TESTS = $(bin_PROGRAMS)
LDADD = ../src/liblisp.a

BUILT_SOURCES = multi00.lisp multi01.lisp eval-5-or-6.lisp list-sort.lisp

strmath0_SOURCES = strmath/main.c strmath/guts.c strmath/memory.c

lisp0_SOURCES = lisp/main.c lisp/guts.c
#lisp0_CFLAGS = -DSRCDIR=\"$(srcdir)/lisp/\"

multi00.lisp:
	cp $(srcdir)/lisp/multi00.lisp $(builddir)/

multi01.lisp:
	cp $(srcdir)/lisp/multi01.lisp $(builddir)/

eval-5-or-6.lisp:
	cp $(srcdir)/lisp/eval-5-or-6.lisp $(builddir)/

list-sort.lisp:
	cp $(srcdir)/lisp/list-sort.lisp $(builddir)/

