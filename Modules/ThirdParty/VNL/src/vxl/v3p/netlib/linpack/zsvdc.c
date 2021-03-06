/* linpack/zsvdc.f -- translated by f2c (version 20050501).
   You must link the resulting object file with libf2c:
        on Microsoft Windows system, link with libf2c.lib;
        on Linux or Unix systems, link with .../path/to/libf2c.a -lm
        or, if you install libf2c.a in a standard place, with -lf2c -lm
        -- in that order, at the end of the command line, as in
                cc *.o -lf2c -lm
        Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

                http://www.netlib.org/f2c/libf2c.zip
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "v3p_netlib.h"

/* Table of constant values */

static integer c__1 = 1;
static doublecomplex c_b10 = {1.,0.};
static doublecomplex c_b60 = {-1.,0.};

/*<       subroutine zsvdc(x,ldx,n,p,s,e,u,ldu,v,ldv,work,job,info) >*/
/* Subroutine */ int zsvdc_(doublecomplex *x, integer *ldx, integer *n, 
        integer *p, doublecomplex *s, doublecomplex *e, doublecomplex *u, 
        integer *ldu, doublecomplex *v, integer *ldv, doublecomplex *work, 
        integer *job, integer *info)
{
    /* System generated locals */
    integer x_dim1, x_offset, u_dim1, u_offset, v_dim1, v_offset, i__1, i__2, 
            i__3, i__4, i__5;
    doublereal d__1, d__2, d__3, d__4;
    doublecomplex z__1, z__2, z__3;

    /* Builtin functions */
    double z_abs(doublecomplex *);
    void z_div(doublecomplex *, doublecomplex *, doublecomplex *), d_cnjg(
            doublecomplex *, doublecomplex *);
    double sqrt(doublereal);

    /* Local variables */
    doublereal b, c__, f, g;
    integer i__, j, k, l=0, m;
    doublecomplex r__, t;
    doublereal t1, el;
    integer kk;
    doublereal cs;
    integer ll, mm, ls=0;
    doublereal sl;
    integer lu;
    doublereal sm, sn;
    integer lm1, mm1, lp1, mp1, nct, ncu, lls, nrt;
    doublereal emm1, smm1;
    integer kase, jobu, iter;
    doublereal test;
    integer nctp1, nrtp1;
    doublereal scale;
    extern /* Subroutine */ int zscal_(integer *, doublecomplex *, 
            doublecomplex *, integer *);
    doublereal shift;
    extern /* Subroutine */ int drotg_(doublereal *, doublereal *, doublereal 
            *, doublereal *);
    integer maxit;
    extern /* Double Complex */ VOID zdotc_(doublecomplex *, integer *, 
            doublecomplex *, integer *, doublecomplex *, integer *);
    logical wantu, wantv;
    extern /* Subroutine */ int zdrot_(integer *, doublecomplex *, integer *, 
            doublecomplex *, integer *, doublereal *, doublereal *), zswap_(
            integer *, doublecomplex *, integer *, doublecomplex *, integer *)
            ;
    doublereal ztest;
    extern /* Subroutine */ int zaxpy_(integer *, doublecomplex *, 
            doublecomplex *, integer *, doublecomplex *, integer *);
    extern doublereal dznrm2_(integer *, doublecomplex *, integer *);

/*<       integer ldx,n,p,ldu,ldv,job,info >*/
/*<       complex*16 x(ldx,1),s(1),e(1),u(ldu,1),v(ldv,1),work(1) >*/


/*     zsvdc is a subroutine to reduce a complex*16 nxp matrix x by */
/*     unitary transformations u and v to diagonal form.  the */
/*     diagonal elements s(i) are the singular values of x.  the */
/*     columns of u are the corresponding left singular vectors, */
/*     and the columns of v the right singular vectors. */

/*     on entry */

/*         x         complex*16(ldx,p), where ldx.ge.n. */
/*                   x contains the matrix whose singular value */
/*                   decomposition is to be computed.  x is */
/*                   destroyed by zsvdc. */

/*         ldx       integer. */
/*                   ldx is the leading dimension of the array x. */

/*         n         integer. */
/*                   n is the number of rows of the matrix x. */

/*         p         integer. */
/*                   p is the number of columns of the matrix x. */

/*         ldu       integer. */
/*                   ldu is the leading dimension of the array u */
/*                   (see below). */

/*         ldv       integer. */
/*                   ldv is the leading dimension of the array v */
/*                   (see below). */

/*         work      complex*16(n). */
/*                   work is a scratch array. */

/*         job       integer. */
/*                   job controls the computation of the singular */
/*                   vectors.  it has the decimal expansion ab */
/*                   with the following meaning */

/*                        a.eq.0    do not compute the left singular */
/*                                  vectors. */
/*                        a.eq.1    return the n left singular vectors */
/*                                  in u. */
/*                        a.ge.2    returns the first min(n,p) */
/*                                  left singular vectors in u. */
/*                        b.eq.0    do not compute the right singular */
/*                                  vectors. */
/*                        b.eq.1    return the right singular vectors */
/*                                  in v. */

/*     on return */

/*         s         complex*16(mm), where mm=min(n+1,p). */
/*                   the first min(n,p) entries of s contain the */
/*                   singular values of x arranged in descending */
/*                   order of magnitude. */

/*         e         complex*16(p). */
/*                   e ordinarily contains zeros.  however see the */
/*                   discussion of info for exceptions. */

/*         u         complex*16(ldu,k), where ldu.ge.n.  if joba.eq.1 */
/*                                   then k.eq.n, if joba.ge.2 then */

/*                                   k.eq.min(n,p). */
/*                   u contains the matrix of left singular vectors. */
/*                   u is not referenced if joba.eq.0.  if n.le.p */
/*                   or if joba.gt.2, then u may be identified with x */
/*                   in the subroutine call. */

/*         v         complex*16(ldv,p), where ldv.ge.p. */
/*                   v contains the matrix of right singular vectors. */
/*                   v is not referenced if jobb.eq.0.  if p.le.n, */
/*                   then v may be identified whth x in the */
/*                   subroutine call. */

/*         info      integer. */
/*                   the singular values (and their corresponding */
/*                   singular vectors) s(info+1),s(info+2),...,s(m) */
/*                   are correct (here m=min(n,p)).  thus if */
/*                   info.eq.0, all the singular values and their */
/*                   vectors are correct.  in any event, the matrix */
/*                   b = ctrans(u)*x*v is the bidiagonal matrix */
/*                   with the elements of s on its diagonal and the */
/*                   elements of e on its super-diagonal (ctrans(u) */
/*                   is the conjugate-transpose of u).  thus the */
/*                   singular values of x and b are the same. */

/*     linpack. this version dated 03/19/79 . */
/*              correction to shift calculation made 2/85. */
/*     g.w. stewart, university of maryland, argonne national lab. */

/*     zsvdc uses the following functions and subprograms. */

/*     external zdrot */
/*     blas zaxpy,zdotc,zscal,zswap,dznrm2,drotg */
/*     fortran dabs,dmax1,cdabs,dcmplx */
/*     fortran dconjg,max0,min0,mod,dsqrt */

/*     internal variables */

/*<    >*/
/*<       complex*16 zdotc,t,r >*/
/*<    >*/
/*<       logical wantu,wantv >*/

/*<       complex*16 csign,zdum,zdum1,zdum2 >*/
/*<       double precision cabs1 >*/
/*<       double precision dreal,dimag >*/
/*<       complex*16 zdumr,zdumi >*/
/*<       dreal(zdumr) = zdumr >*/
/*<       dimag(zdumi) = (0.0d0,-1.0d0)*zdumi >*/
/*<       cabs1(zdum) = dabs(dreal(zdum)) + dabs(dimag(zdum)) >*/
/*<       csign(zdum1,zdum2) = cdabs(zdum1)*(zdum2/cdabs(zdum2)) >*/

/*     set the maximum number of iterations. */

/*<       maxit = 1000 >*/
    /* Parameter adjustments */
    x_dim1 = *ldx;
    x_offset = 1 + x_dim1;
    x -= x_offset;
    --s;
    --e;
    u_dim1 = *ldu;
    u_offset = 1 + u_dim1;
    u -= u_offset;
    v_dim1 = *ldv;
    v_offset = 1 + v_dim1;
    v -= v_offset;
    --work;

    /* Function Body */
    maxit = 1000;

/*     determine what is to be computed. */

/*<       wantu = .false. >*/
    wantu = FALSE_;
/*<       wantv = .false. >*/
    wantv = FALSE_;
/*<       jobu = mod(job,100)/10 >*/
    jobu = *job % 100 / 10;
/*<       ncu = n >*/
    ncu = *n;
/*<       if (jobu .gt. 1) ncu = min0(n,p) >*/
    if (jobu > 1) {
        ncu = min(*n,*p);
    }
/*<       if (jobu .ne. 0) wantu = .true. >*/
    if (jobu != 0) {
        wantu = TRUE_;
    }
/*<       if (mod(job,10) .ne. 0) wantv = .true. >*/
    if (*job % 10 != 0) {
        wantv = TRUE_;
    }

/*     reduce x to bidiagonal form, storing the diagonal elements */
/*     in s and the super-diagonal elements in e. */

/*<       info = 0 >*/
    *info = 0;
/*<       nct = min0(n-1,p) >*/
/* Computing MIN */
    i__1 = *n - 1;
    nct = min(i__1,*p);
/*<       nrt = max0(0,min0(p-2,n)) >*/
/* Computing MAX */
/* Computing MIN */
    i__3 = *p - 2;
    i__1 = 0, i__2 = min(i__3,*n);
    nrt = max(i__1,i__2);
/*<       lu = max0(nct,nrt) >*/
    lu = max(nct,nrt);
/*<       if (lu .lt. 1) go to 170 >*/
    if (lu < 1) {
        goto L170;
    }
/*<       do 160 l = 1, lu >*/
    i__1 = lu;
    for (l = 1; l <= i__1; ++l) {
/*<          lp1 = l + 1 >*/
        lp1 = l + 1;
/*<          if (l .gt. nct) go to 20 >*/
        if (l > nct) {
            goto L20;
        }

/*           compute the transformation for the l-th column and */
/*           place the l-th diagonal in s(l). */

/*<             s(l) = dcmplx(dznrm2(n-l+1,x(l,l),1),0.0d0) >*/
        i__2 = l;
        i__3 = *n - l + 1;
        d__1 = dznrm2_(&i__3, &x[l + l * x_dim1], &c__1);
        z__1.r = d__1, z__1.i = 0.;
        s[i__2].r = z__1.r, s[i__2].i = z__1.i;
/*<             if (cabs1(s(l)) .eq. 0.0d0) go to 10 >*/
        i__2 = l;
        i__3 = l;
        z__1.r = s[i__3].r * 0. - s[i__3].i * -1., z__1.i = s[i__3].i * 0. + 
                s[i__3].r * -1.;
        if ((d__1 = s[i__2].r, abs(d__1)) + (d__2 = z__1.r, abs(d__2)) == 0.) 
                {
            goto L10;
        }
/*<                if (cabs1(x(l,l)) .ne. 0.0d0) s(l) = csign(s(l),x(l,l)) >*/
        i__2 = l + l * x_dim1;
        i__3 = l + l * x_dim1;
        z__1.r = x[i__3].r * 0. - x[i__3].i * -1., z__1.i = x[i__3].i * 0. + 
                x[i__3].r * -1.;
        if ((d__1 = x[i__2].r, abs(d__1)) + (d__2 = z__1.r, abs(d__2)) != 0.) 
                {
            i__4 = l;
            d__3 = z_abs(&s[l]);
            i__5 = l + l * x_dim1;
            d__4 = z_abs(&x[l + l * x_dim1]);
            z__3.r = x[i__5].r / d__4, z__3.i = x[i__5].i / d__4;
            z__2.r = d__3 * z__3.r, z__2.i = d__3 * z__3.i;
            s[i__4].r = z__2.r, s[i__4].i = z__2.i;
        }
/*<                call zscal(n-l+1,1.0d0/s(l),x(l,l),1) >*/
        i__2 = *n - l + 1;
        z_div(&z__1, &c_b10, &s[l]);
        zscal_(&i__2, &z__1, &x[l + l * x_dim1], &c__1);
/*<                x(l,l) = (1.0d0,0.0d0) + x(l,l) >*/
        i__2 = l + l * x_dim1;
        i__3 = l + l * x_dim1;
        z__1.r = x[i__3].r + 1., z__1.i = x[i__3].i + 0.;
        x[i__2].r = z__1.r, x[i__2].i = z__1.i;
/*<    10       continue >*/
L10:
/*<             s(l) = -s(l) >*/
        i__2 = l;
        i__3 = l;
        z__1.r = -s[i__3].r, z__1.i = -s[i__3].i;
        s[i__2].r = z__1.r, s[i__2].i = z__1.i;
/*<    20    continue >*/
L20:
/*<          if (p .lt. lp1) go to 50 >*/
        if (*p < lp1) {
            goto L50;
        }
/*<          do 40 j = lp1, p >*/
        i__2 = *p;
        for (j = lp1; j <= i__2; ++j) {
/*<             if (l .gt. nct) go to 30 >*/
            if (l > nct) {
                goto L30;
            }
/*<             if (cabs1(s(l)) .eq. 0.0d0) go to 30 >*/
            i__3 = l;
            i__4 = l;
            z__1.r = s[i__4].r * 0. - s[i__4].i * -1., z__1.i = s[i__4].i * 
                    0. + s[i__4].r * -1.;
            if ((d__1 = s[i__3].r, abs(d__1)) + (d__2 = z__1.r, abs(d__2)) == 
                    0.) {
                goto L30;
            }

/*              apply the transformation. */

/*<                t = -zdotc(n-l+1,x(l,l),1,x(l,j),1)/x(l,l) >*/
            i__3 = *n - l + 1;
            zdotc_(&z__3, &i__3, &x[l + l * x_dim1], &c__1, &x[l + j * x_dim1]
                    , &c__1);
            z__2.r = -z__3.r, z__2.i = -z__3.i;
            z_div(&z__1, &z__2, &x[l + l * x_dim1]);
            t.r = z__1.r, t.i = z__1.i;
/*<                call zaxpy(n-l+1,t,x(l,l),1,x(l,j),1) >*/
            i__3 = *n - l + 1;
            zaxpy_(&i__3, &t, &x[l + l * x_dim1], &c__1, &x[l + j * x_dim1], &
                    c__1);
/*<    30       continue >*/
L30:

/*           place the l-th row of x into  e for the */
/*           subsequent calculation of the row transformation. */

/*<             e(j) = dconjg(x(l,j)) >*/
            i__3 = j;
            d_cnjg(&z__1, &x[l + j * x_dim1]);
            e[i__3].r = z__1.r, e[i__3].i = z__1.i;
/*<    40    continue >*/
/* L40: */
        }
/*<    50    continue >*/
L50:
/*<          if (.not.wantu .or. l .gt. nct) go to 70 >*/
        if (! wantu || l > nct) {
            goto L70;
        }

/*           place the transformation in u for subsequent back */
/*           multiplication. */

/*<             do 60 i = l, n >*/
        i__2 = *n;
        for (i__ = l; i__ <= i__2; ++i__) {
/*<                u(i,l) = x(i,l) >*/
            i__3 = i__ + l * u_dim1;
            i__4 = i__ + l * x_dim1;
            u[i__3].r = x[i__4].r, u[i__3].i = x[i__4].i;
/*<    60       continue >*/
/* L60: */
        }
/*<    70    continue >*/
L70:
/*<          if (l .gt. nrt) go to 150 >*/
        if (l > nrt) {
            goto L150;
        }

/*           compute the l-th row transformation and place the */
/*           l-th super-diagonal in e(l). */

/*<             e(l) = dcmplx(dznrm2(p-l,e(lp1),1),0.0d0) >*/
        i__2 = l;
        i__3 = *p - l;
        d__1 = dznrm2_(&i__3, &e[lp1], &c__1);
        z__1.r = d__1, z__1.i = 0.;
        e[i__2].r = z__1.r, e[i__2].i = z__1.i;
/*<             if (cabs1(e(l)) .eq. 0.0d0) go to 80 >*/
        i__2 = l;
        i__3 = l;
        z__1.r = e[i__3].r * 0. - e[i__3].i * -1., z__1.i = e[i__3].i * 0. + 
                e[i__3].r * -1.;
        if ((d__1 = e[i__2].r, abs(d__1)) + (d__2 = z__1.r, abs(d__2)) == 0.) 
                {
            goto L80;
        }
/*<                if (cabs1(e(lp1)) .ne. 0.0d0) e(l) = csign(e(l),e(lp1)) >*/
        i__2 = lp1;
        i__3 = lp1;
        z__1.r = e[i__3].r * 0. - e[i__3].i * -1., z__1.i = e[i__3].i * 0. + 
                e[i__3].r * -1.;
        if ((d__1 = e[i__2].r, abs(d__1)) + (d__2 = z__1.r, abs(d__2)) != 0.) 
                {
            i__4 = l;
            d__3 = z_abs(&e[l]);
            i__5 = lp1;
            d__4 = z_abs(&e[lp1]);
            z__3.r = e[i__5].r / d__4, z__3.i = e[i__5].i / d__4;
            z__2.r = d__3 * z__3.r, z__2.i = d__3 * z__3.i;
            e[i__4].r = z__2.r, e[i__4].i = z__2.i;
        }
/*<                call zscal(p-l,1.0d0/e(l),e(lp1),1) >*/
        i__2 = *p - l;
        z_div(&z__1, &c_b10, &e[l]);
        zscal_(&i__2, &z__1, &e[lp1], &c__1);
/*<                e(lp1) = (1.0d0,0.0d0) + e(lp1) >*/
        i__2 = lp1;
        i__3 = lp1;
        z__1.r = e[i__3].r + 1., z__1.i = e[i__3].i + 0.;
        e[i__2].r = z__1.r, e[i__2].i = z__1.i;
/*<    80       continue >*/
L80:
/*<             e(l) = -dconjg(e(l)) >*/
        i__2 = l;
        d_cnjg(&z__2, &e[l]);
        z__1.r = -z__2.r, z__1.i = -z__2.i;
        e[i__2].r = z__1.r, e[i__2].i = z__1.i;
/*<             if (lp1 .gt. n .or. cabs1(e(l)) .eq. 0.0d0) go to 120 >*/
        i__2 = l;
        i__3 = l;
        z__1.r = e[i__3].r * 0. - e[i__3].i * -1., z__1.i = e[i__3].i * 0. + 
                e[i__3].r * -1.;
        if (lp1 > *n || (d__1 = e[i__2].r, abs(d__1)) + (d__2 = z__1.r, abs(
                d__2)) == 0.) {
            goto L120;
        }

/*              apply the transformation. */

/*<                do 90 i = lp1, n >*/
        i__2 = *n;
        for (i__ = lp1; i__ <= i__2; ++i__) {
/*<                   work(i) = (0.0d0,0.0d0) >*/
            i__3 = i__;
            work[i__3].r = 0., work[i__3].i = 0.;
/*<    90          continue >*/
/* L90: */
        }
/*<                do 100 j = lp1, p >*/
        i__2 = *p;
        for (j = lp1; j <= i__2; ++j) {
/*<                   call zaxpy(n-l,e(j),x(lp1,j),1,work(lp1),1) >*/
            i__3 = *n - l;
            zaxpy_(&i__3, &e[j], &x[lp1 + j * x_dim1], &c__1, &work[lp1], &
                    c__1);
/*<   100          continue >*/
/* L100: */
        }
/*<                do 110 j = lp1, p >*/
        i__2 = *p;
        for (j = lp1; j <= i__2; ++j) {
/*<    >*/
            i__3 = *n - l;
            i__4 = j;
            z__3.r = -e[i__4].r, z__3.i = -e[i__4].i;
            z_div(&z__2, &z__3, &e[lp1]);
            d_cnjg(&z__1, &z__2);
            zaxpy_(&i__3, &z__1, &work[lp1], &c__1, &x[lp1 + j * x_dim1], &
                    c__1);
/*<   110          continue >*/
/* L110: */
        }
/*<   120       continue >*/
L120:
/*<             if (.not.wantv) go to 140 >*/
        if (! wantv) {
            goto L140;
        }

/*              place the transformation in v for subsequent */
/*              back multiplication. */

/*<                do 130 i = lp1, p >*/
        i__2 = *p;
        for (i__ = lp1; i__ <= i__2; ++i__) {
/*<                   v(i,l) = e(i) >*/
            i__3 = i__ + l * v_dim1;
            i__4 = i__;
            v[i__3].r = e[i__4].r, v[i__3].i = e[i__4].i;
/*<   130          continue >*/
/* L130: */
        }
/*<   140       continue >*/
L140:
/*<   150    continue >*/
L150:
/*<   160 continue >*/
/* L160: */
        ;
    }
/*<   170 continue >*/
L170:

/*     set up the final bidiagonal matrix or order m. */

/*<       m = min0(p,n+1) >*/
/* Computing MIN */
    i__1 = *p, i__2 = *n + 1;
    m = min(i__1,i__2);
/*<       nctp1 = nct + 1 >*/
    nctp1 = nct + 1;
/*<       nrtp1 = nrt + 1 >*/
    nrtp1 = nrt + 1;
/*<       if (nct .lt. p) s(nctp1) = x(nctp1,nctp1) >*/
    if (nct < *p) {
        i__1 = nctp1;
        i__2 = nctp1 + nctp1 * x_dim1;
        s[i__1].r = x[i__2].r, s[i__1].i = x[i__2].i;
    }
/*<       if (n .lt. m) s(m) = (0.0d0,0.0d0) >*/
    if (*n < m) {
        i__1 = m;
        s[i__1].r = 0., s[i__1].i = 0.;
    }
/*<       if (nrtp1 .lt. m) e(nrtp1) = x(nrtp1,m) >*/
    if (nrtp1 < m) {
        i__1 = nrtp1;
        i__2 = nrtp1 + m * x_dim1;
        e[i__1].r = x[i__2].r, e[i__1].i = x[i__2].i;
    }
/*<       e(m) = (0.0d0,0.0d0) >*/
    i__1 = m;
    e[i__1].r = 0., e[i__1].i = 0.;

/*     if required, generate u. */

/*<       if (.not.wantu) go to 300 >*/
    if (! wantu) {
        goto L300;
    }
/*<          if (ncu .lt. nctp1) go to 200 >*/
    if (ncu < nctp1) {
        goto L200;
    }
/*<          do 190 j = nctp1, ncu >*/
    i__1 = ncu;
    for (j = nctp1; j <= i__1; ++j) {
/*<             do 180 i = 1, n >*/
        i__2 = *n;
        for (i__ = 1; i__ <= i__2; ++i__) {
/*<                u(i,j) = (0.0d0,0.0d0) >*/
            i__3 = i__ + j * u_dim1;
            u[i__3].r = 0., u[i__3].i = 0.;
/*<   180       continue >*/
/* L180: */
        }
/*<             u(j,j) = (1.0d0,0.0d0) >*/
        i__2 = j + j * u_dim1;
        u[i__2].r = 1., u[i__2].i = 0.;
/*<   190    continue >*/
/* L190: */
    }
/*<   200    continue >*/
L200:
/*<          if (nct .lt. 1) go to 290 >*/
    if (nct < 1) {
        goto L290;
    }
/*<          do 280 ll = 1, nct >*/
    i__1 = nct;
    for (ll = 1; ll <= i__1; ++ll) {
/*<             l = nct - ll + 1 >*/
        l = nct - ll + 1;
/*<             if (cabs1(s(l)) .eq. 0.0d0) go to 250 >*/
        i__2 = l;
        i__3 = l;
        z__1.r = s[i__3].r * 0. - s[i__3].i * -1., z__1.i = s[i__3].i * 0. + 
                s[i__3].r * -1.;
        if ((d__1 = s[i__2].r, abs(d__1)) + (d__2 = z__1.r, abs(d__2)) == 0.) 
                {
            goto L250;
        }
/*<                lp1 = l + 1 >*/
        lp1 = l + 1;
/*<                if (ncu .lt. lp1) go to 220 >*/
        if (ncu < lp1) {
            goto L220;
        }
/*<                do 210 j = lp1, ncu >*/
        i__2 = ncu;
        for (j = lp1; j <= i__2; ++j) {
/*<                   t = -zdotc(n-l+1,u(l,l),1,u(l,j),1)/u(l,l) >*/
            i__3 = *n - l + 1;
            zdotc_(&z__3, &i__3, &u[l + l * u_dim1], &c__1, &u[l + j * u_dim1]
                    , &c__1);
            z__2.r = -z__3.r, z__2.i = -z__3.i;
            z_div(&z__1, &z__2, &u[l + l * u_dim1]);
            t.r = z__1.r, t.i = z__1.i;
/*<                   call zaxpy(n-l+1,t,u(l,l),1,u(l,j),1) >*/
            i__3 = *n - l + 1;
            zaxpy_(&i__3, &t, &u[l + l * u_dim1], &c__1, &u[l + j * u_dim1], &
                    c__1);
/*<   210          continue >*/
/* L210: */
        }
/*<   220          continue >*/
L220:
/*<                call zscal(n-l+1,(-1.0d0,0.0d0),u(l,l),1) >*/
        i__2 = *n - l + 1;
        zscal_(&i__2, &c_b60, &u[l + l * u_dim1], &c__1);
/*<                u(l,l) = (1.0d0,0.0d0) + u(l,l) >*/
        i__2 = l + l * u_dim1;
        i__3 = l + l * u_dim1;
        z__1.r = u[i__3].r + 1., z__1.i = u[i__3].i + 0.;
        u[i__2].r = z__1.r, u[i__2].i = z__1.i;
/*<                lm1 = l - 1 >*/
        lm1 = l - 1;
/*<                if (lm1 .lt. 1) go to 240 >*/
        if (lm1 < 1) {
            goto L240;
        }
/*<                do 230 i = 1, lm1 >*/
        i__2 = lm1;
        for (i__ = 1; i__ <= i__2; ++i__) {
/*<                   u(i,l) = (0.0d0,0.0d0) >*/
            i__3 = i__ + l * u_dim1;
            u[i__3].r = 0., u[i__3].i = 0.;
/*<   230          continue >*/
/* L230: */
        }
/*<   240          continue >*/
L240:
/*<             go to 270 >*/
        goto L270;
/*<   250       continue >*/
L250:
/*<                do 260 i = 1, n >*/
        i__2 = *n;
        for (i__ = 1; i__ <= i__2; ++i__) {
/*<                   u(i,l) = (0.0d0,0.0d0) >*/
            i__3 = i__ + l * u_dim1;
            u[i__3].r = 0., u[i__3].i = 0.;
/*<   260          continue >*/
/* L260: */
        }
/*<                u(l,l) = (1.0d0,0.0d0) >*/
        i__2 = l + l * u_dim1;
        u[i__2].r = 1., u[i__2].i = 0.;
/*<   270       continue >*/
L270:
/*<   280    continue >*/
/* L280: */
        ;
    }
/*<   290    continue >*/
L290:
/*<   300 continue >*/
L300:

/*     if it is required, generate v. */

/*<       if (.not.wantv) go to 350 >*/
    if (! wantv) {
        goto L350;
    }
/*<          do 340 ll = 1, p >*/
    i__1 = *p;
    for (ll = 1; ll <= i__1; ++ll) {
/*<             l = p - ll + 1 >*/
        l = *p - ll + 1;
/*<             lp1 = l + 1 >*/
        lp1 = l + 1;
/*<             if (l .gt. nrt) go to 320 >*/
        if (l > nrt) {
            goto L320;
        }
/*<             if (cabs1(e(l)) .eq. 0.0d0) go to 320 >*/
        i__2 = l;
        i__3 = l;
        z__1.r = e[i__3].r * 0. - e[i__3].i * -1., z__1.i = e[i__3].i * 0. + 
                e[i__3].r * -1.;
        if ((d__1 = e[i__2].r, abs(d__1)) + (d__2 = z__1.r, abs(d__2)) == 0.) 
                {
            goto L320;
        }
/*<                do 310 j = lp1, p >*/
        i__2 = *p;
        for (j = lp1; j <= i__2; ++j) {
/*<                   t = -zdotc(p-l,v(lp1,l),1,v(lp1,j),1)/v(lp1,l) >*/
            i__3 = *p - l;
            zdotc_(&z__3, &i__3, &v[lp1 + l * v_dim1], &c__1, &v[lp1 + j * 
                    v_dim1], &c__1);
            z__2.r = -z__3.r, z__2.i = -z__3.i;
            z_div(&z__1, &z__2, &v[lp1 + l * v_dim1]);
            t.r = z__1.r, t.i = z__1.i;
/*<                   call zaxpy(p-l,t,v(lp1,l),1,v(lp1,j),1) >*/
            i__3 = *p - l;
            zaxpy_(&i__3, &t, &v[lp1 + l * v_dim1], &c__1, &v[lp1 + j * 
                    v_dim1], &c__1);
/*<   310          continue >*/
/* L310: */
        }
/*<   320       continue >*/
L320:
/*<             do 330 i = 1, p >*/
        i__2 = *p;
        for (i__ = 1; i__ <= i__2; ++i__) {
/*<                v(i,l) = (0.0d0,0.0d0) >*/
            i__3 = i__ + l * v_dim1;
            v[i__3].r = 0., v[i__3].i = 0.;
/*<   330       continue >*/
/* L330: */
        }
/*<             v(l,l) = (1.0d0,0.0d0) >*/
        i__2 = l + l * v_dim1;
        v[i__2].r = 1., v[i__2].i = 0.;
/*<   340    continue >*/
/* L340: */
    }
/*<   350 continue >*/
L350:

/*     transform s and e so that they are double precision. */

/*<       do 380 i = 1, m >*/
    i__1 = m;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*<          if (cabs1(s(i)) .eq. 0.0d0) go to 360 >*/
        i__2 = i__;
        i__3 = i__;
        z__1.r = s[i__3].r * 0. - s[i__3].i * -1., z__1.i = s[i__3].i * 0. + 
                s[i__3].r * -1.;
        if ((d__1 = s[i__2].r, abs(d__1)) + (d__2 = z__1.r, abs(d__2)) == 0.) 
                {
            goto L360;
        }
/*<             t = dcmplx(cdabs(s(i)),0.0d0) >*/
        d__1 = z_abs(&s[i__]);
        z__1.r = d__1, z__1.i = 0.;
        t.r = z__1.r, t.i = z__1.i;
/*<             r = s(i)/t >*/
        z_div(&z__1, &s[i__], &t);
        r__.r = z__1.r, r__.i = z__1.i;
/*<             s(i) = t >*/
        i__2 = i__;
        s[i__2].r = t.r, s[i__2].i = t.i;
/*<             if (i .lt. m) e(i) = e(i)/r >*/
        if (i__ < m) {
            i__2 = i__;
            z_div(&z__1, &e[i__], &r__);
            e[i__2].r = z__1.r, e[i__2].i = z__1.i;
        }
/*<             if (wantu) call zscal(n,r,u(1,i),1) >*/
        if (wantu) {
            zscal_(n, &r__, &u[i__ * u_dim1 + 1], &c__1);
        }
/*<   360    continue >*/
L360:
/*     ...exit */
/*<          if (i .eq. m) go to 390 >*/
        if (i__ == m) {
            goto L390;
        }
/*<          if (cabs1(e(i)) .eq. 0.0d0) go to 370 >*/
        i__2 = i__;
        i__3 = i__;
        z__1.r = e[i__3].r * 0. - e[i__3].i * -1., z__1.i = e[i__3].i * 0. + 
                e[i__3].r * -1.;
        if ((d__1 = e[i__2].r, abs(d__1)) + (d__2 = z__1.r, abs(d__2)) == 0.) 
                {
            goto L370;
        }
/*<             t = dcmplx(cdabs(e(i)),0.0d0) >*/
        d__1 = z_abs(&e[i__]);
        z__1.r = d__1, z__1.i = 0.;
        t.r = z__1.r, t.i = z__1.i;
/*<             r = t/e(i) >*/
        z_div(&z__1, &t, &e[i__]);
        r__.r = z__1.r, r__.i = z__1.i;
/*<             e(i) = t >*/
        i__2 = i__;
        e[i__2].r = t.r, e[i__2].i = t.i;
/*<             s(i+1) = s(i+1)*r >*/
        i__2 = i__ + 1;
        i__3 = i__ + 1;
        z__1.r = s[i__3].r * r__.r - s[i__3].i * r__.i, z__1.i = s[i__3].r * 
                r__.i + s[i__3].i * r__.r;
        s[i__2].r = z__1.r, s[i__2].i = z__1.i;
/*<             if (wantv) call zscal(p,r,v(1,i+1),1) >*/
        if (wantv) {
            zscal_(p, &r__, &v[(i__ + 1) * v_dim1 + 1], &c__1);
        }
/*<   370    continue >*/
L370:
/*<   380 continue >*/
/* L380: */
        ;
    }
