/*------------------------------------------------------------------------------*/

#include "./clear.h"
#include "./memory.h"

void clear_snode(struct memory *pm, struct snode *psnode) {

  struct scell *pscell;


  switch(TYPEGET((pscell = &psnode->s[0])->vtype)) {

  case TYPE_SEXP:
    clear_snode(pm, pscell->u.psexp);
    break;

  case TYPE_OBJECT:
    strfree(pm, &pscell->u.sobject);
    break;
  }

  switch(TYPEGET((pscell = &psnode->s[1])->vtype)) {

  case TYPE_SEXP:
    clear_snode(pm, pscell->u.psexp);
    break;

  case TYPE_OBJECT:
    strfree(pm, &pscell->u.sobject);
    break;
  }

  sexpfree(pm, psnode);
  return;
}

void clear_scell(struct memory *pm, struct scell *pscell) {

  switch(TYPEGET(pscell->vtype)) {

  case TYPE_SEXP:
    clear_snode(pm, pscell->u.psexp);
    break;

  case TYPE_OBJECT:
    strfree(pm, &pscell->u.sobject);
    break;
  }

  return;
}

/*------------------------------------------------------------------------------*/
