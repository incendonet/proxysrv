/*

$Log: irc2pc.c,v $
Revision 1.1  2006/06/26 03:02:53  joegenbaclor
I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.

Revision 2.2  2005/08/08 03:28:42  dereksmithies
Eradicate all "/ * within comments" messages from GCC compilations.

Revision 2.1  2003/03/14 09:53:27  robertj
Updated to openH323 v1.11.7

Revision 1.2  2002/02/15 03:57:55  yurik
Warnings removed during compilation, patch courtesy of Jehan Bing, jehan@bravobrava.com

Revision 1.1  2000/06/05 04:45:12  robertj
Added LPC-10 2400bps codec

 * Revision 1.1  1996/08/19  22:31:56  jaf
 * Initial revision
 *

*/

#ifdef P_R_O_T_O_T_Y_P_E_S
extern int irc2pc_(real *rc, real *pc, integer *order, real *gprime, real *g2pass);
#endif

/*  -- translated by f2c (version 19951025).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* ***************************************************************** */

/* 	IRC2PC Version 48 */

/*
 * $Log: irc2pc.c,v $
 * Revision 1.1  2006/06/26 03:02:53  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 2.2  2005/08/08 03:28:42  dereksmithies
 * Eradicate all "/ * within comments" messages from GCC compilations.
 *
 * Revision 2.1  2003/03/14 09:53:27  robertj
 * Updated to openH323 v1.11.7
 *
 * Revision 1.2  2002/02/15 03:57:55  yurik
 * Warnings removed during compilation, patch courtesy of Jehan Bing, jehan@bravobrava.com
 *
 * Revision 1.1  2000/06/05 04:45:12  robertj
 * Added LPC-10 2400bps codec
 *
 * Revision 1.1  1996/08/19  22:31:56  jaf
 * Initial revision
 * */
/* Revision 1.3  1996/03/20  15:47:19  jaf */
/* Added comments about which indices of array arguments are read or */
/* written. */

/* Revision 1.2  1996/03/14  16:59:04  jaf */
/* Comments added explaining that none of the local variables of this */
/* subroutine need to be saved from one invocation to the next. */

/* Revision 1.1  1996/02/07 14:47:27  jaf */
/* Initial revision */


/* ***************************************************************** */

/*   Convert Reflection Coefficients to Predictor Coeficients */

/* Inputs: */
/*  RC     - Reflection coefficients */
/*           Indices 1 through ORDER read. */
/*  ORDER  - Number of RC's */
/*  GPRIME - Excitation modification gain */
/* Outputs: */
/*  PC     - Predictor coefficients */
/*           Indices 1 through ORDER written. */
/*           Indices 1 through ORDER-1 are read after being written. */
/*  G2PASS - Excitation modification sharpening factor */

/* This subroutine has no local state. */

/* Subroutine */ int irc2pc_(real *rc, real *pc, integer *order, real *gprime,
	 real *g2pass)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    real temp[10];
    integer i__, j;

/* 	Arguments 
 * $Log: irc2pc.c,v $
 * Revision 1.1  2006/06/26 03:02:53  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 2.2  2005/08/08 03:28:42  dereksmithies
 * Eradicate all "/ * within comments" messages from GCC compilations.
 *
 * Revision 2.1  2003/03/14 09:53:27  robertj
 * Updated to openH323 v1.11.7
 *
 * Revision 1.2  2002/02/15 03:57:55  yurik
 * Warnings removed during compilation, patch courtesy of Jehan Bing, jehan@bravobrava.com
 *
 * Revision 1.1  2000/06/05 04:45:12  robertj
 * Added LPC-10 2400bps codec
 *
 * Revision 1.1  1996/08/19  22:31:56  jaf
 * Initial revision
 * */
/* Revision 1.3  1996/03/29  22:03:47  jaf */
/* Removed definitions for any constants that were no longer used. */

/* Revision 1.2  1996/03/26  19:34:33  jaf */
/* Added comments indicating which constants are not needed in an */
/* application that uses the LPC-10 coder. */

/* Revision 1.1  1996/02/07  14:43:51  jaf */
/* Initial revision */

/*   LPC Configuration parameters: */
/* Frame size, Prediction order, Pitch period */
/*       Local variables that need not be saved */
    /* Parameter adjustments */
    --pc;
    --rc;

    /* Function Body */
    *g2pass = 1.f;
    i__1 = *order;
    for (i__ = 1; i__ <= i__1; ++i__) {
	*g2pass *= 1.f - rc[i__] * rc[i__];
    }
    *g2pass = (real)(*gprime * sqrt(*g2pass));
    pc[1] = rc[1];
    i__1 = *order;
    for (i__ = 2; i__ <= i__1; ++i__) {
	i__2 = i__ - 1;
	for (j = 1; j <= i__2; ++j) {
	    temp[j - 1] = pc[j] - rc[i__] * pc[i__ - j];
	}
	i__2 = i__ - 1;
	for (j = 1; j <= i__2; ++j) {
	    pc[j] = temp[j - 1];
	}
	pc[i__] = rc[i__];
    }
    return 0;
} /* irc2pc_ */

