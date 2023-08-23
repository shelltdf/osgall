/* arpack/dseigt.f -- translated by f2c (version 20090411).
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

/* Common Block Declarations */

/*Extern struct { */
/*  integer logfil, ndigit, mgetv0, msaupd, msaup2, msaitr, mseigt, msapps, */
/*          msgets, mseupd, mnaupd, mnaup2, mnaitr, mneigh, mnapps, mngets, */
/*          mneupd, mcaupd, mcaup2, mcaitr, mceigh, mcapps, mcgets, mceupd; */
/*} debug_; */

/*#define debug_1 debug_ */

/*Extern struct { */
/*  integer nopx, nbx, nrorth, nitref, nrstrt; */
/*  real tsaupd, tsaup2, tsaitr, tseigt, tsgets, tsapps, tsconv, tnaupd, */
/*          tnaup2, tnaitr, tneigh, tngets, tnapps, tnconv, tcaupd, tcaup2, */
/*          tcaitr, tceigh, tcgets, tcapps, tcconv, tmvopx, tmvbx, tgetv0, */
/*          titref, trvec; */
/*} timing_; */

/*#define timing_1 timing_ */

/* Table of constant values */

static integer c__1 = 1;

/* ----------------------------------------------------------------------- */
/* \BeginDoc */

/* \Name: dseigt */

/* \Description: */
/*  Compute the eigenvalues of the current symmetric tridiagonal matrix */
/*  and the corresponding error bounds given the current residual norm. */

/* \Usage: */
/*  call dseigt */
/*     ( RNORM, N, H, LDH, EIG, BOUNDS, WORKL, IERR ) */

/* \Arguments */
/*  RNORM   Double precision scalar.  (INPUT) */
/*          RNORM contains the residual norm corresponding to the current */
/*          symmetric tridiagonal matrix H. */

/*  N       Integer.  (INPUT) */
/*          Size of the symmetric tridiagonal matrix H. */

/*  H       Double precision N by 2 array.  (INPUT) */
/*          H contains the symmetric tridiagonal matrix with the */
/*          subdiagonal in the first column starting at H(2,1) and the */
/*          main diagonal in second column. */

/*  LDH     Integer.  (INPUT) */
/*          Leading dimension of H exactly as declared in the calling */
/*          program. */

/*  EIG     Double precision array of length N.  (OUTPUT) */
/*          On output, EIG contains the N eigenvalues of H possibly */
/*          unsorted.  The BOUNDS arrays are returned in the */
/*          same sorted order as EIG. */

/*  BOUNDS  Double precision array of length N.  (OUTPUT) */
/*          On output, BOUNDS contains the error estimates corresponding */
/*          to the eigenvalues EIG.  This is equal to RNORM times the */
/*          last components of the eigenvectors corresponding to the */
/*          eigenvalues in EIG. */

/*  WORKL   Double precision work array of length 3*N.  (WORKSPACE) */
/*          Private (replicated) array on each PE or array allocated on */
/*          the front end. */

/*  IERR    Integer.  (OUTPUT) */
/*          Error exit flag from dstqrb. */

/* \EndDoc */

/* ----------------------------------------------------------------------- */

/* \BeginLib */

/* \Local variables: */
/*     xxxxxx  real */

/* \Routines called: */
/*     dstqrb  ARPACK routine that computes the eigenvalues and the */
/*             last components of the eigenvectors of a symmetric */
/*             and tridiagonal matrix. */
/*     second  ARPACK utility routine for timing. */
/*     dcopy   Level 1 BLAS that copies one vector to another. */

/* \Author */
/*     Danny Sorensen               Phuong Vu */
/*     Richard Lehoucq              CRPC / Rice University */
/*     Dept. of Computational &     Houston, Texas */
/*     Applied Mathematics */
/*     Rice University */
/*     Houston, Texas */

/* \Revision history: */
/*     xx/xx/92: Version ' 2.4' */

/* \SCCS Information: @(#) */
/* FILE: seigt.F   SID: 2.4   DATE OF SID: 8/27/96   RELEASE: 2 */

/* \Remarks */
/*     None */

/* \EndLib */

/* ----------------------------------------------------------------------- */

