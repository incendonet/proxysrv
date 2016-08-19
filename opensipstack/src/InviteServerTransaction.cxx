/*
 *
 * InviteServerTransaction.cxx
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
 * $Log: InviteServerTransaction.cxx,v $
 * Revision 1.17  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.16  2006/11/17 04:22:22  joegenbaclor
 * Added ability to remember core to use via m_UACoreName
 *
 * Revision 1.15  2006/09/01 06:51:15  joegenbaclor
 * Bulk Commit:
 *
 * 1.  A bug is discovered in SIP Transaction Destructor where destruction of the SIPTimers may cause a deadlock on the timer manager effectivily freezing the entire message Queue System.  This was fixed by introducing a separate function to destroy the timers on receipt of the final event just before the transaction is queued for deletion.
 *
 * 2.  There is bug in Invite Server Transaction where ACK transaction is added to the Ack Transaction Pool after sending og none 2xx final response.  This new transaction is never removed from the pool after Timer_H has fired.  this may result to a bad pointer if ACK is received after the timer expired since the transaction may have been deleted by then.
 *
 * 3.  A few tweaks in GC GetName func to return the Class Name if GC NAme is empty.
 *
 * Revision 1.14  2006/08/29 03:56:44  joegenbaclor
 * 1.  Added GC Reference in SIP Event
 * 2.  Corrected buf in ProcessStackEvent where session is checked against NULL not allowing requests to create new sessions
 *
 * Revision 1.13  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.12  2006/08/06 02:32:08  joegenbaclor
 * Corrected bug regarding blank call-ids for UnknownTransaction events
 *
 * Revision 1.11  2006/07/06 05:38:11  joegenbaclor
 * Final last minute changes to CBCOM hash support
 *
 * Revision 1.10  2006/03/27 01:25:33  joegenbaclor
 * Some improvements on NICT and NIST state machines
 *
 * Revision 1.9  2006/03/24 15:42:18  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.8  2006/03/22 08:45:52  joegenbaclor
 * More work on simple-proxy and affected classes
 *
 * Revision 1.7  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "InviteServerTransaction.h"
#include "SIPTransactionManager.h"

#define new PNEW

using namespace SIPFSM;

#ifdef _WIN32
#pragma warning(disable:4311)
#endif

InviteServerTransaction::InviteServerTransaction(
  const TransactionId & transactionId,
  SIPTransactionManager & manager
  ) : SIPTransaction( transactionId, IST, manager )
{
  m_EnableXORHash = FALSE;
}

InviteServerTransaction::~InviteServerTransaction()
{
  LOG_CONTEXT( LogDebug(), m_CallId, "TRANSACTION: " << "(IST) DESTROYED" );
}

void InviteServerTransaction::OnSIPEvent(
  SIPMessage & message,
  BOOL isSendEvent
)
{
  if( m_CallId.IsEmpty() )
    m_CallId = message.GetCallId();

  LOG_CONTEXT( LogDebug(), m_CallId, "TRANSACTION: " << "(IST) " << message.GetStartLine() << " State: " << m_State );

  if( m_EnableXORHash && !message.IsRequest() )
    message.SetXORHash( TRUE );

  switch( m_State )
  {
    case Idle:
      LOG_IF_DEBUG( LogDebug(), "*** IST STATE IDLE ***" << endl << message.GetStartLine() );
      HandleStateIdle( message, isSendEvent );
      break;
    case Proceeding:
      LOG_IF_DEBUG( LogDebug(), "*** IST STATE PROCEEDING ***" << endl << message.GetStartLine() );
      HandleStateProceeding( message, isSendEvent );
      break;
    case Completed:
      LOG_IF_DEBUG( LogDebug(), "*** IST STATE COMPLETED ***" << endl << message.GetStartLine() );
      HandleStateCompleted( message, isSendEvent );
    case Confirmed:
      LOG_IF_DEBUG( LogDebug(), "*** IST STATE CONFIRMED ***" << endl << message.GetStartLine() );
      HandleStateConfirmed( message, isSendEvent );
    break;
  }
}



void InviteServerTransaction::HandleStateIdle(
  SIPMessage & message,
  BOOL isSendEvent
)
{
    /*
  When 
  a server transaction is constructed for a request, it enters the
  "Proceeding" state.  The server transaction MUST generate a 100
  (Trying) response unless it knows that the TU will generate a
  provisional or final response within 200 ms, in which case it MAY
  generate a 100 (Trying) response.  This provisional response is
  needed to quench request retransmissions rapidly in order to avoid
  network congestion.  The 100 (Trying) response is constructed
  according to the procedures in Section 8.2.6, except that the
  insertion of tags in the To header field of the response (when none
  was present in the request) is downgraded from MAY to SHOULD NOT.
  The request MUST be passed to the TU.
  */

  if( !isSendEvent )
  {
    /// double check via before sending a reply
    Via via;
    message.GetViaAt( 0, via );
    if( via.GetAddress().IsEmpty() || via.GetAddress() == "0.0.0.0" )
    {
      SetState( Terminated );
      Terminate();
      return;
    }else 
    {
      /// check if the address is valid
      PIPSocket::Address viaAddr( via.GetAddress() );
      if( !viaAddr.IsValid() )
      {
        /// still not valid, check if it might be a domain
        /// very light check.  Just get a hint if it start with 
        /// a number and assume an invalid ip address
        if( ::isdigit( (char)via.GetAddress()[0] ) )
        {
          SetState( Terminated );
          Terminate();
          return;
        }
      }
    }

    SetOpeningRequest( message );
    
    // check if XOR hash is enabled
    m_EnableXORHash = message.IsXORHashSet();
    
    m_Manager.OnReceivedMessage( message, *this );
    SetState( Proceeding );
    SendProceeding( message );
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateIdle->StateProceeding" );
  }
}


