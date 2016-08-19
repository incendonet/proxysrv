/*
 *
 * InviteClientTransaction.cxx
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
 * $Log: InviteClientTransaction.cxx,v $
 * Revision 1.14  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.13  2006/11/17 04:22:21  joegenbaclor
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
 * Revision 1.9  2006/03/24 15:42:18  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.8  2006/03/22 08:45:52  joegenbaclor
 * More work on simple-proxy and affected classes
 *
 * Revision 1.7  2006/03/20 03:51:14  joegenbaclor
 * This upload marks the first successful OPAL call using opensipstack!!!   There is still some pending issues
 * in 183 Progress handling in OPAL classes but a fix should with the next revision.
 *
 * Revision 1.6  2006/03/14 06:39:11  joegenbaclor
 * Introduced State Connected to ICT to catch 200 ok retransmissions
 *
 * Revision 1.5  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "InviteClientTransaction.h"
#include "SIPTransactionManager.h"

#define new PNEW

using namespace SIPFSM;

#ifdef _WIN32
#pragma warning(disable:4311)
#endif

InviteClientTransaction::InviteClientTransaction(
  const PString & id,
  SIPTransactionManager & manager
) : SIPTransaction( id, ICT, manager )
{
  m_State = Idle;
  m_HasSentInitialRequest = FALSE;
  m_EnableXORHash = FALSE;
}

InviteClientTransaction::~InviteClientTransaction()
{
  LOG_CONTEXT( LogDebug(), m_CallId, "TRANSACTION: " << "(ICT) DESTROYED" );
}


void InviteClientTransaction::OnSIPEvent(
  SIPMessage & message,
  BOOL isSendEvent  
)
{
  if( m_CallId.IsEmpty() )
    m_CallId = message.GetCallId();

  LOG_CONTEXT( LogDebug(), m_CallId, "TRANSACTION: " << "(ICT) " << message.GetStartLine() << " State: " << m_State );
  
  switch( m_State )
  {
    case Idle:
      HandleStateIdle( message, isSendEvent );
      break;
    case Calling:
      HandleStateCalling( message, isSendEvent );
      break;
    case Proceeding:
      HandleStateProceeding( message, isSendEvent );
      break;
    case Completed:
      HandleStateCompleted( message, isSendEvent );
      break;
    case Connected:
      HandleStateConnected( message, isSendEvent );
      break;
  }
}

void InviteClientTransaction::OnUnknownEvent( 
  SIPEvent * /*event*/ 
)
{
  switch( m_State )
  {
    case Idle:
      break;
    case Calling:
      break;
    case Proceeding:
      break;
    case Terminated:
      break;
    case Completed:
      break;
    default:
      PAssertAlways( PLogicError );
  }
}

void InviteClientTransaction::HandleStateIdle(
  SIPMessage & message,
  BOOL isSendEvent  
)
{
  if( !isSendEvent && !m_HasSentInitialRequest )
  {
    message.Finalize();
    SetOpeningRequest( message );

    StartLine startLine;
    message.GetStartLine( startLine );
    PString transport;

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

    PAssert( startLine.IsRequest(), "InviteClientTransaction got none request event on State Idle" );

    if( startLine.GetRequestLine()->GetMethod() *= "INVITE" )
    {
      PAssert( startLine.IsRequest(), "InviteClientTransaction got None-Invite event on State Idle" );
    }
    /// send it to the transport
    ///m_TransportSender( message, (INT)this );
    // check if XOR hash is enabled
    m_EnableXORHash = message.IsXORHashSet();
    m_HasSentInitialRequest = TRUE;
    m_UACoreName = message.GetUACoreName();
    m_Manager.OnSendMessageToTransport( message, *this );

  }else if( isSendEvent )
  {
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateIdle->StateCalling(" << message.GetStartLine() << ")" );
    SetState( Calling );
    StartTimer_A();
    StartTimer_B();
  }else if( !isSendEvent && message.Is1xx() )
  {
    SetState( Calling );
    HandleStateCalling( message, isSendEvent );
  }

}