/*<   390 continue >*/
L390:

/*     main iteration loop for the singular values. */

/*<       mm = m >*/
    mm = m;
/*<       iter = 0 >*/
    iter = 0;
/*<   400 continue >*/
L400:

/*        quit if all the singular values have been found. */

/*     ...exit */
/*<          if (m .eq. 0) go to 660 >*/
    if (m == 0) {
        goto L660;
    }

/*        if too many iterations have been performed, set */
/*        flag and return. */

/*<          if (iter .lt. maxit) go to 410 >*/
    if (iter < maxit) {
        goto L410;
    }
/*<             info = m >*/
    *info = m;
/*     ......exit */
/*<             go to 660 >*/
    goto L660;
/*<   410    continue >*/
L410:

/*        this section of the program inspects for */
/*        negligible elements in the s and e arrays.  on */
/*        completion the variables kase and l are set as follows. */

/*           kase = 1     if s(m) and e(l-1) are negligible and l.lt.m */
/*           kase = 2     if s(l) is negligible and l.lt.m */
/*           kase = 3     if e(l-1) is negligible, l.lt.m, and */
/*                        s(l), ..., s(m) are not negligible (qr step). */
/*           kase = 4     if e(m-1) is negligible (convergence). */

/*<          do 430 ll = 1, m >*/
    i__1 = m;
    for (ll = 1; ll <= i__1; ++ll) {
/*<             l = m - ll >*/
        l = m - ll;
/*        ...exit */
/*<             if (l .eq. 0) go to 440 >*/
        if (l == 0) {
            goto L440;
        }
/*<             test = cdabs(s(l)) + cdabs(s(l+1)) >*/
        test = z_abs(&s[l]) + z_abs(&s[l + 1]);
/*<             ztest = test + cdabs(e(l)) >*/
        ztest = test + z_abs(&e[l]);
/*<             if (ztest .ne. test) go to 420 >*/
        if (ztest != test) {
            goto L420;
        }
/*<                e(l) = (0.0d0,0.0d0) >*/
        i__2 = l;
        e[i__2].r = 0., e[i__2].i = 0.;
/*        ......exit */
/*<                go to 440 >*/
        goto L440;
/*<   420       continue >*/
L420:
/*<   430    continue >*/
/* L430: */
        ;
    }
