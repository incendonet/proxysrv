/*
 *
 * SIPUDPSocketList.cxx
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
 * $Log: SIPUDPSocketList.cxx,v $
 * Revision 1.32  2007/01/06 07:30:33  joegenbaclor
 * Added mask to filter GetDefaultInterfaceAddress
 *
 * Revision 1.31  2007/01/04 09:35:14  joegenbaclor
 * Move call PTRACE in GetSocket()
 *
 * Revision 1.30  2006/12/21 17:40:44  joegenbaclor
 * Corrected Visual Studio 2003 warning
 *
 * Revision 1.29  2006/12/07 09:12:21  joegenbaclor
 * 1.  Changed registration expire value from milliseconds to seconds
 *
 * Revision 1.28  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.27  2006/11/29 05:22:03  rcolobong
 * Fix another bug in GetListenerAddress
 *
 * Revision 1.26  2006/11/28 13:36:03  joegenbaclor
 * Yet again, a bug found in GetListenerAddress()!!!
 *
 * Revision 1.25  2006/11/22 00:05:52  joegenbaclor
 * Bug fixes for multi-listener support
 *
 * Revision 1.24  2006/11/21 06:55:19  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.23  2006/11/17 04:22:22  joegenbaclor
 * Added ability to remember core to use via m_UACoreName
 *
 * Revision 1.22  2006/11/07 12:43:23  joegenbaclor
 * Corrected bug in GetListenerAddress() when binding to a single interface
 *
 * Revision 1.21  2006/11/07 09:30:46  joegenbaclor
 * Added new internal header "IFACE-ADDR" to allow transctions to remember the interface address for sending future responses within the transaction lifespan
 *
 * Revision 1.20  2006/11/07 08:17:57  joegenbaclor
 * More bug fixes to make sure we have a default listener socket
 *
 * Revision 1.19  2006/11/02 05:51:28  joegenbaclor
 * More sanity check on GetSocket() returning NULL
 *
 * Revision 1.18  2006/10/30 02:42:27  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.17  2006/10/23 15:44:58  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.16  2006/09/22 08:52:45  joegenbaclor
 * Temporary fix for OnRequireSDPAnswer to use a semaphore to wait for second leg SDP
 *
 * Revision 1.15  2006/09/18 03:28:55  joegenbaclor
 * 1. More work on media aggregation
 * 2. Check sock->GetPort() for NULL in SIPTransportManager::GetListenerAddress() (thanks to
 *  Vamsi Pottangi for reporting the bug)
 * 3.  Added reference size check to GCObject::ReleaseRef()
 *
 * Revision 1.14  2006/09/06 14:08:42  joegenbaclor
 * Moved Select List to a separate container
 *
 * Revision 1.13  2006/09/06 09:56:26  joegenbaclor
 * Fixed Invalid Index Assertion in slect list.  Somehow GetDataAt() is not reliable
 *
 * Revision 1.12  2006/09/06 04:14:36  joegenbaclor
 * Added check on packet size for SIP UDP Transport Reads
 *
 * Revision 1.11  2006/09/01 13:24:50  rcolobong
 * Create static method SetMinBufferSize to specify Maximum Buffer Size for PSocket
 *
 * Revision 1.10  2006/07/06 05:38:11  joegenbaclor
 * Final last minute changes to CBCOM hash support
 *
 * Revision 1.9  2006/07/03 15:10:05  joegenbaclor
 * Added Grandstream CBCom support for basic XOR hash
 *
 * Revision 1.8  2006/04/06 04:27:51  joegenbaclor
 * More work on OPAL Inbound
 *
 * Revision 1.7  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.6  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.5  2006/03/14 03:53:54  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPUDPSocketList.h"
#include "SIPTransportManager.h"

#define new PNEW

using namespace SIPTransports;

SIPUDPSocketList::SIPUDPSocketList(
  SIPTransportManager & mgr
) : m_TransportManager( mgr )
{
  m_DefaultSocket = NULL;
  m_SocketList.DisallowDeleteObjects();
  m_SelectList.DisallowDeleteObjects();
}

SIPUDPSocketList::~SIPUDPSocketList()
{
  m_SelectList.AllowDeleteObjects();
}

BOOL SIPUDPSocketList::Listen(
  WORD port,
  BOOL append
)
{
  if( append && m_SocketList.GetSize() == 0 )
    return FALSE;

  if( !append )
    m_SocketList.RemoveAll();

  /// listen on all interfaces
  PIPSocket::InterfaceTable ifTable;
  if( !PIPSocket::GetInterfaceTable( ifTable ) )
  {
    PTRACE( 1, "Unable to retrieve interface table" );
    return FALSE;
  }

  /*
  PIPSocket::RouteTable routeTable;
  if (!PIPSocket::GetRouteTable(routeTable))
  {
    PTRACE( 1, "Unable to retrieve route table" );
    return FALSE;
}*/

  BOOL hasListener = FALSE;

  PTRACE( 2, "Found " << ifTable.GetSize() << " interfaces" );

  PINDEX i = 0;

  for( i = 0; i < ifTable.GetSize(); i++ )
  {

    PTRACE( 2, "Found Interface " << ifTable[i].GetName() << " addr=" << ifTable[i].GetAddress() );

    PIPSocket::Address ifaceAddr = ifTable[i].GetAddress();

    if( Listen( FALSE, ifaceAddr, port, append ) )
       hasListener = TRUE;
  }

  
  if( append )
  {
    ///  just return here for we already have a 
    ///  default socket set be previous call to Listen()
    return hasListener; 
  }

  
  /// get the first public and set it as the default socket
  if( hasListener && m_DefaultSocket == NULL )
  {
    for( i = 0; i < m_SocketList.GetSize(); i++ )
    {
      PString key = m_SocketList.GetKeyAt(i);
      PUDPSocket * sock = m_SocketList.GetAt(key);
      PIPSocket::Address addr;
      sock->GetLocalAddress( addr );
      if( !addr.IsLoopback() && !SIPTransport::IsPrivateNetwork( addr ) )
        m_DefaultSocket = sock;
    }
  }

  /// still we dont have a default socket, lets use the first none loopback socket
  if( hasListener && m_DefaultSocket == NULL )
  {
    for( i = 0; i < m_SocketList.GetSize(); i++ )
    {
      PString key = m_SocketList.GetKeyAt(i);
      PUDPSocket * sock = m_SocketList.GetAt(key);
      PIPSocket::Address addr;
      sock->GetLocalAddress( addr );
      if( !addr.IsLoopback() && SIPTransport::IsPrivateNetwork( addr ) )
        m_DefaultSocket = sock;
    }
  }

  return hasListener;
}

