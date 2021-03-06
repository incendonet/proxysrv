/*
 * speexcodec.h
 *
 * Speex codec handler
 *
 * Open H323 Library
 *
 * Copyright (c) 2002 Equivalence Pty. Ltd.
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
 * Contributor(s): ______________________________________.
 *
 * $Log: speexcodec.h,v $
 * Revision 1.1  2006/08/04 03:33:18  joegenbaclor
 * Moved pwlib and OPAL headers to root include directory
 *
 * Revision 1.1  2006/06/26 03:02:27  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 2.10  2005/12/30 14:33:12  dsandras
 * Added support for Packet Loss Concealment frames for framed codecs supporting it similarly to what was done for OpenH323.
 *
 * Revision 2.9  2005/12/28 20:01:52  dsandras
 * Reverted mode to mode 6. Get rid of encoder_frame_size.
 *
 * Revision 2.8  2005/12/24 17:52:08  dsandras
 * Fixed calls to speex_encoder_ctl so that the correct mode is used. Added support for the mode 6 wideband codec.
 *
 * Revision 2.7  2005/11/30 13:35:26  csoutheren
 * Changed tags for Doxygen
 *
 * Revision 2.6  2005/08/28 07:59:17  rjongbloed
 * Converted OpalTranscoder to use factory, requiring sme changes in making sure
 *   OpalMediaFormat instances are initialised before use.
 *
 * Revision 2.5  2005/02/21 12:19:45  rjongbloed
 * Added new "options list" to the OpalMediaFormat class.
 *
 * Revision 2.4  2004/02/19 10:46:43  rjongbloed
 * Merged OpenH323 version 1.13.1 changes.
 *
 * Revision 2.3  2004/02/15 03:12:51  rjongbloed
 * Fixed typo in symbol for Speex codec, thanks Ted Szoczei
 *
 * Revision 2.2  2002/11/10 23:20:52  robertj
 * Fixed class names in static variable macros.
 *
 * Revision 2.1  2002/11/10 11:33:16  robertj
 * Updated to OpenH323 v1.10.3
 *
 * Revision 1.16  2004/01/30 00:55:40  csoutheren
 * Removed the Xiph capability variants per Roger Hardiman as these
 * were development-only code that should not have made it into a release
 *
 * Revision 1.15  2002/12/08 22:59:41  rogerh
 * Add XiphSpeex codec. Not yet finished.
 *
 * Revision 1.11  2002/10/24 05:32:57  robertj
 * MSVC compatibility
 *
 * Revision 1.10  2002/10/22 11:54:32  rogerh
 * Fix including of speex.h
 *
 * Revision 1.9  2002/10/22 11:33:04  rogerh
 * Use the local speex.h header file
 *
 * Revision 1.8  2002/09/16 01:14:15  robertj
 * Added #define so can select if #pragma interface/implementation is used on
 *   platform basis (eg MacOS) rather than compiler, thanks Robert Monaghan.
 *
 * Revision 1.7  2002/08/22 08:30:23  craigs
 * Fixed remainder of Clone operators
 *
 * Revision 1.6  2002/08/20 15:22:50  rogerh
 * Make the codec name include the bitstream format for Speex to prevent users
 * with incompatible copies of Speex trying to use this codec.
 *
 * Revision 1.5  2002/08/15 18:35:36  rogerh
 * Fix more bugs with the Speex codec
 *
 * Revision 1.4  2002/08/14 19:34:31  rogerh
 * fix typo
 *
 * Revision 1.3  2002/08/14 04:26:20  craigs
 * Fixed ifdef problem
 *
 * Revision 1.2  2002/08/13 14:25:25  craigs
 * Added trailing newlines to avoid Linux warnings
 *
 * Revision 1.1  2002/08/13 14:14:28  craigs
 * Initial version
 *
 */

#ifndef __OPAL_SPEEXCODEC_H
#define __OPAL_SPEEXCODEC_H

