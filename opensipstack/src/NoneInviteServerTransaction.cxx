/*
 *
 * NoneInviteServerTransaction.cxx
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
 * $Log: NoneInviteServerTransaction.cxx,v $
 * Revision 1.16  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.15  2006/11/17 04:22:22  joegenbaclor
 * Added ability to remember core to use via m_UACoreName
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
 * Revision 1.10  2006/03/27 01:38:11  joegenbaclor
 * Removed some test related codes
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

#include "NoneInviteServerTransaction.h"
#include "SIPTransactionManager.h"

#define new PNEW

using namespace SIPFSM;

#ifdef _WIN32
#pragma warning(disable:4311)
#endif

NoneInviteServerTransaction::NoneInviteServerTransaction(
  const TransactionId & transactionId,
  SIPTransactionManager & manager
) : SIPTransaction( transactionId, NIST, manager ),
    m_HasSentProvisionalResponse( FALSE )
{
  m_EnableXORHash = FALSE;
}

NoneInviteServerTransaction::~NoneInviteServerTransaction()
{
  LOG_CONTEXT( LogDebug(), m_CallId, "TRANSACTION: " << "(NIST) DESTROYED" );
}

void NoneInviteServerTransaction::OnTimerEvent(
  SIPTimerEvent & timerEvent  
)
{
  if( timerEvent.GetType() == SIPTimerEvent::J  )
  {
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateCompleted->StateTerminated" );
    SetState( Terminated );
    Terminate();
  }else if( timerEvent.GetType() == SIPTimerEvent::LifeSpanExpire )
  {
      SetState( Terminated );
      m_Manager.OnTimerExpire( timerEvent, *this );
      Terminate();
  }
}

void NoneInviteServerTransaction::OnSIPEvent(
  SIPMessage & message,
  BOOL isSendEvent
)
{
  if( m_CallId.IsEmpty() )
    m_CallId = message.GetCallId();

  LOG_CONTEXT( LogDebug(), m_CallId, "TRANSACTION: " << "(NIST) " << message.GetStartLine() << " State: " << m_State );
  
  if( m_EnableXORHash && !message.IsRequest() )
    message.SetXORHash( TRUE );

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
    case Terminated:
      break;
  }
}

void NoneInviteServerTransaction::OnUnknownEvent( 
  SIPEvent * /*event*/ 
)
{
}

void NoneInviteServerTransaction::HandleStateIdle(
  SIPMessage & message,
  BOOL isSendEvent  
)
{

   /*  The state machine is initialized in the "Trying" state and is passed
   a request other than INVITE or ACK when initialized.  This request is
   passed up to the TU.  Once in the "Trying" state, any further request
   retransmissions are discarded.  A request is a retransmission if it
   matches the same server transaction, using the rules specified in
   Section 17.2.3.*/


  if( isSendEvent || !message.IsRequest() )
    return;
  
  // check if XOR hash is enabled
  m_EnableXORHash = message.IsXORHashSet();
  SetOpeningRequest( message );
  LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateIdle->StateTrying" );
  SetState( Trying );
  m_Manager.OnReceivedMessage( message, *this );
  
}

