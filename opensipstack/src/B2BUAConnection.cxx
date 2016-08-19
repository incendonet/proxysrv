/*
 *
 * B2BUAConnection.cxx
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
 * $Log: B2BUAConnection.cxx,v $
 * Revision 1.47  2007/01/22 10:01:58  joegenbaclor
 * Fixed bug in handling Notifies
 *
 * Revision 1.46  2007/01/19 08:21:12  joegenbaclor
 * Modified Redirect behavior.  Second leg would now process redirect instead of the
 *  session relaying 3xx to UAC
 *
 * Revision 1.45  2007/01/10 08:07:16  rcolobong
 * 1. Add new method for OnSendB2BConnect Call Interface.
 * 2. OnSendB2BConnect is supported in B2BUserAgent, B2BUACall and B2BUAConnection
 *
 * Revision 1.44  2006/12/19 10:14:26  rcolobong
 * 1. Add method to insert Warning header in 200 Ok response to inbound
 * 2. Alerting timeout will now do failover
 *
 * Revision 1.43  2006/12/17 13:30:58  joegenbaclor
 * Added call attempt index in call-id string to avoid call-id collision during failovers
 *
 * Revision 1.42  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.41  2006/11/22 00:05:52  joegenbaclor
 * Bug fixes for multi-listener support
 *
 * Revision 1.40  2006/11/16 01:29:33  rcolobong
 * Notify CallHandler during Authentication Required
 *
 * Revision 1.39  2006/11/10 07:55:35  rcolobong
 * 1. Support Fork Invites using B2BUAForkCalls
 * 2. Handle Cancel events during Forking
 *
 * Revision 1.38  2006/10/30 08:35:55  joegenbaclor
 * Removed more references to presence namespace
 *
 * Revision 1.37  2006/10/25 09:07:46  rcolobong
 * Fix bug where leg1 and leg2 should be set to NULL after OnDestroySession
 *
 * Revision 1.36  2006/10/23 15:44:58  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.35  2006/10/11 04:49:47  rcolobong
 * 1. Add support for Notify During Refer
 * 2. Fixed bug where 3xx invoke CallHandler OnRejected
 *
 * Revision 1.34  2006/10/05 13:03:54  rcolobong
 * 1. Fix bug regarding OnRequireCallTransferId calling B2BUA OnRequireRedirectionId Method
 * 2. Add flag in OnSetupOutbound to check if Connection State is already disconnected
 *
 * Revision 1.33  2006/09/25 04:43:30  rcolobong
 * 1. Fix bug in 415 Unsupported Media where CallSession::m_ICTState is not properly initialized during construction.
 * 2. Remove the temporary fix for the 415 Unsupported Media where we use Semaphore to wait for second leg SDP in OnRequireSDPAnswer
 *
 * Revision 1.32  2006/09/22 08:52:45  joegenbaclor
 * Temporary fix for OnRequireSDPAnswer to use a semaphore to wait for second leg SDP
 *
 * Revision 1.31  2006/09/20 11:05:26  joegenbaclor
 * Flagged sending of Session Keep Alive.  Defaults to FALSE
 *
 * Revision 1.30  2006/09/19 10:47:12  rcolobong
 * Disable temporarily session keep alive
 *
 * Revision 1.29  2006/09/18 09:35:16  joegenbaclor
 * Completed initial implementation of RTP media aggregation
 *
 * Revision 1.28  2006/09/18 03:28:55  joegenbaclor
 * 1. More work on media aggregation
 * 2. Check sock->GetPort() for NULL in SIPTransportManager::GetListenerAddress() (thanks to
 *  Vamsi Pottangi for reporting the bug)
 * 3.  Added reference size check to GCObject::ReleaseRef()
 *
 * Revision 1.27  2006/09/15 15:31:39  joegenbaclor
 * Changed Log levels to decrease log size.
 * Changed default session keep alaive interval from 10 seconds to two minutes
 *
 * Revision 1.26  2006/09/15 14:45:22  joegenbaclor
 * Flagged SetSessionState of B2BConnection so it wont propagate events when connection
 *  is already destroyed
 *
 * Revision 1.25  2006/09/15 12:29:45  joegenbaclor
 * Flagged OnDestroySession of B2BConnection so it wont be called twice from different
 *  threads.
 *
 * Revision 1.24  2006/09/08 04:39:17  joegenbaclor
 * Implemented INFO Session Keep-alive
 *
 * Revision 1.23  2006/09/02 17:58:20  joegenbaclor
 * Corrected compile errors in GCC regarding euqation of NULL to GCRef.
 *
 * Revision 1.22  2006/09/01 13:15:58  rcolobong
 * Add Logging during Construction and Destruction of B2BUAConnection
 *
 * Revision 1.21  2006/08/30 08:45:41  rcolobong
 * 1. Update handling for connection and alerting timeout
 * 2. Fix bug where leg1 is destroyed before calling DestroyConnection
 *
 * Revision 1.20  2006/08/30 03:42:05  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.19  2006/08/29 10:47:23  rcolobong
 * *** empty log message ***
 *
 * Revision 1.18  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.17  2006/08/26 14:31:44  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.16  2006/08/16 04:14:33  rcolobong
 * Invoke StopConnectionTimeout after receiving 2xx response
 *
 * Revision 1.15  2006/08/04 05:31:29  rcolobong
 * 1.  Add Alerting and Connecting Timer
 * 2.  Support failover
 * 3.  Add Alerting, CallAbandoned, Rejected, Disconnected, Connected
 *
 * Revision 1.14  2006/07/19 02:19:21  joegenbaclor
 * 1.  Bug fixes concerning B2BUA destruction during during 3xx, 4xx, 5xx, 6xx local response
 * 2.  Fixed bug where Route header is never removed from outbound Invite resulting that may result to a loop when
 *      Inbound Invite has a Route header present
 * 3.  Added RemoveAllRoutes() in SIPMessage
 *
 * Revision 1.13  2006/07/13 06:53:05  joegenbaclor
 * 1.  Introduced Sanity checks for incoming SIP Message
 * 2.  Corrected bug in SIPURI where enum scheme is not getting recognized.
 * 3.  Added ENUM support to routing
 * 4.  Introduced global routing flag to indicate when and not to rewrite outbound To URI's
 *
 * Revision 1.12  2006/07/06 05:38:11  joegenbaclor
 * Final last minute changes to CBCOM hash support
 *
 * Revision 1.11  2006/06/29 05:09:13  joegenbaclor
 * Changed OnOutgoingCall invite parameter from const to none const to give a
 * chance for applications to modify the outbound invite before being sent to the transport
 *
 * Revision 1.10  2006/06/28 14:14:59  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.9  2006/06/23 04:44:04  joegenbaclor
 * SendB2BRefer() added to B2BUACall
 *
 * Revision 1.8  2006/06/22 08:27:57  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.7  2006/06/21 14:29:11  joegenbaclor
 * This upload marks the first successful call by OpenB2BUA
 *
 * Revision 1.6  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
 *
 * Revision 1.5  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.4  2006/06/14 08:43:38  joegenbaclor
 * Initial upload of OpenB2BUA applcation and related classes
 *
 * Revision 1.3  2006/04/11 00:18:54  joegenbaclor
 * More work on B2BUA class structures
 *
 * Revision 1.2  2006/04/10 01:09:03  joegenbaclor
 * Implemented Create methods fo B2BUA sessions
 *
 * Revision 1.1  2006/04/08 06:27:13  joegenbaclor
 * Initial upload of B2BUA classes
 *
 *
 */

