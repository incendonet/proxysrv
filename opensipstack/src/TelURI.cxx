/*
 *
 * TelURI.cxx
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
 * $Log: TelURI.cxx,v $
 * Revision 1.4  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:53:54  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "TelURI.h"
#include "ParserTools.h"

#define new PNEW

using namespace SIPParser;

TelURI::TelURI()
{
}

TelURI::TelURI( 
  const TelURI & uri
)
{
  operator=( uri );
}

TelURI::TelURI(
  const PString & uri
)
{
  operator=( uri );
}

TelURI::TelURI( 
  const PString & number,
  const MimeParam::SortedCollection & params
)
{
  m_Number = number;
  m_Parameters = params;
}

TelURI & TelURI::operator=(
  const TelURI & uri
)
{
  m_Parameters = uri.m_Parameters;
  m_Number = uri.m_Number;
  return *this;
}

TelURI & TelURI::operator=(
  const PString & uri
)
{
  PINDEX hasParams = uri.Find( ';' ); 
  m_Number = uri;

  if( hasParams != P_MAX_INDEX )
  {
    PStringArray tokens = uri.Tokenise( ';' );
    m_Number = uri.Left( hasParams );
    PString params = uri.Mid( hasParams + 1 );
    MimeParam::CreateCollection( params, m_Parameters );
  }

  return *this;
}

void TelURI::PrintOn(
  ostream & strm
)const
{
  strm << m_Number;

  for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
  {
    strm << ";" << m_Parameters.GetDataAt( i );
  }
}

PString TelURI::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

void TelURI::AddParameter(
  const PString & name,
  const PString & value,
  BOOL replaceIfExist
)
{
  BOOL isExist = m_Parameters.Contains( name );

  if( isExist && replaceIfExist )
  {
    MimeParam * param = m_Parameters.GetAt( name );
    //PAssertNULL( param );
    param->SetParamValue( value );
  }else if( !isExist )
  {
    m_Parameters.SetAt( name, new MimeParam( name, value ) );
  }
}

void TelURI::SetParameter(
  const PString & name,
  const PString & value
)
{
  MimeParam * param = m_Parameters.GetAt( name );
  
  if( param != NULL )
    param->SetParamValue( value );
}

PINDEX TelURI::FindParameter(
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

BOOL TelURI::GetParameter(
  const PString & name,
  PString & value
)
{
  MimeParam * param = m_Parameters.GetAt( name ); 
  if( param == NULL )
    return FALSE;
  
  value = param->GetParamValue();
  return TRUE;
}

MimeParam & TelURI::GetParameter(
  PINDEX index
)
{
  return m_Parameters.GetDataAt( index ); 
}

BOOL TelURI::IsGlobalNumber()
{
  m_Number = m_Number.Trim();
  return m_Number.Left(1) == "+";
}

PObject * TelURI::Clone()const
{
  return new TelURI( *this );
}


