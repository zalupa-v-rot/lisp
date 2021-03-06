/* ------------------------------------------------------------------------- */

#include "./common.h"


#define test_init()                              \
  struct test_head stesth;                       \
  test_init_(&stesth)

#define test_sum(A, B, C, D) assert(test_sum_(&stesth, A, B, C, D) == TRUE)
#define test_mul(A, B, C)    assert(test_mul_(&stesth, A, B, C) == TRUE)
#define test_div(A, B, C, D) assert(test_div_(&stesth, A, B, C, D) == TRUE)
#define test_cmp(A, B, C)    assert(test_cmp_(&stesth, A, B, C) == TRUE)
#define test_not(A, B)       assert(test_not_(&stesth, A, B) == TRUE)
#define test_sqrt(A, B, C)   assert(test_sqrt_(&stesth, A, B, C) == TRUE)


#define success() \
  printf("success\n"); \
  return TRUE


int main(void) {

  test_init();


#ifndef OPTION_MATH

  printf("warning: no OPTION_MATH\n");

#else

  /* ## add/sub ----------------------------------------------- */

  /* сложение/вычитание с нулём */

  test_sum("987654321", "0", "987654321", sm_add);
  test_sum("0", "123456789", "123456789", sm_add);

  test_sum("-987654321", "0", "-987654321", sm_add);
  test_sum("0", "-123456789", "-123456789", sm_add);

  test_sum("987654321", "0", "987654321", sm_sub);
  test_sum("0", "123456789", "-123456789", sm_sub);

  test_sum("-987654321", "0", "-987654321", sm_sub);
  test_sum("0", "-123456789", "123456789", sm_sub);


  test_sum("1", "0", "1", sm_add);
  test_sum("0", "1", "1", sm_add);

  test_sum("1", "0", "1", sm_sub);
  test_sum("0", "-1", "1", sm_sub);

  test_sum("-1", "0", "-1", sm_add);
  test_sum("0", "-1", "-1", sm_add);

  test_sum("-1", "0", "-1", sm_sub);
  test_sum("0", "1", "-1", sm_sub);

  /* вычитание в ноль */

  test_sum("123456789", "-123456789", "0", sm_add);
  test_sum("-123456789", "123456789", "0", sm_add);

  test_sum("123456789", "123456789", "0", sm_sub);
  test_sum("-123456789", "-123456789", "0", sm_sub);

  /* сложение без переноса */

  test_sum("1987654321", "987654321", "2975308642", sm_add);
  test_sum("987654321", "1987654321", "2975308642", sm_add);

  test_sum("-1987654321", "-987654321", "-2975308642", sm_add);
  test_sum("-987654321", "-1987654321", "-2975308642", sm_add);

  test_sum("1987654321", "-987654321", "2975308642", sm_sub);
  test_sum("987654321", "-1987654321", "2975308642", sm_sub);

  test_sum("-1987654321", "987654321", "-2975308642", sm_sub);
  test_sum("-987654321", "1987654321", "-2975308642", sm_sub);

  /* вычитание без ужатия */

  test_sum("987654321", "-123456789", "864197532", sm_add);
  test_sum("-123456789", "987654321", "864197532", sm_add);

  test_sum("-987654321", "123456789", "-864197532", sm_add);
  test_sum("123456789", "-987654321", "-864197532", sm_add);

  test_sum("987654321", "123456789", "864197532", sm_sub);
  test_sum("-123456789", "-987654321", "864197532", sm_sub);

  test_sum("-987654321", "-123456789", "-864197532", sm_sub);
  test_sum("123456789", "987654321", "-864197532", sm_sub);

  /* прямой перенос с переполнением */

  test_sum("9999", "1", "10000", sm_add);
  test_sum("1", "99999", "100000", sm_add);

  test_sum("-9999", "-1", "-10000", sm_add);
  test_sum("-1", "-99999", "-100000", sm_add);

  test_sum("9999", "-1", "10000", sm_sub);
  test_sum("1", "-99999", "100000", sm_sub);

  test_sum("-9999", "1", "-10000", sm_sub);
  test_sum("-1", "99999", "-100000", sm_sub);


  /* сложный перенос с переполнением */

#define A  "987654321987654321987654321987654321987654321987654321987654321987654321987654321987654321"
#define B   "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
#define C "1000000000888888889877777778776666667666555556555544445444443334333333223222222212111111110"

  test_sum(A, B, C, sm_add);
  test_sum(B, A, C, sm_add);

  test_sum("-" A, "-" B, "-" C, sm_add);
  test_sum("-" B, "-" A, "-" C, sm_add);

  test_sum(A, "-" B, C, sm_sub);
  test_sum(B, "-" A, C, sm_sub);

  test_sum("-" A, B, "-" C, sm_sub);
  test_sum("-" B, A, "-" C, sm_sub);

#undef A
#undef B
#undef C

  /* прямое заимствование с ужатием */

  test_sum("10000", "-1", "9999", sm_add);
  test_sum("-1", "10000", "9999", sm_add);

  test_sum("-10000", "1", "-9999", sm_add);
  test_sum("1", "-10000", "-9999", sm_add);

  test_sum("10000", "1", "9999", sm_sub);
  test_sum("-1", "-10000", "9999", sm_sub);

  test_sum("-10000", "-1", "-9999", sm_sub);
  test_sum("1", "10000", "-9999", sm_sub);


  /* сложное заимствование с ужатием */

#define A "9012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789"
#define B  "987654321987654321987654321987654321987654321987654321987654321987654321987654321987654321"
#define C "8024691356913580245902469134801358023691246912580235801469134690358024579246913568135802468"

  test_sum(A, "-" B, C, sm_add);
  test_sum("-" B, A, C, sm_add);

  test_sum("-" A, B, "-" C, sm_add);
  test_sum(B, "-" A, "-" C, sm_add);

  test_sum(A, B, C, sm_sub);
  test_sum("-" B, "-" A, C, sm_sub);

  test_sum("-" A, "-" B, "-" C, sm_sub);
  test_sum(B, A, "-" C, sm_sub);

#undef A
#undef B
#undef C

  /* ## mul --------------------------------------------------- */

  /* умножение с нулём */

  test_mul("9876543210", "0", "0");
  test_mul("0", "9876543210", "0");
  test_mul("-9876543210", "0", "0");
  test_mul("0", "-9876543210", "0");

  /* умножение с единицей */

  test_mul("9876543210", "1", "9876543210");
  test_mul("1", "9876543210", "9876543210");

  test_mul("-9876543210", "-1", "9876543210");
  test_mul("-1", "-9876543210", "9876543210");

  test_mul("-9876543210", "1", "-9876543210");
  test_mul("1", "-9876543210", "-9876543210");

  test_mul("9876543210", "-1", "-9876543210");
  test_mul("-1", "9876543210", "-9876543210");

  /* умножение - максимальное расширение */

  test_mul("9999999999", "9999999999", "99999999980000000001");
  test_mul("-9999999999", "-9999999999", "99999999980000000001");
  test_mul("9999999999", "-9999999999", "-99999999980000000001");
  test_mul("-9999999999", "9999999999", "-99999999980000000001");

  /* умножение - минимальное расширение */
  test_mul("9999999999", "99", "989999999901");
  test_mul("-9999999999", "-99", "989999999901");
  test_mul("-9999999999", "99", "-989999999901");
  test_mul("9999999999", "-99", "-989999999901");

  test_mul("99", "9999999999", "989999999901");
  test_mul("-99", "-9999999999", "989999999901");
  test_mul("-99", "9999999999", "-989999999901");
  test_mul("99", "-9999999999", "-989999999901");


  /* (2^61-1) * (2^31-1)  */

#define A "2305843009213693951"
#define B "2147483647"
#define C "4951760154835678088235319297"

  test_mul(A, B, C);
  test_mul("-" A, B, "-" C);
  test_mul(A, "-" B, "-" C);
  test_mul("-" A, "-" B, C);

  test_mul(B, A, C);
  test_mul("-" B, A, "-" C);
  test_mul(B, "-" A, "-" C);
  test_mul("-" B, "-" A, C);

#undef A
#undef B
#undef C

  /* (2^107-1) * (2^89-1)  */

#define A "162259276829213363391578010288127"
#define B "618970019642690137449562111"
#define C "100433627766186892221372630609062766858404681029709092356097"

  test_mul(A, B, C);
  test_mul("-" A, B, "-" C);
  test_mul(A, "-" B, "-" C);
  test_mul("-" A, "-" B, C);

  test_mul(B, A, C);
  test_mul("-" B, A, "-" C);
  test_mul(B, "-" A, "-" C);
  test_mul("-" B, "-" A, C);

#undef A
#undef B
#undef C

  /*
    каждая цифра множит куаждую
    a*10^(10^2) * b*10^(10^2)
   */
#define A "9999999999888888888877777777776666666666555555555544444444443333333333222222222211111111110000000000"
#define B "9876543210987654321098765432109876543210987654321098765432109876543210987654321098765432109876543210"
#define C "9876543210877914952076817558306584362140548696845043895747603292181070219478738010973936899999999999" \
          "0123456789122085047923182441693415637859451303154956104252396707818929780521261989026063100000000000"

  test_mul(A, B, C);
  test_mul(B, A, C);

  test_mul("-" A, B, "-" C);
  test_mul(B, "-" A, "-" C);

  test_mul(A, "-" B, "-" C);
  test_mul("-" B, A, "-" C);

  test_mul("-" A, "-" B, C);
  test_mul("-" B, "-" A, C);

#undef A
#undef B
#undef C

  /* ## div --------------------------------------------------- */

  /* деление от малого */

  test_div("1", "1", "1", "0");
  test_div("-1", "1", "-1", "0");

  test_div("1", "-1", "-1", "0");
  test_div("-1", "-1", "1", "0");

  test_div("0", "1", "0", "0");
  test_div("0", "-1", "0", "0");

  test_div("5", "3", "1", "2");
  test_div("-5", "3", "-1", "-2");
  test_div("5", "-3", "-1", "2");
  test_div("-5", "-3", "1", "-2");

  test_div("9876543210", "1", "9876543210", "0");
  test_div("-9876543210", "1", "-9876543210", "0");
  test_div("9876543210", "-1", "-9876543210", "0");
  test_div("-9876543210", "-1", "9876543210", "0");

  test_div("1234567890", "9876543210", "0", "1234567890");
  test_div("-1234567890", "9876543210", "0", "-1234567890");
  test_div("1234567890", "-9876543210", "0", "1234567890");
  test_div("-1234567890", "-9876543210", "0", "-1234567890");


  /* ровное */

#define A "900000000008000000000700000000600000005000000400000300002000100"
#define B "100"
#define C "9000000000080000000007000000006000000050000004000003000020001"

  test_div(A, B, C, "0");
  test_div(A, C, B, "0");
  test_div("-" A, B, "-" C, "0");
  test_div("-" A, C, "-" B, "0");
  test_div(A, "-" B, "-" C, "0");
  test_div(A, "-" C, "-" B, "0");
  test_div("-" A, "-" B, C, "0");
  test_div("-" A, "-" C, B, "0");

#undef A
#undef B
#undef C

  /* (2^61-1) * (2^31-1)  */

#define A "2305843009213693951"
#define B "2147483647"
#define C "4951760154835678088235319297"

  test_div(C, A, B, "0" );
  test_div(C, B, A, "0" );

  test_div("-" C, A, "-" B, "0" );
  test_div("-" C, B, "-" A, "0" );

  test_div(C, "-" A, "-" B, "0" );
  test_div(C, "-" B, "-" A, "0" );

  test_div("-" C, "-" A, B, "0" );
  test_div("-" C, "-" B, A, "0" );

#undef A
#undef B
#undef C

  /* (2^107-1) * (2^89-1)  */

#define A "162259276829213363391578010288127"
#define B "618970019642690137449562111"
#define C "100433627766186892221372630609062766858404681029709092356097"

  test_div(C, A, B, "0" );
  test_div(C, B, A, "0" );

  test_div("-" C, A, "-" B, "0" );
  test_div("-" C, B, "-" A, "0" );

  test_div(C, "-" A, "-" B, "0" );
  test_div(C, "-" B, "-" A, "0" );

  test_div("-" C, "-" A, B, "0" );
  test_div("-" C, "-" B, A, "0" );

#undef A
#undef B
#undef C

  /* с остатком */

  /* (2^521-1) / (2^107-1)  */

#define A "6864797660130609714981900799081393217269435300143305409394463459185543183397656" \
          "052122559640661454554977296311391480858037121987999716643812574028291115057151"
#define B "162259276829213363391578010288127"

#define C "4230758200257591033292257971409760728962287052393304039526563929957352327133730" \
          "5648194152986460468097094724553524878434631680"
#define D "9903520314283042199192993791"


  test_div(A, B, C, D);
  test_div(A, C, B, D);

  test_div("-" A, B, "-" C, "-" D);
  test_div("-" A, C, "-" B, "-" D);

  test_div(A, "-" B, "-" C, D);
  test_div(A, "-" C, "-" B, D);

  test_div("-" A, "-" B, C, "-" D);
  test_div("-" A, "-" C, B, "-" D);

  /*  */

  test_cmp("0", "0", 0);
  test_cmp("0", "1", -1);
  test_cmp("0", "2", -1);
  test_cmp("0", "-1", 1);
  test_cmp("0", "-2", 1);

  test_cmp("1", "0", 1);
  test_cmp("1", "1", 0);
  test_cmp("1", "2", -1);
  test_cmp("1", "-1", 1);
  test_cmp("1", "-2", 1);

  test_cmp("2", "0", 1);
  test_cmp("2", "1", 1);
  test_cmp("2", "2", 0);
  test_cmp("2", "-1", 1);
  test_cmp("2", "-2", 1);

  test_cmp("-1", "0", -1);
  test_cmp("-1", "1", -1);
  test_cmp("-1", "2", -1);
  test_cmp("-1", "-1", 0);
  test_cmp("-1", "-2", 1);

  test_cmp("-2", "0", -1);
  test_cmp("-2", "1", -1);
  test_cmp("-2", "2", -1);
  test_cmp("-2", "-1", -1);
  test_cmp("-2", "-2", 0);

  test_cmp("123456789", "123457689", -1);
  test_cmp("123457689", "123456789", 1);

  test_cmp("-123456789", "123457689", -1);
  test_cmp("-123457689", "123456789", -1);

  test_cmp("123456789", "-123457689", 1);
  test_cmp("123457689", "-123456789", 1);

  test_cmp("-123456789", "-123457689", 1);
  test_cmp("-123457689", "-123456789", -1);

  /*  */

  test_not("0", "0");
  test_not("1", "-1");
  test_not("-1", "1");
  test_not("1234567890", "-1234567890");
  test_not("-1234567890", "1234567890");

  /* */
  
  test_sqrt("0", "0", "0");
  test_sqrt("1", "1", "0");
  test_sqrt("2", "1", "1");

  test_sqrt("0", "0", "0");
  test_sqrt("-1", "1", "0");
  test_sqrt("-2", "1", "1");

  test_sqrt("16", "4", "0");
  test_sqrt("-16", "4", "0");

  test_sqrt("1234567890", "35136", "29394");
  test_sqrt("123456789", "11111", "2468");

  test_sqrt("-1234567890", "35136", "29394");
  test_sqrt("-123456789", "11111", "2468");

  test_sqrt("9876543210", "99380", "158810");
  test_sqrt("-9876543210", "99380", "158810");

  test_sqrt("987654321", "31426", "60845");
  test_sqrt("-987654321", "31426", "60845");

  /* (2^89-1)^2 */
  test_sqrt("383123885216472214589586755549637256619304505646776321", "618970019642690137449562111", "0");
  test_sqrt("-383123885216472214589586755549637256619304505646776321", "618970019642690137449562111", "0");

  /* (2^107-1)^2 */
  test_sqrt("26328072917139296674479506920917283561170115423410494657557168129",
           "162259276829213363391578010288127", "0");
  test_sqrt("-26328072917139296674479506920917283561170115423410494657557168129",
           "162259276829213363391578010288127", "0");


  /* (2^89-0,123456789)^2  */
  test_sqrt("383123885216472214589586756634745193679977877162917368", "618970019642690137449562111",
           "1085107937060673371516141047");

  test_sqrt("-383123885216472214589586756634745193679977877162917368", "618970019642690137449562111",
           "1085107937060673371516141047");

  /* (2^107-0,123456789)^2 */
  test_sqrt("26328072917139296674479506920917568015705168256570797384836158021",
           "162259276829213363391578010288127", "284454535052833160302727278989892");

  test_sqrt("-26328072917139296674479506920917568015705168256570797384836158021",
           "162259276829213363391578010288127", "284454535052833160302727278989892");

#endif

  success();
}

/* ------------------------------------------------------------------------- */
