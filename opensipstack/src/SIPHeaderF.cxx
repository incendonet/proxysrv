/*
 *
 * SIPHeaderF.cxx
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
 * $Log: SIPHeaderF.cxx,v $
 * Revision 1.6  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.4  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPHeaderF.h"

using namespace SIPParser;

#define new PNEW

SIPHeaderF::SIPHeaderF(
  const PString & headerName
) 
  : SIPHeaderC( headerName )
{
}

PObject * SIPHeaderF::Clone()const
{
  SIPHeaderF * h = new SIPHeaderF( m_HeaderName );
  h->m_CollectionEntries = m_CollectionEntries;
  h->m_HeaderBody = m_HeaderBody;
  return h;
}

SIPHeaderF::SIPHeaderF(
  const PString & headerName,
  const SIPHeaderF & header 
) 
  : SIPHeaderC( headerName, (SIPHeaderC&)header )
{
}

SIPHeaderF::SIPHeaderF( 
  const PString & headerName,
  const PString & header 
) 
  : SIPHeaderC( headerName, header )
{
}

SIPHeaderF::SIPHeaderF( 
  const PString & headerName,
  const MimeHeader & header 
) 
  : SIPHeaderC( headerName, header )
{
}

SIPHeaderF::SIPHeaderF( 
  const PString & headerName,
  const PStringArray & header 
)
  : SIPHeaderC( headerName, header )
{
}

SIPHeaderF::SIPHeaderF(
  const PString & headerName,
  const RouteURI::Collection & uri
) : SIPHeaderC( headerName )
{
  operator=(uri);
}

SIPHeaderF & SIPHeaderF::operator=(
  const RouteURI::Collection & uri
)
{
  for( PINDEX i = 0; i < uri.GetSize(); i++ )
  {
    AddURI( uri[i] );
  }

  return *this;
}

RouteURI SIPHeaderF::PopTopURI()
{
  StringParam p = PopTopEntry();
  return RouteURI( p );
}

RouteURI SIPHeaderF::PopLastURI()
{
  StringParam p = PopLastEntry();
  return RouteURI( p );
}

BOOL SIPHeaderF::GetURI(
  RouteURI & uri,
  PINDEX uriIndex
)
{
  PString buff = GetEntry(uriIndex);
  
  if( buff.IsEmpty() )
    return FALSE;

  uri = buff;

  return TRUE;
}

BOOL SIPHeaderF::AddURI(
  const RouteURI & uri
)
{
  /// adds a URI at the beginning of a uri list
  PStringStream buff;
  buff << uri;
  PrependEntry( buff );
  return TRUE;
}

///////////////////////////////////////////////////////////

SIPHeaderFList::SIPHeaderFList()
{
}
    
SIPHeaderFList::SIPHeaderFList(
  const SIPHeaderFList & list
)
{
  operator=( list );
}

SIPHeaderFList::SIPHeaderFList(
  const SIPHeaderF::Collection & list
)
{
  operator=( list );
}

BOOL SIPHeaderFList::Append( 
  const RouteURI & uri,
  PINDEX idx
)
{
  SIPHeaderF & h = m_Collection[idx];
  return h.AddURI( uri );
}

BOOL SIPHeaderFList::Append(
  const SIPHeaderF & header
)
{
  PINDEX i;
  SIPHeaderF::Collection old;

  for( i = 0; i < m_Collection.GetSize(); i++ )
    old.Append( new SIPHeaderF( m_Collection[i] ) );
  
  m_Collection.RemoveAll();

  m_Collection.Append( new SIPHeaderF( header ) );

  for( i = 0; i < old.GetSize(); i++ )
    m_Collection.Append( new SIPHeaderF( old[i] ) );

  return TRUE;
}

SIPHeaderF & SIPHeaderFList::GetAt(
  PINDEX idx
)const
{
  return m_Collection[idx];
}

RouteURI SIPHeaderFList::GetAt(
  PINDEX idx1,
  PINDEX idx2
)const
{
  SIPHeaderF & h = m_Collection[idx1];
  RouteURI uri;
  h.GetURI( uri, idx2 );
  return uri;
}

RouteURI SIPHeaderFList::PopTopURI()
{
  SIPHeaderF h =  m_Collection[0];
  RouteURI uri = h.PopTopURI();
  if( h.GetSize() == 0 )
    m_Collection.RemoveAt(0);
  return uri;
}

RouteURI SIPHeaderFList::PopLastURI()
{
  PINDEX colSize = m_Collection.GetSize();
  SIPHeaderF h =  m_Collection[colSize-1];
  RouteURI uri = h.PopLastURI();
  if( h.GetSize() == 0 )
    m_Collection.RemoveAt(colSize-1);
  return uri;
}
void SIPHeaderFList::PrintOn(
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

PString SIPHeaderFList::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

SIPHeaderFList & SIPHeaderFList::operator=(
  const SIPHeaderFList & list
)
{
  m_Collection = list.m_Collection;
  m_Collection.MakeUnique();
  return *this;
}

SIPHeaderFList & SIPHeaderFList::operator=(
  const SIPHeaderF::Collection & list
)
{
  m_Collection = list;
  m_Collection.MakeUnique();
  return *this;
}

SIPHeaderFList & SIPHeaderFList::operator+=( 
  const SIPHeaderF & header
)
{
  m_Collection.Append( new SIPHeaderF( header ) );
  return *this;
}

SIPHeaderF & SIPHeaderFList::operator[](
  PINDEX idx
)
{
  return m_Collection[idx];
}

PObject * SIPHeaderFList::Clone()const
{
  return new SIPHeaderFList( *this );
}



