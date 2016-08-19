/*
 *
 * CallSession.cxx
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
 * $Log: CallSession.cxx,v $
 * Revision 1.80  2007/01/17 00:12:19  joegenbaclor
 * 1.  Fixed bug in call session where CANCEL may not be sent if a call ahs been authenticated due to a misplaced SetState( StateDisconnected ) for 4xx events
 * 2.  Fixed register session to ignore authorization headers if allow all reg is flagged
 * 3.  Temporary hacks for syslog
 * 4.  More launcher code -  almost there!
 *
 * Revision 1.79  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.78  2006/11/30 00:51:14  joegenbaclor
 * 1.  Corrected bug in authorization construction where the URI can have a double port concatenation.
 * 2.  Modified AsString() method of SIPURI to allow not returning the URI parameters.
 * 3.  Added GetBasicURI method
 *
 * Revision 1.77  2006/11/25 03:58:38  joegenbaclor
 * Corrected bug where reINVITE where proxy authentication may by-pass outbound proxy as reported by Klaus Darilion
 *
 * Revision 1.76  2006/11/21 06:55:19  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.75  2006/11/08 06:46:20  joegenbaclor
 * Added session level NIC/Interface binding
 *
 * Revision 1.74  2006/11/07 07:50:32  joegenbaclor
 * Corrected bug in Session timer processing.
 *
 * Revision 1.73  2006/10/30 02:42:27  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.72  2006/10/23 15:44:58  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.71  2006/10/11 04:49:47  rcolobong
 * 1. Add support for Notify During Refer
 * 2. Fixed bug where 3xx invoke CallHandler OnRejected
 *
 * Revision 1.70  2006/10/03 04:28:45  joegenbaclor
 * Added INFO DTMF relay support
 *
 * Revision 1.69  2006/09/25 17:56:33  joegenbaclor
 * Corrected typos in last commit
 *
 * Revision 1.68  2006/09/25 16:34:07  joegenbaclor
 * Corrected bug in 200 ok retransmission thread where packets may be sent unhashed for XOR enabled calls
 *
 * Revision 1.67  2006/09/25 04:43:30  rcolobong
 * 1. Fix bug in 415 Unsupported Media where CallSession::m_ICTState is not properly initialized during construction.
 * 2. Remove the temporary fix for the 415 Unsupported Media where we use Semaphore to wait for second leg SDP in OnRequireSDPAnswer
 *
 * Revision 1.66  2006/09/18 09:35:16  joegenbaclor
 * Completed initial implementation of RTP media aggregation
 *
 * Revision 1.65  2006/09/13 10:36:25  joegenbaclor
 * Added Release-Minimal build for smaller DLL footprint for ATLSIP
 *
 * Revision 1.64  2006/09/08 04:39:17  joegenbaclor
 * Implemented INFO Session Keep-alive
 *
 * Revision 1.63  2006/09/07 09:49:58  joegenbaclor
 * Fixed bug on possible race conditions when multiple thread calls Session::Destroy
 *  at the same time
 *
 * Revision 1.62  2006/09/07 04:25:26  joegenbaclor
 * Added callback for 200 Ok retransmission timeout
 *
 * Revision 1.61  2006/08/30 07:48:56  joegenbaclor
 * Implemented OntimerExpire
 *
 * Revision 1.60  2006/08/30 03:42:05  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.59  2006/08/29 10:47:23  rcolobong
 * *** empty log message ***
 *
 * Revision 1.58  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.57  2006/08/26 14:31:44  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.56  2006/08/24 10:19:13  joegenbaclor
 * 1.  Couple of bug fixes in SIP Intitializarion and Disconnect cause detection
 * 2.  Added ProxyAuthentication call backs to Register Session
 *
 * Revision 1.55  2006/08/23 03:33:54  joegenbaclor
 * Some Opal related bug fixes discovered duriong intial development of ATLSIP
 *
 * Revision 1.54  2006/08/15 15:42:09  joegenbaclor
 * 1. Added Instant Messaging functions to OPAL
 *
 * Revision 1.53  2006/08/14 12:22:16  joegenbaclor
 * Added m_CanSendCancel flag to determine if provisional response has been received prior to sending CANCEL
 *
 * Revision 1.52  2006/08/14 10:30:35  joegenbaclor
 * Fixed bug in MakeCall where send address is never set
 *
 * Revision 1.51  2006/08/14 01:29:08  joegenbaclor
 * Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
 *
 * Revision 1.50  2006/08/11 09:30:51  joegenbaclor
 * Corrected several compile warnings in GCC
 *
 * Revision 1.49  2006/08/09 12:17:48  joegenbaclor
 * Added RFC 3428 Support
 *
 * Revision 1.48  2006/08/06 02:32:08  joegenbaclor
 * Corrected bug regarding blank call-ids for UnknownTransaction events
 *
 * Revision 1.47  2006/06/28 14:14:59  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.46  2006/06/22 08:27:57  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.45  2006/06/21 14:29:11  joegenbaclor
 * This upload marks the first successful call by OpenB2BUA
 *
 * Revision 1.44  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
 *
 * Revision 1.43  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.42  2006/06/09 07:37:30  joegenbaclor
 * 1.  Added Publish, SIPETag & SIPIfMatch  support in SIPMessage
 * 2.  Started work on StatusPage Resource for OSSAppConfig
 *
 * Revision 1.41  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.40  2006/04/29 03:22:54  joegenbaclor
 * 1.  More Refer support code added to call session
 * 2.  Added StoreRegistrationInfo callback to Registrar classes
 *
 * Revision 1.39  2006/04/26 01:00:11  joegenbaclor
 * Added REFER NIST callbacks
 *
 * Revision 1.38  2006/04/25 03:41:10  joegenbaclor
 * 1.  Added REFER UAC subscription callbacks.
 * 2.  Renamed Event class in StatefulEventQueue to avoid collision with Event SIP Header
 *
 * Revision 1.37  2006/04/25 02:14:06  joegenbaclor
 * 1.  Added SendRefer method.
 * 2.  Added StateCallTransfer to SessionState enum
 *
 * Revision 1.36  2006/04/20 14:32:28  joegenbaclor
 * Fixed early media bug where soucd channels in OPAL ay not be opened in time for ringback to start streaming.
 *
 * Revision 1.35  2006/04/08 02:38:15  joegenbaclor
 * 1.  Corrected bug in call session where  OnConnected() is called befoe OnSDPOffe.
 * 2.  Corrected bug where Cancel syncpoint may not be signalled making calls to SendCancel block
 *
 * Revision 1.34  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.33  2006/04/06 04:27:51  joegenbaclor
 * More work on OPAL Inbound
 *
 * Revision 1.32  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.31  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.30  2006/03/23 05:37:58  joegenbaclor
 * Got rid of some compile warning in Visual Studio 2003
 *
 * Revision 1.29  2006/03/21 03:29:56  joegenbaclor
 * Added Logger functionality to log to PTrace
 *
 * Revision 1.28  2006/03/20 03:51:14  joegenbaclor
 * This upload marks the first successful OPAL call using opensipstack!!!   There is still some pending issues
 * in 183 Progress handling in OPAL classes but a fix should with the next revision.
 *
 * Revision 1.27  2006/03/17 13:51:00  joegenbaclor
 * 1.  Implemented UAC Call Redirection
 * 2.  Implemented SendCancel() method in CallSession
 * 3.  Introduced ClearInternalHeaders() function in SIP Message to erase internal headers inheritted from previous transaction.
 *
 * Revision 1.26  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.25  2006/03/15 16:40:44  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.24  2006/03/14 12:55:22  joegenbaclor
 * Removed Separate SIPClassese for simplicity.
 * Instead we will implement dialog support directly in SIPSession.
 *
 * Revision 1.23  2006/03/14 12:34:52  joegenbaclor
 * Implemented UAC OnAlerting(), OnConnect() and OnDisconnect() methods
 *
 * Revision 1.22  2006/03/14 05:17:12  joegenbaclor
 * Implemented CallSessionManager::OnRequireSDPOffer() to generate an interim Serssion Description
 *
 * Revision 1.21  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "CallSession.h"
#include "CallSessionManager.h"
#include "ossbuildopts.h"

#define new PNEW

using namespace UACORE;
using namespace UACORE;

/// UAS, does NOT use profiles
CallSession::CallSession(
  CallSessionManager & sessionManager,
  const SIPMessage & request,
  const PString & sessionId
) : SIPSession( (SIPSessionManager &)sessionManager, request, sessionId ),
    m_CallSessionManager( sessionManager )
{
  m_CurrentInviteCSeq = -1;
  m_CallAnswerResponse = DontDeny;
  m_State = StateIdle;
  m_ICTState = ICT_StateIdle;
  m_CallEndStatusCode = m_CallEndReason = SIPMessage::CodeUknown;
  m_HasSentProxyAuth = FALSE;
  m_IsRedirectingCall = FALSE;
  m_RedirectAttempts = 0;
  m_IsReferredCall = FALSE;
  m_IsReferrerCall = FALSE;
  m_TransferedCall = NULL;
  m_CanSendCancel = FALSE;
  m_MinSessionExpire = PTimeInterval( 0, 90 );
  m_MaxSessionExpire = PTimeInterval( 0, 3600 );
  m_LogTag = "CALL";
  m_XORHashOutgoingCalls = FALSE;
  m_EnableSessionTimer = FALSE;
  LOG_IF_DEBUG( LogInfo(), "*** CREATED *** Call Session" );
}

CallSession::CallSession(
  CallSessionManager & manager,
  const PString & sessionId,
  const ProfileUA & profile
) : SIPSession( (SIPSessionManager &)manager, sessionId, profile ),
    m_CallSessionManager( manager )
{
  m_CurrentInviteCSeq = -1;
  m_CallAnswerResponse = DontDeny;
  m_State = StateIdle;
  m_ICTState = ICT_StateIdle;
  m_CallEndStatusCode = m_CallEndReason = SIPMessage::CodeUknown;
  m_HasSentProxyAuth = FALSE;
  m_IsRedirectingCall = FALSE;
  m_RedirectAttempts = 0;
  m_IsReferredCall = FALSE;
  m_IsReferrerCall = FALSE;
  m_TransferedCall = NULL;
  m_CanSendCancel = FALSE;
  m_MinSessionExpire = PTimeInterval( 0, 90 );
  m_MaxSessionExpire = PTimeInterval( 0, 3600 );
  m_LogTag = "CALL";
  m_XORHashOutgoingCalls = FALSE;
  m_EnableSessionTimer = FALSE;
  LOG_IF_DEBUG( LogInfo(), "*** CREATED *** Call Session" );
}

CallSession::~CallSession()
{
  /// release reference to transfered call if its present
  ReleaseTransferedCall();

  LOG_IF_DEBUG( LogInfo(), "*** DESTROYED *** Call Session" );
}

BOOL CallSession::MakeCall(
  const SIPURI & uri
)
{
  PIPSocket::Address targetAddress = uri.GetAddress();
  /*WORD targetPort;*/

  m_SessionProfile.SetRemoteURI( uri.AsString() );
  SIPMessage invite;

  invite.SetXORHash( m_XORHashOutgoingCalls );
  m_EnableXORHash = m_XORHashOutgoingCalls;

  BOOL willUseProxy = FALSE;

  /// create the request line
  RequestLine requestLine;
  requestLine.SetMethod( "INVITE" );

  PString requestURI = uri.AsString();
  willUseProxy = !m_SessionProfile.GetProxyProfile().GetServerAddress().IsEmpty();
  requestLine.SetRequestURI( requestURI );
  invite.SetStartLine( requestLine );

  if( willUseProxy )
  {
    /// put the proxy as the static route
    SIPURI sendAddress( m_SessionProfile.GetProxyProfile().GetServerAddress() );
    invite.SetSendAddress( sendAddress );
    targetAddress = sendAddress.GetAddress();
  }

  PString transport;
  if( !uri.GetParameter( "transport", transport ) )
    transport = "udp";

  Via via;
  GetSessionManager().ConstructVia( targetAddress, via, SIPTransport::UDP );
  via.SetBranch( ParserTools::GenBranchParameter() );
  via.AddParameter( "rport", "" );
  invite.AppendVia( via );

  /// Set the inteface address to be used for future requests
  /// based it from the via host
  m_InterfaceAddress = via.GetAddress();
  m_InterfacePort = via.GetPort();

  /// Set From header
  From from;
  PStringStream userURI; 

  if( !m_SessionProfile.GetDisplayName().IsEmpty() )
    from.SetDisplayName( m_SessionProfile.GetDisplayName() );

  PString userName = m_SessionProfile.GetUser();
  
  if( !userName.IsEmpty() )
    userURI << userName << "@"; 

  PString localHost;

  if( willUseProxy )
  {
    localHost = m_SessionProfile.GetProxyProfile().GetDomain();
    if( localHost.IsEmpty() )
    {
      ///get the domain from the proxy address
      SIPURI uri( m_SessionProfile.GetRegistrationProfile().GetServerAddress() );
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

  invite.SetFrom( from );

  /// Set To header
  To to;
  to.SetURI( uri );
  invite.SetTo( to );

  /// Set the call Id
  CallId callId;
  callId.SetHeaderBody( GetSessionId() );
  invite.SetCallId( callId );

  /// Set the CSeq
  CSeq cSeq;
  cSeq.SetMethod( "INVITE" );
  cSeq.SetSequence( 4711 );
  invite.SetCSeq( cSeq );

  /// Set the contact
  SIPURI contactURI = fromURI;
  contactURI.SetHost( via.GetURI().GetHost() );
  contactURI.SetPort( via.GetURI().GetPort() );

  ContactURI contact( contactURI, m_SessionProfile.GetDisplayName() );
  invite.AppendContact( contact );

  ProfileMedia & mediaProfile = m_SessionProfile.GetMediaProfile();

  if( mediaProfile.GetMediaFormats().GetSize() > 0)
  {
    PString offer;
    
    if( !mediaProfile.ContructOffer( offer ) )
    {
      /// Do not return FALSE here.  The higher level applications
      /// might intentionally want to send media in ACK
      OnRequireSDPOffer( invite );
    }else
      invite.SetSDP( offer );
    
  }


  SetRequest( invite );

  m_CurrentUACInvite = invite;
  
  m_ICTState = CallSession::ICT_StateTrying;

  return SendRequest( invite );
}

BOOL CallSession::MakeCallTransfer(
  CallSession & callToTransfer,
  const SIPMessage & refer
)
{
  return FALSE;
}

void CallSession::OnIncomingSIPMessage(
  SIPMessageArrival & messageEvent
)
{
  /// make sure we never get deleted while we are doing something
  GCREF( "CallSession::OnIncomingSIPMessage" );

  SIPSession::OnIncomingSIPMessage( messageEvent );

  const SIPMessage & msg = messageEvent.GetMessage();
  TransactionId tid = messageEvent.GetTransactionId();
  PString fsm = tid.GetStateMachine();

  //LOG_IF_DEBUG( LogDebugVeryHigh(), msg );

  if( fsm == "ICT" )
  {
    if( msg.Is1xx() )
      EnqueueMessageEvent( ICT_Recv1xx, msg );
    else if( msg.Is2xx() )
      EnqueueMessageEvent( ICT_Recv2xx, msg );
    else if( msg.Is3xx() )
      EnqueueMessageEvent( ICT_Recv3xx, msg );
    else if( msg.Is4xx() )
      EnqueueMessageEvent( ICT_Recv4xx, msg );
    else if( msg.Is5xx() )
      EnqueueMessageEvent( ICT_Recv5xx, msg );
    else if( msg.Is6xx() )
      EnqueueMessageEvent( ICT_Recv6xx, msg );
    else
      EnqueueMessageEvent( ICT_RecvUnknown, msg );
  }else if( fsm == "NICT" )
  {
    if( msg.Is1xx() )
      EnqueueMessageEvent( NICT_Recv1xx, msg );
    else if( msg.Is2xx() )
      EnqueueMessageEvent( NICT_Recv2xx, msg );
    else if( msg.Is3xx() )
      EnqueueMessageEvent( NICT_Recv3xx, msg );
    else if( msg.Is4xx() )
      EnqueueMessageEvent( NICT_Recv4xx, msg );
    else if( msg.Is5xx() )
      EnqueueMessageEvent( NICT_Recv5xx, msg );
    else if( msg.Is6xx() )
      EnqueueMessageEvent( NICT_Recv6xx, msg );
    else
      EnqueueMessageEvent( NICT_RecvUnknown, msg );
  }else if( fsm == "IST" )
  {
    if( msg.IsInvite() )
    {
      // check the CSeq if its just a retransmission or a new INVITE
      CSeq cseq;
      if( !msg.GetCSeq( cseq ) )
      {
        return;  // we got a malformed CSeq.  Silently discard?
      }

      if( m_CurrentInviteCSeq == -1 )
      {
        /// this is a new invite
        m_CurrentInviteCSeq = cseq.GetSequence();

        EnqueueMessageEvent( IST_RecvInvite, msg );

        return;
      }else if( ((DWORD)m_CurrentInviteCSeq) < cseq.GetSequence() )
      {
        /// check if CSeq is higher
        EnqueueMessageEvent( IST_RecvMoreInvite, msg );
      }
    }else if( msg.IsAck() )
    {
      EnqueueMessageEvent( IST_RecvAck, msg );
    }else
    {
      EnqueueMessageEvent( IST_RecvUnknown, msg );
    }
  }else if( fsm == "NIST" )
  {
    if( msg.IsBye() )
      EnqueueMessageEvent( NIST_RecvBye, msg );
    else if( msg.IsOptions() )
      EnqueueMessageEvent( NIST_RecvOptions, msg );
    else if( msg.IsInfo() )
      EnqueueMessageEvent( NIST_RecvInfo, msg );
    else if( msg.IsCancel() )
      EnqueueMessageEvent( NIST_RecvCancel, msg );
    else if( msg.IsNotify() )
      EnqueueMessageEvent( NIST_RecvNotify, msg );
    else if( msg.IsSubscribe() )
      EnqueueMessageEvent( NIST_RecvSubscribe, msg );
    else if( msg.IsRefer() )
      EnqueueMessageEvent( NIST_RecvRefer, msg );
    else if( msg.IsMessage() )
      EnqueueMessageEvent( NIST_RecvMessage, msg );
    else if( msg.IsUpdate() )
      EnqueueMessageEvent( NIST_RecvUpdate, msg );
    else
      EnqueueMessageEvent( NIST_RecvUnknown, msg );
  }else
  {
    PAssertAlways( PLogicError );
  }

}

void CallSession::OnSIPMessageSent(
  SIPTransportWrite & messageEvent
)
{
  /// make sure we never get deleted while we are doing something
  GCREF( "CallSession::OnSIPMessageSent" );

  SIPSession::OnSIPMessageSent( messageEvent );

  const SIPMessage & msg = messageEvent.GetMessage();
  TransactionId tid = messageEvent.GetTransactionId();
  PString fsm = tid.GetStateMachine();

  //LOG_IF_DEBUG( LogDebugVeryHigh(), msg );

  if( fsm == "ICT" )
  {
    if( msg.IsInvite() )
      EnqueueMessageEvent( ICT_InviteSent, msg );
    else if( msg.IsAck() )
      EnqueueMessageEvent( ICT_AckSent, msg );
    else
      EnqueueMessageEvent( ICT_UnknownSent, msg );
  }else if( fsm == "NICT" )
  {
    if( msg.IsBye() )
      EnqueueMessageEvent( NICT_ByeSent, msg );
    else if( msg.IsOptions() )
      EnqueueMessageEvent( NICT_OptionsSent, msg );
    else if( msg.IsInfo() )
      EnqueueMessageEvent( NICT_InfoSent, msg );
    else if( msg.IsCancel() )
      EnqueueMessageEvent( NICT_CancelSent, msg );
    else if( msg.IsNotify() )
      EnqueueMessageEvent( NICT_NotifySent, msg );
    else if( msg.IsSubscribe() )
      EnqueueMessageEvent( NICT_SubscribeSent, msg );
    else
      EnqueueMessageEvent( NICT_UnknownSent, msg );

  }else if( fsm == "IST" )
  {
    if( msg.Is1xx() )
      EnqueueMessageEvent( IST_1xxSent, msg );
    else if( msg.Is2xx() )
      EnqueueMessageEvent( IST_2xxSent, msg );
    else if( msg.Is3xx() )
      EnqueueMessageEvent( IST_3xxSent, msg );
    else if( msg.Is4xx() )
      EnqueueMessageEvent( IST_4xxSent, msg );
    else if( msg.Is5xx() )
      EnqueueMessageEvent( IST_5xxSent, msg );
    else if( msg.Is6xx() )
      EnqueueMessageEvent( IST_6xxSent, msg );
    else
      EnqueueMessageEvent( IST_UnknownSent, msg );

  }else if( fsm == "NIST" )
  {
    if( msg.Is1xx() )
      EnqueueMessageEvent( NIST_1xxSent, msg );
    else if( msg.Is2xx() )
      EnqueueMessageEvent( NIST_2xxSent, msg );
    else if( msg.Is3xx() )
      EnqueueMessageEvent( NIST_3xxSent, msg );
    else if( msg.Is4xx() )
      EnqueueMessageEvent( NIST_4xxSent, msg );
    else if( msg.Is5xx() )
      EnqueueMessageEvent( NIST_5xxSent, msg );
    else if( msg.Is6xx() )
      EnqueueMessageEvent( NIST_6xxSent, msg );
    else
      EnqueueMessageEvent( NIST_UnknownSent, msg );
  }else
  {
    if( msg.IsAck() )
      EnqueueMessageEvent( ICT_AckSent, msg );
  }
}

void CallSession::OnSessionEvent(
  int event,
  const SIPMessage & eventMsg
)
{
  /// make sure we never get deleted while we are doing something
  GCREF( "Auto" );
  /// Handle one event at a time
  PWaitAndSignal lock( m_EventMutex );

  switch( event )
  {
    /// Invite Client Transaction Events
    case ICT_Recv1xx:
      ICT_OnReceived1xx( eventMsg );
      break;
    case ICT_Recv2xx:
      ICT_OnReceived2xx( eventMsg );
      SetState( StateConnected );
      break;
    case ICT_Recv3xx:
      m_CallEndReason = ICT_Recv3xx;
      m_CallEndStatusCode = eventMsg.GetStatusCode();
      ICT_OnReceived3xx( eventMsg );
      break;
    case ICT_Recv4xx:
      m_CallEndReason = ICT_Recv4xx;
      m_CallEndStatusCode = eventMsg.GetStatusCode();
      ICT_OnReceived4xx( eventMsg );
      break;
    case ICT_Recv5xx:
      m_CallEndReason = ICT_Recv5xx;
      m_CallEndStatusCode = eventMsg.GetStatusCode();
      
      ICT_OnReceived5xx( eventMsg );
      if( GetState() < StateConnected )
        SetState( StateDisconnected );

      break;
    case ICT_Recv6xx:
      m_CallEndReason = ICT_Recv6xx;
      m_CallEndStatusCode = eventMsg.GetStatusCode();
      
      ICT_OnReceived6xx( eventMsg );
      if( GetState() < StateConnected )
        SetState( StateDisconnected );

      break;
    case ICT_RecvUnknown:
      ICT_OnReceivedUnknown( eventMsg );
      break;
    case ICT_InviteSent:
      ICT_OnInviteSent( eventMsg );
      break;
    case ICT_AckSent:
      ICT_OnAckSent( eventMsg );
      break;
    case ICT_UnknownSent:
      ICT_OnUnknownSent( eventMsg );
      break;
    /// Invite Server Transaction Events
    case IST_RecvInvite:
      if( m_State == StateIdle )
        SetState( StateNewCall );
      IST_OnReceivedInvite( eventMsg );
      break;
    case IST_RecvMoreInvite:
      IST_OnReceivedMoreInvite( eventMsg );
      break;
    case IST_RecvAck:
      IST_OnReceivedAck( eventMsg );
      break;
    case IST_RecvUnknown:
      IST_OnReceivedUnknown( eventMsg );
      break;
    case IST_1xxSent:
      IST_On1xxSent( eventMsg );
      break;
    case IST_2xxSent:
      SetState( StateConnected );
      IST_On2xxSent( eventMsg );
      break;
    case IST_3xxSent:
      m_CallEndReason = IST_3xxSent;
      m_CallEndStatusCode = eventMsg.GetStatusCode();
      IST_On3xxSent( eventMsg );
      break;
    case IST_4xxSent:
      m_CallEndReason = IST_4xxSent;
      m_CallEndStatusCode = eventMsg.GetStatusCode();
      IST_On4xxSent( eventMsg );
      break;
    case IST_5xxSent:
      m_CallEndReason = IST_5xxSent;
      m_CallEndStatusCode = eventMsg.GetStatusCode();
      IST_On5xxSent( eventMsg );
      break;
    case IST_6xxSent:
      m_CallEndReason = IST_6xxSent;
      m_CallEndStatusCode = eventMsg.GetStatusCode();
      IST_On6xxSent( eventMsg );
      break;
    case IST_UnknownSent:
      IST_OnUnknownSent( eventMsg );
      break;
    case IST_AnswerCallNow:                // Sends 200 OK back with SDP after receipt of INVITE
      InternalAnswerCall( AnswerCallNow, eventMsg );
      SetState( StateConnected );
      break;
    case IST_AnswerCallDeferred:           // Answers with 180 ringing
      InternalAnswerCall( AnswerCallDeferred, eventMsg );
      SetState( StateAlerting );
      break;
    case IST_AnswerCallDeferredWithMedia:  // Like AnswerCallDefered Only media is sent in 183 progress
      InternalAnswerCall( AnswerCallDeferredWithMedia, eventMsg );
      SetState( StateProceeding );
      break;
    case IST_AnswerCallRedirect:           // Redirect the call to another address
      InternalAnswerCall( AnswerCallRedirect, eventMsg );
      SetState( StateRedirecting );
      break;
    case IST_AnswerCallRedirectToProxy:    // Redirect the call to a proxy
      InternalAnswerCall( AnswerCallRedirectToProxy, eventMsg );
      SetState( StateRedirecting );
      break;
    case IST_AnswerCallQueued:             // Tell remote that the call is queued
      InternalAnswerCall( AnswerCallQueued, eventMsg );
      SetState( StateQueued );
      break;
    case IST_AnswerCallDenied:              // Reject the call
      InternalAnswerCall( AnswerCallDenied, eventMsg );
      if( GetState() < StateConnected )
        SetState( StateDisconnected );
      break;
    /// None Invite Server Transaction Events
    case NIST_RecvBye:
      m_CallEndReason = NIST_RecvBye;
      if( GetState() < StateConnected )
        SetState( StateDisconnected );
      NIST_OnReceivedBye( eventMsg );
      break;
    case NIST_RecvOptions:
      NIST_OnReceivedOptions( eventMsg );
      break;
    case NIST_RecvInfo:
      NIST_OnReceivedInfo( eventMsg );
      break;
    case NIST_RecvCancel:
      m_CallEndReason = NIST_RecvCancel;
      NIST_OnReceivedCancel( eventMsg );
      break;
    case NIST_RecvNotify:
      NIST_OnReceivedNotify( eventMsg );
      break;
    case NIST_RecvSubscribe:
      NIST_OnReceivedSubscribe( eventMsg );
      break;
    case NIST_RecvRefer:
      NIST_OnReceivedRefer( eventMsg );
      break;
    case NIST_RecvMessage:
      NIST_OnReceivedMessage( eventMsg );
      break;
    case NIST_RecvUpdate:
      NIST_OnReceivedUpdate( eventMsg );
      break;
    case NIST_RecvUnknown:
      NIST_OnReceivedUnknown( eventMsg );
      break;
    case NIST_1xxSent:
      NIST_On1xxSent( eventMsg );
      break;
    case NIST_2xxSent:
      NIST_On2xxSent( eventMsg );
      break;
    case NIST_3xxSent:
      NIST_On3xxSent( eventMsg );
      break;
    case NIST_4xxSent:
      NIST_On4xxSent( eventMsg );
      break;
    case NIST_5xxSent:
      NIST_On5xxSent( eventMsg );
      break;
    case NIST_6xxSent:
      NIST_On6xxSent( eventMsg );
      break;
    case NIST_UnknownSent:
      NIST_OnUnknownSent( eventMsg );
      break;
    /// None Invite Client Transaction Events
    case NICT_Recv1xx:
      NICT_OnReceived1xx( eventMsg);
      break;
    case NICT_Recv2xx:
      NICT_OnReceived2xx( eventMsg );
      break;
    case NICT_Recv3xx:
      NICT_OnReceived3xx( eventMsg );
      break;
    case NICT_Recv4xx:
      NICT_OnReceived4xx( eventMsg );
      break;
    case NICT_Recv5xx:
      NICT_OnReceived5xx( eventMsg );
      break;
    case NICT_Recv6xx:
      NICT_OnReceived6xx( eventMsg );
      break;
    case NICT_RecvUnknown:
      NICT_OnReceivedUnknown( eventMsg );
      break;
    case NICT_ByeSent:
      m_CallEndReason = NICT_ByeSent;
      NICT_OnByeSent( eventMsg );
      break;
    case NICT_OptionsSent:
      NICT_OnOptionsSent( eventMsg );
      break;
    case NICT_InfoSent:
      NICT_OnInfoSent( eventMsg );
      break;
    case NICT_CancelSent:
      m_CallEndReason = NICT_CancelSent;
      NICT_OnCancelSent( eventMsg );
      break;
    case NICT_NotifySent:
      NICT_OnNotifySent( eventMsg );
      break;
    case NICT_SubscribeSent:
      NICT_OnSubscribeSent( eventMsg );
      break;
    case NICT_UnknownSent:
      NICT_OnUnknownSent( eventMsg );
      break;

    default:
      break;
  }
}

void CallSession::IST_OnReceivedInvite(
  const SIPMessage & msg
)
{
  PWaitAndSignal lock( m_AnswerCallMutex );

  if( GetState() == StateConnected )
  {
    IST_OnReceivedMoreInvite( msg );
    return;
  }
  
  SIPMessage response;
  
  m_CurrentUASInvite = msg;

  /// set the interface address to be used for future requests
  /// within the session;

  m_InterfaceAddress = msg.GetInterfaceAddress();
  m_InterfacePort = msg.GetInterfacePort();

  if( !OnIncomingConnection( msg ) )
  {
    LOG_IF_DEBUG( LogError(), "OnIncomingSession FAILED.  Rejecting incoming INVITE." );

    msg.CreateResponse(
      response,
      (WORD)(GetCallAnswerResponse() == DontDeny ?
          SIPMessage::Code404_NotFound : (SIPMessage::StatusCodes)GetCallAnswerResponse())
    );

    SendRequest( response );
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

      msg.CreateResponse(
        response,
        (WORD)(GetCallAnswerResponse() == DontDeny ?
        SIPMessage::Code415_UnsupportedMedia : (SIPMessage::StatusCodes)GetCallAnswerResponse() )
      );

      PTRACE( 1 , "CallSession::IST_OnReceivedInvite Rejecting Media" );
      SendRequest( response );
      return;
    }
  }

  if( !OnSetupIncomingCall( msg ) )
  {
    LOG_IF_DEBUG( LogError(), "OnSetupIncomingCall FAILED.  Rejecting incoming INVITE." );

    msg.CreateResponse(
      response,
      (WORD)(GetCallAnswerResponse() == DontDeny ?
          SIPMessage::Code404_NotFound : (SIPMessage::StatusCodes)GetCallAnswerResponse())
    );

    SendRequest( response );
    return;
  }
  

  /// now lets answer the call
  InternalAnswerCall( m_CallSessionManager.OnAnswerCall( msg, *this ), msg );

}