#ifdef P_USE_PRAGMA
#pragma interface
#endif

#include <opal/transcoders.h>

#ifndef NO_H323
#include <h323/h323caps.h>
#endif


#define OPAL_SPEEX_NARROW_5k95 "SpeexNarrow-5.95k"
#define OPAL_SPEEX_NARROW_8k   "SpeexNarrow-8k"
#define OPAL_SPEEX_NARROW_11k  "SpeexNarrow-11k"
#define OPAL_SPEEX_NARROW_15k  "SpeexNarrow-15k"
#define OPAL_SPEEX_NARROW_18k2 "SpeexNarrow-18.2k"
#define OPAL_SPEEX_WIDE_20k6   "SpeexWide-20.6k"

extern const OpalAudioFormat & GetOpalSpeexNarrow_5k95();
extern const OpalAudioFormat & GetOpalSpeexNarrow_8k();
extern const OpalAudioFormat & GetOpalSpeexNarrow_11k();
extern const OpalAudioFormat & GetOpalSpeexNarrow_15k();
extern const OpalAudioFormat & GetOpalSpeexNarrow_18k2();
extern const OpalAudioFormat & GetOpalSpeexWide_20k6();

#define OpalSpeexNarrow_5k95 GetOpalSpeexNarrow_5k95()
#define OpalSpeexNarrow_8k   GetOpalSpeexNarrow_8k()
#define OpalSpeexNarrow_11k  GetOpalSpeexNarrow_11k()
#define OpalSpeexNarrow_15k  GetOpalSpeexNarrow_15k()
#define OpalSpeexNarrow_18k2 GetOpalSpeexNarrow_18k2()
#define OpalSpeexWide_20k6   GetOpalSpeexWide_20k6()


struct SpeexBits;


///////////////////////////////////////////////////////////////////////////////

#ifndef NO_H323

/**This class describes the Speex codec capability.
 */
class SpeexNonStandardAudioCapability : public H323NonStandardAudioCapability
{
  PCLASSINFO(SpeexNonStandardAudioCapability, H323NonStandardAudioCapability);

  public:
    SpeexNonStandardAudioCapability(int mode, 
				    int sampleRate);
};

/////////////////////////////////////////////////////////////////////////

class SpeexNarrow2AudioCapability : public SpeexNonStandardAudioCapability
{
  PCLASSINFO(SpeexNarrow2AudioCapability, SpeexNonStandardAudioCapability);

  public:
    SpeexNarrow2AudioCapability();
    PObject * Clone() const;
    PString GetFormatName() const;
};

class SpeexNarrow3AudioCapability : public SpeexNonStandardAudioCapability
{
  PCLASSINFO(SpeexNarrow3AudioCapability, SpeexNonStandardAudioCapability);

  public:
    SpeexNarrow3AudioCapability();
    PObject * Clone() const;
    PString GetFormatName() const;
};

class SpeexNarrow4AudioCapability : public SpeexNonStandardAudioCapability
{
  PCLASSINFO(SpeexNarrow4AudioCapability, SpeexNonStandardAudioCapability);

  public:
    SpeexNarrow4AudioCapability();
    PObject * Clone() const;
    PString GetFormatName() const;
};

class SpeexNarrow5AudioCapability : public SpeexNonStandardAudioCapability
{
  PCLASSINFO(SpeexNarrow5AudioCapability, SpeexNonStandardAudioCapability);

  public:
    SpeexNarrow5AudioCapability();
    PObject * Clone() const;
    PString GetFormatName() const;
};

class SpeexNarrow6AudioCapability : public SpeexNonStandardAudioCapability
{
  PCLASSINFO(SpeexNarrow6AudioCapability, SpeexNonStandardAudioCapability);

  public:
    SpeexNarrow6AudioCapability();
    PObject * Clone() const;
    PString GetFormatName() const;
};

