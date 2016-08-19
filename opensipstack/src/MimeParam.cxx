/*
 *
 * MimeParam.cxx
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
 * $Log: MimeParam.cxx,v $
 * Revision 1.5  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "MimeParam.h"
#include "ParserTools.h"

#define new PNEW

using namespace SIPParser;

MimeParam::MimeParam()
{
  m_StartsInNewLine = FALSE;
  m_EndsWithNewLine = FALSE;
}

MimeParam::MimeParam(
  const MimeParam & param
)
{
  operator=( param );
}

MimeParam::MimeParam(
  const PString & param 
)
{
  operator=( param );
}

MimeParam::MimeParam(
  const PString & paramName,
  const PString & paramValue
)
{
  m_Name = paramName;
  m_Value = paramValue;
  m_StartsInNewLine = FALSE;
  m_EndsWithNewLine = FALSE;
}

MimeParam & MimeParam::operator=( 
  const MimeParam & param
)
{
  m_Name = param.m_Name;
  m_Value = param.m_Value;
  m_StartsInNewLine = param.m_StartsInNewLine;
  m_EndsWithNewLine = param.m_EndsWithNewLine;
  return *this;
}

MimeParam & MimeParam::operator=(
  const PString & _param
)
{
  PString param( _param );
  param.Replace( "\r\n", "%CRLF%", TRUE );

  PStringArray tokens = param.Tokenise( '=' );
  
  if( tokens.GetSize() == 2 )
  {
    m_Name = tokens[0].Trim();
    m_Value = tokens[1].Trim();
  }else if( tokens.GetSize() > 2 ) /// there are more than one "="
  {
    m_Name = tokens[0].Trim();
    for( PINDEX i = 1; i < tokens.GetSize(); i++ )
    {
      m_Value += tokens[i].Trim();
      if( i < tokens.GetSize() -1 )
        m_Value += "=";
    }
  }else
  {
    m_Value = param;
  }

  m_StartsInNewLine = FALSE;
  m_EndsWithNewLine = FALSE;

  if( m_Name.Find( "%CRLF%" ) != P_MAX_INDEX )
  {
    m_Name.Replace( "%CRLF%", "", TRUE );
    m_StartsInNewLine = TRUE;
  }

  if( m_Value.Find( "%CRLF%" ) != P_MAX_INDEX )
  {
    m_Value.Replace( "%CRLF%", "", TRUE );
    m_EndsWithNewLine = TRUE;
  } 

  
  return *this;
}

void MimeParam::CreateCollection(
  const PString & mime,
  Collection & params,
  const char & delimiter
)
{
  MimeHeader h( mime );
  MimeParam::CreateCollection( h, params, delimiter );
}

void MimeParam::CreateCollection(
  const MimeHeader & mime,
  Collection & params,
  const char & delimiter
)
{
  params.RemoveAll();

  PStringArray tokens = mime.GetMimeValue().Tokenise( delimiter );

  for( PINDEX i = 0; i < tokens.GetSize(); i++ )
  {
    if( tokens[i].Find( '=' ) != P_MAX_INDEX )
      params.Append( new MimeParam( tokens[i] ) );
  }
}


void MimeParam::CreateCollection(
  const PString & mime,
  SortedCollection & params,
  const char & delimiter
)
{
  MimeHeader h( mime );
  MimeParam::CreateCollection( h, params, delimiter );
}

void MimeParam::CreateCollection(
  const MimeHeader & mime,
  SortedCollection & params,
  const char & delimiter
)
{
  params.RemoveAll();

  PStringArray tokens = mime.GetMimeValue().Tokenise( delimiter );

  for( PINDEX i = 0; i < tokens.GetSize(); i++ )
  {
    if( tokens[i].Find( '=' ) != P_MAX_INDEX )
    {
      MimeParam * param = new MimeParam( tokens[i] );
      params.SetAt( param->GetParamName(), param );
    }
  }
}


void MimeParam::PrintOn( 
  ostream & strm 
)const
{
  if( m_StartsInNewLine )
    strm << "\r\n\t";

  strm << m_Name;
  
  if( !m_Value.IsEmpty() )
    strm << "=" << m_Value;

  if( m_EndsWithNewLine )
    strm << "\r\n\t";
}

PObject * MimeParam::Clone()const
{
  return new MimeParam( *this );
}

PString MimeParam::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}
