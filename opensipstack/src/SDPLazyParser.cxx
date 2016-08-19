/*
 *
 * SDPLazyParser.cxx
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
 * $Log: SDPLazyParser.cxx,v $
 * Revision 1.6  2006/12/08 09:54:26  joegenbaclor
 * Added ability to get Media direction
 *
 * Revision 1.5  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/08/14 03:58:32  joegenbaclor
 * Added setting of formatName from SDPMediaFmt::PayloadNames if rtpmap is not available
 *
 * Revision 1.3  2006/08/14 03:52:35  joegenbaclor
 * Set formatName to empty initially when calling GetMediaFormat()
 *
 * Revision 1.2  2006/06/14 10:15:46  joegenbaclor
 * Added SetMediaAddress method
 *
 * Revision 1.1  2006/06/14 08:43:38  joegenbaclor
 * Initial upload of OpenB2BUA applcation and related classes
 *
 *
 */

#include "SDPLazyParser.h"
#include "SDPMediaFormat.h"

#define new PNEW

using namespace SDP;

SDPLazyParser::SDPLazyParser()
{
}

SDPLazyParser::SDPLazyParser(
  const PString & sdp
)
{
  operator=( sdp );
}

SDPLazyParser::SDPLazyParser(
  const SDPLazyParser & sdp
)
{
  operator=( sdp );
}

SDPLazyParser & SDPLazyParser::operator=( 
  const PString & sdp 
)
{
  PString::operator=( sdp );
  return *this;
}

SDPLazyParser & SDPLazyParser::operator=( 
  const SDPLazyParser & sdp 
)
{
  PString::operator=( (const PString &)sdp );
  return *this;
}

static std::ostream& operator<<( 
  std::ostream& os, SDPLazyParser::MediaType type )
{
  switch( type )
  {
  case SDPLazyParser::Audio:
    os << "audio";
    break;
  case SDPLazyParser::Video:
    os << "video";
    break;
  case SDPLazyParser::Image:
    os << "image";
    break;
  default:
    os << "unknown";
  }

  return os;
}

BOOL SDPLazyParser::GetMediaAddress(
  MediaType type,
  SIPURI & address,
  PINDEX offSet
)
{
  if( IsEmpty() )
    return FALSE;
  
  PStringArray headers = Lines();

  PStringStream mLine;
  mLine << "m=" << type;

  PINDEX currentOffSet = 0;
  PINDEX targetMediaIndex = P_MAX_INDEX;
  PINDEX matchSize = mLine.GetLength();

  for( PINDEX i = 0; i < headers.GetSize(); i++ )
  {
    if( mLine *= headers[i].Left( matchSize ) )
    {
      if( currentOffSet == offSet )
      {
        targetMediaIndex = i;
        break;
      }else
        currentOffSet++;
    }
  }

  if( targetMediaIndex == P_MAX_INDEX )
    return FALSE;

  PStringArray tokens = headers[targetMediaIndex].Trim().Tokenise( ' ', FALSE );
  if( tokens.GetSize() >= 3 )
  {
    address.SetPort( tokens[1] );
    address.SetScheme( tokens[2] );
  }else
    return FALSE;

  PString connectionData;
  /// now look of there is a connection data available
  for( PINDEX j = targetMediaIndex + 1; j < headers.GetSize(); j++ )
  {
    if( headers[j].Left(2) *= "m=" )
      break;
    else if( headers[j].Left(2) *= "c=" )
    {
      connectionData = headers[j];
      break;
    }
  }

  /// not found in the annoucement
  /// try global headers
  if( connectionData.IsEmpty() )
  {
    for( PINDEX z = 0; z < headers.GetSize(); z++ )
    {
      if( headers[z].Left(2) *= "m=" )
        break;
      else if( headers[z].Left(2) *= "c=" )
      {
        connectionData = headers[z];
        break;
      }
    }
  }

  if( connectionData.IsEmpty() )
    return FALSE;

  tokens = connectionData.Trim().Tokenise( ' ', FALSE );

  if( tokens.GetSize() == 3 )
    address.SetHost( tokens[2] );
  else
    return FALSE;
  

  return TRUE;
}

PINDEX SDPLazyParser::GetMediaCount(
  MediaType type
)
{
  if( IsEmpty() )
    return FALSE;
  
  PStringArray headers = Lines();

  PStringStream mLine;
  mLine << "m=" << type;

  PINDEX matchSize = mLine.GetLength();
  PINDEX currentOffSet = 0;

  for( PINDEX i = 0; i < headers.GetSize(); i++ )
  {
    if( mLine *= headers[i].Left( matchSize ) )
       currentOffSet++;
  }

  return currentOffSet;
}