void CallSession::IST_OnReceivedMoreInvite(
  const SIPMessage & msg
)
{

  if( msg.GetCSeqNumber() <= m_CurrentUASInvite.GetCSeqNumber() )
    return;   /// ignore, this is a retransmission
  
  

  if( !OnReinvite( msg ) )
  {
    LOG_IF_DEBUG( LogError(), "OnIncomingSession FAILED.  Rejecting incoming INVITE." );
    SIPMessage response;

    msg.CreateResponse(
      response,
      (WORD)(GetCallAnswerResponse() == DontDeny ?
      SIPMessage::Code404_NotFound : (SIPMessage::StatusCodes)GetCallAnswerResponse())
    );

    SendRequest( response );
  }

}

void CallSession::IST_OnReceivedAck(
  const SIPMessage & msg
)
{
  LOG_IF_DEBUG( LogDebugVeryHigh(), msg );
  CallSessionManager::RetransmitThread * retran = (CallSessionManager::RetransmitThread *)m_CallSessionManager.GetRetransmitThread();
  retran->Expire200Ok( msg );
}

void CallSession::IST_On1xxSent(
  const SIPMessage & msg
)
{
}

void CallSession::IST_On2xxSent(
  const SIPMessage & msg
)
{
}

void CallSession::IST_On3xxSent(
  const SIPMessage & msg
)
{
}