/*<   440    continue >*/
L440:
/*<          if (l .ne. m - 1) go to 450 >*/
    if (l != m - 1) {
        goto L450;
    }
/*<             kase = 4 >*/
    kase = 4;
/*<          go to 520 >*/
    goto L520;
/*<   450    continue >*/
L450:
/*<             lp1 = l + 1 >*/
    lp1 = l + 1;
/*<             mp1 = m + 1 >*/
    mp1 = m + 1;
/*<             do 470 lls = lp1, mp1 >*/
    i__1 = mp1;
    for (lls = lp1; lls <= i__1; ++lls) {
/*<                ls = m - lls + lp1 >*/
        ls = m - lls + lp1;
/*           ...exit */
/*<                if (ls .eq. l) go to 480 >*/
        if (ls == l) {
            goto L480;
        }
/*<                test = 0.0d0 >*/
        test = 0.;
/*<                if (ls .ne. m) test = test + cdabs(e(ls)) >*/
        if (ls != m) {
            test += z_abs(&e[ls]);
        }
/*<                if (ls .ne. l + 1) test = test + cdabs(e(ls-1)) >*/
        if (ls != l + 1) {
            test += z_abs(&e[ls - 1]);
        }
/*<                ztest = test + cdabs(s(ls)) >*/
        ztest = test + z_abs(&s[ls]);
/*<                if (ztest .ne. test) go to 460 >*/
        if (ztest != test) {
            goto L460;
        }
/*<                   s(ls) = (0.0d0,0.0d0) >*/
        i__2 = ls;
        s[i__2].r = 0., s[i__2].i = 0.;
/*           ......exit */
/*<                   go to 480 >*/
        goto L480;
/*<   460          continue >*/
L460:
/*<   470       continue >*/
/* L470: */
        ;
    }
