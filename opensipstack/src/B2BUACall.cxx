/*
 *
 * B2BUACall.cxx
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
 * $Log: B2BUACall.cxx,v $
 * Revision 1.38  2007/01/22 12:25:55  joegenbaclor
 * Added record route for B2B calls.
 *
 * Revision 1.37  2007/01/22 10:01:58  joegenbaclor
 * Fixed bug in handling Notifies
 *
 * Revision 1.36  2007/01/19 08:21:12  joegenbaclor
 * Modified Redirect behavior.  Second leg would now process redirect instead of the
 *  session relaying 3xx to UAC
 *
 * Revision 1.35  2007/01/10 08:07:16  rcolobong
 * 1. Add new method for OnSendB2BConnect Call Interface.
 * 2. OnSendB2BConnect is supported in B2BUserAgent, B2BUACall and B2BUAConnection
 *
 * Revision 1.34  2006/12/21 05:39:37  joegenbaclor
 * Fixed REPLACES to use connection to get the LEG 2 call-id
 *
 * Revision 1.33  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.32  2006/11/22 03:16:18  joegenbaclor
 * Removed Record Routes from B2B Outbound invites
 *
 * Revision 1.31  2006/11/22 00:05:52  joegenbaclor
 * Bug fixes for multi-listener support
 *
 * Revision 1.30  2006/11/21 06:55:19  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.29  2006/11/10 07:53:10  rcolobong
 * 1. Change Access modifier for method SetB2BUAConnection
 * 2. Support Fork Calls Messages by giving 1xx-6xx events to B2BUAForkCalls
 *
 * Revision 1.28  2006/11/08 06:46:20  joegenbaclor
 * Added session level NIC/Interface binding
 *
 * Revision 1.27  2006/11/07 07:50:32  joegenbaclor
 * Corrected bug in Session timer processing.
 *
 * Revision 1.26  2006/10/23 15:44:58  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.25  2006/10/11 04:49:47  rcolobong
 * 1. Add support for Notify During Refer
 * 2. Fixed bug where 3xx invoke CallHandler OnRejected
 *
 * Revision 1.24  2006/10/05 12:56:07  rcolobong
 * Change Redirection id which is "redir" to "rdir"
 *
 * Revision 1.23  2006/09/21 05:09:19  joegenbaclor
 * Corrected typo from last commit
 *
 * Revision 1.22  2006/09/21 04:43:18  joegenbaclor
 * Improved RTP logging
 *
 * Revision 1.21  2006/09/20 09:51:15  joegenbaclor
 * 1.  Disabled Media Aggregation in B2BMediaInterface
 * 2.  Added logging for RTP statitics for media proxy sessions
 *
 * Revision 1.20  2006/09/19 10:25:51  joegenbaclor
 * Added sanity check to B2BAnswerCall to check the CSeq method of the response for commiting
 *  to an AnswerCall()
 *
 * Revision 1.19  2006/09/18 09:35:16  joegenbaclor
 * Completed initial implementation of RTP media aggregation
 *
 * Revision 1.18  2006/09/08 04:39:17  joegenbaclor
 * Implemented INFO Session Keep-alive
 *
 * Revision 1.17  2006/09/07 09:49:58  joegenbaclor
 * Fixed bug on possible race conditions when multiple thread calls Session::Destroy
 *  at the same time
 *
 * Revision 1.16  2006/08/30 03:42:05  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.15  2006/08/29 10:47:23  rcolobong
 * *** empty log message ***
 *
 * Revision 1.14  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.13  2006/08/26 14:31:44  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.12  2006/07/19 02:19:21  joegenbaclor
 * 1.  Bug fixes concerning B2BUA destruction during during 3xx, 4xx, 5xx, 6xx local response
 * 2.  Fixed bug where Route header is never removed from outbound Invite resulting that may result to a loop when
 *      Inbound Invite has a Route header present
 * 3.  Added RemoveAllRoutes() in SIPMessage
 *
 * Revision 1.11  2006/07/17 11:36:45  joegenbaclor
 * 1.  More routing enhancements to B2BUA
 *
 * Revision 1.10  2006/07/13 06:53:05  joegenbaclor
 * 1.  Introduced Sanity checks for incoming SIP Message
 * 2.  Corrected bug in SIPURI where enum scheme is not getting recognized.
 * 3.  Added ENUM support to routing
 * 4.  Introduced global routing flag to indicate when and not to rewrite outbound To URI's
 *
 * Revision 1.9  2006/07/11 13:59:36  joegenbaclor
 * Completed Proxy functionalities for B2BUA
 *
 * Revision 1.8  2006/07/10 06:29:38  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.7  2006/06/28 14:14:59  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.6  2006/06/26 03:06:06  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to
 * avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL
 * componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 1.5  2006/06/23 04:44:04  joegenbaclor
 * SendB2BRefer() added to B2BUACall
 *
 * Revision 1.4  2006/06/22 08:27:56  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.3  2006/06/21 14:29:11  joegenbaclor
 * This upload marks the first successful call by OpenB2BUA
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
#include "B2BUACall.h"
#include "B2BUAConnection.h"
#include "B2BUAForkCalls.h"

#define new PNEW

using namespace B2BUA;

/// UAC constructor
B2BUACall::B2BUACall(
  B2BUAEndPoint & manager,
  const PString & sessionId,
  const ProfileUA & profile
) : CallSession( manager, sessionId, profile )
{
  m_LegIndex = 1;
  m_ConnectionRef = NULL;
#if HAS_OPAL
  m_HasClosedMediaStreams = FALSE;
  m_AudioRTPSessionThread = NULL;
  m_VideoRTPSessionThread = NULL;
  m_AudioAggregator = NULL;
  m_VideoAggregator = NULL;
#endif
  m_LogTag = "CALL: (outbound) ";
}

/// UAS constructor
B2BUACall::B2BUACall(
  B2BUAEndPoint & sessionManager,
  const SIPMessage & request,
  const PString & sessionId
) : CallSession( sessionManager, request, sessionId )
{
  m_LegIndex = 0;
  m_ConnectionRef = NULL;
#if HAS_OPAL
  m_HasClosedMediaStreams = FALSE;
  m_AudioRTPSessionThread = NULL;
  m_VideoRTPSessionThread = NULL;
  m_AudioAggregator = NULL;
  m_VideoAggregator = NULL;
#endif
  SetLogContextId( sessionId + PString("-inbound") );

  m_LogTag = "CALL: (inbound) ";
}

B2BUACall::~B2BUACall()
{
  if( m_ConnectionRef != NULL )
  {
    delete m_ConnectionRef;
    m_ConnectionRef = NULL;
  }
}

void B2BUACall::SetB2BUAConnection( B2BUAConnection * conn )
{ 
  m_Connection = conn; 
  m_ConnectionRef = new SIPSession::GCRef( conn->GCCreateRef( "B2BUACall::SetB2BUAConnection" ) ); 
};

void B2BUACall::CloseMediaStreams()
{
  PWaitAndSignal lock( m_MediaStreamsMutex );
  if( m_HasClosedMediaStreams )
    return;

  m_HasClosedMediaStreams = TRUE;

  RTP_UDP * audio = (RTP_UDP *)m_RTPSessionManager.GetSession( OpalMediaFormat::DefaultAudioSessionID );
  if( audio != NULL )
  {
    PStringStream stats;
    
    stats << "RTP: (Audio) ";
    stats << "addr=" << audio->GetLocalAddress() << ":" << audio->GetLocalDataPort() << "->";
    stats << audio->GetRemoteAddress() << ":" << audio->GetRemoteDataPort() << " ";
    stats << "xor=" << audio->IsXORHashEnabled() << " ";
    stats << "rx=" << audio->GetPacketsReceived() << " ";
    stats << "tx=" << audio->GetPacketsSent() << " ";
    stats << "lost=" << audio->GetPacketsLost() << " ";
    stats << "outOfOrder=" << audio->GetPacketsOutOfOrder() << " ";
    stats << "late=" << audio->GetPacketsTooLate() << " ";

    stats << "rxTime=" << audio->GetAverageReceiveTime() << 
    "/" << audio->GetMaximumReceiveTime() << " ";

    stats << "txTime=" << audio->GetAverageSendTime() <<
    "/" << audio->GetMaximumSendTime() << " ";

    stats << "jitter=" << audio->GetAvgJitterTime() << 
    "/" << audio->GetMaxJitterTime();


    LOG( LogDetail(), stats );

    audio->Close(TRUE);
    audio->Close(FALSE);
  }

  RTP_UDP * video = (RTP_UDP *)m_RTPSessionManager.GetSession( OpalMediaFormat::DefaultVideoSessionID );
  if( video != NULL )
  {
    PStringStream stats;
    stats << "RTP: (Video) ";
    
    stats << "addr=" << video->GetLocalAddress() << ":" << video->GetLocalDataPort() << "->";
    stats << video->GetRemoteAddress() << ":" << video->GetRemoteDataPort() << " ";
    stats << "xor=" << video->IsXORHashEnabled() << " ";
    stats << "rx=" << video->GetPacketsReceived() << " ";
    stats << "tx=" << video->GetPacketsSent() << " ";
    stats << "lost=" << video->GetPacketsLost() << " ";
    stats << "outOfOrder=" << video->GetPacketsOutOfOrder() << " ";
    stats << "late=" << video->GetPacketsTooLate() << " ";

    stats << "rxTime=" << video->GetAverageReceiveTime() << 
    "/" << video->GetMaximumReceiveTime() << " ";

    stats << "txTime=" << video->GetAverageSendTime() <<
    "/" << video->GetMaximumSendTime() << " ";

    stats << "jitter=" << video->GetAvgJitterTime() << 
    "/" << video->GetMaxJitterTime();

    LOG( LogDetail(), stats );

    video->Close(TRUE);
    video->Close(FALSE);
  }

}

void B2BUACall::OnDestroySession()
{
#if HAS_OPAL
  CloseMediaStreams();

  if( m_AudioRTPSessionThread != NULL )
  {
    PAssert( m_AudioRTPSessionThread->WaitForTermination( 10000 ), "Audio Session Thread Failed to terminate" );
    delete m_AudioRTPSessionThread;
  }
  
  if( m_VideoRTPSessionThread != NULL )
  {
    PAssert( m_VideoRTPSessionThread->WaitForTermination( 10000 ), "Video Session Thread Failed to terminate" );
    delete m_VideoRTPSessionThread;
  }

  if( m_AudioAggregator != NULL )
  {
    PAssert( m_AudioAggregator->WaitForTermination( 10000 ), "Audio Session Thread Failed to terminate" );
    m_AudioAggregator->GCCollect();
    m_AudioAggregator = NULL;
  }

  if( m_VideoAggregator != NULL )
  {
    PAssert( m_VideoAggregator->WaitForTermination( 10000 ), "Video Session Thread Failed to terminate" );
    m_VideoAggregator->GCCollect();
    m_VideoAggregator = NULL;
  }

#endif
}

void B2BUACall::OnSessionEvent(
  int event,
  const SIPMessage & eventMsg
)
{
  switch( event )
  {
    case IST_AnswerCallNow:                // Sends 200 OK back with SDP after receipt of INVITE
      if( SendB2BConnect( eventMsg ) )
        SetState( StateConnected );
      break;
    case IST_AnswerCallDeferred:           // Answers with 180 ringing
      //InternalAnswerCall( AnswerCallDeferred, eventMsg );
      SendB2BProgress( eventMsg );
      SetState( StateAlerting );
      break;
    case IST_AnswerCallDeferredWithMedia:  // Like AnswerCallDefered Only media is sent in 183 progress
      SendB2BProgress( eventMsg );
      SetState( StateProceeding );
      break;
    case IST_AnswerCallRedirect:           // Redirect the call to another address
#if 0
      SendB2BRedirect( eventMsg );
#endif
      SetState( StateRedirecting );
      break;
    case IST_AnswerCallRedirectToProxy:    // Redirect the call to a proxy
#if 0      
      SendB2BRedirect( eventMsg );
#endif
      SetState( StateRedirecting );
      break;
    case IST_AnswerCallDenied:              // Reject the call
      SendB2BReject( eventMsg );
      if( GetState() < StateConnected )
        SetState( StateDisconnected );
      break;
    default:
      CallSession::OnSessionEvent( event, eventMsg );
  }
}

BOOL B2BUACall::SendB2BConnect( 
  const SIPMessage & secondLegResponse 
)
{
  GCREF( "B2BUACall::SendB2BConnect" );

  if( !secondLegResponse.Is2xx() )
    PAssertAlways( PLogicError );

  SIPMessage ok;
  if( !m_CurrentUASInvite.CreateResponse( ok, SIPMessage::Code200_Ok ) )
  {
    return SendReject( 
      m_CurrentUASInvite, 
      SIPMessage::Code500_InternalServerError
    );

    return FALSE;
  }


  PString sdp;
  if( secondLegResponse.HasSDP() )
  {
    if( !OnRequireSDPAnswer( m_CurrentUASInvite, ok ) )
    {
      PTRACE( 1, "B2BUACall::SendB2BConnect Rejecting Media" );

      SendReject( m_CurrentUASInvite, GetCallAnswerResponse() == DontDeny ?
                            SIPMessage::Code415_UnsupportedMedia :
          (SIPMessage::StatusCodes)GetCallAnswerResponse() );

          return FALSE;
    }
  }


  PIPSocket::Address localAddress, targetAddress;
  WORD localPort = 0, targetPort = 0;

  SIPTransport::Resolve( ok, targetAddress, targetPort );
  GetSessionManager().GetTransportManager()->GetListenerAddress( 
    SIPTransport::UDP, targetAddress, localAddress, localPort );

  /// set the contact address
  SIPURI curi;
  curi.SetHost( localAddress.AsString() );
  curi.SetPort( PString( localPort ) );
  ContactURI contactURI;
  contactURI.SetURI( curi );
  ok.AppendContact( contactURI );

  /// Here we should send the 200 ok to the retransmitter
  CallSessionManager::RetransmitThread * retran = (CallSessionManager::RetransmitThread *)m_CallSessionManager.GetRetransmitThread();
  retran->AddMessage( GetSessionId(), ok );
  InitializeDialogAsUAS( m_CurrentUASInvite, ok );

  if( m_EnableSessionTimer && m_CurrentUASInvite.IsSupportedExtension( "timer" ) && !OnProcessSessionTimers( m_CurrentUASInvite, ok ) )
      return FALSE;  

  /// Let's add our record route just in case UAs would not honor our contact address
  /// proxies in the middle would be able to route back to us correctly

  RouteURI recordRoute;
  recordRoute.SetURI( curi );
  recordRoute.SetLooseRouter( TRUE );
  ok.AppendRecordRoute( recordRoute );

  
  /// Send it to the FSM once to terminate the IST transaction
  if( !SendRequest( ok ) )
    return FALSE;

  GetB2BUAConnection().OnSendB2BConnect( *this, secondLegResponse );

  return TRUE;

}

BOOL B2BUACall::SendB2BProgress( 
  const SIPMessage & secondLegResponse 
)
{
  GCREF( "B2BUACall::SendB2BProgress" );

  PAssert( secondLegResponse.Is1xx(), PLogicError );

  SIPMessage progress;
  if( !m_CurrentUASInvite.CreateResponse( 
    progress, 
    (SIPMessage::StatusCodes)secondLegResponse.GetStatusCode() ) )
    return FALSE;

  if( secondLegResponse.HasSDP() )
  {
    if( !OnRequireSDPAnswer( m_CurrentUASInvite, progress ) )
    {
      PTRACE( 1, "B2BUACall::SendB2BProgress Rejecting Media" );
      SendReject( m_CurrentUASInvite, GetCallAnswerResponse() == DontDeny ?
                             SIPMessage::Code415_UnsupportedMedia :
          (SIPMessage::StatusCodes)GetCallAnswerResponse() );
      return FALSE;
    }
  }

  return SendRequest( progress );
}

BOOL B2BUACall::SendB2BRedirect( 
  const SIPMessage & secondLegResponse 
)
{
  GCREF( "B2BUACall::SendB2BRedirect" );

  PAssert( secondLegResponse.Is3xx(), PLogicError );

  To to;
  m_CurrentUASInvite.GetTo( to );
  SIPMessage redirect;
  m_CurrentUASInvite.CreateResponse( redirect, secondLegResponse.GetStatusCode() );
  

  if( secondLegResponse.GetContactSize() == 0 )
    return FALSE;

  PString userName = secondLegResponse.GetTopContactURI().GetUser();
  if( userName.IsEmpty() )
    return FALSE;

  PString redirectionId;
  m_Connection->OnRequireRedirectionId( *this, redirectionId );

  PStringStream redirectUser;
  redirectUser << "rdir-" << redirectionId << "-" << userName;

  PIPSocket::Address localAddress, targetAddress;
  WORD localPort = 0, targetPort = 0;

  SIPTransport::Resolve( redirect, targetAddress, targetPort );
  GetSessionManager().GetTransportManager()->GetListenerAddress( 
    SIPTransport::UDP, targetAddress, localAddress, localPort );

  /// set the contact address
  SIPURI curi;
  curi.SetHost( localAddress.AsString() );
  curi.SetPort( PString( localPort ) );
  curi.SetUser( redirectUser );
  ContactURI contactURI;
  contactURI.SetURI( curi );
  Contact contactHeader;
  contactHeader.AddURI( contactURI );
  ContactList contactList;
  contactList.Append( contactHeader );
  redirect.SetContactList( contactList );
  
  return SendRequest( redirect );
}

BOOL B2BUACall::SendB2BRefer(
  B2BUACall & referror,
  const SIPMessage & refer 
)
{
  ReferTo referTo;
  if( !refer.GetReferTo( referTo ) )
    return FALSE;

  PString userName = referTo.GetURI().GetUser();
  if( userName.IsEmpty() )
    return FALSE;

  SIPMessage referClone;
  if( !CloneRequestWithinDialog( refer, referClone ) )
    return FALSE;

  PString xferId;
  m_Connection->OnRequireCallTransferId( referror, xferId );

  SIPURI referToURI = referTo.GetURI();
  
  PStringStream referToUser;
  referToUser << "xfer-" << xferId << "-" << referToURI.GetUser();
  referToURI.SetUser( referToUser );

  /// see if it already resolves to us
  if( !SIPTransport::IsLocalAddress( referToURI ) )
  {
    referToURI.SetHost( m_LocalVia.GetAddress() );
    referToURI.SetPort( PString( m_LocalVia.GetPort() ) );
  }

  PString repl;
  if( referToURI.GetParameter( "replaces", repl ) )
  {
    Replaces replaces = ParserTools::UnescapeAsRFC2396( repl );

    /// find the connection for this callId.
    PString callId = replaces.GetCallId() + "-connection";

    SIPSession::GCRef gcRef = GetSessionManager().FindGCRefByCallId( callId );

    if( gcRef != NULL )
    {
      B2BUAConnection * conn = dynamic_cast<B2BUAConnection *>( gcRef.GetObject() );
      if( conn != NULL )
      {
        if( conn->GetLeg2Call() != NULL )
        {
          replaces.SetCallId( conn->GetLeg2Call()->GetCallId() );
          referToURI.SetParameter( "replaces", replaces.EscapeAsRFC2396() );
        }
      }
    }
  }

  referTo.SetURI( referToURI );
  referClone.SetReferTo( referTo );
 
  return SendRequest( referClone );
}

BOOL B2BUACall::SendB2BNotify(
  B2BUACall & call,
  const SIPMessage & notify
)
{
  SIPMessage notifyClone;
  if( !CloneRequestWithinDialog( notify, notifyClone ) )
    return FALSE;
  SIPMessage ok;

  /// respond with 200
  notify.CreateResponse( ok, SIPMessage::Code200_Ok );
  SendRequest( ok );

  return SendRequest( notifyClone );
}

BOOL B2BUACall::SendB2BReject(
  const SIPMessage & secondLegResponse 
)
{
  GCREF( "B2BUACall::SendB2BReject" );

  if( secondLegResponse.GetStatusCode() == SIPMessage::Code407_ProxyAuthenticationRequired )
  {
    To to;
    m_CurrentUASInvite.GetTo( to );
    SIPMessage auth;
    m_CurrentUASInvite.CreateResponse( auth, SIPMessage::Code407_ProxyAuthenticationRequired );
    
    ProxyAuthenticate proxyAuth;
    if( secondLegResponse.GetProxyAuthenticate( proxyAuth ) )
      auth.SetProxyAuthenticate( proxyAuth );

    LOG_IF_DEBUG( LogDebugVeryHigh(), "*** REJECTED ***" << " INCOMING request " << endl << m_CurrentUASInvite << " with " << endl << auth );
    
    return SendRequest( auth );
  }else if( secondLegResponse.GetStatusCode() == SIPMessage::Code401_Unauthorized )
  {
    To to;
    m_CurrentUASInvite.GetTo( to );
    SIPMessage auth;
    m_CurrentUASInvite.CreateResponse( auth, SIPMessage::Code407_ProxyAuthenticationRequired );
    
    WWWAuthenticate wwwAuth;
    if( secondLegResponse.GetWWWAuthenticate( wwwAuth ) )
      auth.SetWWWAuthenticate( wwwAuth );

    LOG_IF_DEBUG( LogDebugVeryHigh(), "*** REJECTED ***" << " INCOMING request " << endl << m_CurrentUASInvite << " with " << endl << auth );
    
    return SendRequest( auth );
  }

  return SendReject( 
    m_CurrentUASInvite, 
    secondLegResponse.GetStatusCode() 
  );
}

void B2BUACall::AnswerB2BCall( 
  B2BUACall & secondLeg,
  const SIPMessage & secondLegResponse
)
{
  GCREF( "Auto" );

  if( GetLegIndex() != 0 )
    PAssertAlways( PLogicError );

  PWaitAndSignal lock( m_AnswerCallMutex );

  if( secondLegResponse.GetCSeqMethod() *= "INVITE" )
  {
    if( secondLegResponse.Is1xx() )
    {
      if( secondLegResponse.HasSDP() )
      {
        EnqueueMessageEvent( 
          IST_AnswerCallDeferredWithMedia, 
          secondLegResponse );
      }else
      {
        EnqueueMessageEvent( 
          IST_AnswerCallDeferred, 
          secondLegResponse );
      }
    }else if( secondLegResponse.Is2xx() )
    {
      EnqueueMessageEvent( 
        IST_AnswerCallNow, 
        secondLegResponse );
    }else if( secondLegResponse.Is3xx() )
    {
      EnqueueMessageEvent( 
        IST_AnswerCallRedirect, 
        secondLegResponse );
    }else if( !secondLegResponse.IsRequest() ) // 4xx - 6xx
    {
      EnqueueMessageEvent( 
        IST_AnswerCallDenied, 
        secondLegResponse );
    }
  }else
  {
    LOG( LogError(), "Got  a none-INVITE response while call is proceeding or alerting!!!\n" << secondLegResponse );

  }

  return;
}

BOOL B2BUACall::OnIncomingSDPOffer(
  const SIPMessage & sdp
)
{
  GCREF( "B2BUACall::AnswerB2BCall" );
  return m_Connection->OnIncomingSDPOffer( *this, sdp );
}



/// your last chance to produce an answer to the offer.  
/// This will be called either before sending 183, 200 or ACK 
BOOL B2BUACall::OnRequireSDPAnswer(
  const SIPMessage & offer,
  SIPMessage & answer
)
{
  GCREF( "B2BUACall::OnRequireSDPAnswer" );
  return m_Connection->OnRequireSDPAnswer( *this, offer, answer );
}

/// this will be called if an offer is not received in INVITE or when we are initiating the call
BOOL B2BUACall::OnRequireSDPOffer(
  SIPMessage & offer
)
{
  GCREF( "B2BUACall::OnRequireSDPOffer" );
  return m_Connection->OnRequireSDPOffer( *this, offer );
}

void B2BUACall::OnSessionTimerExpire()
{
  GCREF( "B2BUACall::OnSessionTimerExpire" );
  LOG( LogError(), "SIP Session Timer Expired!!!" );
  m_Connection->DestroyConnection(); 
}

/** IST CallBacks */
void B2BUACall::IST_OnReceivedInvite(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::IST_OnReceivedInvite" );
  PWaitAndSignal lock( m_AnswerCallMutex );

  if( GetState() == StateConnected )
  {
    IST_OnReceivedMoreInvite( msg );
    return;
  }

  SIPMessage response;

  m_CurrentUASInvite= msg;

  /// set the interface address to be used for future requests
  /// within the session;
  m_InterfaceAddress = msg.GetInterfaceAddress();
  m_InterfacePort = msg.GetInterfacePort();

  if( !OnIncomingConnection( msg ) )
  {
    LOG_IF_DEBUG( LogError(), "OnIncomingSession FAILED.  Rejecting incoming INVITE." );

    SetCallAnswerResponse( GetCallAnswerResponse() == DontDeny ?
      CallSession::DisconnectWithNotFound : GetCallAnswerResponse());
    
    m_Connection->DestroyConnection( msg );
    return;
  }

  /// IncomingConnection Allowed this call to process
  /// check for presence of SDP

  BOOL hasSDP = msg.HasSDP();
  PString sdpOffer;

  if( hasSDP )
  {
    PString sdp;
    msg.GetBody( sdp );
    sdpOffer = PString( sdp );
    if( !OnIncomingSDPOffer( msg ) )
    {
      LOG_IF_DEBUG( LogError(), "OnIncomingSDPOffer FAILED.  Rejecting incoming INVITE." );


      SetCallAnswerResponse( GetCallAnswerResponse() == DontDeny ?
        CallSession::DisconnectWithUnsupportedMediaType : GetCallAnswerResponse());
    
      m_Connection->DestroyConnection( msg );

      return;
    }
  }

  if( !OnSetupIncomingCall( msg ) )
  {
    LOG_IF_DEBUG( LogError(), "OnIncomingSession FAILED.  Rejecting incoming INVITE." );

    SetCallAnswerResponse( GetCallAnswerResponse() == DontDeny ?
      CallSession::DisconnectWithNotFound : GetCallAnswerResponse());
    
    m_Connection->DestroyConnection( msg );
    return;
  }
  
  /// We CAN'T answer the call at this point
  /// InternalAnswerCall( m_CallSessionManager.OnAnswerCall( msg, *this ), msg );

  m_Connection->EnqueueSessionEvent( new SIPSessionEvent( *m_Connection, B2BUAConnection::SetupOutbound, msg ) );
}

