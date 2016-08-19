/*
 *
 * ContactURI.cxx
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
 * $Log: ContactURI.cxx,v $
 * Revision 1.6  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.4  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "ContactURI.h"

#define new PNEW

using namespace SIPParser;

ContactURI::ContactURI() 
  : SIPHeaderB( PString::Empty() )
{
}

ContactURI::ContactURI(
  const ContactURI & header
) : SIPHeaderB( PString::Empty(), header )
{
}

ContactURI::ContactURI(
  const PString & header
) : SIPHeaderB( PString::Empty(), header )
{
}

ContactURI::ContactURI(
  const SIPURI & uri,
  const PString & displayName
  ) : SIPHeaderB( PString::Empty(), uri, displayName )
{
}

ContactURI::ContactURI(
  const MimeHeader & header
) : SIPHeaderB( PString::Empty(), header )
{
}

PObject * ContactURI::Clone()const
{
  ContactURI * uri = new ContactURI();
  uri->m_DisplayName = m_DisplayName;
  uri->m_HeaderBody = m_HeaderBody;
  uri->m_HeaderName = m_HeaderName;
  uri->m_Parameters = m_Parameters;
  uri->m_HasURIEnclosure = m_HasURIEnclosure;
  uri->m_URI = m_URI;
  return  uri;
}


void ContactURI::PrintOn(
  ostream & strm
)const
{
  if( !GetHeaderName().IsEmpty() )
    strm << GetHeaderName() << ": ";
  
  BOOL hasDisplayName = !m_DisplayName.IsEmpty();

  if( hasDisplayName )
  {
    if( m_DisplayName[0] != '\"' )
      strm << "\"" << m_DisplayName << "\"";
    else
      strm << m_DisplayName;
  }
    
  
  strm << " <" << m_URI << ">";
  

  if( m_Parameters.GetSize() > 0 )
  {
    for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
      strm << ";" << m_Parameters.GetDataAt( i );
  }

}


