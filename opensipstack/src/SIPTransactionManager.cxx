/*
 *
 * SIPTransactionManager.cxx
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
 * $Log: SIPTransactionManager.cxx,v $
 * Revision 1.24  2007/01/12 10:49:10  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.23  2007/01/01 05:12:53  joegenbaclor
 * Introduced termiantion flag to avoid crashes on exit.
 * TO DO: Maybe the best thing to do here is to have a WaitForTermination()
 * function exposed for the SIPStack
 *
 * Revision 1.22  2006/12/20 08:57:40  joegenbaclor
 * More cleanup beautification
 *
 * Revision 1.21  2006/12/19 23:38:34  joegenbaclor
 * Fixed bug in softphone proxy authentication where passwords <= 3 in length may not be properly handled
 *
 * Revision 1.20  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.19  2006/11/17 03:28:45  rcolobong
 * Update Method in Adding, removing and Finding an ACK Transaction to remove checking of To Tag
 *
 * Revision 1.18  2006/09/01 03:12:26  joegenbaclor
 * Bulk Commit:
 *
 * 1.  A bug is discovered in SIP Transaction Destructor where destruction of the SIPTimers may cause a deadlock on the timer manager effectivily freezing the entire message Queue System.  This was fixed by introducing a separate function to destroy the timers on receipt of the final event just before the transaction is queued for deletion.
 *
 * 2.  There is bug in Invite Server Transaction where ACK transaction is added to the Ack Transaction Pool after sending og none 2xx final response.  This new transaction is never removed from the pool after Timer_H has fired.  this may result to a bad pointer if ACK is received after the timer expired since the transaction may have been deleted by then.
 *
 * 3.  A few tweaks in GC GetName func to return the Class Name if GC NAme is empty.
 *
 * Revision 1.17  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.16  2006/08/15 15:42:09  joegenbaclor
 * 1. Added Instant Messaging functions to OPAL
 *
 * Revision 1.15  2006/08/14 01:29:09  joegenbaclor
 * Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
 *
 * Revision 1.14  2006/07/06 05:38:11  joegenbaclor
 * Final last minute changes to CBCOM hash support
 *
 * Revision 1.13  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.12  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.11  2006/03/24 15:42:18  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.10  2006/03/21 03:29:56  joegenbaclor
 * Added Logger functionality to log to PTrace
 *
 * Revision 1.9  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPTransactionManager.h"
#include "SIPTransactionStage.h"
#include "SIPTransportManager.h"

#define new PNEW

using namespace SIPTransactions;
using namespace SIPTransports;

SIPTransactionManager::SIPTransactionManager(
  PINDEX threadCount
)
{
  m_IsTerminating = FALSE;
  m_TransactionPool.DisallowDeleteObjects();
  m_AckTransactionPool.DisallowDeleteObjects();
  m_TransactionStage = new SIPTransactionStage( threadCount, 1 );
}

SIPTransactionManager::~SIPTransactionManager()
{
  m_IsTerminating = TRUE;
  TerminateTimers();
  delete m_TransactionStage;
}

BOOL SIPTransactionManager::CreateTransaction(
  const SIPMessage & event,
  BOOL fromTransport,
  Logger * logger
)
{
  PWaitAndSignal lock( m_TransactionPoolMutex );
  if( m_IsTerminating )
    return FALSE;

  PString idPrefix;
  SIPTransaction::Type type;

  if( logger == NULL )
  {
    logger = (Logger*)this;
    logger->SetLogContextId( event.GetCallId() );
  }

  if( fromTransport )
  {
    ///type = SIPTransaction::Server;
    if( event.IsInvite() )
    {
      type = SIPTransaction::IST;
      idPrefix = "IST";
    }else
    {
      type = SIPTransaction::NIST;
      idPrefix = "NIST";
    }
  }else
  {
    if( event.IsInvite() )
    {
      type = SIPTransaction::ICT;
      idPrefix = "ICT";
    }else
    {
      type = SIPTransaction::NICT;
      idPrefix = "NICT";

    }
  }

  TransactionId id = event.GetTransactionId();

  if( id.AsString().IsEmpty() )
  {
    LOG_IF_DEBUG( logger->LogInfo(), "CreateTransaction() - Unable to create transaction because transaction-id is empty" );
    return FALSE;
  }

  id.SetStateMachine( idPrefix );

  SIPTransaction * transaction = OnCreateTransaction( event, id, type, logger );
  if( transaction != NULL )
  {
    if( logger != NULL )
    {
      ((Logger&)(*transaction))=*logger;
      transaction->m_TimerInterfaceLogger = *logger;
    }

    m_TransactionPool.SetAt( id.AsString(), transaction );

    SIPMessage * msg = new SIPMessage( event );
    transaction->EnqueueEvent( new SIPEvent( msg ) );
  }

  return TRUE;
}

SIPTransaction * SIPTransactionManager::OnCreateTransaction(
  const SIPMessage & /*event*/,
  const TransactionId & transactionId,
  SIPTransaction::Type type,
  Logger * logger
)
{
  if( m_IsTerminating )
    return NULL;

  return new SIPTransaction( transactionId, type, *this );
}