void B2BUACall::OnSetupOutbound(
  const SIPMessage & inboundInvite,
  B2BUAConnection & connection
)
{
  GCREF( "B2BUACall::OnSetupOutbound" );
  B2BRoutingInterface::B2BRouteList routes = connection.GetRoutes();

  if( routes.GetSize() == 0 )
  {
    connection.EnqueueSessionEvent( 
      new SIPSessionEvent( connection, B2BUAConnection::ErrorNoRoute ) );
    return;
  }

  PIPSocket::Address targetAddress;
  WORD targetPort;
  SIPURI uri( routes[0] );

  
  if( !SIPTransport::Resolve( uri, targetAddress, targetPort ) )
  {
    LOG_IF_DEBUG( LogWarning(), uri << " does not resolve to an IP Address" );
     connection.EnqueueSessionEvent( 
      new SIPSessionEvent( connection, B2BUAConnection::ErrorRouteResolution ) );
    return;
  }

  m_SessionProfile.SetRemoteURI( uri.AsString() );
  SIPMessage invite = inboundInvite;

  invite.RemoveAllRoutes();

  BOOL willUseProxy = FALSE;

  /// create the request line
  RequestLine requestLine;
  requestLine.SetMethod( "INVITE" );
  willUseProxy = !m_SessionProfile.GetProxyProfile().GetServerAddress().IsEmpty();
  requestLine.SetRequestURI( uri );
  invite.SetStartLine( requestLine );

  /// set the to uri;
  if( connection.WillRewriteToURI() )
  {
    To to;
    invite.GetTo( to );
    to.SetURI( uri );
    invite.SetTo( to );
  }
  
  if( willUseProxy )
  {
    /// put the proxy as the static route
    SIPURI sendAddress( m_SessionProfile.GetProxyProfile().GetServerAddress() );
    invite.SetSendAddress( sendAddress );
  }

  PString transport;
  if( !uri.GetParameter( "transport", transport ) )
    transport = "udp";

  
  Via via;
  GetSessionManager().ConstructVia( targetAddress, via, SIPTransport::UDP, connection.GetInterfacePort() );
  via.SetBranch( ParserTools::GenBranchParameter() );
  via.AddParameter( "rport", "" );
  invite.RemoveAllVias();
  invite.AppendVia( via );

  /// Set the inteface address to be used for future requests
  /// based it from the via host
  m_InterfaceAddress = via.GetAddress();
  m_InterfacePort = via.GetPort();

  /// Set the call Id
  CallId callId;
  callId.SetHeaderBody( GetSessionId() );
  invite.SetCallId( callId );

  /// Set the contact
  SIPURI contactURI;
  contactURI.SetHost( via.GetURI().GetHost() );
  contactURI.SetPort( via.GetURI().GetPort() );
  ContactURI contact( contactURI, m_SessionProfile.GetDisplayName() );
  invite.RemoveAllContact();
  invite.AppendContact( contact );

  OnRequireSDPOffer( invite );
  SetRequest( invite );
  m_CurrentUACInvite = invite;

  /// check if the route is to use an XOR Hash
  if( uri.GetParameter( "XORHASH" ) == "TRUE" )
  {
    invite.SetXORHash( TRUE );
    EnableXORHash();
  }

  invite.RemoveAllRecordRoutes();

  SendRequest( invite );
}

