/*
 *
 * SDPPacket.cxx
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
 * $Log: SDPPacket.cxx,v $
 * Revision 1.6  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.4  2006/03/15 16:40:46  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SDPPacket.h"

using namespace SDP;

#define new PNEW

SDPPacket::SDPPacket()
{
}

SDPPacket::SDPPacket( 
  const PString & sdp
)
{
  operator=( sdp );
}

SDPPacket::SDPPacket(
  const SDPPacket & sdp
)
{
  operator=( sdp );
}

SDPPacket::~SDPPacket()
{
}

SDPPacket & SDPPacket::operator=( 
  const PString & sdp 
)
{
  PStringArray allHeaders;
  PStringArray sessionHeaders;
  PStringArray mediaHeaders;

  SDPHeader::Tokenize( sdp, allHeaders );

  PINDEX i = 0;
  /// get the session headers
  for( i = 0; i < allHeaders.GetSize(); i++ )
  {
    PString token = allHeaders[i].Trim();
    if( token.Left(1) *= "m" )
      break;
    else
      sessionHeaders.AppendString( token );
  }

  m_Session = SDPSessionDesc( sessionHeaders );

  for( ; i < allHeaders.GetSize(); i++ )
  {
    PString token = allHeaders[i].Trim();
    mediaHeaders.AppendString( token );
  }

  ParseMediaHeaders( mediaHeaders );

  return *this;
}

void SDPPacket::ParseMediaHeaders(
  const PStringArray & headers
)
{
  if( headers.GetSize() == 0 )
    return;

  PAssert( headers[0].Left( 1 ) *= "m", PLogicError );

  SDPMediaDesc * mediaDescription = NULL;

  for( PINDEX i = 0; i < headers.GetSize(); i++ )
  {
    SDPHeader h = headers[i];
    
    if( h.GetSDPName() *= "m" )
    {
      mediaDescription = new SDPMediaDesc();
      SDPMediaAnnouncement announcement( h );
      mediaDescription->SetMediaAnnouncement( announcement );
      m_MediaSessions.Append( mediaDescription );
    }else
    {
      //PAssertNULL( mediaDescription );
      mediaDescription->AppendHeader( h );
    }

  }
}

SDPPacket & SDPPacket::operator=(
  const SDPPacket & sdp
)
{
  m_Session = sdp.m_Session;
 
  m_MediaSessions = sdp.m_MediaSessions;
  m_MediaSessions.MakeUnique();

  return *this;
}

void SDPPacket::PrintOn(
  ostream & strm
)const
{
  strm << m_Session;
  for( PINDEX i = 0; i < m_MediaSessions.GetSize(); i++ )
    strm << m_MediaSessions[i];
}

PObject * SDPPacket::Clone()
{
  return new SDPPacket( *this );
}

PString SDPPacket::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

BOOL SDPPacket::Construct( 
  SDPPacket & packet,
  DWORD sessionId,
  DWORD versionId,
  const PString & mediaAddress,
  const PString & addressType,
  const PString & user,
  const PString & sessionName,
  const PString & description,
  const PString & email,
  const PString & phone
)
{
  packet.GetSession().SetProtocolVersion( "0" );

  if( !email.IsEmpty() )
    packet.GetSession().SetEmailAddress( email );

  if( !phone.IsEmpty() )
    packet.GetSession().SetPhoneNumber( phone );
  
  if( !sessionName.IsEmpty() )
    packet.GetSession().SetSessionName( sessionName  );

  if( !description.IsEmpty() )
  packet.GetSession().SetSessionInformation( description );

  SDPTime validity( 0, 0 );
  packet.GetSession().SetTime( validity ); 
  
  SDPOrigin origin;
  origin.SetAddress( mediaAddress );
  origin.SetSessionId( PString( sessionId ) );
  origin.SetUserName( user);
  origin.SetVersion( PString( versionId ) );
  origin.SetAddressType( addressType );
  packet.GetSession().SetOrigin( origin );

  SDPConnectionData connection;
  connection.SetAddress( mediaAddress );
  connection.SetAddressType( addressType );
  packet.GetSession().SetConnectionData( connection );

  return TRUE;
}