void InviteClientTransaction::HandleStateCalling(
  SIPMessage & message,
  BOOL isSendEvent  
)
{
  /// this is a retransmission, ignore it
  if( message.IsRequest() || isSendEvent)
    return;

  StartLine startLine;
  message.GetStartLine( startLine );

  if( startLine.GetStatusLine() == NULL )
    return;

  WORD statusCode = startLine.GetStatusLine()->GetStatusCode();

  StopTimer_A();
  StopTimer_B();

  if( statusCode >= 100 && statusCode < 200 )
  {
    /*If the client transaction receives a provisional response while in
      the "Calling" state, it transitions to the "Proceeding" state. In the
      "Proceeding" state, the client transaction SHOULD NOT retransmit the
      request any longer. Furthermore, the provisional response MUST be
      passed to the TU.  Any further provisional responses MUST be passed
      up to the TU while in the "Proceeding" state.*/

    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateCalling->StateProceeding(" << startLine << ")" );
  
    SetState( Proceeding );
    ///m_TUMessageHandler( message, (INT)this );
    m_Manager.OnReceivedMessage( message, *this );
 
  }
  else if( statusCode >= 200 && statusCode < 300 )
  {
    /* When in either the "Calling" or "Proceeding" states, reception of a
     2xx response MUST cause the client transaction to enter the
     "Terminated" state, and the response MUST be passed up to the TU.
     The handling of this response depends on whether the TU is a proxy
     core or a UAC core.  A UAC core will handle generation of the ACK for
     this response, while a proxy core will always forward the 200 (OK)
     upstream.  The differing treatment of 200 (OK) between proxy and UAC
     is the reason that handling of it does not take place in the
     transaction layer.*/

    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateConnected(" << startLine << ")" );

    SetState( Connected );
    ///m_TUMessageHandler( message, (INT)this );
    m_Manager.OnReceivedMessage( message, *this );
   
    StartTimer_D();

  }
  else if( statusCode >= 300 && statusCode < 400 )
  {
    /*When in either the "Calling" or "Proceeding" states, reception of a
     response with status code from 300-699 MUST cause the client
     transaction to transition to "Completed".  The client transaction
     MUST pass the received response up to the TU, and the client
     transaction MUST generate an ACK request, even if the transport is
     reliable (guidelines for constructing the ACK from the response are
     given in Section 17.1.1.3) and then pass the ACK to the transport
     layer for transmission.  The ACK MUST be sent to the same address,
     port, and transport to which the original request was sent.  The
     client transaction SHOULD start timer D when it enters the
     "Completed" state, with a value of at least 32 seconds for unreliable
     transports, and a value of zero seconds for reliable transports.
     Timer D reflects the amount of time that the server transaction can
     remain in the "Completed" state when unreliable transports are used.
     This is equal to Timer H in the INVITE server transaction, whose*/
   
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateCalling->StateCompleted(" << startLine << ")" );

    SetState( Completed );
    ///m_TUMessageHandler( message, (INT)this );
    m_Manager.OnReceivedMessage( message, *this );
    SendACK( message );
    StartTimer_D();
    
  }
  else if( statusCode >= 400 && statusCode < 500 )
  {
    /*When in either the "Calling" or "Proceeding" states, reception of a
     response with status code from 300-699 MUST cause the client
     transaction to transition to "Completed".  The client transaction
     MUST pass the received response up to the TU, and the client
     transaction MUST generate an ACK request, even if the transport is
     reliable (guidelines for constructing the ACK from the response are
     given in Section 17.1.1.3) and then pass the ACK to the transport
     layer for transmission.  The ACK MUST be sent to the same address,
     port, and transport to which the original request was sent.  The
     client transaction SHOULD start timer D when it enters the
     "Completed" state, with a value of at least 32 seconds for unreliable
     transports, and a value of zero seconds for reliable transports.
     Timer D reflects the amount of time that the server transaction can
     remain in the "Completed" state when unreliable transports are used.
     This is equal to Timer H in the INVITE server transaction, whose*/
   
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateCalling->StateCompleted(" << startLine << ")" );

    SetState( Completed );

    ///m_TUMessageHandler( message, (INT)this );
    m_Manager.OnReceivedMessage( message, *this );
    SendACK( message );

    StartTimer_D();
  }
  else if( statusCode >= 500 && statusCode < 600 )
  {
    /*When in either the "Calling" or "Proceeding" states, reception of a
     response with status code from 300-699 MUST cause the client
     transaction to transition to "Completed".  The client transaction
     MUST pass the received response up to the TU, and the client
     transaction MUST generate an ACK request, even if the transport is
     reliable (guidelines for constructing the ACK from the response are
     given in Section 17.1.1.3) and then pass the ACK to the transport
     layer for transmission.  The ACK MUST be sent to the same address,
     port, and transport to which the original request was sent.  The
     client transaction SHOULD start timer D when it enters the
     "Completed" state, with a value of at least 32 seconds for unreliable
     transports, and a value of zero seconds for reliable transports.
     Timer D reflects the amount of time that the server transaction can
     remain in the "Completed" state when unreliable transports are used.
     This is equal to Timer H in the INVITE server transaction, whose*/
   
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateCalling->StateCompleted(" << startLine << ")" );

    SetState( Completed );

    ///m_TUMessageHandler( message, (INT)this );
    m_Manager.OnReceivedMessage( message, *this );
    SendACK( message );
    

    StartTimer_D();
  }
  else if( statusCode >= 600 && statusCode < 700 )
  {
    /*When in either the "Calling" or "Proceeding" states, reception of a
     response with status code from 300-699 MUST cause the client
     transaction to transition to "Completed".  The client transaction
     MUST pass the received response up to the TU, and the client
     transaction MUST generate an ACK request, even if the transport is
     reliable (guidelines for constructing the ACK from the response are
     given in Section 17.1.1.3) and then pass the ACK to the transport
     layer for transmission.  The ACK MUST be sent to the same address,
     port, and transport to which the original request was sent.  The
     client transaction SHOULD start timer D when it enters the
     "Completed" state, with a value of at least 32 seconds for unreliable
     transports, and a value of zero seconds for reliable transports.
     Timer D reflects the amount of time that the server transaction can
     remain in the "Completed" state when unreliable transports are used.
     This is equal to Timer H in the INVITE server transaction, whose*/
   
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateCalling->StateCompleted(" << startLine << ")" );

    SetState( Completed );
    ///m_TUMessageHandler( message, (INT)this );
    m_Manager.OnReceivedMessage( message, *this );
    SendACK( message );

    StartTimer_D();
  }
}

