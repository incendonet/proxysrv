/*
 *
 * B2BMediaInterface.cxx
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
 * $Log: B2BMediaInterface.cxx,v $
 * Revision 1.21  2007/01/10 04:57:34  joegenbaclor
 * Added check to not proxy media if localAddress and targetAddress are the same
 *
 * Revision 1.20  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.19  2006/10/25 09:24:55  rcolobong
 * Fix bug where m_Leg1Call might be NULL during TranslateSDPAnswer
 *
 * Revision 1.18  2006/09/26 03:26:04  rcolobong
 * RTP will be automatically encrypted if SIP Messages is encrypted
 *
 * Revision 1.17  2006/09/25 04:43:30  rcolobong
 * 1. Fix bug in 415 Unsupported Media where CallSession::m_ICTState is not properly initialized during construction.
 * 2. Remove the temporary fix for the 415 Unsupported Media where we use Semaphore to wait for second leg SDP in OnRequireSDPAnswer
 *
 * Revision 1.16  2006/09/22 08:52:45  joegenbaclor
 * Temporary fix for OnRequireSDPAnswer to use a semaphore to wait for second leg SDP
 *
 * Revision 1.15  2006/09/21 08:12:24  joegenbaclor
 * We now set socket information for inbound calls if media address is public
 *
 * Revision 1.14  2006/09/20 10:17:38  joegenbaclor
 * Corrected bug where RTP_SessionManager is not acquired as a reference in TranslateSDPOffer
 *
 * Revision 1.13  2006/09/20 09:51:15  joegenbaclor
 * 1.  Disabled Media Aggregation in B2BMediaInterface
 * 2.  Added logging for RTP statitics for media proxy sessions
 *
 * Revision 1.12  2006/09/19 10:36:29  rcolobong
 * OnRequireSDPAnswer will return false if no SDP available for both leg 1 and 2
 *
 * Revision 1.11  2006/09/18 09:35:16  joegenbaclor
 * Completed initial implementation of RTP media aggregation
 *
 * Revision 1.10  2006/09/18 03:28:55  joegenbaclor
 * 1. More work on media aggregation
 * 2. Check sock->GetPort() for NULL in SIPTransportManager::GetListenerAddress() (thanks to
 *  Vamsi Pottangi for reporting the bug)
 * 3.  Added reference size check to GCObject::ReleaseRef()
 *
 * Revision 1.9  2006/09/15 15:31:39  joegenbaclor
 * Changed Log levels to decrease log size.
 * Changed default session keep alaive interval from 10 seconds to two minutes
 *
 * Revision 1.8  2006/09/13 10:36:25  joegenbaclor
 * Added Release-Minimal build for smaller DLL footprint for ATLSIP
 *
 * Revision 1.7  2006/09/08 04:39:17  joegenbaclor
 * Implemented INFO Session Keep-alive
 *
 * Revision 1.6  2006/09/07 09:49:58  joegenbaclor
 * Fixed bug on possible race conditions when multiple thread calls Session::Destroy
 *  at the same time
 *
 * Revision 1.5  2006/09/01 13:17:58  rcolobong
 * 1. Creating RTP is now controlled in B2BUAConnection
 * 2. Fixed bug if we will CreateRTP even for Public Addresses
 *
 * Revision 1.4  2006/06/29 00:45:22  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.3  2006/06/28 14:14:58  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.2  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
 *
 * Revision 1.1  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 *
 */

#include "B2BUA.h"
#include "B2BMediaInterface.h"
#include "B2BUACall.h"
#include "B2BUAConnection.h"
#include "SDPLazyParser.h"

#define new PNEW

using namespace B2BUA;
using namespace SDP;

B2BMediaInterface::B2BMediaInterface(  
    B2BUserAgent & b2bua 
) : B2BInterface( b2bua )
{
  m_OpalManager.SetRtpIpPorts( 30000, 35000 );
  m_UseAggregatedMedia = FALSE;
  m_MediaAggregationList.DisallowDeleteObjects();
  m_RTPSocketList.DisallowDeleteObjects();
  m_RTPSelectList.DisallowDeleteObjects();
  m_MediaAggregationThread = NULL;
  m_MediaAggragationPool = NULL;
  m_AggregationCallBack = PCREATE_NOTIFIER( OnProcessAggregation );
}

