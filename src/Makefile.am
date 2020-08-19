lib_LIBRARIES = liblisp.a

liblisp_a_SOURCES = clear.c error.c inter_core.c inter_io.c lisp.c load.c memory.c memory_std.c parse.c snode_wrapper.c string.c keywords.c
#liblisp_a_DEPENDENCIES = ../simplelib/src/libsimple.a
#liblisp_a_LIBADD = ../simplelib/src/libsimple.a

if WITH_REPL
bin_PROGRAMS = lisp
lisp_SOURCES = repl.c
lisp_LDADD = liblisp.a
lisp_LDFLAGS = -lreadline
endif

if OPTION_MATH
liblisp_a_SOURCES += keywords-math.c strmath.c
endif


liblisp.a: $(liblisp_a_OBJECTS) ../simplelib/src/libsimple.a
	rm -f $@
	cp ../simplelib/src/libsimple.a $@
	$(AR) r $@ $(liblisp_a_OBJECTS)
	$(RANLIB) $@
