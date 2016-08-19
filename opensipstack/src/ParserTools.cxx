/*
 *
 * ParserTools.cxx
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
 * $Log: ParserTools.cxx,v $
 * Revision 1.24  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.23  2006/11/10 08:01:29  rcolobong
 * Add property method to configure XOR Hash keys
 *
 * Revision 1.22  2006/10/23 15:44:58  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.21  2006/10/17 00:12:24  joegenbaclor
 * Added support for RFC 4028 SIP Session Timers header
 *
 * Revision 1.20  2006/08/18 07:12:24  joegenbaclor
 * 1.  added Function overload for GetContentType
 * 2.  Changed unix make files to not include opensbc in make process
 *
 * Revision 1.19  2006/08/11 08:38:42  joegenbaclor
 * Initial upload of Presence package
 *
 * Revision 1.18  2006/07/24 07:33:56  joegenbaclor
 * Minor bug fixes allover
 *
 * Revision 1.17  2006/07/17 11:36:45  joegenbaclor
 * 1.  More routing enhancements to B2BUA
 *
 * Revision 1.16  2006/07/13 06:53:05  joegenbaclor
 * 1.  Introduced Sanity checks for incoming SIP Message
 * 2.  Corrected bug in SIPURI where enum scheme is not getting recognized.
 * 3.  Added ENUM support to routing
 * 4.  Introduced global routing flag to indicate when and not to rewrite outbound To URI's
 *
 * Revision 1.15  2006/07/10 06:29:38  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.14  2006/07/06 05:38:11  joegenbaclor
 * Final last minute changes to CBCOM hash support
 *
 * Revision 1.13  2006/07/04 02:30:41  joegenbaclor
 * Removed xor variable becaseu it seems to be a reserved word in GCC
 *
 * Revision 1.12  2006/07/04 01:37:55  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.11  2006/07/04 01:31:42  joegenbaclor
 * Changed BYTE to unsigned char in XOR functions
 *
 * Revision 1.10  2006/07/03 15:10:05  joegenbaclor
 * Added Grandstream CBCom support for basic XOR hash
 *
 * Revision 1.9  2006/06/30 07:12:21  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.8  2006/06/09 07:37:30  joegenbaclor
 * 1.  Added Publish, SIPETag & SIPIfMatch  support in SIPMessage
 * 2.  Started work on StatusPage Resource for OSSAppConfig
 *
 * Revision 1.7  2006/05/31 07:21:52  joegenbaclor
 * 1.  Added OSSApplication and StringTuple classes.
 * 2.  Corrected some typos
 *
 * Revision 1.6  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.5  2006/04/21 09:55:10  joegenbaclor
 * Added RFC 3265 header requirements
 *
 * Revision 1.4  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include <ptlib.h>
#include <ptlib/sockets.h>
#include <ptclib/random.h>

#include "ParserTools.h"

#define new PNEW

using namespace SIPParser;

PSortedStringList ParserTools::m_RegisteredHeaders;
BOOL ParserTools::m_RegisteredHeaderInitialize = FALSE;
PMutex ParserTools::m_RegisteredHeadersMutex;
WORD ParserTools::m_DefaultHashKey1 = GS_HASH_KEY_1;
BYTE ParserTools::m_DefaultHashKey2 = GS_HASH_KEY_2;


BOOL ParserTools::IsKnownHeader(
  const PString & header
)
{
  PString h = header.ToLower();

  if( !ParserTools::m_RegisteredHeaderInitialize )
  { 
    PWaitAndSignal lock(ParserTools::m_RegisteredHeadersMutex);
    
    ///ParserTools::m_RegisteredHeaders.AppendString(
    ParserTools::m_RegisteredHeaders.AppendString( "accept" );
    ParserTools::m_RegisteredHeaders.AppendString( "accept-encoding" );
    ParserTools::m_RegisteredHeaders.AppendString( "accept-language" );
    ParserTools::m_RegisteredHeaders.AppendString( "alert-info" );
    ParserTools::m_RegisteredHeaders.AppendString( "allow" );
    ParserTools::m_RegisteredHeaders.AppendString( "authentication-info" );
    ParserTools::m_RegisteredHeaders.AppendString( "authorization" );
    ParserTools::m_RegisteredHeaders.AppendString( "call-id" );
    ParserTools::m_RegisteredHeaders.AppendString( "call-info" );
    ParserTools::m_RegisteredHeaders.AppendString( "contact" );
    ParserTools::m_RegisteredHeaders.AppendString( "content-disposition" );
    ParserTools::m_RegisteredHeaders.AppendString( "content-encoding" );
    ParserTools::m_RegisteredHeaders.AppendString( "content-language" );
    ParserTools::m_RegisteredHeaders.AppendString( "content-length" );
    ParserTools::m_RegisteredHeaders.AppendString( "content-type" );
    ParserTools::m_RegisteredHeaders.AppendString( "cseq" );
    ParserTools::m_RegisteredHeaders.AppendString( "date" );
    ParserTools::m_RegisteredHeaders.AppendString( "error-info" );
    ParserTools::m_RegisteredHeaders.AppendString( "expires" );
    ParserTools::m_RegisteredHeaders.AppendString( "from" );
    ParserTools::m_RegisteredHeaders.AppendString( "in-reply-to" );
    ParserTools::m_RegisteredHeaders.AppendString( "max-forwards" );
    ParserTools::m_RegisteredHeaders.AppendString( "min-expires" );
    ParserTools::m_RegisteredHeaders.AppendString( "mime-version" );
    ParserTools::m_RegisteredHeaders.AppendString( "organization" );
    ParserTools::m_RegisteredHeaders.AppendString( "priority" );
    ParserTools::m_RegisteredHeaders.AppendString( "proxy-authenticate" );
    ParserTools::m_RegisteredHeaders.AppendString( "proxy-authorization" );
    ParserTools::m_RegisteredHeaders.AppendString( "proxy-require" );
    ParserTools::m_RegisteredHeaders.AppendString( "record-route" );
    ParserTools::m_RegisteredHeaders.AppendString( "reply-to" );
    ParserTools::m_RegisteredHeaders.AppendString( "require" );
    ParserTools::m_RegisteredHeaders.AppendString( "retry-after" );
    ParserTools::m_RegisteredHeaders.AppendString( "route" );
    ParserTools::m_RegisteredHeaders.AppendString( "server" );
    ParserTools::m_RegisteredHeaders.AppendString( "subject" );
    ParserTools::m_RegisteredHeaders.AppendString( "supported" );
    ParserTools::m_RegisteredHeaders.AppendString( "TimeStamp" );
    ParserTools::m_RegisteredHeaders.AppendString( "to" );
    ParserTools::m_RegisteredHeaders.AppendString( "unsupported" );
    ParserTools::m_RegisteredHeaders.AppendString( "user-agent" );
    ParserTools::m_RegisteredHeaders.AppendString( "via" );
    ParserTools::m_RegisteredHeaders.AppendString( "warning" );
    ParserTools::m_RegisteredHeaders.AppendString( "www-authenticate" );

    /// extension headers
    ParserTools::m_RegisteredHeaders.AppendString( "allow-events" );
    ParserTools::m_RegisteredHeaders.AppendString( "event" );
    ParserTools::m_RegisteredHeaders.AppendString( "refer-to" );
    ParserTools::m_RegisteredHeaders.AppendString( "subscription-state" );
    ParserTools::m_RegisteredHeaders.AppendString( "sip-etag" );
    ParserTools::m_RegisteredHeaders.AppendString( "sip-if-match" );

    /// session timers
    ParserTools::m_RegisteredHeaders.AppendString( "session-expires" );
    ParserTools::m_RegisteredHeaders.AppendString( "min-se" );

    ParserTools::m_RegisteredHeaderInitialize = TRUE;
  }

  return ParserTools::m_RegisteredHeaders.GetStringsIndex( h ) != P_MAX_INDEX;
}

PString ParserTools::LineFeedSanityCheck( 
  const PString & buff 
)
{
  PString pdu, sbuff;
  PStringStream newPDU;
  pdu = sbuff = PString( buff );
  PINDEX i = 0;
  

  /** Get rid of trailing spaces and carriage returns */

  for( i = 0; i < sbuff.GetLength(); i++ )
  {
    char c = sbuff[i];
    if( ::isspace( c ) )
    {
      pdu = pdu.Right( pdu.GetLength() - 1 );
    }else{
      break;
    }
  }

  PStringArray tokens = pdu.Tokenise( "\n" );

  for( PINDEX i = 0; i < tokens.GetSize(); i++ )
  {
    tokens[i] = tokens[i].Trim();
    tokens[i] = tokens[i] + "\r\n";
  }


  int linefeedCount = 0;
  int packetEnd = 0;
  for( PINDEX j = tokens.GetSize() -1; j >= 0; j-- )
  {
    if( tokens[j].Trim().IsEmpty() )
      linefeedCount++;
    else
    {
      packetEnd = j;
      break;
    }
  }

  PStringStream newPacket;

  for( PINDEX z = 0; z <= packetEnd; z++ )
    newPacket << tokens[z];

  newPacket << "\r\n";

  return newPacket;

}