void CallSession::IST_On4xxSent(
  const SIPMessage & msg
)
{
}

void CallSession::IST_On5xxSent(
  const SIPMessage & msg
)
{
}

void CallSession::IST_On6xxSent(
  const SIPMessage & msg
)
{
}

void CallSession::IST_OnUnknownSent(
    const SIPMessage & msg
)
{
}


void CallSession::IST_OnReceivedUnknown(
  const SIPMessage & msg
)
{
}


/** NIST Callbacks */
void CallSession::NIST_OnReceivedRegister(
  const SIPMessage & msg
)
{
}

void CallSession::NIST_OnReceivedBye(
  const SIPMessage & msg
)
{
  SetCallEndReason( CallSession::NIST_RecvBye );
  OnDisconnected();
  m_CallSessionManager.OnDisconnected( *this, msg );
  SIPMessage ok;
  msg.CreateResponse( ok, SIPMessage::Code200_Ok );
  SendRequest( ok );
}

void CallSession::NIST_OnReceivedOptions(
  const SIPMessage & msg
)
{
  SIPMessage ok;
  msg.CreateResponse( ok, SIPMessage::Code200_Ok );
  SendRequest( ok );
}

void CallSession::NIST_OnReceivedInfo(
  const SIPMessage & msg
)
{
  SIPMessage ok;
  msg.CreateResponse( ok, SIPMessage::Code200_Ok );
  SendRequest( ok );

  if( msg.GetContentType() *= "application/dtmf-relay" )
    OnReceivedINFODTMF( msg );
}

