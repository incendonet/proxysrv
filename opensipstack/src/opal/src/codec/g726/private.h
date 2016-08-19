/**
 *
 * $Log: private.h,v $
 * Revision 1.1  2006/06/26 03:02:37  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 1.2  2002/11/20 04:53:16  robertj
 * Included optimisations for G.711 and G.726 codecs, thanks Ted Szoczei
 *
 * Revision 1.1  2002/02/11 23:24:23  robertj
 * Updated to openH323 v1.8.0
 *
 * Revision 1.2  2002/02/10 21:14:54  dereks
 * Add cvs log history to head of the file.
 * Ensure file is terminated by a newline.
 *
 *
 *
 */
#if !defined G726_PRIVATE
#define G726_PRIVATE

int linear2ulaw(int pcm_val);
int ulaw2linear(int u_val);
int linear2alaw(int pcm_val);
int alaw2linear(int u_val);

static int fmult(int an, int srn);
int predictor_zero(	g726_state *state_ptr);
int predictor_pole(	g726_state *state_ptr);
int step_size(	g726_state *state_ptr);
int quantize(	int		d,	/* Raw difference signal sample */
	int		y,	/* Step size multiplier */
	int *	table,	/* quantization table */
	int		size);	/* table size of short integers */
int reconstruct(	int		sign,	/* 0 for non-negative value */
	int		dqln,	/* G.72x codeword */
	int		y);	/* Step size multiplier */
void update(	int		code_size,	/* distinguish 723_40 with others */
	int		y,		/* quantizer step size */
	int		wi,		/* scale factor multiplier */
	int		fi,		/* for long/short term energies */
	int		dq,		/* quantized prediction difference */
	int		sr,		/* reconstructed signal */
	int		dqsez,		/* difference from 2-pole predictor */
	g726_state *state_ptr);	/* coder state pointer */
int tandem_adjust_alaw(
	int		sr,	/* decoder output linear PCM sample */
	int		se,	/* predictor estimate sample */
	int		y,	/* quantizer step size */
	int		i,	/* decoder input code */
	int		sign,
	int *	qtab);
int tandem_adjust_ulaw(
	int		sr,	/* decoder output linear PCM sample */
	int		se,	/* predictor estimate sample */
	int		y,	/* quantizer step size */
	int		i,	/* decoder input code */
	int		sign,
	int *	qtab);

#endif // G726_PRIVATE

