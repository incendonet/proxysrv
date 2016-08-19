/*
 *
 * CallSessionManager.cxx
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
 * $Log: CallSessionManager.cxx,v $
 * Revision 1.49  2006/12/21 05:47:48  joegenbaclor
 * Changed response code from 405 to 200 for BYEs outside of a dialog
 *
 * Revision 1.48  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.47  2006/09/25 16:34:07  joegenbaclor
 * Corrected bug in 200 ok retransmission thread where packets may be sent unhashed for XOR enabled calls
 *
 * Revision 1.46  2006/09/07 04:25:26  joegenbaclor
 * Added callback for 200 Ok retransmission timeout
 *
 * Revision 1.45  2006/09/02 17:58:20  joegenbaclor
 * Corrected compile errors in GCC regarding euqation of NULL to GCRef.
 *
 * Revision 1.44  2006/09/01 13:20:57  rcolobong
 * Provide Setter for Expiry in IST200OkRetransmission
 *
 * Revision 1.43  2006/08/30 03:42:05  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.42  2006/08/29 10:47:23  rcolobong
 * *** empty log message ***
 *
 * Revision 1.41  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.40  2006/08/26 14:31:44  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.39  2006/08/24 10:19:14  joegenbaclor
 * 1.  Couple of bug fixes in SIP Intitializarion and Disconnect cause detection
 * 2.  Added ProxyAuthentication call backs to Register Session
 *
 * Revision 1.38  2006/08/18 07:12:24  joegenbaclor
 * 1.  added Function overload for GetContentType
 * 2.  Changed unix make files to not include opensbc in make process
 *
 * Revision 1.37  2006/08/15 15:42:09  joegenbaclor
 * 1. Added Instant Messaging functions to OPAL
 *
 * Revision 1.36  2006/08/11 09:30:51  joegenbaclor
 * Corrected several compile warnings in GCC
 *
 * Revision 1.35  2006/08/09 12:17:48  joegenbaclor
 * Added RFC 3428 Support
 *
 * Revision 1.34  2006/07/19 02:19:21  joegenbaclor
 * 1.  Bug fixes concerning B2BUA destruction during during 3xx, 4xx, 5xx, 6xx local response
 * 2.  Fixed bug where Route header is never removed from outbound Invite resulting that may result to a loop when
 *      Inbound Invite has a Route header present
 * 3.  Added RemoveAllRoutes() in SIPMessage
 *
 * Revision 1.33  2006/07/11 13:59:36  joegenbaclor
 * Completed Proxy functionalities for B2BUA
 *
 * Revision 1.32  2006/06/28 14:15:00  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.31  2006/06/22 08:27:57  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.30  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.29  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.28  2006/04/29 03:22:54  joegenbaclor
 * 1.  More Refer support code added to call session
 * 2.  Added StoreRegistrationInfo callback to Registrar classes
 *
 * Revision 1.27  2006/04/26 01:00:11  joegenbaclor
 * Added REFER NIST callbacks
 *
 * Revision 1.26  2006/04/20 00:47:28  joegenbaclor
 * 1.  Update ossbuildopts.h to include macros for RTP
 * 2.  More preliminary design work on SIPRTPBridge classes
 * 3.  Separated RTPFrame classes from SIPRTPBridge files
 *
 * Revision 1.25  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.24  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.23  2006/03/23 05:37:58  joegenbaclor
 * Got rid of some compile warning in Visual Studio 2003
 *
 * Revision 1.22  2006/03/23 02:17:40  joegenbaclor
 * Corrected Linux compile errors
 *
 * Revision 1.21  2006/03/17 13:51:00  joegenbaclor
 * 1.  Implemented UAC Call Redirection
 * 2.  Implemented SendCancel() method in CallSession
 * 3.  Introduced ClearInternalHeaders() function in SIP Message to erase internal headers inheritted from previous transaction.
 *
 * Revision 1.20  2006/03/15 16:40:46  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.19  2006/03/14 12:55:22  joegenbaclor
 * Removed Separate SIPClassese for simplicity.
 * Instead we will implement dialog support directly in SIPSession.
 *
 * Revision 1.18  2006/03/14 05:17:12  joegenbaclor
 * Implemented CallSessionManager::OnRequireSDPOffer() to generate an interim Serssion Description
 *
 * Revision 1.17  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "CallSessionManager.h"
#include "ossbuildopts.h"

using namespace UACORE;

#define new PNEW

///////////////////////////////////////////////////////////////////////////////

CallSessionManager::IST200OkRetransmission::IST200OkRetransmission(
  const SIPMessage & msg,
  const PString & sessionId,
  CallSessionManager & manager
) : m_CallSessionManager( manager )
{
  m_200Ok = msg;
  m_200Ok.SetXORHash( msg.IsXORHashSet() );
  m_IsExpired = FALSE;
  m_TimerValue = SIP_TIMER_T1;  /// ms
  m_RetransmitCount = 0;
  m_SessionId = sessionId;

  PString cid = msg.GetCallId();
  PString toTag = msg.GetToTag();
  PString fromTag = msg.GetFromTag();
  int cseqNo = msg.GetCSeqNumber();

  PStringStream r_id;
  r_id << cid << "|" << toTag << "|" << fromTag << "|" << cseqNo << "|ACK";

  m_RetransmissionId = r_id;
}

CallSessionManager::IST200OkRetransmission::~IST200OkRetransmission()
{
}


void CallSessionManager::IST200OkRetransmission::Retransmit()
{
  PWaitAndSignal lock( m_LocalMutex );

  if( m_IsExpired )
    return;

  PTimeInterval interval;
  interval = PTime() - m_StartTime;

  if( interval.GetMilliSeconds() >= m_TimerValue )
  {
    ++m_RetransmitCount;
    if( m_RetransmitCount < 7 )
    {
      /// clear the internal headers from previous retransmission
      m_200Ok.ClearInternalHeaders();
      m_CallSessionManager.GetUserAgent().SendRequest( m_200Ok );
      m_TimerValue = m_TimerValue * 2;
      if( m_TimerValue > SIP_TIMER_T2 )
        m_TimerValue = SIP_TIMER_T2;
    }

    ///Timer H  64*T1            Section 17.2.1       Wait time for ACK receipt
    /// only retransmit 6 times (500, 1000, 2000, 4000, 4000, 4000, 500) total = 16000 MS
    if( m_RetransmitCount == 6 )
      m_TimerValue = 500;  /// wait 500 more ms before we expire post an expire
    else if( m_RetransmitCount == 7 )
    {
      PostExpireEvent();
      SIPSession::GCRef gcRef;
      CallSession * session = m_CallSessionManager.FindGCRefBySessionId<CallSession>( m_SessionId, gcRef ); 
      if( session != NULL )
      {
        session->OnAckFor200OkExpire();
      }
    }
  }
}

void CallSessionManager::IST200OkRetransmission::PostExpireEvent()
{
  PWaitAndSignal lock( m_LocalMutex );
  m_IsExpired = TRUE;
  RetransmitThread * thrd = (RetransmitThread *)m_CallSessionManager.GetRetransmitThread();
  thrd->Expire200Ok( GetRetransmissionId() );
}


BOOL CallSessionManager::IST200OkRetransmission::IsExpired()
{
  return m_IsExpired;
}

////////////////////////////////////////////////////////////////


CallSessionManager::RetransmitThread::RetransmitThread( CallSessionManager & manager )
  :  PThread(
      10000,
      PThread::NoAutoDeleteThread,
      PThread::NormalPriority,
      "IST 200 Ok Retransmission" ),
     m_CallSessionManager( manager )
{
  m_ExitFlag = FALSE;
  Resume();
}

CallSessionManager::RetransmitThread::~RetransmitThread()
{
  m_ExitFlag = TRUE;
  m_SyncPoint.Signal();
  WaitForTermination();
}

void CallSessionManager::RetransmitThread::Expire200Ok(
  const SIPMessage & ack
)
{
  PString cid = ack.GetCallId();
  PString toTag = ack.GetToTag();
  PString fromTag = ack.GetFromTag();
  int cseqNo = ack.GetCSeqNumber();

  PStringStream r_id;
  r_id << cid << "|" << toTag << "|" << fromTag << "|" << cseqNo << "|ACK";

  Expire200Ok( r_id );
}

void CallSessionManager::RetransmitThread::Expire200Ok(
  const PString & id
)
{
  m_200OkRetransmissionMutex.Wait();
  m_200OkToBeCleaned+=id;
  m_200OkRetransmissionMutex.Signal();
  m_SyncPoint.Signal();
}

void CallSessionManager::RetransmitThread::AddMessage(
  const PString & sessionId,
  const SIPMessage & msg
)
{
  IST200OkRetransmission * retran = new IST200OkRetransmission( msg, sessionId, m_CallSessionManager );
  m_200OkRetransmissionMutex.Wait();
  PString currentId = retran->GetRetransmissionId();
  PTRACE( 4, "Adding 200 OK retransmission for " << currentId );
  m_200OkRetransmissionQueue.SetAt( currentId, retran );
  m_200OkRetransmissionMutex.Signal();
  m_SyncPoint.Signal();
}

void CallSessionManager::RetransmitThread::Handle200OkRetransmission()
{
  PWaitAndSignal lock( m_200OkRetransmissionMutex );

  while( m_200OkToBeCleaned.GetSize()  != 0 )
  {
    
    PString currentId = m_200OkToBeCleaned.GetKeyAt( 0 );

    if( Has200OkMessage( currentId ) )
      PTRACE( 4, "Stopping 200 OK retransmission for " << currentId );
    else
      PTRACE( 4, "No corresponding 200 ok retransmission for " << currentId );

    IST200OkRetransmission* retran = m_200OkRetransmissionQueue.GetAt( currentId );
    if( retran )
      retran->SetExpired();

    m_200OkRetransmissionQueue.RemoveAt( currentId );
    m_200OkToBeCleaned-=currentId;
  }

  for( PINDEX i = 0; i < m_200OkRetransmissionQueue.GetSize(); i++ )
  {
    /// this is where we retransmit
     m_200OkRetransmissionQueue.GetDataAt( i ).Retransmit();
  }
}

void CallSessionManager::RetransmitThread::Main()
{
  while( TRUE )
  {
    m_SyncPoint.Wait( SIP_TIMER_T1 ); /** minimm retransmission time */

    if( m_ExitFlag )
      return;

    Handle200OkRetransmission();
  }
}