void CallSession::NIST_OnReceivedCancel(
  const SIPMessage & msg
)
{
  if( GetState() < StateConnected )
  {
    SetCallEndReason( CallSession::NIST_RecvCancel );
    SIPMessage ok;
    msg.CreateResponse( ok, SIPMessage::Code200_Ok );
    SendRequest( ok );
    SetCallAnswerResponse( CallSession::DisconnectWithRequestCancelled );
    if( m_CurrentUASInvite.IsValid() )
    {
      InternalAnswerCall( CallSession::AnswerCallDenied, m_CurrentUASInvite );
      OnDisconnected();
      m_CallSessionManager.OnDisconnected( *this, msg );
    }
  }
}

void CallSession::NIST_OnReceivedNotify(
  const SIPMessage & msg
)
{
  if( GetState() == StateCallTransfer && msg.HasEvent() )
  {
    Event event;
    msg.GetEvent( event );
    if( event.GetHeaderBody() *= "REFER" )
    {
      /// get the subscription state
      SubscriptionState state;
      if( !msg.GetSubscriptionState( state ) )
        return;

      /// get the SIP fragment from the body
      PString sipFrag = msg.GetBody();

      OnReferSubscriptionState( event, state, sipFrag );
    }
  }
}

void CallSession::NIST_OnReceivedSubscribe(
  const SIPMessage & msg
)
{
  SIPMessage ok;
  msg.CreateResponse( ok, SIPMessage::Code200_Ok );
  SendRequest( ok );
}


void CallSession::NIST_OnReceivedRefer(
  const SIPMessage & msg
)
{
  SIPMessage response;
  if( !m_CallSessionManager.OnCallBeingTransfered( *this, msg ) )
  {
    msg.CreateResponse( response, SIPMessage::Code406_NotAcceptable );
    SendRequest( response );
  }else
  {
    msg.CreateResponse( response, SIPMessage::Code202_Accepted );
    SendRequest( response );
    SetState( CallSession::StateCallTransfer );
    m_CallSessionManager.TransferCallAsUAS( *this, msg );
  }
}

void CallSession::NIST_OnReceivedMessage(
  const SIPMessage & msg
)
{
  SIPMessage ok;
  msg.CreateResponse( ok, SIPMessage::Code200_Ok );
  SendRequest( ok );
}

void CallSession::NIST_OnReceivedUpdate(
  const SIPMessage & msg
)
{
  SIPMessage ok;
    msg.CreateResponse( ok, SIPMessage::Code200_Ok );

  if( m_EnableSessionTimer && msg.IsSupportedExtension( "timer" ) )
    if( !OnProcessSessionTimers( msg, ok ) )
      return;
  
  SendRequest( ok );
}

void CallSession::NIST_OnReceivedUnknown(
  const SIPMessage & msg
)
{
}


void CallSession::NIST_On1xxSent(
  const SIPMessage & msg
)
{
}

void CallSession::NIST_On2xxSent(
  const SIPMessage & msg
)
{

}

void CallSession::NIST_On3xxSent(
  const SIPMessage & msg
)
{
}

void CallSession::NIST_On4xxSent(
  const SIPMessage & msg
)
{

}

void CallSession::NIST_On5xxSent(
  const SIPMessage & msg
)
{
}

void CallSession::NIST_On6xxSent(
  const SIPMessage & msg
)
{
}

void CallSession::NIST_OnUnknownSent(
  const SIPMessage & msg
)
{
}


/** NICT CALLBACKS */
void CallSession::NICT_OnRegisterSent(
  const SIPMessage & msg
)
{
}

void CallSession::NICT_OnByeSent(
  const SIPMessage & msg
)
{
  SetCallEndReason( CallSession::NICT_ByeSent );
  OnDisconnected();
  m_CallSessionManager.OnDisconnected( *this, msg );
}

void CallSession::NICT_OnOptionsSent(
  const SIPMessage & msg
)
{
}

void CallSession::NICT_OnInfoSent(
  const SIPMessage & msg
)
{
}

void CallSession::NICT_OnCancelSent(
  const SIPMessage & msg
)
{
}

void CallSession::NICT_OnNotifySent(
  const SIPMessage & msg
)
{
}

void CallSession::NICT_OnSubscribeSent(
  const SIPMessage & msg
)
{
}

void CallSession::NICT_OnUnknownSent(
  const SIPMessage & msg
)
{
}

void CallSession::NICT_OnReceived1xx(
  const SIPMessage & msg
)
{
}

void CallSession::NICT_OnReceived2xx(
  const SIPMessage & msg
)
{
  PString method = msg.GetCSeqMethod();

  if( GetState() == StateCallTransfer )
  {
    /// check if this is a 202 Accepted for refer
    if( method *= "REFER" )
    {
      OnReferAccepted();
    }
  }
}