BOOL SIPUDPSocketList::Listen(
  BOOL isOnlyListener,
  const PIPSocket::Address & addr,
  WORD port,
  BOOL append
)
{

  if( addr == 0 )
    return TRUE;

  if( port == 0 )
    port = 5060;

  PCaselessString key = addr.AsString() + ":" + PString( port );

  if( m_SocketList.Contains( key ) && !append ) // just return TRUE... we already have a listener in this address
  {
    PTRACE( 2, "Skipping Listener at " << key );
    return TRUE;
  }

  PTRACE( 2, "Opening Listener at " << key );
  
  PUDPSocket * sock = NULL;
  BOOL ok = FALSE;

  NAT::SIPSTUNClient * stun = m_TransportManager.GetSTUN();
  if( stun != NULL )
  {
    ok = stun->CreateSocket( sock, addr, port );
  }

  if( !ok )
  {
    sock = new PUDPSocket( port );
    SIPTransport::SetMinBufferSize( *sock, SO_RCVBUF, SIP_UDP_DEFAULT_BUFFER_SIZE );
    //SIPTransport::SetMinBufferSize( *sock, SO_SNDBUF, SIP_UDP_DEFAULT_BUFFER_SIZE );
    ok = sock->Listen( addr );
  }

  if( !ok )
  {
    PTRACE( 1, "*** LISTENER FAILED *** " << addr.AsString() << ":" << port << " error=" << sock->GetErrorText() );
    delete sock;
  }else
  {

    m_SocketList.SetAt( key, sock );
    m_SelectList.Append( sock );

    PStringStream strm;
    if( !append )
    {
      PString def;

      PString defKey = SIPTransportManager::GetDefaultInterfaceAddress().AsString() + ":" + PString( port );

      if( key == defKey || isOnlyListener )
      {
        PAssert( m_DefaultSocket == NULL, PLogicError );
        def = " [*** DEFAULT LISTENER ***]";
        m_DefaultSocket = sock;
      }

      strm <<  "*** LISTENER STARTED *** " << addr.AsString() << ":" << port << def;
      
    }else
    {
      strm << "*** LISTENER STARTED *** " << addr.AsString() << ":" << port;
    }

    PTRACE( 1, strm );

    if( SIPTransport::m_HasReadNotifier )
    {
      strm << "\r\n";
      SIPTransport::m_ReadNotifier( strm, 0 );
    }
    
  }

  return ok;
}