B2BMediaInterface::~B2BMediaInterface()
{
  if( m_MediaAggregationThread != NULL )
    delete m_MediaAggregationThread;

  if( m_MediaAggragationPool != NULL )
    delete m_MediaAggragationPool;
}

/// Called when an invite is received with SDP
/// or a 180, 183, 200 is received with SDP
BOOL B2BMediaInterface::OnIncomingSDPOffer(
  B2BUAConnection & connection,
  B2BUACall & call,
  const SIPMessage & sdp
)
{
  if( call.GetLegIndex() == 0 )
    connection.SetLeg1SDP( sdp.GetBody() );
  else
  {
    connection.SetLeg2SDP( sdp.GetBody() );
  }
#if HAS_OPAL
  return CreateRTPSession( connection, call, sdp, connection.GetMediaProxyIfPrivate() );
#else // ! HAS_OPAL
  return TRUE;
#endif // ! HAS_OPAL

}

/// Called when sending an outbound INVITE or reInvites
BOOL B2BMediaInterface::OnRequireSDPOffer(
  B2BUAConnection & connection,
  B2BUACall & call,
  SIPMessage & offer
)
{
  if( call.GetLegIndex() == 0 )
  { 
    if( connection.GetLeg2SDP().IsEmpty() )
      return FALSE;

    offer.SetSDP( connection.GetLeg2SDP() );

  }else if( call.GetLegIndex() == 1 )
  {
    if( connection.GetLeg1SDP().IsEmpty() )
      return FALSE;

    offer.SetSDP( connection.GetLeg1SDP() );
  }
  
  return TranslateSDPOffer( connection, call, offer );
}

/// called when a previously received offer is to be answered
/// usually called when sending 183 0r 200 OK
BOOL B2BMediaInterface::OnRequireSDPAnswer(
  B2BUAConnection & connection,
  B2BUACall & call,
  const SIPMessage & offer,
  SIPMessage & answer
)
{
  if( call.GetLegIndex() == 0 )
  { 
    if( connection.GetLeg2SDP().IsEmpty() )
    {
      return FALSE;
    }

    answer.SetSDP( connection.GetLeg2SDP() );

  }else if( call.GetLegIndex() == 1 )
  {
    if( connection.GetLeg1SDP().IsEmpty() )
    {
      return FALSE;
    }

    answer.SetSDP( connection.GetLeg1SDP() );
  }

  if( TranslateSDPAnswer( connection, call, offer, answer ) )
    return OnOpenMediaStreams( connection );

  return FALSE;
}

#if HAS_OPAL

