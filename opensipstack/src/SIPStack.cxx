/*
 *
 * SIPStack.cxx
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
 * $Log: SIPStack.cxx,v $
 * Revision 1.31  2007/01/22 10:01:58  joegenbaclor
 * Fixed bug in handling Notifies
 *
 * Revision 1.30  2007/01/01 05:12:53  joegenbaclor
 * Introduced termiantion flag to avoid crashes on exit.
 * TO DO: Maybe the best thing to do here is to have a WaitForTermination()
 * function exposed for the SIPStack
 *
 * Revision 1.29  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.28  2006/11/21 06:55:19  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.27  2006/11/17 11:23:24  joegenbaclor
 * 1.  Added Max-Forwards header to all outbound requests if not present
 * 2.  SetUACoreName() in SIPStack::OnReceivedMessage()
 *
 * Revision 1.26  2006/11/17 04:22:22  joegenbaclor
 * Added ability to remember core to use via m_UACoreName
 *
 * Revision 1.25  2006/11/08 06:46:20  joegenbaclor
 * Added session level NIC/Interface binding
 *
 * Revision 1.24  2006/11/07 12:43:23  joegenbaclor
 * Corrected bug in GetListenerAddress() when binding to a single interface
 *
 * Revision 1.23  2006/11/07 09:30:46  joegenbaclor
 * Added new internal header "IFACE-ADDR" to allow transctions to remember the interface address for sending future responses within the transaction lifespan
 *
 * Revision 1.22  2006/10/30 02:42:27  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.21  2006/09/15 15:31:39  joegenbaclor
 * Changed Log levels to decrease log size.
 * Changed default session keep alaive interval from 10 seconds to two minutes
 *
 * Revision 1.20  2006/09/10 00:57:08  joegenbaclor
 * modified logging to include XOR hash status
 *
 * Revision 1.19  2006/09/01 07:29:24  rcolobong
 * Change LOG to LOG_CONTEXT during receiving and transmit logging
 *
 * Revision 1.18  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.17  2006/08/14 10:30:35  joegenbaclor
 * Fixed bug in MakeCall where send address is never set
 *
 * Revision 1.16  2006/08/14 01:29:09  joegenbaclor
 * Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
 *
 * Revision 1.15  2006/08/06 02:32:08  joegenbaclor
 * Corrected bug regarding blank call-ids for UnknownTransaction events
 *
 * Revision 1.14  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.13  2006/04/18 04:31:52  hartley2481
 * *** empty log message ***
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
 * Revision 1.10  2006/03/21 14:11:51  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.9  2006/03/21 09:03:57  joegenbaclor
 * Introduced Terminate() method in SIPTransportManager to allow
 * SIPStack to terminate Transport threads prior to the deletion of the manager
 *
 * Revision 1.8  2006/03/17 13:51:00  joegenbaclor
 * 1.  Implemented UAC Call Redirection
 * 2.  Implemented SendCancel() method in CallSession
 * 3.  Introduced ClearInternalHeaders() function in SIP Message to erase internal headers inheritted from previous transaction.
 *
 * Revision 1.7  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include <ptclib/cypher.h>
#include "SIPStack.h"


using namespace SIP;

#define new PNEW

SIPStack::SIPStack()
  : m_ReadQueueSync( 0, INT_MAX )
{
  m_TransportManager = new SIPTransportManager( PCREATE_NOTIFIER( _OnTransportEvent ) );
}

SIPStack::~SIPStack()
{
  delete m_TransportManager;
}

/// pure virtuals from SIPTransactionManager
void SIPStack::OnReceivedMessage(
  const SIPMessage & msg,
  SIPTransaction & transaction
)
{
  PWaitAndSignal lock(m_ReadQueueMutex);

    if( m_IsTerminating )
    return;

  LOG( LogDebug(), "Event: " << "SIPStack::Enqueue(" <<  msg.GetStartLine() << ")" );

  SIPMessage message = msg;
  message.SetUACoreName( transaction.GetUACoreName() );
  m_ReadQueue.Enqueue( new SIPMessageArrival( message, transaction.GetIdentifier() ) );
  m_ReadQueueSync.Signal();
}

void SIPStack::OnSendMessageToTransport(
  const SIPMessage & msg,
  SIPTransaction & transaction
)
{
  if( m_IsTerminating )
    return;

  SIPMessage message = msg;
  SIPHeader hProto( "SendProto" );
  SIPTransportEvent::Type eventType = SIPTransportEvent::PacketSend;

  if( message.GetInternalHeader( "SENDPROTO", hProto ) )
  {
    if( hProto.GetHeaderBody() *= "UDP" )
      eventType = SIPTransportEvent::UDPPacketSend;
    else if( hProto.GetHeaderBody() *= "TCP" )
      eventType = SIPTransportEvent::TCPPacketSend;
    else if( hProto.GetHeaderBody() *= "TLS" )
      eventType = SIPTransportEvent::TLSPacketSend;
  }

  message.SetInterfaceAddress( transaction.GetInterfaceAddress() );
  message.SetInterfacePort( transaction.GetInterfacePort() );

  PIPSocket::Address dst;
  WORD dstPort;
  SIPTransport::Resolve( msg, dst, dstPort );

  PStringStream logInfo;
  
  logInfo << "TRANSMIT: ";

  if( message.IsRequest() )
  {
    logInfo << " REQ: " << message.GetCSeqMethod() << 
      " (" << message.GetCSeqNumber() << ")" << endl;
    logInfo << " TO: " << message.GetToURI() << endl;
  }else
  {
    logInfo << "RESPONSE: " << message.GetStartLine() << endl;
    logInfo << " TO: " << message.GetToURI() << endl;
    logInfo << " REQ: " << message.GetCSeqMethod() << 
            " (" << message.GetCSeqNumber() << ")" << endl;
  }

  logInfo << " XOR: " << (message.IsXORHashSet() ? "TRUE" : "FALSE") << endl; 
  LOG_CONTEXT( LogDebug(), msg.GetCallId(), logInfo << " DST: " << dst << " PORT: " << dstPort;  );

  
  message.SetInternalHeader( "TRN-ID", transaction.GetIdentifier().AsString() );
  EnqueueTransportWriteEvent( message, eventType );
}

void SIPStack::EnqueueTransportWriteEvent(
  const SIPMessage & message,
  SIPTransportEvent::Type eventType
)
{
  if( m_IsTerminating )
    return;

  SIPMessage * msg = new SIPMessage( message );
  
  msg->SetInternalHeader( "TRN-ID", msg->GetTransactionId().AsString());

  m_TransportManager->EnqueueEvent( new SIPTransportEvent( msg, eventType ) );
}



void SIPStack::OnTimerExpire(
  SIPTimerEvent & timer,
  SIPTransaction & transaction
)
{
  PWaitAndSignal lock(m_ReadQueueMutex);
  if( m_IsTerminating )
    return;

  m_ReadQueue.Enqueue( new SIPTimerExpire( timer.GetType(), transaction.GetIdentifier(), transaction.GetUACoreName() ) );
  m_ReadQueueSync.Signal();
}

void SIPStack::OnUnknownTransaction(
  const SIPMessage & message,
  BOOL fromTransport
)
{
  PWaitAndSignal lock(m_ReadQueueMutex);
  if( m_IsTerminating )
    return;

  TransactionId tid( message );
  m_ReadQueue.Enqueue( new SIPUnknownTransaction( message, tid, fromTransport ) );
  m_ReadQueueSync.Signal();
}

void SIPStack::_OnTransportEvent(
  SIPTransportManager & /*manager*/,
  INT event
)
{
  if( m_IsTerminating )
    return;

  if( event != 0 )
    OnTransportEvent( (SIPTransportEvent *)event );
}