void InviteClientTransaction::HandleStateProceeding(
  SIPMessage & message,
  BOOL isSendEvent  
)
{
    /// this is a retransmission, ignore it
  if( message.IsRequest() || isSendEvent )
    return;

  StartLine startLine;
  message.GetStartLine( startLine );

  if( startLine.GetStatusLine() == NULL )
    return;

  WORD statusCode = startLine.GetStatusLine()->GetStatusCode();

  StopTimer_A();
  StopTimer_B();

  if( statusCode >= 100 && statusCode < 200 )
  {
    /*  Any further provisional responses MUST be passed
        up to the TU while in the "Proceeding" state. */

    ///LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateProceeding(" << startLine << ")" );
    
    ///m_TUMessageHandler( message, (INT)this );
    m_Manager.OnReceivedMessage( message, *this );
  }
  else if( statusCode >= 200 && statusCode < 300 )
  {
    /* When in either the "Calling" or "Proceeding" states, reception of a
     2xx response MUST cause the client transaction to enter the
     "Terminated" state, and the response MUST be passed up to the TU.
     The handling of this response depends on whether the TU is a proxy
     core or a UAC core.  A UAC core will handle generation of the ACK for
     this response, while a proxy core will always forward the 200 (OK)
     upstream.  The differing treatment of 200 (OK) between proxy and UAC
     is the reason that handling of it does not take place in the
     transaction layer.*/

    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateConnected(" << startLine << ")" );

    SetState( Connected );
    ///m_TUMessageHandler( message, (INT)this );
    m_Manager.OnReceivedMessage( message, *this );
   
    StartTimer_D();
  }
  else if( statusCode >= 300 && statusCode < 400 )
  {
    /*When in either the "Calling" or "Proceeding" states, reception of a
     response with status code from 300-699 MUST cause the client
     transaction to transition to "Completed".  The client transaction
     MUST pass the received response up to the TU, and the client
     transaction MUST generate an ACK request, even if the transport is
     reliable (guidelines for constructing the ACK from the response are
     given in Section 17.1.1.3) and then pass the ACK to the transport
     layer for transmission.  The ACK MUST be sent to the same address,
     port, and transport to which the original request was sent.  The
     client transaction SHOULD start timer D when it enters the
     "Completed" state, with a value of at least 32 seconds for unreliable
     transports, and a value of zero seconds for reliable transports.
     Timer D reflects the amount of time that the server transaction can
     remain in the "Completed" state when unreliable transports are used.
     This is equal to Timer H in the INVITE server transaction, whose*/

    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateCompleted(" << startLine << ")" );
   
    SetState( Completed );
    ///m_TUMessageHandler( message, (INT)this );
    m_Manager.OnReceivedMessage( message, *this );
    SendACK( message );

    StartTimer_D();
    
  }
  else if( statusCode >= 400 && statusCode < 500 )
  {
    /*When in either the "Calling" or "Proceeding" states, reception of a
     response with status code from 300-699 MUST cause the client
     transaction to transition to "Completed".  The client transaction
     MUST pass the received response up to the TU, and the client
     transaction MUST generate an ACK request, even if the transport is
     reliable (guidelines for constructing the ACK from the response are
     given in Section 17.1.1.3) and then pass the ACK to the transport
     layer for transmission.  The ACK MUST be sent to the same address,
     port, and transport to which the original request was sent.  The
     client transaction SHOULD start timer D when it enters the
     "Completed" state, with a value of at least 32 seconds for unreliable
     transports, and a value of zero seconds for reliable transports.
     Timer D reflects the amount of time that the server transaction can
     remain in the "Completed" state when unreliable transports are used.
     This is equal to Timer H in the INVITE server transaction, whose*/
   
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateCompleted(" << startLine << ")" );

    SetState( Completed );
    ///m_TUMessageHandler( message, (INT)this );
    m_Manager.OnReceivedMessage( message, *this );
    SendACK( message );
    

    StartTimer_D();
  }
  else if( statusCode >= 500 && statusCode < 600 )
  {
    /*When in either the "Calling" or "Proceeding" states, reception of a
     response with status code from 300-699 MUST cause the client
     transaction to transition to "Completed".  The client transaction
     MUST pass the received response up to the TU, and the client
     transaction MUST generate an ACK request, even if the transport is
     reliable (guidelines for constructing the ACK from the response are
     given in Section 17.1.1.3) and then pass the ACK to the transport
     layer for transmission.  The ACK MUST be sent to the same address,
     port, and transport to which the original request was sent.  The
     client transaction SHOULD start timer D when it enters the
     "Completed" state, with a value of at least 32 seconds for unreliable
     transports, and a value of zero seconds for reliable transports.
     Timer D reflects the amount of time that the server transaction can
     remain in the "Completed" state when unreliable transports are used.
     This is equal to Timer H in the INVITE server transaction, whose*/

    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateCompleted(" << startLine << ")" );
   
    SetState( Completed );
    ///m_TUMessageHandler( message, (INT)this );
    m_Manager.OnReceivedMessage( message, *this );
    SendACK( message );
    StartTimer_D();
  }
  else if( statusCode >= 600 && statusCode < 700 )
  {
    /*When in either the "Calling" or "Proceeding" states, reception of a
     response with status code from 300-699 MUST cause the client
     transaction to transition to "Completed".  The client transaction
     MUST pass the received response up to the TU, and the client
     transaction MUST generate an ACK request, even if the transport is
     reliable (guidelines for constructing the ACK from the response are
     given in Section 17.1.1.3) and then pass the ACK to the transport
     layer for transmission.  The ACK MUST be sent to the same address,
     port, and transport to which the original request was sent.  The
     client transaction SHOULD start timer D when it enters the
     "Completed" state, with a value of at least 32 seconds for unreliable
     transports, and a value of zero seconds for reliable transports.
     Timer D reflects the amount of time that the server transaction can
     remain in the "Completed" state when unreliable transports are used.
     This is equal to Timer H in the INVITE server transaction, whose*/
   
    LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tStateProceeding->StateCompleted(" << startLine << ")" );

    SetState( Completed );
    ///m_TUMessageHandler( message, (INT)this );
    m_Manager.OnReceivedMessage( message, *this );
    SendACK( message );

    StartTimer_D();
  }
}