void ParserTools::SliceMIME(
  const PString & mime,
  PString & name,
  PString & value
)
{
  PINDEX colon = mime.Find( ": " );

  if( colon != P_MAX_INDEX )
  {
    name = mime.Left( colon );
    value = mime.Mid( colon + 2 );
  }else
  {
    colon = mime.Find( ":" );
    if( colon != P_MAX_INDEX )
    {
      name = mime.Left( colon );
      if( ParserTools::IsKnownHeader( name ) )
        value = mime.Mid( colon + 1 );
      else
      {
        name = "";
        value = mime;
      }
    }else
    {
      value = mime;
    }
  }
}


BOOL ParserTools::IsNumeric(
  const PString & value
)
{
  for( PINDEX j = 0; j < value.GetSize(); j++ )
    if( ::isalpha( value[j] ) != 0 )
      return FALSE;

  return TRUE;
}

PString ParserTools::EscapeAsRFC2396( 
  const PString & str 
)
{
  PString xlat = str;

  PString safeChars = "abcdefghijklmnopqrstuvwxyz"
                      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                      "0123456789$-_.!*'(),";

  PINDEX pos = (PINDEX)-1;

  while ((pos += (int)(1+strspn(&xlat[pos+1], safeChars))) < xlat.GetLength())
    xlat.Splice(psprintf("%%%02X", (BYTE)xlat[pos]), pos, 1);

  return xlat;
}


