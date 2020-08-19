#ifdef OPTION_NUMBERS
/* определение чисел при интерпритации. и вообще, соотвецтвенно их
 * присуцтвие. */
#endif

#ifdef OPTION_MATH
/* математичнские функции. это также вводит понятие дробей в парсинг. */
#ifndef OPTION_NUMBERS
#error
#endif

#endif

#ifdef OPTION_FIO

#ifndef OPTION_NUMBERS
#error
#endif

#endif


/* ~~~ опции которые я сомниваюсь что вообще нужны. но и не знаю что с ними делать
   до конца ~~~ */

#ifdef OPTION_HOOK_INTER_RET
/* для перехвата функции inter() на возврате и анализа её возвратного значения -
 * состояние отпарсенного куска кода. */
#endif

#ifdef OPTION_EXTERN_ERRORS
/* внешние коды ошибок */
#endif
