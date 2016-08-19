/*
 *
 * SIPURI.cxx
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
 * $Log: SIPURI.cxx,v $
 * Revision 1.19  2006/12/19 23:38:34  joegenbaclor
 * Fixed bug in softphone proxy authentication where passwords <= 3 in length may not be properly handled
 *
 * Revision 1.18  2006/12/03 06:17:23  joegenbaclor
 * Made sure printon returns an empty string if user and host is not set
 *
 * Revision 1.17  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.16  2006/11/30 00:51:15  joegenbaclor
 * 1.  Corrected bug in authorization construction where the URI can have a double port concatenation.
 * 2.  Modified AsString() method of SIPURI to allow not returning the URI parameters.
 * 3.  Added GetBasicURI method
 *
 * Revision 1.15  2006/07/27 02:51:43  joegenbaclor
 * Introduced static variable for known URI schemes for better readability
 *
 * Revision 1.14  2006/07/26 00:44:52  joegenbaclor
 * Added Voice File classes for upcoming IVR support
 *
 * Revision 1.13  2006/07/17 11:36:45  joegenbaclor
 * 1.  More routing enhancements to B2BUA
 *
 * Revision 1.12  2006/07/13 06:53:05  joegenbaclor
 * 1.  Introduced Sanity checks for incoming SIP Message
 * 2.  Corrected bug in SIPURI where enum scheme is not getting recognized.
 * 3.  Added ENUM support to routing
 * 4.  Introduced global routing flag to indicate when and not to rewrite outbound To URI's
 *
 * Revision 1.11  2006/07/07 10:05:14  joegenbaclor
 * Added functionality to autogenerate A1 Hash from SIPURI's
 *
 * Revision 1.10  2006/07/07 09:53:26  joegenbaclor
 * Added loose comparisong operator to SIPURI
 *
 * Revision 1.9  2006/06/28 14:15:00  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.8  2006/06/22 08:27:57  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.7  2006/06/06 13:30:25  joegenbaclor
 * Initial upload of PIDF files - thanks Solegy Systems!
 *
 * Revision 1.6  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.5  2006/03/17 13:51:00  joegenbaclor
 * 1.  Implemented UAC Call Redirection
 * 2.  Implemented SendCancel() method in CallSession
 * 3.  Introduced ClearInternalHeaders() function in SIP Message to erase internal headers inheritted from previous transaction.
 *
 * Revision 1.4  2006/03/14 03:53:54  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPURI.h"
#include "ParserTools.h"
#include "SIPTransport.h"



#define new PNEW
using namespace SIPParser;
using namespace SIPTransports;
////sip:user:password@host:port;uri-parameters?headers

static PString KnownSchemes = 
"callto "
"enum "
"file "
"ftp "
"http "
"https "
"ip "
"mailto "
"nfs "
"pop3 "
"rtbe "
"sip "  
"sips "   
"smtp "
"tcp "
"tls "
"udp ";
   
SIPURI::SIPURI()
{
  m_Scheme = "sip";
}

SIPURI::SIPURI( 
  const SIPURI & uri
)
{
  operator=( uri );
}

SIPURI::SIPURI(
  const PString & uri 
)
{
  operator=( uri );
}

SIPURI::SIPURI(
  const MimeHeader & header 
)
{
  operator=( header );
}



SIPURI & SIPURI::operator=( 
  const SIPURI & uri
)
{
  m_Scheme = uri.m_Scheme;
  m_User = uri.m_User;
  m_Password = uri.m_Password;
  m_Host = uri.m_Host;
  m_Port = uri.m_Port;

  /// copy the parameters
  m_Parameters.RemoveAll();
  for( PINDEX i = 0; i < uri.m_Parameters.GetSize(); i++ )
  {
    MimeParam param = uri.m_Parameters.GetDataAt( i );
    AddParameter( param.GetParamName(), param.GetParamValue() );
  }

  m_Headers = uri.m_Headers;
  return *this;
}

SIPURI & SIPURI::operator=(
  const PString & uri 
)
{
  MimeHeader h( uri );
  return operator=( h );
}

SIPURI & SIPURI::operator=(
  const MimeHeader & header 
)
{
  
  /*
  sip:alice@atlanta.com
  sip:alice:secretword@atlanta.com;transport=tcp
  sips:alice@atlanta.com?subject=project%20x&priority=urgent
  sip:+1-212-555-1212:1234@gateway.com;user=phone
  sips:1212@gateway.com
  sip:alice@192.0.2.4
  sip:atlanta.com;method=REGISTER?to=alice%40atlanta.com
  sip:alice;day=tuesday@atlanta.com
  */

  /*
   When the header field value contains a display name, the URI
   including all URI parameters is enclosed in "<" and ">".  If no "<"
   and ">" are present, all parameters after the URI are header
   parameters, not URI parameters.  The display name can be tokens, or a
   quoted string, if a larger character set is desired.
  */


  PString value = header.GetMimeValue();
  /// check if we have a display name before the <> enclosure
  PINDEX begin = value.Find( '<' ) + 1;
  PINDEX end = value.Find( '>' );
  BOOL hasEnclosure = begin != P_MAX_INDEX && end != P_MAX_INDEX;
  PString uri;

  if( hasEnclosure )
  {
    PINDEX rightOffSet = value.GetLength() - end;
    PINDEX length = value.GetLength() - begin - rightOffSet;
    uri = value.Mid( begin, length ).Trim();
  }else
  {
    if( !header.GetMimeName().IsEmpty() ) /// treat as a full SIP header witout a display name
    {
      /// we have header parameters
      /// just get the leading token as the uri
      PStringArray t0 = value.Tokenise( ';' );
      if( t0.GetSize() > 1 )
        uri = t0[0];
      else
        uri = value;
    }else
    {
      uri = value.Trim();
    }
  }

  PStringArray t1, t2;
  PString tBuff = value;

  PINDEX atIndex = value.FindLast( '@' );

  if( atIndex != P_MAX_INDEX )
    tBuff = value.Mid( atIndex + 1 );
 
  t1 = tBuff.Tokenise( ';' );
  PINDEX hasParams = t1.GetSize() - 1;
  t2 = tBuff.Tokenise( '?' );
  PINDEX hasHeaders = t2.GetSize() - 1;

  if( hasParams != 0 )
  {
    for( PINDEX i = 1; i < t1.GetSize(); i++ )
    {
      MimeParam * param = new MimeParam( t1[i] );
      if( !param->GetParamName().IsEmpty() )
        m_Parameters.SetAt( param->GetParamName(), param );
      else
        delete param;
    }

    /// strip the headers from the uri
    if( !IsPhoneURI() )
      uri = uri.Tokenise( ';' )[0];
    else
    {
      if( atIndex != P_MAX_INDEX )
      {
        /// only get rid of the params after the @ sign
        PStringArray hostUserToken = uri.Tokenise( '@' );
        PString hostToken = hostUserToken[1].Tokenise( ';' )[0];
        uri = hostUserToken[0] + "@" + hostToken;
      }else
      {
        PString hostToken = uri.Tokenise( ';' )[0];
        uri = hostToken;
      }
    }
  }

  if( hasHeaders != 0)
  {
    PStringArray finalHeaders = t2[1].Tokenise( '?' );

    for( PINDEX i = 0; i < finalHeaders.GetSize(); i++ )
    {
      MimeParam * param = new MimeParam( finalHeaders[i] );
      if( !param->GetParamName().IsEmpty() )
        m_Headers.Append(  param );
      else
        delete param;
    }
  }

  ///sip:user:password@host:port;uri-parameters?headers
  PString scheme, user, password, host, port;
  PINDEX at = uri.FindLast( '@' );

  if( at != P_MAX_INDEX )
  {
    PString buff = uri.Left( at );

      PStringArray userTokens = buff.Tokenise( ':' );
      PINDEX ts = userTokens.GetSize();
      switch ( ts )
      {
        case 0:
          break;
        case 1:
          scheme = "sip";
          user = userTokens[0];
          break;
        case 2: /// scheme with user
          scheme = userTokens[0];
          user = userTokens[1];
          break;
        default: /// scheme with user:password
          scheme = userTokens[0];
          user = userTokens[1];
          password = userTokens[2];
          //ignore the rest
          break;

      }
      /// host would be all chars after the @ sign
      host = uri.Mid( at + 1 );
      
      /// now check if host has a port
      PStringArray hostTokens = host.Tokenise( ':' );
      if( hostTokens.GetSize() == 2 )
      {
        host = hostTokens[0];
        port = hostTokens[1];
      }
  }else
  {
    /// No @ sign found, lets assume its a host address
    ///sip:host:port
    PString buff = uri;
    PStringArray userTokens = buff.Tokenise( ':' );
    PINDEX ts = userTokens.GetSize();
    switch ( ts )
    {
      case 0:
        return *this;
      case 1:
        host = userTokens[0];
        scheme = "sip";
        break;
      case 2: /// scheme with host
        if( !ParserTools::IsNumeric( userTokens[1] ) )
        {
          scheme = userTokens[0];
          host = userTokens[1];
        }else
        {
          ///check if  userTokens[0] is a known scheme
          PString tmpScheme = userTokens[0].ToLower().Trim();
          if( KnownSchemes.Find( tmpScheme ) != P_MAX_INDEX )
          {
            /// scheme is present
            scheme = userTokens[0];
            host = userTokens[1];
          }else
          {
            /// no scheme present
            scheme = "sip";
            host = userTokens[0];
            port = userTokens[1];
          }
        }
        break;
      default: /// scheme with user:password
        scheme = userTokens[0];
        host = userTokens[1];
        port = userTokens[2];
        //ignore the rest
        break;
    };
  }

  if( user.Left( 2 ) == "//" )
    user = user.Mid( 2 );

  m_Scheme = scheme;
  m_User = user;
  m_Password = password;
  m_Port = port;
  m_Host = host;

  return *this;
}



