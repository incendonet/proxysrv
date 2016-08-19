/*
 *
 * NoneInviteClientTransaction.cxx
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
 * $Log: NoneInviteClientTransaction.cxx,v $
 * Revision 1.14  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.13  2006/11/17 04:22:22  joegenbaclor
 * Added ability to remember core to use via m_UACoreName
 *
 * Revision 1.12  2006/08/29 03:56:44  joegenbaclor
 * 1.  Added GC Reference in SIP Event
 * 2.  Corrected buf in ProcessStackEvent where session is checked against NULL not allowing requests to create new sessions
 *
 * Revision 1.11  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.10  2006/07/06 05:38:11  joegenbaclor
 * Final last minute changes to CBCOM hash support
 *
 * Revision 1.9  2006/03/27 01:25:33  joegenbaclor
 * Some improvements on NICT and NIST state machines
 *
 * Revision 1.8  2006/03/25 18:40:21  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.7  2006/03/22 08:45:52  joegenbaclor
 * More work on simple-proxy and affected classes
 *
 * Revision 1.6  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "NoneInviteClientTransaction.h"
#include "SIPTransactionManager.h"

#define new PNEW

using namespace SIPFSM;

#ifdef _WIN32
#pragma warning(disable:4311)
#endif



NoneInviteClientTransaction::NoneInviteClientTransaction(
  const PString & id,
  SIPTransactionManager & manager
) : SIPTransaction( id, NICT, manager )
{
  m_HasSentInitialRequest = FALSE;
  m_EnableXORHash = FALSE;
}

NoneInviteClientTransaction::~NoneInviteClientTransaction()
{
  LOG_CONTEXT( LogDebug(), m_CallId, "TRANSACTION: " << "(NICT) DESTROYED" );
}

void NoneInviteClientTransaction::OnSIPEvent(
  SIPMessage & message,
  BOOL isSendEvent  
)
{
  if( m_CallId.IsEmpty() )
    m_CallId = message.GetCallId();

  LOG_CONTEXT( LogDebug(), m_CallId, "TRANSACTION: " << "(NICT) " << message.GetStartLine() << " State: " << m_State );

  switch( m_State )
  {
    case Idle:
      HandleStateIdle( message, isSendEvent );
      break;
    case Trying:
      HandleStateTrying( message, isSendEvent );
      break;
    case Proceeding:
      HandleStateProceeding( message, isSendEvent );
      break;
    case Completed:
      HandleStateCompleted( message, isSendEvent );
      break;
  }
}

void NoneInviteClientTransaction::OnTimerEvent(
  SIPTimerEvent & timerEvent  
)
{
  switch( timerEvent.GetType() )
  {
    case SIPTimerEvent::LifeSpanExpire:
      SetState( Terminated );
      m_Manager.OnTimerExpire( timerEvent, *this );
      Terminate();
      break;
    case SIPTimerEvent::E:
      if( m_State == Trying )
      {
        /*If timer E fires while still in this state,
        the timer is reset, but this time with a value of MIN(2*T1, T2).
        When the timer fires again, it is reset to a MIN(4*T1, T2).*/
        m_Manager.OnSendMessageToTransport( m_OpeningRequest, *this );
        PInt64 interval = timerEvent.GetInterval() * 2 > SIP_TIMER_T2 ? SIP_TIMER_T2 : timerEvent.GetInterval() *2;
        StartTimer_E( interval );
      }else if( m_State == Proceeding )
      {
        /*
        If Timer E fires while in the "Proceeding" state, the request MUST be
        passed to the transport layer for retransmission, and Timer E MUST be
        reset with a value of T2 seconds. 
        */
        ///m_TransportSender( m_OpeningRequest, (INT)this );
        m_Manager.OnSendMessageToTransport( m_OpeningRequest, *this );
        StartTimer_E( SIP_TIMER_T2 );
      }
      break;
    case SIPTimerEvent::F:
      if( m_State == Trying )
      {
        /* If Timer F fires while the client transaction is still in the
        "Trying" state, the client transaction SHOULD inform the TU about the
        timeout, and then it SHOULD enter the "Terminated" state. */
        LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateTrying->StateTerminated" );
        SetState( Terminated );
        StopTimer_E();
        m_Manager.OnTimerExpire( timerEvent, *this );
        Terminate();
      }else if( m_State == Proceeding )
      {
        /*If timer F fires while in the
        "Proceeding" state, the TU MUST be informed of a timeout, and the
        client transaction MUST transition to the terminated state.
        */
        LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateTerminated" );
        SetState( Terminated );
        StopTimer_E();
        ///m_TUTimerHandler( timerEvent, (INT)this );
        m_Manager.OnTimerExpire( timerEvent, *this );
        Terminate();
      }
      break;
    case SIPTimerEvent::K:
      /*Once the client transaction enters the "Completed" state, it MUST set
      Timer K to fire in T4 seconds for unreliable transports, and zero
      seconds for reliable transports.
      */
      ///m_TUTimerHandler( timerEvent, (INT)this );
      //m_Manager.OnTimerExpire( timerEvent, *this );
      SetState( Terminated );
      Terminate();
      break;
    default:
      break;
  }
}

