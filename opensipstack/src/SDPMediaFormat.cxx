/*
 *
 * SDPMediaFmt.cxx
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
 * $Log: SDPMediaFormat.cxx,v $
 * Revision 1.7  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/04/18 04:31:52  hartley2481
 * *** empty log message ***
 *
 * Revision 1.5  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.4  2006/03/15 16:40:46  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SDPMediaFormat.h"

using namespace SDP;





const char * const SDPMediaFmt::PayloadNames[SDPMediaFmt::LastKnownPayloadType] = 
{
  "PCMU",     //0
  "FS1016",   //1
  "G721",     //2
  "GSM",      //3
  "G7231",    //4
  "DVI4-8",   //5
  "DVI4-16",  //6
  "LPC",      //7
  "PCMA",     //8
  "G722",     //9
  "L16",      //10
  "L16",      //11
  "G723",     //12
  "CN",       //13
  "MPA",      //14
  "G728",     //15
  "DVI4-11",  //16
  "DVI4-22",  //17
  "G729",     //18
  "CiscoCN",  //19
  "",         //20
  "",         //21
  "",         //22
  "",         //23
  "",         //24
  "CelB",     //25
  "JPEG",     //26
  "",         //27
  "",         //28
  "",         //29
  "",         //30
  "H261",     //31
  "MPV",      //32
  "MP2T",     //33
  "H263"      //34
};


namespace SDP
{
  /// wrappers to commonly used audio codecs
  SDPMediaFmt const SDPG711uLaw( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::PCMU, 
    SDPMediaFmt::PayloadNames[SDPMediaFmt::PCMU], 
    SDPMediaFmt::AudioClockRate,
    20 // ms sampling rate
  );

  SDPMediaFmt const SDPG711ALaw( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::PCMA, 
    SDPMediaFmt::PayloadNames[SDPMediaFmt::PCMA], 
    SDPMediaFmt::AudioClockRate,
    20 // ms sampling rate
  );

  SDPMediaFmt const SDPLPC10( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::LPC, 
    "LPC-10", 
    SDPMediaFmt::AudioClockRate,
    20 // ms sampling rate
  );

  SDPMediaFmt const SDPG728( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::G728, 
    SDPMediaFmt::PayloadNames[SDPMediaFmt::G728], 
    SDPMediaFmt::AudioClockRate,
    10 // ms sampling rate
  );

  SDPMediaFmt const SDPG729( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::G729, 
    SDPMediaFmt::PayloadNames[SDPMediaFmt::G729], 
    SDPMediaFmt::AudioClockRate,
    10 // ms sampling rate
  );

  SDPMediaFmt const SDPG729A( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::G729, 
    "G729a", 
    SDPMediaFmt::AudioClockRate,
    10 // ms sampling rate
  );

  SDPMediaFmt const SDPG729B( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::G729, 
    "G729b", 
    SDPMediaFmt::AudioClockRate,
    10 // ms sampling rate
  );

  SDPMediaFmt const SDPG729AB( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::G729, 
    "G729ab", 
    SDPMediaFmt::AudioClockRate,
    10 // ms sampling rate
  );

  SDPMediaFmt const SDPG7231_6k3( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::G7231, 
    SDPMediaFmt::PayloadNames[SDPMediaFmt::G7231], 
    SDPMediaFmt::AudioClockRate,
    30 // ms sampling rate
  );

  SDPMediaFmt const SDPG7231_5k3( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::G7231, 
    SDPMediaFmt::PayloadNames[SDPMediaFmt::G7231], 
    SDPMediaFmt::AudioClockRate,
    30 // ms sampling rate
  );

  SDPMediaFmt const SDPGSM( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::GSM, 
    SDPMediaFmt::PayloadNames[SDPMediaFmt::GSM], 
    SDPMediaFmt::AudioClockRate,
    10 // ms sampling rate
  );

  SDPMediaFmt const SDPMS_GSM( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::GSM, 
    "MS-GSM", 
    SDPMediaFmt::AudioClockRate,
    10 // ms sampling rate
  );

  /// custom audio codecs

  SDPMediaFmt const SDPRFC2833( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::RFC2833, 
    "telephone-event", 
    SDPMediaFmt::AudioClockRate,
    0 // ms sampling rate
  );

  SDPMediaFmt const SDPMS_IMA( 
    SDPMediaFmt::Audio, 
    114, 
    "MS-IMA-ADPCM", 
    SDPMediaFmt::AudioClockRate,
    0 // ms sampling rate
  );


  
  SDPMediaFmt const SDPSpeex( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::Speex, 
    "SPEEX", 
    SDPMediaFmt::AudioClockRate,
    20 // ms sampling rate
  );

  SDPMediaFmt const SDPSpeexWide( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::Speex, 
    "SPEEX", 
    SDPMediaFmt::AudioClockRate * 2,
    20 // ms sampling rate
  );

  SDPMediaFmt const SDPSpeex5_95K( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::Speex, 
    "SpeexNarrow-5.95k", 
    SDPMediaFmt::AudioClockRate * 2,
    20 // ms sampling rate
  );

  SDPMediaFmt const SDPSpeex11K( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::Speex, 
    "SpeexNarrow-11k", 
    SDPMediaFmt::AudioClockRate * 2,
    20 // ms sampling rate
  );

  SDPMediaFmt const SDPSpeex15K( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::Speex, 
    "SpeexNarrow-15k", 
    SDPMediaFmt::AudioClockRate * 2,
    20 // ms sampling rate
  );

  SDPMediaFmt const SDPSpeex18K( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::Speex, 
    "SpeexNarrow-18.2k", 
    SDPMediaFmt::AudioClockRate * 2,
    20 // ms sampling rate
  );


  SDPMediaFmt const SDPiLBC( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::iLBC, 
    "iLBC", 
    SDPMediaFmt::AudioClockRate,
    20 // ms sampling rate
  );

  SDPMediaFmt const SDPiLBC5K2( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::iLBC, 
    "iLBC-15k2", 
    SDPMediaFmt::AudioClockRate,
    20 // ms sampling rate
  );

  SDPMediaFmt const SDPG726_16( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::G726, 
    "G726-16", 
    SDPMediaFmt::AudioClockRate,
    10 // ms sampling rate
  );

  SDPMediaFmt const SDPG726_24( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::G726, 
    "G726-24", 
    SDPMediaFmt::AudioClockRate,
    10 // ms sampling rate
  );

  SDPMediaFmt const SDPG726_32( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::G726, 
    "G726-32", 
    SDPMediaFmt::AudioClockRate,
    10 // ms sampling rate
  );

  SDPMediaFmt const SDPG726_40( 
    SDPMediaFmt::Audio, 
    SDPMediaFmt::G726, 
    "G726-40", 
    SDPMediaFmt::AudioClockRate,
    10 // ms sampling rate
  );


  /// commmonly used video compression
  SDPMediaFmt const SDPH261(  
    SDPMediaFmt::Video, 
    SDPMediaFmt::H261, 
    SDPMediaFmt::PayloadNames[SDPMediaFmt::H261], 
    SDPMediaFmt::VideoClockRate,
    0 // ms sampling rate
  );

  SDPMediaFmt const SDPH263(  
    SDPMediaFmt::Video, 
    SDPMediaFmt::H263, 
    SDPMediaFmt::PayloadNames[SDPMediaFmt::H263], 
    SDPMediaFmt::VideoClockRate,
    0 // ms sampling rate
  );

  SDPMediaFmt const SDPMPV(  
    SDPMediaFmt::Video, 
    SDPMediaFmt::MPV, 
    SDPMediaFmt::PayloadNames[SDPMediaFmt::MPV], 
    SDPMediaFmt::VideoClockRate,
    0 // ms sampling rate
  );

  // hmendoza add 20060418 sdptest error
  SDPMediaFmt const SDPPCM16(
    SDPMediaFmt::Audio,
    SDPMediaFmt::L16_Stereo,
    SDPMediaFmt::PayloadNames[SDPMediaFmt::L16_Stereo],
    SDPMediaFmt::AudioClockRate,
    0 //ms sampling rate
  );
  // hmendoza add 20060418 sdptest error

  SDPMediaFmt const SDPIllegalFormat(  
    SDPMediaFmt::Audio, 
    SDPMediaFmt::IllegalPayloadType, 
    "", 
    0,
    0 // ms sampling rate
  );

  /*
a=rtpmap:8 PCMA/8000
a=rtpmap:0 PCMU/8000
a=rtpmap:99 G726-16k/8000
a=rtpmap:98 G726-24k/8000
a=rtpmap:2 G721/8000
a=rtpmap:97 G726-40k/8000
a=rtpmap:18 G729/8000
a=rtpmap:3 GSM/8000
a=rtpmap:108 iLBC/8000
a=rtpmap:96 iLBC-15k2/8000
a=rtpmap:7 LPC/8000
a=rtpmap:100 MS-GSM/8000
a=rtpmap:114 MS-IMA-ADPCM/8000
a=rtpmap:104 SpeexNarrow-11k/8000
a=rtpmap:105 SpeexNarrow-15k/8000
a=rtpmap:106 SpeexNarrow-18.2k/8000
a=rtpmap:102 SpeexNarrow-5.95k/8000
a=rtpmap:103 SPEEX/8000
a=rtpmap:107 SPEEX/16000
m=video 5002 RTP/AVP 31
a=rtpmap:31 H261/90000
*/


  SDPMediaFmt SDPMediaFmt::CreateOpalCompatibleFormat(
    const PString & opalFormatName
  )
  {
    SDPMediaFmt format;
    if( opalFormatName *= "G.711-ALaw-64k" )
      format = SDPG711ALaw;
    else if( opalFormatName *= "G.711-uLaw-64k" )
      format = SDPG711uLaw;
    else if( opalFormatName *= "G.726-16k" )
      format = SDPG726_16;
    else if( opalFormatName *= "G.726-24k" )
      format = SDPG726_24;
    else if( opalFormatName *= "G.726-32k" )
      format = SDPG726_32;
    else if( opalFormatName *= "G.726-40k" )
      format = SDPG726_40;
    else if( opalFormatName *= "G.729" )
      format = SDPG729;
    else if( opalFormatName *= "G.729A" )
      format = SDPG729;
    else if( opalFormatName *= "GSM-06.10" )
      format = SDPGSM;
    else if( opalFormatName *= "H.261(CIF)" )
      format = SDPH261;
    else if( opalFormatName *= "H.261(QCIF)" )
      format = SDPH261;
    else if( opalFormatName *= "iLBC-13k3" )
      format = SDPiLBC;
    else if( opalFormatName *= "iLBC-15k2" )
      format = SDPiLBC5K2;
    else if( opalFormatName *= "LPC-10" )
      format = SDPLPC10;
    else if( opalFormatName *= "MS-GSM" )
      format = SDPMS_GSM;
    else if( opalFormatName *= "MS-IMA-ADPCM" )
      format = SDPMS_IMA;
    else if( opalFormatName *= "SpeexNarrow-8k" )
      format = SDPSpeex;
    else if( opalFormatName *= "SpeexNarrow-11k" )
      format = SDPSpeex11K;
    else if( opalFormatName *= "SpeexNarrow-15k" )
      format = SDPSpeex15K;
    else if( opalFormatName *= "SpeexNarrow-18.2k" )
      format = SDPSpeex18K;
    else if( opalFormatName *= "SpeexNarrow-5.95k" )
      format = SDPSpeex5_95K;
    else if( opalFormatName *= "SpeexWide-20.6k" )
      format = SDPSpeexWide;
    else
    {
      format == SDPIllegalFormat;
    }
    
    
    /*
    
    if( opalFormatName *= OSS_OPAL_G711_ULAW_64K )
    {
      format = SDPG711uLaw;
    }else if( opalFormatName *= OSS_OPAL_G711_ALAW_64K )
    {
      format = SDPG711ALaw;
    }else if( opalFormatName *= OSS_OPAL_G728 )
    {
      format = SDPG728;
    }else if( opalFormatName *= OSS_OPAL_G729 )
    {
      format = SDPG729;
    }else if( opalFormatName *= OSS_OPAL_G729A )
    {
      format = SDPG729A;
    }else if( opalFormatName *= OSS_OPAL_G729B )
    {
      format = SDPG729B;
    }else if( opalFormatName *= OSS_OPAL_G729AB )
    {
      format = SDPG729AB;
    }else if( opalFormatName *= OSS_OPAL_G7231 )
    {
      format = SDPG7231_5k3;
    }else if( opalFormatName *= OSS_OPAL_G7231_6k3 )
    {
      format = SDPG7231_6k3;
    }else if( opalFormatName *= OSS_OPAL_G7231_5k3 )
    {
      format = SDPG7231_5k3;
    }else if( opalFormatName *= OSS_OPAL_G7231A_6k3 )
    {
      format = SDPG7231_6k3;
    }else if( opalFormatName *= OSS_OPAL_G7231A_5k3 )
    {
      format = SDPG7231_5k3;
    }else if( opalFormatName *= OSS_OPAL_GSM0610 )
    {
      format = SDPGSM;
    }else if( opalFormatName *= OSS_OPAL_RFC2833 )
    {
      format = SDPRFC2833;
    }else if( opalFormatName *= OSS_OPAL_H261_QCIF )
    {
      format = SDPRFC2833;
    }else if( opalFormatName *= OSS_OPAL_H261_CIF  )
    {
      format = SDPH261;
    }else if( opalFormatName *= OSS_OPAL_H263 )
    {
      format = SDPH263;
    }else if( opalFormatName *= OSS_OPAL_H263_SQCIF )
    {
      format = SDPH263;
    }else if( opalFormatName *= OSS_OPAL_H263_QCIF )
    {
      format = SDPH263;
    }else if( opalFormatName *= OSS_OPAL_H263_CIF  )
    {
      format = SDPH263;
    }else if( opalFormatName *= OSS_OPAL_RGB24 )
    {
      format = SDPMPV;
    }else if( opalFormatName *= OSS_OPAL_RGB32 )
    {
      format = SDPMPV;
    }
    */

    return format;
  }

}