BOOL B2BMediaInterface::CreateRTPSession(
  B2BUAConnection & connection,
  B2BUACall & call,
  const SIPMessage & sdp,
  BOOL createOnlyIfPrivate
)
{
  SDPLazyParser parser = sdp.GetBody();
  SIPURI mediaAddress, videoAddress, faxAddress;
  RTP_SessionManager & manager = call.GetRTPSessionManager();
  
  if( sdp.GetViaSize() == 0 )
    return FALSE;

  
  Via targetVia;
  Via localVia;

  if( !sdp.GetViaAt( sdp.GetViaSize() - 1, targetVia ) )
    return FALSE;

  
  PIPSocket::Address targetAddress, localAddress;
  WORD targetPort = 0;
  
  /// incoming invite
  if( sdp.IsInvite() )
  {
    targetAddress = PIPSocket::Address( targetVia.GetReceiveAddress() );
    if( targetAddress.IsValid() )
      if( !call.GetSessionManager().ConstructVia( targetAddress, localVia, SIPTransport::UDP ) )
          return FALSE;
    
    localAddress = PIPSocket::Address( localVia.GetAddress() );
  }  

  if( parser.GetMediaAddress( SDPLazyParser::Audio, mediaAddress ) &&
      manager.GetSession( OpalMediaFormat::DefaultAudioSessionID ) == NULL )
  {
    if( !mediaAddress.IsPrivateNetwork() )
    {
      targetAddress = mediaAddress.GetAddress();
      targetPort = mediaAddress.GetPort().AsUnsigned();
    }

    if( !sdp.IsInvite() )
    {
      if( mediaAddress.IsPrivateNetwork() )
      {
        PString addr;
        sdp.GetViaAt( 0, targetVia );
        targetVia.GetParameter( "uas-addr", addr );
        if( addr.IsEmpty() )
          targetAddress = mediaAddress.GetAddress();
        else
          targetAddress = PIPSocket::Address( addr );
      }

      if( targetAddress.IsValid() )
        if( !call.GetSessionManager().ConstructVia( targetAddress, localVia, SIPTransport::UDP ) )
            return FALSE;
    
      localAddress = PIPSocket::Address( localVia.GetAddress() );
    }

    if( targetAddress != localAddress )
    {
      if( ( mediaAddress.IsPrivateNetwork() || !createOnlyIfPrivate )&& 
        targetAddress.IsValid() && localAddress.IsValid()  )
      {
        if( !CreateRTPSession( 
          OpalMediaFormat::DefaultAudioSessionID,
          localAddress,
          targetAddress,
          targetPort,
          parser,
          manager,
          call.IsXORHashEnabled()
        ) )return FALSE;
      }
    }
  }

  if( parser.GetMediaAddress( SDPLazyParser::Video, videoAddress ) &&
    manager.GetSession( OpalMediaFormat::DefaultVideoSessionID )== NULL )
  {
    if( !videoAddress.IsPrivateNetwork() )
    {
      targetAddress = videoAddress.GetAddress();
      targetPort = videoAddress.GetPort().AsUnsigned();
    }

    if( !sdp.IsInvite() )
    {
      if( videoAddress.IsPrivateNetwork() )
      {
        PString addr;
        sdp.GetViaAt( 0, targetVia );
        targetVia.GetParameter( "uas-addr", addr );
        if( addr.IsEmpty() )
          targetAddress = videoAddress.GetAddress();
        else
          targetAddress = PIPSocket::Address( addr );
      }

      if( targetAddress.IsValid() )
        if( !call.GetSessionManager().ConstructVia( targetAddress, localVia, SIPTransport::UDP ) )
            return FALSE;
    
      localAddress = PIPSocket::Address( localVia.GetAddress() );
    }

    if( targetAddress != localAddress )
    {
      if( ( videoAddress.IsPrivateNetwork() || !createOnlyIfPrivate )&& 
        targetAddress.IsValid() && localAddress.IsValid() 
      )
      {
        if( !CreateRTPSession( 
          OpalMediaFormat::DefaultVideoSessionID,
          localAddress,
          targetAddress,
          targetPort,
          parser,
          manager,
          call.IsXORHashEnabled()
        ) )return FALSE;
      }
    }
  }
  
  return TRUE;
}

BOOL B2BMediaInterface::CreateRTPSession(
  unsigned sessionId,
  const PIPSocket::Address & localAddress,
  const PIPSocket::Address & targetAddress,
  WORD targetPort,
  const PString & sdp,
  RTP_SessionManager & manager,
  BOOL isXORHashed
)
{
  // create an RTP session
  RTP_UDP * rtpSession = new RTP_UDP(sessionId);
  WORD firstPort = m_OpalManager.GetRtpIpPortPair();
  WORD nextPort = firstPort;
  while (!rtpSession->Open(localAddress,
                           nextPort, nextPort,
                           m_OpalManager.GetRtpIpTypeofService(),
                           NULL,
                           NULL)) {
    nextPort = m_OpalManager.GetRtpIpPortPair();
    if (nextPort == firstPort) {
      delete rtpSession;
      return FALSE;
    }
  }

  rtpSession->SetXORHash( isXORHashed );

  rtpSession->SetLocalAddress(localAddress);
  if( targetAddress.IsValid() && targetPort != 0 )
  {
    rtpSession->SetRemoteSocketInfo( targetAddress, targetPort, TRUE );
    rtpSession->SetRemoteSocketInfo( targetAddress, targetPort + 1, FALSE );
  }

  manager.AddSession( rtpSession );
  
  return TRUE;
}