BOOL SIPUDPSocketList::ReadFrom(
  BYTE * bytes,
  int len,
  PIPSocket::Address & addr,
  WORD & port,
  PIPSocket::Address & localAddress,
  WORD & localPort,
  PINDEX & bytesRead
)
{
  PSocket::SelectList read;
  for( PINDEX i = 0; i < m_SelectList.GetSize(); i++ )
    read.Append( m_SelectList.GetAt(i) );

  PChannel::Errors error = PSocket::Select( read );

  if (error != PChannel::NoError )
    return FALSE;

  if( read.GetSize() == 0)
  {
    return FALSE;  
  }

  PUDPSocket & socket = (PUDPSocket &)read[0];

  PBYTEArray pdu;

  //PWaitAndSignal lock( m_ReadWriteMutex );

  BOOL ok = socket.ReadFrom( bytes, len, addr, port );

  socket.GetLocalAddress( localAddress );
  localPort = socket.GetPort();

  if( ok )
    bytesRead = socket.GetLastReadCount();
  else
    bytesRead = 0;
  
  return ok;
}

BOOL SIPUDPSocketList::WriteString(
  const PString & packet,
  const PIPSocket::Address & sendAddress,
  WORD sendPort,
  const PString & iface,
  WORD localPort
)
{
  if( packet.IsEmpty() || m_SocketList.GetSize() == 0 )
    return FALSE;

  PUDPSocket * sock = NULL;
  
  if( !iface.IsEmpty() )
    sock = m_SocketList.GetAt( iface );
  
  if( sock == NULL )
    sock = GetSocket( sendAddress, localPort );

  //if( sock == NULL )
  //  return FALSE;
 
  if( SIPTransport::IsLocalAddress( sendAddress ) && sock->GetPort() == sendPort )
  {
    PTRACE( 1, "UDP Transport LOOP Detected!!!" );
    return FALSE;
  }

  //PWaitAndSignal lock( m_ReadWriteMutex );

  if( sock != NULL )
  {
    sock->SetSendAddress( sendAddress, sendPort );
    return sock->WriteString( packet );
  }else
  {
    BOOL oneWentThrough = FALSE;
    for( PINDEX i = 0; i < m_SocketList.GetSize(); i++ )
    {
      PString key = m_SocketList.GetKeyAt(i);
      sock = m_SocketList.GetAt(key);
      sock->SetSendAddress( sendAddress, sendPort );

      if( sock->WriteString( packet ) )
        oneWentThrough = TRUE;
    }

    return oneWentThrough;
  }

  return FALSE;

}

