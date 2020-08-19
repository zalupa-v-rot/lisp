/*------------------------------------------------------------------------------*/

#include "./common.h"

int teq(bitptr_t sa, bitptr_t sb, int vm) {

  return ((bitptr_getbyte(sa) == bitptr_getbyte(sb))<<1 |
          (bitptr_getbit(sa) == bitptr_getbit(sb))) == vm;
}

bitptr_t ptrbitoff_(void *p, uint vmode, uint voff) {

  return bitptr_add(bitptr(p, vmode), vmode, voff);
}

/*------------------------------------------------------------------------------*/
