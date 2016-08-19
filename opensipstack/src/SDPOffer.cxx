/*
 *
 * SDPOffer.cxx
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
 * $Log: SDPOffer.cxx,v $
 * Revision 1.10  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.9  2006/04/20 14:32:28  joegenbaclor
 * Fixed early media bug where soucd channels in OPAL ay not be opened in time for ringback to start streaming.
 *
 * Revision 1.8  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.7  2006/03/15 16:40:46  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.6  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SDPOffer.h"

using namespace SDP;

#define new PNEW
SDPOffer::SDPOffer()
{
  m_LocalAudioPort = 0;
  m_LocalVideoPort = 0;
  m_LocalAudioMediaDescription = NULL;
  m_LocalVideoMediaDescription = NULL;
  m_AddressType = SIPTransport::IPV4;
}

SDPOffer::SDPOffer(
  DWORD sessionId,
  DWORD versionId,
  const PString & localAddress,
  WORD audioPort,
  WORD videoPort,
  //const PString & /*addressType*/
  SIPTransport::Version addressType
)
{

  m_AddressType = addressType;

  m_LocalAudioPort = 0;
  m_LocalVideoPort = 0;
  
  m_LocalAudioMediaDescription = NULL;
  m_LocalVideoMediaDescription = NULL;

  m_LocalSessionId = sessionId;
  m_LocalVersionId = versionId;
  m_LocalAudioPort = audioPort;
  m_LocalVideoPort = videoPort;
  m_LocalAudioAddress = localAddress;
  m_LocalVideoAddress = localAddress;

  SDPPacket::Construct(
    *this,
    sessionId,
    versionId,
    localAddress, 
    addressType == SIPTransport::IPV4 ? "IP4" : "IP6"
  );

  m_LocalAudioMediaDescription = new SDPMediaDesc( "audio", audioPort );

  GetMediaSessions().Append( m_LocalAudioMediaDescription );

  m_LocalVideoMediaDescription = NULL;

  if( videoPort != 0 )
  {
    m_LocalVideoMediaDescription = new SDPMediaDesc( "video", videoPort );
    GetMediaSessions().Append( m_LocalVideoMediaDescription );
  }
}

SDPOffer::SDPOffer( 
  const SDPOffer & offer 
)
{
  operator=( offer );
}

SDPOffer & SDPOffer::operator =( 
  const SDPOffer & offer 
)
{
  m_LocalSessionId = offer.m_LocalSessionId;
  m_LocalVersionId = offer.m_LocalVersionId;
  m_LocalAudioPort = offer.m_LocalAudioPort;
  m_LocalVideoPort = offer.m_LocalVideoPort;
  m_LocalAudioAddress = offer.m_LocalAudioAddress;
  m_LocalVideoAddress = offer.m_LocalVideoAddress;
  m_AddressType = offer.m_AddressType;

  m_LocalAudioMediaDescription = NULL;
  m_LocalVideoMediaDescription = NULL;

  SDPPacket::Construct(
    *this,
    m_LocalSessionId,
    m_LocalVersionId,
    m_LocalAudioAddress,
    m_AddressType == SIPTransport::IPV4 ? "IP4" : "IP6"
  );

  if( offer.m_LocalAudioMediaDescription != NULL )
    m_LocalAudioMediaDescription = new SDPMediaDesc( *offer.m_LocalAudioMediaDescription );

  GetMediaSessions().Append( m_LocalAudioMediaDescription );

  if( offer.m_LocalVideoMediaDescription != NULL )
  {
    m_LocalVideoMediaDescription = new SDPMediaDesc( *offer.m_LocalVideoMediaDescription );
    GetMediaSessions().Append( m_LocalVideoMediaDescription );
  }

  return *this;
}

SDPOffer::SDPOffer(
  const PString & sdp 
)
{
  m_LocalAudioPort = 0;
  m_LocalVideoPort = 0;
  
  m_LocalAudioMediaDescription = NULL;
  m_LocalVideoMediaDescription = NULL;

  m_LocalAudioAddress = 0;
  m_LocalVideoAddress = 0;


  operator=( sdp );
}

