/*
 *
 * MessageHeaders.h
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
 * $Log: MessageHeaders.h,v $
 * Revision 1.13  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.12  2006/11/03 09:43:27  joegenbaclor
 * Some modification on Event header to support optional parameter parsing
 *
 * Revision 1.11  2006/11/02 22:53:46  joegenbaclor
 * Added Path header support (RFC 3327)
 *
 * Revision 1.10  2006/10/23 15:44:54  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.9  2006/10/12 08:41:22  joegenbaclor
 * Added support for RFC 4028 headers
 *
 * Revision 1.8  2006/08/09 09:32:57  joegenbaclor
 * 1.  Added Privacy header support
 * 2.  Reintroduced VC++ 7.1 Project Files
 *
 * Revision 1.7  2006/06/09 07:37:30  joegenbaclor
 * 1.  Added Publish, SIPETag & SIPIfMatch  support in SIPMessage
 * 2.  Started work on StatusPage Resource for OSSAppConfig
 *
 * Revision 1.6  2006/04/26 00:45:06  joegenbaclor
 * Modified Expires to accept PTimeInterval as constructor parameter
 *
 * Revision 1.5  2006/04/21 09:55:09  joegenbaclor
 * Added RFC 3265 header requirements
 *
 * Revision 1.4  2006/04/21 03:55:21  joegenbaclor
 * Added ReferTo header
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef MESSAGEHEADERS_H
#define MESSAGEHEADERS_H

#include <ptlib.h>
#include "ParserTools.h"
#include "SIPHeader.h"
#include "SIPHeaderB.h"
#include "SIPHeaderC.h"
#include "SIPHeaderD.h"
#include "SIPHeaderE.h"
#include "SIPHeaderF.h"

namespace SIPParser
{
  /// Standad SIP Headers
  ///DECLARE_STANDARD_SIP_HEADER( AlertInfo );
  class AlertInfo : public SIPHeader
  {
    PCLASSINFO( AlertInfo, SIPHeader );
  public:
    AlertInfo();
    AlertInfo( const PString & body );
    AlertInfo( const MimeHeader & value );
    AlertInfo( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( CallId );
  class CallId : public SIPHeader
  {
    PCLASSINFO( CallId, SIPHeader );
  public:
    CallId();
    CallId( const PString & body );
    CallId( const MimeHeader & value );
    CallId( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( CallInfo );
  class CallInfo : public SIPHeader
  {
    PCLASSINFO( CallInfo, SIPHeader );
  public:
    CallInfo();
    CallInfo( const PString & body );
    CallInfo( const MimeHeader & value );
    CallInfo( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( ContentDisposition );
  class ContentDisposition : public SIPHeader
  {
    PCLASSINFO( ContentDisposition, SIPHeader );
  public:
    ContentDisposition();
    ContentDisposition( const PString & body );
    ContentDisposition( const MimeHeader & value );
    ContentDisposition( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( ContentLanguage );
  class ContentLanguage : public SIPHeader
  {
    PCLASSINFO( ContentLanguage, SIPHeader );
  public:
    ContentLanguage();
    ContentLanguage( const PString & body );
    ContentLanguage( const MimeHeader & value );
    ContentLanguage( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( ContentLength );
  class ContentLength : public SIPHeader
  {
    PCLASSINFO( ContentLength, SIPHeader );
  public:
    ContentLength();
    ContentLength( const PString & body );
    ContentLength( const MimeHeader & value );
    ContentLength( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( ContentType );
  class ContentType : public SIPHeader
  {
    PCLASSINFO( ContentType, SIPHeader );
  public:
    ContentType();
    ContentType( const PString & body );
    ContentType( const MimeHeader & value );
    ContentType( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( Date );
  class Date : public SIPHeader
  {
    PCLASSINFO( Date, SIPHeader );
  public:
    Date();
    Date( const PString & body  );
    Date( const MimeHeader & value );
    Date( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( ErrorInfo );
  class ErrorInfo : public SIPHeader
  {
    PCLASSINFO( ErrorInfo, SIPHeader );
  public:
    ErrorInfo();
    ErrorInfo( const PString & body );
    ErrorInfo( const MimeHeader & value );
    ErrorInfo( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( MIMEVersion );
  class MIMEVersion : public SIPHeader
  {
    PCLASSINFO( MIMEVersion, SIPHeader );
  public:
    MIMEVersion();
    MIMEVersion( const PString & body );
    MIMEVersion( const MimeHeader & value );
    MIMEVersion( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( Organization );
  class Organization : public SIPHeader
  {
    PCLASSINFO( Organization, SIPHeader );
  public:
    Organization();
    Organization( const PString & body );
    Organization( const MimeHeader & value );
    Organization( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( Priority );
  class Priority : public SIPHeader
  {
    PCLASSINFO( Priority, SIPHeader );
  public:
    Priority();
    Priority( const PString & body );
    Priority( const MimeHeader & value );
    Priority( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( RetryAfter );
  class RetryAfter : public SIPHeader
  {
    PCLASSINFO( RetryAfter, SIPHeader );
  public:
    RetryAfter();
    RetryAfter( const PString & body );
    RetryAfter( const MimeHeader & value );
    RetryAfter( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( Server );
  class Server : public SIPHeader
  {
    PCLASSINFO( Server, SIPHeader );
  public:
    Server();
    Server( const PString & body );
    Server( const MimeHeader & value );
    Server( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( Subject );
  class Subject : public SIPHeader
  {
    PCLASSINFO( Subject, SIPHeader );
  public:
    Subject();
    Subject( const PString & body );
    Subject( const MimeHeader & value );
    Subject( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( UserAgent );
  class UserAgent : public SIPHeader
  {
    PCLASSINFO( UserAgent, SIPHeader );
  public:
    UserAgent();
    UserAgent( const PString & body );
    UserAgent( const MimeHeader & value );
    UserAgent( const SIPHeader & header );
  };

  //DECLARE_STANDARD_SIP_HEADER( Warning );
  class Warning : public SIPHeader
  {
    PCLASSINFO( Warning, SIPHeader );
  public:
    Warning();
    Warning( const PString & body );
    Warning( const MimeHeader & value );
    Warning( const SIPHeader & header );
  };


  //DECLARE_URI_HEADER( From );
  class From : public SIPHeaderB
  {
  PCLASSINFO( From, SIPHeaderB );
  public:
    From();
    From(const From & header);
    From(const PString & header);
    From(const MimeHeader & header);
    From(const SIPURI & uri,const PString & displayName = PString::Empty(),
      const PString & tag = PString::Empty());
  };

  class To : public SIPHeaderB
  {
  PCLASSINFO( To, SIPHeaderB );
  public:
    To();
    To(const To & header);
    To(const PString & header);
    To(const MimeHeader & header);
    To(const SIPURI & uri,const PString & displayName = PString::Empty(),
      const PString & tag = PString::Empty());
  };

  //DECLARE_URI_HEADER( ReplyTo );
  class ReplyTo : public SIPHeaderB
  {
  PCLASSINFO( ReplyTo, SIPHeaderB );
  public:
    ReplyTo();
    ReplyTo(const ReplyTo & header);
    ReplyTo(const PString & header);
    ReplyTo(const MimeHeader & header);
    ReplyTo(const SIPURI & uri,const PString & displayName = PString::Empty(),
      const PString & tag = PString::Empty());
  };

  /// Comma delimited headers
  //DECLARE_COMMA_DELIMITED_HEADER( Accept );
  class Accept : public SIPHeaderC
  { 
    PCLASSINFO( Accept, SIPHeaderC );
  public:
    Accept(); 
    Accept( const Accept & header ); 
    Accept( const PString & header ); 
    Accept( const MimeHeader & header ); 
    Accept( const PStringArray & header ); 
  }; 
  class AcceptList : public SIPHeaderCList 
  { 
  public: 
    AcceptList(); 
    AcceptList( const SIPHeaderCList & list ); 
    AcceptList( const SIPHeaderC::Collection & list ); 
  };

  //DECLARE_COMMA_DELIMITED_HEADER( AcceptEncoding );
  class AcceptEncoding : public SIPHeaderC
  { 
    PCLASSINFO( AcceptEncoding, SIPHeaderC );
  public:
    AcceptEncoding(); 
    AcceptEncoding( const AcceptEncoding & header ); 
    AcceptEncoding( const PString & header ); 
    AcceptEncoding( const MimeHeader & header ); 
    AcceptEncoding( const PStringArray & header ); 
  }; 
  class AcceptEncodingList : public SIPHeaderCList 
  { 
  public: 
    AcceptEncodingList(); 
    AcceptEncodingList( const SIPHeaderCList & list ); 
    AcceptEncodingList( const SIPHeaderC::Collection & list ); 
  };

  //DECLARE_COMMA_DELIMITED_HEADER( AcceptLanguage );
  class AcceptLanguage : public SIPHeaderC
  { 
    PCLASSINFO( AcceptLanguage, SIPHeaderC );
  public:
    AcceptLanguage(); 
    AcceptLanguage( const AcceptLanguage & header ); 
    AcceptLanguage( const PString & header ); 
    AcceptLanguage( const MimeHeader & header ); 
    AcceptLanguage( const PStringArray & header ); 
  }; 
  class AcceptLanguageList : public SIPHeaderCList 
  { 
  public: 
    AcceptLanguageList(); 
    AcceptLanguageList( const SIPHeaderCList & list ); 
    AcceptLanguageList( const SIPHeaderC::Collection & list ); 
  };

  //DECLARE_COMMA_DELIMITED_HEADER( Allow );
  class Allow : public SIPHeaderC
  { 
    PCLASSINFO( Allow, SIPHeaderC );
  public:
    Allow(); 
    Allow( const Allow & header ); 
    Allow( const PString & header ); 
    Allow( const MimeHeader & header ); 
    Allow( const PStringArray & header ); 
  }; 
  class AllowList : public SIPHeaderCList 
  { 
  public: 
    AllowList(); 
    AllowList( const SIPHeaderCList & list ); 
    AllowList( const SIPHeaderC::Collection & list ); 
  };


  //DECLARE_COMMA_DELIMITED_HEADER( ContentEncoding );
  class ContentEncoding : public SIPHeaderC
  { 
    PCLASSINFO( ContentEncoding, SIPHeaderC );
  public:
    ContentEncoding(); 
    ContentEncoding( const ContentEncoding & header ); 
    ContentEncoding( const PString & header ); 
    ContentEncoding( const MimeHeader & header ); 
    ContentEncoding( const PStringArray & header ); 
  }; 
  class ContentEncodingList : public SIPHeaderCList 
  { 
  public: 
    ContentEncodingList(); 
    ContentEncodingList( const SIPHeaderCList & list ); 
    ContentEncodingList( const SIPHeaderC::Collection & list ); 
  };

  //DECLARE_COMMA_DELIMITED_HEADER( InReplyTo );
  class InReplyTo : public SIPHeaderC
  { 
    PCLASSINFO( InReplyTo, SIPHeaderC );
  public:
    InReplyTo(); 
    InReplyTo( const InReplyTo & header ); 
    InReplyTo( const PString & header ); 
    InReplyTo( const MimeHeader & header ); 
    InReplyTo( const PStringArray & header ); 
  }; 
  class InReplyToList : public SIPHeaderCList 
  { 
  public: 
    InReplyToList(); 
    InReplyToList( const SIPHeaderCList & list ); 
    InReplyToList( const SIPHeaderC::Collection & list ); 
  };

  //DECLARE_COMMA_DELIMITED_HEADER( ProxyRequire );
  class ProxyRequire : public SIPHeaderC
  { 
    PCLASSINFO( ProxyRequire, SIPHeaderC );
  public:
    ProxyRequire(); 
    ProxyRequire( const ProxyRequire & header ); 
    ProxyRequire( const PString & header ); 
    ProxyRequire( const MimeHeader & header ); 
    ProxyRequire( const PStringArray & header ); 
  }; 
  class ProxyRequireList : public SIPHeaderCList 
  { 
  public: 
    ProxyRequireList(); 
    ProxyRequireList( const SIPHeaderCList & list ); 
    ProxyRequireList( const SIPHeaderC::Collection & list ); 
  };

  //DECLARE_COMMA_DELIMITED_HEADER( Require );
  class Require : public SIPHeaderC
  { 
    PCLASSINFO( Require, SIPHeaderC );
  public:
    Require(); 
    Require( const Require & header ); 
    Require( const PString & header ); 
    Require( const MimeHeader & header ); 
    Require( const PStringArray & header ); 
  }; 
  class RequireList : public SIPHeaderCList 
  { 
  public: 
    RequireList(); 
    RequireList( const SIPHeaderCList & list ); 
    RequireList( const SIPHeaderC::Collection & list ); 
  };

  //DECLARE_COMMA_DELIMITED_HEADER( Supported );
  class Supported : public SIPHeaderC
  { 
    PCLASSINFO( Supported, SIPHeaderC );
  public:
    Supported(); 
    Supported( const Supported & header ); 
    Supported( const PString & header ); 
    Supported( const MimeHeader & header ); 
    Supported( const PStringArray & header ); 
  }; 
  class SupportedList : public SIPHeaderCList 
  { 
  public: 
    SupportedList(); 
    SupportedList( const SIPHeaderCList & list ); 
    SupportedList( const SIPHeaderC::Collection & list ); 
  };

  //DECLARE_COMMA_DELIMITED_HEADER( Unsupported );
  class Unsupported : public SIPHeaderC
  { 
    PCLASSINFO( Unsupported, SIPHeaderC );
  public:
    Unsupported(); 
    Unsupported( const Unsupported & header ); 
    Unsupported( const PString & header ); 
    Unsupported( const MimeHeader & header ); 
    Unsupported( const PStringArray & header ); 
  }; 
  class UnsupportedList : public SIPHeaderCList 
  { 
  public: 
    UnsupportedList(); 
    UnsupportedList( const SIPHeaderCList & list ); 
    UnsupportedList( const SIPHeaderC::Collection & list ); 
  };

  /// Comma delimited named parameters
  //DECLARE_COMMA_DELIMITED_PARAM_HEADER( AuthenticationInfo );
  class AuthenticationInfo : public SIPHeaderD 
  { 
  PCLASSINFO( AuthenticationInfo, SIPHeaderD ); 
  public: 
    AuthenticationInfo(); 
    AuthenticationInfo(const AuthenticationInfo & header); 
    AuthenticationInfo(const PString & header); 
    AuthenticationInfo(const MimeHeader & header); 
  };

  //DECLARE_COMMA_DELIMITED_PARAM_HEADER( Authorization );
  class Authorization : public SIPHeaderD 
  { 
  PCLASSINFO( Authorization, SIPHeaderD ); 
  public: 
    Authorization(); 
    Authorization(const Authorization & header); 
    Authorization(const PString & header); 
    Authorization(const MimeHeader & header); 
  };

  //DECLARE_COMMA_DELIMITED_PARAM_HEADER( ProxyAuthenticate );
  class ProxyAuthenticate : public SIPHeaderD
  {
  PCLASSINFO( ProxyAuthenticate, SIPHeaderD );
  public:
    ProxyAuthenticate();
    ProxyAuthenticate(const ProxyAuthenticate & header);
    ProxyAuthenticate(const PString & header);
    ProxyAuthenticate(const MimeHeader & header);
  };

  ///DECLARE_COMMA_DELIMITED_PARAM_HEADER( ProxyAuthorization );
  class ProxyAuthorization : public SIPHeaderD 
  { 
  PCLASSINFO( ProxyAuthorization, SIPHeaderD ); 
  public: 
    ProxyAuthorization(); 
    ProxyAuthorization(const ProxyAuthorization & header); 
    ProxyAuthorization(const PString & header); 
    ProxyAuthorization(const MimeHeader & header); 
  };

  ///DECLARE_COMMA_DELIMITED_PARAM_HEADER( WWWAuthenticate );
  class WWWAuthenticate : public SIPHeaderD 
  { 
  PCLASSINFO( WWWAuthenticate, SIPHeaderD ); 
  public: 
    WWWAuthenticate(); 
    WWWAuthenticate(const WWWAuthenticate & header); 
    WWWAuthenticate(const PString & header); 
    WWWAuthenticate(const MimeHeader & header); 
  };

  /// Integer headers
  //DECLARE_INTEGER_HEADER( Expires );
  class Expires : public SIPHeaderE {
    PCLASSINFO( Expires, SIPHeaderE );
  public:
    Expires();
    Expires( const PTimeInterval & interval );
    Expires( PInt64 value );
    Expires( const PString & header );
    Expires( const MimeHeader & header );
    Expires & operator=( const PTimeInterval & interval );
  };

  /// RFC 4028 Header Requirement
  class MinSE : public SIPHeaderE {
    PCLASSINFO( MinSE, SIPHeaderE );
  public:
    MinSE();
    MinSE( const PTimeInterval & interval );
    MinSE( PInt64 value );
    MinSE( const PString & header );
    MinSE( const MimeHeader & header );
    MinSE & operator=( const PTimeInterval & interval );
  };

  /// RFC 4028 Header Requirement
  class SessionExpires : public SIPHeaderE {
    PCLASSINFO( SessionExpires, SIPHeaderE );
  public:
    SessionExpires();
    SessionExpires( const PTimeInterval & interval );
    SessionExpires( PInt64 value );
    SessionExpires( const PString & header );
    SessionExpires( const MimeHeader & header );
    SessionExpires & operator=( const PTimeInterval & interval );
  };

  //DECLARE_INTEGER_HEADER( MaxForwards );
  class MaxForwards : public SIPHeaderE {
    PCLASSINFO( MaxForwards, SIPHeaderE );
  public:
    MaxForwards();
    MaxForwards( PInt64 value );
    MaxForwards( const PString & header );
    MaxForwards( const MimeHeader & header );
  };

  //DECLARE_INTEGER_HEADER( MinExpires );
  class MinExpires : public SIPHeaderE {
    PCLASSINFO( MinExpires, SIPHeaderE );
  public:
    MinExpires();
    MinExpires( PInt64 value );
    MinExpires( const PString & header );
    MinExpires( const MimeHeader & header );
  };

  //DECLARE_INTEGER_HEADER( TimeStamp );
  class TimeStamp : public SIPHeaderE {
    PCLASSINFO( TimeStamp, SIPHeaderE );
  public:
    TimeStamp();
    TimeStamp( PInt64 value );
    TimeStamp( const PString & header );
    TimeStamp( const MimeHeader & header );
  };

  /// Route Headers
  //DECLARE_ROUTE_HEADER( Route );
  class Route : public SIPHeaderF 
  { 
  PCLASSINFO( Route, SIPHeaderF ); 
  public: 
    Route(); 
    Route(const SIPHeaderF & header); 
    Route(const PString & header); 
    Route(const MimeHeader & header); 
    Route(const PStringArray & header); 
    Route(const RouteURI::Collection & uri); 
  }; 
  class RouteList : public SIPHeaderFList 
  { 
  PCLASSINFO( RouteList, SIPHeaderFList ); 
  public: 
    RouteList(); 
    RouteList(const SIPHeaderFList & list); 
    RouteList(const SIPHeaderF::Collection & list); 
  };

  ///DECLARE_ROUTE_HEADER( RecordRoute );
  class RecordRoute : public SIPHeaderF 
  { 
  PCLASSINFO( RecordRoute, SIPHeaderF ); 
  public: 
    RecordRoute(); 
    RecordRoute(const SIPHeaderF & header); 
    RecordRoute(const PString & header); 
    RecordRoute(const MimeHeader & header); 
    RecordRoute(const PStringArray & header); 
    RecordRoute(const RouteURI::Collection & uri); 
  }; 
  class RecordRouteList : public SIPHeaderFList 
  { 
  PCLASSINFO( RecordRouteList, SIPHeaderFList ); 
  public: 
    RecordRouteList(); 
    RecordRouteList(const SIPHeaderFList & list); 
    RecordRouteList(const SIPHeaderF::Collection & list); 
  };

  ////Path Header (RFC 3327 )
  class Path : public SIPHeaderF 
  { 
  PCLASSINFO( Path, SIPHeaderF ); 
  public: 
    Path(); 
    Path(const SIPHeaderF & header); 
    Path(const PString & header); 
    Path(const MimeHeader & header); 
    Path(const PStringArray & header); 
    Path(const RouteURI::Collection & uri); 
  }; 
  class PathList : public SIPHeaderFList 
  { 
  PCLASSINFO( PathList, SIPHeaderFList ); 
  public: 
    PathList(); 
    PathList(const SIPHeaderFList & list); 
    PathList(const SIPHeaderF::Collection & list); 
  };



  /// extension headers

  class ReferTo : public SIPHeaderB
  {
  PCLASSINFO( ReferTo, SIPHeaderB );
  public:
    ReferTo();
    ReferTo(const ReferTo & header);
    ReferTo(const PString & header);
    ReferTo(const MimeHeader & header);
    ReferTo(const SIPURI & uri,const PString & displayName = PString::Empty(),
      const PString & tag = PString::Empty());
  };

  class Event : public SIPHeader
  {
    PCLASSINFO( Event, SIPHeader );
  public:
    Event();
    Event( const PString & body );
    Event( const MimeHeader & value );
    Event( const SIPHeader & header );
    PString GetParameter( const PString & paramName )const;
    void SetParameter(const PString & paramName, const PString & paramValue );
    PString GetEventName()const;
    PString GetEventId()const;
    void SetEventName( const PString & eventName );
    void SetEventId( const PString & eventId );
  };

  class SIPETag : public SIPHeader
  {
    PCLASSINFO( SIPETag, SIPHeader );
  public:
    SIPETag();
    SIPETag( const PString & body );
    SIPETag( const MimeHeader & value );
    SIPETag( const SIPHeader & header );
  };

  class SIPIfMatch : public SIPHeader
  {
    PCLASSINFO( SIPIfMatch, SIPHeader );
  public:
    SIPIfMatch();
    SIPIfMatch( const PString & body );
    SIPIfMatch( const MimeHeader & value );
    SIPIfMatch( const SIPHeader & header );
  };

  class AllowEvents : public SIPHeaderC
  { 
    PCLASSINFO( AllowEvents, SIPHeaderC );
  public:
    AllowEvents(); 
    AllowEvents( const AllowEvents & header ); 
    AllowEvents( const PString & header ); 
    AllowEvents( const MimeHeader & header ); 
    AllowEvents( const PStringArray & header ); 
  }; 
  class AllowEventsList : public SIPHeaderCList 
  { 
    PCLASSINFO( AllowEventsList, SIPHeaderCList );
  public: 
    AllowEventsList(); 
    AllowEventsList( const SIPHeaderCList & list ); 
    AllowEventsList( const SIPHeaderC::Collection & list ); 
  };

  class SubscriptionState : public SIPHeader
  {
    PCLASSINFO( SubscriptionState, SIPHeader );
  public:
    SubscriptionState();
    SubscriptionState( const PString & body );
    SubscriptionState( const MimeHeader & value );
    SubscriptionState( const SIPHeader & header );
  };

  /// Privacy headers
  class PAssertedIdentity : public SIPHeaderF 
  { 
  PCLASSINFO( PAssertedIdentity, SIPHeaderF ); 
  public: 
    PAssertedIdentity(); 
    PAssertedIdentity(const SIPHeaderF & header); 
    PAssertedIdentity(const PString & header); 
    PAssertedIdentity(const MimeHeader & header); 
    PAssertedIdentity(const PStringArray & header); 
    PAssertedIdentity(const RouteURI::Collection & uri); 
  }; 
  class PAssertedIdentityList : public SIPHeaderFList 
  { 
  PCLASSINFO( PAssertedIdentityList, SIPHeaderFList ); 
  public: 
    PAssertedIdentityList(); 
    PAssertedIdentityList(const SIPHeaderFList & list); 
    PAssertedIdentityList(const SIPHeaderF::Collection & list); 
  };

  
  class PPreferredIdentity : public SIPHeaderF 
  { 
  PCLASSINFO( PPreferredIdentity, SIPHeaderF ); 
  public: 
    PPreferredIdentity(); 
    PPreferredIdentity(const SIPHeaderF & header); 
    PPreferredIdentity(const PString & header); 
    PPreferredIdentity(const MimeHeader & header); 
    PPreferredIdentity(const PStringArray & header); 
    PPreferredIdentity(const RouteURI::Collection & uri); 
  }; 
  class PPreferredIdentityList : public SIPHeaderFList 
  { 
  PCLASSINFO( PPreferredIdentityList, SIPHeaderFList ); 
  public: 
    PPreferredIdentityList(); 
    PPreferredIdentityList(const SIPHeaderFList & list); 
    PPreferredIdentityList(const SIPHeaderF::Collection & list); 
  };

  /// RFC3891 Replaces Header

  class Replaces : public SIPHeader
  {
    PCLASSINFO( Replaces, SIPHeader );
  public:
    Replaces();
    Replaces( const PString & body );
    Replaces( const MimeHeader & value );
    Replaces( const SIPHeader & header );
    
    PString GetCallId()const;
    
    PString GetFromTag()const;

    PString GetToTag()const;

    BOOL IsEarlyDialogFlagSet()const;
    
    PString GetParameter( 
      const PString & paramName
    )const;

    void SetCallId( 
      const PString & callId
    );

    void SetFromTag(
      const PString & fromTag
    );

    void SetToTag(
      const PString & toTag
    );

    void SetParameter(
      const PString & paramName,
      const PString & paramValue
    );

    void SetEarlyDialogFlag(
      BOOL early = TRUE
    );

    PString EscapeAsRFC2396()const;
  };

}; ///namespace SIPParser

/// Custom headers
#include "Via.h"
#include "CSeq.h"
#include "Contact.h"
#include "StartLine.h"
#include "TransactionId.h"
#include "RecordRouteBranch.h"

#endif