BOOL B2BMediaInterface::TranslateSDPOffer(
  B2BUAConnection & connection,
  B2BUACall & call,
  SIPMessage & offer
)
{
  if( !offer.IsInvite() )
    return FALSE; /// we only support offers from invites for the time being

  SDPLazyParser parser = offer.GetBody();
  SIPURI mediaAddress, videoAddress, faxAddress;
  RTP_SessionManager & manager = call.GetRTPSessionManager();

  if( call.GetLegIndex() == 0 )
  { 
    //this can only be a reinvite initiated by callee
    if( parser.GetMediaCount( SDPLazyParser::Audio ) > 0 )
    {
      /// check if leg1 has an RTP session created by the very first invite
      RTP_UDP * rtp = (RTP_UDP *)manager.GetSession( OpalMediaFormat::DefaultAudioSessionID );

      if( rtp != NULL )
      {
        PIPSocket::Address addr = rtp->GetLocalAddress();
        WORD port = rtp->GetLocalDataPort();
        parser.SetMediaAddress(
          SDPLazyParser::Audio,
          0,
          addr.AsString(),
          port );
      }
    }

    if( parser.GetMediaCount( SDPLazyParser::Video ) > 0 )
    {
      /// check if leg2 has an RTP session created by the very first invite
      RTP_UDP * rtp = (RTP_UDP *)manager.GetSession( OpalMediaFormat::DefaultVideoSessionID );

      if( rtp != NULL )
      {
        PIPSocket::Address addr = rtp->GetLocalAddress();
        WORD port = rtp->GetLocalDataPort();
        parser.SetMediaAddress(
          SDPLazyParser::Video,
          0,
          addr.AsString(),
          port );
      }
    }
  }else if( call.GetLegIndex() == 1 )
  {
    /// this could be an outbound invite
    /// or a reInvite invoked by the caller
    B2BUACall * leg1 = connection.GetLeg1Call();
    if( leg1 == NULL )
      return FALSE;

    // Create Reference here. There are chances that it might be deleted
    // if the B2BUAConnection is in state of destruction.
    leg1->GCCreateRef( "B2BMediaInterface::TranslateSDPOffer" );

    RTP_SessionManager & leg1RTPManager = leg1->GetRTPSessionManager();
    ///check if leg 1 has an RTP session
    /// if it has, then lets create a corresponding stream
    /// for leg 2 if its not there
    RTP_UDP * leg1Audio = (RTP_UDP *)leg1RTPManager.GetSession( 
      OpalMediaFormat::DefaultAudioSessionID );

    RTP_UDP * leg1Video = (RTP_UDP *)leg1RTPManager.GetSession( 
      OpalMediaFormat::DefaultVideoSessionID );

    if( leg1Audio != NULL || leg1Video != NULL )
    {
      /// leg 1 has RTP Sessions, now check leg2.
      RTP_UDP * leg2Audio = (RTP_UDP *)manager.GetSession( 
        OpalMediaFormat::DefaultAudioSessionID );

      RTP_UDP * leg2Video = (RTP_UDP *)manager.GetSession( 
        OpalMediaFormat::DefaultVideoSessionID );

      if( leg2Audio == NULL || leg2Video == NULL )
      {
        /// create leg 2 RTP sessions.  we will base localAddress
        /// from the via we have in the INVITE
        Via via;
        offer.GetViaAt( 0, via );
        PIPSocket::Address localAddress = via.GetAddress();
        PIPSocket::Address targetAddress;
        WORD targetPort;
        SIPTransport::Resolve( offer, targetAddress, targetPort );

        if( leg1Audio != NULL && leg2Audio == NULL  )
          CreateRTPSession(
            OpalMediaFormat::DefaultAudioSessionID,
            localAddress,
            targetAddress,
            0,
            "",
            manager );

        if( leg1Video != NULL && leg2Video == NULL )
          CreateRTPSession(
            OpalMediaFormat::DefaultVideoSessionID,
            localAddress,
            targetAddress,
            0,
            "",
            manager );

        /// now translate the offer
        //this can only be a reinvite initiated by callee
        if( parser.GetMediaCount( SDPLazyParser::Audio ) > 0 )
        {
          /// check if leg1 has an RTP session created by the very first invite
          RTP_UDP * rtp = (RTP_UDP *)manager.GetSession( OpalMediaFormat::DefaultAudioSessionID );

          if( rtp != NULL )
          {
            PIPSocket::Address addr = rtp->GetLocalAddress();
            WORD port = rtp->GetLocalDataPort();
            parser.SetMediaAddress(
              SDPLazyParser::Audio,
              0,
              addr.AsString(),
              port );
          }
        }

        if( parser.GetMediaCount( SDPLazyParser::Video ) > 0 )
        {
          /// check if leg2 has an RTP session created by the very first invite
          RTP_UDP * rtp = (RTP_UDP *)manager.GetSession( OpalMediaFormat::DefaultVideoSessionID );

          if( rtp != NULL )
          {
            PIPSocket::Address addr = rtp->GetLocalAddress();
            WORD port = rtp->GetLocalDataPort();
            parser.SetMediaAddress(
              SDPLazyParser::Video,
              0,
              addr.AsString(),
              port );
          }
        }
      }
    } 
  }

  offer.SetSDP( parser );

  return TRUE;
}

