/* ------------------------------------------------------------------------- */

static int add_u(struct sm_head *ph,
                 struct string *pa, uintp vsigna,
                 const struct string *pb, uintp vsignb) {

  uintp vblen, valen, vminlen;
  uchar vc;


  valen = pa->vlen - vsigna;
  vblen = pb->vlen - vsignb;


  if (valen < vblen) {

    if (ph->strrealloc_l(ph->pmh, pa, vblen + vsigna)) {

      return 1;
    }

    zeros(&pa->pstr[vsigna], vblen - valen);

    valen = pa->vlen - vsigna;
    vminlen = valen;

  } else {

    vminlen = vblen;
  }


  if (vc = add(&pa->pstr[pa->vlen-1], valen, &pb->pstr[pb->vlen-1], vminlen)) {

    if (ph->strrealloc_l(ph->pmh, pa, pa->vlen + 1)) {

      return 1;
    }
    pa->pstr[vsigna] = '0' + vc;
  }

  if (vsigna) {

    pa->pstr[0] = '-';
  }

  return 0;
}

/* a sign b    <   >
   +1  -  +1 = -   +
   -1  -  -1 = +   -
   +1  +  -1 = -   +
   -1  +  +1 = +   - */

static int sub_u(struct sm_head *ph,
                 struct string *pa, uintp vsigna,
                 const struct string *pb, uintp vsignb) {

  schar vcmp;
  const char *pmin, *pmax;
  uintp vminlen, valen, vblen, vsignc;


  valen = pa->vlen - vsigna;
  vblen = pb->vlen - vsignb;

  /* вычитание только младшего из старшего */
  if ((vcmp = cmp(&pa->pstr[vsigna], valen, &pb->pstr[vsignb], vblen)) < 0) {

    vsignc = vsigna ^ 1;

    if (ph->strrealloc_l(ph->pmh, pa, vblen + vsignc)) {

      return 1;
    }

    zeros(&pa->pstr[vsignc], vblen - valen);

    valen = pa->vlen - vsignc;

    pmin = &pa->pstr[pa->vlen-1];
    pmax = &pb->pstr[pb->vlen-1];
    vminlen = valen;

  } else {

    vsignc = vsigna;

    pmin = &pb->pstr[pb->vlen-1];
    pmax = &pa->pstr[pa->vlen-1];
    vminlen = vblen;
  }


  sub_from_eqlarger(pmax, pmin, vminlen, &pa->pstr[pa->vlen-1]);
  reduce_lead_zeros(&pa->pstr[vsignc], &valen);

  if (valen == 0) {

    valen = 1, vsignc = 0;
  }
  if (vsignc) {

    pa->pstr[pa->vlen - valen - 1] = '-';
  }

  return ph->strrealloc_l(ph->pmh, pa, vsignc + valen);
}

static int mul_u(struct sm_head *ph,
                 const struct string *pa, uintp vsigna,
                 const struct string *pb, uintp vsignb,
                 struct string *pc, uintp vsignc) {

  uintp vclen = pa->vlen - vsigna + pb->vlen - vsignb;


  if (ph->stralloc(ph->pmh, pc, vclen + vsignc)) {

    return 1;
  }

  zeros(&pc->pstr[vsignc], pc->vlen - vsignc);

  muladd(&pa->pstr[pa->vlen-1], pa->vlen - vsigna,
         &pb->pstr[pb->vlen-1], pb->vlen - vsignb,
         &pc->pstr[pc->vlen-1]);

  reduce_lead_zeros(&pc->pstr[vsignc], &vclen);

  if (vclen == 0) {

    vclen = 1, vsignc = 0;
  }
  if (vsignc) {

    pc->pstr[pc->vlen - vclen - 1] = '-';
  }

  if (ph->strrealloc_l(ph->pmh, pc, vsignc + vclen)) {

    ph->strfree(ph->pmh, pc);
    return 1;
  }

  return 0;
}

static int div_u(struct sm_head *ph,
                 struct string *pa, uintp vsigna,
                 const struct string *pb, uintp vsignb,
                 struct string *pc, uintp vsignc) {

  uintp valen, vblen, vclen;


  valen = pa->vlen - vsigna;
  vblen = pb->vlen - vsignb;

  if (valen <= vblen) {

    vclen = 1;

  } else {

    vclen = valen - vblen + 1;
  }

  if (ph->stralloc(ph->pmh, pc, vsignc + vclen)) {

    return 1;
  }

  div(&pa->pstr[vsigna] + valen - 1, &valen, &pb->pstr[vsignb], vblen,
      &pc->pstr[vsignc], &vclen);

  if (vclen == 0) {

    vclen = 1, vsignc = 0;
  }
  if (vsignc) {

    pc->pstr[pc->vlen - vclen - 1] = '-';
  }
  if (ph->strrealloc_l(ph->pmh, pc, vsignc + vclen)) {

    ph->strfree(ph->pmh, pc);
    return 1;
  }


  if (valen == 0) {

    valen = 1, vsigna = 0;
  }
  if (vsigna) {

    pa->pstr[pa->vlen - valen - 1] = '-';
  }
  if (ph->strrealloc_l(ph->pmh, pa, vsigna + valen)) {

    ph->strfree(ph->pmh, pc);
    return 1;
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