BOOL SIPTransactionManager::FindTransactionAndAddEvent(
  SIPMessage & event,
  TransactionId & transactionId,
  BOOL fromTransport,
  Logger * logger
)
{
  PWaitAndSignal lock( m_TransactionPoolMutex );

  if( m_IsTerminating )
    return FALSE;

  BOOL hasExternalLogger = (logger != NULL);

  if( !hasExternalLogger )
    logger = (Logger*)this;

  if( event.GetStartLine().Trim() *= "sip:" )
    LOG_IF_DEBUG( LogError(), "Malformed Event!!!!" );

  LOG_IF_DEBUG( logger->LogDebugVeryHigh(), "Finding transaction for " << event.GetStartLine() );

    
  //PString id = event.GetTransactionId();
  PString idPrefix;
  SIPTransaction::Type type;

  if( fromTransport )
  {
    //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected event is from Transport" );
    if( event.IsRequest() )
    {
      //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected event is an incoming request" );
      if( event.IsInvite() )
      {
        //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected event is an INVITE.  Setting state machine to IST" );
        type = SIPTransaction::IST;
        idPrefix = "IST";
      }else if( event.IsAck() )
      {
        //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected event is an ACK." );
        if( FindAckTransactionAndAddEvent( event, transactionId ) )
          return TRUE;
      }else
      {
        //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected event is neither an ACK nor an INVITE. .  Setting state machine to IST" );
        type = SIPTransaction::NIST;
        idPrefix = "NIST";
      }
    }else
    {
      //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected event is an incoming response" );
      ///check if this is a response to an INVITE
      CSeq cseq;

      if( !event.GetCSeq( cseq ) )
      {
        LOG_IF_DEBUG( logger->LogWarning(), "Unable to match transaction because CSeq is empty or malformed" );
        return FALSE;
      }
      if( cseq.GetMethod() *= "INVITE" )
      {
        //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected event is a response to INVITE.  Setting state machine to ICT" );
        type = SIPTransaction::ICT;
        idPrefix = "ICT";
      }else
      {
        //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected event is a response to none-INVITE.  Setting state machine to NICT" );
        type = SIPTransaction::NICT;
        idPrefix = "NICT";
      }
    }
  }else
  {
    //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected none-transport event" );
    if( event.IsRequest() )
    {
      if( event.IsInvite() )
      {
        //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected event is an Invite Request.  Setting state machine to ICT" );
        type = SIPTransaction::ICT;
        idPrefix = "ICT";
      }else
      {
        //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected event is a none-INVITE Request.  Setting state machine to NICT" );
        type = SIPTransaction::NICT;
        idPrefix = "NICT";
      }
    }else
    {
      ///check if this is a response to an INVITE
      CSeq cseq;
      if( !event.GetCSeq( cseq ) )
      {
        LOG_IF_DEBUG( logger->LogWarning(), "Unable to match transaction because CSeq is empty or malformed" );
        return FALSE;
      }
      if( cseq.GetMethod() *= "INVITE" )
      {
        //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected event is a response to INVITE.  Setting state machine to IST" );
        type = SIPTransaction::IST;
        idPrefix = "IST";
      }else
      {
        //LOG_IF_DEBUG( logger->LogDebugHigh(), "Detected event is a response to none-INVITE.  Setting state machine to NIST" );
        type = SIPTransaction::NIST;
        idPrefix = "NIST";
      }
    }
  }

  PString id = event.GetTransactionId().AsString();

  LOG_IF_DEBUG( logger->LogDebugVeryHigh(), "Setting Transaction ID to " << id );

  if( id.IsEmpty() )
  {
    LOG_IF_DEBUG( logger->LogWarning(), "CreateTransaction() - Unable to create transaction because transaction-id is empty" );
    return FALSE;
  }

  id = idPrefix + "|" + id;


  transactionId = id;

  SIPTransaction * transaction = m_TransactionPool.GetAt( id );

  //LOG_IF_DEBUG( logger->LogDebugHigh(), "Transaction Pool Critical Section Acquired" );

  if( transaction == NULL )
  {
    
    if( event.IsRequest() && !event.IsAck() )
    {
      /// lets make sure that if this is a send status message
      /// that it does not create a new transaction
      if( event.GetInternalHeader( "RETRANSMISSION" ).IsEmpty() )
      {
        LOG_IF_DEBUG( logger->LogDebug(), "*** CREATING TRANSACTION (" << idPrefix << ") ***" <<
          endl << "Message: " << event.GetStartLine() << 
          endl << "Call-Id: " << transactionId.GetCallId() 
        );
        return CreateTransaction( event, fromTransport, hasExternalLogger ? logger : NULL );
      }else if( event.GetInternalHeader( "RETRANSMISSION" ) *= "FALSE" )
      {
        LOG_IF_DEBUG( logger->LogDebug(), "*** CREATING TRANSACTION (" << idPrefix << ") ***" <<
          endl << "Message: " << event.GetStartLine() << 
          endl << "Call-Id: " << transactionId.GetCallId() 
        );
        return CreateTransaction( event, fromTransport, hasExternalLogger ? logger : NULL );
      }
    }else
    {
      LOG_IF_DEBUG( logger->LogDebug(), "*** TRANSACTION DOES NOT EXIST ***" <<
        endl << "Message: " << event.GetStartLine() << 
        endl << "Call-Id: " << transactionId.GetCallId() 
      );

      OnUnknownTransaction( event, fromTransport );
      return FALSE;
    }
  }else
  {
    LOG_IF_DEBUG( logger->LogDetail(), "Found " << id << " for " << event.GetStartLine() );
    event.Finalize();
    
    SIPMessage * msg = new SIPMessage( event );
    transaction->EnqueueEvent( new SIPEvent( msg ) );
  }

  return TRUE;
}