/*<        >*/
/* Subroutine */ int dseigt_(doublereal *rnorm, integer *n, doublereal *h__,
        integer *ldh, doublereal *eig, doublereal *bounds, doublereal *workl,
        integer *ierr)
{
    /* System generated locals */
    integer h_dim1, h_offset, i__1;
    doublereal d__1;

    /* Local variables */
    integer k;
/*  static real t0, t1; */
    extern /* Subroutine */ int dcopy_(integer *, doublereal *, integer *,
            doublereal *, integer *), second_(real *);
/*  integer msglvl; */
    extern /* Subroutine */ int dstqrb_(integer *, doublereal *, doublereal *,
             doublereal *, doublereal *, integer *);


/*     %----------------------------------------------------% */
/*     | Include files for debugging and timing information | */
/*     %----------------------------------------------------% */

/*<       include   'debug.h' >*/
/*<       include   'stat.h' >*/

/* \SCCS Information: @(#) */
/* FILE: debug.h   SID: 2.3   DATE OF SID: 11/16/95   RELEASE: 2 */

/*     %---------------------------------% */
/*     | See debug.doc for documentation | */
/*     %---------------------------------% */
/*<        >*/
/*<       integer    ierr, ldh, n >*/

/*     %------------------% */
/*     | Scalar Arguments | */
/*     %------------------% */

/*     %--------------------------------% */
/*     | See stat.doc for documentation | */
/*     %--------------------------------% */

/* \SCCS Information: @(#) */
/* FILE: stat.h   SID: 2.2   DATE OF SID: 11/16/95   RELEASE: 2 */

/*<       save       t0, t1, t2, t3, t4, t5 >*/

/*<       integer    nopx, nbx, nrorth, nitref, nrstrt >*/
/*<        >*/
/*<        >*/
/*<        >*/

/*     %-----------------% */
/*     | Array Arguments | */
/*     %-----------------% */

/*<        >*/

/*     %------------% */
/*     | Parameters | */
/*     %------------% */

/*<        >*/
/*<       parameter (zero = 0.0D+0) >*/

/*     %---------------% */
/*     | Local Scalars | */
/*     %---------------% */

/*<       integer    i, k, msglvl >*/

/*     %----------------------% */
/*     | External Subroutines | */
/*     %----------------------% */

/*<       external   dcopy, dstqrb, second >*/

/*     %-----------------------% */
/*     | Executable Statements | */
/*     %-----------------------% */

/*     %-------------------------------% */
/*     | Initialize timing statistics  | */
/*     | & message level for debugging | */
/*     %-------------------------------% */

/*<       call second (t0) >*/
    /* Parameter adjustments */
    --workl;
    --bounds;
    --eig;
    h_dim1 = *ldh;
    h_offset = 1 + h_dim1;
    h__ -= h_offset;

    /* Function Body */
/*  second_(&t0); */
/*<       msglvl = mseigt >*/
/*  msglvl = debug_1.mseigt; */

/*      if (msglvl .gt. 0) then */
/*         call dvout (logfil, n, h(1,2), ndigit, */
/*     &              '_seigt: main diagonal of matrix H') */
/*         if (n .gt. 1) then */
/*         call dvout (logfil, n-1, h(2,1), ndigit, */
/*     &              '_seigt: sub diagonal of matrix H') */
/*         end if */
/*      end if */

/*<       call dcopy  (n, h(1,2), 1, eig, 1) >*/
    dcopy_(n, &h__[(h_dim1 << 1) + 1], &c__1, &eig[1], &c__1);
/*<       call dcopy  (n-1, h(2,1), 1, workl, 1) >*/
    i__1 = *n - 1;
    dcopy_(&i__1, &h__[h_dim1 + 2], &c__1, &workl[1], &c__1);
/*<       call dstqrb (n, eig, workl, bounds, workl(n+1), ierr) >*/
    dstqrb_(n, &eig[1], &workl[1], &bounds[1], &workl[*n + 1], ierr);
/*<       if (ierr .ne. 0) go to 9000 >*/
    if (*ierr != 0) {
        goto L9000;
    }
/*      if (msglvl .gt. 1) then */
/*         call dvout (logfil, n, bounds, ndigit, */
/*     &              '_seigt: last row of the eigenvector matrix for H') */
/*      end if */

/*     %-----------------------------------------------% */
/*     | Finally determine the error bounds associated | */
/*     | with the n Ritz values of H.                  | */
/*     %-----------------------------------------------% */

/*<       do 30 k = 1, n >*/
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
/*<          bounds(k) = rnorm*abs(bounds(k)) >*/
        bounds[k] = *rnorm * (d__1 = bounds[k], abs(d__1));
/*<    30 continue >*/
/* L30: */
    }

/*<       call second (t1) >*/
/*  second_(&t1); */
/*<       tseigt = tseigt + (t1 - t0) >*/
/*  timing_1.tseigt += t1 - t0; */

/*<  9000 continue >*/
L9000:
/*<       return >*/
    return 0;

/*     %---------------% */
/*     | End of dseigt | */
/*     %---------------% */

/*<       end >*/
} /* dseigt_ */

#ifdef __cplusplus
        }
#endif
