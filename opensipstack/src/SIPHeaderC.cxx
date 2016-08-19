/*
 *
 * SIPHeaderC.cxx
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
 * $Log: SIPHeaderC.cxx,v $
 * Revision 1.5  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPHeaderC.h"

#define new PNEW

using namespace SIPParser;

SIPHeaderC::SIPHeaderC( 
  const PString & headerName
) : SIPHeader( headerName )
{
}

SIPHeaderC::SIPHeaderC(
  const PString & headerName,
  const SIPHeaderC & header 
) : SIPHeader( headerName )
{
  operator=( header );
}

SIPHeaderC::SIPHeaderC(
  const PString & headerName,
  const PString & header 
) : SIPHeader( headerName )
{
  operator=( header );
}

SIPHeaderC::SIPHeaderC(
  const PString & headerName,
  const MimeHeader & header
) : SIPHeader( headerName )
{
  m_HeaderBody = header.GetMimeValue();
  operator=( header );
}

SIPHeaderC::SIPHeaderC(
  const PString & headerName,
  const PStringArray & header
) : SIPHeader( headerName )
{
  operator=(header);
}

SIPHeaderC & SIPHeaderC::operator=( 
  const SIPHeaderC & header 
)
{
  m_CollectionEntries = header.m_CollectionEntries;
  m_HeaderBody = header.m_HeaderBody;
  m_HeaderName = header.m_HeaderName;
  return *this;
}

PObject * SIPHeaderC::Clone()const
{
  SIPHeaderC * h = new SIPHeaderC( m_HeaderName );
  h->m_CollectionEntries = m_CollectionEntries;
  h->m_HeaderBody = m_HeaderBody;
  return h;
}

SIPHeaderC & SIPHeaderC::operator=(
  const PString & header 
)
{
  MimeHeader h( header );
  return operator=(h);
}

SIPHeaderC & SIPHeaderC::operator=(
  const MimeHeader & header
)
{
  PString value = header.GetMimeValue();
  PStringArray entries = value.Tokenise( ',' );
  return operator=(entries);
}

SIPHeaderC & SIPHeaderC::operator=(
  const PStringArray & header
)
{

  for( PINDEX i = 0; i < header.GetSize(); i++ )
    m_CollectionEntries.Append( new StringParam( header[i] ) );

  PrepareBody();
  return *this;
}

void SIPHeaderC::PrintOn( 
  ostream & strm 
)const
{
  ///Accept: application/sdp;level=1, application/x-private, text/html
  strm << m_HeaderName << ": ";
  for( PINDEX i = 0; i < m_CollectionEntries.GetSize(); i++ )
  {
    strm << m_CollectionEntries[i];

    if( i < m_CollectionEntries.GetSize() - 1 )
    {
      strm << ", ";
    }
  }

}

void SIPHeaderC::PrepareBody()
{
  PStringStream body;

  for( PINDEX i = 0; i < m_CollectionEntries.GetSize(); i++ )
  {
    body << m_CollectionEntries[i];

    if( i < m_CollectionEntries.GetSize() - 1 )
    {
      body << ", ";
    }
  }

  m_HeaderBody = body;
}


StringParam SIPHeaderC::PopTopEntry()
{
  StringParam param = GetEntry( 0 );
  m_CollectionEntries.RemoveAt( 0 );
  return param;
}

StringParam SIPHeaderC::PopLastEntry()
{
  PINDEX collSize = m_CollectionEntries.GetSize();
  StringParam param = GetEntry( collSize - 1 );
  m_CollectionEntries.RemoveAt( collSize - 1 );
  return param;
}

PString SIPHeaderC::GetEntry(
  PINDEX index
)
{
  if( index >= m_CollectionEntries.GetSize() )
  {
    PTRACE( 1, "*** INVALID INDEX *** SIPHeaderC::GetEntry()" );
    return PString::Empty();
  }

  PString entry = m_CollectionEntries[index];
  return entry.Trim();
}

void SIPHeaderC::AddEntry(
  const PString & entry
)
{
  m_CollectionEntries.Append( new StringParam(entry) );
}

void SIPHeaderC::PrependEntry(
  const PString & entry
)
{
  PINDEX i;
  StringParam::Collection old;
  for( i = 0; i < m_CollectionEntries.GetSize(); i++ )
    old.Append( new StringParam( m_CollectionEntries[i] ) );
  
  m_CollectionEntries.RemoveAll();
  
  m_CollectionEntries.Append( new StringParam(entry) );

  for( i = 0; i < old.GetSize(); i++ )
    m_CollectionEntries.Append( new StringParam(old[i]) );
}

PString SIPHeaderC::operator[]( PINDEX index )
{
  return GetEntry( index );
}

void SIPHeaderC::operator+=(
  const PString & entry
)
{
  AddEntry( entry );
}

void SIPHeaderC::RemoveAll()
{
  m_CollectionEntries = StringParam::Collection();
}


///////////////////////////////////////////////////////////////////



SIPHeaderCList::SIPHeaderCList()
{
}

SIPHeaderCList::SIPHeaderCList(
  const SIPHeaderCList & list
)
{
  operator=(list);
}

SIPHeaderCList::SIPHeaderCList(
  const SIPHeaderC::Collection & list
)
{
  operator=(list);
}

BOOL SIPHeaderCList::Append( 
  const PString & token,
  PINDEX idx
)
{
  if( m_Collection.GetSize() == 0 )
    return FALSE;

  SIPHeaderC &h = m_Collection[idx];
  h+=token;
  return TRUE;
}

BOOL SIPHeaderCList::Append(
  const SIPHeaderC & header
)
{
  m_Collection.Append( new SIPHeaderC( header ) );
  return TRUE;
}

const SIPHeaderC & SIPHeaderCList::GetAt(
  PINDEX idx
)const
{
  return m_Collection[idx];
}

PString SIPHeaderCList::GetTokenAt(
  PINDEX idx1,
  PINDEX idx2
)
{
  return m_Collection[idx1].GetEntry( idx2 );
}

PString SIPHeaderCList::operator()(
  PINDEX idx1,
  PINDEX idx2
)
{
  return GetTokenAt( idx1, idx2 );
}

SIPHeaderCList & SIPHeaderCList::operator=(
  const SIPHeaderCList & list
)
{
  m_Collection = list.m_Collection;
  return *this;
}

SIPHeaderCList & SIPHeaderCList::operator=(
  const SIPHeaderC::Collection & list
)
{
  m_Collection = list;
  return *this;
}

SIPHeaderCList & SIPHeaderCList::operator+=( 
  const SIPHeaderC & header
)
{
  Append( header );
  return *this;
}

SIPHeaderC & SIPHeaderCList::operator[](
  PINDEX idx
)
{
  return m_Collection[idx];
}


void SIPHeaderCList::PrintOn(
  ostream & strm
)const
{
  for( PINDEX i = 0; i < m_Collection.GetSize(); i++ )
  {
    if( !m_Collection[i].GetHeaderBody().IsEmpty() )
    {
      strm << m_Collection[i];
      if( i != m_Collection.GetSize() - 1 )
        strm << "\r\n";
    }
  }
}

PString SIPHeaderCList::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

PObject * SIPHeaderCList::Clone()const
{
  return new SIPHeaderCList( *this );
}