void InviteServerTransaction::HandleStateProceeding(
  SIPMessage & message,
  BOOL isSendEvent
)
{
  if( isSendEvent )
    return;

   /*The TU passes any number of provisional responses to the server
   transaction.  So long as the server transaction is in the
   "Proceeding" state, each of these MUST be passed to the transport
   layer for transmission.  They are not sent reliably by the
   transaction layer (they are not retransmitted by it) and do not cause
   a change in the state of the server transaction.  If a request
   retransmission is received while in the "Proceeding" state, the most
   recent provisional response that was received from the TU MUST be
   passed to the transport layer for retransmission.  A request is a
   retransmission if it matches the same server transaction based on the
   rules of Section 17.2.3.

      If, while in the "Proceeding" state, the TU passes a 2xx response to
   the server transaction, the server transaction MUST pass this
   response to the transport layer for transmission.  It is not
   retransmitted by the server transaction; retransmissions of 2xx
   responses are handled by the TU.  The server transaction MUST then
   transition to the "Terminated" state.

   While in the "Proceeding" state, if the TU passes a response with
   status code from 300 to 699 to the server transaction, the response
   MUST be passed to the transport layer for transmission, and the state
   machine MUST enter the "Completed" state.  For unreliable transports,
   timer G is set to fire in T1 seconds, and is not set to fire for
   reliable transports.

      This is a change from RFC 2543, where responses were always
      retransmitted, even over reliable transports.

   When the "Completed" state is entered, timer H MUST be set to fire in
   64*T1 seconds for all transports.  Timer H determines when the server
   transaction abandons retransmitting the response.  Its value is
   chosen to equal Timer B, the amount of time a client transaction will
   continue to retry sending a request.  If timer G fires, the response
   is passed to the transport layer once more for retransmission, and
   timer G is set to fire in MIN(2*T1, T2) seconds.  From then on, when
   timer G fires, the response is passed to the transport again for
   transmission, and timer G is reset with a value that doubles, unless
   that value exceeds T2, in which case it is reset with the value of
   T2.  This is identical to the retransmit behavior for requests in the
   "Trying" state of the non-INVITE client transaction.  Furthermore,
   while in the "Completed" state, if a request retransmission is
   received, the server SHOULD pass the response to the transport for
   retransmission.

   If an ACK is received while the server transaction is in the
   "Completed" state, the server transaction MUST transition to the
   "Confirmed" state.  As Timer G is ignored in this state, any
   retransmissions of the response will cease.

   If timer H fires while in the "Completed" state, it implies that the
   ACK was never received.  In this case, the server transaction MUST
   transition to the "Terminated" state, and MUST indicate to the TU
   that a transaction failure has occurred.

    The purpose of the "Confirmed" state is to absorb any additional ACK
   messages that arrive, triggered from retransmissions of the final
   response.  When this state is entered, timer I is set to fire in T4
   seconds for unreliable transports, and zero seconds for reliable
   transports.  Once timer I fires, the server MUST transition to the
   "Terminated" state.

   Once the transaction is in the "Terminated" state, it MUST be
   destroyed immediately.  As with client transactions, this is needed
   to ensure reliability of the 2xx responses to INVITE.
   */

  
  if( message.IsRequest() && !message.IsAck() )
  {
    /// retrnasmit the last response sent by the TU
    //m_TransportSender( m_LastResponse, (INT)this );
    m_Manager.OnSendMessageToTransport( m_LastResponse, *this );
  }
  else
  {
    m_LastResponse = message;
    
    if( m_EnableXORHash )
      m_LastResponse.SetXORHash( TRUE );

    m_UACoreName = m_LastResponse.GetUACoreName();

    StartLine startLine;
    message.GetStartLine( startLine );
    WORD statusCode = startLine.GetStatusLine()->GetStatusCode();

    //// 1xx
    if( statusCode >= 100 && statusCode < 200 ) 
    {
      m_Manager.OnSendMessageToTransport( m_LastResponse, *this );
    //// 2xx Response
    }else if( statusCode >= 200 && statusCode < 300 )
    {
#if 0 
      /*      
      From RFC 3261

      "Note that when a UAS core sends a 2xx response to INVITE, the server transaction is
      destroyed."

      This is accepted as a bug in RFC 3261.

      http://bugs.sipit.net/show_bug.cgi?id=769

      The fix will involve having the server transaction continue to exist long
      enough to drain any retransmissions of the INVITE 
      */

      LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateTerminated(" << startLine << ")" );
      SetState( Terminated );
      Terminate();
#else
      /// act as if we have received an ACK for none-2xx response
      m_Manager.OnSendMessageToTransport( m_LastResponse, *this );
      StopTimer_G();
      StopTimer_H();
      LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateConfirmed" );
      SetState( Confirmed );
      StartTimer_I();
      return;
#endif

      //// 3xx, 4xx, 5xx Response
    }else if( statusCode >= 300 )
    {
      LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateCompleted(" << startLine << ")" );
      SetState( Completed );

      StartTimer_H();

      /// check if via needs a reliable transport
      Via via;
      if( !message.GetViaAt( 0, via ) )
        return;
     
      PString proto = via.GetProtocol();

      
      StartTimer_G();


      /// tell the manager that we are expecting an ACK
      m_Manager.AddAckTransaction( m_LastResponse, this );
      m_Manager.OnSendMessageToTransport( m_LastResponse, *this );
    }
  }
}