void B2BUACall::OnSessionKeepAliveTimeout()
{
  m_Connection->OnSessionKeepAliveTimeout( *this );
}


void B2BUACall::IST_OnReceivedMoreInvite(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::IST_OnReceivedMoreInvite" );
  CallSession::IST_OnReceivedMoreInvite( msg );
  m_Connection->IST_OnReceivedMoreInvite( *this, msg );
}

void B2BUACall::IST_OnReceivedAck(
  const SIPMessage & msg
)
{
  CallSession::IST_OnReceivedAck( msg );
}

void B2BUACall::IST_OnReceivedUnknown(
  const SIPMessage & msg
)
{
  CallSession::IST_OnReceivedUnknown( msg );
}

/** NIST Callbacks */
void B2BUACall::NIST_OnReceivedRegister(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NIST_OnReceivedRegister" );
  CallSession::NIST_OnReceivedRegister( msg );
  m_Connection->NIST_OnReceivedRegister( *this, msg );
}

void B2BUACall::NIST_OnReceivedRefer(
  const SIPMessage & msg
)
{
  /// don't call CallSession::NIST_OnReceivedRefer() here
  /// we want a pure pass through behavior for REFER
  GCREF( "B2BUACall::NIST_OnReceivedRefer" );
  m_Connection->NIST_OnReceivedRefer( *this, msg );
}

