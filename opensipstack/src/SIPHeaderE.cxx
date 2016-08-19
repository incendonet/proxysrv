/*
 *
 * SIPHeaderE.cxx
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
 * $Log: SIPHeaderE.cxx,v $
 * Revision 1.6  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/10/23 15:44:58  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.4  2006/10/12 08:41:24  joegenbaclor
 * Added support for RFC 4028 headers
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPHeaderE.h"

#define new PNEW

using namespace SIPParser;
SIPHeaderE::SIPHeaderE(
  const PString & name
) : SIPHeader( name, "" )
{
}

SIPHeaderE::SIPHeaderE(
  const PString & name,
  PInt64 value,
  PInt64 minValue,
  PInt64 maxValue,
  PInt64 defaultValue
) : SIPHeader( name, PString( value ) )
{
  m_MinValue = minValue;
  m_MaxValue = maxValue;
  m_DefaultValue = defaultValue;
}

PObject * SIPHeaderE::Clone()const
{
  SIPHeaderE * clone =  new SIPHeaderE( 
    m_HeaderName,
    m_HeaderBody.AsUnsigned(),
    m_MinValue,
    m_MaxValue,
    m_DefaultValue
  );


  return clone;
}

SIPHeaderE::SIPHeaderE(
  const PString & name,
  const PString & header,
  PInt64 minValue,
  PInt64 maxValue,
  PInt64 defaultValue
) : SIPHeader( name, header )
{
  m_MinValue = minValue;
  m_MaxValue = maxValue;
  m_DefaultValue = defaultValue;
}

SIPHeaderE::SIPHeaderE(
  const MimeHeader & header,
  PInt64 minValue,
  PInt64 maxValue,
  PInt64 defaultValue
) : SIPHeader( header )
{
  m_MinValue = minValue;
  m_MaxValue = maxValue;
  m_DefaultValue = defaultValue;
  m_HeaderBody = header.GetMimeValue();
}



SIPHeaderE & SIPHeaderE::operator=( 
  PInt64 & val 
)
{
  if( m_MaxValue != 0 )
    PAssert( val < m_MaxValue + 1, PLogicError );
  
  PAssert( val > m_MinValue, PLogicError );
  
  m_HeaderBody = PString( val );
  return *this;
}

SIPHeaderE & SIPHeaderE::operator--()
{
  PInt64 val = AsInt64();
  PAssert( val > m_MinValue, "SIPHeaderE::operator--() value exceeding minimum value!" );
  *this = --val;
  return *this;
}

SIPHeaderE & SIPHeaderE::operator++()
{
  PInt64 val = AsInt64();

  if( m_MaxValue != 0 )
    PAssert( val < m_MaxValue, "SIPHeaderE::operator++() exceeding maximum value!" );
  
  *this = ++val;
  return *this;
}

BOOL SIPHeaderE::operator<( PInt64 value )
{
  PInt64 val = AsInt64();
  return value < val;
}

BOOL SIPHeaderE::operator>( PInt64 value )
{
  PInt64 val = AsInt64();
  return value > val;
}


PInt64 SIPHeaderE::AsInt64()const
{
  if( m_HeaderBody.IsEmpty() )
    return 0;

  PStringArray tokens = m_HeaderBody.Tokenise( ';' );
  if( tokens.GetSize() > 1 )
    return tokens[0].AsInt64();

  return m_HeaderBody.AsInt64();
}

void SIPHeaderE::SetValue( PInt64 value )
{
  PStringArray tokens = m_HeaderBody.Tokenise( ";" );
  if( tokens.GetSize() > 0 )
    tokens[0] = PString( value );

  PStringStream newVal;
  for( PINDEX i = 0; i < tokens.GetSize(); i++ )
  {
    newVal << tokens[i];
    if( i < tokens.GetSize() - 1 )
      newVal << ";";
  }

  m_HeaderBody = newVal;
}

void SIPHeaderE::SetParameter(
  const PString & paramName,
  const PString & paramValue
)
{
  PStringArray tokens = m_HeaderBody.Tokenise( ";" );
  if( tokens.GetSize() == 0 )
   return;

  for( PINDEX i = 1; i < tokens.GetSize(); i++ )
  {
    MimeParam param = tokens[i];
    if( param.GetParamName() *= paramName )
    {
      param.SetParamValue( paramValue );
      tokens[i] = param.AsString();
      break;
    }
  }

  PStringStream newVal;
  for( PINDEX j = 0; j < tokens.GetSize(); j++ )
  {
     newVal << tokens[j];

     if( j < tokens.GetSize() - 1 )
       newVal << ";";
  }

  m_HeaderBody = newVal;
}

PString SIPHeaderE::GetParameter( 
  const PString & name
)
{
  PINDEX hasParams = m_HeaderBody.Find( ';' );
  if( hasParams == P_MAX_INDEX )
    return PString::Empty();

  MimeParam::SortedCollection parameters;

  /// parse the parameters
  PString params = m_HeaderBody.Mid( hasParams + 1 );
  MimeParam::CreateCollection( params, parameters );

  MimeParam * mp = parameters.GetAt( name );
  
  if( mp != NULL )
    return mp->GetParamValue();

  return PString::Empty();
}