PString ParserTools::UnescapeAsRFC2396(
  const PString & str
)
{
  PString xlat = str;
  xlat.MakeUnique();

  PINDEX pos;


  pos = (PINDEX)-1;
  while ((pos = xlat.Find('%', pos+1)) != P_MAX_INDEX) {
    int digit1 = xlat[pos+1];
    int digit2 = xlat[pos+2];
    if (isxdigit(digit1) && isxdigit(digit2)) {
      xlat[pos] = (char)(
            (isdigit(digit2) ? (digit2-'0') : (toupper(digit2)-'A'+10)) +
           ((isdigit(digit1) ? (digit1-'0') : (toupper(digit1)-'A'+10)) << 4));
      xlat.Delete(pos+1, 2);
    }
  }

  return xlat;
}


PString ParserTools::GenGUID()
{
  PBYTEArray guidArray(16);

  // Want time of UTC in 0.1 microseconds since 15 Oct 1582.
  PInt64 TimeStamp;
  static PInt64 deltaTime = PInt64(10000000)*24*60*60*
                            (  16            // Days from 15th October
                             + 31            // Days in December 1583
                             + 30            // Days in November 1583
#ifdef _WIN32
                             + (1601-1583)*365   // Whole years
                             + (1601-1583)/4);   // Leap days

  // Get nanoseconds since 1601
#ifndef _WIN32_WCE
  GetSystemTimeAsFileTime((LPFILETIME)&TimeStamp);
#else
  SYSTEMTIME SystemTime;
  GetSystemTime(&SystemTime);
  SystemTimeToFileTime(&SystemTime, (LPFILETIME)&TimeStamp);
#endif // _WIN32_WCE

  TimeStamp /= 100;
#else // _WIN32
                             + (1970-1583)*365 // Days in years
                             + (1970-1583)/4   // Leap days
                             - 3);             // Allow for 1700, 1800, 1900 not leap years