/*<   480       continue >*/
L480:
/*<             if (ls .ne. l) go to 490 >*/
    if (ls != l) {
        goto L490;
    }
/*<                kase = 3 >*/
    kase = 3;
/*<             go to 510 >*/
    goto L510;
/*<   490       continue >*/
L490:
/*<             if (ls .ne. m) go to 500 >*/
    if (ls != m) {
        goto L500;
    }
/*<                kase = 1 >*/
    kase = 1;
/*<             go to 510 >*/
    goto L510;
/*<   500       continue >*/
L500:
/*<                kase = 2 >*/
    kase = 2;
/*<                l = ls >*/
    l = ls;
/*<   510       continue >*/
L510:
/*<   520    continue >*/
L520:
/*<          l = l + 1 >*/
    ++l;

/*        perform the task indicated by kase. */

/*<          go to (530, 560, 580, 610), kase >*/
    switch (kase) {
        case 1:  goto L530;
        case 2:  goto L560;
        case 3:  goto L580;
        case 4:  goto L610;
    }

/*        deflate negligible s(m). */

/*<   530    continue >*/
L530:
/*<             mm1 = m - 1 >*/
    mm1 = m - 1;
/*<             f = dreal(e(m-1)) >*/
    i__1 = m - 1;
    f = e[i__1].r;
