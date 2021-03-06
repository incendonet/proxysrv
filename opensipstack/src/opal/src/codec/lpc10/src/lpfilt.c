/*

$Log: lpfilt.c,v $
Revision 1.1  2006/06/26 03:02:53  joegenbaclor
I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.

Revision 2.2  2005/08/08 03:28:42  dereksmithies
Eradicate all "/ * within comments" messages from GCC compilations.

Revision 2.1  2003/03/14 09:53:27  robertj
Updated to openH323 v1.11.7

Revision 1.1  2000/06/05 04:45:12  robertj
Added LPC-10 2400bps codec

 * Revision 1.1  1996/08/19  22:31:35  jaf
 * Initial revision
 *

*/

#ifdef P_R_O_T_O_T_Y_P_E_S
extern int lpfilt_(real *inbuf, real *lpbuf, integer *len, integer *nsamp);
#endif

/*  -- translated by f2c (version 19951025).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* *********************************************************************** */

/* 	LPFILT Version 55 
 * $Log: lpfilt.c,v $
 * Revision 1.1  2006/06/26 03:02:53  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 2.2  2005/08/08 03:28:42  dereksmithies
 * Eradicate all "/ * within comments" messages from GCC compilations.
 *
 * Revision 2.1  2003/03/14 09:53:27  robertj
 * Updated to openH323 v1.11.7
 *
 * Revision 1.1  2000/06/05 04:45:12  robertj
 * Added LPC-10 2400bps codec
 *
 * Revision 1.1  1996/08/19  22:31:35  jaf
 * Initial revision
 * */
/* Revision 1.3  1996/03/15  16:53:49  jaf */
/* Just put comment header in standard form. */

/* Revision 1.2  1996/03/12  23:58:06  jaf */
/* Comments added explaining that none of the local variables of this */
/* subroutine need to be saved from one invocation to the next. */

/* Revision 1.1  1996/02/07 14:47:44  jaf */
/* Initial revision */


/* *********************************************************************** */

/*   31 Point Equiripple FIR Low-Pass Filter */
/*     Linear phase, delay = 15 samples */

/* 	Passband:  ripple = 0.25 dB, cutoff =  800 Hz */
/* 	Stopband:  atten. =  40. dB, cutoff = 1240 Hz */

/* Inputs: */
/*  LEN    - Length of speech buffers */
/*  NSAMP  - Number of samples to filter */
/*  INBUF  - Input speech buffer */
/*           Indices len-nsamp-29 through len are read. */
/* Output: */
/*  LPBUF  - Low passed speech buffer (must be different array than INBUF) */
/*           Indices len+1-nsamp through len are written. */

/* This subroutine has no local state. */

/* Subroutine */ int lpfilt_(real *inbuf, real *lpbuf, integer *len, integer *
	nsamp)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    integer j;
    real t;

/* 	Arguments */
/* 	Parameters/constants */
/*       Local variables that need not be saved */
/*       Local state */
/*       None */
    /* Parameter adjustments */
    --lpbuf;
    --inbuf;

    /* Function Body */
    i__1 = *len;
    for (j = *len + 1 - *nsamp; j <= i__1; ++j) {
	t = (inbuf[j] + inbuf[j - 30]) * -.0097201988f;
	t += (inbuf[j - 1] + inbuf[j - 29]) * -.0105179986f;
	t += (inbuf[j - 2] + inbuf[j - 28]) * -.0083479648f;
	t += (inbuf[j - 3] + inbuf[j - 27]) * 5.860774e-4f;
	t += (inbuf[j - 4] + inbuf[j - 26]) * .0130892089f;
	t += (inbuf[j - 5] + inbuf[j - 25]) * .0217052232f;
	t += (inbuf[j - 6] + inbuf[j - 24]) * .0184161253f;
	t += (inbuf[j - 7] + inbuf[j - 23]) * 3.39723e-4f;
	t += (inbuf[j - 8] + inbuf[j - 22]) * -.0260797087f;
	t += (inbuf[j - 9] + inbuf[j - 21]) * -.0455563702f;
	t += (inbuf[j - 10] + inbuf[j - 20]) * -.040306855f;
	t += (inbuf[j - 11] + inbuf[j - 19]) * 5.029835e-4f;
	t += (inbuf[j - 12] + inbuf[j - 18]) * .0729262903f;
	t += (inbuf[j - 13] + inbuf[j - 17]) * .1572008878f;
	t += (inbuf[j - 14] + inbuf[j - 16]) * .2247288674f;
	t += inbuf[j - 15] * .250535965f;
	lpbuf[j] = t;
    }
    return 0;
} /* lpfilt_ */