#ifdef P_VXWORKS
  struct timespec ts;
  clock_gettime(0,&ts);
  TimeStamp = (ts.tv_sec*(PInt64)1000000 + ts.tv_nsec*1000)*10;
#else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  TimeStamp = (tv.tv_sec*(PInt64)1000000 + tv.tv_usec)*10;
#endif // P_VXWORKS
#endif // _WIN32

  TimeStamp += deltaTime;

  guidArray[0] = (BYTE)(TimeStamp&0xff);
  guidArray[1] = (BYTE)((TimeStamp>>8)&0xff);
  guidArray[2] = (BYTE)((TimeStamp>>16)&0xff);
  guidArray[3] = (BYTE)((TimeStamp>>24)&0xff);
  guidArray[4] = (BYTE)((TimeStamp>>32)&0xff);
  guidArray[5] = (BYTE)((TimeStamp>>40)&0xff);
  guidArray[6] = (BYTE)((TimeStamp>>48)&0xff);
  guidArray[7] = (BYTE)(((TimeStamp>>56)&0x0f) + 0x10);  // Version number is 1

  static WORD clockSequence = (WORD)PRandom::Number();
  static PInt64 lastTimeStamp = 0;
  if (lastTimeStamp < TimeStamp)
    lastTimeStamp = TimeStamp;
  else
    clockSequence++;

  guidArray[8] = (BYTE)(((clockSequence>>8)&0x1f) | 0x80); // DCE compatible GUID
  guidArray[9] = (BYTE)clockSequence;

  static PEthSocket::Address macAddress;
  static BOOL needMacAddress = TRUE;

  if( needMacAddress )
  {
    PRandom rand;
    macAddress.ls.l = rand;
    macAddress.ls.s = (WORD)rand;
    macAddress.b[0] |= '\x80';
    needMacAddress = FALSE;
    
  }

  memcpy(guidArray.GetPointer()+10, macAddress.b, 6);

  PStringStream strm;
  char fillchar = strm.fill();
  strm << hex << setfill('0')
       << setw(2) << (unsigned)(BYTE)guidArray[0]
       << setw(2) << (unsigned)(BYTE)guidArray[1]
       << setw(2) << (unsigned)(BYTE)guidArray[2]
       << setw(2) << (unsigned)(BYTE)guidArray[3] << '-'
       << setw(2) << (unsigned)(BYTE)guidArray[4]
       << setw(2) << (unsigned)(BYTE)guidArray[5] << '-'
       << setw(2) << (unsigned)(BYTE)guidArray[6]
       << setw(2) << (unsigned)(BYTE)guidArray[7] << '-'
       << setw(2) << (unsigned)(BYTE)guidArray[8]
       << setw(2) << (unsigned)(BYTE)guidArray[9] << '-'
       << setw(2) << (unsigned)(BYTE)guidArray[10]
       << setw(2) << (unsigned)(BYTE)guidArray[11]
       << setw(2) << (unsigned)(BYTE)guidArray[12]
       << setw(2) << (unsigned)(BYTE)guidArray[13]
       << setw(2) << (unsigned)(BYTE)guidArray[14]
       << setw(2) << (unsigned)(BYTE)guidArray[15]
       << dec << setfill(fillchar);

  return strm;
}

PString ParserTools::GetRFC1123Date()
{
  /// Date: Sat, 13 Nov 2010 23:29:00 GMT
  PTime now;
  return now.AsString( PTime::RFC1123, PTime::GMT );
}