#include "B2BUAConnection.h"
#include "B2BUAForkCalls.h"

#define new PNEW

using namespace B2BUA;

PAtomicInteger B2BUAConnection::m_ConnectionCounter = 0;
DWORD B2BUAConnection::m_ConnectionTimeout = 0;
DWORD B2BUAConnection::m_AlertingTimeout = 0;

B2BUAConnection::B2BUAConnection(
  B2BUAEndPoint & ep,
  const PString & sessionId
) : SIPSession( ep, sessionId )
{
  m_LogTag = "CONNECTION";
  m_RewriteToURI = FALSE;

  m_Leg1Call = NULL;
  m_Leg2Call = NULL;
  m_ForkCalls = new B2BUAForkCalls( *this );

  m_SessionState = Idle;
  m_CallSessions.DisallowDeleteObjects();

  ++m_ConnectionCounter;
  m_MediaProxyIfPrivate = FALSE;  /// Always Proxy Media by default
  m_EnableForkCalls = FALSE;

  LOG_CONTEXT( LogInfo(), sessionId, "B2BUAConnection Created 0x" << std::hex << this 
    << " (Active Count: " << std::dec << m_ConnectionCounter << ")" );

#if HAS_OPAL
  m_HasStartedMediaStream = FALSE;
#endif

  m_ConnectionDestroyed = FALSE;
  m_SendSessionKeepAlive = FALSE;
  m_HasReceivedSDP = FALSE;
  m_OutboundAttempts = 0;
}

