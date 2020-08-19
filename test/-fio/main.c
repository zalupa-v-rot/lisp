/* ------------------------------------------------------------------------- */

#include "./comm.h"

#define M strmode(vmode)

int main(void) {

  struct fio sfl, * const pf = &sfl;


  strmagic(sbuf, BUFSIZE1);

  /* открыть только существующий файл */

  {
    int vi, vmode;

    for (vi = 0; vi != CODES_COMB; vi++) {

      vmode = mode_code(vi);

      if (vmode & FIOM_CREATE) { continue; }

      assert_info(fio_open(pf, T"/not-exist", vmode, BM_DEFAULT) != 0, M);
      assert_info(fio_close(pf) != 0, M);
      assert_info(check_file_notexist(T"/not-exist"), M);
    }
  }

  /* открыть несуществующий файл - создать */

  {
    int vi, va = 0, vmode;
    const char *pfname;

    for (vi = 1; vi != CODES_COMB; vi++) {

      vmode = mode_code(vi);
      pfname = sprintf_b(T"/exist-%i", va);

      if (!(vmode & FIOM_CREATE) ||
          (!(vmode & FIOM_READ) && !(vmode & FIOM_WRITE))) {

        assert_info(fio_open(pf, T"/not-exist", vmode, BM_DEFAULT) != 0, M);
        assert_info(check_file_notexist(T"/not-exist"), M);
        continue;
      }

      assert_info(fio_open(pf, pfname, vmode, BM_DEFAULT) == 0, M);
      /* assert_info(fio_ptell(pf, FIO_PEND) == 0, M); */
      /* assert_info(fio_ptell(pf, FIO_PBEG) == 0, M); */
      assert_info(fio_close(pf) == 0, M);
      assert_info(fio_open(pf, pfname, FIOM_READ, BM_DEFAULT) == 0, M);
      /* assert_info(fio_ptell(pf, FIO_PEND) == 0, M); */
      /* assert_info(fio_ptell(pf, FIO_PBEG) == 0, M); */
      assert_info(fio_close(pf) == 0, M);
      assert_info(check_file_exist(pfname), M);
      assert_info(check_file_size(pfname, 0), M);
      va++;
    }

    for ( ; va != CODES_COMB; va++) {

      vmode = FIOM_CREATE|FIOM_READ;
      pfname = sprintf_b(T"/exist-%i", va);

      assert_info(fio_open(pf, pfname, vmode, BM_DEFAULT) == 0, M);
      /* assert_info(fio_ptell(pf, FIO_PEND) == 0, M); */
      /* assert_info(fio_ptell(pf, FIO_PBEG) == 0, M); */
      assert_info(fio_close(pf) == 0, M);
      assert_info(fio_open(pf, pfname, FIOM_READ, BM_DEFAULT) == 0, M);
      /* assert_info(fio_ptell(pf, FIO_PEND) == 0, M); */
      /* assert_info(fio_ptell(pf, FIO_PBEG) == 0, M); */
      assert_info(fio_close(pf) == 0, M);
      assert_info(check_file_exist(pfname), M);
      assert_info(check_file_size(pfname, 0), M);
    }
  }


  /* что-бы расширить файл нужны права FIOM_WRITE и EXTEND своего доступа. */

  {
    int vi, va = 0, vmode;
    const char *pfname;

    for (vi = 1; vi != CODES_COMB; vi++) {

      vmode = mode_code(vi);
      pfname = sprintf_b(T"/exist-%i", va);

      do {

        if (!(vmode & FIOM_READ) && !(vmode & FIOM_WRITE)) {

          assert_info(fio_open(pf, pfname, vmode, BM_DEFAULT) != 0, M);
          break;
        }

        assert_info(fio_open(pf, pfname, vmode, BM_DEFAULT) == 0, M);

        if (!(vmode & FIOM_WRITE) && !(vmode & FIOM_WEXTEND)) {
          assert_info(fio_write(pf, NULL2, BUFSIZE0) == 0, M);
        }
        if (!(vmode & FIOM_WRITE) && !(vmode & FIOM_REXTEND)) {
          assert_info(fio_read(pf, NULL2, BUFSIZE0) == 0, M);
        }

        assert_info(fio_close(pf) == 0, M);
        va++;

      } while(0);

      assert_info(check_file_exist(pfname), M);
      assert_info(check_file_size(pfname, 0), M);
      assert_info(fio_open(pf, pfname, FIOM_READ, BM_DEFAULT) == 0, M);
      /* assert_info(fio_ptell(pf, FIO_PEND) == 0, M); */
      /* assert_info(fio_ptell(pf, FIO_PBEG) == 0, M); */
      assert_info(fio_read(pf, NULL2, BUFSIZE0) == 0, M);
      /* assert_info(fio_ptell(pf, FIO_PEND) == 0, M); */
      /* assert_info(fio_ptell(pf, FIO_PBEG) == 0, M); */
      assert_info(fio_close(pf) == 0, M);
      assert_info(check_file_size(pfname, 0), M);
    }
  }


  /* изменить существующий файл через расширение. */

  {
    int vi, va = 0, vmode, vb;
    const char *pfname;


    for (vi = 1; vi != CODES_COMB; vi++) {

      vb = 0;

      vmode = mode_code(vi);
      pfname = sprintf_b(T"/exist-%i", va);

      if (!(vmode & FIOM_WRITE) ||

          (!(vmode & FIOM_WEXTEND) &&
           (!(vmode & FIOM_READ) || !(vmode & FIOM_REXTEND))) ) {

        continue;
      }

      assert_info(fio_open(pf, pfname, vmode, BM_DEFAULT) == 0, M);

      if (vmode & FIOM_WEXTEND) {

        assert_info(fio_write(pf, sbuf, BUFSIZE0) == BUFSIZE0, M);
        vb++;
      }
      if ((vmode & FIOM_READ) && (vmode & FIOM_REXTEND)) {

        assert_info(fio_read(pf, sbuf, BUFSIZE0) == BUFSIZE0, M);
        vb++;
      }

      /* assert_info(fio_ptell(pf, FIO_PEND) == 0, M); */
      /* assert_info(fio_ptell(pf, FIO_PBEG) == -(BUFSIZE0 * vb), M); */
      assert_info(fio_close(pf) == 0, M);

      assert_info(check_file_exist(pfname), M);
      printf("!!%i %i\n", vb, (int)(BUFSIZE0 * vb));
      assert_info(check_file_size(pfname, BUFSIZE0 * vb), M);
      assert_info(fio_open(pf, pfname, FIOM_READ, BM_DEFAULT) == 0, M);
      /* assert_info(fio_ptell(pf, FIO_PEND) == (BUFSIZE0 * vb), M); */
      /* assert_info(fio_ptell(pf, FIO_PBEG) == 0, M); */

      assert_info(fio_read(pf, sbuf2, BUFSIZE0) == BUFSIZE0, M);
      /* assert_info(fio_ptell(pf, FIO_PEND) == BUFSIZE0*vb-BUFSIZE0, M); */
      /* assert_info(fio_ptell(pf, FIO_PBEG) == -BUFSIZE0, M); */
      assert_info(memcmp(sbuf, sbuf2, BUFSIZE0) == 0, M);
      if (vb == 2) {
        assert_info(fio_read(pf, sbuf2, BUFSIZE0) == BUFSIZE0, M);
        /* assert_info(fio_ptell(pf, FIO_PEND) == 0, M); */
        /* assert_info(fio_ptell(pf, FIO_PBEG) == -BUFSIZE0*2, M); */
        assert_info(memcmp(sbuf, sbuf2, BUFSIZE0) == 0, M);
      }
      assert_info(fio_close(pf) == 0, M);
      assert_info(check_file_exist(pfname), M);
      assert_info(check_file_size(pfname, BUFSIZE0 * vb), M);
      va++;
    }
  }

  success();
}

/* ------------------------------------------------------------------------- */
