/*
 *
 * SIPTransportManager.cxx
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
 * $Log: SIPTransportManager.cxx,v $
 * Revision 1.23  2007/01/10 09:39:15  joegenbaclor
 * minor bug fix related to last commit
 *
 * Revision 1.22  2007/01/10 09:03:56  joegenbaclor
 * Modified GetDefaultInterfaceAddress() to return the mask entry if size is one
 *
 * Revision 1.21  2007/01/06 07:30:33  joegenbaclor
 * Added mask to filter GetDefaultInterfaceAddress
 *
 * Revision 1.20  2006/12/22 07:37:21  joegenbaclor
 * Introduced separate funtion call  for starting of transport thread.  This is necessary
 *  to make sure that the backdoor listener is already initialized prior to calling transport
 *  reads
 *
 * Revision 1.19  2006/12/06 06:40:56  joegenbaclor
 * STUN related fixes
 *
 * Revision 1.18  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.17  2006/11/21 06:55:19  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.16  2006/11/16 01:35:59  rcolobong
 * Fixed bug in GetDefaultInterfaceAddress during failure to retrieve route table for a private network
 *
 * Revision 1.15  2006/11/09 12:48:19  joegenbaclor
 * Fixed bug in GetDefaultInterfaceAddress() caused by none retrieval of the route table
 *
 * Revision 1.14  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.13  2006/04/20 14:32:28  joegenbaclor
 * Fixed early media bug where soucd channels in OPAL ay not be opened in time for ringback to start streaming.
 *
 * Revision 1.12  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.11  2006/03/21 14:11:51  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.10  2006/03/21 09:03:57  joegenbaclor
 * Introduced Terminate() method in SIPTransportManager to allow
 * SIPStack to terminate Transport threads prior to the deletion of the manager
 *
 * Revision 1.9  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.8  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPTransportManager.h"
#include "SIPUDPTransport.h"

using namespace SIPTransports;
using namespace NAT;

#define new PNEW

SIPTransportManager::SIPTransportManager(
  PNotifier eventNotifier
)
{
  m_EventNotifier = eventNotifier;
  m_STUNClient = NULL;
  m_UDPTransport = NULL;
  m_TCPTransport = NULL;
  m_TLSTransport = NULL;
  m_IsTerminated = FALSE;
  SIPTransport::Initialize();
}

SIPTransportManager::~SIPTransportManager()
{
  Terminate();
}

BOOL SIPTransportManager::Terminate()
{
  if( m_IsTerminated )
    return TRUE;

  m_IsTerminated = TRUE;

  if( m_UDPTransport != NULL )
  {
    m_UDPTransport->CleanUp();
    delete m_UDPTransport;
  }

  if( m_TCPTransport != NULL )
  {
    m_TCPTransport->CleanUp();
    delete m_TCPTransport;
  }

  if( m_TLSTransport != NULL )
  {
    m_TLSTransport->CleanUp();
    delete m_TLSTransport;
  }

  if( m_STUNClient != NULL )
    delete m_STUNClient;

  SIPTransport::Deinitialize();

  return TRUE;
}

BOOL SIPTransportManager::AppendTransport(
  SIPTransport::Protocol protocol,
  const PString & iface,
  WORD port
)
{
  return AddTransport( protocol, iface, port, TRUE );
}

BOOL SIPTransportManager::AppendTransport(
  SIPTransport::Protocol protocol,
  const PIPSocket::Address & iface,
  WORD port
)
{
  return AddTransport( protocol, iface, port, TRUE );
}

BOOL SIPTransportManager::AddTransport(
  SIPTransport::Protocol protocol,
  const PIPSocket::Address & _iface,
  WORD port,
  BOOL append
)
{
  PString iface = "*";
  if( _iface != 0 )
  {
    iface = _iface.AsString();
  }

  BOOL ret =  AddTransport( protocol, iface, port, append );

  return ret;
}

BOOL SIPTransportManager::AddTransport(
  SIPTransport::Protocol protocol,
  const PString & iface,
  WORD port,
  BOOL append
)
{
  SIPTransport * transport = NULL;

  if( !append )
  {
    transport = OnCreateTransport( protocol );
    if( transport == NULL )
    {
      LOG_IF_DEBUG( LogError(), "*** TRANSPORT ERROR *** " << "Unable to create transport object on " << iface << ":" << port );
      return FALSE;
    }

    if( protocol == SIPTransport::UDP && m_UDPTransport == NULL )
    {
      LOG_IF_DEBUG( LogDetail(), "Starting UDP Transport on " << iface << ":" << port );
      m_UDPTransport = transport;
    }else if( protocol == SIPTransport::TCP && m_TCPTransport == NULL )
    {
       LOG_IF_DEBUG( LogDetail(), "Starting TCP Transport on " << iface << ":" << port );
      m_TCPTransport = transport;
    }else if( protocol == SIPTransport::TLS && m_TLSTransport == NULL )
    {
       LOG_IF_DEBUG( LogDetail(), "Starting TLS Transport on " << iface << ":" << port );
      m_TLSTransport = transport;
    }else
    {
      LOG_IF_DEBUG( LogError(), "*** TRANSPORT ERROR *** " << "Starting Transport on " << iface << ":" << port );
      transport->CleanUp();
      delete transport;
      return FALSE;
    }

    
  }else
  {
    if( protocol == SIPTransport::UDP && m_UDPTransport != NULL )
    {
      transport = m_UDPTransport;
      LOG_IF_DEBUG( LogDetail(), "Starting UDP Transport on " << iface << ":" << port );

    }else if( protocol == SIPTransport::TCP && m_TCPTransport != NULL )
    {
      transport = m_TCPTransport;
      LOG_IF_DEBUG( LogDetail(), "Starting TCP Transport on " << iface << ":" << port );
    }else if( protocol == SIPTransport::TLS && m_TLSTransport != NULL )
    {
      transport = m_TLSTransport;
      LOG_IF_DEBUG( LogDetail(), "Starting TLS Transport on " << iface << ":" << port );
    }else
    {
      return FALSE;
    }
  }

  BOOL ok = transport->Listen( iface, port, append );

  if( ok )
  {
    LOG_IF_DEBUG( LogDetail(), "*** TRANSPORT STARTED *** " << " Listener: " << iface << ":" << port );
  }else
  {
    LOG_IF_DEBUG( LogError(), "*** TRANSPORT FAILED *** " << " Listener: " << iface << ":" << port );
  }

  return ok;
}

BOOL SIPTransportManager::HasListener()const
{
  /// we only support UDP for now.
  if( m_UDPTransport == NULL )
    return FALSE;

  return m_UDPTransport->IsListening();
}

SIPTransport * SIPTransportManager::OnCreateTransport(
  SIPTransport::Protocol protocol
)
{
  if( protocol == SIPTransport::UDP )
    return new SIPUDPTransport( this );

  return NULL;
}



/////////////////////
BOOL SIPTransportManager::EnqueueEvent(
  SIPTransportEvent * event
)
{
  //PAssertNULL( event );
  SIPTransport * transport = NULL;
  SIPMessage * msg = (SIPMessage*)event->GetEvent();

  if( event->m_Type == SIPTransportEvent::UDPPacketSend && m_UDPTransport != NULL )
  {
    transport = m_UDPTransport;
  }else if( event->m_Type ==  SIPTransportEvent::TCPPacketSend && m_TCPTransport != NULL )
  {
    transport = m_TCPTransport;
  }else if( event->m_Type ==  SIPTransportEvent::TLSPacketSend && m_TLSTransport != NULL )
  {
    transport = m_TLSTransport;
  }else if( event->m_Type == SIPTransportEvent::PacketSend )
  {
    if( !msg->IsKeepAlive() )
    {
      SIPSrvRecord srvRec;
      srvRec = *msg;

      SIPTransport::Protocol serverProto = DetermineServerProtocol( event );

      if( srvRec.GetSize( "UDP" ) > 0 && serverProto == SIPTransport::UDP )
        transport = m_UDPTransport;
      else if( srvRec.GetSize( "TCP" ) > 0 && serverProto == SIPTransport::TCP )
        transport = m_TCPTransport;
      else if( srvRec.GetSize( "TLS" ) > 0 && serverProto == SIPTransport::TLS )
        transport = m_TLSTransport;
      else
        transport = m_UDPTransport;
    }else
    {
      transport = m_UDPTransport;
    }
  }else
  {
    delete event;
    return FALSE;
  }

  if( transport != NULL )
    transport->Enqueue( *msg );

  delete event;

  return TRUE;
}

#ifdef _WIN32
#pragma warning(disable:4311)
#endif

void SIPTransportManager::OnTransportEvent(
  SIPTransportEvent * event
)
{
  m_EventNotifier( *this, (INT)event );
}

SIPTransport::Protocol SIPTransportManager::DetermineServerProtocol(
  SIPTransportEvent * event
)
{
  SIPMessage * msg = (SIPMessage *)event->GetEvent();
  PString transport;

  if( msg->IsRequest() )
  {
    /// check the URI protocol
    StartLine startLine;
    msg->GetStartLine( startLine );
    SIPURI & requestURI = startLine.GetRequestLine()->GetRequestURI();
    requestURI.GetParameter( "transport", transport );
  }else
  {
    /// check the via protocol
    if( msg->GetViaSize() > 0 )
    {
      Via via;
      msg->GetViaAt( 0, via );
      transport = via.GetProtocol();
    }
  }

  if( transport *= "udp" )
    return SIPTransport::UDP;
  else if( transport *= "tcp" )
    return SIPTransport::TCP;
  else if( transport *= "tls" )
    return SIPTransport::TLS;
  else
    return SIPTransport::Unknown;
}




PIPSocket::Address SIPTransportManager::GetDefaultInterfaceAddress( 
  BOOL includeLoopBack,
  PStringArray * mask 
)
{
  PIPSocket::Address gateway;
  if( !PIPSocket::GetGatewayAddress( gateway ) )
  {
    PTRACE( 1, "Unable to retrieve gateway address" );
    return 0;
  }

  return GetDefaultInterfaceAddress( gateway, includeLoopBack, mask );
}

static BOOL IsAddressInMask( PIPSocket::Address & addr, PStringArray & mask )
{
  PString address = addr.AsString();
  for( PINDEX i = 0; i < mask.GetSize(); i++ )
    if( address == mask[i] )
      return TRUE;

  return FALSE;
}



PIPSocket::Address SIPTransportManager::GetDefaultInterfaceAddress(
  const PIPSocket::Address & destination,
  BOOL includeLoopBack,
  PStringArray * mask 
)
{
  if( destination.IsLoopback() )
    return destination;

  if( mask != NULL  && mask->GetSize() == 1 )
  {
    return PIPSocket::Address( (*mask)[0] );
  }

  PIPSocket::InterfaceTable ifTable;
  if( !PIPSocket::GetInterfaceTable( ifTable ) )
  {
    PTRACE( 1, "Unable to retrieve interface table" );
    return 0;
  }
  PIPSocket::RouteTable routeTable;
  if (!PIPSocket::GetRouteTable(routeTable) )
  {
    PTRACE( 1, "Unable to retrieve route table" );

    ///lets use the interface table instead and guess the best interface
    PINDEX i = 0;
    /// get the first public interface and set it as the default address
    if( !SIPTransport::IsPrivateNetwork( destination ) )
    {
      for( i = 0; i < ifTable.GetSize(); i++ )
      {
        PIPSocket::Address addr = ifTable[i].GetAddress();
        if( !addr.IsLoopback() && !SIPTransport::IsPrivateNetwork( addr ) )
        {
          if( mask != NULL && IsAddressInMask( addr, *mask ) )
            return addr;
        }
      }

      /// mask comparison failed.  we are desperate just use the first interface
      for( i = 0; i < ifTable.GetSize(); i++ )
      {
        PIPSocket::Address addr = ifTable[i].GetAddress();
        if( !addr.IsLoopback() && !SIPTransport::IsPrivateNetwork( addr ) )
          return addr;
      }
    }else
    {
      /// still we dont have a default address, lets use the first none loopback address
      for( i = 0; i < ifTable.GetSize(); i++ )
      {
        PIPSocket::Address addr = ifTable[i].GetAddress();
        if( !addr.IsLoopback() && SIPTransport::IsPrivateNetwork( addr ) )
          if( mask != NULL && IsAddressInMask( addr, *mask ) )
            return addr;
      }

      /// mask comparison failed.  we are desperate just use the first private interface
      for( i = 0; i < ifTable.GetSize(); i++ )
      {
        PIPSocket::Address addr = ifTable[i].GetAddress();
        if( !addr.IsLoopback() && SIPTransport::IsPrivateNetwork( addr ) )
          return addr;
      }

      /// no private interface available, just use the first public interface
      for( i = 0; i < ifTable.GetSize(); i++ )
      {
        PIPSocket::Address addr = ifTable[i].GetAddress();
        if( !addr.IsLoopback() && !SIPTransport::IsPrivateNetwork( addr ) )
          return addr;
      }
    }

    return 0;
  }

  // Ok we have a route table
  PIPSocket::Address localAddress;

  for( PINDEX i = 0; i < routeTable.GetSize(); i++ )
  {
    PIPSocket::RouteEntry & entry = routeTable[i];

    if( entry.GetNetMask() == INADDR_ANY  )
      continue;

    
    if( routeTable[i].GetDestination().IsLoopback() && !includeLoopBack )
      continue;

    PIPSocket::Address netMask = entry.GetNetMask();
    if( entry.GetDestination() == destination ||
      ((destination & netMask) == entry.GetNetwork() ) )
    {
      localAddress = routeTable[i].GetDestination();


      if( localAddress == 0 )
      {
        for( PINDEX j = 0; j < ifTable.GetSize(); j++ )
        {
          if( ifTable[j].GetName() == entry.GetInterface() )
          {
            localAddress = ifTable[j].GetAddress();
          }
        }
      }

      if( localAddress.IsLoopback() && !includeLoopBack )
        continue;

      if( mask != NULL && !IsAddressInMask( localAddress, *mask ) )
        continue;

      return localAddress;
    }
  }

  if( localAddress.IsLoopback() && !includeLoopBack )
    localAddress = GetDefaultInterfaceAddress( FALSE );

  if( mask != NULL && !IsAddressInMask( localAddress, *mask ) )
    return 0;

  return localAddress;
}


SIPSTUNClient * SIPTransportManager::GetSTUN(const PIPSocket::Address & ip) const
{
  if (ip.IsValid() && SIPTransport::IsLocalAddress(ip))
    return NULL;

  return m_STUNClient;
}


SIPSTUNClient::NatTypes SIPTransportManager::SetSTUNServer(
  const PString & server,
  WORD udpPortBase,
  WORD udpPortMax,
  WORD rtpPortBase,
  WORD rtpPortMax
)
{
  if( m_STUNClient != NULL )
    delete m_STUNClient;

  if (server.IsEmpty()) {
    m_STUNClient = NULL;
    return SIPSTUNClient::UnknownNat;
  }

    m_STUNClient = new SIPSTUNClient(server,
                           udpPortBase, udpPortMax,
                           rtpPortBase, rtpPortMax );
  SIPSTUNClient::NatTypes type = m_STUNClient->GetNatType();
  if (type != SIPSTUNClient::BlockedNat && type != SIPSTUNClient::OpenNat)
  {
    m_STUNClient->GetExternalAddress(m_TranslationAddress);
  }else
  {
    delete m_STUNClient;
    m_STUNClient = NULL;
  }

  LOG_IF_DEBUG( LogInfo(), "STUN server \"" << server << "\" replies " << type << ", external IP " << m_TranslationAddress);

  
  if( SIPTransport::m_HasReadNotifier )
  {
    PStringStream strm;
    strm << "STUN server \"" << server << "\" replies " << type << ", external IP " << m_TranslationAddress << "\r\n";
    SIPTransport::m_ReadNotifier( strm, 0 );
  };

  return type;
}

BOOL SIPTransportManager::TranslateIPAddress(
  PIPSocket::Address & localAddress,
  const PIPSocket::Address & remoteAddress
 )
{
  if (m_TranslationAddress.IsLoopback())
    return FALSE; // Have nothing to translate it to

  if (!m_TranslationAddress.IsValid())
    return FALSE; // Have nothing to translate it to

  if (!SIPTransport::IsLocalAddress(localAddress))
    return FALSE; // Is already translated

  if (SIPTransport::IsLocalAddress(remoteAddress))
    return FALSE; // Does not need to be translated

  // Tranlsate it!
  localAddress = m_TranslationAddress;
  return TRUE;
}

BOOL SIPTransportManager::GetListenerAddress(
  SIPTransport::Protocol proto,
  const PIPSocket::Address & target,
  PIPSocket::Address & listenerAddress,
  WORD & listenerPort
)
{
  BOOL ok = FALSE;

  if( proto == SIPTransport::UDP && m_UDPTransport != NULL )
  {
    ok = m_UDPTransport->GetListenerAddress( target, listenerAddress, listenerPort );
  }else if( proto == SIPTransport::TCP && m_TCPTransport != NULL )
  {
    ok = FALSE;
  }else if( proto == SIPTransport::TLS && m_TLSTransport != NULL )
  {
    ok = FALSE;
  }

  TranslateIPAddress( listenerAddress, listenerPort );

  return ok;
}

BOOL SIPTransportManager::IsLocalAddressAndPort(
  const SIPURI & uri
)const
{
  PIPSocket::Address addr;
  WORD port;
  if( !SIPTransport::Resolve( uri, addr, port ) )
    return FALSE;

  return IsLocalAddressAndPort( addr, port );
}

BOOL SIPTransportManager::IsLocalAddressAndPort(
  const PIPSocket::Address & addr,
  WORD port
)const
{
  if( port == 0 )
    port = 5060;

  return PIPSocket::IsLocalHost( addr ) && port == m_UDPTransport->GetListenerPort();
}

BOOL SIPTransportManager::StartThreads()
{

  if( m_UDPTransport != NULL )
    if( !m_UDPTransport->StartThreads() )
      return FALSE;

  if( m_TCPTransport != NULL )
    if( !m_TCPTransport->StartThreads() )
      return FALSE;

  if( m_TLSTransport != NULL )
    if( !m_TLSTransport->StartThreads() )
      return FALSE;

  return TRUE;
}



