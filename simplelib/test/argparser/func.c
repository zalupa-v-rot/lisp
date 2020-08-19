/*------------------------------------------------------------------------------*/

#include "./common.h"

int simple_argsmaker(const char *pcline, int *pargc, char ***pargv) {

  uintp argc, vsize, vps, vpe, va;
  void *pmem;
  char **argv;


  for (vsize = argc = vpe = 0; ; ) {

    vps = vpe;

    for ( ; pcline[vps] == ' '; vps++);
    if (pcline[vps] == '\0') { break; }

    vpe = vps;

    for (argc++; ++vpe, pcline[vpe] != ' ' && pcline[vpe] != '\0'; );

    vsize += vpe-vps+1;
  }

  if ((pmem = malloc(sizeof(char*) * argc + sizeof(char) * vsize)) == NULL) {

    return 1;
  }

  argv = pmem;
  pmem = (char*)pmem + sizeof(char*) * argc;

  for (va = vpe = 0; ; va++) {

    vps = vpe;

    for ( ; pcline[vps] == ' '; vps++);
    if (pcline[vps] == '\0') { break; }

    vpe = vps;

    for (; ++vpe, pcline[vpe] != ' ' && pcline[vpe] != '\0'; );

    argv[va] = pmem;

    memcpy(argv[va], &pcline[vps], vpe-vps);
    (argv[va])[vpe-vps] = '\0';

    pmem = (char*)pmem + (vpe-vps)+1;
  }

  assert(argc <= INT_MAX);
  *pargc = argc;
  *pargv = argv;

  return 0;
}

void free_args(int vargc, char **pargv) {

  UNUSED(vargc);

  free(pargv);
  return;
}

/*------------------------------------------------------------------------------*/
