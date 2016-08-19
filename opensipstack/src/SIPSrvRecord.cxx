/*
 *
 * SIPSrvRecord.cxx
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
 * $Log: SIPSrvRecord.cxx,v $
 * Revision 1.7  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.5  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.4  2006/03/21 14:11:51  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPSrvRecord.h"

using namespace SIPTransports;

#define new PNEW

SIPSrvRecord::SIPSrvRecord()
{
}

SIPSrvRecord::SIPSrvRecord(
  const PString & domain
)
{
  operator=(domain);
}

SIPSrvRecord::SIPSrvRecord(
  const SIPMessage & message
)
{
  operator=(message);
}

SIPSrvRecord::SIPSrvRecord(
  const SIPURI & uri
)
{
  operator=( uri );
}

SIPSrvRecord & SIPSrvRecord::operator=(
  const SIPURI & uri
)
{
  SIPURI sipURI = uri;

  PString domain = sipURI.GetHost();
  PString proto;
  WORD port = 5060;
  sipURI.GetParameter( "transport", proto );

  if( !sipURI.GetPort().IsEmpty() )
    port = (WORD)sipURI.GetPort().AsUnsigned();
  
  Resolve( domain, proto.IsEmpty() ? "*":proto, port );
  return *this;
}


SIPSrvRecord & SIPSrvRecord::operator=(
  const SIPMessage & packet
)
{

  BOOL hasTransport = FALSE;
  SIPHeader hSendAddr( "SENDADDR" );
  SIPURI sipURI;

  if( !packet.GetInternalHeader( "SENDADDR", hSendAddr ) )
  { /// internal header not present, determine destination using route set or startline uri
    if( packet.IsRequest() )
    {
      /// always use the startline URI for keep-alive
      if( packet.IsKeepAlive() )
      {
        StartLine startLine;
        RequestLine * requestLine;

        if( !packet.GetStartLine( startLine ) )
          return *this;

        requestLine = startLine.GetRequestLine();

        if( requestLine == NULL )
          return *this;

        operator=( requestLine->GetRequestURI() );
        hasTransport = TRUE;
      }

      if( packet.GetRouteSize() > 0 )
      {
        hasTransport = TRUE;
        Route route = packet.GetRouteAt( 0 );
        RouteURI routeURI;
        
        if( route.GetURI( routeURI ) )
          operator=(routeURI.GetURI());
      }

      if( !hasTransport )
      { /// use startline URI
        StartLine startLine;
        RequestLine * requestLine;

        if( !packet.GetStartLine( startLine ) )
          return *this;

        requestLine = startLine.GetRequestLine();

        if( requestLine == NULL )
          return *this;

        operator=( requestLine->GetRequestURI() );
       
      }
    }else
    { /// use via URI
      if( packet.GetViaSize() == 0 )
        return *this;

      Via via;
      if( packet.GetViaAt( 0, via ) )
      {
        PString domain;
        PString proto;
        WORD port;

        if( !via.IsBehindNAT() && !via.IsLoopback() )
        {
          domain = via.GetAddress();
          proto = via.GetProtocol();
          port = via.GetPort();
          
        }else
        {
          domain = via.GetReceiveAddress().AsString();
          proto = via.GetProtocol();
          port = via.GetRPort();
        }

        Resolve( domain, proto, port );
      }

    }

  }else
  {
    PString sendAddr, sendPort, sendProto;
    SIPHeader hSendPort( "SENDPORT" );
    SIPHeader hSendProto( "SENDPROTO" );
    
    packet.GetInternalHeader( "SENDPORT", hSendPort );
    packet.GetInternalHeader( "SENDPROTO", hSendProto );

    sendAddr = hSendAddr.GetHeaderBody();
    sendPort = hSendPort.GetHeaderBody();
    sendProto = hSendProto.GetHeaderBody();
    WORD port = 5060;

    if( !sendPort.IsEmpty() )
      port = (WORD)sendPort.AsUnsigned();

    Resolve( sendAddr, sendProto.IsEmpty() ? "UDP" : sendProto, port );

  }

  return *this;
}

SIPSrvRecord & SIPSrvRecord::operator=( 
  const PString & domain 
)
{
/*
According to RFC 2782, the protocol designations are to be prefixed by an underscore, so that the correct entries are: 

_sip._tcp         SRV 0 0 5060 sip-server.cs.columbia.edu.
                  SRV 1 0 5060 backup.ip-provider.net.
_sip._udp         SRV 0 0 5060 sip-server.cs.columbia.edu.
                  SRV 1 0 5060 backup.ip-provider.net.
*/
  Resolve( domain );
  return *this;
}