void SIPStack::OnTransportEvent(
  SIPTransportEvent * event
)
{
  PWaitAndSignal lock(m_ReadQueueMutex);

   if( m_IsTerminating )
    return;

  SIPHeader h( "TRN-ID" );
  TransactionId trnId;
  SIPMessage * msg = NULL;
  SIPMessage msg2;
  PStringStream logInfo;

  switch( event->m_Type )
  {
    case SIPTransportEvent::TLSWriteError:
    case SIPTransportEvent::TCPWriteError:
    case SIPTransportEvent::UDPWriteError:
      msg = (SIPMessage*)event->GetEvent();
      //PAssertNULL( msg );
      msg->GetInternalHeader( "TRN-ID", h );
      trnId = h.GetHeaderBody();
      PAssert( !trnId.AsString().IsEmpty(), PLogicError );
      m_ReadQueue.Enqueue( new SIPTransportWriteError( *msg , trnId, event->m_SocketError ) );
      break;
    case SIPTransportEvent::UDPPacketSent:
    case SIPTransportEvent::TCPPacketSent:
    case SIPTransportEvent::TLSPacketSent:

      msg = (SIPMessage*)event->GetEvent();
      msg->GetInternalHeader( "TRN-ID", h );
      trnId = h.GetHeaderBody();
      PAssert( !trnId.AsString().IsEmpty(), PLogicError );

      m_ReadQueue.Enqueue( new SIPTransportWrite( *msg , trnId ) );

      //// clone the msg
      msg2 = SIPMessage( *msg );
      /// Inform the transport that this message would be a retransmission
      /// if resent in the future
      ///if( msg2.GetInternalHeader( "RETRANSMISSION" ).IsEmpty() )
      msg2.SetInternalHeader( "RETRANSMISSION", "TRUE" );
      
      FindTransactionAndAddEvent( msg2, trnId, FALSE );
      break;
    case SIPTransportEvent::UDPPacketArrival:
    case SIPTransportEvent::TCPPacketArrival:
    case SIPTransportEvent::TLSPacketArrival:

      msg = (SIPMessage*)event->GetEvent();
      msg->Finalize();

      if( msg->IsRequest() )
        msg->FixViaNATParameters();

      logInfo << "RECEIVE: ";

      if( msg->IsRequest() )
      {
        logInfo << " REQ: " << msg->GetCSeqMethod() << 
          " (" << msg->GetCSeqNumber() << ")" << endl;
        logInfo << " FROM: " << msg->GetFromURI() << endl;
        logInfo << " TO: " << msg->GetToURI() << endl;
      }else
      {
        logInfo << "RESPONSE: " << msg->GetStartLine() << endl;
        logInfo << " TO: " << msg->GetToURI() << endl;
        logInfo << " REQ: " << msg->GetCSeqMethod() << 
                " (" << msg->GetCSeqNumber() << ")" << endl;
      }

      logInfo << " XOR: " << (msg->IsXORHashSet()? "TRUE" : "FALSE") << endl; 

      LOG_CONTEXT( LogDebug(), msg->GetCallId(), logInfo << "SRC: " 
        << msg->GetInternalHeader( "RCVADDR" ) 
        << " PORT: " <<  msg->GetInternalHeader( "RCVPORT" ) );

     
      FindTransactionAndAddEvent( *msg, trnId, TRUE );
      break;
    default:
      break;
  };

  delete event;

  m_ReadQueueSync.Signal();
}

SIPStackEvent * SIPStack::ReadEvent()
{
  m_ReadQueueSync.Wait();
  PWaitAndSignal lock( m_ReadQueueMutex );
  return m_ReadQueue.Dequeue();
}

void SIPStack::Terminate(
  PINDEX signalCount
)
{
  PWaitAndSignal lock(m_ReadQueueMutex);
  m_IsTerminating = TRUE;

  for( PINDEX i = 0; i < signalCount; i++ )
  {
    m_ReadQueue.Enqueue( new SIPStackExit() );
    m_ReadQueueSync.Signal();
  }

  m_TransportManager->Terminate();
}

//void SIPStack::DestroyObject(
//  SIPSmartObject * staleObject
//)
//{
//  PWaitAndSignal lock(m_ReadQueueMutex);
//  m_ReadQueue.Enqueue( new SIPGarbageCollect( staleObject ) );
//  m_ReadQueueSync.Signal();

//}

BOOL SIPStack::ConstructInvite(
  SIPMessage & invite,
  const PString & uri,
  const PString & toURI,
  const PString & fromURI,
  const SDPPacket & sdp
)
{
    return TRUE;
}




