/*
 *
 * SDPMediaFormat.h
 * 
 * Open SIP Stack ( OSS )
 *
 * Copyright (C) 2006 Joegen E. Baclor. All Rights Reserved.
 *
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is OpenSIPStack Library.
 *
 * The Initial Developer of the Original Code is Joegen E. Baclor.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * $Log: SDPMediaFormat.h,v $
 * Revision 1.8  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.7  2006/04/18 04:31:52  hartley2481
 * *** empty log message ***
 *
 * Revision 1.6  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.5  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.4  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SDPMEDIAFORMAT_H
#define SDPMEDIAFORMAT_H

#include <ptlib.h>

namespace SDP
{
  class SDPMediaFmt : public PObject
  {
    PCLASSINFO( SDPMediaFmt, PObject );
  public:

    enum Type 
    {
      PCMU,         // G.711 u-Law
      FS1016,       // Federal Standard 1016 CELP
      G721,         // ADPCM - Subsumed by G.726
      G726 = G721,
      GSM,          // GSM 06.10
      G7231,        // G.723.1 at 6.3kbps or 5.3 kbps
      DVI4_8k,      // DVI4 at 8kHz sample rate
      DVI4_16k,     // DVI4 at 16kHz sample rate
      LPC,          // LPC-10 Linear Predictive CELP
      PCMA,         // G.711 A-Law
      G722,         // G.722
      L16_Stereo,   // 16 bit linear PCM
      L16_Mono,     // 16 bit linear PCM
      G723,         // G.723
      CN,           // Confort Noise
      MPA,          // MPEG1 or MPEG2 audio
      G728,         // G.728 16kbps CELP
      DVI4_11k,     // DVI4 at 11kHz sample rate
      DVI4_22k,     // DVI4 at 22kHz sample rate
      G729,         // G.729 8kbps
      Cisco_CN,     // Cisco systems comfort noise (unofficial)
      CelB = 25,    // Sun Systems Cell-B video
      JPEG,         // Motion JPEG
      H261 = 31,    // H.261
      MPV,          // MPEG1 or MPEG2 video
      MP2T,         // MPEG2 transport system
      H263,         // H.263
      LastKnownPayloadType,
      DynamicBase = 96,
      Speex = 97,
      iLBC = 98,
      RFC2833 = 101,
      MaxPayloadType = 127,
      IllegalPayloadType
    };

    enum StandardClockRate {
      AudioClockRate = 8000,  // 8kHz sample rate
      VideoClockRate = 90000  // 90kHz sample rate
    };

    enum StandardMediaType
    {
      Audio = 0,
      Video = 1
    };

    static const char * const PayloadNames[LastKnownPayloadType];
    
    static SDPMediaFmt CreateOpalCompatibleFormat(
      const PString & opalFormatName
    );

    SDPMediaFmt();
    
    SDPMediaFmt( 
      const SDPMediaFmt & format 
    );

    SDPMediaFmt(
      int mediaType,
      int payloadType,
      const PString & encodingName,
      int clockRate,
      int samplingRate
    );

    SDPMediaFmt(
      const PString & format
    );

    virtual ~SDPMediaFmt();

    SDPMediaFmt & operator =( 
      const SDPMediaFmt & format 
    );
    SDPMediaFmt & operator =(
      const PString & format
    );

    BOOL operator==(
       const SDPMediaFmt & format 
    );

    virtual PObject * Clone()const;


    operator PString();

    virtual void PrintOn( ostream & strm )const;

    PString AsString()const;

    PINLINE int GetPayloadType()const{ return m_PayloadType; };
    PINLINE int GetMediaType()const{ return m_MediaType; };
    PINLINE const PString & GetEncodingName()const{ return m_EncodingName; };
    PINLINE int GetClockRate()const{ return m_ClockRate; };
    PINLINE int GetSamplingRate()const{ return m_SamplingRate; };
    PINLINE void SetPayloadType( int type ){ m_PayloadType = type; };
    PINLINE void SetEncodingName( const PString & encodingName ){ m_EncodingName = encodingName; };
    PINLINE void SetClockRate( int clockRate ){ m_ClockRate = clockRate; };
    PINLINE void SetSamplingRate( int samplingRate ){ m_SamplingRate = samplingRate; };
    PINLINE const PStringArray & GetCustomAttributes()const{ return m_CustomAttributes; };
    PINLINE void AppendCustomAttribute( const PString & attribute ){ m_CustomAttributes.AppendString( attribute ); };

    PLIST( Collection, SDPMediaFmt );
    PDICTIONARY( SortedCollection, PCaselessString, SDPMediaFmt );

  protected:

    int m_MediaType;
    int m_PayloadType;
    PString  m_EncodingName;
    int m_ClockRate;
    int m_SamplingRate;
    PStringArray m_CustomAttributes;

  };

  /// OPAL Media Format Compatible names
  #define OSS_OPAL_PCM16          "PCM-16"
  #define OSS_OPAL_L16_MONO_8KHZ  "Linear-16-Mono-8kHz"
  #define OSS_OPAL_L16_MONO_16KHZ "Linear-16-Mono-16kHz"
  #define OSS_OPAL_G711_ULAW_64K  "G.711-uLaw-64k"
  #define OSS_OPAL_G711_ALAW_64K  "G.711-ALaw-64k"
  #define OSS_OPAL_G728           "G.728"
  #define OSS_OPAL_G729           "G.729"
  #define OSS_OPAL_G729A          "G.729A"
  #define OSS_OPAL_G729B          "G.729B"
  #define OSS_OPAL_G729AB         "G.729A/B"
  #define OSS_OPAL_G7231          "G.723.1"
  #define OSS_OPAL_G7231_6k3      OSS_OPAL_G7231
  #define OSS_OPAL_G7231_5k3      "G.723.1(5.3k)"
  #define OSS_OPAL_G7231A_6k3     "G.723.1A(6.3k)"
  #define OSS_OPAL_G7231A_5k3     "G.723.1A(5.3k)"
  #define OSS_OPAL_GSM0610        "GSM-06.10"
  #define OSS_OPAL_RFC2833        "UserInput/RFC2833"

  #define OSS_OPAL_H261_QCIF "H.261(QCIF)"
  #define OSS_OPAL_H261_CIF  "H.261(CIF)"
  #define OSS_OPAL_H263 "H.263"
  #define OSS_OPAL_H263_SQCIF "H.263(SQCIF)"
  #define OSS_OPAL_H263_QCIF "H.263(QCIF)"
  #define OSS_OPAL_H263_CIF  "H.263(CIF)"
  #define OSS_OPAL_RGB24   "RGB24"
  #define OSS_OPAL_RGB32   "RGB32"

  extern SDPMediaFmt const SDPG711uLaw; 
  extern SDPMediaFmt const SDPG711ALaw; 
  extern SDPMediaFmt const SDPLPC10; 
  extern SDPMediaFmt const SDPG728; 
  extern SDPMediaFmt const SDPG729; 
  extern SDPMediaFmt const SDPG729A; 
  extern SDPMediaFmt const SDPG729B; 
  extern SDPMediaFmt const SDPG729AB; 
  extern SDPMediaFmt const SDPG7231_6k3; 
  extern SDPMediaFmt const SDPG7231_5k3; 
  extern SDPMediaFmt const SDPGSM; 
  extern SDPMediaFmt const SDPMS_GSM; 
  extern SDPMediaFmt const SDPRFC2833; 
  extern SDPMediaFmt const SDPMS_IMA; 
  extern SDPMediaFmt const SDPSpeex; 
  extern SDPMediaFmt const SDPSpeexWide; 
  extern SDPMediaFmt const SDPSpeex5_95K; 
  extern SDPMediaFmt const SDPSpeex11K; 
  extern SDPMediaFmt const SDPSpeex15K; 
  extern SDPMediaFmt const SDPSpeex18K; 
  extern SDPMediaFmt const SDPiLBC; 
  extern SDPMediaFmt const SDPiLBC5K2; 
  extern SDPMediaFmt const SDPG726_16; 
  extern SDPMediaFmt const SDPG726_24; 
  extern SDPMediaFmt const SDPG726_32; 
  extern SDPMediaFmt const SDPG726_40; 
  extern SDPMediaFmt const SDPH261;  
  extern SDPMediaFmt const SDPH263;  
  extern SDPMediaFmt const SDPMPV;  
  extern SDPMediaFmt const SDPIllegalFormat;  
  //hmendoza add 20060418 error in sdptest
  extern SDPMediaFmt const SDPPCM16; 
  //hmendoza add 20060418 error in sdptest

  #define SDPG7231 SDPG7231_6k3
};

#endif