void SIPURI::PrintOn( 
  ostream & strm 
)const
{
  if( m_User.IsEmpty() && m_Host.IsEmpty() )
    return;

  ///sip:user:password@host:port;uri-parameters?headers
  ///sip:alice:secretword@atlanta.com;transport=tcp
  strm << m_Scheme << ":";

  if( m_User.IsEmpty() && m_Host.IsEmpty() )
    return;

  if( !m_User.IsEmpty() )
  {
    strm << m_User;
    
    if( !m_Password.IsEmpty() )
      strm << ":" << m_Password;

    strm << "@";
  }

  strm << m_Host;

  if( !m_Port.IsEmpty() )
    strm << ":" << m_Port;

  for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
  {
    strm << ";" << m_Parameters.GetDataAt(i);
  }

  if( m_Headers.GetSize() > 0 )
  {
    strm << "?";
    for( PINDEX j = 0; j < m_Headers.GetSize(); j++ )
    {
      strm << m_Headers[j];

      if( j < m_Headers.GetSize() - 1 )
        strm << "&";
    }
  }

}

PString SIPURI::AsString( BOOL includeURIParams, BOOL includeUser, BOOL includePort )const
{
  PStringStream strm;


  if( m_User.IsEmpty() && m_Host.IsEmpty() )
    return PString::Empty();

  ///sip:user:password@host:port;uri-parameters?headers
  ///sip:alice:secretword@atlanta.com;transport=tcp
  strm << m_Scheme << ":";

  if( m_User.IsEmpty() && m_Host.IsEmpty() )
    return PString::Empty();

  if( !m_User.IsEmpty() && includeUser )
  {
    strm << m_User;
    
    if( !m_Password.IsEmpty() )
      strm << ":" << m_Password;

    strm << "@";
  }

  strm << m_Host;

  if( !m_Port.IsEmpty() && includePort )
    strm << ":" << m_Port;

  if( includeURIParams )
  {
    for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
    {
      strm << ";" << m_Parameters.GetDataAt(i);
    }

    if( m_Headers.GetSize() > 0 )
    {
      strm << "?";
      for( PINDEX j = 0; j < m_Headers.GetSize(); j++ )
      {
        strm << m_Headers[j];

        if( j < m_Headers.GetSize() - 1 )
          strm << "&";
      }
    }
  }
  

  return strm;
}