void NoneInviteServerTransaction::HandleStateTrying(
  SIPMessage & message,
  BOOL isSendEvent  
)
{
  /*
   While in the "Trying" state, if the TU passes a provisional response
   to the server transaction, the server transaction MUST enter the
   "Proceeding" state.  The response MUST be passed to the transport
   layer for transmission.  Any further provisional responses that are
   received from the TU while in the "Proceeding" state MUST be passed
   to the transport layer for transmission.  If a retransmission of the
   request is received while in the "Proceeding" state, the most
   recently sent provisional response MUST be passed to the transport
   layer for retransmission.  If the TU passes a final response (status
   codes 200-699) to the server while in the "Proceeding" state, the
   transaction MUST enter the "Completed" state, and the response MUST
   be passed to the transport layer for transmission.
   */

  if( isSendEvent )
    return;
 
  if( !message.IsRequest() )
  {
    m_LastResponse = message;

    if( m_EnableXORHash )
      m_LastResponse.SetXORHash( TRUE );

    m_UACoreName = m_LastResponse.GetUACoreName();

    if( message.Is1xx() )
    {
      LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateTrying->StateProceeding" );
      SetState( Proceeding );
      
      m_HasSentProvisionalResponse = TRUE;
      m_Manager.OnSendMessageToTransport( m_LastResponse, *this );
    }else
    {
      m_LastResponse = message;
      LOG_IF_DEBUG( LogDetail(), m_InternalId << "HandleStateTrying->StateCompleted" );
      SetState( Completed );
      StartTimer_J();
      m_Manager.OnSendMessageToTransport( m_LastResponse, *this );
    }
  }
}

void NoneInviteServerTransaction::HandleStateProceeding(
  SIPMessage & message,
  BOOL isSendEvent  

)
{
  if( isSendEvent )
    return;

  if( message.IsRequest() && m_HasSentProvisionalResponse )
  {
    /// retrnasmit the last provisional response we sent
    m_Manager.OnSendMessageToTransport( m_LastResponse, *this );
  }else if( !message.IsRequest() )
  {
    
      m_LastResponse = message;

    if( m_EnableXORHash )
      m_LastResponse.SetXORHash( TRUE );

    if( message.Is1xx() )
    {
      
      m_HasSentProvisionalResponse = TRUE;
      m_Manager.OnSendMessageToTransport( m_LastResponse, *this );
    }else
    {
      ///this is a 2xx-6xx response
      LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateCompleted" );
      SetState( Completed );
      StartTimer_J();
      m_Manager.OnSendMessageToTransport( m_LastResponse, *this );
    }
  }
}

void NoneInviteServerTransaction::HandleStateCompleted(
  SIPMessage & message,
  BOOL isSendEvent  
)
{
  /*
  17.2.2 Non-INVITE Server Transaction

   The state machine for the non-INVITE server transaction is shown in
   Figure 8.

   The state machine is initialized in the "Trying" state and is passed
   a request other than INVITE or ACK when initialized.  This request is
   passed up to the TU.  Once in the "Trying" state, any further request
   retransmissions are discarded.  A request is a retransmission if it
   matches the same server transaction, using the rules specified in
   Section 17.2.3.

   While in the "Trying" state, if the TU passes a provisional response
   to the server transaction, the server transaction MUST enter the
   "Proceeding" state.  The response MUST be passed to the transport
   layer for transmission.  Any further provisional responses that are
   received from the TU while in the "Proceeding" state MUST be passed
   to the transport layer for transmission.  If a retransmission of the
   request is received while in the "Proceeding" state, the most
   recently sent provisional response MUST be passed to the transport
   layer for retransmission.  If the TU passes a final response (status
   codes 200-699) to the server while in the "Proceeding" state, the
   transaction MUST enter the "Completed" state, and the response MUST
   be passed to the transport layer for transmission.

   When the server transaction enters the "Completed" state, it MUST set
   Timer J to fire in 64*T1 seconds for unreliable transports, and zero
   seconds for reliable transports.  While in the "Completed" state, the
   server transaction MUST pass the final response to the transport
   layer for retransmission whenever a retransmission of the request is
   received.  Any other final responses passed by the TU to the server
   transaction MUST be discarded while in the "Completed" state.  The
   server transaction remains in this state until Timer J fires, at
   which point it MUST transition to the "Terminated" state.

   The server transaction MUST be destroyed the instant it enters the
   "Terminated" state.

*/

  if( !isSendEvent && message.IsRequest() )
  {
    /// retrnasmit the last final response we sent
    m_Manager.OnSendMessageToTransport( m_LastResponse, *this );
  }
}