BOOL B2BMediaInterface::TranslateSDPAnswer(
  B2BUAConnection & connection,
  B2BUACall & call,
  const SIPMessage & offer,
  SIPMessage & answer
)
{
  if( answer.IsRequest() )
    return FALSE; /// we only support answers from responses for the time being

  SDPLazyParser parser = answer.GetBody();
  SIPURI mediaAddress, videoAddress, faxAddress;
  RTP_SessionManager & manager = call.GetRTPSessionManager();

  //this can only be a reinvite initiated by callee
  if( parser.GetMediaCount( SDPLazyParser::Audio ) > 0 )
  {
    /// check if leg1 has an RTP session created by the very first invite
    RTP_UDP * rtp = (RTP_UDP *)manager.GetSession( OpalMediaFormat::DefaultAudioSessionID );

    if( rtp != NULL )
    {
      PIPSocket::Address addr = rtp->GetLocalAddress();
      WORD port = rtp->GetLocalDataPort();
      parser.SetMediaAddress(
        SDPLazyParser::Audio,
        0,
        addr.AsString(),
        port );
    }
  }

  if( parser.GetMediaCount( SDPLazyParser::Video ) > 0 )
  {
    /// check if leg2 has an RTP session created by the very first invite
    RTP_UDP * rtp = (RTP_UDP *)manager.GetSession( OpalMediaFormat::DefaultVideoSessionID );

    if( rtp != NULL )
    {
      PIPSocket::Address addr = rtp->GetLocalAddress();
      WORD port = rtp->GetLocalDataPort();
      parser.SetMediaAddress(
        SDPLazyParser::Video,
        0,
        addr.AsString(),
        port );
    }
  }

  answer.SetBody( parser );

  return TRUE;
}

