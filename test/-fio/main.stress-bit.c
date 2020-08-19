/* ------------------------------------------------------------------------- */

#include "./comm.h"

#define M                                                         \
  sprintf_b("mode:%s, pointmode:%s, bufmode:%s, size:%lu(%lu)\n",     \
            strmode(vmode), strbmode(vpointmode), strbmode(vbufmode), \
            vbufbit, vbufbyte)

static_assert(MAGIC >= FIO_PART_SIZE*2, "");


int main(void) {

  int vpointmode, vbufmode, vii;
  uintp vbufbit, vbufbyte;
  bitptr_t sb1, sb2;
  struct fio sfl, * const pf = &sfl;


  strmagic(sbuf, BUFSIZE1);
  rand_init(&sr, 0);


  for (vii = 0; vii != 16; vii++) {


    vpointmode = ((vii&1)?BM_DO:BM_IO) | ((vii&2)?BM_MSF:BM_LSF);
    vbufmode = ((vii&4)?BM_DO:BM_IO) | ((vii&8)?BM_MSF:BM_LSF);

    if (vbufmode & BM_ORIENT == BM_DO) {

      sb1 = bitptr(&sbuf[BUFSIZE1-1], 0);
      sb2 = bitptr(&sbuf2[BUFSIZE1-1], 0);

    } else {

      sb1 = bitptr(&sbuf[0], 0);
      sb2 = bitptr(&sbuf2[0], 0);
    }

    vbufbit = rand_next(&sr) % (BUFBIT1/2) + (BUFBIT1+1)/2;
    vbufbyte = (vbufbit + (CHAR_BIT-1)) / CHAR_BIT;

    /* стресс - создать файлы. */

    {
      int vi, vmode, sm[3] = { FIOM_WEXTEND, FIOM_REXTEND,
                               FIOM_WEXTEND|FIOM_REXTEND };
      const char *pfname;


      for (vi = 0; vi != CODES_COMB; vi++) {

      printf("-----------|adsdas\n");
        vmode = FIOM_CREATE|FIOM_WRITE|FIOM_READ | sm[vi%3];
        pfname = sprintf_b(T"/exist-bit-%i", vi + vii*CODES_COMB);

        /* if (!(vmode & FIOM_CREATE) || !(vmode & FIOM_WRITE) || !(vmode & FIOM_READ) || */
        /*     (!(vmode & FIOM_WEXTEND) && !(vmode & FIOM_REXTEND))) { */

        /*   continue; */
        /* } */

        assert_info(fio_open(pf, pfname, vmode, vpointmode) == 0, M);
        memset(sbuf2, 0, vbufbyte);

        if (!(vmode & FIOM_WEXTEND)) {

          uintp vc, vp;

          /* требуется расширение чтением */

          for (vp = 0; ; ) {

            vc = rand_next(&sr) % MAGIC + 1;
            if (vp + vc > vbufbit) { vc = vbufbit - vp; }

            assert_info(fio_readb(pf, bitptr_add(sb2, vbufmode, vp),
                                  vbufmode, vc) == vc, M);

            if ((vp += vc) == vbufbit) { break; }
          }

          assert_info(fio_close(pf) == 0, M);
          assert_info(check_file_exist(pfname), M);
          assert_info(check_file_size(pfname, vbufbyte), M);
          assert_info(fio_open(pf, pfname, vmode, vpointmode) == 0, M);
        }

        {
          uintp vc, vp;

          for (vp = 0; ; ) {

            vc = rand_next(&sr) % MAGIC + 1;
            if (vp + vc > vbufbit) { vc = vbufbit - vp; }

            assert_info(fio_writeb(pf, bitptr_add(sb1, vbufmode, vp),
                                   vbufmode, vc) == vc, M);

            if ((vp += vc) == vbufbit) { break; }
          }
        }

        assert_info(fio_close(pf) == 0, M);
        assert_info(check_file_exist(pfname), M);
        assert_info(check_file_size(pfname, vbufbyte), M);

        assert_info(fio_open(pf, pfname, vmode, vpointmode) == 0, M);
        memset(sbuf2, 0, vbufbyte);

        {
          uintp vc, vp;

          for (vp = 0; ; ) {

            vc = rand_next(&sr) % MAGIC + 1;
            if (vp + vc > vbufbit) { vc = vbufbit - vp; }

            assert_info(fio_readb(pf, bitptr_add(sb2, vbufmode, vp),
                                  vbufmode, vc) == vc, M);

            if ((vp += vc) == vbufbit) { break; }
          }
        }

        assert_info(fio_close(pf) == 0, M);
        assert_info(check_file_exist(pfname), M);
        assert_info(check_file_size(pfname, vbufbyte), M);
        assert_info(bitmemcmp(sb1, vbufmode, sb2, vbufmode, vbufbit) == 0, M);
      }
    }


    /* стресс - изменить файлы */


    {
      int vi, vmode;
      const char *pfname;


      for (vi = 0; vi != CODES_COMB; vi++) {


        vmode = mode_code(vi);
        pfname = sprintf_b(T"/exist-bit-%i", vi + vii*CODES_COMB);

        if (!(vmode & FIOM_READ)) {

          continue;
        }

        {
          uintp vc, vp;

          assert_info(fio_open(pf, pfname, vmode, vpointmode) == 0, M);
          memset(sbuf2, 0, vbufbyte);

          for (vp = 0; ; ) {

            vc = rand_next(&sr) % MAGIC + 1;
            if (vp + vc > vbufbit) { vc = vbufbit - vp; }

            assert_info(fio_readb(pf, bitptr_add(sb2, vbufmode, vp),
                                  vbufmode, vc) == vc, M);

            if ((vp += vc) == vbufbit) { break; }
          }

          assert_info(fio_close(pf) == 0, M);
          assert_info(check_file_exist(pfname), M);
          assert_info(check_file_size(pfname, vbufbyte), M);
          assert_info(bitmemcmp(sb1, vbufmode, sb2, vbufmode, vbufbit) == 0, M);
        }

        if (vmode & FIOM_WRITE) {

          uintp vc, vp;

          assert_info(fio_open(pf, pfname, vmode, vpointmode) == 0, M);
          memset(sbuf2, 0, vbufbyte);

          for (vp = 0; ; ) {

            vc = rand_next(&sr) % MAGIC + 1;
            if (vp + vc > vbufbit) { vc = vbufbit - vp; }

            assert_info(fio_writeb(pf, bitptr_add(sb1, vbufmode, vp),
                                   vbufmode, vc) == vc, M);

            if ((vp += vc) == vbufbit) { break; }
          }

          assert_info(fio_close(pf) == 0, M);
          assert_info(check_file_exist(pfname), M);
          assert_info(check_file_size(pfname, vbufbyte), M);
        }

        assert_info(fio_open(pf, pfname, vmode, vpointmode) == 0, M);
        assert_info(fio_readb(pf, sb2, vbufmode, vbufbit) == vbufbit, M);
        assert_info(fio_close(pf) == 0, M);
        assert_info(check_file_exist(pfname), M);
        assert_info(check_file_size(pfname, vbufbyte), M);
        assert_info(bitmemcmp(sb1, vbufmode, sb2, vbufmode, vbufbit) == 0, M);
      }
    }

  }

  success();
}

/* ------------------------------------------------------------------------- */
