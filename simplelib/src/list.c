/*---------------------------------------------------------------------------*/

#include "./list.h"
#include "./assert.h"


void dlist_init(struct dlist_head *ph) {

  assert(ph != NULL);


  ph->pfir = NULL;
  ph->plas = NULL;

  return;
}

void dlist_addf(struct dlist_head *ph, struct dlist_node *pn) {

  assert(ph != NULL);
  assert(pn != NULL);


  pn->ptof = NULL;

  if (ph->pfir != NULL) {

    ph->pfir->ptof = pn;
    pn->ptol = ph->pfir;

  } else {

    ph->plas = pn;
    pn->ptol = NULL;
  }

  ph->pfir = pn;

  return;
}

void dlist_addl(struct dlist_head *ph, struct dlist_node *pn) {

  assert(ph != NULL);
  assert(pn != NULL);


  pn->ptol = NULL;

  if (ph->plas != NULL) {

    ph->plas->ptol = pn;
    pn->ptof = ph->plas;

  } else {

    ph->pfir = pn;
    pn->ptof = NULL;
  }

  ph->plas = pn;

  return;
}

void dlist_del(struct dlist_head *ph, struct dlist_node *pn) {

  assert(ph != NULL);
  assert(pn != NULL);


  if (pn->ptof == NULL) {

    ph->pfir = pn->ptol;

  } else {

    pn->ptof->ptol = pn->ptol;
  }

  if (pn->ptol == NULL) {

    ph->plas = pn->ptof;

  } else {

    pn->ptol->ptof = pn->ptof;
  }

  return;
}

void dlist_addafter(struct dlist_head *ph, struct dlist_node *paft,
                    struct dlist_node *pn) {

  assert(ph != NULL);
  assert(paft != NULL);
  assert(pn != NULL);


  pn->ptof = paft;
  pn->ptol = paft->ptol;

  if (paft->ptol == NULL) {

    ph->plas = pn;

  } else {

    pn->ptol->ptof = pn;
  }

  paft->ptol = pn;

  return;
}

void dlist_addbefore(struct dlist_head *ph, struct dlist_node *pbef,
                     struct dlist_node *pn) {

  assert(ph != NULL);
  assert(pbef != NULL);
  assert(pn != NULL);


  pn->ptof = pbef->ptof;
  pn->ptol = pbef;

  if (pbef->ptof != NULL) {

    ph->pfir = pn;

  } else {

    pn->ptof->ptol = pn;
  }

  pbef->ptof = pn;

  return;
}


struct dlist_node*
        dlist_search(struct dlist_head *ph, void *p, dlist_cmp_t *cmp) {

  struct dlist_node *pc;


  for (pc = ph->pfir; ; pc = pc->ptol) {

    if (pc == NULL || cmp(p, pc)) {

      return pc;
    }
  }
}

/*---------------------------------------------------------------------------*/
