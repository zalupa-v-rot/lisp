/* ------------------------------------------------------------------------- */

#include "./memory.h"

struct memory_std {

  uintp   vsexp,
          vstr,
          vstrtotal,
          vfunc,
          vvar,
          vfd,

          vsexpmax,
          vstrmax,
          vstrtotalmax,
          vfuncmax,
          vvarmax,
          vfdmax;

  struct memory
          sm;
};

struct memory* memory_std_init(struct memory_std *pms,
                               uintp vsexpmax,
                               uintp vstrmax,
                               uintp vstrtotalmax,
                               uintp vfuncmax,
                               uintp vvarmax,
                               uintp vfdmax);

struct mstat_std {

  uintp   vsexp,
          vstr,
          vstrtotal,
          vfunc,
          vvar,
          vfdmax;
};

void mstat_std(struct memory_std *pms,
               struct mstat_std *pstat);

/* ------------------------------------------------------------------------- */
