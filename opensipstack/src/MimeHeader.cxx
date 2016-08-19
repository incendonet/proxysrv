/*
 *
 * MimeHeader.cxx
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
 * $Log: MimeHeader.cxx,v $
 * Revision 1.7  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/06/08 04:54:20  joegenbaclor
 * !.  Added OnRegistrationTimeout Call back to Registrar classes
 * 2. Moved ECCIAPI to Tools namespace
 *
 * Revision 1.5  2006/06/08 01:32:59  joegenbaclor
 * Modified MimeHeaderList::AddHeader() to filter out empty strings
 *
 * Revision 1.4  2006/06/07 09:18:33  joegenbaclor
 * Added the ability to SIPMessage to read from a PChannel
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 */

#include "MimeHeader.h"
#include "ParserTools.h"

#define new PNEW

using namespace SIPParser;

MimeHeader::MimeHeader()
{
}

MimeHeader::MimeHeader( 
  const MimeHeader & header
)
{
  operator=( header );
}

MimeHeader::MimeHeader(
  const PString & header
)
{
  operator=( header );
}

MimeHeader::MimeHeader(
  const PString & name,
  const PString & value
)
{
  m_Name = ParserTools::GetExpandedHeader( name );
  m_Value = value;
}

MimeHeader & MimeHeader::operator=( 
  const MimeHeader & header 
)
{
  m_Name = ParserTools::GetExpandedHeader( header.m_Name );
  m_Value = header.m_Value;
  return *this;
}

MimeHeader & MimeHeader::operator=( 
  const PString & header 
)
{
  ParserTools::SliceMIME( header, m_Name, m_Value );
  m_Name = ParserTools::GetExpandedHeader( m_Name );
  return *this;
}

void MimeHeader::PrintOn( 
  ostream & strm 
)const
{
  strm << m_Name << ": " << m_Value;
}

PString MimeHeader::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

PObject * MimeHeader::Clone()const
{
  return new MimeHeader( *this );
}

MimeHeader::operator PString()
{
  return AsString();
}

/////////////////////////////////////////////////////////////////

MimeHeaderList::MimeHeaderList()
{
}
    
MimeHeaderList::MimeHeaderList( 
  const MimeHeaderList & list 
)
{
  operator=(list);
}
    
MimeHeaderList::MimeHeaderList( 
  istream &strm
)
{
  ReadFrom( strm );
}

void MimeHeaderList::ReadFrom(
  istream &strm   ///< Stream to read the objects contents from.
)
{
  m_MimeHeaders.RemoveAll();

  PString line;
  PString lastLine;
  while (!strm.bad() && !strm.eof()) {
    strm >> line;
    if (line.IsEmpty())
      break;
    if (line[0] == ' ') 
      lastLine += line;
    else {
      AddHeader(lastLine);
      lastLine = line;
    }
  }

  if (!lastLine.IsEmpty()) {
    AddHeader(lastLine);
  }
}

void MimeHeaderList::PrintOn(
  ostream &strm
)const
{
  for( PINDEX i = 0; i < m_MimeHeaders.GetSize(); i++ )
    strm << m_MimeHeaders[i] << "\r\n";
}

MimeHeaderList & MimeHeaderList::operator=( 
  const MimeHeaderList & list 
)
{
  m_MimeHeaders.MakeUnique();
  m_MimeHeaders.RemoveAll();
  m_MimeHeaders = list.m_MimeHeaders;
  return *this;
}

void MimeHeaderList::AddHeader(
  const PString & header
)
{
  if( !header.IsEmpty() )
    m_MimeHeaders.Append( new MimeHeader( header ) );
}

void MimeHeaderList::AddHeader(
  const PString & key,
  const PString & value
)
{
  if( !key.IsEmpty() && value.IsEmpty() )
    m_MimeHeaders.Append( new MimeHeader( key, value ) );
}

void MimeHeaderList::AddHeader(
  const MimeHeader & header
)
{
  m_MimeHeaders.Append( new MimeHeader( header ) );
}

const MimeHeader * MimeHeaderList::GetHeaderAt(
  const PString &key,
  PINDEX idx
)const 
{
  PINDEX currentIdx = 0;
  for( PINDEX i = 0; i < m_MimeHeaders.GetSize(); i++ )
  {
    const MimeHeader & h = m_MimeHeaders[i];
    if( h.GetMimeName() *= key )
    {
      if( currentIdx == idx )
        return &m_MimeHeaders[i];
      else
        currentIdx++;
    }
  }

  return NULL;
}

const MimeHeader * MimeHeaderList::GetHeaderAt(
  PINDEX idx
)const
{
  if( idx < m_MimeHeaders.GetSize() ) 
    return &m_MimeHeaders[idx];
  
  return NULL;
}

PString MimeHeaderList::GetString(
  const PString & key,
  const PString & dflt,
  PINDEX i
)
{
  const MimeHeader * h = GetHeaderAt( key, i );
  if( h != NULL )
    return h->GetMimeValue();

  return dflt;
}

int MimeHeaderList::GetInteger(
  const PString &key,
  int dflt,
  PINDEX i
)
{
  const MimeHeader * h = GetHeaderAt( key, i );
  if( h != NULL )
    return h->GetMimeValue().AsInteger();

  return dflt;
}


