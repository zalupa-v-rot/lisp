/*------------------------------------------------------------------------------*/

#include "./common.h"

#define OFFSETOF_VALUE(I) offsetof(struct conf, svalues)+sizeof(uintp)*(I)
#define OFFSETOF_STRING(I) offsetof(struct conf, pstrings)+sizeof(char*)*(I)

/*------------------------------------------------------------------------------*/

int arg_input_file(void * const *popt, void *pbase, int *pargi, int vargc, char **pargv,
                   struct argerror *perr);

int arg_output_file(void* const *popt, void *pbase, int *pargi, int vargc, char **pargv,
                    struct argerror *perr);

#define ENUM0_N 4
#define ENUM1_N 4

const char *penum0[ENUM0_N] = { "your", "your", "our", "their" };
const char *penum1[ENUM1_N] = { "he", "she", "she", "they" };

#define SOME_FLAG_0 1
#define SOME_FLAG_1 2
#define SOME_FLAG_2 4

#define OPTIONS0    18

struct option gl_sopt0[OPTIONS0] =
  {
    OPTION_FLAG_UINTP("-f0", OFFSETOF_VALUE(0), SOME_FLAG_0, SOME_FLAG_0),
    OPTION_FLAG_UINTP("-f1", OFFSETOF_VALUE(0), SOME_FLAG_1, SOME_FLAG_1),
    OPTION_FLAG_UINTP("-f01", OFFSETOF_VALUE(0), SOME_FLAG_0|SOME_FLAG_1, SOME_FLAG_0|SOME_FLAG_1),

    OPTION_STRING("--input", OFFSETOF_STRING(0)),
    OPTION_STRING("-i", OFFSETOF_STRING(0)),
    OPTION_STRING("--output", OFFSETOF_STRING(1)),
    OPTION_STRING("-o", OFFSETOF_STRING(1)),

    OPTION_CALLBACK("-input", OFFSETOF_STRING(0), arg_input_file),
    OPTION_CALLBACK("-output", OFFSETOF_STRING(1), arg_output_file),

    OPTION_ENUM_UINTP("--enum0", OFFSETOF_VALUE(1), ENUM0_N, penum0),
    OPTION_ENUM_UINTP("-e0", OFFSETOF_VALUE(1), ENUM0_N, penum0),
    OPTION_ENUM_UINTP("--enum1", OFFSETOF_VALUE(2), ENUM1_N, penum1),
    OPTION_ENUM_UINTP("-e1", OFFSETOF_VALUE(2), ENUM1_N, penum1),

    OPTION_INIT_UINTP("--value0", OFFSETOF_VALUE(3)),
    OPTION_INIT_UINTP("--value1", OFFSETOF_VALUE(4)),

    OPTION_INIT_PAIR_UINTP("--value00", OFFSETOF_VALUE(3), OFFSETOF_VALUE(3)),
    OPTION_INIT_PAIR_UINTP("--value11", OFFSETOF_VALUE(4), OFFSETOF_VALUE(4)),
    OPTION_INIT_PAIR_UINTP("--value01", OFFSETOF_VALUE(3), OFFSETOF_VALUE(4)),
  };

int arg_input_file(void * const *popt, void *pbase, int *pargi, int vargc, char **pargv,
                   struct argerror *perr) {

  void *poffset = 0;
  char *ppath;

  UNUSED(pbase);


  if (arg_string(&poffset, &ppath, pargi, vargc, pargv, perr)) {

    return 1;
  }

  *(char**)((char*)pbase+(uintp)(*popt)) = ppath;

  return 0;
}

int arg_output_file(void * const *popt, void *pbase, int *pargi, int vargc, char **pargv,
                    struct argerror *perr) {

  void *poffset = 0;
  char *ppath;

  UNUSED(pbase);


  if (arg_string(&poffset, &ppath, pargi, vargc, pargv, perr)) {

    return 1;
  }

  *(char**)((char*)pbase+(uintp)(*popt)) = ppath;

  return 0;
}

/*------------------------------------------------------------------------------*/

#define TEST_N 6

struct test gl_test[TEST_N] =
  {

   /* простой */
   { "./test -f0 --enum0 our -e1 they --value1 123 --value0 6666 --output /tmp/someelse",
     OPTIONS0, gl_sopt0, 0,
     { {0}, {0} }, { { SOME_FLAG_0, 2, 3, 6666, 123}, { NULL, "/tmp/someelse" } }
   }
   ,
   { "./test --input /tmp/some -e0 your -o /tmp/someelse --enum1 she --value01 6666x123 -f01",
     OPTIONS0, gl_sopt0, 0,
     { {0}, {0} }, { { SOME_FLAG_0|SOME_FLAG_1, 0, 1, 6666, 123}, { "/tmp/some", "/tmp/someelse" } }
   }
   ,
   { "./test -input /what/if -output /we/would",
     OPTIONS0, gl_sopt0, 0,
     { {0}, {0} }, { { 0, 0, 0, 0, 0}, { "/what/if", "/we/would" } }
   }
   ,
   /* наложение */
   { "./test -i /tmp/some -o /tmp/someelse --enum0 our --enum1 he -e0 their -e1 she --value11 6666x123 --value00 123x6666 -f01 -f0 -f1 --input /my/name --output /your/nuclear",
     OPTIONS0, gl_sopt0, 0,
     { {0}, {0} }, { { SOME_FLAG_0|SOME_FLAG_1, 3, 1, 6666, 123}, { "/my/name", "/your/nuclear" } }
   }
   ,
   /* парсинг через неопр. опцию */
   { "./test -f1 -o /first/1 "UNDEF_OPTION" -f0 -o /second/2 "UNDEF_OPTION,
     OPTIONS0, gl_sopt0, 0,
     { {0}, {0} }, { { SOME_FLAG_0|SOME_FLAG_1, 0, 0, 0, 0}, { NULL, "/second/2" } }
   }
   ,
   /* обнаружение дубликата */
   { "./test -o /first/1 -i /nothing/or -o /second/2",
     OPTIONS0, gl_sopt0, WITH_DUPLICATES,
     { {0}, {0} }, { { 0, 0, 0, 0, 0}, { "/nothing/or", "/second/2" } }
   }

  };

/*------------------------------------------------------------------------------*/

int main(void) {

  int va;

  for (va = 0; va != TEST_N; va++) {

    test(&gl_test[va], va);
  }

  return 0;

}

/*------------------------------------------------------------------------------*/