BOOL CallSessionManager::RetransmitThread::Has200OkMessage( const PString & id )const
{
  PWaitAndSignal lock( m_200OkRetransmissionMutex );
  return m_200OkRetransmissionQueue.Contains( id );
}

////////////////////////////////////////////////////////////////

CallSessionManager::CallSessionManager(
  SIPUserAgent & ua,
  PINDEX sessionThreadCount,
  PINDEX stackSize
) : SIPSessionManager( ua, sessionThreadCount, stackSize, "Call" )
{
  m_200OkRetransmitThread = new RetransmitThread( *this );
}

CallSessionManager::~CallSessionManager()
{
  if( m_200OkRetransmitThread != NULL )
    delete m_200OkRetransmitThread;
}

SIPSession::GCRef CallSessionManager::MakeCall(
  SIPURI & remoteURI,
  ProfileUA * profile
)
{
  PString callId = ParserTools::GenGUID();
  ProfileUA defaultProfile;
  if( profile != NULL )
    defaultProfile = *profile;
  else
    defaultProfile = GetUserAgent().GetDefaultProfile();

  CallSession * callSession = (CallSession*)CreateClientSession( defaultProfile, callId );

  if( callSession == NULL )
    return SIPSession::GCRef( NULL, "SIPSession" );

  SIPSession::GCRef call = callSession->GCCreateRef();

  if( !callSession->MakeCall( remoteURI ) )
  {
    callSession->Destroy();
    return SIPSession::GCRef( NULL, "SIPSession" );
  }

  return call;
}

