/*---------------------------------------------------------------------------*/

#include "./types.h"

struct rand {

  ulong  vseed;
};

void rand_init(struct rand *ph, uint vseed);
uint rand_next(struct rand *ph);

/*---------------------------------------------------------------------------*/
