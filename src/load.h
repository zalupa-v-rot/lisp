/* ------------------------------------------------------------------------- */

#include "./types.h"

/*
  если pfdata - NULL, то загрузить файл по имени pfname.

  pfname может быть NULL в случае если pfdata - не NULLю.

  возврат != 0 - ошибка.
  peret указатели у pcontext и pm восстанавливаются при выходе.
*/


/*

 */
void load_file(struct error_stack **pcnterr, const struct scell *pname);


int load_data(struct inter *pcontext,
              const char *pfdata, uintp vfsize, const char *pfname);

/* ------------------------------------------------------------------------- */