/// will be called by the call session before calling TransferCallAsUAS.
/// returning FALSE here will result to the rejection of the transfer request
BOOL CallSessionManager::OnCallBeingTransfered(
  CallSession & callToTransfer,
  const SIPMessage & refer
)
{
  return TRUE;
}

/// will be called by a CallSession after receipt of a REFER.
/// this method should NOT be called from outside the call session object
/// returns a smart pointer to the new call
SIPSession::GCRef CallSessionManager::TransferCallAsUAS(
  CallSession & callToTransfer,
  const SIPMessage & refer
)
{
  PString callId = ParserTools::GenGUID();
  ProfileUA defaultProfile =  callToTransfer.GetSessionProfile();
  CallSession * callSession = (CallSession*)CreateClientSession( defaultProfile, callId );

  if( callSession == NULL )
    return SIPSession::GCRef( NULL, "SIPSession" );

  SIPSession::GCRef call = callSession->GCCreateRef();
  callSession->SetReferredCall();
  
  if( !callSession->MakeCallTransfer( callToTransfer, refer ) )
  {
    callSession->Destroy();
    return SIPSession::GCRef( NULL, "SIPSession" );
  }

  callToTransfer.SetReferrerCall();

  /// from here onwards  callSession will take responsibility 
  /// on when to destroy callToTransfer
  callSession->SetTransferedCall( &callToTransfer );

  return call;
}