/*<             e(m-1) = (0.0d0,0.0d0) >*/
    i__1 = m - 1;
    e[i__1].r = 0., e[i__1].i = 0.;
/*<             do 550 kk = l, mm1 >*/
    i__1 = mm1;
    for (kk = l; kk <= i__1; ++kk) {
/*<                k = mm1 - kk + l >*/
        k = mm1 - kk + l;
/*<                t1 = dreal(s(k)) >*/
        i__2 = k;
        t1 = s[i__2].r;
/*<                call drotg(t1,f,cs,sn) >*/
        drotg_(&t1, &f, &cs, &sn);
/*<                s(k) = dcmplx(t1,0.0d0) >*/
        i__2 = k;
        z__1.r = t1, z__1.i = 0.;
        s[i__2].r = z__1.r, s[i__2].i = z__1.i;
/*<                if (k .eq. l) go to 540 >*/
        if (k == l) {
            goto L540;
        }
/*<                   f = -sn*dreal(e(k-1)) >*/
        i__2 = k - 1;
        f = -sn * e[i__2].r;
/*<                   e(k-1) = cs*e(k-1) >*/
        i__2 = k - 1;
        i__3 = k - 1;
        z__1.r = cs * e[i__3].r, z__1.i = cs * e[i__3].i;
        e[i__2].r = z__1.r, e[i__2].i = z__1.i;
/*<   540          continue >*/
L540:
/*<                if (wantv) call zdrot(p,v(1,k),1,v(1,m),1,cs,sn) >*/
        if (wantv) {
            zdrot_(p, &v[k * v_dim1 + 1], &c__1, &v[m * v_dim1 + 1], &c__1, &
                    cs, &sn);
        }
/*<   550       continue >*/
/* L550: */
    }