PString ParserTools::GenTagParameter()
{
  PBYTEArray guidArray(16);

  // Want time of UTC in 0.1 microseconds since 15 Oct 1582.
  PInt64 TimeStamp;
  static PInt64 deltaTime = PInt64(10000000)*24*60*60*
                            (  16            // Days from 15th October
                             + 31            // Days in December 1583
                             + 30            // Days in November 1583
#ifdef _WIN32
                             + (1601-1583)*365   // Whole years
                             + (1601-1583)/4);   // Leap days

  // Get nanoseconds since 1601
#ifndef _WIN32_WCE
  GetSystemTimeAsFileTime((LPFILETIME)&TimeStamp);
#else
  SYSTEMTIME SystemTime;
  GetSystemTime(&SystemTime);
  SystemTimeToFileTime(&SystemTime, (LPFILETIME)&TimeStamp);
#endif // _WIN32_WCE

  TimeStamp /= 100;
#else // _WIN32
                             + (1970-1583)*365 // Days in years
                             + (1970-1583)/4   // Leap days
                             - 3);             // Allow for 1700, 1800, 1900 not leap years

#ifdef P_VXWORKS
  struct timespec ts;
  clock_gettime(0,&ts);
  TimeStamp = (ts.tv_sec*(PInt64)1000000 + ts.tv_nsec*1000)*10;
#else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  TimeStamp = (tv.tv_sec*(PInt64)1000000 + tv.tv_usec)*10;
#endif // P_VXWORKS
#endif // _WIN32

  TimeStamp += deltaTime;

  guidArray[0] = (BYTE)(TimeStamp&0xff);
  guidArray[1] = (BYTE)((TimeStamp>>8)&0xff);
  guidArray[2] = (BYTE)((TimeStamp>>16)&0xff);
  guidArray[3] = (BYTE)((TimeStamp>>24)&0xff);
  guidArray[4] = (BYTE)((TimeStamp>>32)&0xff);
  guidArray[5] = (BYTE)((TimeStamp>>40)&0xff);
  guidArray[6] = (BYTE)((TimeStamp>>48)&0xff);
  guidArray[7] = (BYTE)(((TimeStamp>>56)&0x0f) + 0x10);  // Version number is 1

  static WORD clockSequence = (WORD)PRandom::Number();
  static PInt64 lastTimeStamp = 0;
  if (lastTimeStamp < TimeStamp)
    lastTimeStamp = TimeStamp;
  else
    clockSequence++;

  guidArray[8] = (BYTE)(((clockSequence>>8)&0x1f) | 0x80); // DCE compatible GUID
  guidArray[9] = (BYTE)clockSequence;

  static PEthSocket::Address macAddress;
  static BOOL needMacAddress = TRUE;

  if( needMacAddress )
  {
    PRandom rand;
    macAddress.ls.l = rand;
    macAddress.ls.s = (WORD)rand;
    macAddress.b[0] |= '\x80';
    needMacAddress = FALSE;
    
  }

  memcpy(guidArray.GetPointer()+10, macAddress.b, 6);

  PStringStream strm;
  char fillchar = strm.fill();
  strm << hex << setfill('0')
       << setw(2) << (unsigned)(BYTE)guidArray[0]
       << setw(2) << (unsigned)(BYTE)guidArray[1]
       << setw(2) << (unsigned)(BYTE)guidArray[2]
       << setw(2) << (unsigned)(BYTE)guidArray[3] 
       << setw(2) << (unsigned)(BYTE)guidArray[4]
       << setw(2) << (unsigned)(BYTE)guidArray[5] 
       << setw(2) << (unsigned)(BYTE)guidArray[6]
       << setw(2) << (unsigned)(BYTE)guidArray[7] 
       << setw(2) << (unsigned)(BYTE)guidArray[8]
       << setw(2) << (unsigned)(BYTE)guidArray[9] 
       << setw(2) << (unsigned)(BYTE)guidArray[10]
       << setw(2) << (unsigned)(BYTE)guidArray[11]
       << setw(2) << (unsigned)(BYTE)guidArray[12]
       << setw(2) << (unsigned)(BYTE)guidArray[13]
       << setw(2) << (unsigned)(BYTE)guidArray[14]
       << setw(2) << (unsigned)(BYTE)guidArray[15]
       << dec << setfill(fillchar);

  return strm;
}

PString ParserTools::GenBranchParameter()
{
  static const PString magicCookie = "z9hG4bK";
  return magicCookie + ParserTools::GenTagParameter();
}