SDPPacket & SDPOffer::operator=( const PString & sdp )
{
  SDPPacket::operator =( sdp );

  m_AddressType = SIPTransport::IPV4;

  if( ((SDPPacket*)this)->GetSession().GetOrigin() != NULL )
  {
    if( !((SDPPacket*)this)->GetSession().GetOrigin()->GetSessionId().IsEmpty() )
    /*int*/ m_LocalSessionId = ((SDPPacket*)this)->GetSession().GetOrigin()->GetSessionId().AsInteger();
    if( !((SDPPacket*)this)->GetSession().GetOrigin()->GetVersion().IsEmpty() )
    /*int*/ m_LocalVersionId = ((SDPPacket*)this)->GetSession().GetOrigin()->GetVersion().AsInteger();
    if( !((SDPPacket*)this)->GetSession().GetOrigin()->GetAddressType().IsEmpty() )
    {
      m_AddressType = SIPTransport::IPV4;
      if( ((SDPPacket*)this)->GetSession().GetOrigin()->GetAddressType() *= "IP6" )
        m_AddressType = SIPTransport::IPV6;
    }

    if( ((SDPPacket*)this)->GetSession().GetConnectionData() != NULL )
    {
      m_LocalAudioAddress = ((SDPPacket*)this)->GetSession().GetConnectionData()->GetAddress();
      m_LocalVideoAddress = ((SDPPacket*)this)->GetSession().GetConnectionData()->GetAddress();
    }

    SDPMediaDesc::Collection & mediaSessions = ((SDPPacket*)this)->GetMediaSessions();
    if( mediaSessions.GetSize() > 0 )
    {

      for( PINDEX i = 0; i < mediaSessions.GetSize(); i++ )
      {
        SDPMediaDesc & mediaDesc = mediaSessions[i];
        if( mediaDesc.GetMediaAnnouncement() != NULL )
        {
          PString m = mediaDesc.GetMediaAnnouncement()->GetMedia().ToLower().Trim();

          if( m == "audio" &&
            m_LocalAudioMediaDescription == NULL
          )
          {
            m_LocalAudioMediaDescription = (SDPMediaDesc*)mediaSessions.GetAt( i );
            if( mediaDesc.GetConnectionData() != NULL )
              m_LocalAudioAddress = mediaDesc.GetConnectionData()->GetAddress();
            if( mediaDesc.GetMediaAnnouncement() != NULL )
              m_LocalAudioPort = mediaDesc.GetMediaAnnouncement()->GetPort();
          }else if( m == "video" &&
            m_LocalVideoMediaDescription == NULL
          )
          {
            m_LocalVideoMediaDescription = (SDPMediaDesc*)mediaSessions.GetAt( i );
            if( mediaDesc.GetConnectionData() != NULL )
              m_LocalVideoAddress = mediaDesc.GetConnectionData()->GetAddress();
            if( mediaDesc.GetMediaAnnouncement() != NULL )
              m_LocalVideoPort = mediaDesc.GetMediaAnnouncement()->GetPort();
          }
        }
      }
    }
  }

  return *this;
}

void SDPOffer::PrintOn( 
  ostream & strm
)const
{
  SDPPacket::PrintOn( strm );
}

BOOL SDPOffer::AddFormat(
  int mediaType,
  BYTE format,
  const SDPAttribute & attribute
)
{
  if( mediaType == SDPMediaFmt::Audio )
  {
    m_LocalAudioMediaDescription->GetMediaAnnouncement()->AppendFormat( format );
    m_LocalAudioMediaDescription->AppendAttribute( attribute );
  }else if( mediaType == SDPMediaFmt::Video )
  {
    m_LocalVideoMediaDescription->GetMediaAnnouncement()->AppendFormat( format );
    m_LocalVideoMediaDescription->AppendAttribute( attribute );
  }else
  {
    /// we only support audio/video currently
    return FALSE;
  }

  return TRUE;
}