void NoneInviteClientTransaction::OnUnknownEvent( 
  SIPEvent * /*event*/ 
)
{
}

void NoneInviteClientTransaction::HandleStateIdle(
  SIPMessage & message,
  BOOL isSendEvent
)
{
  /*
17.1.2 Non-INVITE Client Transaction

17.1.2.1 Overview of the non-INVITE Transaction

   Non-INVITE transactions do not make use of ACK.  They are simple
   request-response interactions.  For unreliable transports, requests
   are retransmitted at an interval which starts at T1 and doubles until
   it hits T2.  If a provisional response is received, retransmissions
   continue for unreliable transports, but at an interval of T2.  The
   server transaction retransmits the last response it sent, which can
   be a provisional or final response, only when a retransmission of the
   request is received.  This is why request retransmissions need to
   continue even after a provisional response; they are to ensure
   reliable delivery of the final response.

   Unlike an INVITE transaction, a non-INVITE transaction has no special
   handling for the 2xx response.  The result is that only a single 2xx
   response to a non-INVITE is ever delivered to a UAC.

17.1.2.2 Formal Description

   The state machine for the non-INVITE client transaction is shown in
   Figure 6.  It is very similar to the state machine for INVITE.

   The "Trying" state is entered when the TU initiates a new client
   transaction with a request.  When entering this state, the client
   transaction SHOULD set timer F to fire in 64*T1 seconds.  The request
   MUST be passed to the transport layer for transmission.  If an
   unreliable transport is in use, the client transaction MUST set timer
   E to fire in T1 seconds.  If timer E fires while still in this state,
   the timer is reset, but this time with a value of MIN(2*T1, T2).
   When the timer fires again, it is reset to a MIN(4*T1, T2).  This
   process continues so that retransmissions occur with an exponentially
   increasing interval that caps at T2.  The default value of T2 is 4s,
   and it represents the amount of time a non-INVITE server transaction
   will take to respond to a request, if it does not respond
   immediately.  For the default values of T1 and T2, this results in
   intervals of 500 ms, 1 s, 2 s, 4 s, 4 s, 4 s, etc.

   If Timer F fires while the client transaction is still in the
   "Trying" state, the client transaction SHOULD inform the TU about the
   timeout, and then it SHOULD enter the "Terminated" state.  If a
   provisional response is received while in the "Trying" state, the
   response MUST be passed to the TU, and then the client transaction
   SHOULD move to the "Proceeding" state.  If a final response (status
   codes 200-699) is received while in the "Trying" state, the response
   MUST be passed to the TU, and the client transaction MUST transition
   to the "Completed" state.

   If Timer E fires while in the "Proceeding" state, the request MUST be
   passed to the transport layer for retransmission, and Timer E MUST be
   reset with a value of T2 seconds.  If timer F fires while in the
   "Proceeding" state, the TU MUST be informed of a timeout, and the
   client transaction MUST transition to the terminated state.  If a
   final response (status codes 200-699) is received while in the
   "Proceeding" state, the response MUST be passed to the TU, and the
   client transaction MUST transition to the "Completed" state.

   Once the client transaction enters the "Completed" state, it MUST set
   Timer K to fire in T4 seconds for unreliable transports, and zero
   seconds for reliable transports.  The "Completed" state exists to
   buffer any additional response retransmissions that may be received
   (which is why the client transaction remains there only for
   unreliable transports).  T4 represents the amount of time the network
   will take to clear messages between client and server transactions.
   The default value of T4 is 5s.  A response is a retransmission when
   it matches the same transaction, using the rules specified in Section
   17.1.3.  If Timer K fires while in this state, the client transaction
   MUST transition to the "Terminated" state.

   Once the transaction is in the terminated state, it MUST be destroyed
   immediately.
*/

  
  /*
  The state machine for the INVITE client transaction is shown in
  Figure 5.  The initial state, "calling", MUST be entered when the TU
  initiates a new client transaction with an INVITE request.  The
  client transaction MUST pass the request to the transport layer for
  transmission (see Section 18).  If an unreliable transport is being
  used, the client transaction MUST start timer A with a value of T1.
  If a reliable transport is being used, the client transaction SHOULD
  NOT start timer A (Timer A controls request retransmissions).  For
  any transport, the client transaction MUST start timer B with a value
  of 64*T1 seconds (Timer B controls transaction timeouts).
  */


  StartLine startLine;
  message.GetStartLine( startLine );

  if( !isSendEvent && message.IsRequest() )
  {
    // check if XOR hash is enabled
    m_EnableXORHash = message.IsXORHashSet();
    m_UACoreName = message.GetUACoreName();
    SetOpeningRequest( message );
    PString transport;
    PAssert( startLine.IsRequest(), "NoneInviteClientTransaction got none request event on State Idle" );
    m_Manager.OnSendMessageToTransport( message, *this );
  }else if( message.IsRequest() && isSendEvent && !m_HasSentInitialRequest )
  {
    m_HasSentInitialRequest = TRUE;
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateIdle->StateTrying(" << startLine << ")" );
    SetState( Trying );
    StartTimer_E();
    StartTimer_F();
  }else if( !message.Is1xx() && !message.IsRequest() )  /// we received a 2xx while in state idle ?
  {
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateTrying->StateCompleted" );
    SetState( Completed );
    StopTimer_E();
    StopTimer_F();
    m_Manager.OnReceivedMessage( message, *this );

#if NICT_USE_TIMER_K
    StartTimer_K();
#else
    SetState( Terminated );
    Terminate();
#endif
  }
}