BOOL SIPSrvRecord::Resolve( 
  const PString & domain,
  const PString & _proto,
  WORD port
)
{
  /*
According to RFC 2782, the protocol designations are to be prefixed by an underscore, so that the correct entries are: 

_sip._tcp         SRV 0 0 5060 sip-server.cs.columbia.edu.
                  SRV 1 0 5060 backup.ip-provider.net.
_sip._udp         SRV 0 0 5060 sip-server.cs.columbia.edu.
                  SRV 1 0 5060 backup.ip-provider.net.
*/
  PString proto = _proto.ToUpper();
  m_UdpHostRecords.RemoveAll();
  m_TcpHostRecords.RemoveAll();

  PIPSocket::Address addr;
  BOOL hasDNS = FALSE;
  
  /// check if domain is an ap address;
  addr = domain;
  if( addr.IsValid() )
  {
    if( proto != "*" )
    {
      HostRecord * rec = new HostRecord();
      rec->m_Address = addr;
      rec->m_Type = HostRecord::Addr;
      rec->m_Port = port == 0 ? 5060 : port;
      rec->m_Proto = proto;
      m_UdpHostRecords.Append( rec );
      
    }else
    {
      HostRecord * recUDP = new HostRecord();
      recUDP->m_Address = addr;
      recUDP->m_Type = HostRecord::Addr;
      recUDP->m_Port = port == 0 ? 5060 : port;
      recUDP->m_Proto = "UDP";
      m_UdpHostRecords.Append( recUDP );

      HostRecord * recTCP = new HostRecord();
      recTCP->m_Address = addr;
      recTCP->m_Type = HostRecord::Addr;
      recTCP->m_Port = port == 0 ? 5060 : port;
      recTCP->m_Proto = "TCP";
      m_TcpHostRecords.Append( recTCP );
    }

    return TRUE;
  }
  
  // see if the domain is actually a host
  if( PIPSocket::GetHostAddress( domain, addr ) ) 
  {  
    if( proto != "*" )
    {
      if( proto == "UDP" )
      {
        HostRecord * rec = new HostRecord();
        rec->m_Address = addr;
        rec->m_Type = HostRecord::Addr;
        rec->m_Port = port == 0 ? 5060 : port;
        rec->m_Proto = proto;
        m_UdpHostRecords.Append( rec );
      }else
      {
        HostRecord * rec = new HostRecord();
        rec->m_Address = addr;
        rec->m_Type = HostRecord::Addr;
        rec->m_Port = port == 0 ? 5060 : port;
        rec->m_Proto = proto;
        m_TcpHostRecords.Append( rec );
      }
      return TRUE;
    }else
    {
      HostRecord * recUDP = new HostRecord();
      recUDP->m_Address = addr;
      recUDP->m_Type = HostRecord::Addr;
      recUDP->m_Port = port == 0 ? 5060 : port;
      recUDP->m_Proto = "UDP";
      m_UdpHostRecords.Append( recUDP );

      HostRecord * recTCP = new HostRecord();
      recTCP->m_Address = addr;
      recTCP->m_Type = HostRecord::Addr;
      recTCP->m_Port = port == 0 ? 5060 : port;
      recTCP->m_Proto = "TCP";
      m_TcpHostRecords.Append( recTCP );
    }
  }

  PDNS::SRVRecordList srvUdpRecords, srvTcpRecords;
  PString srvLookupStr = PString( "_sip._udp." ) + domain;
  if( proto == "*" || proto == "UDP" )
  {
    BOOL hasDNS = PDNS::GetRecords(srvLookupStr, srvUdpRecords);
    if(hasDNS ) 
    {
      PDNS::SRVRecord * recPtr = srvUdpRecords.GetFirst();
      while (recPtr != NULL) 
      {
        HostRecord * rec = new HostRecord();
        rec->m_Address = recPtr->hostAddress;
        rec->m_Port = recPtr->port;
        rec->m_Proto = "UDP";
        rec->m_Type = HostRecord::Srv;
        m_UdpHostRecords.Append( rec );
        recPtr = srvUdpRecords.GetNext();
      }
    }
  } 

  if( proto == "*" || proto == "TCP"  )
  {
    srvLookupStr = PString( "_sip._tcp." ) + domain;
    hasDNS = PDNS::GetRecords(srvLookupStr, srvTcpRecords);
    if(hasDNS) 
    {

      PDNS::SRVRecord * recPtr = srvTcpRecords.GetFirst();
      while (recPtr != NULL) 
      {
        HostRecord * rec = new HostRecord();
        rec->m_Address = recPtr->hostAddress;
        rec->m_Port = recPtr->port;
        rec->m_Proto = "TCP";
        rec->m_Type = HostRecord::Srv;
        m_TcpHostRecords.Append( rec );
        recPtr = srvTcpRecords.GetNext();
      }
    }
  }

  return TRUE;
}

PINDEX SIPSrvRecord::GetSize( 
  const PString & proto
)const
{
  if( proto *= "UDP" )
    return m_UdpHostRecords.GetSize();
  else
    return m_TcpHostRecords.GetSize();
}

const SIPSrvRecord::HostRecord * SIPSrvRecord::GetRecordAt( 
  PINDEX i,
  const PString & proto
)const
{
  if( i >= GetSize( proto ) )
     return NULL;

  if( proto *= "UDP" )
    return &m_UdpHostRecords[i];
  else
    return &m_TcpHostRecords[i];
}


void SIPSrvRecord::PrintOn(
  ostream & strm
)const
{
  PINDEX i = 0;
  for( i = 0; i < GetSize( "UDP" ); i++ )
  {
    const HostRecord * recPtr = GetRecordAt( i, "UDP" );
    strm << *recPtr << " ";
  }

  for( i = 0; i < GetSize( "TCP" ); i++ )
  {
    const HostRecord * recPtr = GetRecordAt( i, "TCP" );
    strm << *recPtr << " ";
  }
  
}