BOOL B2BMediaInterface::OnOpenMediaStreams(
  B2BUAConnection & connection
)
{
  PWaitAndSignal lock( m_MediaStreamsMutex );

  if( connection.HasStartedMediaStream() )
    return TRUE;

  B2BUACall * leg1Call = connection.GetLeg1Call();
  B2BUACall * leg2Call = connection.GetLeg2Call();
  RTP_SessionManager & leg1Manager = leg1Call->GetRTPSessionManager();
  RTP_SessionManager & leg2Manager = leg2Call->GetRTPSessionManager();

  //start audio media streams
  RTP_UDP * leg1Audio = (RTP_UDP*)leg1Manager.GetSession( OpalMediaFormat::DefaultAudioSessionID );
  RTP_UDP * leg2Audio = (RTP_UDP*)leg2Manager.GetSession( OpalMediaFormat::DefaultAudioSessionID );

  if( leg1Audio != NULL && leg2Audio != NULL )
  {
    if( !m_UseAggregatedMedia )
    {
      leg2Call->AttachAudioRTPSessionThread(
        new OpalMediaThread( leg2Audio, leg1Audio ) );
      
      leg1Call->AttachAudioRTPSessionThread(
        new OpalMediaThread( leg1Audio, leg2Audio ) );
    }else
    {
      CreateMediaAggregation( leg2Audio, leg1Audio );
      CreateMediaAggregation( leg1Audio, leg2Audio );
    }
  }

  RTP_UDP * leg1Video = (RTP_UDP*)leg1Manager.GetSession( OpalMediaFormat::DefaultVideoSessionID );
  RTP_UDP * leg2Video = (RTP_UDP*)leg2Manager.GetSession( OpalMediaFormat::DefaultVideoSessionID );

  if( leg1Video != NULL && leg2Video != NULL )
  {
    if( !m_UseAggregatedMedia )
    {
      leg2Call->AttachVideoRTPSessionThread(
        new OpalMediaThread( leg2Video, leg1Video ) );
      leg1Call->AttachVideoRTPSessionThread(
        new OpalMediaThread( leg1Video, leg2Video ) );
    }else
    {
      CreateMediaAggregation( leg2Video, leg1Video );
      CreateMediaAggregation( leg1Video, leg2Video );
    }
  }

  connection.StartMediaStream();

  return TRUE;
}

B2BMediaInterface::OpalMediaThread::OpalMediaThread(
  RTP_UDP * leg1,
  RTP_UDP * leg2
) : PThread( 
      10000,
      NoAutoDeleteThread,
      HighestPriority,
      "OpalMediaThread:%x" )
{
  m_Leg1 = leg1;
  m_Leg2 = leg2;
  Resume();
}

void B2BMediaInterface::OpalMediaThread::Main()
{
  PTRACE( 1, "RTP_UDP\tStarted Media Stream" );
  for(;;)
  {
    RTP_DataFrame frame;
    if( !m_Leg1->ReadData( frame ) )
      break;

    if( !m_Leg2->WriteData( frame ) )
      break;
    

  }
  PTRACE( 1, "RTP_UDP\tClosing Media Stream" );
}

////////////////////////////////////////////////////////////////////////////////
GCCREATECOLLECTOR( B2BMediaInterface::AggregatedMedia );

B2BMediaInterface::AggregatedMedia::AggregatedMedia(
  B2BMediaInterface & mediaInterface,
  RTP_UDP * leg1,
  RTP_UDP * leg2
) : m_MediaInterface( mediaInterface ),
    m_ExitSync( 0, P_MAX_INDEX )
{
  m_Leg1 = leg1;
  m_Leg2 = leg2;

  m_DataSocketHandle = m_Leg1->GetDataSocket().GetHandle();
  m_ControlSocketHandle = m_Leg1->GetControlSocket().GetHandle();
  m_ExitFlag = FALSE;
}

B2BMediaInterface::AggregatedMedia::~AggregatedMedia()
{
  PAssert( WaitForTermination( 30000 ), "Aggregated Media failed to terminate!!!" );  
}

BOOL B2BMediaInterface::AggregatedMedia::WaitForTermination( 
  const PTimeInterval & wait
)
{
  m_ExitFlag = TRUE;
  return m_ExitSync.Wait( wait );
}

void B2BMediaInterface::AggregatedMedia::Close()
{
  m_ExitFlag = TRUE;
  m_ExitSync.Signal();
  GCRef gcRef = GCCreateRef( "AggregatedMedia::Close" );
  m_MediaInterface.OnClosedAggregatedMedia( gcRef );
}

