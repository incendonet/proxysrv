/*
 * g729codec.h
 *
 * H.323 interface for G.729A codec
 *
 * Open H323 Library
 *
 * Copyright (c) 2001 Equivalence Pty. Ltd.
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is Open H323 Library.
 *
 * The Initial Developer of the Original Code is Equivalence Pty. Ltd.
 *
 * Portions of this code were written with the assisance of funding from
 * Vovida Networks, Inc. http://www.vovida.com.
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: g729codec.h,v $
 * Revision 1.1  2006/08/04 03:33:18  joegenbaclor
 * Moved pwlib and OPAL headers to root include directory
 *
 * Revision 1.1  2006/06/26 03:02:26  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 2.9  2005/08/28 07:59:17  rjongbloed
 * Converted OpalTranscoder to use factory, requiring sme changes in making sure
 *   OpalMediaFormat instances are initialised before use.
 *
 * Revision 2.8  2004/02/23 01:32:06  rjongbloed
 * Removed duplicate #if for if voice age included and made sure there was a
 *   dummy G729 declaration macro if no G.729 included.
 *
 * Revision 2.7  2004/02/19 10:46:43  rjongbloed
 * Merged OpenH323 version 1.13.1 changes.
 *
 * Revision 2.6  2003/06/02 04:04:54  rjongbloed
 * Changed to use new autoconf system
 *
 * Revision 2.5  2003/01/07 04:39:52  robertj
 * Updated to OpenH323 v1.11.2
 *
 * Revision 2.4  2002/11/10 11:33:16  robertj
 * Updated to OpenH323 v1.10.3
 *
 * Revision 2.3  2002/09/16 02:52:33  robertj
 * Added #define so can select if #pragma interface/implementation is used on
 *   platform basis (eg MacOS) rather than compiler, thanks Robert Monaghan.
 *
 * Revision 2.2  2002/09/04 06:01:46  robertj
 * Updated to OpenH323 v1.9.6
 *
 * Revision 2.1  2002/07/01 04:56:29  robertj
 * Updated to OpenH323 v1.9.1
 *
 * Revision 1.7  2003/05/05 11:59:21  robertj
 * Changed to use autoconf style selection of options and subsystems.
 *
 * Revision 1.6  2002/11/12 00:06:10  robertj
 * Added check for Voice Age G.729 only being able to do a single instance
 *   of the encoder and decoder. Now fails the second isntance isntead of
 *   interfering with the first one.
 *
 * Revision 1.5  2002/09/16 01:14:15  robertj
 * Added #define so can select if #pragma interface/implementation is used on
 *   platform basis (eg MacOS) rather than compiler, thanks Robert Monaghan.
 *
 * Revision 1.4  2002/09/03 06:19:36  robertj
 * Normalised the multi-include header prevention ifdef/define symbol.
 *
 * Revision 1.3  2002/08/05 10:03:47  robertj
 * Cosmetic changes to normalise the usage of pragma interface/implementation.
 *
 * Revision 1.2  2002/06/27 03:08:31  robertj
 * Added G.729 capabilitity support even though is really G.729A.
 * Added code to include G.729 codecs on static linking.
 *
 * Revision 1.1  2001/09/21 02:54:47  robertj
 * Added new codec framework with no actual implementation.
 *
 */

#ifndef __OPAL_G729CODEC_H
#define __OPAL_G729CODEC_H

#ifdef P_USE_PRAGMA
#pragma interface
#endif

#include <opal/buildopts.h>

#if VOICE_AGE_G729A

#include <opal/transcoders.h>


///////////////////////////////////////////////////////////////////////////////

class Opal_G729_PCM : public OpalFramedTranscoder {
  public:
    Opal_G729_PCM();
    ~Opal_G729_PCM();
    virtual BOOL ConvertFrame(const BYTE * src, BYTE * dst);
};

typedef Opal_G729_PCM Opal_G729A_PCM;


///////////////////////////////////////////////////////////////////////////////

class Opal_PCM_G729 : public OpalFramedTranscoder {
  public:
    Opal_PCM_G729();
    ~Opal_PCM_G729();
    virtual BOOL ConvertFrame(const BYTE * src, BYTE * dst);
};

typedef Opal_PCM_G729 Opal_PCM_G729A;


///////////////////////////////////////////////////////////////////////////////

#define OPAL_REGISTER_G729() \
          OPAL_REGISTER_TRANSCODER(Opal_G729_PCM,  OpalG729,  OpalPCM16); \
          OPAL_REGISTER_TRANSCODER(Opal_PCM_G729,  OpalPCM16, OpalG729); \
          OPAL_REGISTER_TRANSCODER(Opal_G729A_PCM, OpalG729A, OpalPCM16); \
          OPAL_REGISTER_TRANSCODER(Opal_PCM_G729A, OpalPCM16, OpalG729A)


#else // VOICE_AGE_G729A

#define OPAL_REGISTER_G729()

#endif // VOICE_AGE_G729A

#endif // __OPAL_G729CODEC_H


/////////////////////////////////////////////////////////////////////////////