void NoneInviteClientTransaction::HandleStateTrying(
  SIPMessage & message,
  BOOL isSendEvent
)
{

   /*If Timer F fires while the client transaction is still in the
   "Trying" state, the client transaction SHOULD inform the TU about the
   timeout, and then it SHOULD enter the "Terminated" state.  If a
   provisional response is received while in the "Trying" state, the
   response MUST be passed to the TU, and then the client transaction
   SHOULD move to the "Proceeding" state.  If a final response (status
   codes 200-699) is received while in the "Trying" state, the response
   MUST be passed to the TU, and the client transaction MUST transition
   to the "Completed" state.*/

  if( isSendEvent || message.IsRequest()  )
    return;

  if( message.Is1xx() )
  {
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateTrying->StateProceeding" );
    SetState( Proceeding );
    m_Manager.OnReceivedMessage( message, *this );
  }else
  {
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateTrying->StateCompleted" );
    
    StopTimer_E();
    StopTimer_F();
    m_Manager.OnReceivedMessage( message, *this );
#if NICT_USE_TIMER_K
    SetState( Completed );
    StartTimer_K();
#else
    SetState( Terminated );
    Terminate();
#endif
  }
  
}

void NoneInviteClientTransaction::HandleStateProceeding(
  SIPMessage & message,
  BOOL isSendEvent
)
{

  /*If Timer E fires while in the "Proceeding" state, the request MUST be
   passed to the transport layer for retransmission, and Timer E MUST be
   reset with a value of T2 seconds.  If timer F fires while in the
   "Proceeding" state, the TU MUST be informed of a timeout, and the
   client transaction MUST transition to the terminated state.  If a
   final response (status codes 200-699) is received while in the
   "Proceeding" state, the response MUST be passed to the TU, and the
   client transaction MUST transition to the "Completed" state.
   */

  if( isSendEvent && message.IsRequest()  )
    return;

 
  m_Manager.OnReceivedMessage( message, *this );

  if( !message.Is1xx() )  /// final response
  {
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateCompleted" );
    SetState( Completed );
    StopTimer_E();
    StopTimer_F();  
#if NICT_USE_TIMER_K
    StartTimer_K();
#else
    SetState( Terminated );
    Terminate();
#endif
  }
 
}

void NoneInviteClientTransaction::HandleStateCompleted(
  SIPMessage & message,
  BOOL isSendEvent
)
{
  /*
   Once the client transaction enters the "Completed" state, it MUST set
   Timer K to fire in T4 seconds for unreliable transports, and zero
   seconds for reliable transports.  The "Completed" state exists to
   buffer any additional response retransmissions that may be received
   (which is why the client transaction remains there only for
   unreliable transports).  T4 represents the amount of time the network
   will take to clear messages between client and server transactions.
   The default value of T4 is 5s.  A response is a retransmission when
   it matches the same transaction, using the rules specified in Section
   17.1.3.  If Timer K fires while in this state, the client transaction
   MUST transition to the "Terminated" state.

   Once the transaction is in the terminated state, it MUST be destroyed
   immediately.
   */


  if( isSendEvent && !message.IsRequest() )
  {
    m_Manager.OnReceivedMessage( message, *this );
  }
}