BOOL SDPOffer::AddFormat(
  const SDPMediaFmt & format
)
{
  if( format.GetMediaType() == SDPMediaFmt::Audio  )
  {
    //PAssertNULL( m_LocalAudioMediaDescription );
    m_LocalAudioMediaDescription->GetMediaAnnouncement()->AppendFormat( (BYTE)format.GetPayloadType() );
    m_LocalAudioMediaDescription->AppendAttribute( format );
  }else if( format.GetMediaType() == SDPMediaFmt::Video  )
  {
    //PAssertNULL( m_LocalVideoMediaDescription );
    m_LocalVideoMediaDescription->GetMediaAnnouncement()->AppendFormat( (BYTE)format.GetPayloadType()  );
    m_LocalVideoMediaDescription->AppendAttribute( format );
  }else
  {
    /// we only support audio/video currently
    return FALSE;
  }

  return TRUE;
}

int SDPOffer::ProcessAnswer(
  SDPPacket & answer,
  SDPMediaFmt::Collection & remoteAudioCaps,
  PString & remoteAudioAddress,
  WORD & remoteAudioPort,
  SDPMediaFmt::Collection & remoteVideoCaps,
  PString & remoteVideoAddress,
  WORD & remoteVideoPort
)
{
  int state = 0;
  SDPMediaDesc::Collection & mediaSessions = answer.GetMediaSessions();

  const SDPConnectionData * sessionConnectionData = answer.GetSession().GetConnectionData();
  if( sessionConnectionData != NULL )
  {
    remoteVideoAddress = remoteAudioAddress = sessionConnectionData->GetAddress();
  }

  if( m_LocalVideoMediaDescription != NULL && 
      m_LocalVideoMediaDescription != NULL && 
      mediaSessions.GetSize() != 2 )
  {
    state = StateAnswerInvalid;
    return StateAnswerInvalid;
  }

  /// we got the same number of media descriptions, now get the common formats among audio and video codecs
  

  if( m_LocalAudioMediaDescription != NULL )
  {
    SDPMediaFmt::Collection localCaps, remoteCaps;
    SDPMediaDesc & remoteDescription = mediaSessions[0];

    if( remoteDescription.GetMediaAnnouncement()->GetMedia() != "audio" )
    {
      for( PINDEX a = 1; a < mediaSessions.GetSize(); a++ )
      {
        remoteDescription = mediaSessions[a];
        if( remoteDescription.GetMediaAnnouncement()->GetMedia() *= "audio" )
          break;
      }
    }

    if( remoteDescription.GetMediaAnnouncement()->GetMedia() *= "audio" )
    {
      SDPOffer::GetMediaFormats( *m_LocalAudioMediaDescription, localCaps );
      SDPOffer::GetMediaFormats( remoteDescription, remoteCaps );
      
      /// reset the address if we have media level connection data
      const SDPConnectionData * connectionData = remoteDescription.GetConnectionData();
      if( connectionData != NULL )
        remoteAudioAddress = remoteDescription.GetConnectionData()->GetAddress();

      remoteAudioPort = remoteDescription.GetMediaAnnouncement()->GetPort();
    }
    
    if( remoteCaps.GetSize() > 0 )
    {

      for( PINDEX i = 0; i < localCaps.GetSize(); i++ )
      {
        PString localFormat = localCaps[i].GetEncodingName();
        
        /// now check if this format is among the remote capability list
        for( PINDEX j = 0; j < remoteCaps.GetSize(); j++ )
        {
          PString remoteFormat = remoteCaps[j].GetEncodingName();
          /// if found, append to merged capabilities
          if( remoteFormat *= localFormat )
            remoteAudioCaps.Append( new SDPMediaFmt( remoteCaps[j] ) );
        }
      }

      if( remoteAudioCaps.GetSize() <= 1 )
      {
        state = StateAnswerFinal;
      }else if( remoteAudioCaps.GetSize() == 2 )
      {
        ///check if one of them is RFC2833
        if( remoteAudioCaps[0].GetEncodingName() == "telephone-event"  ||
            remoteAudioCaps[1].GetEncodingName() == "telephone-event" )
        {
          state = StateAnswerFinal;
        }else
        {
          state = StateAnswerAmbiguous;
        }
      }else
      {
        state = StateAnswerAmbiguous;
      }
    }
  }

  if( m_LocalVideoMediaDescription != NULL )
  {
    SDPMediaFmt::Collection localCaps, remoteCaps;
    SDPMediaDesc & remoteDescription = mediaSessions[0];

    if( remoteDescription.GetMediaAnnouncement()->GetMedia() != "video" )
    {
      for( PINDEX a = 1; a < mediaSessions.GetSize(); a++ )
      {
        remoteDescription = mediaSessions[a];
        if( remoteDescription.GetMediaAnnouncement()->GetMedia() *= "video" )
          break;
      }
    }

    if( remoteDescription.GetMediaAnnouncement()->GetMedia() *= "video" )
    {
      SDPOffer::GetMediaFormats( *m_LocalVideoMediaDescription, localCaps );
      SDPOffer::GetMediaFormats( remoteDescription, remoteCaps );

      /// reset the address if we have media level connection data
      const SDPConnectionData * connectionData = remoteDescription.GetConnectionData();
      if( connectionData != NULL )
        remoteVideoAddress = remoteDescription.GetConnectionData()->GetAddress();

      remoteVideoPort = remoteDescription.GetMediaAnnouncement()->GetPort();
    }
    
    if( remoteCaps.GetSize() > 0 )
    {

      for( PINDEX i = 0; i < localCaps.GetSize(); i++ )
      {
        PString localFormat = localCaps[i].GetEncodingName();
        
        /// now check if this format is among the remote capability list
        for( PINDEX j = 0; j < remoteCaps.GetSize(); j++ )
        {
          PString remoteFormat = remoteCaps[j].GetEncodingName();
          /// if found, append to merged capabilities
          if( remoteFormat *= localFormat )
            remoteVideoCaps.Append( new SDPMediaFmt( remoteCaps[j] ) );
        }
      }

      if( remoteVideoCaps.GetSize() > 1 )
      {
        state = StateAnswerAmbiguous;
      }
    }
  }

  return state;
}