void B2BUACall::NIST_OnReceivedBye(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NIST_OnReceivedBye" );
  CallSession::NIST_OnReceivedBye( msg );
  m_Connection->NIST_OnReceivedBye( *this, msg );
}

void B2BUACall::NIST_OnReceivedOptions(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NIST_OnReceivedOptions" );
  CallSession::NIST_OnReceivedOptions( msg );
  m_Connection->NIST_OnReceivedOptions( *this, msg );
}

void B2BUACall::NIST_OnReceivedInfo(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NIST_OnReceivedInfo" );
  m_Connection->NIST_OnReceivedInfo( *this, msg );
}

void B2BUACall::NIST_OnReceivedMessage(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NIST_OnReceivedMessage" );
  m_Connection->NIST_OnReceivedMessage( *this, msg );
}

void B2BUACall::NIST_OnReceivedCancel(
  const SIPMessage & msg
)
{
  GCREF( "NIST_OnReceivedCancel" );
  CallSession::NIST_OnReceivedCancel( msg );
  m_Connection->NIST_OnReceivedCancel( *this, msg );
}

void B2BUACall::NIST_OnReceivedNotify(
  const SIPMessage & msg
)
{
  GCREF( "NIST_OnReceivedNotify" );
  CallSession::NIST_OnReceivedNotify( msg );
  m_Connection->NIST_OnReceivedNotify( *this, msg );
}