/// will send a REFER to UAS and ask it to transfer the 
/// call to a remote URI. Returns FALSE if send fails
BOOL CallSessionManager::TransferCallAsUAC(
  CallSession & callToTransfer,
  SIPURI & remoteURI
)
{
  PWaitAndSignal lock( m_SessionsMutex );
  ReferTo referTo( remoteURI );
  return callToTransfer.SendRefer( referTo );
}

SIPSession::GCRef CallSessionManager::OnCreateServerSession(
  const SIPMessage & request
)
{
  PWaitAndSignal lock( m_SessionsMutex );

  if( !request.IsInvite() )
  {
    LOG_IF_DEBUG( LogWarning(), "CallSessionManager::OnCreateServerSession() got a None-INVITE request" );
    LOG_IF_DEBUG( LogWarning(), "CallSessionManager::OnCreateServerSession() returning NULL" );
    return SIPSession::GCRef( NULL, "SIPSession" );
  }

  CallId callId;
  request.GetCallId( callId );
  PString sessionId = callId.GetHeaderBody();

  CallSession * callSession = OnCreateServerCallSession( request, sessionId );
  return callSession->GCCreateRef( "CallSessionManager::OnCreateServerSession" );
}

CallSession * CallSessionManager::OnCreateServerCallSession(
  const SIPMessage & request,
  const PString & sessionId
)
{
  return new CallSession( *this, request, sessionId );
}

SIPSession * CallSessionManager::OnCreateClientSession(
  const ProfileUA & profile,
  const PString & sessionId
)
{
  PWaitAndSignal lock( m_SessionsMutex );
  return OnCreateClientCallSession( profile, sessionId );
}

CallSession * CallSessionManager::OnCreateClientCallSession(
  const ProfileUA & profile,
  const PString & sessionId
)
{
  return new CallSession( *this, sessionId, profile  );
}

void CallSessionManager::OnUnknownTransaction(
  SIPUnknownTransaction & event
)
{
  const SIPMessage & msg = event.GetMessage();
  /// find a session

  SIPSession::GCRef ref = FindGCRefByCallId( msg.GetCallId() );
  if( ref == NULL )
    return;

  CallSession * session = (CallSession *)ref.GetObject();

  CSeq cseq;
  msg.GetCSeq( cseq );
  PString method = cseq.GetMethod();

  if( event.IsFromTransport() )
  {
    if( msg.IsAck() )
    {
      session->IST_OnReceivedAck( msg );
    }else if( msg.Is2xx() )
    {
      /// this is a 200 ok retransmision
      CSeq cseq;
      msg.GetCSeq( cseq );
      if( cseq.GetMethod() *= "INVITE" )
        session->ICT_OnReceived2xx( msg );
    }
  }else
  {
    /// its not from transport.  Check if its 200 Ok retransmision
    if( msg.Is2xx() )
    {
      if( method *= "INVITE" )
      {
        session->IST_On2xxSent( msg );
      }
    }
  }
}

/// returning FALSE here will automatically reject the incoming call
BOOL CallSessionManager::OnIncomingConnection(
  const SIPMessage & invite,
  CallSession & session
)
{
  return TRUE;
}

BOOL CallSessionManager::OnSetupIncomingCall(
  const SIPMessage & invite,
  CallSession & session
)
{
  return TRUE;
}

BOOL CallSessionManager::OnReinvite(
  const SIPMessage & invite,
  CallSession & session
)
{
  return TRUE;
}

