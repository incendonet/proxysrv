#ifndef G7231CODEC_H
#define G7231CODEC_H

#ifdef P_USE_PRAGMA
#pragma interface
#endif

#include <opal/buildopts.h>

/*
 *
 * g7231codec.h
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
 * $Log: g7231codec.h,v $
 * Revision 1.2  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/09/04 13:57:59  joegenbaclor
 * *** empty log message ***
 *
 *
 */

#if ENABLE_MYACM

#include <opal/transcoders.h>


///////////////////////////////////////////////////////////////////////////////

class Opal_G7231_PCM : public OpalFramedTranscoder {
  public:
    Opal_G7231_PCM();
    ~Opal_G7231_PCM();
    virtual BOOL ConvertFrame(const BYTE * src, BYTE * dst);
};

typedef Opal_G7231_PCM Opal_G7231_6k3_PCM;


///////////////////////////////////////////////////////////////////////////////

class Opal_PCM_G7231 : public OpalFramedTranscoder {
  public:
    Opal_PCM_G7231();
    ~Opal_PCM_G7231();
    virtual BOOL ConvertFrame(const BYTE * src, BYTE * dst);
};

typedef Opal_PCM_G7231 Opal_PCM_G7231_6k3;


///////////////////////////////////////////////////////////////////////////////

#define OPAL_REGISTER_G7231() \
          OPAL_REGISTER_TRANSCODER(Opal_G7231_PCM,  OpalG7231_5k3,  OpalPCM16); \
          OPAL_REGISTER_TRANSCODER(Opal_PCM_G7231,  OpalPCM16, OpalG7231_5k3);


#else // ENABLE_MYACM

#define OPAL_REGISTER_G7231()

#endif // ENABLE_MYACM

#endif // G7231CODEC_H
