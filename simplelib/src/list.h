/*---------------------------------------------------------------------------*/

#ifndef LIST_H
#define LIST_H

#include "./types.h"


struct dlist_node {

  struct dlist_node
          *ptof,
          *ptol;
};

struct dlist_head {

  struct dlist_node
          *pfir,
          *plas;
};

#define DLIST_INIT { NULL, NULL }

void dlist_init(struct dlist_head *ph);

#define dlist_first(PH) ((PH)->pfir)
#define dlist_last(PH) ((PH)->plas)

#define dlist_findf(PH) ((PH)->pfir)
#define dlist_findl(PH) ((PH)->plas)
#define dlist_tof(PN)   ((PN)->ptof)
#define dlist_tol(PN)   ((PN)->ptol)

void dlist_addf(struct dlist_head *ph, struct dlist_node *pn);
void dlist_addl(struct dlist_head *ph, struct dlist_node *pn);

void dlist_del(struct dlist_head *ph, struct dlist_node *pn);

void dlist_addafter(struct dlist_head *ph, struct dlist_node *paft,
                    struct dlist_node *pn);
void dlist_addbefore(struct dlist_head *ph, struct dlist_node *pbef,
                     struct dlist_node *pn);

typedef uchar dlist_cmp_t(void*, struct dlist_node*);

struct dlist_node*
        dlist_search(struct dlist_head *ph, void *p, dlist_cmp_t *cmp);

#endif

/*---------------------------------------------------------------------------*/