PString ParserTools::GetReasonPhrase( 
  WORD statusCode 
)
{
  
  static PString s100("Trying");
  static PString s180("Ringing");
  static PString s181("Call Is Being Forwarded");
  static PString s182("Queued");
  static PString s183("Session Progress");
  static PString s200("OK");
  static PString s202("Accepted");
  static PString s300("Multiple Choices");
  static PString s301("Moved Permanently");
  static PString s302("Moved Temporarily");
  static PString s305("Use Proxy");
  static PString s380("Alternative Service");
  static PString s400("Bad Request");
  static PString s401("Unauthorized");
  static PString s402("Payment Required");
  static PString s403("Forbidden");
  static PString s404("Not Found");
  static PString s405("Method Not Allowed");
  static PString s406("Not Acceptable");
  static PString s407("Proxy Authentication Required");
  static PString s408("Request Timeout");
  static PString s409("Conflict");
  static PString s410("Gone");
  static PString s411("Length Required");
  static PString s412("Conditional Request Failed");
  static PString s413("Request Entity Too Large");
  static PString s414("Request-URI Too Large");
  static PString s415("Unsupported Media Type");
  static PString s416("Unsupported Uri Scheme");
  static PString s420("Bad Extension");
  static PString s422("Session Interval Too Small" );
  static PString s423("Interval Too Short");
  static PString s480("Temporarily not available");
  static PString s481("Call Leg/Transaction Does Not Exist");
  static PString s482("Loop Detected");
  static PString s483("Too Many Hops");
  static PString s484("Address Incomplete");
  static PString s485("Ambiguous");
  static PString s486("Busy Here");
  static PString s487("Request Cancelled");
  static PString s488("Not Acceptable Here");
  static PString s489("Bad Event");
  static PString s500("Internal Server Error");
  static PString s501("Not Implemented");
  static PString s502("Bad Gateway");
  static PString s503("Service Unavailable");
  static PString s504("Gateway Time-out");
  static PString s505("SIP Version not supported");
  static PString s600("Busy Everywhere");
  static PString s603("Decline");
  static PString s604("Does not exist anywhere");
  static PString s606("Not Acceptable");

  static PString s999( "Reason Unknown" );

  int i = statusCode / 100;

  if (i == 1)
  {				/* 1xx  */
    if (statusCode == 100)
      return s100;
    if (statusCode == 180)
      return s180;
    if (statusCode == 181)
      return s181;
    if (statusCode == 182)
      return s182;
    if (statusCode == 183)
      return s183;
  }else if (i == 2)
  {	if (statusCode == 200)
      return s200;
    if (statusCode == 202)
      return s202;
  }else if (i == 3)
  {				/* 3xx */
    if (statusCode == 300)
      return s300;
    if (statusCode == 301)
      return s301;
    if (statusCode == 302)
      return s302;
    if (statusCode == 305)
      return s305;
    if (statusCode == 380)
      return s380;
  }else if (i == 4)
  {				/* 4xx */
    if (statusCode == 400)
      return s400;
    if (statusCode == 401)
      return s401;
    if (statusCode == 402)
      return s402;
    if (statusCode == 403)
      return s403;
    if (statusCode == 404)
      return s404;
    if (statusCode == 405)
      return s405;
    if (statusCode == 406)
      return s406;
    if (statusCode == 407)
      return s407;
    if (statusCode == 408)
      return s408;
    if (statusCode == 409)
      return s409;
    if (statusCode == 410)
      return s410;
    if (statusCode == 411)
      return s411;
    if (statusCode == 413)
      return s413;
    if (statusCode == 414)
      return s414;
    if (statusCode == 415)
      return s415;
    if (statusCode == 416)
      return s416;
    if (statusCode == 420)
      return s420;
    if (statusCode == 422)
      return s422;
    if (statusCode == 423)
      return s423;
    if (statusCode == 480)
      return s480;
    if (statusCode == 481)
      return s481;
    if (statusCode == 482)
      return s482;
    if (statusCode == 483)
      return s483;
    if (statusCode == 484)
      return s484;
    if (statusCode == 485)
      return s485;
    if (statusCode == 486)
      return s486;
    if (statusCode == 487)
      return s487;
    if (statusCode == 488)
      return s488;
    if (statusCode == 489)
      return s489;
  }else if (i == 5)
  {				/* 5xx */
    if (statusCode == 500)
      return s500;
    if (statusCode == 501)
      return s501;
    if (statusCode == 502)
      return s502;
    if (statusCode == 503)
      return s503;
    if (statusCode == 504)
      return s504;
    if (statusCode == 505)
      return s505;
  }else if (i == 6)
  {				/* 6xx */
    if (statusCode == 600)
      return s600;
    if (statusCode == 603)
      return s603;
    if (statusCode == 604)
      return s604;
    if (statusCode == 606)
      return s606;
  }

  return s999;
}