void CallSession::NICT_OnReceived3xx(
  const SIPMessage & msg
)
{
  PString method = msg.GetCSeqMethod();

  if( GetState() == StateCallTransfer )
  {
    if( method *= "REFER" )
    {
      OnReferRejected( msg );
    }
  }
}

void CallSession::NICT_OnReceived4xx(
  const SIPMessage & msg
)
{
PTRACE(1, "### NICT_OnReceived4xx()");
  PString method = msg.GetCSeqMethod();

  if( GetState() == StateCallTransfer )
  {
   if( method *= "REFER" )
    {
      OnReferRejected( msg );
    }
  }
}

void CallSession::NICT_OnReceived5xx(
  const SIPMessage & msg
)
{
  PString method = msg.GetCSeqMethod();

  if( GetState() == StateCallTransfer )
  {
    if( method *= "REFER" )
    {
      OnReferRejected( msg );
    }
  }
}

void CallSession::NICT_OnReceived6xx(
  const SIPMessage & msg
)
{
  PString method = msg.GetCSeqMethod();

  if( GetState() == StateCallTransfer )
  {
    if( method *= "REFER" )
    {
      OnReferRejected( msg );
    }
  }
}

void CallSession::NICT_OnReceivedUnknown(
  const SIPMessage & msg
)
{
}

/** ICT CALLBACKS */
void CallSession::ICT_OnInviteSent(
  const SIPMessage & msg
)
{
  m_ICTState = CallSession::ICT_StateTrying;
}

void CallSession::ICT_OnAckSent(
  const SIPMessage & msg
)
{
}

void CallSession::ICT_OnReceived1xx(
  const SIPMessage & msg
)
{
  m_ICTState = CallSession::ICT_StateAlerting;
  
  m_CanSendCancel = TRUE;

  if( GetState() == StateCancelling )
    return;

  if( msg.HasSDP() )
  {
    OnIncomingSDPOffer( msg );
  }

  switch( msg.GetStatusCode() )
  {
    case SIPMessage::Code180_Ringing:
        
        if( !msg.HasSDP() )
        {
          LOG_IF_DEBUG( LogInfo(), "Remote is ALERTING" );
          m_CallSessionManager.OnAlerting( *this, msg );
        }else
        {
          LOG_IF_DEBUG( LogInfo(), "Remote is PROGRESSING with MEDIA" );
          m_CallSessionManager.OnProgress( *this, msg );
        }
        break;
    case SIPMessage::Code183_SessionProgress:
        LOG_IF_DEBUG( LogInfo(), "Remote is PROGRESSING with MEDIA" );
        m_CallSessionManager.OnProgress( *this, msg );
        break;
    case SIPMessage::Code100_Trying:
        LOG_IF_DEBUG( LogInfo(), "Remote is TRYING the call" );
        break;
    default:
        LOG_IF_DEBUG( LogWarning(), "Unhandled progress response" );
        LOG_IF_DEBUG( LogWarning(), msg );
        return;
  }


}

void CallSession::ICT_OnReceived2xx(
  const SIPMessage & msg
)
{
  m_CanSendCancel = FALSE;

  if( m_ICTState == CallSession::ICT_StateConfirmed )
  {
    /// This is a retransmission, just send an ACK
    SendAck( msg );
    return;
  }
  
  m_ICTState = CallSession::ICT_StateConfirmed;

  if( GetState() == StateCancelling )
  {
    SendBye();
    return;
  }

  if( msg.HasSDP() )
  {
    OnIncomingSDPOffer( msg );
  }

  InitializeDialogAsUAC( m_CurrentUACInvite, msg );

  /// clear out the proxy auth flag to allow reinvites to be 
  /// authenticated
  m_HasSentProxyAuth = FALSE;

  /// Clear out the redirecting flag
  m_IsRedirectingCall = FALSE;

  LOG_IF_DEBUG( LogInfo(), "Remote is CONNECTED" );
  OnConnected();
  m_CallSessionManager.OnConnected( *this, msg );
  SIPMessage connect = msg;

  SendAck( msg );
}


void CallSession::ICT_OnReceived3xx(
  const SIPMessage & msg
)
{
  if( GetState() == StateCancelling )
  {
    m_UACCancelSync.Signal();
    OnDisconnected();
    m_CallSessionManager.OnDisconnected( *this,msg );
    return;
  }

  if( !((CallSessionManager&)GetSessionManager()).OnCallRedirect( *this, msg ) ||
        !OnCallRedirect( msg ) )
  {
    SetCallEndReason( ICT_Recv3xx );
    OnDisconnected();
    m_CallSessionManager.OnDisconnected( *this, msg );
  }
}

void CallSession::ICT_OnReceived4xx(
  const SIPMessage & msg
)
{
  m_UACCancelSync.Signal();

PTRACE(1, "### ICT_OnReceived4xx()");
  if( ( msg.GetStatusCode() == SIPMessage::Code407_ProxyAuthenticationRequired ||
        msg.GetStatusCode() == SIPMessage::Code401_Unauthorized ) &&
        GetState() != StateCancelling 
    )
  {
    if( !OnProxyAuthentication( msg ) )
    {
      SetState( StateDisconnected );
      OnDisconnected();
      m_CallSessionManager.OnDisconnected( *this, msg );
    }
  }else
  {
    SetState( StateDisconnected );
    OnDisconnected();
    m_CallSessionManager.OnDisconnected( *this,msg );
  }
}

void CallSession::ICT_OnReceived5xx(
  const SIPMessage & msg
)
{
  m_UACCancelSync.Signal();

  OnDisconnected();
  ((CallSessionManager&)GetSessionManager()).OnDisconnected( *this, msg );
}

void CallSession::ICT_OnReceived6xx(
  const SIPMessage & msg
)
{
  m_UACCancelSync.Signal();
  OnDisconnected();
  ((CallSessionManager&)GetSessionManager()).OnDisconnected( *this, msg );
}

void CallSession::ICT_OnReceivedUnknown(
  const SIPMessage & msg
)
{
}

void CallSession::ICT_OnUnknownSent(
    const SIPMessage & msg
)
{
}

void CallSession::EnqueueMessageEvent(
  int event,
  const SIPMessage & msg
)
{
  EnqueueSessionEvent( new SIPSessionEvent( *this, event, msg ) );
}

/// returning FALSE here will automatically reject the incoming call
BOOL CallSession::OnIncomingConnection(
  const SIPMessage & invite
)
{
  return m_CallSessionManager.OnIncomingConnection( invite, *this );
}

BOOL CallSession::OnProcessSessionTimers(
  const SIPMessage & invite,
  SIPMessage & ok
)
{
  SessionExpires sessionExpires;
  PTimeInterval expire = m_MaxSessionExpire;

  if( invite.GetSessionExpires( sessionExpires ) )
    expire = sessionExpires.AsInt64();
  
  if( expire < m_MinSessionExpire )
  {
    SIPMessage tooSmall;
    invite.CreateResponse( tooSmall, SIPMessage::Code422_SessionIntervalTooSmall );
    MinSE minse( m_MinSessionExpire );
    tooSmall.SetMinSE( minse );
    SendRequest( tooSmall );
    return FALSE;
  }else
  {
    if( expire > m_MaxSessionExpire )
      sessionExpires.SetValue( m_MaxSessionExpire.GetSeconds() );

    sessionExpires.SetParameter( "refresher", "uac" );
    ok.SetSessionExpires( sessionExpires );
    PWaitAndSignal lock( m_SessionTimerMutex );
    m_SessionTimer.Stop();
    m_SessionTimer = PTimer( sessionExpires.AsInt64() * 1000 );
    m_SessionTimer.SetNotifier( PCREATE_NOTIFIER( _OnSessionTimerExpire ) );
    m_SessionTimer.Resume();
  }

  return TRUE;
}

void CallSession::_OnSessionTimerExpire( PTimer & , INT)
{
  OnSessionTimerExpire();
}

void CallSession::OnSessionTimerExpire()
{
  LOG( LogError(), "Session Timer Expired!!!" );
  SendBye();
}

BOOL CallSession::OnSetupIncomingCall( 
  const SIPMessage & invite 
)
{
  return m_CallSessionManager.OnSetupIncomingCall( invite, *this );
}

BOOL CallSession::OnReinvite(
  const SIPMessage & invite
)
{
  /// tell the manager we are being reinvited
  if( !m_CallSessionManager.OnReinvite( invite, *this ) )
    return FALSE;

  m_CurrentUASInvite = invite;
  /// CallSessionManager Allowed this call to process
  /// check for presence of SDP

  BOOL hasSDP = invite.HasSDP();
  PString sdpOffer;

  if( hasSDP )
  {
    sdpOffer = invite.GetBody();
    if( !OnIncomingSDPOffer( invite ) )
    {
      LOG_IF_DEBUG( LogError(), "OnIncomingSDPOffer FAILED.  Rejecting incoming INVITE." );
      
      SIPMessage response;
      
      invite.CreateResponse(
        response,
        (WORD)(GetCallAnswerResponse() == DontDeny ?
        SIPMessage::Code415_UnsupportedMedia : (SIPMessage::StatusCodes)GetCallAnswerResponse() )
      );

      PTRACE( 1 , "CallSession::OnReinvite Rejecting Media" );

      SendRequest( response );
      return FALSE;
    }
  }

  /// now lets answer the call
  return SendConnect( invite );
}