void B2BUACall::NIST_OnReceivedSubscribe(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NIST_OnReceivedSubscribe" );
  CallSession::NIST_OnReceivedSubscribe( msg );
  m_Connection->NIST_OnReceivedSubscribe( *this, msg );
}

void B2BUACall::NIST_OnReceivedUnknown(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NIST_OnReceivedUnknown" );
  CallSession::NIST_OnReceivedUnknown( msg );
  m_Connection->NIST_OnReceivedUnknown( *this, msg );
}

/** NICT CALLBACKS */
void B2BUACall::NICT_OnReceived1xx(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NICT_OnReceived1xx" );
  CallSession::NICT_OnReceived1xx( msg );
  m_Connection->NICT_OnReceived1xx( *this, msg );
}

void B2BUACall::NICT_OnReceived2xx(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NICT_OnReceived2xx" );
  CallSession::NICT_OnReceived2xx( msg );
  m_Connection->NICT_OnReceived2xx( *this, msg );
}

void B2BUACall::NICT_OnReceived3xx(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NICT_OnReceived3xx" );
  CallSession::NICT_OnReceived3xx( msg );
  m_Connection->NICT_OnReceived3xx( *this, msg );
}

void B2BUACall::NICT_OnReceived4xx(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NICT_OnReceived4xx" );
  CallSession::NICT_OnReceived4xx( msg );
  m_Connection->NICT_OnReceived4xx( *this, msg );
}