SDPLazyParser::MediaDirection SDPLazyParser::GetMediaDirection(
  MediaType type,
  PINDEX offSet
)
{
  return NumMediaDescription;
}

BOOL SDPLazyParser::SetMediaDirection(
  MediaDirection direction,
  MediaType type,
  PINDEX offSet
)
{
  return FALSE;
}

/*
v=0
o=user1 53655765 2353687637 IN IP4 128.3.4.5
s=Mbone Audio
i=Discussion of Mbone Engineering Issues
e=mbone@somewhere.com
c=IN IP4 224.2.0.1/127
t=3149328700 0
m=audio 3456 RTP/AVP 0 
a=rtpmap:0 PCMU/8000
*/

BOOL SDPLazyParser::GetMediaFormat(
  MediaType type,
  PINDEX mediaOffSet,
  PINDEX formatOffSet,
  int & payloadType,
  PString & formatName,
  SIPURI & address
)
{
  formatName = PString::Empty();

  if( IsEmpty() )
    return FALSE;
 
  PStringArray headers = Lines();

  PStringStream mLine;
  mLine << "m=" << type;

  PINDEX currentOffSet = 0;
  PINDEX targetMediaIndex = P_MAX_INDEX;
  PINDEX matchSize = mLine.GetLength();

  for( PINDEX i = 0; i < headers.GetSize(); i++ )
  {
    if( mLine *= headers[i].Left( matchSize ) )
    {
      if( currentOffSet == mediaOffSet )
      {
        targetMediaIndex = i;
        break;
      }else
        currentOffSet++;
    }
  }

  if( targetMediaIndex == P_MAX_INDEX )
    return FALSE;

  PStringArray tokens = headers[targetMediaIndex].Trim().Tokenise( ' ', FALSE );
  PBYTEArray formats;

  if( tokens.GetSize() >= 3 && tokens.GetSize() > formatOffSet + 3 )
  {
    address.SetPort( tokens[1] );
    address.SetScheme( tokens[2] );
    payloadType = (BYTE)tokens[ formatOffSet + 3 ].AsUnsigned();
  }else
    return FALSE;

  PString connectionData;
  /// now look of there is a connection data available
  /// and find the formatName
  PStringStream formatAttrMatch;
  formatAttrMatch << "a=rtpmap:" << payloadType;

  for( PINDEX j = targetMediaIndex + 1; j < headers.GetSize(); j++ )
  {
    PString currentHeader = headers[j];
    PString attrMatch = currentHeader.Left( formatAttrMatch.GetLength() );
    if( currentHeader.Left(2) *= "m=" )
      break;
    else if( currentHeader.Left(2) *= "c=" )
    {
      if( connectionData.IsEmpty() )
        connectionData = currentHeader;
    }else if(  attrMatch *= formatAttrMatch )
    {
      /// a=rtpmap:0 PCMU/8000
      tokens = currentHeader.Trim().Tokenise( ' ', FALSE );
      if( tokens.GetSize() > 1 )
      {
        tokens = tokens[1].Trim().Tokenise( '/', FALSE );
        formatName = tokens[0];
      }
    }
  }

  if( formatName.IsEmpty() && payloadType < SDPMediaFmt::LastKnownPayloadType )
    formatName = SDPMediaFmt::PayloadNames[payloadType];

  /// not found in the annoucement
  /// try global headers
  if( connectionData.IsEmpty() )
  {
    for( PINDEX z = 0; z < headers.GetSize(); z++ )
    {
      if( headers[z].Left(2) *= "m=" )
        break;
      else if( headers[z].Left(2) *= "c=" )
      {
        connectionData = headers[z];
        break;
      }
    }
  }

  if( !connectionData.IsEmpty() )
  {
    tokens = connectionData.Trim().Tokenise( ' ', FALSE );

    if( tokens.GetSize() == 3 )
      address.SetHost( tokens[2] );
  }

  
  return TRUE;
}

PINDEX SDPLazyParser::GetMediaFormatCount(
  MediaType type,
  PINDEX mediaOffSet
)
{
  if( IsEmpty() )
    return 0;
 
  PStringArray headers = Lines();

  PStringStream mLine;
  mLine << "m=" << type;

  PINDEX currentOffSet = 0;
  PINDEX targetMediaIndex = P_MAX_INDEX;
  PINDEX matchSize = mLine.GetLength();

  for( PINDEX i = 0; i < headers.GetSize(); i++ )
  {
    if( mLine *= headers[i].Left( matchSize ) )
    {
      if( currentOffSet == mediaOffSet )
      {
        targetMediaIndex = i;
        break;
      }else
        currentOffSet++;
    }
  }

  if( targetMediaIndex == P_MAX_INDEX )
    return 0;

  PString currentHeader = headers[targetMediaIndex];

  PStringArray tokens = currentHeader.Trim().Trim().Tokenise( ' ', FALSE );
  PBYTEArray formats;
  int ret = 0;
  if( tokens.GetSize() >= 3 )
    ret = tokens.GetSize() - 3;
 
  return ret;
}

