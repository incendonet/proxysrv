/*
 *
 * Via.cxx
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
 * $Log: Via.cxx,v $
 * Revision 1.9  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.8  2006/06/28 14:15:01  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.7  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.6  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.5  2006/03/24 15:42:18  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.4  2006/03/21 14:11:51  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.3  2006/03/14 03:53:54  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "Via.h"
#include "SIPTransport.h"

#define new PNEW

using namespace SIPParser;
using namespace SIPTransports;

Via::Via() 
  : SIPHeaderB( "Via" )
{
  m_Port = 0;
  m_Protocol = "UDP";
}

Via::Via( 
  const Via & via 
) : SIPHeaderB( "Via" )
{
  m_Port = 0;
  m_Protocol = "UDP";
  operator=(via);
}

Via::Via(
  const PString & mime
) : SIPHeaderB( "Via" )
{
  m_Port = 0;
  m_Protocol = "UDP";
  operator=( mime );
}

Via::Via(
  const MimeHeader & mime
) : SIPHeaderB( "Via" )
{
  m_Port = 0;
  m_Protocol = "UDP";
  operator=( mime );
}

Via::Via(
  const PString & protocol,
  const PString & address,
  const MimeParam::SortedCollection & params
) : SIPHeaderB( "Via" )
{
  SetAddress( address );
  m_Protocol = protocol;
  m_Parameters = params;
  m_Parameters.MakeUnique();
}

Via & Via::operator=( 
  const Via & via
)
{
  m_Protocol = via.m_Protocol;
  m_Address = via.m_Address;
  m_Port = via.m_Port;
  m_Parameters = via.m_Parameters;
  m_Parameters.MakeUnique();
  return *this;
}

Via & Via::operator=( 
  const PString & via
)
{
  ///Via: SIP/2.0/UDP pc33.atlanta.com;branch=z9hG4bK776asdhds
  MimeHeader h( via );
  return operator=( h );
}

Via & Via::operator=(
  const MimeHeader & mime
)
{
  /// SIP/2.0/UDP pc33.atlanta.com;branch=z9hG4bK776asdhds

  PString mimeValue = mime.GetMimeValue();

  PINDEX nextSpace = mimeValue.Find( ' ' );

  if( nextSpace == P_MAX_INDEX )
    return *this;

  /// get the protocol
  PString protoVersion = mimeValue.Left( nextSpace );
  if( protoVersion.IsEmpty() )
    return *this;

  PStringArray protoVersionTokens = protoVersion.Tokenise( '/' );

  if( protoVersionTokens.GetSize() != 3 )
    return *this;

  PString v = mimeValue.Mid( nextSpace + 1 );

  PINDEX nextSemiColon = v.Find( ';' );

  PString address = v;
  if( nextSemiColon != P_MAX_INDEX )
  {
    address = v.Left( nextSemiColon );
  }else
  {
    /// via without a branch parameter????
    /// should we return FALSE????
  }

  m_Protocol = protoVersionTokens[2];
  SetAddress( address );
 
  MimeParam::CreateCollection( mime, m_Parameters );
  m_Parameters.MakeUnique();

  return *this;
}

void Via::SetProtocol(
  const PString & transport
)
{
  m_Protocol = transport;
}

void Via::SetAddress(
  const PString & address
)
{
  m_Address = address;

  PINDEX hasPort = address.Find( ":" );
  if( hasPort != P_MAX_INDEX )
  {
    PString port = address.Mid( hasPort + 1);
    PString addr = address.Left( hasPort  );
    m_Port = (WORD)port.AsUnsigned();
    m_Address = addr;
  }
  
}

void Via::SetPort(
  WORD port
)
{
  m_Port = port;
}


const PString & Via::GetProtocol()const
{
  return m_Protocol;
}

const PString & Via::GetAddress()const
{
  return m_Address;
}

WORD Via::GetPort()const
{
  return m_Port;
}

PString Via::GetBranch()const
{
  PString value;
  GetParameter( "branch", value );
  return value;
}

void Via::SetBranch(
  const PString & _branch 
)
{
  PString branch( _branch );
  if( branch.IsEmpty() )
    branch = ParserTools::GenBranchParameter();

  BOOL replace = TRUE;
  AddParameter( "branch", branch, replace );
}


void Via::PrintOn(
  ostream & strm 
) const
{
  strm << "Via: SIP/2.0/" <<  m_Protocol.ToUpper() << " " << m_Address;

  if( m_Port != 0 )
    strm << ":" << m_Port;

  for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
  {
    strm << ";";
    strm << m_Parameters.GetDataAt( i );
  }
}

void Via::PrepareBody()
{
  PStringStream strm;

  strm << "SIP/2.0/" <<  m_Protocol.ToUpper() << " " << m_Address;

  for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
  {
    strm << ";";
    strm << m_Parameters.GetDataAt( i );
  }

  m_HeaderBody = strm;
}


PObject * Via::Clone()const
{
  return new Via( *this );
}


SIPURI Via::GetURI()const
{
  SIPURI uri;
  uri.SetHost( GetAddress() );

  if( GetPort() != 0 )
    uri.SetPort( PString( GetPort() ) );

  MimeParam::SortedCollection params = GetParameters();
  params.MakeUnique();
  uri.SetParameters( params );

  return uri;
}

WORD Via::GetRPort()const
{
  PString rport;
  if( !GetParameter( "rport", rport ) )
    return 0;

  return (WORD)rport.AsUnsigned();
}

PIPSocket::Address Via::GetReceiveAddress()const
{
  PIPSocket::Address received;
  PString val;
  if( GetParameter( "received", val ) )
    received = PIPSocket::Address( val );
  
  if( val.IsEmpty() )
    received = PIPSocket::Address( GetAddress() );

  return received;
}

void Via::SetRPort(
  WORD rport
)
{
  AddParameter( "rport", PString( rport ), TRUE );
}

void Via::SetReceiveAddress(
  const PIPSocket::Address & raddr 
)
{
  AddParameter( "received", raddr.AsString(), TRUE );
}

BOOL Via::IsBehindNAT()const
{
  SIPURI uri = GetURI();
  
  PIPSocket::Address addr;
  WORD port;

  SIPTransport::Resolve( uri, addr, port );

  BOOL isPrivate = SIPTransport::IsPrivateNetwork( addr );

  if( isPrivate )
  {
    PIPSocket::Address raddr = GetReceiveAddress();
    if( raddr.IsValid() )
      return raddr != addr;
  }

  return FALSE;
}

BOOL Via::IsLoopback()const
{
  SIPURI uri = GetURI();
  
  PIPSocket::Address addr;
  WORD port;

  SIPTransport::Resolve( uri, addr, port );

  return addr.IsLoopback();
}