/*<          go to 650 >*/
    goto L650;

/*        split at negligible s(l). */

/*<   560    continue >*/
L560:
/*<             f = dreal(e(l-1)) >*/
    i__1 = l - 1;
    f = e[i__1].r;
/*<             e(l-1) = (0.0d0,0.0d0) >*/
    i__1 = l - 1;
    e[i__1].r = 0., e[i__1].i = 0.;
/*<             do 570 k = l, m >*/
    i__1 = m;
    for (k = l; k <= i__1; ++k) {
/*<                t1 = dreal(s(k)) >*/
        i__2 = k;
        t1 = s[i__2].r;
/*<                call drotg(t1,f,cs,sn) >*/
        drotg_(&t1, &f, &cs, &sn);
/*<                s(k) = dcmplx(t1,0.0d0) >*/
        i__2 = k;
        z__1.r = t1, z__1.i = 0.;
        s[i__2].r = z__1.r, s[i__2].i = z__1.i;
/*<                f = -sn*dreal(e(k)) >*/
        i__2 = k;
        f = -sn * e[i__2].r;
/*<                e(k) = cs*e(k) >*/
        i__2 = k;
        i__3 = k;
        z__1.r = cs * e[i__3].r, z__1.i = cs * e[i__3].i;
        e[i__2].r = z__1.r, e[i__2].i = z__1.i;
/*<                if (wantu) call zdrot(n,u(1,k),1,u(1,l-1),1,cs,sn) >*/
        if (wantu) {
            zdrot_(n, &u[k * u_dim1 + 1], &c__1, &u[(l - 1) * u_dim1 + 1], &
                    c__1, &cs, &sn);
        }
/*<   570       continue >*/
/* L570: */
    }
