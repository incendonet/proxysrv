/*
 *
 * Contact.cxx
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
 * $Log: Contact.cxx,v $
 * Revision 1.4  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "Contact.h"

#define new PNEW

using namespace SIPParser;

Contact::Contact() 
  : SIPHeaderC( "Contact" )
{
}

Contact::Contact( const Contact & header ) 
  : SIPHeaderC( "Contact" )
{
  m_CollectionEntries = StringParam::Collection( header.m_CollectionEntries );
  m_HeaderBody = header.m_HeaderBody;
  m_CollectionEntries.MakeUnique();
}

Contact::Contact( const PString & header ) 
  : SIPHeaderC( "Contact", header )
{
}

Contact::Contact( const MimeHeader & header ) 
  : SIPHeaderC( "Contact", header )
{
}

Contact::Contact( const PStringArray & header )
  : SIPHeaderC( "Contact", header )
{
}

Contact::Contact(
  const ContactURI::Collection & uri
) : SIPHeaderC( "Contact" )
{
  operator=(uri);
}

Contact & Contact::operator=(
  const ContactURI::Collection & uri
)
{
  for( PINDEX i = 0; i < uri.GetSize(); i++ )
  {
    AddURI( uri[i] );
  }

  PString str = AsString();
  MimeHeader h( str );
  m_HeaderBody = h.GetMimeValue();

  return *this;
}

BOOL Contact::GetURI(
  ContactURI & uri,
  PINDEX uriIndex
)
{
  PString buff = GetEntry(uriIndex);
  
  if( buff.IsEmpty() )
    return FALSE;

  uri = buff;

  return TRUE;
}

BOOL Contact::AddURI(
  const ContactURI & uri
)
{
  PStringStream buff;
  buff << uri;
  AddEntry( buff );
  PString str = AsString();
  MimeHeader h( str );
  m_HeaderBody = h.GetMimeValue();
  return TRUE;
}

PObject * Contact::Clone()const
{
  Contact * h = new Contact( m_HeaderName );
  h->m_CollectionEntries = m_CollectionEntries;
  h->m_HeaderBody = m_HeaderBody;
  h->m_CollectionEntries.MakeUnique();
  return h;
}

