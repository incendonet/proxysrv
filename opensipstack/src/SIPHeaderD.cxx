/*
 *
 * SIPHeaderD.cxx
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
 * $Log: SIPHeaderD.cxx,v $
 * Revision 1.4  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPHeaderD.h"

#define new PNEW

using namespace SIPParser;

SIPHeaderD::SIPHeaderD(
  const PString & headerName 
  ) : SIPHeader( headerName )
{
}

SIPHeaderD::SIPHeaderD(
  const PString & headerName,
  const SIPHeaderD & header 
  ) : SIPHeader( headerName )
{
  operator=( header );
}

SIPHeaderD::SIPHeaderD(
  const PString & headerName,
  const PString & header 
  ) : SIPHeader( headerName )
{
  operator=( header );
}

SIPHeaderD::SIPHeaderD(
  const PString & headerName,
  const MimeHeader & header
  ) : SIPHeader( headerName )
{
  m_HeaderBody = header.GetMimeValue();
  operator=( header );
}

SIPHeaderD & SIPHeaderD::operator=(
  const SIPHeaderD & header 
)
{
  m_HeaderName = header.m_HeaderName;
  m_HeaderBody = header.m_HeaderBody;
  m_Parameters = header.m_Parameters;
  m_LeadString = header.m_LeadString;
  return *this;
}

PObject * SIPHeaderD::Clone()const
{
  SIPHeaderD * h = new SIPHeaderD( m_HeaderName );
  h->m_HeaderBody = m_HeaderBody;
  h->m_Parameters = m_Parameters;
  h->m_LeadString = m_LeadString;
  return h;
}

SIPHeaderD & SIPHeaderD::operator=(
  const PString & header 
)
{
  MimeHeader h( header );
  return operator=( h );
}

SIPHeaderD & SIPHeaderD::operator=(
  const MimeHeader & header
)
{
  PString mimeValue = header.GetMimeValue();
  
  MimeParam::CreateCollection( mimeValue, m_Parameters, ',' );
  
  if( m_Parameters.GetSize() > 0 )
  {
    PStringArray tokens  = m_Parameters[0].GetParamName().Tokenise( ' ' );
    if( tokens.GetSize() == 2 )
    {
      m_LeadString = tokens[0];
      m_Parameters[0].SetParamName( tokens[1] );
    }
  }
 
  PrepareBody();

  return *this;
}

void SIPHeaderD::PrintOn(
  ostream & strm
)const
{
  ///Accept: application/sdp;level=1, application/x-private, text/html
  strm << m_HeaderName << ": ";

  if( !m_LeadString.IsEmpty() )
    strm << m_LeadString << " ";

  for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
  {

    strm << m_Parameters[i];

    if( i < m_Parameters.GetSize() - 1 )
    {
      strm << ", ";
    }
  }
}

void SIPHeaderD::PrepareBody()
{
  PStringStream strm;
  
  if( !m_LeadString.IsEmpty() )
    strm << m_LeadString << " ";

  for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
  {
    if( m_Parameters[i].IsStartInNewLine() )
      strm << "\r\n\t";

    strm << m_Parameters[i];

    if( i < m_Parameters.GetSize() - 1 )
    {
      strm << ", ";
    }
  }

  m_HeaderBody = strm;
}

void SIPHeaderD::AddParameter(
  const PString & name,
  const PString & value
)
{
  MimeParam * param = new MimeParam( name, value );
  m_Parameters.Append( param );
  PrepareBody();
}

void SIPHeaderD::SetParameter(
  const PString & name,
  const PString & value,
  PINDEX offSet
)
{
  PINDEX currentOffSet = 0;
  for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
  {
    if( m_Parameters[i].GetParamName() *= name  )
    {
      if( currentOffSet == offSet )
        m_Parameters[i].SetParamValue( value );
      else
        currentOffSet++;
    }
  }

  PrepareBody();
}

PINDEX SIPHeaderD::FindParameter(
  const PString & name,
  PINDEX offSet
)
{
  PINDEX currentOffSet = 0;
  for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
  {
    if( m_Parameters[i].GetParamName() *= name  )
    {
      if( currentOffSet == offSet )
        return i;
      else
        currentOffSet++;
    }
  }

  return P_MAX_INDEX;
}

BOOL SIPHeaderD::GetParameter(
  const PString & name,
  PString & paramValue,
  PINDEX offSet
)
{
  PINDEX currentOffSet = 0;
  for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
  {
    if( m_Parameters[i].GetParamName() *= name  )
    {
      if( currentOffSet == offSet )
      {
        paramValue = m_Parameters[i].GetParamValue();
        return TRUE;
      }else
        currentOffSet++;
    }
  }

  return FALSE;
}

MimeParam & SIPHeaderD::GetParameter(
  PINDEX index
)
{
  return m_Parameters[index];
}


