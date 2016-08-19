/*
 *
 * SDPOrigin.cxx
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
 * $Log: SDPOrigin.cxx,v $
 * Revision 1.5  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SDPOrigin.h"

using namespace SDP;

#define new PNEW

SDPOrigin::SDPOrigin()
{
  m_Name = "o";
  m_UserName = "phone";
  m_SessionId = "0";
  m_Version = "0";
  m_NetworkType = "IN";
  m_AddressType = "IP4";
  m_Address = "0.0.0.0";

  
}

SDPOrigin::SDPOrigin( 
  const SDPHeader & header
) : SDPHeader( header )
{
  PStringArray tokens = m_Value.Tokenise( ' ', FALSE );

  if( tokens.GetSize() == 6 )
  {
    m_UserName = tokens[0];
    m_SessionId = tokens[1];
    m_Version = tokens[2];
    m_NetworkType = tokens[3];
    m_AddressType = tokens[4];
    m_Address = tokens[5];
  }

  m_Name = "o";

  PStringStream body;

  body << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}

SDPOrigin::SDPOrigin(
  const PString & header
) 
{
  operator=( header );
  m_Name = "o";
}

SDPOrigin::SDPOrigin(
  const PString & name,
  const PString & value
) : SDPHeader( name, value )
{
  PStringArray tokens = m_Value.Tokenise( ' ', FALSE );

  if( tokens.GetSize() == 6 )
  {
    m_UserName = tokens[0];
    m_SessionId = tokens[1];
    m_Version = tokens[2];
    m_NetworkType = tokens[3];
    m_AddressType = tokens[4];
    m_Address = tokens[5];
  }
  m_Name = "o";

  PStringStream body;

  body << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}

/*
o=<username> <session id> <version> <network type> <address type> <address>
o=alice 2890844526 2890844526 IN IP4 host.anywhere.com

The "o=" field gives the originator of the session (their username
and the address of the user's host) plus a session id and session
version number.
*/

SDPOrigin::SDPOrigin(
  const PString & userName,
  const PString & sessionId,
  const PString & version,
  const PString & networkType,
  const PString & addressType,
  const PString & address
)
{
  m_UserName = userName;
  m_SessionId = sessionId;
  m_Version = version;
  m_NetworkType = networkType;
  m_AddressType = addressType;
  m_Address = address;
  m_Name = "o";

  PStringStream body;

  body << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}

SDPOrigin::SDPOrigin(
  const SDPOrigin & header
) : SDPHeader( header )
{
  m_UserName = header.m_UserName;
  m_SessionId = header.m_SessionId;
  m_Version = header.m_Version;
  m_NetworkType = header.m_NetworkType;
  m_AddressType = header.m_AddressType;
  m_Address = header.m_Address;
  m_Name = "o";

  PStringStream body;

  body << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}


SDPOrigin & SDPOrigin::operator=( const PString & header )
{
  SDPHeader::operator=( header );
  PStringArray tokens = m_Value.Tokenise( ' ', FALSE );

  if( tokens.GetSize() == 6 )
  {
    m_UserName = tokens[0];
    m_SessionId = tokens[1];
    m_Version = tokens[2];
    m_NetworkType = tokens[3];
    m_AddressType = tokens[4];
    m_Address = tokens[5];
  }

  m_Name = "o";

  PStringStream body;

  body << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;

  return *this;
}

SDPOrigin & SDPOrigin::operator=( const SDPOrigin & header )
{
  SDPHeader::operator=( header );
  m_UserName = header.m_UserName;
  m_SessionId = header.m_SessionId;
  m_Version = header.m_Version;
  m_NetworkType = header.m_NetworkType;
  m_AddressType = header.m_AddressType;
  m_Address = header.m_Address;
  m_Name = "o";

  PStringStream body;

  body << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;

  return *this;
}


void SDPOrigin::PrintOn( 
  ostream & strm 
)const
{
  strm << "o=" 
       << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;
}

void SDPOrigin::SetUserName(
  const PString & value
)
{
  m_UserName = value;
  PStringStream body;

  body << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}

void SDPOrigin::SetSessionId(
  const PString & value
)
{
  m_SessionId = value;
  PStringStream body;

  body << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}

void SDPOrigin::SetVersion(
  const PString & value
)
{
  m_Version = value;
  PStringStream body;

  body << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}

void SDPOrigin::SetNetworkType(
  const PString & value
)
{
  m_NetworkType = value;
  PStringStream body;

  body << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}

void SDPOrigin::SetAddressType(
  const PString & value
)
{
  m_AddressType = value;
  PStringStream body;

  body << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}

void SDPOrigin::SetAddress(
  const PString & value
)
{
  m_Address = value;
  PStringStream body;

  body << m_UserName << " "
       << m_SessionId << " "
       << m_Version << " "
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}

PObject * SDPOrigin::Clone()const
{
  return new SDPOrigin( *this );
}