/*<          go to 650 >*/
    goto L650;

/*        perform one qr step. */

/*<   580    continue >*/
L580:

/*           calculate the shift. */

/*<    >*/
/* Computing MAX */
    d__1 = z_abs(&s[m]), d__2 = z_abs(&s[m - 1]), d__1 = max(d__1,d__2), d__2 
            = z_abs(&e[m - 1]), d__1 = max(d__1,d__2), d__2 = z_abs(&s[l]), 
            d__1 = max(d__1,d__2), d__2 = z_abs(&e[l]);
    scale = max(d__1,d__2);
/*<             sm = dreal(s(m))/scale >*/
    i__1 = m;
    sm = s[i__1].r / scale;
/*<             smm1 = dreal(s(m-1))/scale >*/
    i__1 = m - 1;
    smm1 = s[i__1].r / scale;
/*<             emm1 = dreal(e(m-1))/scale >*/
    i__1 = m - 1;
    emm1 = e[i__1].r / scale;
/*<             sl = dreal(s(l))/scale >*/
    i__1 = l;
    sl = s[i__1].r / scale;
/*<             el = dreal(e(l))/scale >*/
    i__1 = l;
    el = e[i__1].r / scale;
/*<             b = ((smm1 + sm)*(smm1 - sm) + emm1**2)/2.0d0 >*/
/* Computing 2nd power */
    d__1 = emm1;
    b = ((smm1 + sm) * (smm1 - sm) + d__1 * d__1) / 2.;
/*<             c = (sm*emm1)**2 >*/
/* Computing 2nd power */
    d__1 = sm * emm1;
    c__ = d__1 * d__1;
/*<             shift = 0.0d0 >*/
    shift = 0.;
/*<             if (b .eq. 0.0d0 .and. c .eq. 0.0d0) go to 590 >*/
    if (b == 0. && c__ == 0.) {
        goto L590;
    }
/*<                shift = dsqrt(b**2+c) >*/
/* Computing 2nd power */
    d__1 = b;
    shift = sqrt(d__1 * d__1 + c__);
/*<                if (b .lt. 0.0d0) shift = -shift >*/
    if (b < 0.) {
        shift = -shift;
    }
/*<                shift = c/(b + shift) >*/
    shift = c__ / (b + shift);
/*<   590       continue >*/
L590:
/*<             f = (sl + sm)*(sl - sm) + shift >*/
    f = (sl + sm) * (sl - sm) + shift;
/*<             g = sl*el >*/
    g = sl * el;

/*           chase zeros. */

/*<             mm1 = m - 1 >*/
    mm1 = m - 1;