void InviteClientTransaction::HandleStateConnected(
  SIPMessage & message,
  BOOL isSendEvent
)
{
  int statusCode = message.GetStatusCode();

  if( statusCode >= 200 && statusCode < 300 )
  {
    /* When in either the "Calling" or "Proceeding" states, reception of a
     2xx response MUST cause the client transaction to enter the
     "Terminated" state, and the response MUST be passed up to the TU.
     The handling of this response depends on whether the TU is a proxy
     core or a UAC core.  A UAC core will handle generation of the ACK for
     this response, while a proxy core will always forward the 200 (OK)
     upstream.  The differing treatment of 200 (OK) between proxy and UAC
     is the reason that handling of it does not take place in the
     transaction layer.*/

    m_Manager.OnReceivedMessage( message, *this );
  }
}

void InviteClientTransaction::HandleStateCompleted(
  SIPMessage & message,
  BOOL /*isSendEvent*/  
)
{
  /*Any retransmissions of the final response that are received while in
   the "Completed" state MUST cause the ACK to be re-passed to the
   transport layer for retransmission, but the newly received response
   MUST NOT be passed up to the TU.  A retransmission of the response is
   defined as any response which would match the same client transaction
   based on the rules of Section 17.1.3.*/

  SendACK( message );
}





void InviteClientTransaction::OnTimerEvent(
  SIPTimerEvent & timerEvent  
)
{

  SIPTimerEvent::Type timerType = timerEvent.GetType();

  switch( timerType )
  {
    case SIPTimerEvent::LifeSpanExpire:
      SetState( Terminated );
      m_Manager.OnTimerExpire( timerEvent, *this );
      Terminate();
      break;
    case SIPTimerEvent::A:
      if( m_State == Calling )
      {
        /*  
        When timer A fires, the client transaction MUST retransmit the
        request by passing it to the transport layer, and MUST reset the
        timer with a value of 2*T1.  The formal definition of retransmit
        within the context of the transaction layer is to take the message
        previously sent to the transport layer and pass it to the transport
        layer once more. */

        /// retransmit
        ///m_TransportSender( m_OpeningRequest, (INT)this );
        m_Manager.OnSendMessageToTransport( m_OpeningRequest, *this );
        
        /// retstart timer with compounded interval 
        StartTimer_A( timerEvent.GetInterval() * 2 );
      }
      break;
    case SIPTimerEvent::B:

      if(  m_State == Calling )
      {
        /*If the client transaction is still in the "Calling" state when timer
        B fires, the client transaction SHOULD inform the TU that a timeout
        has occurred.  The client transaction MUST NOT generate an ACK.  The
        value of 64*T1 is equal to the amount of time required to send seven
        requests in the case of an unreliable transport.*/

        StopTimer_A();

        SetState( Terminated );
        ///m_TUTimerHandler( timerEvent, (INT)this );
        m_Manager.OnTimerExpire( timerEvent, *this );
        Terminate();
      }
      break;
    case SIPTimerEvent::D:
    /*
      The client transaction SHOULD start timer D when it enters the
      "Completed" state, with a value of at least 32 seconds for unreliable
      transports, and a value of zero seconds for reliable transports.
      Timer D reflects the amount of time that the server transaction can
      remain in the "Completed" state when unreliable transports are used.
      This is equal to Timer H in the INVITE server transaction, whose
      default is 64*T1.
      */

      if( m_State == Completed || m_State == Connected )
      {
        SetState( Terminated );
        Terminate();
      }else
      {
        PAssertAlways( PLogicError );
      }

      break;

    default:
      break;
  };

}

