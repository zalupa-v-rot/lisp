#!/bin/sh

export CC=''
export CFLAGS=''

abort() {

  set +x
  echo '*** an error occurred ***' >&2
  exit 1
}

trap 'abort' 0
set -e # exit on error
# set -x # echo executed commands

get_cc() {

  if which gcc > /dev/null 2>&1; then

    CC='gcc'
  fi

  return 0
}

get_cflags_test() {


  if which gcc > /dev/null 2>&1; then

    local __GNUC__; local __GNUC_MINOR__; local __GNUC_PATCHLEVEL__; local GCC_VERSION;


    CFLAGS='-g -Wall -Wextra -pedantic -Wno-parentheses -Wfatal-errors -Werror -std=c89'

    __GNUC__=$(gcc -dumpversion | cut -f1 -d.)
    __GNUC_MINOR__=$(gcc -dumpversion | cut -f2 -d.)
    __GNUC_PATCHLEVEL__=$(gcc -dumpversion | cut -f3 -d.)
    GCC_VERSION=$(expr $__GNUC__ \* 10000 + $__GNUC_MINOR__ \* 100 + $__GNUC_PATCHLEVEL__)


    if [ $(expr $GCC_VERSION \>= 40800) -eq '1' ]; then
      # https://gcc.gnu.org/gcc-4.8/changes.html
      #
      # AddressSanitizer , a fast memory error detector, has been added and can
      # be enabled via -fsanitize=address. Memory access instructions will be
      # instrumented to detect heap-, stack-, and global-buffer overflow as well
      # as use-after-free bugs. To get nicer stacktraces, use
      # -fno-omit-frame-pointer. The AddressSanitizer is available on
      # IA-32/x86-64/x32/PowerPC/PowerPC64 GNU/Linux and on x86-64 Darwin.
      CFLAGS="$CFLAGS -fsanitize=address"
      # LDLIBS += -lasan
    fi

    if [ $(expr $GCC_VERSION \>= 40900) -eq '1' ]; then
      # https://gcc.gnu.org/gcc-4.9/changes.html
      #
      # Value range propagation now assumes that the this pointer in C++ member
      # functions is non-null. This eliminates common null pointer checks but
      # also breaks some non-conforming code-bases (such as Qt-5, Chromium,
      # KDevelop). As a temporary work-around -fno-delete-null-pointer-checks
      # can be used. Wrong code can be identified by using -fsanitize=undefined.
      CFLAGS="$CFLAGS -fsanitize=undefined"
    fi

    if [ $(expr $GCC_VERSION \>= 50000) -eq '1' ]; then
      # https://gcc.gnu.org/gcc-5/changes.html
      #
      # -fsanitize=bounds: enable instrumentation of array bounds and detect out-of-bounds accesses;
      # -fsanitize=alignment: enable alignment checking, detect various misaligned objects;
      # -fsanitize=object-size: enable object size checking, detect various out-of-bounds accesses.
      CFLAGS="$CFLAGS -fsanitize=bounds -fsanitize=alignment -fsanitize=object-size"
    fi

    if [ $(expr $GCC_VERSION \>= 60000) -eq '1' ]; then
      # https://gcc.gnu.org/gcc-6/changes.html
      #
      # UndefinedBehaviorSanitizer gained a new sanitization option,
      # -fsanitize=bounds-strict, which enables strict checking of array
      # bounds. In particular, it enables -fsanitize=bounds as well as
      # instrumentation of flexible array member-like arrays.
      CFLAGS="$CFLAGS -fsanitize=bounds-strict"
      # -Wduplicated-cond warns about duplicated conditions in an if-else-if chain.
      # -Wshift-negative-value warns about left shifting a negative value
      CFLAGS="$CFLAGS -Wduplicated-cond -Wshift-negative-value"
    fi

    if [ $(expr $GCC_VERSION \>= 70000) -eq '1' ]; then
      # https://gcc.gnu.org/gcc-7/changes.html
      #
      # The -fsanitize=signed-integer-overflow suboption of the
      # UndefinedBehavior Sanitizer now diagnoses arithmetic overflows even on
      # arithmetic operations with generic vectors.
      CFLAGS="$CFLAGS -fsanitize=signed-integer-overflow"
    fi

    if [ $(expr $GCC_VERSION \>= 80000) -eq '1' ]; then
      # https://gcc.gnu.org/gcc-8/changes.html
      #
      # AddressSanitizer gained a new pair of sanitization options,
      # -fsanitize=pointer-compare and -fsanitize=pointer-subtract, which warn
      # about subtraction (or comparison) of pointers that point to a different
      # memory object:
      CFLAGS="$CFLAGS -fsanitize=pointer-compare -fsanitize=pointer-subtract"
    fi
  fi

  return 0
}

get_cc
get_cflags_test
BUILDDIR="$PWD" REALDIR="$(dirname "$0")" NAME="$(basename "$1")" make -f "$(dirname "$0")/tester.makefile"

# set +x
trap : 0
exit $?