SDPMediaFmt::SDPMediaFmt()
{
  m_MediaType = 0;
  m_PayloadType = IllegalPayloadType;
}

SDPMediaFmt::SDPMediaFmt( 
  const SDPMediaFmt & format 
)
{
  m_MediaType = 0;
  m_PayloadType = IllegalPayloadType;
  operator=( format );
}

SDPMediaFmt::SDPMediaFmt(
  int mediaType,
  int payloadType,
  const PString & encodingName,
  int clockRate,
  int samplingRate
)
{
  m_MediaType = mediaType;
  m_PayloadType = payloadType;
  m_EncodingName = encodingName;
  m_ClockRate = clockRate;
  m_SamplingRate = samplingRate;
}

SDPMediaFmt::SDPMediaFmt(
  const PString & format
)
{
  operator=( format );
}

SDPMediaFmt::~SDPMediaFmt()
{
}

SDPMediaFmt & SDPMediaFmt::operator =( 
  const SDPMediaFmt & format 
)
{
  m_MediaType = format.m_MediaType;
  m_PayloadType = format.m_PayloadType;
  m_EncodingName = format.m_EncodingName;
  m_ClockRate = format.m_ClockRate;
  m_SamplingRate = format.m_SamplingRate;
  m_CustomAttributes = format.m_CustomAttributes;
  m_CustomAttributes.MakeUnique();
  return *this;
}