BOOL CallSession::OnProxyAuthentication(
  const SIPMessage & challenge
)
{
  if( challenge.GetStatusCode() == SIPMessage::Code401_Unauthorized )
    return OnWWWAuthentication( challenge );

  PWaitAndSignal lock( m_ProxyAuthMutex );


  if( m_HasSentProxyAuth )
    return FALSE;

  ProfileRegister & profile = GetSessionProfile().GetProxyProfile();
  PString userName = profile.GetAccount();
  PString password = profile.GetPassword();

  if( userName.IsEmpty() )
  {
    userName = profile.GetUser();
    if( userName.IsEmpty() )
    {
      LOG_IF_DEBUG( LogWarning(), "*** Proxy Authentication User is EMPTY!!! ***" );
      LOG_IF_DEBUG( LogWarning(), "*** Unable to authenticate Call!!! ***" );
      return FALSE;
    }
  }

  if( password.IsEmpty() )
  {
    LOG_IF_DEBUG( LogWarning(), "Proxy authentication password is Empty" );
  }

  if( !challenge.HasProxyAuthenticate() )
    return FALSE;

  ProxyAuthenticate proxyAuth;
  challenge.GetProxyAuthenticate( proxyAuth );

  /// clear the internal headers from the previous transaction
  m_CurrentUACInvite.ClearInternalHeaders();

  /// increment cseq
  CSeq cseq;
  m_CurrentUACInvite.GetCSeq( cseq );
  m_CurrentUACInvite.SetCSeq( ++cseq );

  StartLine startLine;
  m_CurrentUACInvite.GetStartLine( startLine );
  startLine.GetRequestLine()->GetRequestURI().AsString();
  PString uriText = startLine.GetRequestLine()->GetRequestURI().AsString( FALSE );

  PString realm;
  proxyAuth.GetParameter( "realm", realm );
  ParserTools::UnQuote( realm );
  PString nonce;
  proxyAuth.GetParameter( "nonce", nonce );
  ParserTools::UnQuote( nonce );

  MD5::A1Hash a1( userName,realm, password );
  MD5::A2Hash a2( startLine.GetRequestLine()->GetMethod(), uriText );
  PString hResponse = MD5::MD5Authorization::Construct( a1.AsString(), nonce, a2.AsString() );

  ProxyAuthorization authorization;
  authorization.SetLeadString( "Digest" );
  authorization.AddParameter( "username", ParserTools::Quote( userName ) );
  authorization.AddParameter( "realm", ParserTools::Quote( realm ) );
  authorization.AddParameter( "nonce", ParserTools::Quote( nonce ) );
  authorization.AddParameter( "uri", ParserTools::Quote( uriText ) );
  authorization.AddParameter( "response", ParserTools::Quote( hResponse ) );
  authorization.AddParameter( "algorithm", "MD5" );

  m_CurrentUACInvite.SetProxyAuthorization( authorization );

  Via via;
  m_CurrentUACInvite.GetViaAt( 0, via );
  via.AddParameter( "branch", ParserTools::GenBranchParameter(), TRUE );
  m_CurrentUACInvite.SetViaAt( 0, via );

  m_HasSentProxyAuth = TRUE;

  /// check if we are configured to use an outbound proxy
  if(  !m_SessionProfile.GetProxyProfile().GetServerAddress().IsEmpty() )
  {
    /// put the proxy as the static route
    SIPURI sendAddress( m_SessionProfile.GetProxyProfile().GetServerAddress() );
    m_CurrentUACInvite.SetSendAddress( sendAddress );
  }

  
  return SendRequest( m_CurrentUACInvite );
}

BOOL CallSession::OnWWWAuthentication(
  const SIPMessage & challenge
)
{
    PWaitAndSignal lock( m_ProxyAuthMutex );

  if( m_HasSentProxyAuth )
    return FALSE;

  ProfileRegister & profile = GetSessionProfile().GetProxyProfile();
  PString userName = profile.GetAccount();
  PString password = profile.GetPassword();

  if( userName.IsEmpty() )
    return FALSE;

  if( !challenge.HasWWWAuthenticate() )
    return FALSE;

  WWWAuthenticate proxyAuth;
  challenge.GetWWWAuthenticate( proxyAuth );

  /// clear the internal headers from the previous transaction
  m_CurrentUACInvite.ClearInternalHeaders();

  /// increment cseq
  CSeq cseq;
  m_CurrentUACInvite.GetCSeq( cseq );
  m_CurrentUACInvite.SetCSeq( ++cseq );

  StartLine startLine;
  m_CurrentUACInvite.GetStartLine( startLine );
  startLine.GetRequestLine()->GetRequestURI().AsString();
  PString uriText = startLine.GetRequestLine()->GetRequestURI().AsString( FALSE );

  PString realm;
  proxyAuth.GetParameter( "realm", realm );
  ParserTools::UnQuote( realm );
  PString nonce;
  proxyAuth.GetParameter( "nonce", nonce );
  ParserTools::UnQuote( nonce );

  MD5::A1Hash a1( userName,realm, password );
  MD5::A2Hash a2( startLine.GetRequestLine()->GetMethod(), uriText );
  PString hResponse = MD5::MD5Authorization::Construct( a1.AsString(), nonce, a2.AsString() );

  Authorization authorization;
  authorization.SetLeadString( "Digest" );
  authorization.AddParameter( "username", ParserTools::Quote( userName ) );
  authorization.AddParameter( "realm", ParserTools::Quote( realm ) );
  authorization.AddParameter( "nonce", ParserTools::Quote( nonce ) );
  authorization.AddParameter( "uri", ParserTools::Quote( uriText ) );
  authorization.AddParameter( "response", ParserTools::Quote( hResponse ) );
  authorization.AddParameter( "algorithm", "MD5" );

  m_CurrentUACInvite.SetAuthorization( authorization );

  Via via;
  m_CurrentUACInvite.GetViaAt( 0, via );
  via.AddParameter( "branch", ParserTools::GenBranchParameter(), TRUE );
  m_CurrentUACInvite.SetViaAt( 0, via );

  m_HasSentProxyAuth = TRUE;

  /// check if we are configured to use an outbound proxy
  if(  !m_SessionProfile.GetProxyProfile().GetServerAddress().IsEmpty() )
  {
    /// put the proxy as the static route
    SIPURI sendAddress( m_SessionProfile.GetProxyProfile().GetServerAddress() );
    m_CurrentUACInvite.SetSendAddress( sendAddress );
  }

  
  return SendRequest( m_CurrentUACInvite );
}

BOOL CallSession::OnCallRedirect(
  const SIPMessage & redirect
)
{
  PWaitAndSignal lock( m_FailOverQueueMutex );

  if( redirect.GetContactSize() == 0 )
  {
    /// returning FALSE will disconnect the call.
    /// if we are already in rediretion mode,
    /// it only means we get redirected once more
    /// and there is no contact info.  Just
    /// silently discard
    return m_IsRedirectingCall;
  }

  SetState( StateRedirecting );
  m_IsRedirectingCall = TRUE;

  /// get the first contact in the list

  for( PINDEX i = 0; i < redirect.GetContactSize(); i++ )
  {
    Contact & contact = redirect.GetContactAt( i );
    ContactURI contactURI;
  
  /// get the first URI in the contact header
    for( PINDEX j = 0; j < contact.GetSize(); j++ )
    {
      if( contact.GetURI( contactURI, j ) )
      {
        SIPURI * remoteURI = new SIPURI( contactURI.GetURI() );
        // put the uri in queue
        m_UACRedirectFailoverQueue.Enqueue( remoteURI );
      }
    }
  }
  
  return OnHandleCallRedirect();
}

BOOL CallSession::OnHandleCallRedirect()
{
  PWaitAndSignal lock( m_FailOverQueueMutex );

  if( !m_IsRedirectingCall || ++m_RedirectAttempts > ICT_MAX_REDIRECT_ATTEMPTS )
    return FALSE;

  SIPURI * redirectURI = m_UACRedirectFailoverQueue.Dequeue();

  if( redirectURI == NULL )
    return FALSE;

  PIPSocket::Address redirectAddress( redirectURI->GetHost() );
  if( redirectAddress.IsValid() )
  {
    /// contact is an ip address.  check if we have a port;
    if( redirectURI->GetPort().IsEmpty() )
      redirectURI->SetPort( "5060" );
  }

  StartLine startLine;
  m_CurrentUACInvite.GetStartLine( startLine );

  LOG_IF_DEBUG( LogInfo(), "Redirecting Call: \n" 
    << startLine.GetRequestLine()->GetRequestURI() << "\n"
    << *redirectURI );

  startLine.GetRequestLine()->SetRequestURI( *redirectURI );
  m_CurrentUACInvite.SetStartLine( startLine );
  m_CurrentUACInvite.SetFromTag( ParserTools::GenTagParameter() );

  /// increment the cseq
  CSeq cseq;
  m_CurrentUACInvite.GetCSeq( cseq );
  m_CurrentUACInvite.SetCSeq( ++cseq );

  Via via;
  m_CurrentUACInvite.GetViaAt( 0, via );
  via.SetParameter( "branch", ParserTools::GenBranchParameter() );
  m_CurrentUACInvite.SetViaAt( 0, via );

  delete redirectURI;
  redirectURI = NULL;
  
  /// clear internal headers from the last transaction;
  m_CurrentUACInvite.ClearInternalHeaders();

  return SendRequest( m_CurrentUACInvite );
}

/// will be called when a trasfered call

