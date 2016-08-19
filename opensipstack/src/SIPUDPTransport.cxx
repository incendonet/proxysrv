/*
 *
 * SIPUDPTransport.cxx
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
 * $Log: SIPUDPTransport.cxx,v $
 * Revision 1.28  2006/12/22 07:37:21  joegenbaclor
 * Introduced separate funtion call  for starting of transport thread.  This is necessary
 *  to make sure that the backdoor listener is already initialized prior to calling transport
 *  reads
 *
 * Revision 1.27  2006/12/20 16:24:46  joegenbaclor
 * Implemented keep-alives for upper reg.
 *
 * Revision 1.26  2006/12/06 06:40:56  joegenbaclor
 * STUN related fixes
 *
 * Revision 1.25  2006/12/04 08:55:32  joegenbaclor
 * Added log hook to transport
 *
 * Revision 1.24  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.23  2006/11/21 06:55:19  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.22  2006/11/17 03:35:53  rcolobong
 * Add PTrace during WritePacket and ReadPacket
 *
 * Revision 1.21  2006/11/10 08:10:51  rcolobong
 * Use ParserTools GetDefaultHashKey1 and GetDefaultHashKey2 in XORHash rather than using the MACRO GS_HASH_KEY_1/GS_HASH_KEY_2
 *
 * Revision 1.20  2006/11/08 06:46:22  joegenbaclor
 * Added session level NIC/Interface binding
 *
 * Revision 1.19  2006/11/07 12:43:23  joegenbaclor
 * Corrected bug in GetListenerAddress() when binding to a single interface
 *
 * Revision 1.18  2006/11/07 09:30:46  joegenbaclor
 * Added new internal header "IFACE-ADDR" to allow transctions to remember the interface address for sending future responses within the transaction lifespan
 *
 * Revision 1.17  2006/09/06 04:14:36  joegenbaclor
 * Added check on packet size for SIP UDP Transport Reads
 *
 * Revision 1.16  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.15  2006/07/17 11:36:45  joegenbaclor
 * 1.  More routing enhancements to B2BUA
 *
 * Revision 1.14  2006/07/13 06:53:05  joegenbaclor
 * 1.  Introduced Sanity checks for incoming SIP Message
 * 2.  Corrected bug in SIPURI where enum scheme is not getting recognized.
 * 3.  Added ENUM support to routing
 * 4.  Introduced global routing flag to indicate when and not to rewrite outbound To URI's
 *
 * Revision 1.13  2006/07/10 06:29:38  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.12  2006/07/06 05:38:12  joegenbaclor
 * Final last minute changes to CBCOM hash support
 *
 * Revision 1.11  2006/07/04 00:38:41  joegenbaclor
 * Added Grandstream CBCom support for RTP Sessions
 *
 * Revision 1.10  2006/07/03 15:10:05  joegenbaclor
 * Added Grandstream CBCom support for basic XOR hash
 *
 * Revision 1.9  2006/06/28 14:15:00  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.8  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.7  2006/03/21 14:11:51  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.6  2006/03/14 03:53:54  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPUDPTransport.h"


using namespace SIPTransports;

#define new PNEW

SIPUDPTransport::SIPUDPTransport(
  SIPTransportManager * manager
) 
: SIPTransport( manager, UDP ),
  m_TransportSocket( *manager )
{
}

SIPUDPTransport::~SIPUDPTransport()
{
}

BOOL SIPUDPTransport::Listen(
  const PString & iface,
  WORD port,
  BOOL append
)
{
  PIPSocket::Address addr = iface;

  BOOL ok  = FALSE;

  if( addr.IsValid() )
    ok = m_TransportSocket.Listen( TRUE, addr, port, append );
  else
    ok = m_TransportSocket.Listen( port, append );

  if( ok && !append )
  {
    PAssert( m_ReadThread == NULL && m_WriteThread == NULL, PLogicError );
    m_InterfaceAddress = addr.AsString();
    m_ListenerPort = port;
  }

  m_IsListening = ok;

  return m_IsListening;
}


BOOL SIPUDPTransport::ReadPacket( 
  SIPMessage *& packet,
  PIPSocket::Address & addr,
  WORD & port,
  PIPSocket::Address & localAddress,
  WORD & localPort,
  int & len
)
{

  PBYTEArray pdu;
  PString buff;
  PINDEX bytesRead = 0;

  if (m_TransportSocket.ReadFrom(
    pdu.GetPointer(MAX_SIP_MESSAGE_LENGTH), 
    MAX_SIP_MESSAGE_LENGTH, 
    addr, 
    port,
    localAddress,
    localPort,
    bytesRead )
  )
  {
    len = bytesRead;
    if( !bytesRead )
      return FALSE;

    pdu.SetSize( bytesRead );
#if 1   //ENABLE_GRANSTREAM_XOR

    
    PBYTEArray temp(bytesRead);
    ::memcpy( temp.GetPointer(), pdu.GetPointer(), pdu.GetSize() );
    temp.SetSize( pdu.GetSize() + 1 );
    temp[temp.GetSize()]='\0';
    buff = PString( (char*)temp.GetPointer() );
    buff = ParserTools::LineFeedSanityCheck( buff );
    *packet = buff;

    if( packet->GetCSeqMethod().IsEmpty() )
    {
      PBYTEArray in = pdu;
      PBYTEArray out;
      LOG_IF_DEBUG( LogDetail(), in );
      
      ParserTools::XORHash( in, out, 
        ParserTools::GetDefaultXORHashKey1(), 
        ParserTools::GetDefaultXORHashKey2() );

      LOG_IF_DEBUG( LogDebugVeryHigh(), out );
      out.SetSize( out.GetSize() + 1); 
      out[out.GetSize()]='\0';
      buff = PString( (char*)out.GetPointer() );
      buff = ParserTools::LineFeedSanityCheck( buff );
      *packet = buff;
      pdu = out;

      if( !packet->GetCSeqMethod().IsEmpty() )
      {
        
        packet->SetXORHash( TRUE );
      }
    
    }
#else
    pdu.SetSize( pdu.GetSize() + 1 );
    pdu[pdu.GetSize()]='\0';
    buff = PString( (char*)pdu.GetPointer() );
    buff = ParserTools::LineFeedSanityCheck( buff );
    *packet = buff;
#endif

    if( !packet->IsValid() )
    {
      len = 0;
      return FALSE;
    }
    
    /// Set Receive Address custom Headers for NAT traversal support
    PString addrStr = addr.AsString();
    PString portStr( port );
    SIPHeader rcvAddr( "RCVADDR", addrStr );
    SIPHeader rcvPort( "RCVPORT", portStr );
    SIPHeader rcvTran( "RCVTRAN", "udp" );
    
    packet->AddInternalHeader( rcvAddr );
    packet->AddInternalHeader( rcvPort );
    packet->AddInternalHeader( rcvTran );

    LOG_IF_DEBUG( LogDebug(), "RCV: XOR=" << packet->IsXORHashSet() 
      << " " << len << " Bytes from " << rcvAddr << ":" 
      << rcvPort << ":UDP (" << packet->GetStartLine() << ")" );
    
    PStringStream traceStream;
    traceStream << "RCV: XOR=" << packet->IsXORHashSet() << " " 
      << len << " Bytes from " << rcvAddr << ":" << rcvPort 
      << ":UDP (" << packet->GetStartLine() << ")" 
      << "\r\n" << *packet;

    PTRACE( 5, traceStream );

    if( SIPTransport::m_HasReadNotifier )
    {
      SIPTransport::m_ReadNotifier( traceStream, 0 );
    }

    return TRUE;
  }

  return FALSE;
}

BOOL SIPUDPTransport::WritePacket( 
  SIPMessage & packet,
  const PIPSocket::Address & address,
  WORD port,
  const PString & iface
)
{
  PString ifaceAddress = iface;
  PIPSocket::Address sendAddr = address;
  WORD sendPort = port;

  if( sendAddr == 0 )
  {
    SIPSrvRecord srvRec;
    srvRec = packet;
    
    if( srvRec.GetSize( "UDP" ) == 0)
      return FALSE;

    const SIPSrvRecord::HostRecord * rec = srvRec.GetRecordAt( 0, "UDP" );
    sendAddr = rec->m_Address;
    sendPort = rec->m_Port;
  }

  if( packet.IsRequest() )
  {
    Via via = packet.PopTopVia();
    via.SetParameter( "uas-addr", sendAddr.AsString() );
    packet.AppendVia( via );
  }

  PString sendPacket;
  
  if( !packet.IsKeepAlive() )
    sendPacket = packet.AsString();
  else
    sendPacket = "\r\n\r\n";

  ifaceAddress = packet.GetInterfaceAddress();

  LOG_IF_DEBUG( LogDebug(), "SEND: XOR=" << packet.IsXORHashSet() 
    << " " <<  sendPacket.GetLength() << " Bytes to " 
    << sendAddr << ":" << sendPort << ":UDP (" 
    << packet.GetStartLine() << ")" 
    << " Interface Address=" << ifaceAddress );

  PStringStream traceStream;
  traceStream <<  "SEND: XOR=" << packet.IsXORHashSet() 
    << " " <<  sendPacket.GetLength() << " Bytes to " 
    << sendAddr << ":" << sendPort << ":UDP (" 
    << packet.GetStartLine() << ")" 
    << " Interface Address=" << ifaceAddress
    << "\r\n" << packet;

  PTRACE( 5, traceStream );

  if( SIPTransport::m_HasWriteNotifier )
  {
    SIPTransport::m_WriteNotifier( traceStream, 0 );
  }

#if 1
  if( packet.IsXORHashSet() )
  {
    PBYTEArray out;
    packet.GetXORHash( out );
    return m_TransportSocket.Write( out, sendAddr, sendPort, ifaceAddress );
  }
#endif

  if( packet.IsInvite() )
    cout << "";
  return m_TransportSocket.WriteString( sendPacket, sendAddr, sendPort, ifaceAddress );
}

void SIPUDPTransport::CleanUp()
{
  m_TransportSocket.Close();
  SIPTransport::CleanUp();
}

BOOL SIPUDPTransport::GetListenerAddress(
  const PIPSocket::Address & target,
  PIPSocket::Address & listenerAddress,
  WORD & listenerPort
)
{
  return m_TransportSocket.GetListenerAddress( target, listenerAddress, listenerPort );
}

PUDPSocket * SIPUDPTransport::GetSocket( 
  const PIPSocket::Address & sendAddress,
  WORD localPort
)
{
  return m_TransportSocket.GetSocket( sendAddress,localPort );
}