/*<             do 600 k = l, mm1 >*/
    i__1 = mm1;
    for (k = l; k <= i__1; ++k) {
/*<                call drotg(f,g,cs,sn) >*/
        drotg_(&f, &g, &cs, &sn);
/*<                if (k .ne. l) e(k-1) = dcmplx(f,0.0d0) >*/
        if (k != l) {
            i__2 = k - 1;
            z__1.r = f, z__1.i = 0.;
            e[i__2].r = z__1.r, e[i__2].i = z__1.i;
        }
/*<                f = cs*dreal(s(k)) + sn*dreal(e(k)) >*/
        i__2 = k;
        i__3 = k;
        f = cs * s[i__2].r + sn * e[i__3].r;
/*<                e(k) = cs*e(k) - sn*s(k) >*/
        i__2 = k;
        i__3 = k;
        z__2.r = cs * e[i__3].r, z__2.i = cs * e[i__3].i;
        i__4 = k;
        z__3.r = sn * s[i__4].r, z__3.i = sn * s[i__4].i;
        z__1.r = z__2.r - z__3.r, z__1.i = z__2.i - z__3.i;
        e[i__2].r = z__1.r, e[i__2].i = z__1.i;
/*<                g = sn*dreal(s(k+1)) >*/
        i__2 = k + 1;
        g = sn * s[i__2].r;
/*<                s(k+1) = cs*s(k+1) >*/
        i__2 = k + 1;
        i__3 = k + 1;
        z__1.r = cs * s[i__3].r, z__1.i = cs * s[i__3].i;
        s[i__2].r = z__1.r, s[i__2].i = z__1.i;
/*<                if (wantv) call zdrot(p,v(1,k),1,v(1,k+1),1,cs,sn) >*/
        if (wantv) {
            zdrot_(p, &v[k * v_dim1 + 1], &c__1, &v[(k + 1) * v_dim1 + 1], &
                    c__1, &cs, &sn);
        }
/*<                call drotg(f,g,cs,sn) >*/
        drotg_(&f, &g, &cs, &sn);
/*<                s(k) = dcmplx(f,0.0d0) >*/
        i__2 = k;
        z__1.r = f, z__1.i = 0.;
        s[i__2].r = z__1.r, s[i__2].i = z__1.i;
/*<                f = cs*dreal(e(k)) + sn*dreal(s(k+1)) >*/
        i__2 = k;
        i__3 = k + 1;
        f = cs * e[i__2].r + sn * s[i__3].r;
/*<                s(k+1) = -sn*e(k) + cs*s(k+1) >*/
        i__2 = k + 1;
        d__1 = -sn;
        i__3 = k;
        z__2.r = d__1 * e[i__3].r, z__2.i = d__1 * e[i__3].i;
        i__4 = k + 1;
        z__3.r = cs * s[i__4].r, z__3.i = cs * s[i__4].i;
        z__1.r = z__2.r + z__3.r, z__1.i = z__2.i + z__3.i;
        s[i__2].r = z__1.r, s[i__2].i = z__1.i;
/*<                g = sn*dreal(e(k+1)) >*/
        i__2 = k + 1;
        g = sn * e[i__2].r;
/*<                e(k+1) = cs*e(k+1) >*/
        i__2 = k + 1;
        i__3 = k + 1;
        z__1.r = cs * e[i__3].r, z__1.i = cs * e[i__3].i;
        e[i__2].r = z__1.r, e[i__2].i = z__1.i;
/*<    >*/
        if (wantu && k < *n) {
            zdrot_(n, &u[k * u_dim1 + 1], &c__1, &u[(k + 1) * u_dim1 + 1], &
                    c__1, &cs, &sn);
        }
/*<   600       continue >*/
/* L600: */
    }
/*<             e(m-1) = dcmplx(f,0.0d0) >*/
    i__1 = m - 1;
    z__1.r = f, z__1.i = 0.;
    e[i__1].r = z__1.r, e[i__1].i = z__1.i;
/*<             iter = iter + 1 >*/
    ++iter;
/*<          go to 650 >*/
    goto L650;

/*        convergence. */

/*<   610    continue >*/
L610:

/*           make the singular value  positive */

/*<             if (dreal(s(l)) .ge. 0.0d0) go to 620 >*/
    i__1 = l;
    if (s[i__1].r >= 0.) {
        goto L620;
    }
/*<                s(l) = -s(l) >*/
    i__1 = l;
    i__2 = l;
    z__1.r = -s[i__2].r, z__1.i = -s[i__2].i;
    s[i__1].r = z__1.r, s[i__1].i = z__1.i;
/*<                if (wantv) call zscal(p,(-1.0d0,0.0d0),v(1,l),1) >*/
    if (wantv) {
        zscal_(p, &c_b60, &v[l * v_dim1 + 1], &c__1);
    }
/*<   620       continue >*/
L620:

/*           order the singular value. */

/*<   630       if (l .eq. mm) go to 640 >*/
L630:
    if (l == mm) {
        goto L640;
    }
/*           ...exit */
/*<                if (dreal(s(l)) .ge. dreal(s(l+1))) go to 640 >*/
    i__1 = l;
    i__2 = l + 1;
    if (s[i__1].r >= s[i__2].r) {
        goto L640;
    }
/*<                t = s(l) >*/
    i__1 = l;
    t.r = s[i__1].r, t.i = s[i__1].i;
/*<                s(l) = s(l+1) >*/
    i__1 = l;
    i__2 = l + 1;
    s[i__1].r = s[i__2].r, s[i__1].i = s[i__2].i;
/*<                s(l+1) = t >*/
    i__1 = l + 1;
    s[i__1].r = t.r, s[i__1].i = t.i;
/*<    >*/
    if (wantv && l < *p) {
        zswap_(p, &v[l * v_dim1 + 1], &c__1, &v[(l + 1) * v_dim1 + 1], &c__1);
    }
/*<    >*/
    if (wantu && l < *n) {
        zswap_(n, &u[l * u_dim1 + 1], &c__1, &u[(l + 1) * u_dim1 + 1], &c__1);
    }
/*<                l = l + 1 >*/
    ++l;
/*<             go to 630 >*/
    goto L630;
/*<   640       continue >*/
L640:
/*<             iter = 0 >*/
    iter = 0;
/*<             m = m - 1 >*/
    --m;
/*<   650    continue >*/
L650:
/*<       go to 400 >*/
    goto L400;
/*<   660 continue >*/
L660:
/*<       return >*/
    return 0;
/*<       end >*/
} /* zsvdc_ */

#ifdef __cplusplus
        }
#endif