BOOL SIPTransactionManager::FindTransactionAndAddEvent(
  const TransactionId & id,
  SIPEvent * event,
  Logger * logger
)
{
  PWaitAndSignal lock( m_TransactionPoolMutex );

  if( m_IsTerminating )
    return FALSE;

  SIPTransaction * transaction = m_TransactionPool.GetAt( id.AsString() );
  if( transaction == NULL )
  {
    delete event;
    return FALSE;
  }

  if( logger == NULL )
    logger = (Logger*)this;

  LOG_IF_DEBUG( logger->LogDetail(), "Found " << id.AsString() << " for " << *event->GetEvent() );
  transaction->EnqueueEvent( event );

  return TRUE;
}

BOOL SIPTransactionManager::RemoveTransaction(
  const TransactionId & transactionId
)
{
  PWaitAndSignal lock( m_TransactionPoolMutex );
  LOG_IF_DEBUG( LogDetail(), "*** REMOVED TRANSACTION *** " << transactionId.AsString() );
  return m_TransactionPool.RemoveAt( transactionId.AsString() ) != NULL;
}

BOOL SIPTransactionManager::FindAckTransactionAndAddEvent(
  const SIPMessage & ack,
  TransactionId & transactionId,
  Logger * logger
)
{
  if( m_IsTerminating )
    return FALSE;

  if( !ack.IsAck() )
    return FALSE;

  PString ackId;
  CallId callId;
  PString cid;
  To to;
  PString toTag;
  Via via;
  PString viaBranch;

  if( !ack.GetCallId( callId ) )
    return FALSE;

  if( !ack.GetTo( to ) )
    return FALSE;

  if( !ack.GetViaAt(0, via ) )
    return FALSE;


  to.GetParameter( "tag", toTag );
  //if( toTag.IsEmpty() )
  //  return FALSE;

  viaBranch = via.GetBranch();
  ///if( viaBranch.IsEmpty() )
  ///  return FALSE;

  cid = callId.AsString();
  if( cid.IsEmpty() )
    return FALSE;

  ackId = cid + toTag + "|" + viaBranch + "|ACK";

  transactionId = ackId;

  PWaitAndSignal lock( m_AckTransactionPoolMutex );

  SIPTransaction * transaction = m_AckTransactionPool.RemoveAt( ackId );
  if( transaction == NULL )
    return FALSE;

  SIPMessage * msg = new SIPMessage( ack );
  transaction->EnqueueEvent( new SIPEvent( msg ) );

  return TRUE;
}

