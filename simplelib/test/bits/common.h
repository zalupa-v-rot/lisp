/*------------------------------------------------------------------------------*/

#include "./lib/bits.h"
#include "./lib/assert.h"

#include <stdlib.h>
#include <stdio.h>

#define TRUE  EXIT_SUCCESS
#define FALSE EXIT_FAILURE


#define TEQ_BYTE 2
#define TEQ_BIT  1
int teq(bitptr_t sa, bitptr_t sb, int vm);

#define ptrbitoff(P, M, O) ptrbitoff_((void*)(P), (M), (O))
bitptr_t ptrbitoff_(void *p, uint vmode, uint voff);

/*------------------------------------------------------------------------------*/
