LISP_SRC=lisp.c load.c parse.c inter_io.c snode_wrapper.c keywords.c keywords-math.c inter_core.c clear.c memory_std.c strmath.c strmath2.c string.c error.c

DIR?=/tmp/lisp-tests
MODULES?=lisp-common strmath

MODULESDIR=$(addprefix $(DIR)/, $(MODULES))
NAME=


export LDFLAGS
export LDLIBS
export FO
export OUT=a.out
export CC?=gcc
export CFLAGS?=-g -O0 -fprofile-arcs -ftest-coverage
# 
###
export LDFLAGS=-L$(DIR)/simplelib/
export LDLIBS=-lsimple
export FO=.o
###

#ifneq (,$(shell which gcc))
ifeq (,$(shell which gcc))
export LDFLAGS=-L$(DIR)/simplelib/
export LDLIBS=-lsimple
export FO=.o
CC=gcc
CFLAGS += -Wall -Wextra -pedantic -Wno-parentheses -Wfatal-errors -Werror -std=c89

__GNUC__:=$(shell gcc -dumpversion | cut -f1 -d.)
__GNUC_MINOR__:=$(shell gcc -dumpversion | cut -f2 -d.)
__GNUC_PATCHLEVEL__:=$(shell gcc -dumpversion | cut -f3 -d.)
GCC_VERSION:=$(shell expr $(__GNUC__) \* 10000 + $(__GNUC_MINOR__) \* 100 + $(__GNUC_PATCHLEVEL__))


ifeq (1,$(shell expr $(GCC_VERSION) \>= 40800))
# https://gcc.gnu.org/gcc-4.8/changes.html
# AddressSanitizer , a fast memory error detector, has been added and can be enabled via -fsanitize=address. Memory access instructions will be instrumented to detect heap-, stack-, and global-buffer overflow as well as use-after-free bugs. To get nicer stacktraces, use -fno-omit-frame-pointer. The AddressSanitizer is available on IA-32/x86-64/x32/PowerPC/PowerPC64 GNU/Linux and on x86-64 Darwin.
CFLAGS += -fsanitize=address
endif

ifeq (1,$(shell expr $(GCC_VERSION) \>= 40900))
# https://gcc.gnu.org/gcc-4.9/changes.html
# Value range propagation now assumes that the this pointer in C++ member functions is non-null. This eliminates common null pointer checks but also breaks some non-conforming code-bases (such as Qt-5, Chromium, KDevelop). As a temporary work-around -fno-delete-null-pointer-checks can be used. Wrong code can be identified by using -fsanitize=undefined.
CFLAGS += -fsanitize=undefined
endif

ifeq (1,$(shell expr $(GCC_VERSION) \>= 50000))
# https://gcc.gnu.org/gcc-5/changes.html
# -fsanitize=bounds: enable instrumentation of array bounds and detect out-of-bounds accesses;
# -fsanitize=alignment: enable alignment checking, detect various misaligned objects;
# -fsanitize=object-size: enable object size checking, detect various out-of-bounds accesses.
CFLAGS += -fsanitize=bounds -fsanitize=alignment -fsanitize=object-size
endif

ifeq (1,$(shell expr $(GCC_VERSION) \>= 60000))
# https://gcc.gnu.org/gcc-6/changes.html
# UndefinedBehaviorSanitizer gained a new sanitization option, -fsanitize=bounds-strict, which enables strict checking of array bounds. In particular, it enables -fsanitize=bounds as well as instrumentation of flexible array member-like arrays.
CFLAGS += -fsanitize=bounds-strict
# -Wduplicated-cond warns about duplicated conditions in an if-else-if chain.
# -Wshift-negative-value warns about left shifting a negative value
CFLAGS += -Wduplicated-cond -Wshift-negative-value
endif

ifeq (1,$(shell expr $(GCC_VERSION) \>= 70000))
# https://gcc.gnu.org/gcc-7/changes.html
# The -fsanitize=signed-integer-overflow suboption of the UndefinedBehavior Sanitizer now diagnoses arithmetic overflows even on arithmetic operations with generic vectors.
CFLAGS += -fsanitize=signed-integer-overflow
endif

ifeq (1,$(shell expr $(GCC_VERSION) \>= 80000))
# https://gcc.gnu.org/gcc-8/changes.html
# AddressSanitizer gained a new pair of sanitization options, -fsanitize=pointer-compare and -fsanitize=pointer-subtract, which warn about subtraction (or comparison) of pointers that point to a different memory object:
CFLAGS += -fsanitize=pointer-compare -fsanitize=pointer-subtract
endif

endif

ifeq ($(DIR),)
$(error)
endif
ifeq ($(DIR),/)
$(error)
endif


# 
build: $(MODULESDIR)

# проброс к репе теста 
$(DIR)/%: ./% _FORCE
	$(MAKE) -C ./$< NAME=$< DIR=$(DIR) hidden_build

# тест билдит себя и стартует. назначив: DIR, LISP_DEFINES, NOT_OVERRIDE_SRC
# DIR - чтобы иметь моножество билдов
# NOT_OVERRIDE_SRC - чтобы не переопределять SRC
build_me_please: $(DIR)/BUILD

$(DIR)/BUILD: $(DIR)/src
	# cd $(NAME); $(DIR)/$(OUT)
	echo '' > $(DIR)/BUILD

$(DIR)/simplelib: ../lib
	rm -rf $(DIR)/simplelib
	mkdir -p $(DIR)/simplelib
	$(MAKE) -C ../lib build DIR='$(DIR)/simplelib'

$(DIR)/src: $(DIR)/simplelib ../ ./$(NAME)
	rm -rf $(DIR)/src
	rm -rf $(DIR)/lisp
	mkdir -p $(DIR)/src
	mkdir -p $(DIR)/lisp
	$(MAKE) -f ./master.makefile CDEFINES='$(LISP_DEFINES)' DIR='$(DIR)' NAME=$(NAME) SRC='$(NOT_OVERRIDE_SRC)' LISP_SRC='$(LISP_SRC)'

_FORCE:
