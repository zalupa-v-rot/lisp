/*------------------------------------------------------------------------------*/

struct snode;
struct scell;
struct error_stack;

/* keywords.c */
void kw_list(struct error_stack **pcnterr,
             struct snode *phead, struct scell *pret);
void kw_append(struct error_stack **pcnterr,
               struct snode *phead, struct scell *pret);
void kw_car(struct error_stack **pcnterr,
            struct snode *phead, struct scell *pret);
void kw_cdr(struct error_stack **pcnterr,
            struct snode *phead, struct scell *pret);
void kw_quote(struct error_stack **pcnterr,
              struct snode *phead, struct scell *pret);
void kw_cond(struct error_stack **pcnterr,
             struct snode *phead, struct scell *pret);
void kw_defun(struct error_stack **pcnterr,
              struct snode *phead, struct scell *pret);
void kw_eval(struct error_stack **pcnterr,
             struct snode *phead, struct scell *pret);
void kw_set(struct error_stack **pcnterr,
            struct snode *phead, struct scell *pret);
void kw_let(struct error_stack **pcnterr,
            struct snode *phead, struct scell *pret);

void kw_load(struct error_stack **pcnterr,
             struct snode *phead, struct scell *pret);

#ifdef OPTION_MATH
void kw_open(struct error_stack **pcnterr,
             struct snode *phead, struct scell *pret);
void kw_read(struct error_stack **pcnterr,
             struct snode *phead, struct scell *pret);
void kw_write(struct error_stack **pcnterr,
              struct snode *phead, struct scell *pret);
#endif

/* keywords-math.c */
#ifdef OPTION_MATH
void kw_addition(struct error_stack **pcnterr,
                 struct snode *phead, struct scell *pret);
void kw_subtraction(struct error_stack **pcnterr,
                    struct snode *phead, struct scell *pret);
void kw_multiplication(struct error_stack **pcnterr,
                       struct snode *phead, struct scell *pret);
void kw_division(struct error_stack **pcnterr,
                 struct snode *phead, struct scell *pret);
void kw_divremainder(struct error_stack **pcnterr,
                     struct snode *phead, struct scell *pret);

void kw_equality(struct error_stack **pcnterr,
                 struct snode *phead, struct scell *pret);
void kw_increase(struct error_stack **pcnterr,
                 struct snode *phead, struct scell *pret);
void kw_increase_oreq(struct error_stack **pcnterr,
                      struct snode *phead, struct scell *pret);
void kw_decrease(struct error_stack **pcnterr,
                 struct snode *phead, struct scell *pret);
void kw_decrease_oreq(struct error_stack **pcnterr,
                      struct snode *phead, struct scell *pret);
#endif

/*------------------------------------------------------------------------------*/