void InviteClientTransaction::SendACK(
  const SIPMessage & response
)
{
   /* RFC Says:

   17.1.1.3 Construction of the ACK Request

   This section specifies the construction of ACK requests sent within
   the client transaction.  A UAC core that generates an ACK for 2xx
   MUST instead follow the rules described in Section 13.

   The ACK request constructed by the client transaction MUST contain
   values for the Call-ID, From, and Request-URI that are equal to the
   values of those header fields in the request passed to the transport
   by the client transaction (call this the "original request").  The To
   header field in the ACK MUST equal the To header field in the
   response being acknowledged, and therefore will usually differ from
   the To header field in the original request by the addition of the
   tag parameter.  The ACK MUST contain a single Via header field, and
   this MUST be equal to the top Via header field of the original
   request.  The CSeq header field in the ACK MUST contain the same
   value for the sequence number as was present in the original request,
   but the method parameter MUST be equal to "ACK".
   If the INVITE request whose response is being acknowledged had Route
   header fields, those header fields MUST appear in the ACK.  This is
   to ensure that the ACK can be routed properly through any downstream
   stateless proxies.

   Although any request MAY contain a body, a body in an ACK is special
   since the request cannot be rejected if the body is not understood.
   Therefore, placement of bodies in ACK for non-2xx is NOT RECOMMENDED,
   but if done, the body types are restricted to any that appeared in
   the INVITE, assuming that the response to the INVITE was not 415.  If
   it was, the body in the ACK MAY be any type listed in the Accept
   header field in the 415.

   For example, consider the following request:

   INVITE sip:bob@biloxi.com SIP/2.0
   Via: SIP/2.0/UDP pc33.atlanta.com;branch=z9hG4bKkjshdyff
   To: Bob <sip:bob@biloxi.com>
   From: Alice <sip:alice@atlanta.com>;tag=88sja8x
   Max-Forwards: 70
   Call-ID: 987asjd97y7atg
   CSeq: 986759 INVITE

   The ACK request for a non-2xx final response to this request would
   look like this:

   ACK sip:bob@biloxi.com SIP/2.0
   Via: SIP/2.0/UDP pc33.atlanta.com;branch=z9hG4bKkjshdyff
   To: Bob <sip:bob@biloxi.com>;tag=99sa0xk
   From: Alice <sip:alice@atlanta.com>;tag=88sja8x
   Max-Forwards: 70
   Call-ID: 987asjd97y7atg
   CSeq: 986759 ACK

  */

  SIPMessage ack = m_OpeningRequest;

  /// change the RequestLine method
  StartLine startLine;
  m_OpeningRequest.GetStartLine( startLine );
  startLine.GetRequestLine()->SetMethod( "ACK" );
  ack.SetStartLine( startLine );

  //Via via;
  //m_OpeningRequest.GetViaAt( 0, via );
  //ack.AppendVia( via );

  /// Clone the response TO Header
  To to;
  response.GetTo( to );
  ack.SetTo( to );

  /// Clone the from
  From from;
  m_OpeningRequest.GetFrom( from );
  ack.SetFrom( from );

  /// Clone the callId
  CallId callId;
  m_OpeningRequest.GetCallId( callId );
  ack.SetCallId( callId );

  /// Change CSeq method to ACK
  ///SIPCSeq cseq = ack.GetMIME().GetCSeq();
  CSeq cseq;
  m_OpeningRequest.GetCSeq( cseq );
  cseq.SetMethod( "ACK" );
  ack.SetCSeq( cseq );

  /// set the max forwards to 70
  MaxForwards maxForwards( 70 );
  ack.SetMaxForwards( maxForwards );

  /// blank out body
  ack.SetBody( "" );

  /// Save ack message for retransmission
  //m_TransportSender( ack, (INT)this );
  m_Manager.OnSendMessageToTransport( ack, *this );
  
}
