/*
 *
 * g7231codec.cxx
 * 
 * Open SIP Stack ( OSS )
 *
 ***************************************************************
 * Note: Most of the codes here have been based or copied from 
 * sipcon.cxx from the OPAL Library
 * Special thanks to the OPAL Team for their wonderful work!
 ***************************************************************
 *
 * Copyright (c) Joegen E. Baclor.
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either expressed or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is OpenSIPStack.
 *
 * The Initial Developer of the Original Code is opensipstack.org.
 *
 * The author of this code is Joegen E. Baclor
 *
 * $Log: g7231codec.cxx,v $
 * Revision 1.3  2006/12/18 10:31:35  joegenbaclor
 * Fixed ACM G.723.1 one way audio bug
 *
 * Revision 1.2  2006/11/30 08:19:39  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/09/04 13:58:00  joegenbaclor
 * *** empty log message ***
 *
 *
 */

#include <ptlib.h>

#ifdef __GNUC__
#pragma implementation "g7231codec.h"
#endif

#include <codec/g7231codec.h>


#if ENABLE_MYACM

extern "C" {
#include "myacmlib.h"
};


#if defined(_MSC_VER)
#pragma comment(lib, MYACM_LIBRARY)
#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#endif

#define new PNEW


#define H323_NAME_MS "Microsoft ACM G.723.1{sw}"	// Microsoft ACM codec
static BOOL isAcmEncoder = FALSE; 
static BOOL isAcmDecoder = FALSE; 

static unsigned const FrameSizes[4] = { 24, 20, 4, 1 };
static unsigned const FrameSamples = 240;

/////////////////////////////////////////////////////////////////////////////

Opal_G7231_PCM::Opal_G7231_PCM()
  : OpalFramedTranscoder(OpalG7231_5k3, OpalPCM16, 24, FrameSamples*2)
{
  isAcmDecoder = acm_init_decoder_g7321();

  if( isAcmDecoder )
    PTRACE(1, "Codec\tG.731 decoder created");
  else
    PTRACE(1, "Codec\tG.731 decoder creation failed");
}


Opal_G7231_PCM::~Opal_G7231_PCM()
{
  acm_close_decoder();
	isAcmDecoder = FALSE;
}


BOOL Opal_G7231_PCM::ConvertFrame(const BYTE * src, BYTE * dst)
{
  int length = FrameSizes[*src&3];
  unsigned int written = 0;

  if(!isAcmDecoder )
    return FALSE;
  
  acm_decoder((unsigned char *)src, length, (short*)dst, written);

  return TRUE;
}


Opal_PCM_G7231::Opal_PCM_G7231()
  : OpalFramedTranscoder(OpalPCM16, OpalG7231_5k3, FrameSamples*2, 24)
{
  isAcmEncoder = acm_init_encoder_g7321();

  if( isAcmEncoder )
    PTRACE(1, "Codec\tG.731 encoder created");
  else
    PTRACE(1, "Codec\tG.731 encoder creation failed");
}


Opal_PCM_G7231::~Opal_PCM_G7231()
{
  acm_close_encoder();
	isAcmEncoder = FALSE;
}


BOOL Opal_PCM_G7231::ConvertFrame(const BYTE * src, BYTE * dst)
{
  if(!isAcmEncoder)
    return FALSE;	

  unsigned int length = 240 * 2;
  unsigned int consumed = 0;

  acm_encoder((short *)src, length, (unsigned char *)dst, consumed );

  return TRUE;
}


#endif // 

