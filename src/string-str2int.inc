/* ------------------------------------------------------------------------- *
 * шаблон функций для конвертации из строк в числа                           *
 *                                                                           *
 * <T>  - type                                                               *
 * <SL> - type limit signed max                                              *
 * <UL> - type limit unsiged max                                             *
 * ------------------------------------------------------------------------- */

#define COMBINE(A, X)   COMBINE_(A, X)
#define COMBINE_(A, X)  COMBINE__(A, X)
#define COMBINE__(A, X) A ## X


int COMBINE(string_to_, T)(const struct string *pobj, T *pnum) {

  uintp vp, vs;
  T vnum, va;


  vs = (pobj->pstr[0] == '-')?1:0;

  for (vnum = 0, vp = pobj->vlen; vp-vs; vnum = va) {

    if ((va = vnum * 10 + pobj->pstr[--vp] - '0') < vnum) {

      return 1;
    }
  }

  if (vs) {

    /* вместимость числа в знаковый тип */
    if (vnum > SL) {

      return 1;
    }

    /* конвертация по стандарту c99 */
    vnum = UL - vnum + 1;
  }

  *pnum = vnum;

  return 0;
}

#undef T
#undef SL
#undef UL

#undef COMBINE
#undef COMBINE_
#undef COMBINE__
