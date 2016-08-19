/*
 *
 * SDPConnectionData.cxx
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
 * $Log: SDPConnectionData.cxx,v $
 * Revision 1.5  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
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

#include "SDPConnectionData.h"

using namespace SDP;

#define new PNEW

SDPConnectionData::SDPConnectionData()
{
  m_Name = "c";
  m_NetworkType = "IN";
  m_AddressType = "IP4";
}

SDPConnectionData::SDPConnectionData( 
  const SDPHeader & header
) : SDPHeader( header )
{
  PStringArray tokens = m_Value.Tokenise( ' ', FALSE );

  if( tokens.GetSize() == 3 )
  {
    m_NetworkType = tokens[0];
    m_AddressType = tokens[1];
    m_Address = tokens[2];
  }

  m_Name = "c";
}

SDPConnectionData::SDPConnectionData(
  const PString & header
) 
{
  operator=( header );
  m_Name = "c";
}

SDPConnectionData::SDPConnectionData(
  const PString & name,
  const PString & value
) : SDPHeader( name, value )
{
  PStringArray tokens = m_Value.Tokenise( ' ', FALSE );

  if( tokens.GetSize() == 3 )
  {
    m_NetworkType = tokens[0];
    m_AddressType = tokens[1];
    m_Address = tokens[2];
  }
  m_Name = "c";
}

PObject * SDPConnectionData::Clone()const
{
  return new SDPConnectionData( *this );
}


SDPConnectionData::SDPConnectionData(
  const PString & networkType,
  const PString & addressType,
  const PString & address
)
{
  m_NetworkType = networkType;
  m_AddressType = addressType;
  m_Address = address;
  m_Name = "c";
}

SDPConnectionData::SDPConnectionData(
  const SDPConnectionData & header
) : SDPHeader( header )
{
  m_NetworkType = header.m_NetworkType;
  m_AddressType = header.m_AddressType;
  m_Address = header.m_Address;
  m_Name = "c";
}


SDPConnectionData & SDPConnectionData::operator=( const PString & header )
{
  SDPHeader::operator=( header );
  PStringArray tokens = m_Value.Tokenise( ' ', FALSE );

  if( tokens.GetSize() == 3 )
  {
    m_NetworkType = tokens[0];
    m_AddressType = tokens[1];
    m_Address = tokens[2];
  }

  m_Name = "c";

  return *this;
}

SDPConnectionData & SDPConnectionData::operator=( const SDPConnectionData & header )
{
  SDPHeader::operator=( header );
  m_NetworkType = header.m_NetworkType;
  m_AddressType = header.m_AddressType;
  m_Address = header.m_Address;
  m_Name = "c";
  return *this;
}


void SDPConnectionData::PrintOn( 
  ostream & strm 
)const
{
  strm << "c=" 
       << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;
}

void SDPConnectionData::SetNetworkType(
  const PString & value
)
{
  m_NetworkType = value;
  PStringStream body;

  body << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}

void SDPConnectionData::SetAddressType(
  const PString & value
)
{
  m_AddressType = value;
  PStringStream body;

  body << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}

void SDPConnectionData::SetAddress(
  const PString & value
)
{
  m_Address = value;
  PStringStream body;

  body << m_NetworkType << " "
       << m_AddressType << " "
       << m_Address;

  m_Value = body;
}