BOOL SIPTransactionManager::AddAckTransaction(
  const SIPMessage & response,
  SIPTransaction * transaction
)
{
  if( m_IsTerminating )
    return FALSE;

  const SIPMessage & request = transaction->GetOpeningRequest();

  PString ackId;
  CallId callId;
  PString cid;
  To to;
  PString toTag;
  Via via;
  PString viaBranch;

  if( !request.GetCallId( callId ) )
    return FALSE;

  if( !response.GetTo( to ) )
    return FALSE;

  if( !request.GetViaAt(0, via ) )
    return FALSE;


  to.GetParameter( "tag", toTag );
  //if( toTag.IsEmpty() )
  //  return FALSE;

  viaBranch = via.GetBranch();
  //if( viaBranch.IsEmpty() )
  //  return FALSE;

  cid = callId.AsString();
  if( cid.IsEmpty() )
    return FALSE;

  ackId = cid + toTag + "|" + viaBranch + "|ACK";

  PWaitAndSignal lock( m_AckTransactionPoolMutex );

  m_AckTransactionPool.SetAt( ackId, transaction );

  return TRUE;
}

BOOL SIPTransactionManager::RemoveAckTransaction(
  const SIPMessage & response,
  SIPTransaction * transaction
)
{
  const SIPMessage & request = transaction->GetOpeningRequest();

  PString ackId;
  CallId callId;
  PString cid;
  To to;
  PString toTag;
  Via via;
  PString viaBranch;

  if( !request.GetCallId( callId ) )
    return FALSE;

  if( !response.GetTo( to ) )
    return FALSE;

  if( !request.GetViaAt(0, via ) )
    return FALSE;


  to.GetParameter( "tag", toTag );
  //if( toTag.IsEmpty() )
  //  return FALSE;

  viaBranch = via.GetBranch();
  //if( viaBranch.IsEmpty() )
  //  return FALSE;

  cid = callId.AsString();
  if( cid.IsEmpty() )
    return FALSE;

  ackId = cid + toTag + "|" + viaBranch + "|ACK";

  PWaitAndSignal lock( m_AckTransactionPoolMutex );

  m_AckTransactionPool.RemoveAt( ackId );

  return TRUE;
}

void SIPTransactionManager::OnUnknownTransaction(
  const SIPMessage &,
  BOOL
)
{

}


