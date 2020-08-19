#include "./lisp.h"
#include "./parse.h"
#include "./memory_std.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>


void outres(struct inter *pcontext, const char *pfilename, struct scell *proot, void *ptr) {

  static char sbuf[512];
  uintp vs, vep;

  UNUSED(pcontext), UNUSED(ptr);


  vs = print_tree(proot, sbuf, 512, &vep);

  assert(vs <= UINT_MAX);

  if (vs == vep) {

    printf("out %s:: %.*s\n", pfilename, (uint)vs, sbuf);

  } else {

    printf("out %s::%"PRIuPTR" %.*s\n", pfilename, vep, (uint)vs, sbuf);
  }


  return;
}

int main(int argc, char** argv) {

  struct error_stack sestack, * const pestack = &sestack;
  struct inter scontext, * const pcontext = &scontext;
  struct memory_std sms;

  UNUSED(argc), UNUSED(argv);

  printf("lisp repl\n"
         "c-c to exit\n");

  /*
    4мб - как максимум
    4 * 1024 * 1024 = 4194304
    sexp   0.3
    string 0.6
    func   0.05
    var    0.05

    4194304 * 0.05 / 28(func) = 7489
    4194304 * 0.05 / 32(var) = 6553
    4194304 * 0.3 / 32(sxp) = 39321
    4194304 * 0.6 = 2516582

    2516582 * 0.1 / 8(string) = 31457
    2516582 * 0.9 (total) = 2264923
  */

  if (lisp_init(pcontext, memory_std_init(&sms, 39321, 31457,
                                          2264923, 7489, 6553, 8),
                pestack)) {

    return 1;
  }

  inter_sethook_inter_ret(pcontext, NULL, outres);


  for ( ; ; ) {

    char *pinput = readline("lisp>");

    if (pinput != NULL && *pinput != '\0') {

      add_history(pinput);

      lisp_enter_str(pcontext, pinput);

      free(pinput);

      if (0) {
        struct mstat_std smstat;

        mstat_std(&sms, &smstat);

        printf("memory usage: sexp %"PRIuPTR", str %"PRIuPTR", strtotal %"PRIuPTR", func %"PRIuPTR", var %"PRIuPTR"\n",
               smstat.vsexp,
               smstat.vstr,
               smstat.vstrtotal,
               smstat.vfunc,
               smstat.vvar);
      }
    }
  }

  lisp_free(pcontext);
  return 0;
}
