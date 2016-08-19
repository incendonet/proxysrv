/*
 *
 * SIPHeaderB.cxx
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
 * $Log: SIPHeaderB.cxx,v $
 * Revision 1.8  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.7  2006/05/19 06:30:37  joegenbaclor
 * 1.  Fixed bug in SIPHeaderB where tag may not be parsed properly if no < > enclosure is present
 * 2.  Various enhancements to OpenSBC and proxy session
 *
 * Revision 1.6  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.5  2006/03/21 14:11:51  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.4  2006/03/17 13:51:00  joegenbaclor
 * 1.  Implemented UAC Call Redirection
 * 2.  Implemented SendCancel() method in CallSession
 * 3.  Introduced ClearInternalHeaders() function in SIP Message to erase internal headers inheritted from previous transaction.
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPHeaderB.h"

#define new PNEW

using namespace SIPParser;

SIPHeaderB::SIPHeaderB( 
  const PString & headerName
  ) : SIPHeader( headerName )
{
  m_HasURIEnclosure = FALSE;
}

SIPHeaderB::SIPHeaderB(
  const PString & headerName,
  const SIPHeaderB & header 
)  : SIPHeader( headerName )
{
  m_HasURIEnclosure = FALSE;
  operator=( header );
}

SIPHeaderB::SIPHeaderB(
  const PString & headerName,
  const PString & header 
)  : SIPHeader( headerName )
{
  m_HasURIEnclosure = FALSE;
  operator=( header );
}

SIPHeaderB::SIPHeaderB(
  const PString & headerName,
  const MimeHeader & header
)  : SIPHeader( headerName )
{
  m_HasURIEnclosure = FALSE;
  m_HeaderBody = header.GetMimeValue();
  operator=( header );
}

SIPHeaderB::SIPHeaderB(
  const PString & headerName,
  const SIPURI & header,
  const PString & displayName,
  const PString & tag
) : SIPHeader( headerName )
{
  m_HasURIEnclosure = FALSE;
  m_URI = header;
  m_DisplayName = displayName;
  
  if( !tag.IsEmpty() )
    AddParameter( "tag", tag, TRUE );
}

SIPHeaderB & SIPHeaderB::operator=( 
  const SIPHeaderB & header 
)
{
  m_DisplayName = header.m_DisplayName;
  m_HeaderName = header.m_HeaderName;
  m_HeaderBody = header.m_HeaderBody;
  m_Parameters = header.m_Parameters;
  m_URI = header.m_URI;
  m_HasURIEnclosure = header.m_HasURIEnclosure;
  return *this;
}

SIPHeaderB & SIPHeaderB::operator=(
  const PString & header 
)
{
  MimeHeader h( header );
  return operator=( h );
}

SIPHeaderB & SIPHeaderB::operator=(
  const MimeHeader & header
)
{
  PString mimeValue = header.GetMimeValue();
  /// SIPHeaderB: "Bob" <sips:bob@biloxi.com> ;tag=a48s
  /// SIPHeaderB: sip:+12125551212@phone2net.com;tag=887s
  /// SIPHeaderB: Anonymous <sip:c8oqz84zk7z@privacy.org;transport=udp>;tag=hyh8
   
  PINDEX hasDisplayName = mimeValue.Find( '<' );
  PINDEX greater = mimeValue.Find( '>' );
  PINDEX hasParams = mimeValue.Find( ';', greater );
  PString uriString = mimeValue;
  if( hasDisplayName != P_MAX_INDEX )
  {
    /// parse the display name
    m_HasURIEnclosure = TRUE;
    m_DisplayName = mimeValue.Left( hasDisplayName ).Trim();
    uriString = uriString.Mid( hasDisplayName + 1, greater - hasDisplayName -1 );
  }

  if( hasParams != P_MAX_INDEX )
  {
    /// parse the parameters
    PString params = mimeValue.Mid( hasParams + 1 );
    MimeParam::CreateCollection( params, m_Parameters );
    PString uriString = mimeValue.Left( hasParams );
  }

  m_URI = uriString;
  
  return *this;
}

PObject * SIPHeaderB::Clone()const
{
  SIPHeaderB * h = new SIPHeaderB( m_HeaderName );
  h->m_DisplayName = m_DisplayName;
  h->m_HeaderName = m_HeaderName;
  h->m_HeaderBody = m_HeaderBody;
  h->m_Parameters = m_Parameters;
  h->m_URI = m_URI;
  h->m_HasURIEnclosure = m_HasURIEnclosure;
  return h;
}

void SIPHeaderB::PrintOn(
  ostream & strm
)const
{
  if( !GetHeaderName().IsEmpty() )
    strm << GetHeaderName() << ": ";
  
  BOOL hasDisplayName = !m_DisplayName.IsEmpty();

  if( hasDisplayName )
  {
    ///if( m_DisplayName[0] != '\"' )
    ///  strm << "\"" << m_DisplayName << "\"";
    ///else
    strm << m_DisplayName;

    strm << " <" << m_URI << ">";
    
  }else
  {
    if( m_Parameters.GetSize() > 0 || m_HasURIEnclosure )
      strm << " <"; 
    
    strm << m_URI;
    
    if( m_Parameters.GetSize() > 0 || m_HasURIEnclosure )
      strm << ">";
  }

  if( m_Parameters.GetSize() > 0 )
  {
    for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
      strm << ";" << m_Parameters.GetDataAt( i );
  }

}

void SIPHeaderB::PrepareBody()
{
  PStringStream strm;

  BOOL hasDisplayName = !m_DisplayName.IsEmpty();

  if( hasDisplayName )
  {
    strm << m_DisplayName;
    strm << " <" << m_URI << ">";
  }else
  {
    strm << m_URI;
  }

  if( m_Parameters.GetSize() > 0 )
  {
    for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
      strm << ";" << m_Parameters.GetDataAt( i );
  }

  m_HeaderBody = strm;
}

void SIPHeaderB::AddParameter(
  const PString & name,
  const PString & value,
  BOOL replaceIfExist
)
{
  BOOL exist = m_Parameters.Contains( name );
  if( !exist )
  {
    m_Parameters.SetAt( name, new MimeParam( name, value ) );
  }else
  {
    if( replaceIfExist )
    {
      MimeParam * param = m_Parameters.GetAt( name );
      //PAssertNULL( param );
      param->SetParamName( name );
      param->SetParamValue( value );
    }
  }
}

void SIPHeaderB::SetParameter(
  const PString & name,
  const PString & value
)
{
  BOOL exist = m_Parameters.Contains( name );
  if( exist )
  {
      MimeParam * param = m_Parameters.GetAt( name );
      //PAssertNULL( param );
      param->SetParamName( name );
      param->SetParamValue( value );
  }else
  {
    AddParameter( name, value );
  }
}

PINDEX SIPHeaderB::FindParameter(
  const PString & paramName
)
{
  for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
  {
    if( m_Parameters.GetDataAt( i ).GetParamName() *= paramName )
      return i;
  }

  return P_MAX_INDEX;
}

BOOL SIPHeaderB::GetParameter(
  const PString & name,
  PString & value
)const
{
  MimeParam * param = m_Parameters.GetAt( name );
  
  if( param != NULL )
  {
    value = param->GetParamValue();
    return TRUE;
  }

  return FALSE;
}

MimeParam & SIPHeaderB::GetParameter(
  PINDEX index
)const
{
  return m_Parameters.GetDataAt(index);
}