void InviteServerTransaction::HandleStateCompleted(
  SIPMessage & message,
  BOOL isSendEvent
)
{
  if( isSendEvent )
    return;

  if( message.IsRequest() )
  {
    /// check if its an ACK
    if( message.IsAck() )
    {
      StopTimer_G();
      StopTimer_H();
      LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateCompleted->StateConfirmed" );
      SetState( Confirmed );
      StartTimer_I();
    }else
    {
      /// Not an ACK
      /// retrnasmit the last response sent by the TU
      ///m_TransportSender( m_LastResponse, (INT)this );
      m_Manager.OnSendMessageToTransport( m_LastResponse, *this );
    }
  }
}

void InviteServerTransaction::HandleStateConfirmed(
  SIPMessage & /*message*/,
  BOOL /*isSendEvent*/

)
{
  /// we are not expecting any event here
}

void InviteServerTransaction::OnTimerEvent(
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
  case SIPTimerEvent::G:
    if( m_State == Completed )
    {
      /// retransmit
      ///m_TransportSender( m_LastResponse, (INT)this );
      m_Manager.OnSendMessageToTransport( m_LastResponse, *this );
      
      /// retstart timer with compounded interval 
      if( timerEvent.GetInterval() < SIP_TIMER_T2 ) 
        StartTimer_G( timerEvent.GetInterval() * 2 );
      else
        StartTimer_G( SIP_TIMER_T2 );
    }
    break;
  case SIPTimerEvent::H:
    if( m_State == Completed )
    {
      StopTimer_G();
      LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateCompleted->StateTerminated" );
      SetState( Terminated );
      //m_TUTimerHandler( timerEvent, (INT)this );
      m_Manager.RemoveAckTransaction( m_LastResponse, this );
      m_Manager.OnTimerExpire( timerEvent, *this );
      Terminate();
    }
    break;
  case SIPTimerEvent::I:
    if( m_State == Confirmed )
    {
      Terminate();
      SetState( Terminated );
    }
    break;

  default:
    break;
  }
}

void InviteServerTransaction::OnUnknownEvent( 
  SIPEvent * /*event*/ 
)
{
}

void InviteServerTransaction::SendProceeding( 
  const SIPMessage & request 
)
{
  SIPMessage proceeding;
  request.CreateResponse( proceeding, SIPMessage::Code100_Trying );

  if( m_EnableXORHash )
  {
    proceeding.SetXORHash( TRUE );
  }

  m_LastResponse = proceeding;
  m_Manager.OnSendMessageToTransport( proceeding, *this );
}

void InviteServerTransaction::OnCancelEvent()
{
  if( m_State != Terminated )
  {

    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tEventCancel->StateTerminated" );
    SetState( Completed );

    StartTimer_H();

    SIPMessage response;
    m_OpeningRequest.CreateResponse( response, SIPMessage::Code487_RequestCancelled );

    m_LastResponse = response;
    /// tell the manager that we are expecting an ACK
    m_Manager.AddAckTransaction( response, this );
    response.Finalize();
    ///m_TransportSender( response, (INT)this );
    m_Manager.OnSendMessageToTransport( response, *this );
    //m_Manager.OnReceivedMessage( message, *this );
  }

}