SIPURI SIPURI::GetBasicURI()const
{
  return SIPURI( AsString( FALSE ) );
}

void SIPURI::AddParameter(
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

void SIPURI::SetParameter(
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

PINDEX SIPURI::FindParameter(
  const PString & paramName
)const
{
  for( PINDEX i = 0; i < m_Parameters.GetSize(); i++ )
  {
    if( m_Parameters.GetDataAt( i ).GetParamName() *= paramName )
      return i;
  }

  return P_MAX_INDEX;
}

PString SIPURI::GetParameter(
  const PString & name
)const
{
  PString param;
  GetParameter( name, param );
  return param;
}

BOOL SIPURI::GetParameter(
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

MimeParam & SIPURI::GetParameter(
  PINDEX index
)const
{
  return m_Parameters.GetDataAt(index);
}

//--

void SIPURI::AddHeader(
  const PString & name,
  const PString & value,
  BOOL replaceIfExist
)
{
  PINDEX index = FindHeader( name );

  if( index != P_MAX_INDEX && replaceIfExist )
  {
    GetHeader( index ).SetParamValue( value );
  }else if( index == P_MAX_INDEX )
  {
    m_Headers.Append( new MimeParam( name, value ) );
  }
}

void SIPURI::SetHeader(
  const PString & name,
  const PString & value
)
{
  PINDEX index = FindHeader( name );

  if( index != P_MAX_INDEX  )
  {
    GetHeader( index ).SetParamValue( value );
  }
}

PINDEX SIPURI::FindHeader(
  const PString & paramName
)const
{
  for( PINDEX i = 0; i < m_Headers.GetSize(); i++ )
  {
    if( m_Headers[i].GetParamName() *= paramName )
      return i;
  }

  return  P_MAX_INDEX;
}

BOOL SIPURI::GetHeader(
  const PString & name,
  PString & value
)const
{
  PINDEX index = FindHeader( name );

  if( index == P_MAX_INDEX )
    return FALSE;
  
  value = GetHeader( index ).GetParamValue();
  return TRUE;
}

MimeParam & SIPURI::GetHeader(
  PINDEX index
)const
{
  return m_Headers[index];
}


BOOL SIPURI::IsPhoneURI()const
{
  PString value;
  GetParameter( "user", value );
  if( value *= "phone" )
    return TRUE;
  else
    return FALSE;
}

BOOL SIPURI::IsPrivateNetwork()const
{
  return SIPTransport::IsPrivateNetwork( *this );
}

PIPSocket::Address SIPURI::GetAddress()const
{
  PIPSocket::Address address;
  WORD port;
  SIPTransport::Resolve( *this, address, port );
  return address;
}

BOOL SIPURI::operator*=(
  const SIPURI & uri
)
{
  BOOL userTheSame;
  BOOL hostTheSame;

  userTheSame = (GetUser() *= uri.GetUser());
  hostTheSame = (GetHost() *= uri.GetHost());

  return userTheSame && hostTheSame;
}

PString SIPURI::AsA1Hash()const
{
  MD5::A1Hash hash( GetUser(), GetHost(), GetPassword() );
  return hash.AsString();
}

