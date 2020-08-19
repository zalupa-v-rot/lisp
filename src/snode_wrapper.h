/* ------------------------------------------------------------------------- */

#define UPWARD  0
#define FORWARD 1

#include "./types.h"


int snode_move(struct snode **ppoint, uintp vi);
void snode_set_nil(struct snode *ph, uint vdir);

uint snode_get_type(struct snode *ph, uint vdir);
uint snode_get_mark(struct snode *ph, uint vdir);

struct scell* snode_get_scell(struct snode *ph, uint vdir);

void snode_transfer(struct snode *pdst, uint vdst_dir,
                    struct snode *psrc, uint vsrc_dir);

/* ------------------------------------------------------------------------- */
