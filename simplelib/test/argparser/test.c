/*------------------------------------------------------------------------------*/

#include "./common.h"

#define tassert(x) ((x) == 0 ? tassert_(__FILE__, __LINE__, vtest, vfunc) : (void)0)

void tassert_(const char *pfile, int vline, int vtest, int vfunc) {

  fprintf(stderr, "error file: %s, line: %i, test: %i (%s)\n", pfile, vline, vtest, ((vfunc)?"argparser_nd":"argparser"));
  exit(1);
}

void test(struct test *pt, int vtest) {

  struct conf sinit;
  int argc, vfunc; char **argv;


  memcpy(&sinit, &pt->sconf, sizeof(struct conf));


  for (vfunc = 0; vfunc != 2; vfunc++) {

    tassert(simple_argsmaker(pt->pcline, &argc, &argv) == 0);

/*  */
    {
      int argi = 1;
      struct argerror serr;

      if (vfunc) {

        do {

          if (argparser_nd(pt->voptions, pt->popt, &pt->sconf, &argi, argc, argv, &serr)) {

            if ((pt->vf & WITH_DUPLICATES) && serr.vcod == ARGPARSER_DUPLICATED_OPTION) {

              continue;
            }

            if (serr.vcod == ARGPARSER_UNRECOGNIZED_OPTION &&
                strcmp(argv[argi], UNDEF_OPTION) == 0) {

              argi++;
              continue;
            }

            argparser_printerr(&serr);
            tassert(0);
          }

          break;

        } while(1);

      } else {

        do {

          if (argparser(pt->voptions, pt->popt, &pt->sconf, &argi, argc, argv, &serr)) {

            if (serr.vcod == ARGPARSER_UNRECOGNIZED_OPTION &&
                strcmp(argv[argi], UNDEF_OPTION) == 0) {

              argi++;
              continue;
            }

            argparser_printerr(&serr);
            tassert(0);
          }

          break;

        } while(1);

      }

      tassert(argi == argc);
    }

    {
      uint va;

      for (va = 0; va != VALUES_MAX;
           tassert(pt->sconf.svalues[va] == pt->sconfres.svalues[va]), va++);


      for (va = 0; va != STRINGS_MAX; va++) {

        if (pt->sconfres.pstrings[va] != NULL) {

          tassert(strcmp(pt->sconf.pstrings[va], pt->sconfres.pstrings[va]) == 0);
        }
      }
    }


    free_args(argc, argv);

    memcpy(&pt->sconf, &sinit, sizeof(struct conf));
  }

  return;
}

/*------------------------------------------------------------------------------*/
