/* ------------------------------------------------------------------------- */

#define SNODE_WRAPPER_C
#include "./snode_wrapper.h"


/* чтобы юзать struct scell* как  struct snode* с dir 0 */
static_assert(offsetof(struct snode, s) == 0, "");

int snode_move(struct snode **ppoint, uintp vi) {

  if (TYPEGET((*ppoint)->s[vi].vtype) != TYPE_SEXP) {

    return 1;
  }

  *ppoint = (*ppoint)->s[vi].u.psexp;
  return 0;
}

void snode_set_nil(struct snode *ph, uint vdir) {

  TYPESET(ph->s[vdir].vtype, TYPE_NIL);
  return;
}

uint snode_get_type(struct snode *ph, uint vdir) {

  return TYPEGET(ph->s[vdir].vtype);
}

uint snode_get_mark(struct snode *ph, uint vdir) {

  return MARKGET(ph->s[vdir].vtype);
}

struct scell* snode_get_scell(struct snode *ph, uint vdir) {

  return &ph->s[vdir];
}

void snode_transfer(struct snode *pdst, uint vdst_dir,
                    struct snode *psrc, uint vsrc_dir) {

  pdst->s[vdst_dir] = psrc->s[vsrc_dir];
  snode_set_nil(psrc, vsrc_dir);
  return;
}

/* ------------------------------------------------------------------------- */