/// returning FALSE here will reject the call with no acceptable format
/// depending on call state.  Offers received in ACK will be disconnected
/// using a BYE.
BOOL CallSessionManager::OnIncomingSDPOffer(
  const SIPMessage & sdp,
  CallSession & session
)
{
  /// Media UA should implement this method
  return TRUE;
}

/// your last chance to produce an answer to the offer.
/// This will be called either before sending 183, 200 or ACK.
/// AnsweCall is therefore a prerequisite before this callback
/// is called
BOOL CallSessionManager::OnRequireSDPAnswer(
  const SIPMessage & offer,
  SIPMessage & answer,
  CallSession & session
)
{
  /// Media UA should implement this method
  return FALSE;
}

BOOL CallSessionManager::OnRequireSDPOffer(
  SIPMessage & _offer,
  CallSession & session
)
{
  /// Media UA should implement this method
  return FALSE;
}


/// this is where implementors may modify or add headers before underlying implementation
/// sends the actual response to INVITE to the FSM
BOOL CallSessionManager::OnSendingResponseToInvite(
  SIPMessage & response,
  CallSession & session
)
{
  return TRUE;
}

/// this is where implementors may modify or add headers before underlying implementation
/// sends the actual response messages to the FSM for none-invite transactions
BOOL CallSessionManager::OnSendingResponseToNoneInvite(
  SIPMessage & response,
  CallSession & session
)
{
  return TRUE;
}


/// answers the call.  based on the mode.  May be called several times
BOOL CallSessionManager::AnswerCall(
  const PString & sessionId,
  CallSession::AnswerCallResponse mode
)
{

  SIPSession::GCRef autoRef( "CallSessionManager::AnswerCall" );
  CallSession * session = (CallSession*)FindGCRefBySessionId<CallSession>( sessionId, autoRef );

  if( session == NULL )
  {
    return FALSE;
  }

  return session->AnswerCall( mode );
}

/// this is where implementors would populate the response to AnswerCall.
/// It would be the responsibility of the implementatio to override this
CallSession::AnswerCallResponse CallSessionManager::OnAnswerCall(
  const SIPMessage & invite,
  CallSession & session
)
{
  return CallSession::NumAnswerCall;
}

/// will be called if a rediration URI is needed by OnAnswerCall
BOOL CallSessionManager::OnRequireRedirectionURI(
  ContactList & contacts,
  CallSession & session,
  BOOL isProxyRedirect
)
{
  return FALSE;
}

/// will be called when the call is established
void CallSessionManager::OnConnected(
  CallSession & session,
  const SIPMessage & msg
)
{
}

/// will be called when the call is disconnected
void CallSessionManager::OnDisconnected(
  CallSession & session,
  const SIPMessage & message
)
{
}

void CallSessionManager::OnAlerting(
  CallSession & session,
  const SIPMessage & message
)
{
}

void  CallSessionManager::OnProgress(
  CallSession & session,
  const SIPMessage & message
)
{
}

BOOL CallSessionManager::OnCallRedirect(
  CallSession & session,
  const SIPMessage & redirect
)
{
  return TRUE;
}

// will be called when the REFER is responded to by a 202 or 200
void CallSessionManager::OnReferAccepted(
  CallSession & session
)
{
}

/// will be called if the refer is rejected by the UAS using 3xx-6xx response
void CallSessionManager::OnReferRejected(
  CallSession & session,
  const SIPMessage & reject
)
{
}

void CallSessionManager::OnReferSubscriptionState(
  CallSession & session,
  const Event & event,
  const SubscriptionState & state,
  const PString sipFrag
)
{
}

void CallSessionManager::OnOrphanedMessage(
  const SIPMessage & message
)
{
  if( message.IsRequest() )
  {
    if( message.IsOptions() )
    {
      OnReceivedOptions( message );
    }else if( message.IsMessage() )
    {
      InternalOnReceivedInstantMessage( message );
    }else
    {
      SIPMessage response;

      if( !message.IsInvite() )
      {
        if( message.IsBye() )
        {
          message.CreateResponse( response, SIPMessage::Code200_Ok );
          SendRequest( response );
        }else
        {
          message.CreateResponse( response, SIPMessage::Code405_MethodNotAllowed );
          SendRequest( response );
        }
      }
    }
  }
}