/// returning FALSE here will reject the call with no acceptable format
/// depending on call state.  Offers received in ACK will be disconnected
/// using a BYE.
BOOL CallSession::OnIncomingSDPOffer(
  const SIPMessage & _offer
)
{
#if 0
  PString offer = _offer.GetBody();
  BOOL hasMediaProfile = FALSE;
  /// first check if the session profile has registered some codecs
  ProfileMedia sessionProfile;

  if( GetSessionProfile().MediaProfile().GetMediaFormats().GetSize() > 0 )
  {
    /// use the session level profile to construct the answer
    sessionProfile = GetSessionProfile().MediaProfile();
    hasMediaProfile = TRUE;
  }else if( GetSessionManager().GetUserAgent().GetDefaultProfile().MediaProfile().GetMediaFormats().GetSize() > 0 )
  {
    /// use the default UA profile
    sessionProfile = GetSessionManager().GetUserAgent().GetDefaultProfile().MediaProfile();
    hasMediaProfile = TRUE;
  }

  if( !hasMediaProfile )
    return FALSE;

  /// check if we have common audio/video media formats.
  ProfileMedia finalProfile = sessionProfile;
  finalProfile.ClearFormats();
  SDPMediaFmt::Collection answerFormats = sessionProfile.GetMediaFormats();

  BOOL hasAudioFormat = FALSE;
  BOOL hasVideoFormat = FALSE;

  SDPOffer sdpOffer( offer );

  for( PINDEX i = 0; i < answerFormats.GetSize(); i++ )
  {
    SDPMediaFmt & format = answerFormats[i];

    if( format.GetEncodingName() *= "telephone-event" )
    {
      
      if( sdpOffer.FindFormat( format.GetEncodingName() ) != NULL )
      {
        LOG_IF_DEBUG( LogDebug(), "SDP\tFound telephone-event in media offer" );
        finalProfile.AddMediaFormat( format );
      }else
      {
        LOG_IF_DEBUG( LogDebug(), "SDP\tTelephone-event not present in media offer" );
        finalProfile.RemoveMediaFormat( format );
      }

      continue;
    }

    /// check if the format is in the offer
    if( sdpOffer.FindFormat( format.GetEncodingName() ) != NULL )
    {
      LOG_IF_DEBUG( LogDebug(), "SDP\tFound " << format.GetEncodingName() << " in media offer" );
      if( format.GetMediaType() == SDPMediaFmt::Audio && !hasAudioFormat )
      {
        hasAudioFormat = TRUE;
        finalProfile.AddMediaFormat( format );
      }else if( format.GetMediaType() == SDPMediaFmt::Video && !hasVideoFormat )
      {
        hasVideoFormat = TRUE;
        finalProfile.AddMediaFormat( format );
      }
    }else
    {
      finalProfile.RemoveMediaFormat( format );
      LOG_IF_DEBUG( LogDebug(), "SDP\t" << format.GetEncodingName() << " not found in media offer" );
    }
  }

  if( !hasAudioFormat )
    return FALSE;

  m_SessionProfile.GetMediaProfile() = finalProfile;
#endif
  return m_CallSessionManager.OnIncomingSDPOffer( _offer, *this );
}

/// your last chance to produce an answer to the offer.
/// This will be called either before sending 183, 200 or ACK
BOOL CallSession::OnRequireSDPAnswer(
  const SIPMessage & offer,
  SIPMessage & answer
)
{
  return m_CallSessionManager.OnRequireSDPAnswer( offer, answer, *this );
}

/// this will be called if an offer is not received in INVITE
/// or when we are initiating the call
BOOL CallSession::OnRequireSDPOffer(
  SIPMessage & offer
)
{
  return m_CallSessionManager.OnRequireSDPOffer( offer, *this );
}


/// this is where implementors may modify or add headers before underlying implementation
/// sends the actual response to INVITE to the FSM
BOOL CallSession::OnSendingResponseToInvite(
  SIPMessage & response
)
{
  return m_CallSessionManager.OnSendingResponseToInvite( response, *this );
}

/// this is where implementors may modify or add headers before underlying implementation
/// sends the actual response messages to the FSM for none-invite transactions
BOOL CallSession::OnSendingResponseToNoneInvite(
  SIPMessage & response
)
{
  return m_CallSessionManager.OnSendingResponseToNoneInvite( response, *this );
}

/// answers the call.  based on the mode.  May be called several times
BOOL CallSession::AnswerCall(
  AnswerCallResponse mode
)
{
  GCREF( "CallSession::OnSessionEvent" );


  if( GetState() >= StateMaxAnswerCall )
    return FALSE;

  switch( mode )
  {
    case AnswerCallNow:                // Sends 200 OK back with SDP after receipt of INVITE
      EnqueueMessageEvent( IST_AnswerCallNow, m_CurrentUASInvite );
      break;
    case AnswerCallDeferred:           // Answers with 180 ringing
      EnqueueMessageEvent( IST_AnswerCallDeferred, m_CurrentUASInvite );
      break;
    case AnswerCallDeferredWithMedia:  // Like AnswerCallDefered Only media is sent in 183 progress
      EnqueueMessageEvent( IST_AnswerCallDeferredWithMedia, m_CurrentUASInvite );
      break;
    case AnswerCallRedirect:           // Redirect the call to another address
      EnqueueMessageEvent( IST_AnswerCallRedirect, m_CurrentUASInvite  );
      break;
    case AnswerCallRedirectToProxy:    // Redirect the call to a proxy
      EnqueueMessageEvent( IST_AnswerCallRedirectToProxy, m_CurrentUASInvite  );
      break;
    case AnswerCallQueued:             // Tell remote that the call is queued
      EnqueueMessageEvent( IST_AnswerCallQueued, m_CurrentUASInvite  );
      break;
    case AnswerCallDenied:              // Reject the call
      EnqueueMessageEvent( IST_AnswerCallDenied, m_CurrentUASInvite  );
      break;
    default:
      PAssertAlways( PLogicError );
  }
  return TRUE;
}

BOOL CallSession::InternalAnswerCall(
  AnswerCallResponse mode,
  const SIPMessage & invite
)
{
  GCREF( "CallSession::InternalAnswerCall" );

  PWaitAndSignal lock( m_AnswerCallMutex );

  switch (mode)
  {
    case AnswerCallNow :               // Sends 200 OK back with SDP after receipt of INVITE
      SendConnect( invite );
      break;
    case AnswerCallDeferredWithMedia:  // Like AnswerCallDefered Only media is sent in 183 progress
      SendProgress( invite );
      break;
    case AnswerCallDeferred:           // Answers with 180 ringing
      SendAlerting( invite );
      break;
    case AnswerCallRedirect:           // Redirect the call to another address
      SendUARedirect( invite );
      break;
    case AnswerCallRedirectToProxy:    // Redirect the call to a proxy
      SendProxyRedirect( invite );
      break;
    case AnswerCallQueued:             // Tell remote that the call is queued
      SendCallQueued( invite );
      break;
    case AnswerCallDenied:             // Reject the call
      SendReject( invite, GetCallAnswerResponse() == DontDeny ?
          SIPMessage::Code403_Forbidden : (SIPMessage::StatusCodes)GetCallAnswerResponse()
      );
      break;
    case NumAnswerCall:
      break;
    default:
      PAssertAlways( PLogicError );
  }

  return TRUE;
}
/// Sends 182 Call Queue
BOOL CallSession::SendCallQueued( const SIPMessage & request )
{
  GCREF( "CallSession::SendCallQueued" );
  SIPMessage alerting;

  request.CreateResponse(
    alerting,
    SIPMessage::Code182_Queued
  );

  return SendRequest( alerting );
}