BOOL SDPLazyParser::SetMediaAddress(
  MediaType type,
  PINDEX offSet,
  const PString & host,
  WORD port
)
{
  if( IsEmpty() )
    return FALSE;
  
  PStringArray headers = Lines();

  PStringStream mLine;
  mLine << "m=" << type;

  PINDEX currentOffSet = 0;
  PINDEX targetMediaIndex = P_MAX_INDEX;
  PINDEX matchSize = mLine.GetLength();

  for( PINDEX i = 0; i < headers.GetSize(); i++ )
  {
    if( mLine *= headers[i].Left( matchSize ) )
    {
      if( currentOffSet == offSet )
      {
        targetMediaIndex = i;
        break;
      }else
        currentOffSet++;
    }
  }

  if( targetMediaIndex == P_MAX_INDEX )
    return FALSE;

  PStringArray tokens = headers[targetMediaIndex].Trim().Tokenise( ' ', FALSE );
  if( tokens.GetSize() >= 3 )
  {
    tokens[1] = PString( port );
    PString newHeader;
    for( PINDEX i = 0; i < tokens.GetSize(); i++ )
      newHeader += tokens[i] + " ";
    headers[targetMediaIndex] = newHeader.Trim();
  }else
    return FALSE;

  PString connectionData;
  /// now look of there is a connection data available
  for( PINDEX j = targetMediaIndex + 1; j < headers.GetSize(); j++ )
  {
    if( headers[j].Left(2) *= "m=" )
      break;
    else if( headers[j].Left(2) *= "c=" )
    {
      connectionData = headers[j];
      tokens = connectionData.Trim().Tokenise( ' ', FALSE );
      if( tokens.GetSize() == 3 )
      {
        tokens[2] = host;
        PString newHeader;
        for( PINDEX i = 0; i < tokens.GetSize(); i++ )
          newHeader += tokens[i] + " ";
        headers[j] = newHeader;
      }else
        return FALSE;
    }
  }


  /// try global headers

  for( PINDEX z = 0; z < headers.GetSize(); z++ )
  {
    if( headers[z].Left(2) *= "m=" )
      break;
    else if( headers[z].Left(2) *= "c=" )
    {
      connectionData = headers[z];
      tokens = connectionData.Trim().Tokenise( ' ', FALSE );
      if( tokens.GetSize() == 3 )
      {
        tokens[2] = host;
        PString newHeader;
        for( PINDEX i = 0; i < tokens.GetSize(); i++ )
          newHeader += tokens[i] + " ";
        headers[z] = newHeader;
      }else
        return FALSE;
    }
  }

  if( connectionData.IsEmpty() )
    return FALSE;

  PStringStream newSDP;
  for( PINDEX i = 0; i < headers.GetSize(); i++ )
    newSDP << headers[i] << "\r\n";

  *this = newSDP;

  return TRUE;
}


BOOL SDPLazyParser::Read(
  PChannel & inputStream
)
{
  // Do this to force a Read() by the PChannelBuffer outside of the
  // ios::lock() mutex which would prevent simultaneous reads and writes.
  inputStream.SetReadTimeout( PMaxTimeInterval );
#if defined(__MWERKS__) || (__GNUC__ >= 3) || (_MSC_VER >= 1300) || defined(SOLARIS)
  if (inputStream.rdbuf()->pubseekoff(0, ios_base::cur) == streampos(-1))
#else
  if (inputStream.rdbuf()->seekoff(0, ios::cur, ios::in) == EOF)
#endif                  
    inputStream.clear(ios::badbit);

  if (!inputStream.IsOpen())
    return FALSE;

  // get the message from transport into cmd and parse MIME
  PString startLine;
  
  PStringStream strm;
 
  while(!inputStream.bad() && !inputStream.eof()) 
  {
    PString buff;
    inputStream.SetReadTimeout(3000);
    inputStream >> buff;
    strm << buff << "\r\n";
  }

  if (inputStream.bad()) 
  {
    PTRACE_IF(1, inputStream.GetErrorCode(PChannel::LastReadError) != PChannel::NoError,
              "SDP Read failed: " << inputStream.GetErrorText(PChannel::LastReadError));
    return FALSE;
  }
  
  *this = strm;

  return TRUE;
}