class SpeexWide6AudioCapability : public SpeexNonStandardAudioCapability
{
  PCLASSINFO(SpeexWide6AudioCapability, SpeexNonStandardAudioCapability);

  public:
    SpeexWide6AudioCapability();
    PObject * Clone() const;
    PString GetFormatName() const;
};


#ifdef H323_STATIC_LIB
H323_STATIC_LOAD_REGISTER_CAPABILITY(SpeexNarrow2AudioCapability);
H323_STATIC_LOAD_REGISTER_CAPABILITY(SpeexNarrow3AudioCapability);
H323_STATIC_LOAD_REGISTER_CAPABILITY(SpeexNarrow4AudioCapability);
H323_STATIC_LOAD_REGISTER_CAPABILITY(SpeexNarrow5AudioCapability);
H323_STATIC_LOAD_REGISTER_CAPABILITY(SpeexNarrow6AudioCapability);
H323_STATIC_LOAD_REGISTER_CAPABILITY(SpeexWide6AudioCapability);
#endif


#define OPAL_REGISTER_SPEEX_H323 \
          H323_REGISTER_CAPABILITY(SpeexNarrow2AudioCapability, OPAL_SPEEX_NARROW_5k95) \
          H323_REGISTER_CAPABILITY(SpeexNarrow3AudioCapability, OPAL_SPEEX_NARROW_8k) \
          H323_REGISTER_CAPABILITY(SpeexNarrow4AudioCapability, OPAL_SPEEX_NARROW_11k) \
          H323_REGISTER_CAPABILITY(SpeexNarrow5AudioCapability, OPAL_SPEEX_NARROW_15k) \
          H323_REGISTER_CAPABILITY(SpeexNarrow6AudioCapability, OPAL_SPEEX_NARROW_18k2) \
          H323_REGISTER_CAPABILITY(SpeexWide6AudioCapability, OPAL_SPEEX_WIDE_20k6) 


#else // ifndef NO_H323

#define OPAL_REGISTER_SPEEX_H323

#endif // ifndef NO_H323


///////////////////////////////////////////////////////////////////////////////

class Opal_Speex_Transcoder : public OpalFramedTranscoder {
  public:
    Opal_Speex_Transcoder(
      const OpalMediaFormat & inputMediaFormat,  ///<  Input media format
      const OpalMediaFormat & outputMediaFormat, ///<  Output media format
      unsigned inputBytesPerFrame,  ///<  Number of bytes in an input frame
      unsigned outputBytesPerFrame  ///<  Number of bytes in an output frame
    );
    ~Opal_Speex_Transcoder();
  protected:
    SpeexBits * bits;
};


///////////////////////////////////////////////////////////////////////////////

class Opal_Speex_Decoder : public Opal_Speex_Transcoder {
  public:
    Opal_Speex_Decoder(
      const OpalMediaFormat & inputMediaFormat,  ///<  Input media format
      int mode,
      int sampleRate
    );
    ~Opal_Speex_Decoder();
    virtual BOOL ConvertFrame(const BYTE * src, BYTE * dst);
    virtual BOOL ConvertSilentFrame(BYTE * dst);
  protected:
    void * decoder;
    int    samples_per_frame;
};


///////////////////////////////////////////////////////////////////////////////

class Opal_Speex_Encoder : public Opal_Speex_Transcoder {
  public:
    Opal_Speex_Encoder(
      const OpalMediaFormat & outputMediaFormat, ///<  Output media format
      int mode,
      int sampleRate
    );
    ~Opal_Speex_Encoder();
    virtual BOOL ConvertFrame(const BYTE * src, BYTE * dst);
  protected:
    void   * encoder;
    int      samples_per_frame;
};


///////////////////////////////////////////////////////////////////////////////

class Opal_Speex_5k95_PCM : public Opal_Speex_Decoder {
  public:
    Opal_Speex_5k95_PCM();
};


