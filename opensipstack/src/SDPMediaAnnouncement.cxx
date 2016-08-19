/*
 *
 * SDPMediaAnnouncement.cxx
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
 * $Log: SDPMediaAnnouncement.cxx,v $
 * Revision 1.7  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/04/18 07:18:21  joegenbaclor
 * Fixed unix build environment errors and warnings
 *
 * Revision 1.5  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
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

#include "SDPMediaAnnouncement.h"

#define new PNEW

using namespace SDP;

SDPMediaAnnouncement::SDPMediaAnnouncement()
{
  //m=audio 49170 RTP/AVP 0
  m_Name = "m";
  m_Media = " audio";
  m_Port = 0;
  m_Transport = "RTP/AVP";
}

SDPMediaAnnouncement::SDPMediaAnnouncement( 
  const SDPHeader & header
) : SDPHeader( header ) 
{
  m_Name = "m";
  PStringArray tokens = m_Value.Tokenise( ' ', FALSE );
  if( tokens.GetSize() >= 3 )
  {
    m_Media = tokens[0];
    m_Port = (WORD)tokens[1].AsUnsigned();
    m_Transport = tokens[2];

    for( PINDEX i = 0; i < tokens.GetSize() - 3; i++ )
      m_FormatList[i] = (BYTE)tokens[ i + 3 ].AsUnsigned();
  }
}

SDPMediaAnnouncement::SDPMediaAnnouncement(
  const PString & header
) : SDPHeader( header ) 
{
  m_Name = "m";
  PStringArray tokens = m_Value.Tokenise( ' ', FALSE );
  if( tokens.GetSize() >= 3 )
  {
    m_Media = tokens[0];
    m_Port = (WORD)tokens[1].AsUnsigned();
    m_Transport = tokens[2];

    for( PINDEX i = 0; i < tokens.GetSize() - 3; i++ )
      m_FormatList[i] = (BYTE)tokens[ i + 3 ].AsUnsigned();
  }
}

SDPMediaAnnouncement::SDPMediaAnnouncement(
  const PString & name,
  const PString & value
) : SDPHeader( name, value ) 
{
  m_Name = "m";
  PStringArray tokens = m_Value.Tokenise( ' ', FALSE );
  if( tokens.GetSize() >= 3 )
  {
    m_Media = tokens[0];
    m_Port = (WORD)tokens[1].AsUnsigned();
    m_Transport = tokens[2];

    for( PINDEX i = 0; i < tokens.GetSize() - 3; i++ )
      m_FormatList[i] = (BYTE)tokens[ i + 3 ].AsUnsigned();
  }
}


SDPMediaAnnouncement::SDPMediaAnnouncement(
  const SDPMediaAnnouncement & header
) : SDPHeader( header ) 
{
  m_Name = "m";
  PStringArray tokens = m_Value.Tokenise( ' ', FALSE );

  if( tokens.GetSize() >= 3 )
  {
    m_Media = tokens[0];
    m_Port = (WORD)tokens[1].AsUnsigned();
    m_Transport = tokens[2];

    for( PINDEX i = 0; i < tokens.GetSize() - 3; i++ )
      m_FormatList[i] = (BYTE)tokens[ i + 3 ].AsUnsigned();
  }
}


SDPMediaAnnouncement::SDPMediaAnnouncement(
  const PString & media,
  WORD port,
  const PString & transport,
  const PBYTEArray & formatList
)
{
  m_Name = "m";
  m_Media = media;
  m_Port = port;
  m_Transport = transport;
  m_FormatList = formatList;

  m_Value = m_Media + PString( " " ) + PString( m_Port ) + PString( " " ) + m_Transport;
  
  for( PINDEX i = 0; i < m_FormatList.GetSize(); i++ )
    m_Value += PString( " " ) + PString( m_FormatList[i] );

}

SDPMediaAnnouncement & SDPMediaAnnouncement::operator=( const PString & header )
{
  SDPHeader::operator =( header );
  m_Name = "m";
  PStringArray tokens = m_Value.Tokenise( ' ', FALSE );
  if( tokens.GetSize() >= 3 )
  {
    m_Media = tokens[0];
    m_Port = (WORD)tokens[1].AsUnsigned();
    m_Transport = tokens[2];

    for( PINDEX i = 0; i < tokens.GetSize() - 3; i++ )
      m_FormatList[i] = (BYTE)tokens[ i + 3 ].AsUnsigned();
  }
  return *this;
}

SDPMediaAnnouncement & SDPMediaAnnouncement::operator=( const SDPMediaAnnouncement & header )
{
  SDPHeader::operator =( header );
  m_Name = "m";
  PStringArray tokens = m_Value.Tokenise( ' ', FALSE );
  if( tokens.GetSize() >= 3 )
  {
    m_Media = tokens[0];
    m_Port = (WORD)tokens[1].AsUnsigned();
    m_Transport = tokens[2];

    for( PINDEX i = 0; i < tokens.GetSize() - 3; i++ )
      m_FormatList[i] = (BYTE)tokens[ i + 3 ].AsUnsigned();
  }
  return *this;
}

void SDPMediaAnnouncement::SetMedia(
  const PString & media
)
{
  m_Media = media;

  m_Value = m_Media + PString( " " ) + PString( m_Port ) + PString( " " ) + m_Transport;
  
  for( PINDEX i = 0; i < m_FormatList.GetSize(); i++ )
    m_Value += PString( " " ) + PString( m_FormatList[i] );
}

void SDPMediaAnnouncement::SetPort(
  WORD port
)
{
  m_Port = port;

  m_Value = m_Media + PString( " " ) + PString( m_Port ) + PString( " " ) + m_Transport;
  
  for( PINDEX i = 0; i < m_FormatList.GetSize(); i++ )
    m_Value += PString( " " ) + PString( m_FormatList[i] );
}

void SDPMediaAnnouncement::SetTransport(
  const PString & transport
)
{
  m_Transport = transport;

  m_Value = m_Media + PString( " " ) + PString( m_Port ) + PString( " " ) + m_Transport;
  
  for( PINDEX i = 0; i < m_FormatList.GetSize(); i++ )
    m_Value += PString( " " ) + PString( m_FormatList[i] );
}

void SDPMediaAnnouncement::AppendFormat(
  BYTE format
)
{
  m_FormatList[m_FormatList.GetSize()] = format;

  m_Value = m_Media + PString( " " ) + PString( m_Port ) + PString( " " ) + m_Transport;
  
  for( PINDEX i = 0; i < m_FormatList.GetSize(); i++ )
    m_Value += PString( " " ) + PString( m_FormatList[i] );
}

void SDPMediaAnnouncement::AppendFormat(
  const SDPMediaFmt & format
)
{
  AppendFormat( (BYTE)format.GetPayloadType() );
}

PObject * SDPMediaAnnouncement::Clone()const
{
  return new SDPMediaAnnouncement( *this );
}