SDPMediaFmt & SDPMediaFmt::operator =(
  const PString & _format
)
{
  //a=rtpmap:3 GSM/8000

  PString format( _format );
  PINDEX colon = format.Find( ':' ) ;
  if( colon != P_MAX_INDEX )
  {
    format = _format.Mid( colon + 1 ); 
  }

  PStringArray tokens = format.Tokenise( ' ', FALSE );

  if( tokens.GetSize() == 2 )
  {
    if( tokens[0] *= "audio" )
      m_MediaType = Audio;
    else
      m_MediaType = Video;

    PStringArray innerTokens = tokens[1].Tokenise( '/' );
    if( innerTokens.GetSize() == 2 )
    {
      PString type, encodingName, clockRate;
      type = tokens[0];
      encodingName = innerTokens[0];
      clockRate = innerTokens[1];

      m_PayloadType = type.AsInteger();
      m_EncodingName = encodingName;
      m_ClockRate = (WORD)clockRate.AsUnsigned();
    }
  }

  return *this;
}

SDPMediaFmt::operator PString()
{
  return AsString();
}

void SDPMediaFmt::PrintOn( ostream & strm )const
{
  if( m_MediaType == Audio )
    strm << "audio ";
  else
    strm << "video ";

  strm << m_PayloadType << ' ' << m_EncodingName << '/' << m_ClockRate; 
}

PString SDPMediaFmt::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

BOOL SDPMediaFmt::operator==(
    const SDPMediaFmt & format 
)
{
  return  m_MediaType == format.m_MediaType &&
          m_PayloadType == format.m_PayloadType &&
          m_EncodingName == format.m_EncodingName;
}

PObject * SDPMediaFmt::Clone()const
{
  return new SDPMediaFmt( *this );
}

/////////////////////////////////////////////////////////////////////////////

  