B2BUAConnection::~B2BUAConnection()
{
  m_CallSessions.AllowDeleteObjects();
  --m_ConnectionCounter;
  LOG_CONTEXT( LogInfo(), GetSessionId(), "B2BUAConnection Destroyed 0x" << std::hex << this 
    << " (Active Count: " << std::dec << m_ConnectionCounter  << ")" );
}

BOOL B2BUAConnection::AttachCall(
  const PString & id,
  B2BUACall * call 
)
{
  PWaitAndSignal lock( m_CallSessionsMutex );
  
  if( m_Leg1Call == NULL )
  {
    SetInterfacePort( call->GetInterfacePort() );
    m_Leg1Call = call;
  }else
  {
    m_Leg2Call = call;
  }

  call->SetB2BUAConnection( this );
  return m_CallSessions.SetAt( id, new SIPSession::GCRef( call->GCCreateRef() ) );
}

SIPSession::GCRef * B2BUAConnection::DetachCall(
  const PString & id
)
{
  PWaitAndSignal lock( m_CallSessionsMutex );
  return m_CallSessions.RemoveAt( id );
}

SIPSession::GCRef * B2BUAConnection::FindCall(
  const PString & id
)
{
  PWaitAndSignal lock( m_CallSessionsMutex );
  return m_CallSessions.GetAt( id );
}


BOOL B2BUAConnection::OnIncomingCall(
  B2BUACall & call,
  const SIPMessage & invite
)
{
  B2BUserAgent & b2bua = 
    (B2BUserAgent &)GetSessionManager().GetUserAgent();

  return b2bua.OnIncomingCall( *this, call, invite );
}

void B2BUAConnection::OnRequireRedirectionId(
  B2BUACall & call,
  PString & redirectionId
)
{
  B2BUserAgent & b2bua = 
    (B2BUserAgent &)GetSessionManager().GetUserAgent();

  b2bua.OnRequireRedirectionId( *this, call, redirectionId );
}

void B2BUAConnection::OnRequireCallTransferId(
  B2BUACall & call,
  PString & redirectionId
)
{
  B2BUserAgent & b2bua = 
    (B2BUserAgent &)GetSessionManager().GetUserAgent();

  b2bua.OnRequireCallTransferId( *this, call, redirectionId );
}

BOOL B2BUAConnection::OnIncomingSDPOffer(
  B2BUACall & call,
  const SIPMessage & sdp
)
{
  B2BUserAgent & b2bua = 
    (B2BUserAgent &)GetSessionManager().GetUserAgent();
  return b2bua.OnIncomingSDPOffer( *this, call, sdp );
}

/// your last chance to produce an answer to the offer.  
/// This will be called either before sending 183, 200 or ACK 
BOOL B2BUAConnection::OnRequireSDPAnswer(
  B2BUACall & call,
  const SIPMessage & offer,
  SIPMessage & answer
)
{
  B2BUserAgent & b2bua = 
    (B2BUserAgent &)GetSessionManager().GetUserAgent();
  return b2bua.OnRequireSDPAnswer( *this, call, offer, answer );
}

BOOL B2BUAConnection::OnSendB2BConnect(
  B2BUACall &,
  const SIPMessage & message
)
{
  B2BUserAgent & b2bua = 
    (B2BUserAgent &)GetSessionManager().GetUserAgent();
  return b2bua.OnSendB2BConnect( *this, *m_Leg2Call, message );
}

