/*
 *
 * SIPTransport.cxx
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
 * $Log: SIPTransport.cxx,v $
 * Revision 1.21  2007/01/14 13:12:56  joegenbaclor
 * Fixed compile warnings
 *
 * Revision 1.20  2007/01/06 07:30:33  joegenbaclor
 * Added mask to filter GetDefaultInterfaceAddress
 *
 * Revision 1.19  2006/12/22 07:37:21  joegenbaclor
 * Introduced separate funtion call  for starting of transport thread.  This is necessary
 *  to make sure that the backdoor listener is already initialized prior to calling transport
 *  reads
 *
 * Revision 1.18  2006/12/04 08:55:32  joegenbaclor
 * Added log hook to transport
 *
 * Revision 1.17  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.16  2006/11/21 06:55:19  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.15  2006/11/08 06:46:22  joegenbaclor
 * Added session level NIC/Interface binding
 *
 * Revision 1.14  2006/11/07 09:30:46  joegenbaclor
 * Added new internal header "IFACE-ADDR" to allow transctions to remember the interface address for sending future responses within the transaction lifespan
 *
 * Revision 1.13  2006/09/22 05:54:14  joegenbaclor
 * Changed priority of reader and writer thread fro normal to highest priority
 *
 * Revision 1.12  2006/09/01 13:25:52  rcolobong
 * Create static method SetMinBufferSize to specify Maximum Buffer Size for PSocket
 *
 * Revision 1.11  2006/07/17 11:36:45  joegenbaclor
 * 1.  More routing enhancements to B2BUA
 *
 * Revision 1.10  2006/06/28 14:15:00  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.9  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.8  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.7  2006/03/25 18:40:21  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.6  2006/03/24 15:42:18  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.5  2006/03/21 14:11:51  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.4  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPTransport.h"
#include "SIPTransportManager.h"


#if P_DNS
#include <ptclib/pdns.h>
#endif

#define new PNEW

using namespace SIPTransports;

BOOL SIPTransport::m_HasReadNotifier = FALSE;
BOOL SIPTransport::m_HasWriteNotifier = FALSE;
PNotifier SIPTransport::m_ReadNotifier;
PNotifier SIPTransport::m_WriteNotifier;


/////////////////////////////////////////////////////////////////////////////////

SIPTransport::ReadThread::ReadThread(
  SIPTransport * transport
  ) : PThread( 100000, NoAutoDeleteThread, HighestPriority, "Transport( READ )" )
{
  m_Transport = transport;
  m_ExitThread = FALSE;
  Resume();
}

SIPTransport::ReadThread::~ReadThread()
{
  m_ExitThread = TRUE;
  PAssert( WaitForTermination( 5000 ), "Transport Reader Thread failed to terminate!" ); 
}

void SIPTransport::ReadThread::Main()
{
  while( !m_ExitThread )
  {
    if( !m_Transport->ProcessRead() )
      return;
  }
}

/////////////////////////////////////////////////////////////////////////////////

SIPTransport::WriteThread::WriteThread(
  SIPTransport * transport
  ) : PThread( 100000, NoAutoDeleteThread, HighestPriority, "Transport( WRITE )" )
{
  m_Transport = transport;
  m_ExitThread = FALSE;
  Resume();
}

SIPTransport::WriteThread::~WriteThread()
{
  m_ExitThread = TRUE;
  PAssert( WaitForTermination( 5000 ), "Transport Writer Thread failed to terminate!" );
}

void SIPTransport::WriteThread::Main()
{
  while( !m_ExitThread )
  {
    if( !m_Transport->ProcessWrite() )
      break;
  }
}

/////////////////////////////////////////////////////////////////////////////////


SIPTransport::SIPTransport(
  SIPTransportManager * manager,
  Protocol proto
) : m_Manager( manager ),
    m_Protocol( proto ),
    m_WriteQueueSync( 0, INT_MAX )   
{
  m_IsListening = FALSE;
  m_ReadThread = NULL;
  m_WriteThread = NULL;
  m_ListenerPort = 0;
}

SIPTransport::~SIPTransport()
{
}

void SIPTransport::CleanUp()
{
  //m_ReadQueueSync.Signal();
  m_WriteQueueSync.Signal();

  if( m_ReadThread != NULL )
  {
    delete m_ReadThread;
    m_ReadThread = NULL;
  }

  if( m_WriteThread != NULL )
  {
    delete m_WriteThread;
    m_WriteThread = NULL;
  }
}

void SIPTransport::SetMinBufferSize(
  PUDPSocket & sock, 
  int buftype,
  int bufferSize )
{
  int sz = 0;
  if (sock.GetOption(buftype, sz)) {
    if (sz >= bufferSize )
      return;
  }

  if (!sock.SetOption(buftype, bufferSize)) {
    PTRACE(1, "SIPTransport\tSetMinBufferSize(" << buftype << ") failed: " << sock.GetErrorText());
  }
}

/////////////////////////////////////////////////////
/// enqueues a new packet for sending
/// none-blocking
void SIPTransport::Enqueue( 
  const SIPMessage & packet 
)
{
  PWaitAndSignal lock(m_WriteQueueMutex);
  SIPMessage * msg = new SIPMessage( packet );
  m_WriteQueue.Enqueue( msg );
  m_WriteQueueSync.Signal();
}

/*
/// Dequeues the new received packet
/// Will block until a packet is received
BOOL SIPTransport::Dequeue( SIPMessage & packet )
{
  m_ReadQueueSync.Wait();
  PWaitAndSignal lock( m_ReadQueueMutex );
  SIPMessage *msg = m_ReadQueue.Dequeue();

  if( msg == NULL )
    return FALSE;

  packet = *msg;
  delete msg;

  return TRUE;
    
}

/// Enqueue read packets.  Should never be used outside of the transport
/// read thread
void SIPTransport::InternalEnqueueRead(
  SIPMessage * packet
)
{
  PWaitAndSignal lock(m_ReadQueueMutex);
  m_ReadQueue.Enqueue( packet );
  m_ReadQueueSync.Signal();
}
*/
/// Dequeue packets to be written.  Should never be used outside of the transport
/// write thread
SIPMessage * SIPTransport::InternalDequeueWrite()
{
  m_WriteQueueSync.Wait();
  PWaitAndSignal lock( m_WriteQueueMutex );
  return m_WriteQueue.Dequeue();
}

