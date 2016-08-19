/*
 *
 * SDPOffer.h
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
 * $Log: SDPOffer.h,v $
 * Revision 1.8  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.7  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.6  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SDPOFFER_H
#define SDPOFFER_H
#include "SDPPacket.h"
#include "SIPTransport.h"

namespace SDP
{
  using namespace SIPTransports;

  class SDPOffer : public SDPPacket
  {
    PCLASSINFO( SDPOffer, SDPPacket );
  public:

    enum AnswerState
    {
      StateAnswerAmbiguous,
      StateAnswerFinal,
      StateAnswerInvalid
    };

    SDPOffer();

    SDPOffer(
      DWORD sessionId,
      DWORD versionId,
      const PString & localAddress,
      WORD audioPort,
      WORD videoPort = 0,
      SIPTransport::Version addressType = SIPTransport::IPV4
    );

    SDPOffer(
      const PString & sdp 
    );

    SDPOffer( 
      const SDPOffer & offer 
    );

    BOOL AddFormat(
      int mediaType,
      BYTE format,
      const SDPAttribute & attribute
    );

    BOOL AddFormat(
      const SDPMediaFmt & attribute
    );

    const SDPMediaFmt * FindFormat(
      const PString & encodingName
    )const;

    SDPMediaFmt * GetFormat(
      const PString & encodingName
    );

    int ProcessAnswer(
      SDPPacket & answer,
      SDPMediaFmt::Collection & remoteAudioCaps,
      PString & remoteAudioAddress,
      WORD & remoteAudioPort,
      SDPMediaFmt::Collection & remoteVideoCaps,
      PString & remoteVideoAddress,
      WORD & remoteVideoPort
    );

    void PrintOn( 
      ostream & strm
    )const;

    SDPPacket & operator=( const PString & );

    SDPOffer & operator=( const SDPOffer & offer );

    static BOOL GetMediaFormats( 
      SDPMediaDesc & sdp,
      SDPMediaFmt::Collection & formats
    );

    SDPMediaFmt::Collection GetLocalAudioFormats()const;
    
    SDPMediaFmt::Collection GetLocalVideoFormats()const;

    PINLINE SIPTransport::Version GetAddressType()const{ return m_AddressType; };
    PINLINE const int GetLocalSessionId()const{ return m_LocalSessionId; };
    PINLINE const int GetLocalVersionId()const{ return m_LocalVersionId; };
    PINLINE const PString & GetLocalAudioAddress()const{ return m_LocalAudioAddress; };
    PINLINE const PString & GetLocalVideoAddress()const{ return m_LocalVideoAddress; };
    PINLINE WORD GetLocalAudioPort()const{ return m_LocalAudioPort; };
    PINLINE WORD GetLocalVideoPort()const{ return m_LocalVideoPort; };

  protected:

    void SortFormats();

    SDPMediaFmt::SortedCollection m_SortedAudioFormats;
    SDPMediaFmt::SortedCollection m_SortedVideoFormats;
    
    SIPTransport::Version m_AddressType;
    int m_LocalSessionId;
    int m_LocalVersionId;
    PString m_LocalAudioAddress;
    PString m_LocalVideoAddress;
    WORD m_LocalAudioPort;
    WORD m_LocalVideoPort;

    SDPMediaDesc * m_LocalAudioMediaDescription;
    SDPMediaDesc * m_LocalVideoMediaDescription;
    
    
  };
};

#endif

