/*
 *
 * SIPHeader.cxx
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
 * $Log: SIPHeader.cxx,v $
 * Revision 1.5  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/08/14 01:29:08  joegenbaclor
 * Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPHeader.h"
#include "MimeHeader.h"

#define new PNEW

using namespace SIPParser;

SIPHeader::SIPHeader(
  const PString & headerName,
  const PString & headerBody
)
{
  m_HeaderName = headerName;
  MimeHeader h( headerBody );
  m_HeaderBody = h.GetMimeValue();
}

SIPHeader::SIPHeader( 
  const MimeHeader & h 
)
{
  operator=(h);
}

SIPHeader::SIPHeader(
  const SIPHeader & header
)
{
  m_HeaderName = header.m_HeaderName;
  m_HeaderBody = header.m_HeaderBody;
}

SIPHeader & SIPHeader::operator=( const PString & value )
{
  MimeHeader h( value );
  return operator=(h);
}

SIPHeader & SIPHeader::operator=( const MimeHeader & h )
{
  m_HeaderName = h.GetMimeName();
  m_HeaderBody = h.GetMimeValue();
  return *this;
}

SIPHeader & SIPHeader::operator=( const SIPHeader & header )
{
  m_HeaderName = header.m_HeaderName;
  m_HeaderBody = header.m_HeaderBody;
  return *this;
}

PObject * SIPHeader::Clone()const
{
  return new SIPHeader( *this );
}

PString SIPHeader::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

const PString & SIPHeader::GetHeaderBody()const
{
  const_cast<SIPHeader *>(this)->PrepareBody();
  return m_HeaderBody;
}

void SIPHeader::PrintOn(
  ostream & strm 
)const
{
  if( !m_HeaderName.IsEmpty() )
    strm << m_HeaderName << ": ";
  
  strm << m_HeaderBody; 
}

void SIPHeader::PrepareBody()
{
}

SIPHeader::operator PString()
{
  return AsString();
}




