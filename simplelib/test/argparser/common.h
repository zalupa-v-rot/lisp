#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./lib/assert.h"
#include "./lib/types.h"
#include "./lib/argparser.h"

int simple_argsmaker(const char *pcline, int *pargc, char ***pargv);
void free_args(int vargc, char **pargv);


#define VALUES_MAX 5
#define STRINGS_MAX 5

struct conf {

  uintp   svalues[VALUES_MAX];
  char    *pstrings[STRINGS_MAX];
};

#define UNDEF_OPTION "-undef-"
#define WITH_DUPLICATES 1

struct test {

  const char *pcline;

  int voptions;
  struct option *popt;
  int vf;

  struct conf
          sconf,
          sconfres;

};

void test(struct test *pt, int vtest);