BOOL SIPTransactionManager::CancelInviteClientTransaction(
  const SIPMessage & invite
)
{
  if( m_IsTerminating )
    return FALSE;

  /*
  9 Canceling a Request

   The previous section has discussed general UA behavior for generating
   requests and processing responses for requests of all methods.  In
   this section, we discuss a general purpose method, called CANCEL.

   The CANCEL request, as the name implies, is used to cancel a previous
   request sent by a client.  Specifically, it asks the UAS to cease
   processing the request and to generate an error response to that
   request.  CANCEL has no effect on a request to which a UAS has
   already given a final response.  Because of this, it is most useful
   to CANCEL requests to which it can take a server long time to
   respond.  For this reason, CANCEL is best for INVITE requests, which
   can take a long time to generate a response.  In that usage, a UAS
   that receives a CANCEL request for an INVITE, but has not yet sent a
   final response, would "stop ringing", and then respond to the INVITE
   with a specific error response (a 487).

   CANCEL requests can be constructed and sent by both proxies and user
   agent clients.  Section 15 discusses under what conditions a UAC
   would CANCEL an INVITE request, and Section 16.10 discusses proxy
   usage of CANCEL.

   A stateful proxy responds to a CANCEL, rather than simply forwarding
   a response it would receive from a downstream element.  For that
   reason, CANCEL is referred to as a "hop-by-hop" request, since it is
   responded to at each stateful proxy hop.

9.1 Client Behavior

   A CANCEL request SHOULD NOT be sent to cancel a request other than
   INVITE.

      Since requests other than INVITE are responded to immediately,
      sending a CANCEL for a non-INVITE request would always create a
      race condition.





Rosenberg, et. al.          Standards Track                    [Page 53]

RFC 3261            SIP: Session Initiation Protocol           June 2002


   The following procedures are used to construct a CANCEL request.  The
   Request-URI, Call-ID, To, the numeric part of CSeq, and From header
   fields in the CANCEL request MUST be identical to those in the
   request being cancelled, including tags.  A CANCEL constructed by a
   client MUST have only a single Via header field value matching the
   top Via value in the request being cancelled.  Using the same values
   for these header fields allows the CANCEL to be matched with the
   request it cancels (Section 9.2 indicates how such matching occurs).
   However, the method part of the CSeq header field MUST have a value
   of CANCEL.  This allows it to be identified and processed as a
   transaction in its own right (See Section 17).

   If the request being cancelled contains a Route header field, the
   CANCEL request MUST include that Route header field's values.

      This is needed so that stateless proxies are able to route CANCEL
      requests properly.

   The CANCEL request MUST NOT contain any Require or Proxy-Require
   header fields.

   Once the CANCEL is constructed, the client SHOULD check whether it
   has received any response (provisional or final) for the request
   being cancelled (herein referred to as the "original request").

   If no provisional response has been received, the CANCEL request MUST
   NOT be sent; rather, the client MUST wait for the arrival of a
   provisional response before sending the request.  If the original
   request has generated a final response, the CANCEL SHOULD NOT be
   sent, as it is an effective no-op, since CANCEL has no effect on
   requests that have already generated a final response.  When the
   client decides to send the CANCEL, it creates a client transaction
   for the CANCEL and passes it the CANCEL request along with the
   destination address, port, and transport.  The destination address,
   port, and transport for the CANCEL MUST be identical to those used to
   send the original request.

      If it was allowed to send the CANCEL before receiving a response
      for the previous request, the server could receive the CANCEL
      before the original request.

   Note that both the transaction corresponding to the original request
   and the CANCEL transaction will complete independently.  However, a
   UAC canceling a request cannot rely on receiving a 487 (Request
   Terminated) response for the original request, as an RFC 2543-
   compliant UAS will not generate such a response.  If there is no
   final response for the original request in 64*T1 seconds (T1 is




Rosenberg, et. al.          Standards Track                    [Page 54]

RFC 3261            SIP: Session Initiation Protocol           June 2002


   defined in Section 17.1.1.1), the client SHOULD then consider the
   original transaction cancelled and SHOULD destroy the client
   transaction handling the original request.

9.2 Server Behavior

   The CANCEL method requests that the TU at the server side cancel a
   pending transaction.  The TU determines the transaction to be
   cancelled by taking the CANCEL request, and then assuming that the
   request method is anything but CANCEL or ACK and applying the
   transaction matching procedures of Section 17.2.3.  The matching
   transaction is the one to be cancelled.

   The processing of a CANCEL request at a server depends on the type of
   server.  A stateless proxy will forward it, a stateful proxy might
   respond to it and generate some CANCEL requests of its own, and a UAS
   will respond to it.  See Section 16.10 for proxy treatment of CANCEL.

   A UAS first processes the CANCEL request according to the general UAS
   processing described in Section 8.2.  However, since CANCEL requests
   are hop-by-hop and cannot be resubmitted, they cannot be challenged
   by the server in order to get proper credentials in an Authorization
   header field.  Note also that CANCEL requests do not contain a
   Require header field.

   If the UAS did not find a matching transaction for the CANCEL
   according to the procedure above, it SHOULD respond to the CANCEL
   with a 481 (Call Leg/Transaction Does Not Exist).  If the transaction
   for the original request still exists, the behavior of the UAS on
   receiving a CANCEL request depends on whether it has already sent a
   final response for the original request.  If it has, the CANCEL
   request has no effect on the processing of the original request, no
   effect on any session state, and no effect on the responses generated
   for the original request.  If the UAS has not issued a final response
   for the original request, its behavior depends on the method of the
   original request.  If the original request was an INVITE, the UAS
   SHOULD immediately respond to the INVITE with a 487 (Request
   Terminated).  A CANCEL request has no impact on the processing of
   transactions with any other method defined in this specification.

   Regardless of the method of the original request, as long as the
   CANCEL matched an existing transaction, the UAS answers the CANCEL
   request itself with a 200 (OK) response.  This response is
   constructed following the procedures described in Section 8.2.6
   noting that the To tag of the response to the CANCEL and the To tag
   in the response to the original request SHOULD be the same.  The
   response to CANCEL is passed to the server transaction for
   transmission.
*/

  PAssert( invite.IsInvite(), PLogicError );

  PWaitAndSignal lock( m_TransactionPoolMutex );

  TransactionId id = invite.GetTransactionId();

  if( id.AsString().IsEmpty() )
  {
    LOG_IF_DEBUG( LogInfo(), "CancelInviteClientTransaction() - Unable to match transaction because transaction-id is empty" );
    return FALSE;
  }

  id.SetStateMachine( "ICT" );

  SIPTransaction * transaction = m_TransactionPool.GetAt( id.AsString() );
  if( transaction == NULL )
  {
    return FALSE;
  }

  ///SIPTransaction * cancelTrn = OnCreateTransaction(


  StartLine startLine;
  From from;
  To to;
  Via via;
  CSeq cseq;
  CallId callId;
  MaxForwards maxForwards( "70" );

  SIPMessage cancel;

  if( !invite.GetStartLine( startLine ) )
    return FALSE;

  startLine.GetRequestLine()->SetMethod( "CANCEL" );
  cancel.SetStartLine( startLine );

  if( !invite.GetFrom( from ) )
    return FALSE;

  cancel.SetFrom( from );

  if( !invite.GetTo( to ) )
    return FALSE;

  cancel.SetTo( to );

  if( !invite.GetViaAt(0,via) )
    return FALSE;

  cancel.AppendVia( via );

  if( !invite.GetCSeq( cseq ) )
    return FALSE;

  cseq.SetMethod( "CANCEL" );
  cancel.SetCSeq( cseq );

  if( !invite.GetCallId( callId ) )
    return FALSE;

  cancel.SetCallId( callId );

  cancel.SetMaxForwards( maxForwards );

  cancel.SetBody( "" );

  TransactionId transactionId = PString( PString( "NICT|" ) + cancel.GetTransactionId().AsString() );

  SIPTransaction * cancelTransaction = OnCreateTransaction( cancel, transactionId, SIPTransaction::NICT );

  if( cancelTransaction == NULL )
    return FALSE;

  m_TransactionPool.SetAt( transactionId.AsString(), cancelTransaction );
  
  /// make sure we set the XOR Hash for cancel if it was set in the INVITE
  cancel.SetXORHash( invite.IsXORHashSet() );

  PIPSocket::Address targetAddress;
  WORD targetPort = 5060;
  SIPTransport::Resolve( invite, targetAddress, targetPort );
  SIPURI sendAddress;
  sendAddress.SetHost( targetAddress.AsString() );
  sendAddress.SetPort( PString( targetPort ) );
  cancel.SetSendAddress( sendAddress );
  
  SIPEvent * sipEvent = new SIPEvent( new SIPMessage( cancel ) );
  cancelTransaction->EnqueueEvent( sipEvent );
  
  return TRUE;
}

BOOL SIPTransactionManager::CancelInviteServerTransaction(
  const SIPMessage & cancel
)
{
  if( m_IsTerminating )
    return FALSE;

  SIPMessage invite = cancel;
  ///change the method to INVITE
  StartLine startLine;
  invite.GetStartLine( startLine );
  startLine.GetRequestLine()->SetMethod( "INVITE" );
  invite.SetStartLine( startLine );

  CSeq cseq;
  invite.GetCSeq( cseq );
  cseq.SetMethod( "INVITE" );
  invite.SetCSeq( cseq );

  TransactionId transactionId = PString( PString("IST|") + invite.GetTransactionId().AsString() );
  return FindTransactionAndAddEvent( transactionId, new SIPCancelEvent() );

}