/////////////////////////////////////////////////////

BOOL SIPTransport::ProcessRead()
{
  SIPMessage * packet = new SIPMessage();
  
  PIPSocket::Address readAddress;
  PIPSocket::Address ifaceAddress;
  WORD readPort = 0;
  int readLen = 0;
  WORD localPort = 0;
  BOOL ok = ReadPacket( packet, readAddress, readPort, ifaceAddress, localPort, readLen );

  if( readLen < MIN_SIP_MESSAGE_LENGTH )
  {
    /// silently discard
    /// Most NAT devices send empty packets to keep the NAT binding open
    ///if( readLen != 0 )
    ///{
    ///  PTRACE( 4, "RCV: Silently discarding " 
    ///    << readLen << " bytes from " << readAddress << ":" << readPort  );
    ///}

    delete packet;
    return TRUE;
  }
  
  if( !ok )
  {
    delete packet;
    return FALSE;
  }

  packet->SetInterfaceAddress( ifaceAddress.AsString() );
  packet->SetInterfacePort( localPort );

  m_Manager->OnTransportEvent( 
    new SIPTransportEvent( 
      packet, 
      SIPTransportEvent::UDPPacketArrival 
  ) );

  return TRUE;
}

BOOL SIPTransport::ProcessWrite()
{
  SIPMessage * packet = InternalDequeueWrite();

  if( packet == NULL )
    return FALSE;

  BOOL ok = WritePacket( *packet );

  BOOL isKeepalive = packet->IsKeepAlive();

  

  if( isKeepalive )
  {
    delete packet;
    return ok;
  }
  
  if( ok )
  {
    m_Manager->OnTransportEvent( 
    new SIPTransportEvent( 
      packet, 
      SIPTransportEvent::UDPPacketSent 
    ) );
  }else
  {
    m_Manager->OnTransportEvent( 
    new SIPTransportEvent( 
      packet, 
      SIPTransportEvent::UDPWriteError
  ) );
  }
  return TRUE;
}


BOOL SIPTransport::m_IsInitialized = FALSE;


BOOL SIPTransport::Initialize()
{
  if( SIPTransport::m_IsInitialized )
    return TRUE;

  SIPTransport::m_IsInitialized = TRUE;

  return SIPTransport::m_IsInitialized;
}

void SIPTransport::Deinitialize()
{
  if( !SIPTransport::m_IsInitialized )
    return;

  SIPTransport::m_IsInitialized = FALSE;

}



