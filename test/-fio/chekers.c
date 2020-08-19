/* ------------------------------------------------------------------------- */

#include "./chekers.h"
#include "./assert.h"

#include <stdio.h>


int check_file_exist(const char *pfname) {

  FILE *pfd;

  if ((pfd = fopen(pfname, "r")) != NULL) {

    fclose(pfd);
    return 1;
  }

  return 0;
}

int check_file_notexist(const char *pfname) {

  return !check_file_exist(pfname);
}

int check_file_size(const char *pfname, long vsize) {

  long vfsize;
  FILE *pfd;


  if ((pfd = fopen(pfname, "r")) == NULL) {

    assert(0);
  }
  if (fseek(pfd, 0, SEEK_END)) {

    assert(0);
  }
  if ((vfsize = ftell(pfd)) == -1) {

    assert(0);
  }
  if (fclose(pfd)) {

    assert(0);
  }

  return (vsize == vfsize);
}

/* ------------------------------------------------------------------------- */