BOOL CallSession::SendAlerting(  const SIPMessage & request )
{
  GCREF( "CallSession::SendAlerting" );
  SIPMessage alerting;

  request.CreateResponse(
    alerting,
    SIPMessage::Code180_Ringing
  );

  return SendRequest( alerting );
}
/// sends and retransmits 200 Ok for an INVITE with SDP
/// 200 ok response will be generated
BOOL CallSession::SendConnect( const SIPMessage & request )
{
  GCREF( "Auto" );

  SIPMessage ok;
  if( !request.CreateResponse( ok, SIPMessage::Code200_Ok ) )
    return FALSE;

  ok.SetXORHash( IsXORHashEnabled() );

  if( request.IsInvite() )
  {
    PString sdp;
    if( request.HasSDP() )
    {
      PString body;
      request.GetBody( body );
      PString offer( body );
      PString answer;

      if( !OnRequireSDPAnswer( request, ok ) )
      {

        SendReject( request, GetCallAnswerResponse() == DontDeny ?
                              SIPMessage::Code415_UnsupportedMedia :
            (SIPMessage::StatusCodes)GetCallAnswerResponse() );

        return FALSE;
      }
    }else
    {

      if( !OnRequireSDPOffer( ok ) )
      {
        PTRACE( 1 , "CallSession::SendConnect Rejecting Media" );
        SendReject( request, SIPMessage::Code415_UnsupportedMedia );
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

  }

  if( request.IsInvite() )
  {
    /// Here we should send the 200 ok to the retransmitter
    CallSessionManager::RetransmitThread * retran = (CallSessionManager::RetransmitThread *)m_CallSessionManager.GetRetransmitThread();
    retran->AddMessage( GetSessionId(), ok );
    InitializeDialogAsUAS( request, ok );

    if( m_EnableSessionTimer && request.IsSupportedExtension( "timer" ) && !OnProcessSessionTimers( request, ok ) )
      return FALSE;
  }

  /// Send it to the FSM once to terminate the IST transaction
  if( !SendRequest( ok ) )
    return FALSE;

  if( request.IsInvite() )
    OnConnected();

  return TRUE;
}

BOOL CallSession::SendProgress( const SIPMessage & invite )
{
  GCREF( "CallSession::SendConnect" );
  PAssert( invite.IsInvite(), PLogicError );
  SIPMessage progress;
  if( !invite.CreateResponse( progress, SIPMessage::Code183_SessionProgress ) )
    return FALSE;

  PString sdp;
  if( invite.HasSDP() )
  {
    if( !OnRequireSDPAnswer( invite, progress ) )
    {
      PTRACE( 1 , "CallSession::SendProgress Rejecting Media" );
      SendReject( invite, GetCallAnswerResponse() == DontDeny ?
                             SIPMessage::Code415_UnsupportedMedia :
          (SIPMessage::StatusCodes)GetCallAnswerResponse() );
      return FALSE;
    }
  }

  return SendRequest( progress );
}

/// Sends a redirect to proxy
BOOL CallSession::SendProxyRedirect( const SIPMessage & invite )
{
  GCREF( "CallSession::SendProxyRedirect" );

  ContactList contactList;

  if( !OnRequireRedirectionURI( contactList, TRUE ) )
  {
    SendReject( invite );
    return FALSE;
  }

  return SendAcceptByRedirection( invite, contactList, SIPMessage::Code305_UseProxy );
}

/// Sends a redirect to another UAS
BOOL CallSession::SendUARedirect( const SIPMessage & invite )
{
  GCREF( "CallSession::SendUARedirect" );

  ContactList contactList;

  if( !OnRequireRedirectionURI( contactList, FALSE ) )
  {
    SendReject( invite );
    return FALSE;
  }

  return SendAcceptByRedirection( invite, contactList, SIPMessage::Code305_UseProxy );
}

/// Sends a forbidden by default.
/// or GetAnswerCallresponse() if explicitly set
BOOL CallSession::SendReject(
  const SIPMessage & request,
  PINDEX statusCode,
  const PString & reasonPhrase,
  const PString & warning
)
{
  return SendAcceptByRejection( request, statusCode, reasonPhrase, warning );
}


BOOL CallSession::SendAck(
  const SIPMessage & response,
  const PString & sdp
)
{
  SIPMessage ack;
  if( !CreateRequestWithinDialog( SIPMessage::Method_ACK, ack ) )
    return FALSE;

  if( !sdp.IsEmpty() )
    ack.SetSDP( sdp );

  LOG_IF_DEBUG( LogDebugVeryHigh(), ack );

  ack.SetInterfaceAddress( m_InterfaceAddress );
  ack.SetInterfacePort( m_InterfacePort );

  GetSessionManager().GetUserAgent().GetStack().EnqueueTransportWriteEvent( ack );

  return TRUE;
}

/// will be called if a rediration URI is needed by OnAnswerCall
BOOL CallSession::OnRequireRedirectionURI(
  ContactList & contacts,
  BOOL isProxyRedirect
)
{
  return m_CallSessionManager.OnRequireRedirectionURI( contacts, *this, isProxyRedirect );
}


/// will be called when the call is established
void CallSession::OnConnected()
{
}

void CallSession::OnReferAccepted()
{
  m_CallSessionManager.OnReferAccepted( *this );
}

void CallSession::OnReferRejected(
  const SIPMessage & reject
)
{
  m_CallSessionManager.OnReferRejected( *this, reject );
}

/// receives provisional and final responses
/// from the transfer target.  The default behavior of this
/// method is to send NOTIFY to the transfer requestor
void CallSession::OnCallTransferResponse(
  CallSession & callTransfer,
  const SIPMessage & response
)
{
  /// ignore the notifies if the dialog has already been terminated
  if( GetState() >= StateDisconnected )
    return;

  SIPMessage notify;
  if( !CreateRequestWithinDialog( SIPMessage::Method_NOTIFY, notify ) )
    return;

  Event event( "refer" );
  SubscriptionState state;
  
  if( response.Is1xx() )
    state = SubscriptionState( "active;expires=60" );
  else
    state = SubscriptionState( "terminated;reason=noresource" );

  ContentType type( "message/sipfrag" );
  PString startLine = response.GetStartLine();
  
  notify.SetEvent( event );
  notify.SetSubscriptionState( state );
  notify.SetContentType( type );
  notify.SetBody( startLine );

  SendRequest( notify ); 
}


void CallSession::OnReferSubscriptionState(
  const Event & event,
  const SubscriptionState & state,
  const PString sipFrag
)
{
  m_CallSessionManager.OnReferSubscriptionState( *this, event, state, sipFrag );
}

/// will be called when the call is disconnected
void CallSession::OnDisconnected()
{
}

BOOL CallSession::SendCancel()
{
  if( m_Type == Server ) /// Only a UAC can send cancel
    return FALSE;

  if( GetState() == StateConnected )
    return SendBye();

  if( GetState() > StateConnected )
    return FALSE;

  if( !m_CanSendCancel )
    return FALSE;

  SetState( StateCancelling );

  if( !m_CallSessionManager.GetUserAgent().GetStack().CancelInviteClientTransaction( m_CurrentUACInvite ) )
    return FALSE;

  m_CanSendCancel = FALSE;

  /// wait for the cancel transaction to complete
  return m_UACCancelSync.Wait( 5000 );
}

BOOL CallSession::SendBye()
{
  if( GetState() >= StateDisconnected )
    return FALSE;

  StopSessionKeepAlive();

  if( GetState() < StateConnected && m_Type == Client )
    return SendCancel();
  else if( GetState() < StateConnected && m_Type == Server )
  {
    return SendAcceptByRejection( m_CurrentUASInvite);
  }


  SetState( StateDisconnected );

  SIPMessage bye;
  if( !CreateRequestWithinDialog( SIPMessage::Method_BYE, bye ) )
    return FALSE;

  OnDisconnected();
  m_CallSessionManager.OnDisconnected( *this, bye );

  return SendRequest( bye ); 
}

void CallSession::OnAckFor200OkExpire()
{
  m_CallEndStatusCode = SIPMessage::Code408_RequestTimeout;
  SetCallEndReason( CallSession::ICT_Recv4xx );
  OnDisconnected();
  SIPMessage response;
  m_CurrentUACInvite.CreateResponse( response, SIPMessage::Code408_RequestTimeout );
  m_CallSessionManager.OnDisconnected( *this, response );
}

void CallSession::OnTimerExpire(
  SIPTimerExpire & timerEvent
)
{
  SIPTimerEvent::Type timer = timerEvent.GetTimer(); 
  if( m_Type == Server && timer == SIPTimerEvent::H )
  {
    m_CallEndStatusCode = SIPMessage::Code408_RequestTimeout;
    SetCallEndReason( CallSession::IST_4xxSent );
    OnDisconnected();
    SIPMessage response;
    m_CurrentUASInvite.CreateResponse( response, SIPMessage::Code408_RequestTimeout );
    SendRequest( response );
    m_CallSessionManager.OnDisconnected( *this, response );
  }else if( m_Type == Client && timer == SIPTimerEvent::B )
  {
    m_CallEndStatusCode = SIPMessage::Code408_RequestTimeout;
    SetCallEndReason( CallSession::ICT_Recv4xx );
    OnDisconnected();
    SIPMessage response;
    m_CurrentUACInvite.CreateResponse( response, SIPMessage::Code408_RequestTimeout );
    m_CallSessionManager.OnDisconnected( *this, response );
  }
}

BOOL CallSession::SendRefer(
  const ReferTo & referTo
)
{
  if( GetState() >= StateDisconnected || GetState() < StateConnected )
    return FALSE;

  SIPMessage refer;
  if( !CreateRequestWithinDialog( SIPMessage::Method_REFER, refer ) )
    return FALSE;

  refer.SetReferTo( referTo );

  return SendRequest( refer ); 
}

BOOL CallSession::SendNotify(
  const SubscriptionState & state,
  const ContentType & contentType,
  const PString & body
)
{
  if( GetState() >= StateDisconnected || GetState() < StateConnected )
    return FALSE;
    
  SIPMessage notify;
  if( !CreateRequestWithinDialog( SIPMessage::Method_NOTIFY, notify ) )
    return FALSE;

  notify.SetSubscriptionState( state );
  notify.SetContentType( contentType );
  notify.SetBody( body );

  return SendRequest( notify );
}

BOOL CallSession::SendReinvite()
{
  if( GetState() >= StateDisconnected || GetState() < StateConnected )
    return FALSE;

  SIPMessage invite;
  if( !CreateRequestWithinDialog( SIPMessage::Method_INVITE, invite ) )
    return FALSE;

  PString offer;
  if( !OnRequireSDPOffer( invite ) )
    return FALSE;

  return SendRequest( invite ); 
}

static PString RFC2833Table1Events = "0123456789*#ABCD!";

BOOL CallSession::SendINFODTMF(
  const PString & digit,
  int duration
)
{
  if( GetState() >= StateDisconnected || GetState() < StateConnected )
    return FALSE;

  SIPMessage info;
  if( !CreateRequestWithinDialog( SIPMessage::Method_INFO, info ) )
    return FALSE;

  
  PINDEX toneIndex = RFC2833Table1Events.Find( digit );

  if( toneIndex == P_MAX_INDEX )
    return FALSE;

  info.SetContentType( "application/dtmf-relay" );

  PStringStream body;
  body << "Signal=" << toneIndex << "\r\n";
  body << "Duration=" << duration << "\r\n\r\n";

  info.SetBody( body );
 
  return SendRequest( info );
}

void CallSession::OnReceivedINFODTMF(
  const SIPMessage & info
)
{
  PString digit;
  int duration = 0;

  if( info.GetContentType() *= "application/dtmf-relay" )
  {
    PStringArray params = info.GetBody().Lines();
    if( params.GetSize() > 0 )
    {
      for( PINDEX i = 0; i < params.GetSize(); i++ )
      {
        PStringArray param = params[i].Tokenise( "=" );
        if( param.GetSize() == 2 )
        {
          if( param[0].Trim() *= "status" )
            digit = param[1].Trim();
          else if( param[0].Trim()*= "duration" )
            duration = param[1].Trim().AsInteger();
        }
      }

      PINDEX toneIndex = digit.AsInteger();
      if( toneIndex < RFC2833Table1Events.GetSize() && toneIndex >= 0)
        OnReceivedINFODTMF( RFC2833Table1Events[toneIndex], duration );
    }
  }
}

void CallSession::OnReceivedINFODTMF(
  const PString & digit,
  int duration
)
{
  LOG( LogInfo(), "DTMF: digit=" << digit << " duration=" << duration );
}

void CallSession::ReleaseTransferedCall()
{ 
  if( m_TransferedCall !=NULL )
    delete m_TransferedCall; 
  m_TransferedCall = NULL; 
}

void CallSession::SetTransferedCall( CallSession * call )
{ 
  if( m_TransferedCall !=NULL )
    delete m_TransferedCall;  
  m_TransferedCall = new SIPSession::GCRef( GCCreateRef() ); 
}
 