BOOL ParserTools::MsgTokenize(
  const PString & msg,
  PStringArray & finalTokens
)
{
  PStringArray tokens;

  PINDEX lastOffSet  = 0;
  PINDEX offSet = 0;
  BOOL ok = FALSE;

  ///got start of message
  BOOL gotSOM = FALSE;

  BOOL crLFCompliant = msg.Find( "\r\n" ) != P_MAX_INDEX;

  for(;;)
  {
    lastOffSet = offSet;

    if( !crLFCompliant )
      offSet = msg.FindOneOf( "\r\n", offSet );
    else
      offSet = msg.Find( "\r\n", offSet );
    
    if( offSet != P_MAX_INDEX )
    {
      //
      //  lastOffSet++;
      PString token = msg.Mid( lastOffSet, ++offSet - lastOffSet );

      
      char firstChar = token[1];
      token = token.Trim();

      if( ::isspace( firstChar ) && !token.IsEmpty() )
        token = "  " + token;

      if( !gotSOM && token.IsEmpty() )
        continue;
      else
      {
        tokens.AppendString( token );
        gotSOM = TRUE;
      }

      ok = TRUE;
    }else
    {
      break;
    }


  }

      /// now check if there are tokens that has embedded CRLF

    PString buff, buff2;
    for( PINDEX i = 0; i < tokens.GetSize(); i++ )
    {
      buff2 = PString::Empty();
      buff += tokens[i];

      if( i + 1 == tokens.GetSize() )
      {
        finalTokens.AppendString( buff );
        break;
      }

      buff2 = tokens[i+1];
      char firstChar = buff2[0];
      if( ::isspace( firstChar ) )
      {
        buff += "\r\n";
      }else
      {
        buff = buff.Trim();
        finalTokens.AppendString( buff );
        buff = PString::Empty();
      }
    }

  return ok;
}

BOOL ParserTools::MsgTokenize(
  const PBYTEArray & _bytes,
  PStringArray & tokens
)
{
  PBYTEArray bytes = _bytes;
  bytes.SetSize( _bytes.GetSize() + 1 );
  bytes[_bytes.GetSize()]='\0';
  PString msg( (char*)bytes.GetPointer() );
  msg.MakeUnique();
  return ParserTools::MsgTokenize( msg, tokens );
}

PString ParserTools::GetExpandedHeader(
  const PString & compactHeader
)
{
  PString h = compactHeader.ToLower();

  if( h == "a" )
    return "Accept-Contact";
  else if( h == "b" )
    return "Referred-By";
  else if( h == "c" )
    return "Content-Type";
  else if( h == "e" )
    return "Content-Encoding";
  else if( h == "f" )
    return "From";
  else if( h == "i" )
    return "Call-ID";
  else if( h == "k" )
    return "Supported";
  else if( h == "l" )
    return "Content-Length";
  else if( h == "m" )
    return "Contact";
  else if( h == "o" )
    return "Event";
  else if( h == "r" )
    return "Refer-To";
  else if( h == "s" )
    return "Subject";
  else if( h == "t" )
    return "To";
  else if( h == "u" )
    return "Allow-Events";
  else if( h == "v" )
    return "Via";

  return compactHeader;
}