void CallSessionManager::OnReceivedOptions(
  const SIPMessage & options
)
{
  SIPMessage ok;
  options.CreateResponse( ok, SIPMessage::Code200_Ok );

  Allow allow;
  allow.AddEntry( "INVITE" );
  allow.AddEntry( "ACK" );
  allow.AddEntry( "CANCEL" );
  allow.AddEntry( "OPTIONS" );
  allow.AddEntry( "BYE" );
  allow.AddEntry( "REFER" );
  allow.AddEntry( "SUBSCRIBE" );
  allow.AddEntry( "NOTIFY" );
  ok.AppendAllow( allow );

  SendRequest( ok );
}

void CallSessionManager::InternalOnReceivedInstantMessage(
  const SIPMessage & message
)
{
  SIPMessage ok;
  message.CreateResponse( ok, SIPMessage::Code200_Ok );
  SendRequest( ok );
  EnqueueSessionManagerEvent( new SIPMessage( message ) );
}

void CallSessionManager::OnReceivedInstantMessage(
  const SIPMessage & message
)
{
}

void CallSessionManager::SendInstantMessage(
  const SIPURI & uri,
  const PString & body
)
{
  ProfileUA profile = GetUserAgent().GetDefaultProfile();
  PIPSocket::Address targetAddress = uri.GetAddress();
  /*WORD targetPort;*/

  SIPMessage request;
  BOOL willUseProxy = FALSE;

  /// create the request line
  RequestLine requestLine;
  requestLine.SetMethod( "MESSAGE" );

  PString requestURI = uri.AsString();
  willUseProxy = !profile.GetProxyProfile().GetServerAddress().IsEmpty();
  requestLine.SetRequestURI( requestURI );
  request.SetStartLine( requestLine );

  if( willUseProxy )
  {
    /// put the proxy as the static route
    SIPURI sendAddress( profile.GetProxyProfile().GetServerAddress() );
    request.SetSendAddress( sendAddress );
  }

  PString transport;
  if( !uri.GetParameter( "transport", transport ) )
    transport = "udp";

  Via via;
  ConstructVia( targetAddress, via, SIPTransport::UDP );
  
  via.SetBranch( ParserTools::GenBranchParameter() );
  via.AddParameter( "rport", "" );
  request.AppendVia( via );

  /// Set From header
  From from;
  PStringStream userURI; 

  if( !profile.GetDisplayName().IsEmpty() )
    from.SetDisplayName( profile.GetDisplayName() );

  PString userName = profile.GetUser();
  
  if( !userName.IsEmpty() )
    userURI << userName << "@"; 

  PString localHost;

  if( willUseProxy )
  {
    localHost = profile.GetProxyProfile().GetDomain();
    if( localHost.IsEmpty() )
    {
      ///get the domain from the proxy address
      SIPURI uri( profile.GetRegistrationProfile().GetServerAddress() );
      localHost = uri.GetHost();
    }
  }

  if( localHost.IsEmpty() )
  {
    localHost = via.GetAddress();
  }

  userURI << localHost;

  SIPURI fromURI( userURI );
  from.SetURI( fromURI );
  from.AddParameter( "tag", ParserTools::GenTagParameter() );

  request.SetFrom( from );

  /// Set To header
  To to;
  to.SetURI( uri );
  request.SetTo( to );

  /// Set the call Id
  CallId callId;
  callId.SetHeaderBody( ParserTools::GenGUID() );
  request.SetCallId( callId );

  /// Set the CSeq
  CSeq cSeq;
  cSeq.SetMethod( "MESSAGE" );
  cSeq.SetSequence( 4711 );
  request.SetCSeq( cSeq );

  request.SetContentType( "text/plain" );
  request.SetBody( body );

  LOG_IF_DEBUG( LogDebugVeryHigh(), request );

  SendRequest( request );
}

void CallSessionManager::OnSessionManagerEvent(
  PObject * eventObject
)
{
  if( eventObject == NULL )
    return;
  if( PIsDescendant( eventObject, SIPMessage ) )
  {
    SIPMessage * msg = static_cast<SIPMessage*>(eventObject);
    if( msg->IsMessage() )
      OnReceivedInstantMessage( *msg );
  }

  delete eventObject;
}