BOOL SIPUDPSocketList::Write(
  PBYTEArray & packet,
  const PIPSocket::Address & sendAddress,
  WORD sendPort,
  const PString & iface,
  WORD localPort
)
{
  if( packet.GetSize() == 0 || m_SocketList.GetSize() == 0 )
    return FALSE;

  PUDPSocket * sock = NULL;
  
  if( localPort == 0 )
    localPort = m_DefaultSocket->GetPort();

  PString key = iface + ":" + PString( localPort );

  if( !iface.IsEmpty() )
    sock = m_SocketList.GetAt( key );
  
  if( sock == NULL )
    sock = GetSocket( sendAddress, localPort );
 

  if( SIPTransport::IsLocalAddress( sendAddress ) && sock->GetPort() == sendPort )
  {
    PTRACE( 1, "UDP Transport LOOP Detected!!!" );
    return FALSE;
  }

  if( sock != NULL )
  {
    sock->SetSendAddress( sendAddress, sendPort );
    return sock->Write( packet.GetPointer(), packet.GetSize() );
  }else
  {
    BOOL oneWentThrough = FALSE;
    for( PINDEX i = 0; i < m_SocketList.GetSize(); i++ )
    {
      PString key = m_SocketList.GetKeyAt(i);
      sock = m_SocketList.GetAt(key);
      sock->SetSendAddress( sendAddress, sendPort );

      if( sock->Write( packet.GetPointer(), packet.GetSize() ) )
        oneWentThrough = TRUE;
    }

    return oneWentThrough;
  }

  return FALSE;
}

/// Helpers

PUDPSocket * SIPUDPSocketList::GetSocket(
  const PIPSocket::Address & sendAddress,
  WORD localPort
)
{
  if( m_SocketList.GetSize() == 0 )
    return NULL;

  

  PIPSocket::Address ifaceAddress = 0;
  if( !sendAddress.IsLoopback() )
  {
    PStringArray ifaceMask;
    for( PINDEX i = 0; i < m_SocketList.GetSize(); i++ )
    {
      PIPSocket::Address ifAddress;
      m_SocketList.GetDataAt(i).GetLocalAddress( ifAddress );
      ifaceMask.AppendString( ifAddress.AsString() );
    }

    ifaceAddress = 
      SIPTransportManager::GetDefaultInterfaceAddress( sendAddress, FALSE, &ifaceMask );
    
    if( !ifaceAddress.IsValid() || ifaceAddress.IsLoopback() )
      ifaceAddress = SIPTransportManager::GetDefaultInterfaceAddress( FALSE, &ifaceMask );
    
  }else
  {
    ifaceAddress = PIPSocket::Address( "127.0.0.1" );;
  }

  if( localPort == 0 )
    localPort = m_DefaultSocket->GetPort();

  PCaselessString key = ifaceAddress.AsString() + ":" + PString( localPort );
  PUDPSocket * sock = m_SocketList.GetAt( key );
  
/// use the default socket
  if( sock == NULL )
    sock = m_DefaultSocket;

/// still NULL then just return the first socket in the list
/// wasn't able to determine default socket???
  if( sock == NULL )
    sock = &m_SocketList[0];

  sock->GetLocalAddress( ifaceAddress );

  PTRACE( 4, "Using Iface: " << ifaceAddress << " to send to Dest: " << sendAddress );
  

  return sock;
}

BOOL SIPUDPSocketList::GetListenerAddress(
  const PIPSocket::Address & sendAddress,
  PIPSocket::Address & ifaceAddress,
  WORD & listenerPort
)
{
  if( m_SocketList.GetSize() == 0 )
    return FALSE;
  else if( m_SocketList.GetSize() == 1 )
  {
    listenerPort = m_DefaultSocket->GetPort();
    return m_DefaultSocket->GetLocalAddress( ifaceAddress );
  }

  if( !sendAddress.IsLoopback() )
  {

    ifaceAddress = 
      SIPTransportManager::GetDefaultInterfaceAddress( sendAddress );
    
    if( !ifaceAddress.IsValid() || ifaceAddress.IsLoopback() )
      return FALSE;
    
  }else
  {
    ifaceAddress = PIPSocket::Address( "127.0.0.1" );;
  }

  PCaselessString key = ifaceAddress.AsString() + ":" + PString( m_DefaultSocket->GetPort() );
  PUDPSocket * sock = m_SocketList.GetAt( key );

  if( sock == NULL )
    sock = m_DefaultSocket;

  listenerPort = sock->GetPort();
  return sock->GetLocalAddress( ifaceAddress );
}


PINDEX SIPUDPSocketList::GetSize()
{
  return m_SocketList.GetSize();
}

BOOL SIPUDPSocketList::Close()
{
  for( PINDEX i = 0; i < m_SocketList.GetSize(); i++ )
  {
    m_SocketList.GetDataAt( i ).Close();
  }

  return TRUE;
}