BOOL SDPOffer::GetMediaFormats( 
  SDPMediaDesc & sdp,
  SDPMediaFmt::Collection & formats
)
{
  PINDEX lastAttribute = 0;

  if( sdp.GetMediaAnnouncement()->GetFormatListSize() == 0 )
    return FALSE;

  const PBYTEArray & formatList = sdp.GetMediaAnnouncement()->GetFormatList();

  for( PINDEX i = 0; i < formatList.GetSize(); i++ )
  {
    int format = formatList[i];
    SDPMediaFmt * mediaFormat = new SDPMediaFmt();
    mediaFormat->SetPayloadType( format );

    if( format < SDPMediaFmt::DynamicBase )
    {  
      if( format == SDPG711uLaw.GetPayloadType() )
      {
        *mediaFormat = SDPG711uLaw;
      }else if( format == SDPG711ALaw.GetPayloadType() )
      {
        *mediaFormat = SDPG711ALaw;
      }else if( format == SDPG728.GetPayloadType() )
      {
        *mediaFormat = SDPG728;
      }else if( format == SDPG729.GetPayloadType() )
      {
        *mediaFormat = SDPG729;
      }else if( format == SDPG7231.GetPayloadType() )
      {
        *mediaFormat = SDPG7231;
      }else if( format == SDPGSM.GetPayloadType() )
      {
        *mediaFormat = SDPGSM;
      }else if( format == SDPH261.GetPayloadType() )
      {
        *mediaFormat = SDPH261;
      }else if( format == SDPH263.GetPayloadType() )
      {
        *mediaFormat = SDPH263;
      }else if( format == SDPMPV.GetPayloadType() )
      {
        *mediaFormat = SDPMPV;
      }
    }

    PINDEX j;

    for( j = lastAttribute; j < sdp.GetAttributeSize(); j++ )
    {
      SDPAttribute attr;
      sdp.GetAttribute( attr, j );

      if( attr.GetKeyword() *= "rtpmap" )
      {
        
        if( attr.GetAttribute() *= PString( format ) )
        {
          /// get the format name and clock rate
          PStringArray tokens = attr.GetParameter().Tokenise( '/' );
          if( tokens.GetSize() == 2 )
          {
            mediaFormat->SetEncodingName( tokens[0] );
            mediaFormat->SetClockRate( tokens[1].AsInteger() );
          }else
          {
            break;
          }

          /// process custom attributes within the range of this format

          ///const PStringArray & customAttributes = mediaFormat->GetCustomAttributes();

          for( PINDEX x = j + 1; x < sdp.GetAttributeSize(); x++ )
          {
            SDPAttribute customAttr;
            sdp.GetAttribute( customAttr, x);
            
            if( customAttr.GetKeyword() *= "rtpmap" )
              break;  /// this is the beginning of a new format... skip it

            

            if( customAttr.GetKeyword() *= "ptime" )
            {
              mediaFormat->SetSamplingRate( customAttr.GetAttribute().AsInteger() );
            }else
            {
              mediaFormat->AppendCustomAttribute( customAttr.AsString() );
            }
          }

          break; 
        }
      }
    }

    lastAttribute = j + 1;

    formats.Append( mediaFormat );

  }
  
  return FALSE;
}

