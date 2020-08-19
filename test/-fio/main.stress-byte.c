/* ------------------------------------------------------------------------- */

#include "./comm.h"

#define M strmode(vmode)

static_assert(MAGIC >= FIO_PART_SIZE*2, "");


int main(void) {

  struct fio sfl, * const pf = &sfl;


  strmagic(sbuf, BUFSIZE1);


  /* стресс - создать файлы. */

  rand_init(&sr, 0);

  {
    int vi, vmode, sm[3] = { FIOM_WEXTEND, FIOM_REXTEND,
                             FIOM_WEXTEND|FIOM_REXTEND };
    const char *pfname;


    for (vi = 0; vi != CODES_COMB; vi++) {


      vmode = FIOM_CREATE|FIOM_WRITE|FIOM_READ | sm[vi%3];
      pfname = sprintf_b(T"/exist-byte-%i", vi);

      /* if (!(vmode & FIOM_CREATE) || !(vmode & FIOM_WRITE) || !(vmode & FIOM_READ) || */
      /*     (!(vmode & FIOM_WEXTEND) && !(vmode & FIOM_REXTEND))) { */

      /*   continue; */
      /* } */

      assert_info(fio_open(pf, pfname, vmode, BM_DEFAULT) == 0, M);
      memset(sbuf2, 0, BUFSIZE1);

      if (!(vmode & FIOM_WEXTEND)) {

        uintp vc, vp;

        /* требуется расширение чтением */

        for (vp = 0; ; ) {

          vc = rand_next(&sr) % MAGIC + 1;
          if (vp + vc > BUFSIZE1) { vc = BUFSIZE1 - vp; }

          assert_info(fio_read(pf, &sbuf2[vp], vc) == vc, M);

          if ((vp += vc) == BUFSIZE1) { break; }
        }

        /* assert_info(fio_ptell(pf, FIO_PEND) == 0, M); */
        /* assert_info(fio_ptell(pf, FIO_PBEG) == -BUFSIZE1, M); */
        assert_info(fio_close(pf) == 0, M);
        assert_info(check_file_exist(pfname), M);
        assert_info(check_file_size(pfname, BUFSIZE1), M);
        assert_info(fio_open(pf, pfname, vmode, BM_DEFAULT) == 0, M);
        /* assert_info(fio_ptell(pf, FIO_PEND) == BUFSIZE1, M); */
        /* assert_info(fio_ptell(pf, FIO_PBEG) == 0, M); */
      }

      {
        uintp vc, vp;

        for (vp = 0; ; ) {

          vc = rand_next(&sr) % MAGIC + 1;
          if (vp + vc > BUFSIZE1) { vc = BUFSIZE1 - vp; }

          assert_info(fio_write(pf, &sbuf[vp], vc) == vc, M);
          /* printf("%i %i %i %i\n", (int)fio_ptell(pf, FIO_PEND), */
          /*        (int)fio_ptell(pf, FIO_PBEG), BUFSIZE1, (int)(vp+vc)); */

          if ((vp += vc) == BUFSIZE1) { break; }
        }
      }

      /* printf("%i %i %i\n", (int)fio_ptell(pf, FIO_PEND), */
      /*        (int)fio_ptell(pf, FIO_PBEG), BUFSIZE1); */

      /* assert_info(fio_ptell(pf, FIO_PEND) == 0, M); */
      /* assert_info(fio_ptell(pf, FIO_PBEG) == -BUFSIZE1, M); */
      assert_info(fio_close(pf) == 0, M);
      assert_info(check_file_exist(pfname), M);
      assert_info(check_file_size(pfname, BUFSIZE1), M);
      assert_info(fio_open(pf, pfname, vmode, BM_DEFAULT) == 0, M);
      /* assert_info(fio_ptell(pf, FIO_PEND) == BUFSIZE1, M); */
      /* assert_info(fio_ptell(pf, FIO_PBEG) == 0, M); */
      memset(sbuf2, 0, BUFSIZE1);

      {
        uintp vc, vp;

        for (vp = 0; ; ) {

          vc = rand_next(&sr) % MAGIC + 1;
          if (vp + vc > BUFSIZE1) { vc = BUFSIZE1 - vp; }

          assert_info(fio_read(pf, &sbuf2[vp], vc) == vc, M);

          if ((vp += vc) == BUFSIZE1) { break; }
        }
      }

      /* assert_info(fio_ptell(pf, FIO_PEND) == 0, M); */
      /* assert_info(fio_ptell(pf, FIO_PBEG) == -BUFSIZE1, M); */
      assert_info(fio_close(pf) == 0, M);
      assert_info(check_file_exist(pfname), M);
      assert_info(check_file_size(pfname, BUFSIZE1), M);
      assert_info(memcmp(sbuf, sbuf2, BUFSIZE1) == 0, M);
    }
  }

  /* стресс - изменить файлы */

  {
    int vi, vmode;
    const char *pfname;


    for (vi = 0; vi != CODES_COMB; vi++) {


      vmode = mode_code(vi);
      pfname = sprintf_b(T"/exist-byte-%i", vi);

      if (!(vmode & FIOM_READ)) {

        continue;
      }

      {
        uintp vc, vp;

        assert_info(fio_open(pf, pfname, vmode, BM_DEFAULT) == 0, M);
        memset(sbuf2, 0, BUFSIZE0);

        for (vp = 0; ; ) {

          vc = rand_next(&sr) % MAGIC + 1;
          if (vp + vc > BUFSIZE1) { vc = BUFSIZE1 - vp; }

          assert_info(fio_read(pf, &sbuf2[vp], vc) == vc, M);

          if ((vp += vc) == BUFSIZE1) { break; }
        }

        assert_info(fio_close(pf) == 0, M);
        assert_info(check_file_exist(pfname), M);
        assert_info(check_file_size(pfname, BUFSIZE1), M);
        assert_info(memcmp(sbuf, sbuf2, BUFSIZE1) == 0, M);
      }

      if (vmode & FIOM_WRITE) {

        uintp vc, vp;

        assert_info(fio_open(pf, pfname, vmode, BM_DEFAULT) == 0, M);
        memset(sbuf2, 0, BUFSIZE0);

        for (vp = 0; ; ) {

          vc = rand_next(&sr) % MAGIC + 1;
          if (vp + vc > BUFSIZE1) { vc = BUFSIZE1 - vp; }

          assert_info(fio_write(pf, &sbuf[vp], vc) == vc, M);

          if ((vp += vc) == BUFSIZE1) { break; }
        }

        assert_info(fio_close(pf) == 0, M);
        assert_info(check_file_exist(pfname), M);
        assert_info(check_file_size(pfname, BUFSIZE1), M);
      }

      assert_info(fio_open(pf, pfname, vmode, BM_DEFAULT) == 0, M);
      assert_info(fio_read(pf, &sbuf2[0], BUFSIZE1) == BUFSIZE1, M);
      assert_info(fio_close(pf) == 0, M);
      assert_info(check_file_exist(pfname), M);
      assert_info(check_file_size(pfname, BUFSIZE1), M);
      assert_info(memcmp(sbuf, sbuf2, BUFSIZE1) == 0, M);
    }
  }

  success();
}

/* ------------------------------------------------------------------------- */