BOOL B2BMediaInterface::OnClosedAggregatedMedia(
  AggregatedMedia::GCRef & gcRef 
)
{

  PWaitAndSignal lock1( m_MediaAggregationMutex );
  PWaitAndSignal lock2( m_RTPSocketListMutex );

  AggregatedMedia * media = dynamic_cast<AggregatedMedia *>(m_MediaAggregationList.RemoveAt( gcRef->GetDataSocketHandle() ));
  if( media == NULL )
  {
    PAssertAlways( PLogicError );
    return FALSE;
  }

  m_MediaAggregationList.RemoveAt( gcRef->GetControlSocketHandle() );
  m_RTPSocketList.RemoveAt( gcRef->GetDataSocketHandle() );
  m_RTPSocketList.RemoveAt( gcRef->GetControlSocketHandle() );
  m_RTPSelectList.Remove( &(gcRef->GetDataSocket()) );
  m_RTPSelectList.Remove( &(gcRef->GetControlSocket()) );
  
  return TRUE;
}

BOOL B2BMediaInterface::CreateMediaAggregation(
  RTP_UDP * leg1,
  RTP_UDP * leg2
)
{
  PWaitAndSignal lock1( m_MediaAggregationMutex );
  PWaitAndSignal lock2( m_RTPSocketListMutex );
  
  AggregatedMedia * media = new AggregatedMedia( *this, leg1, leg2 );
  m_MediaAggregationList.SetAt( media->GetDataSocketHandle(), media );
  m_MediaAggregationList.SetAt( media->GetControlSocketHandle(), media );
  m_RTPSocketList.SetAt( media->GetDataSocketHandle(), &(leg1->GetDataSocket()) );
  m_RTPSelectList.Append( &(leg1->GetDataSocket()) );
  m_RTPSocketList.SetAt( media->GetControlSocketHandle(), &(leg1->GetControlSocket()) );
  m_RTPSelectList.Append( &(leg1->GetControlSocket()) );
  m_MediaAggregationSync.Signal();
  if( m_MediaAggragationPool == NULL )
    m_MediaAggragationPool = new Tools::ThreadPool( 20, 1024*2, PThread::HighestPriority, "Media Aggregation" );

  if( m_MediaAggregationThread == NULL )
    m_MediaAggregationThread = new MediaAggregationThread( *this );

  return TRUE;
}

BOOL B2BMediaInterface::ProcessMediaAggregation()
{

  if( m_RTPSelectList.GetSize() == 0 )
    m_MediaAggregationSync.Wait();

  PWaitAndSignal lock1( m_MediaAggregationMutex );
  PWaitAndSignal lock2( m_RTPSocketListMutex );
  
  PSocket::SelectList read;

  for( PINDEX i = 0; i < m_RTPSelectList.GetSize(); i++ )
    read.Append( &(m_RTPSelectList[i]) );

  PChannel::Errors error = PSocket::Select( read, 5000 );

  if (error != PChannel::NoError )
    return FALSE;

  if( read.GetSize() == 0)
  {
    return FALSE;  
  }

  for( PINDEX j = 0; j < read.GetSize(); j++ )
  {
    PWaitAndSignal lock1( m_MediaAggregationMutex );
    PUDPSocket & socket = (PUDPSocket &)read[j];
    m_MediaAggragationPool->DoWork( m_AggregationCallBack, socket.GetHandle() );
  }

  return TRUE;
}

void B2BMediaInterface::OnProcessAggregation(
  PThread & /*thrd*/,
  INT handle
)
{
  PWaitAndSignal lock1( m_MediaAggregationMutex );
  AggregatedMedia * media = m_MediaAggregationList.GetAt( handle );
  if( media != NULL )
  {
    AggregatedMedia::GCRef gcRef = media->GCCreateRef( "OnProcessAggregation" );
    media->ProcessRead();
  }
}

BOOL B2BMediaInterface::AggregatedMedia::ProcessRead()
{
  PWaitAndSignal lock( m_ReadMutex );

  if( m_ExitFlag )
  {
    Close();
    return FALSE;
  }

  RTP_DataFrame frame;
  if( m_Leg1->ReadData( frame ) )
  {
    if( !m_Leg2->WriteData( frame ) )
    {
      Close();
      return FALSE;
    }
  }else
  {
    Close();
    return FALSE;
  }

  return TRUE;
}

#endif //HAS_OPAL