/// this will be called if an offer is not received in INVITE or when we are initiating the call
BOOL B2BUAConnection::OnRequireSDPOffer(
  B2BUACall & call,
  SIPMessage & offer
)
{
  B2BUserAgent & b2bua = 
    (B2BUserAgent &)GetSessionManager().GetUserAgent();
  return b2bua.OnRequireSDPOffer( *this, call, offer );
}

void B2BUAConnection::OnSetupOutbound( 
  const SIPMessage & inboundInvite 
)
{
  if( m_SessionState == Disconnected )
    return;

  B2BUserAgent & b2bua = 
    (B2BUserAgent &)GetSessionManager().GetUserAgent();

  SIPMessage outboundInvite = inboundInvite;
  outboundInvite.ClearInternalHeaders();

  StartAlertingTimer();
  StartConnectionTimer();
  b2bua.OnSetupOutbound( outboundInvite, *this );
}

void B2BUAConnection::OnSessionKeepAliveTimeout(
  B2BUACall & call 
)
{
  SIPMessage msg; /// dummy event
  DestroyConnection( msg );
}

/** NIST Callbacks */
void B2BUAConnection::NIST_OnReceivedRegister(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NIST_OnReceivedRefer(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NIST_OnReceivedBye(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NIST_OnReceivedOptions(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NIST_OnReceivedInfo(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NIST_OnReceivedMessage(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NIST_OnReceivedCancel(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NIST_OnReceivedNotify(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NIST_OnReceivedSubscribe(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NIST_OnReceivedUnknown(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}
void B2BUAConnection::IST_OnReceivedMoreInvite(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

/** NICT CALLBACKS */


void B2BUAConnection::NICT_OnReceived1xx(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NICT_OnReceived2xx(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NICT_OnReceived3xx(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NICT_OnReceived4xx(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NICT_OnReceived5xx(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NICT_OnReceived6xx(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::NICT_OnReceivedUnknown(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

/** ICT CALLBACKS */

void B2BUAConnection::ICT_OnReceived1xx(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::ICT_OnReceived2xx(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::ICT_OnReceived3xx(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::ICT_OnReceived4xx(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::ICT_OnReceived5xx(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::ICT_OnReceived6xx(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::ICT_OnReceivedUnknown(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  SetSessionState( call, msg );
}

void B2BUAConnection::SetSessionState(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  PWaitAndSignal lock( m_SessionStateMutex );

  if( m_ConnectionDestroyed )
    return;
  
  if( m_SessionState < Connected )
  {
    OnHandlePreConnectState( call, msg );
  }else if( m_SessionState == Connected )
  {
    OnHandleStateConnected( call, msg );
  }else if( m_SessionState == Disconnected )
  {
    OnHandleStateDisconnected( call, msg );
  }else if( m_SessionState == Terminated )
  {
    OnHandleStateTerminated( call, msg );
  }else
  {
    PAssertAlways( PLogicError );
  }
}

/*
void B2BUAConnection::SetSessionState(
  B2BUACall & call,
  int event
)
{
  PWaitAndSignal lock( m_SessionStateMutex );
  
  if( m_SessionState == Idle )
  {
  }else if( m_SessionState == Trying )
  {
  }else if( m_SessionState == Proceeding )
  {
  }else if( m_SessionState == Connected )
  {
  }else if( m_SessionState == Disconnected )
  {
  }else if( m_SessionState == Terminated )
  {
  }else
  {
    PAssertAlways( PLogicError );
  }
}
*/
void B2BUAConnection::OnHandlePreConnectState( 
  B2BUACall & call,
  const SIPMessage & message
)
{
  SIPMessage msg = message;

  B2BUserAgent& b2bUA = (B2BUserAgent&)GetSessionManager().GetUserAgent();

  if( call.GetLegIndex() == 1 )
  {
    if( msg.GetCSeqMethod() *= "INVITE" )
    {
      if( msg.Is1xx() )
      {
        if( msg.GetStatusCode() == 100 )
        {
          m_SessionState = Trying;
          return;
        }else
        {
          m_SessionState = Proceeding;
          // ok now we are ringing
          // stop connecting timer and start
          // alerting timer
          StopAlertingTimer();
          LOG( LogInfo(), "Connection: Alerting / SDP = " << ( msg.HasSDP() ? "TRUE" : "FALSE" ) );
          b2bUA.OnAlerting( *this, call, msg );
          if( !m_HasReceivedSDP )
            m_HasReceivedSDP = msg.HasSDP();
          

        }
      }else if( msg.Is2xx() )
      {
        m_SessionState = Connected;
        // ok we receive 2xx value
        // stop alerting timer
        StopAlertingTimer();

        // we should also stop connection timer
        // due to it is possible that it
        // will directly send 200 without
        // sending 183 - alerting
        StopConnectionTimer(); 
        b2bUA.OnConnected( *this, call, msg );

        // check if we would post a warning message
        if( !m_200OkWarning.GetHeaderBody().IsEmpty() )
        {
          // for now we would overrider warning messages
          // with our warning message
          msg.SetWarning( m_200OkWarning );
        }

        LOG( LogInfo(), "Connection: Connected / SDP = " << ( msg.HasSDP() ? "TRUE" : "FALSE" ) );
        /// Start the session keep alive timer

        if( m_SendSessionKeepAlive )
        {
          GetLeg1Call()->StartSessionKeepAlive( 120000 );
          GetLeg2Call()->StartSessionKeepAlive( 120000 );
        }

      }else if( !msg.IsRequest() ) // 3xx - 6xx
      {
        // check if we should fail over
        if( !msg.Is3xx() && 
            msg.GetStatusCode() != 401 &&
            msg.GetStatusCode() != 407 && 
            !m_HasReceivedSDP )
        {
          if( m_Routes.GetSize() > 1 )
          {
            // destroy first leg 2 connection
            if( m_Leg2Call )
            {
              m_Leg2Call->Destroy();
              m_Leg2Call = NULL;
            }

            m_Routes.RemoveAt( 0 );

            // Change to FailOver method
            LOG( LogInfo(), "Connection: Failing over to other routes: Error " 
              << msg.GetStatusCode() );
            OnSetupOutbound( m_Leg1Call->GetRequest() );
            return;
          }
        } 

        // if message status is not 3xx then 
        // we should notify ua for rejection
        if( !msg.Is3xx() )
        {
          LOG( LogInfo(), "Connection: Rejected / Code = " << msg.GetStatusCode() );
          m_SessionState = Disconnected;
          b2bUA.OnRejected( *this, call, msg );
        }else
        {
          /// just return as if nothing happend, call is now about to be redirected
            
          return;
        }
      }

      GetLeg1Call()->AnswerB2BCall( call, msg );
    }
  }else if( call.GetLegIndex() == 0 )
  {
    if( msg.IsCancel() )
    {
      SIPMessage ok;
      msg.CreateResponse( ok, SIPMessage::Code200_Ok );
      call.SendRequest( ok );
      SIPMessage reqCancelled;
      call.GetRequest().CreateResponse( reqCancelled, SIPMessage::Code487_RequestCancelled );
      call.SendRequest( reqCancelled );
      
      if( m_SessionState < Connected )
      {
        if( m_EnableForkCalls && m_ForkCalls )
        {
          m_ForkCalls->CancelForkCalls();
        } else 
        {
          B2BUACall * leg2Call = GetLeg2Call();
          if( leg2Call )
            leg2Call->SendCancel();
        }
      }

      m_SessionState = Disconnected;
      LOG( LogInfo(), "Connection: Abandoned" );
      b2bUA.OnCallAbandoned( *this, call, msg );
    }     
  }else
  {
    PAssertAlways( PLogicError );
  }

  if( m_SessionState == Disconnected )
  {
    // if we receive disconnect then
    // stop all timer
    StopConnectionTimer();
    StopAlertingTimer();
    DestroyConnection( msg );    
  } 
}


void B2BUAConnection::OnHandleStateConnected( 
  B2BUACall & call,
  const SIPMessage & msg
)
{
  B2BUACall * otherEnd = NULL;
  if( call.GetLegIndex() == 0 )
    otherEnd = GetLeg2Call();
  else
    otherEnd = GetLeg1Call();


  if( msg.IsCancel() )
  { // cancel is not okayed automatically
    // by the CallSession so we need to ok it here
    // since we are already in connected state,
    // Cancel is already late so just do nothing but respond
    SIPMessage ok;
    msg.CreateResponse( ok, SIPMessage::Code200_Ok );
    call.SendRequest( ok );
  }else if( msg.IsBye() )
  {
    otherEnd->SendBye();
    m_SessionState = Disconnected;
  }else if( msg.IsInfo() || msg.IsMessage() )
  {
    SIPMessage ok;
    msg.CreateResponse( ok, SIPMessage::Code200_Ok );
    call.SendRequest( ok );
    
    SIPMessage clone;
    if( CloneRequestWithinDialog( msg, clone ) )
      otherEnd->SendRequest( clone );

  }else if( msg.IsInvite() && msg.HasSDP() )
  {
    otherEnd->SendReinvite();
  }else if( msg.IsRefer() )
  {
    SIPMessage xferResponse;
    if( !otherEnd->SendB2BRefer( call, msg ) )
    {
      msg.CreateResponse( xferResponse, SIPMessage::Code406_NotAcceptable );
      call.SendRequest( xferResponse );
    } else
    {
      msg.CreateResponse( xferResponse, SIPMessage::Code202_Accepted );
      SendRequest( xferResponse );
      StartAutoDestructTimer( 90000 );
    }
  }else if( msg.IsNotify() )
  {
    SIPMessage notifyResponse;
    if( !otherEnd->SendB2BNotify( call, msg ) )
    {
      msg.CreateResponse( notifyResponse, SIPMessage::Code406_NotAcceptable );
      call.SendRequest( notifyResponse );
    }
  }

  if( m_SessionState == Disconnected )
  {
    B2BUserAgent& b2bUA = (B2BUserAgent&)GetSessionManager().GetUserAgent();
    b2bUA.OnDisconnected( *this, call, msg );
    LOG( LogInfo(), "Connection: Disconnected" );
    DestroyConnection( msg );
  }  
}

void B2BUAConnection::OnHandleStateDisconnected( 
  B2BUACall & call,
  const SIPMessage & msg
)
{
  if( msg.IsRequest() )
  {
    SIPMessage response;
    msg.CreateResponse( response, SIPMessage::Code481_TransactionDoesNotExist );
    call.SendRequest( response );
  }
}

void B2BUAConnection::OnHandleStateTerminated( 
  B2BUACall & call,
  const SIPMessage & msg
)
{
  if( msg.IsRequest() )
  {
    SIPMessage response;
    msg.CreateResponse( response, SIPMessage::Code481_TransactionDoesNotExist );
    call.SendRequest( response );
  }
}

void B2BUAConnection::OnSessionEvent(
  int event,
  const SIPMessage & eventMsg
)
{
  switch( event )
  {
    case SetupOutbound:
      OnSetupOutbound( eventMsg );
      break;
    case DestroySession:
      Destroy();
      break;
    case ErrorNoRoute:
      break;
    case ErrorRouteResolution:
      break;
  }
}

void B2BUAConnection::DestroyConnection()
{
  SIPMessage dummy;
  DestroyConnection( dummy );
}

void B2BUAConnection::DestroyConnection(
  const SIPMessage & msg
)
{
  EnqueueSessionEvent( new SIPSessionEvent( *this, DestroySession, msg ) );
}

void B2BUAConnection::OnDestroySession()
{
  PWaitAndSignal lock( m_SessionStateMutex );

  if( m_ConnectionDestroyed )
    return;

  m_ConnectionDestroyed = TRUE;
  
  // Required data for Sending Auth
  // required before leg 1 is destroyed
  SIPMessage previousInvite = m_Leg1Call->GetRequest();
  ProxyAuthenticate auth = m_Leg1Call->GetProxyAuthenticate();

  if( m_SessionState == Connected )
  {
    m_Leg1Call->SendBye();
    if( m_Leg2Call != NULL )
      m_Leg2Call->SendBye();
  }else if( m_SessionState < Connected )
  {
    if( m_Leg1Call->GetCallAnswerResponse() 
      != CallSession::DisconnectWithUnauthorized && 
      m_Leg1Call->GetCallAnswerResponse() != CallSession::DontDeny
    )
    {
      SIPMessage reject;
      previousInvite.CreateResponse( reject, (SIPMessage::StatusCodes)m_Leg1Call->GetCallAnswerResponse() );
      SendRequest( reject );

      if( m_EnableForkCalls )
      {
        m_ForkCalls->CancelForkCalls();
      }
      else if( m_Leg2Call != NULL )
      {
        m_Leg2Call->SendCancel();
      }
    }
  }

  m_Leg1Call->CloseMediaStreams();
  if( m_Leg2Call != NULL )
    m_Leg2Call->CloseMediaStreams();

  m_Leg1Call->Destroy();
  if( m_Leg2Call != NULL )
    m_Leg2Call->Destroy();

  // Handle here for sending Proxy Authentication Required
  // Problem: Causes was Race Condition
  // Todo: Should be fix race condition and put
  // this method inside on B2BUA::OnIncomingCall
  if( m_Leg1Call->GetCallAnswerResponse() 
    == CallSession::DisconnectWithUnauthorized )
  {
    SIPMessage authChallenge;

    previousInvite.CreateResponse( authChallenge, 
      SIPMessage::Code407_ProxyAuthenticationRequired );

    authChallenge.SetProxyAuthenticate( auth );
    
    B2BUserAgent& b2bUA = (B2BUserAgent&)GetSessionManager().GetUserAgent();
    b2bUA.OnCallAbandoned( *this, *m_Leg1Call, authChallenge );

    SendRequest( authChallenge );
  }

  if( m_ForkCalls )
  {
    delete m_ForkCalls;
    m_ForkCalls = NULL;
  }

  m_CallSessions.AllowDeleteObjects();
  m_CallSessions.RemoveAll();

  m_Leg1Call = NULL;
  m_Leg2Call = NULL;
 
}

void B2BUAConnection::StartConnectionTimer()
{
  if( m_ConnectionTimeout > 0 )
  {
    m_ConnectionTimer = PTimer( m_ConnectionTimeout );
    
    m_ConnectionTimer.SetNotifier( PCREATE_NOTIFIER( OnConnectionTimeout ) );
    m_ConnectionTimer.Resume();
  }
}

void B2BUAConnection::StopConnectionTimer()
{
  m_ConnectionTimer.Stop();
}

void B2BUAConnection::StartAlertingTimer()
{
  if( m_AlertingTimeout > 0 )
  {
    m_AlertingTimer = PTimer( m_AlertingTimeout );
    
    m_AlertingTimer.SetNotifier( PCREATE_NOTIFIER( OnAlertingTimeout ) );
    m_AlertingTimer.Resume();
  }
}

void B2BUAConnection::StopAlertingTimer()
{
  m_AlertingTimer.Stop();
}


void B2BUAConnection::OnConnectionTimeout( 
  PTimer&,
  INT
)
{
  CallSession::GCRef autoRef; 
  B2BUACall* leg1Call = m_Leg1Call->GCCreateRef< B2BUACall >( autoRef, 
    "B2BUAConnection::OnConnectionTimeout" );

  if( leg1Call )
  {
    leg1Call->SetCallAnswerResponse( CallSession::DisconnectWithRequestTimeout );
    DestroyConnection( leg1Call->GetRequest() );
  }
}

void B2BUAConnection::OnAlertingTimeout( 
  PTimer&,
  INT 
)
{
  StopConnectionTimer();

  CallSession::GCRef autoRef; 
  B2BUACall* leg2Call = m_Leg2Call->GCCreateRef< B2BUACall >( autoRef, 
    "B2BUAConnection::OnAlertingTimeout" );

  if( leg2Call )
  {
    //leg1Call->SetCallAnswerResponse( CallSession::DisconnectWithRequestTimeout );
    SIPMessage timeout;
    leg2Call->GetRequest().CreateResponse( timeout, SIPMessage::Code408_RequestTimeout );
    OnHandlePreConnectState( *leg2Call, timeout );
  }
}