BOOL SIPTransport::IsLocalAddress( 
  const PIPSocket::Address & address
)
{
  return PIPSocket::IsLocalHost(address);
}

BOOL SIPTransport::IsLocalAddress(
  const SIPURI & uri
)
{
  PIPSocket::Address destAddr;
  WORD port;
  
  if( !SIPTransport::Resolve( uri, destAddr, port ) )
    return FALSE;

  return SIPTransport::IsLocalAddress( destAddr );
}

BOOL SIPTransport::IsTheSameAddress(
  const SIPURI & uri_a,
  const SIPURI & uri_b,
  BOOL treatNoPortAs5060
)
{
  PIPSocket::Address addr_a, addr_b;
  WORD port_a = 0, port_b = 0;

  if( !SIPTransport::Resolve( uri_a, addr_a, port_a ) )
    return FALSE;

  if( !SIPTransport::Resolve( uri_b, addr_b, port_b ) )
    return FALSE;

  if( treatNoPortAs5060 && port_a == 0 )
    port_a = 5060;

  if( treatNoPortAs5060 && port_b == 0 )
    port_b = 5060;

  return addr_a == addr_b && port_a == port_b;
}

BOOL SIPTransport::Resolve(
  const SIPMessage & msg,
  PIPSocket::Address & destAddr,
  WORD & port
)
{
  port = 0;

  SIPSrvRecord srvRec( msg );

  const SIPSrvRecord::HostRecord * udpRec = srvRec.GetRecordAt( 0, "UDP" );
  if( udpRec != NULL )
  {
    destAddr = udpRec->m_Address;
    port = udpRec->m_Port;
  }else
  {
    const SIPSrvRecord::HostRecord * tcpRec = srvRec.GetRecordAt( 0, "TCP" );
    if( tcpRec != NULL )
    {
      destAddr = tcpRec->m_Address;
      port = udpRec->m_Port;
    }else
    {
      return FALSE;
    }
  }

  return TRUE;
}

BOOL SIPTransport::Resolve(
  const SIPURI & uri,
  PIPSocket::Address & destAddr,
  WORD & port
)
{
  port = 0;

  SIPSrvRecord srvRec( uri );

  const SIPSrvRecord::HostRecord * udpRec = srvRec.GetRecordAt( 0, "UDP" );
  if( udpRec != NULL )
  {
    destAddr = udpRec->m_Address;
    port = udpRec->m_Port;
  }else
  {
    const SIPSrvRecord::HostRecord * tcpRec = srvRec.GetRecordAt( 0, "TCP" );
    if( tcpRec != NULL )
    {
      destAddr = tcpRec->m_Address;
      port = udpRec->m_Port;
    }else
    {
      return FALSE;
    }
  }

  return TRUE;
}


BOOL SIPTransport::IsPrivateNetwork( 
  const PIPSocket::Address & address 
)
{
  return address.IsRFC1918();
}

BOOL SIPTransport::IsPrivateNetwork(
  const SIPURI & address
)
{
  PIPSocket::Address ip;
  WORD port;

  if( !SIPTransport::Resolve( address, ip, port ) )
    return TRUE;

  return ip.IsRFC1918();
}


BOOL SIPTransport::ENUMLookup(
  const PStringArray & enumServer,
  const PString & e164,
  SIPURI & result
)
{
  PDNS::SetENUMServers(enumServer);
  PString enumResult;

  if( PDNS::ENUMLookup( 
    e164,
    "E2U+SIP",
    enumResult ) )
  {
    result = SIPURI( enumResult );
    return !result.GetHost().IsEmpty();
  }

  return FALSE;
}


void SIPTransport::AttachReadNotifier( 
  const PNotifier & notifier 
)
{
  SIPTransport::m_HasReadNotifier = TRUE;
  SIPTransport::m_ReadNotifier = notifier;
}

void SIPTransport::AttachWriteNotifier( 
  const PNotifier & notifier 
)
{
  SIPTransport::m_HasWriteNotifier = TRUE;
  SIPTransport::m_WriteNotifier = notifier;
}

BOOL SIPTransport::StartThreads()
{
  if( m_IsListening && m_ReadThread == NULL && m_WriteThread == NULL )
  {
    m_ReadThread = new ReadThread( this );
    m_WriteThread = new WriteThread( this );
    return TRUE;
  }

  return FALSE;
}