void B2BUACall::NICT_OnReceived5xx(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NICT_OnReceived5xx" );
  CallSession::NICT_OnReceived5xx( msg );
  m_Connection->NICT_OnReceived5xx( *this, msg );
}

void B2BUACall::NICT_OnReceived6xx(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NICT_OnReceived6xx" );
  CallSession::NICT_OnReceived6xx( msg );
  m_Connection->NICT_OnReceived6xx( *this, msg );
}

void B2BUACall::NICT_OnReceivedUnknown(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::NICT_OnReceivedUnknown" );
  CallSession::NICT_OnReceivedUnknown( msg );
  m_Connection->NICT_OnReceivedUnknown( *this, msg );
}

/** ICT CALLBACKS */
void B2BUACall::ICT_OnReceived1xx(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::ICT_OnReceived1xx" );
  CallSession::ICT_OnReceived1xx( msg );

  if( m_Connection->IsForkCalls() )
  {
    B2BUAForkCalls * forkCalls = m_Connection->GetForkCalls();
    if( forkCalls )
    {
      if( !forkCalls->OnReceived1xxResponse( *this, msg ) )
        return;
    }
  }

  m_Connection->ICT_OnReceived1xx( *this, msg );
}

void B2BUACall::ICT_OnReceived2xx(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::ICT_OnReceived2xx" );
  CallSession::ICT_OnReceived2xx( msg );

  if( m_Connection->IsForkCalls() )
  {
    B2BUAForkCalls * forkCalls = m_Connection->GetForkCalls();
    if( forkCalls )
    {
      if( !forkCalls->OnReceived2xxResponse( *this, msg ) )
        return;
    }
  }

  m_Connection->ICT_OnReceived2xx( *this, msg );
}