PString ParserTools::GetCompactedHeader(
  const PString & expandedHeader
)
{
  PString h = expandedHeader.ToLower();

  if( h == "accept-contact" )
    return "a";
  else if( h == "referred-by" )
    return "b";
  else if( h == "content-type" )
    return "c";
  else if( h == "content-encoding" )
    return "e";
  else if( h == "from" )
    return "f";
  else if( h == "call-id" )
    return "i";
  else if( h == "supported" )
    return "k";
  else if( h == "content-length" )
    return "l";
  else if( h == "contact" )
    return "m";
  else if( h == "event" )
    return "o";
  else if( h == "refer-to" )
    return "r";
  else if( h == "subject" )
    return "s";
  else if( h == "to" )
    return "t";
  else if( h == "allow-events" )
    return "u";
  else if( h == "via" )
    return "v";

  return expandedHeader;

}


PString ParserTools::AsHex(const PMessageDigest5::Code & digest)
{
  PStringStream out;
  out << hex << setfill('0');
  for (PINDEX i = 0; i < 16; i++)
    out << setw(2) << (unsigned)((BYTE *)&digest)[i];
  return out;
}

PString ParserTools::Quote( const PString & str )
{
  PStringStream strm;
  strm << "\"" << str << "\"";
  return strm;
}

PString ParserTools::UnQuote( PString & str )
{
  str.Replace( '"', "", TRUE );
  return str;
}

BOOL ParserTools::WildCardCompare(
  const char *wild, 
  const char *string
) 
{
  
  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}

void ParserTools::EndianSwap(WORD& x)
{
    x = (x>>8) | 
        (x<<8);
}

void ParserTools::EndianSwap(DWORD& x)
{
    int i = (x>>24) | 
        ((x<<8) & 0x00FF0000) |
        ((x>>8) & 0x0000FF00) |
        (x<<24);

    x = (DWORD)i;
}

#ifdef P_HAS_INT64
void ParserTools::EndianSwap(PUInt64& x)
{
    x = (x>>56) | 
        ((x<<40) & 0x00FF000000000000) |
        ((x<<24) & 0x0000FF0000000000) |
        ((x<<8)  & 0x000000FF00000000) |
        ((x>>8)  & 0x00000000FF000000) |
        ((x>>24) & 0x0000000000FF0000) |
        ((x>>40) & 0x000000000000FF00) |
        (x<<56);
}
#endif

BOOL ParserTools::XORHash(
  const PBYTEArray & in,
  PBYTEArray & out,
  WORD twoByteKey,
  BYTE oneByteKey,
  BOOL endianSwap
)
{
  BYTE x[2];
  PINDEX j = 0;
  WORD decode = 0;
  out.SetSize(0);

  if( endianSwap )
     ParserTools::EndianSwap( twoByteKey );

  for( PINDEX i = 0; i < in.GetSize(); i+=2 )
  {

      x[0] = in[i];
      x[1] = in[i+1];
      decode = *(WORD*)&x;
      decode = decode ^ twoByteKey;
      
      out[j++] = (BYTE)decode;
      out[j++] = (BYTE)(decode >> 8);

      if( i + 2 == in.GetSize() - 1 )
      {
        BYTE last = in[i+2];
        decode = (*(WORD*)&last) ^ oneByteKey;
        out[j++] = (BYTE)decode;
        break;
      }
  }

  return out.GetSize();
}

void ParserTools::SetDefaultXORHashKey1(
  WORD twoByteKey
)
{
  m_DefaultHashKey1 = twoByteKey;
}

WORD ParserTools::GetDefaultXORHashKey1()
{
  return m_DefaultHashKey1;
}

void ParserTools::SetDefaultXORHashKey2(
  BYTE oneByteKey
)
{
  m_DefaultHashKey2 = oneByteKey;
}

BYTE ParserTools::GetDefaultXORHashKey2()
{
  return m_DefaultHashKey2;
}

PString ParserTools::StripHTMLTags( 
  const PString & htmlPart 
)
{

  PString plainText;
  BOOL insideTag = FALSE;

  for( PINDEX i = 0; i < htmlPart.GetLength(); i++ )
  {
    PString c = htmlPart[i];
    if( c == "<" )
      insideTag = TRUE;
    else if( c == ">" )
      insideTag = FALSE;
    else
      if( !insideTag )
      {
        if( ::isspace( c[0] ) )
          c == " ";

        if( c[0] == '\r' || c[0] == '\n' )
          c = "";

        plainText += c;
      }
  }

  return plainText.Trim();
}