void SDPOffer::SortFormats()
{
  SDPMediaFmt::Collection audioFormats = GetLocalAudioFormats();
  SDPMediaFmt::Collection videoFormats = GetLocalVideoFormats();

  if( m_SortedAudioFormats.GetSize() != audioFormats.GetSize() )
  {
    m_SortedAudioFormats.RemoveAll();
    for( PINDEX i = 0; i < audioFormats.GetSize(); i++ )
    {
      SDPMediaFmt * format = new SDPMediaFmt( audioFormats[i] );
      m_SortedAudioFormats.SetAt( format->GetEncodingName(), format );
    }
  }

  if( m_SortedVideoFormats.GetSize() != videoFormats.GetSize() )
  {
    m_SortedVideoFormats.RemoveAll();
    for( PINDEX i = 0; i < videoFormats.GetSize(); i++ )
    {
      SDPMediaFmt * format = new SDPMediaFmt( videoFormats[i] );
      m_SortedVideoFormats.SetAt( format->GetEncodingName(), format );
    }
  }
}

const SDPMediaFmt * SDPOffer::FindFormat(
  const PString & encodingName
)const
{
  return (const_cast< SDPOffer * >( this ))->GetFormat( encodingName );
}



SDPMediaFmt * SDPOffer::GetFormat(
  const PString & encodingName
)
{
  SortFormats();

  SDPMediaFmt * format = m_SortedAudioFormats.GetAt( encodingName );
  
  if( format == NULL )
    format = m_SortedVideoFormats.GetAt( encodingName );

  return format;
}



SDPMediaFmt::Collection SDPOffer::GetLocalAudioFormats()const
{
  SDPMediaFmt::Collection audioFormats;
  
  if( m_LocalAudioMediaDescription != NULL )
    SDPOffer::GetMediaFormats( *m_LocalAudioMediaDescription, audioFormats );
  
  return audioFormats;
}

SDPMediaFmt::Collection SDPOffer::GetLocalVideoFormats()const
{
  SDPMediaFmt::Collection videoFormats;
  
  if( m_LocalVideoMediaDescription != NULL )
    SDPOffer::GetMediaFormats( *m_LocalVideoMediaDescription, videoFormats );
  
  return videoFormats;
}