void B2BUACall::ICT_OnReceived3xx(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::ICT_OnReceived3xx" );

  if( m_Connection->IsForkCalls() )
  {
    B2BUAForkCalls * forkCalls = m_Connection->GetForkCalls();
    if( forkCalls )
    {
      if( forkCalls->OnReceivedErrorResponses( *this, msg ) )
        return;
    }
  }

  m_Connection->ICT_OnReceived3xx( *this, msg );
  // the default behaviour would try to process the redirect internally
  CallSession::ICT_OnReceived3xx( msg );


}

void B2BUACall::ICT_OnReceived4xx(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::ICT_OnReceived4xx" );
  CallSession::ICT_OnReceived4xx( msg );

  if( m_Connection->IsForkCalls() )
  {
    B2BUAForkCalls * forkCalls = m_Connection->GetForkCalls();
    if( forkCalls )
    {
      forkCalls->OnReceivedErrorResponses( *this, msg );
      return;
    }
  }

  m_Connection->ICT_OnReceived4xx( *this, msg );
}

void B2BUACall::ICT_OnReceived5xx(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::ICT_OnReceived5xx" );
  CallSession::ICT_OnReceived5xx( msg );

  if( m_Connection->IsForkCalls() )
  {
    B2BUAForkCalls * forkCalls = m_Connection->GetForkCalls();
    if( forkCalls )
    {
      forkCalls->OnReceivedErrorResponses( *this, msg );
      return;
    }
  }

  m_Connection->ICT_OnReceived5xx( *this, msg );
}

void B2BUACall::ICT_OnReceived6xx(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::ICT_OnReceived6xx" );
  CallSession::ICT_OnReceived6xx( msg );

  if( m_Connection->IsForkCalls() )
  {
    B2BUAForkCalls * forkCalls = m_Connection->GetForkCalls();
    if( forkCalls )
    {
      forkCalls->OnReceivedErrorResponses( *this, msg );
      return;
    }
  }

  m_Connection->ICT_OnReceived6xx( *this, msg );
}

void B2BUACall::ICT_OnReceivedUnknown(
  const SIPMessage & msg
)
{
  GCREF( "B2BUACall::ICT_OnReceivedUnknown" );
  CallSession::ICT_OnReceivedUnknown( msg );
  m_Connection->ICT_OnReceivedUnknown( *this, msg );
}




