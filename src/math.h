/* ------------------------------------------------------------------------- */

#include "./types.h"

void addition_next(struct inter *pcontext,
                   struct scell *pfirst, struct scell *pnext);

void subtraction_next(struct inter *pcontext,
                      struct scell *pfirst, struct scell *pnext);

void multiplication_next(struct inter *pcontext,
                         struct scell *pfirst, struct scell *pnext);

void division_next(struct inter *pcontext,
                   struct scell *pfirst, struct scell *pnext);

void divremainder_next(struct inter *pcontext,
                       struct scell *pfirst, struct scell *pnext);


/* pfirst == pnext --> 0 */
int equality_next(struct inter *pcontext,
                  struct scell *pfirst, struct scell *pnext);

/* pfirst <  pnext --> 0 */
int increase_next(struct inter *pcontext,
                  struct scell *pfirst, struct scell *pnext);

/* pfirst <= pnext --> 0 */
int increase_oreq_next(struct inter *pcontext,
                       struct scell *pfirst, struct scell *pnext);

/* pfirst >  pnext --> 0 */
int decrease_next(struct inter *pcontext,
                  struct scell *pfirst, struct scell *pnext);

/* pfirst >= pnext --> 0 */
int decrease_oreq_next(struct inter *pcontext,
                       struct scell *pfirst, struct scell *pnext);

/* ------------------------------------------------------------------------- */
