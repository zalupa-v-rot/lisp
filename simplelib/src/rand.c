/*---------------------------------------------------------------------------*/

#include "./rand.h"

void rand_init(struct rand *ph, uint vseed) {

  ph->vseed = vseed;
  return;
}

uint rand_next(struct rand *ph) {

  ph->vseed = ph->vseed * 1103515245UL + 12345;

  return ph->vseed >> 16 & 0x7FFF;
}

/*---------------------------------------------------------------------------*/