class Opal_PCM_Speex_5k95 : public Opal_Speex_Encoder {
  public:
    Opal_PCM_Speex_5k95();
};


///////////////////////////////////////////////////////////////////////////////

class Opal_Speex_8k_PCM : public Opal_Speex_Decoder {
  public:
    Opal_Speex_8k_PCM();
};


class Opal_PCM_Speex_8k : public Opal_Speex_Encoder {
  public:
    Opal_PCM_Speex_8k();
};


///////////////////////////////////////////////////////////////////////////////

class Opal_Speex_11k_PCM : public Opal_Speex_Decoder {
  public:
    Opal_Speex_11k_PCM();
};


class Opal_PCM_Speex_11k : public Opal_Speex_Encoder {
  public:
    Opal_PCM_Speex_11k();
};


///////////////////////////////////////////////////////////////////////////////

class Opal_Speex_15k_PCM : public Opal_Speex_Decoder {
  public:
    Opal_Speex_15k_PCM();
};


class Opal_PCM_Speex_15k : public Opal_Speex_Encoder {
  public:
    Opal_PCM_Speex_15k();
};


///////////////////////////////////////////////////////////////////////////////

class Opal_Speex_18k2_PCM : public Opal_Speex_Decoder {
  public:
    Opal_Speex_18k2_PCM();
};


class Opal_PCM_Speex_18k2 : public Opal_Speex_Encoder {
  public:
    Opal_PCM_Speex_18k2();
};



///////////////////////////////////////////////////////////////////////////////

class Opal_Speex_20k6_PCM : public Opal_Speex_Decoder {
  public:
    Opal_Speex_20k6_PCM();
};


class Opal_PCM_Speex_20k6 : public Opal_Speex_Encoder {
  public:
    Opal_PCM_Speex_20k6();
};


///////////////////////////////////////////////////////////////////////////////

#define OPAL_REGISTER_SPEEX() \
          OPAL_REGISTER_SPEEX_H323 \
          OPAL_REGISTER_TRANSCODER(Opal_Speex_5k95_PCM, OpalSpeexNarrow_5k95, OpalPCM16); \
          OPAL_REGISTER_TRANSCODER(Opal_PCM_Speex_5k95, OpalPCM16,            OpalSpeexNarrow_5k95); \
          OPAL_REGISTER_TRANSCODER(Opal_Speex_8k_PCM,   OpalSpeexNarrow_8k,   OpalPCM16); \
          OPAL_REGISTER_TRANSCODER(Opal_PCM_Speex_8k,   OpalPCM16,            OpalSpeexNarrow_8k); \
          OPAL_REGISTER_TRANSCODER(Opal_Speex_11k_PCM,  OpalSpeexNarrow_11k,  OpalPCM16); \
          OPAL_REGISTER_TRANSCODER(Opal_PCM_Speex_11k,  OpalPCM16,            OpalSpeexNarrow_11k); \
          OPAL_REGISTER_TRANSCODER(Opal_Speex_15k_PCM,  OpalSpeexNarrow_15k,  OpalPCM16); \
          OPAL_REGISTER_TRANSCODER(Opal_PCM_Speex_15k,  OpalPCM16,            OpalSpeexNarrow_15k); \
          OPAL_REGISTER_TRANSCODER(Opal_Speex_18k2_PCM, OpalSpeexNarrow_18k2, OpalPCM16); \
          OPAL_REGISTER_TRANSCODER(Opal_PCM_Speex_18k2, OpalPCM16,            OpalSpeexNarrow_18k2); \
          OPAL_REGISTER_TRANSCODER(Opal_Speex_20k6_PCM, OpalSpeexWide_20k6, OpalPCM16); \
          OPAL_REGISTER_TRANSCODER(Opal_PCM_Speex_20k6, OpalPCM16,            OpalSpeexWide_20k6)



#endif // __OPAL_SPEEXCODEC_H


/////////////////////////////////////////////////////////////////////////
