/*
 *
 * MessageHeaders.cxx
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
 * $Log: MessageHeaders.cxx,v $
 * Revision 1.15  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.14  2006/11/03 09:43:31  joegenbaclor
 * Some modification on Event header to support optional parameter parsing
 *
 * Revision 1.13  2006/11/02 22:53:46  joegenbaclor
 * Added Path header support (RFC 3327)
 *
 * Revision 1.12  2006/10/23 15:44:58  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.11  2006/10/12 08:41:24  joegenbaclor
 * Added support for RFC 4028 headers
 *
 * Revision 1.10  2006/08/09 09:32:58  joegenbaclor
 * 1.  Added Privacy header support
 * 2.  Reintroduced VC++ 7.1 Project Files
 *
 * Revision 1.9  2006/06/13 05:23:04  joegenbaclor
 * Added missing default constructor for SIPETag and SIPIfMatch
 *
 * Revision 1.8  2006/06/09 07:37:30  joegenbaclor
 * 1.  Added Publish, SIPETag & SIPIfMatch  support in SIPMessage
 * 2.  Started work on StatusPage Resource for OSSAppConfig
 *
 * Revision 1.7  2006/04/26 00:45:12  joegenbaclor
 * Modified Expires to accept PTimeInterval as constructor parameter
 *
 * Revision 1.6  2006/04/21 09:55:10  joegenbaclor
 * Added RFC 3265 header requirements
 *
 * Revision 1.5  2006/04/21 03:55:22  joegenbaclor
 * Added ReferTo header
 *
 * Revision 1.4  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "MessageHeaders.h"

#define new PNEW

using namespace SIPParser;

#ifdef WIN32
#pragma warning(disable:4100)
#endif

////////////////////////////////////////////////////////////
AlertInfo::AlertInfo()
  : SIPHeader( "Alert-Info", "" ) 
{
}

AlertInfo::AlertInfo( 
  const PString & body
) : SIPHeader( "Alert-Info", body ) 
{ 
}

AlertInfo::AlertInfo( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

AlertInfo::AlertInfo( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}

////////////////////////////////////////////////////////////

//CREATE_STANDARD_SIP_HEADER( CallId, "Call-ID", ParserTools::GenGUID() );
CallId::CallId()
  : SIPHeader( "Call-ID", "" ) 
{
}

CallId::CallId( 
  const PString & body
) : SIPHeader( "Call-ID", body ) 
{ 
}

CallId::CallId( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

CallId::CallId( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}

////////////////////////////////////////////////////////////
//CREATE_STANDARD_SIP_HEADER( CallInfo, "Call-Info", PString::Empty() );
CallInfo::CallInfo()
  : SIPHeader( "Call-Info", "" ) 
{
}

CallInfo::CallInfo( 
  const PString & body
) : SIPHeader( "Call-Info", body ) 
{ 
}

CallInfo::CallInfo( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

CallInfo::CallInfo( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}

////////////////////////////////////////////////////////////
//CREATE_STANDARD_SIP_HEADER( ContentDisposition, "Content-Disposition", "session" );

ContentDisposition::ContentDisposition()
  : SIPHeader( "Content-Disposition", "" ) 
{
}

ContentDisposition::ContentDisposition( 
  const PString & body
) : SIPHeader( "Content-Disposition", body ) 
{ 
}

ContentDisposition::ContentDisposition( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

ContentDisposition::ContentDisposition( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}

////////////////////////////////////////////////////////////
///CREATE_STANDARD_SIP_HEADER( ContentLanguage, "Content-Language", "en" );

ContentLanguage::ContentLanguage()
  : SIPHeader( "Content-Language", "" ) 
{
}

ContentLanguage::ContentLanguage( 
  const PString & body
) : SIPHeader( "Content-Language", body ) 
{ 
}

ContentLanguage::ContentLanguage( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

ContentLanguage::ContentLanguage( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}

////////////////////////////////////////////////////////////
///CREATE_STANDARD_SIP_HEADER( ContentLength, "Content-Length", "0" );
ContentLength::ContentLength()
  : SIPHeader( "Content-Length", "" ) 
{
}

ContentLength::ContentLength( 
  const PString & body
) : SIPHeader( "Content-Length", body ) 
{ 
}

ContentLength::ContentLength( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

ContentLength::ContentLength( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}

////////////////////////////////////////////////////////////
//CREATE_STANDARD_SIP_HEADER( ContentType, "Content-Type", "application/sdp" );
ContentType::ContentType()
  : SIPHeader( "Content-Type", "" ) 
{
}

ContentType::ContentType( 
  const PString & body
) : SIPHeader( "Content-Type", body ) 
{ 
}

ContentType::ContentType( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

ContentType::ContentType( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}

////////////////////////////////////////////////////////////
//CREATE_STANDARD_SIP_HEADER( Date, "Date", ParserTools::GetRFC1123Date() );
Date::Date()
  : SIPHeader( "Date", "" ) 
{
}

Date::Date( 
  const PString & body
) : SIPHeader( "Date", body ) 
{ 
}

Date::Date( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

Date::Date( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}

////////////////////////////////////////////////////////////
//CREATE_STANDARD_SIP_HEADER( ErrorInfo, "Error-Info", PString::Empty() );
ErrorInfo::ErrorInfo()
  : SIPHeader( "Error-Info", "" ) 
{
}

ErrorInfo::ErrorInfo( 
  const PString & body
) : SIPHeader( "Error-Info", body ) 
{ 
}

ErrorInfo::ErrorInfo( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

ErrorInfo::ErrorInfo( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}
////////////////////////////////////////////////////////////
//CREATE_STANDARD_SIP_HEADER( MIMEVersion, "MIME-Version", "1.0" );

MIMEVersion::MIMEVersion() 
  : SIPHeader( "MIME-Version", "" ) 
{ 
}

MIMEVersion::MIMEVersion( 
  const PString & body
) : SIPHeader( "MIME-Version", body ) 
{ 
}

MIMEVersion::MIMEVersion( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

MIMEVersion::MIMEVersion( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}
////////////////////////////////////////////////////////////
//CREATE_STANDARD_SIP_HEADER( Organization, "Organization", PString::Empty() );
Organization::Organization() 
  : SIPHeader( "Organization", "" ) 
{ 
}

Organization::Organization( 
  const PString & body
) : SIPHeader( "Organization", body ) 
{ 
}

Organization::Organization( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

Organization::Organization( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}
////////////////////////////////////////////////////////////
///CREATE_STANDARD_SIP_HEADER( Priority, "Priority", "normal" );
Priority::Priority() 
  : SIPHeader( "Priority", "" ) 
{ 
}

Priority::Priority( 
  const PString & body
) : SIPHeader( "Priority", body ) 
{ 
}

Priority::Priority( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

Priority::Priority( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}
////////////////////////////////////////////////////////////
///CREATE_STANDARD_SIP_HEADER( RetryAfter, "Retry-After", "120" );
RetryAfter::RetryAfter() 
  : SIPHeader( "RetryAfter", "" ) 
{ 
}

RetryAfter::RetryAfter( 
  const PString & body
) : SIPHeader( "Retry-After", body ) 
{ 
}

RetryAfter::RetryAfter( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

RetryAfter::RetryAfter( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}
////////////////////////////////////////////////////////////
///CREATE_STANDARD_SIP_HEADER( Server, "Server", "JEB SIP v1" );
Server::Server() 
  : SIPHeader( "Server", "" ) 
{ 
}

Server::Server( 
  const PString & body
) : SIPHeader( "Server", body ) 
{ 
}

Server::Server( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

Server::Server( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}
////////////////////////////////////////////////////////////
//CREATE_STANDARD_SIP_HEADER( Subject, "Subject", PString::Empty() );
Subject::Subject() 
  : SIPHeader( "Subject", "" ) 
{ 
}

Subject::Subject( 
  const PString & body
) : SIPHeader( "Subject", body ) 
{ 
}

Subject::Subject( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

Subject::Subject( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}
////////////////////////////////////////////////////////////
///CREATE_STANDARD_SIP_HEADER( UserAgent, "User-Agent", "JEB SIP v1" );
UserAgent::UserAgent() 
  : SIPHeader( "User-Agent", "" ) 
{ 
}

UserAgent::UserAgent( 
  const PString & body
) : SIPHeader( "User-Agent", body ) 
{ 
}

UserAgent::UserAgent( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

UserAgent::UserAgent( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}
////////////////////////////////////////////////////////////
///CREATE_STANDARD_SIP_HEADER( Warning, "Warning", PString::Empty() );
Warning::Warning() 
  : SIPHeader( "Warning", "" ) 
{ 
}

Warning::Warning( 
  const PString & body
) : SIPHeader( "Warning", body ) 
{ 
}

Warning::Warning( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

Warning::Warning( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}
////////////////////////////////////////////////////////////
//CREATE_URI_HEADER( From, "From" );

From::From() 
  : SIPHeaderB( "From" )
{
}

From::From(
  const From & header
) : SIPHeaderB( "From", header )
{
}

From::From(
  const PString & header
) : SIPHeaderB( "From", header )
{
}

From::From(
  const MimeHeader & header
) : SIPHeaderB( "From", header )
{
}

From::From(
  const SIPURI & uri,
  const PString & displayName, 
  const PString & tag 
) : SIPHeaderB( "From", uri, displayName, tag )
{
}

////////////////////////////////////////////////////////////
///CREATE_URI_HEADER( To, "To" );
To::To() 
  : SIPHeaderB( "To" )
{
}

To::To(
  const To & header
) : SIPHeaderB( "To", header )
{
}

To::To(
  const PString & header
) : SIPHeaderB( "To", header )
{
}

To::To(
  const MimeHeader & header
) : SIPHeaderB( "To", header )
{
}

To::To(
  const SIPURI & uri,
  const PString & displayName, 
  const PString & tag 
) : SIPHeaderB( "To", uri, displayName, tag )
{
}

////////////////////////////////////////////////////////////////////

ReferTo::ReferTo() 
  : SIPHeaderB( "Refer-To" )
{
}

ReferTo::ReferTo(
  const ReferTo & header
) : SIPHeaderB( "Refer-To", header )
{
}

ReferTo::ReferTo(
  const PString & header
) : SIPHeaderB( "Refer-To", header )
{
}

ReferTo::ReferTo(
  const MimeHeader & header
) : SIPHeaderB( "Refer-To", header )
{
}

ReferTo::ReferTo(
  const SIPURI & uri,
  const PString & displayName, 
  const PString & tag 
) : SIPHeaderB( "Refer-To", uri, displayName, tag )
{
}

////////////////////////////////////////////////////////////
//CREATE_URI_HEADER( ReplyTo, "Reply-To" );
ReplyTo::ReplyTo() 
  : SIPHeaderB( "Reply-To" )
{
}

ReplyTo::ReplyTo(
  const ReplyTo & header
) : SIPHeaderB( "Reply-To", header )
{
}

ReplyTo::ReplyTo(
  const PString & header
) : SIPHeaderB( "Reply-To", header )
{
}

ReplyTo::ReplyTo(
  const MimeHeader & header
) : SIPHeaderB( "Reply-To", header )
{
}

ReplyTo::ReplyTo(
  const SIPURI & uri,
  const PString & displayName, 
  const PString & tag 
) : SIPHeaderB( "Reply-To", uri, displayName, tag )
{
}

////////////////////////////////////////////////////////////
/// Comma delimited headers
//CREATE_COMMA_DELIMITED_HEADER( Accept, "Accept" );
Accept::Accept() 
  : SIPHeaderC( "Accept" )
{
}

Accept::Accept( 
  const Accept & header 
) : SIPHeaderC( "Accept", (SIPHeaderC&)header )
{
}

Accept::Accept( 
  const PString & header 
) : SIPHeaderC( "Accept", header )
{
}

Accept::Accept( 
  const MimeHeader & header 
) : SIPHeaderC( "Accept", header )
{
}

Accept::Accept( 
  const PStringArray & header 
) : SIPHeaderC( "Accept", header )
{
}

AcceptList::AcceptList()
{
}

AcceptList::AcceptList(
  const SIPHeaderCList & list
) : SIPHeaderCList( list )
{
}

AcceptList::AcceptList(
  const SIPHeaderC::Collection & list
) : SIPHeaderCList(list)
{
}


////////////////////////////////////////////////////////////
//CREATE_COMMA_DELIMITED_HEADER( AcceptEncoding, "Accept-Encoding" );
AcceptEncoding::AcceptEncoding() 
  : SIPHeaderC( "Accept-Encoding" )
{
}

AcceptEncoding::AcceptEncoding( 
  const AcceptEncoding & header 
) : SIPHeaderC( "Accept-Encoding", (SIPHeaderC&)header )
{
}

AcceptEncoding::AcceptEncoding( 
  const PString & header 
) : SIPHeaderC( "Accept-Encoding", header )
{
}

AcceptEncoding::AcceptEncoding( 
  const MimeHeader & header 
) : SIPHeaderC( "Accept-Encoding", header )
{
}

AcceptEncoding::AcceptEncoding( 
  const PStringArray & header 
) : SIPHeaderC( "Accept-Encoding", header )
{
}

AcceptEncodingList::AcceptEncodingList()
{
}

AcceptEncodingList::AcceptEncodingList(
  const SIPHeaderCList & list
) : SIPHeaderCList( list )
{
}

AcceptEncodingList::AcceptEncodingList(
  const SIPHeaderC::Collection & list
) : SIPHeaderCList(list)
{
}


////////////////////////////////////////////////////////////
//CREATE_COMMA_DELIMITED_HEADER( AcceptLanguage, "Accept-Language" );
AcceptLanguage::AcceptLanguage() 
  : SIPHeaderC( "Accept-Language" )
{
}

AcceptLanguage::AcceptLanguage( 
  const AcceptLanguage & header 
) : SIPHeaderC( "Accept-Language", (SIPHeaderC&)header )
{
}

AcceptLanguage::AcceptLanguage( 
  const PString & header 
) : SIPHeaderC( "Accept-Language", header )
{
}

AcceptLanguage::AcceptLanguage( 
  const MimeHeader & header 
) : SIPHeaderC( "Accept-Language", header )
{
}

AcceptLanguage::AcceptLanguage( 
  const PStringArray & header 
) : SIPHeaderC( "Accept-Language", header )
{
}

AcceptLanguageList::AcceptLanguageList()
{
}

AcceptLanguageList::AcceptLanguageList(
  const SIPHeaderCList & list
) : SIPHeaderCList( list )
{
}

AcceptLanguageList::AcceptLanguageList(
  const SIPHeaderC::Collection & list
) : SIPHeaderCList(list)
{
}


////////////////////////////////////////////////////////////
///CREATE_COMMA_DELIMITED_HEADER( Allow, "Allow" );
Allow::Allow() 
  : SIPHeaderC( "Allow" )
{
}

Allow::Allow( 
  const Allow & header 
) : SIPHeaderC( "Allow", (SIPHeaderC&)header )
{
}

Allow::Allow( 
  const PString & header 
) : SIPHeaderC( "Allow", header )
{
}

Allow::Allow( 
  const MimeHeader & header 
) : SIPHeaderC( "Allow", header )
{
}

Allow::Allow( 
  const PStringArray & header 
) : SIPHeaderC( "Allow", header )
{
}

AllowList::AllowList()
{
}

AllowList::AllowList(
  const SIPHeaderCList & list
) : SIPHeaderCList( list )
{
}

AllowList::AllowList(
  const SIPHeaderC::Collection & list
) : SIPHeaderCList(list)
{
}


////////////////////////////////////////////////////////////
//CREATE_COMMA_DELIMITED_HEADER( ContentEncoding, "Content-Encoding" );
ContentEncoding::ContentEncoding() 
  : SIPHeaderC( "Content-Encoding" )
{
}

ContentEncoding::ContentEncoding( 
  const ContentEncoding & header 
) : SIPHeaderC( "Content-Encoding", (SIPHeaderC&)header )
{
}

ContentEncoding::ContentEncoding( 
  const PString & header 
) : SIPHeaderC( "Content-Encoding", header )
{
}

ContentEncoding::ContentEncoding( 
  const MimeHeader & header 
) : SIPHeaderC( "Content-Encoding", header )
{
}

ContentEncoding::ContentEncoding( 
  const PStringArray & header 
) : SIPHeaderC( "Content-Encoding", header )
{
}

ContentEncodingList::ContentEncodingList()
{
}

ContentEncodingList::ContentEncodingList(
  const SIPHeaderCList & list
) : SIPHeaderCList( list )
{
}

ContentEncodingList::ContentEncodingList(
  const SIPHeaderC::Collection & list
) : SIPHeaderCList(list)
{
}


////////////////////////////////////////////////////////////
//CREATE_COMMA_DELIMITED_HEADER( InReplyTo, "In-Reply-To" );
InReplyTo::InReplyTo() 
  : SIPHeaderC( "In-Reply-To" )
{
}

InReplyTo::InReplyTo( 
  const InReplyTo & header 
) : SIPHeaderC( "In-Reply-To", (SIPHeaderC&)header )
{
}

InReplyTo::InReplyTo( 
  const PString & header 
) : SIPHeaderC( "In-Reply-To", header )
{
}

InReplyTo::InReplyTo( 
  const MimeHeader & header 
) : SIPHeaderC( "In-Reply-To", header )
{
}

InReplyTo::InReplyTo( 
  const PStringArray & header 
) : SIPHeaderC( "In-Reply-To", header )
{
}

InReplyToList::InReplyToList()
{
}

InReplyToList::InReplyToList(
  const SIPHeaderCList & list
) : SIPHeaderCList( list )
{
}

InReplyToList::InReplyToList(
  const SIPHeaderC::Collection & list
) : SIPHeaderCList(list)
{
}


////////////////////////////////////////////////////////////
//CREATE_COMMA_DELIMITED_HEADER( ProxyRequire, "Proxy-Require" );
ProxyRequire::ProxyRequire() 
  : SIPHeaderC( "Proxy-Require" )
{
}

ProxyRequire::ProxyRequire( 
  const ProxyRequire & header 
) : SIPHeaderC( "Proxy-Require", (SIPHeaderC&)header )
{
}

ProxyRequire::ProxyRequire( 
  const PString & header 
) : SIPHeaderC( "Proxy-Require", header )
{
}

ProxyRequire::ProxyRequire( 
  const MimeHeader & header 
) : SIPHeaderC( "Proxy-Require", header )
{
}

ProxyRequire::ProxyRequire( 
  const PStringArray & header 
) : SIPHeaderC( "Proxy-Require", header )
{
}

ProxyRequireList::ProxyRequireList()
{
}

ProxyRequireList::ProxyRequireList(
  const SIPHeaderCList & list
) : SIPHeaderCList( list )
{
}

ProxyRequireList::ProxyRequireList(
  const SIPHeaderC::Collection & list
) : SIPHeaderCList(list)
{
}


////////////////////////////////////////////////////////////
//CREATE_COMMA_DELIMITED_HEADER( Require, "Require" );
Require::Require() 
  : SIPHeaderC( "Require" )
{
}

Require::Require( 
  const Require & header 
) : SIPHeaderC( "Require", (SIPHeaderC&)header )
{
}

Require::Require( 
  const PString & header 
) : SIPHeaderC( "Require", header )
{
}

Require::Require( 
  const MimeHeader & header 
) : SIPHeaderC( "Require", header )
{
}

Require::Require( 
  const PStringArray & header 
) : SIPHeaderC( "Require", header )
{
}

RequireList::RequireList()
{
}

RequireList::RequireList(
  const SIPHeaderCList & list
) : SIPHeaderCList( list )
{
}

RequireList::RequireList(
  const SIPHeaderC::Collection & list
) : SIPHeaderCList(list)
{
}


////////////////////////////////////////////////////////////
//CREATE_COMMA_DELIMITED_HEADER( Supported, "Supported" );
Supported::Supported() 
  : SIPHeaderC( "Supported" )
{
}

Supported::Supported( 
  const Supported & header 
) : SIPHeaderC( "Supported", (SIPHeaderC&)header )
{
}

Supported::Supported( 
  const PString & header 
) : SIPHeaderC( "Supported", header )
{
}

Supported::Supported( 
  const MimeHeader & header 
) : SIPHeaderC( "Supported", header )
{
}

Supported::Supported( 
  const PStringArray & header 
) : SIPHeaderC( "Supported", header )
{
}

SupportedList::SupportedList()
{
}

SupportedList::SupportedList(
  const SIPHeaderCList & list
) : SIPHeaderCList( list )
{
}

SupportedList::SupportedList(
  const SIPHeaderC::Collection & list
) : SIPHeaderCList(list)
{
}


////////////////////////////////////////////////////////////
//CREATE_COMMA_DELIMITED_HEADER( Unsupported, "Unsupported" );
Unsupported::Unsupported() 
  : SIPHeaderC( "Unsupported" )
{
}

Unsupported::Unsupported( 
  const Unsupported & header 
) : SIPHeaderC( "Unsupported", (SIPHeaderC&)header )
{
}

Unsupported::Unsupported( 
  const PString & header 
) : SIPHeaderC( "Unsupported", header )
{
}

Unsupported::Unsupported( 
  const MimeHeader & header 
) : SIPHeaderC( "Unsupported", header )
{
}

Unsupported::Unsupported( 
  const PStringArray & header 
) : SIPHeaderC( "Unsupported", header )
{
}

UnsupportedList::UnsupportedList()
{
}

UnsupportedList::UnsupportedList(
  const SIPHeaderCList & list
) : SIPHeaderCList( list )
{
}

UnsupportedList::UnsupportedList(
  const SIPHeaderC::Collection & list
) : SIPHeaderCList(list)
{
}


////////////////////////////////////////////////////////////
/// Comma delimited named parameters
//CREATE_COMMA_DELIMITED_PARAM_HEADER( AuthenticationInfo, "Authentication-Info" );
AuthenticationInfo::AuthenticationInfo() 
  : SIPHeaderD( "Authentication-Info" )
{
}

AuthenticationInfo::AuthenticationInfo(
  const AuthenticationInfo & header
) : SIPHeaderD( "Authentication-Info", header )
{
}

AuthenticationInfo::AuthenticationInfo(
  const PString & header
) : SIPHeaderD( "Authentication-Info", header )
{
}

AuthenticationInfo::AuthenticationInfo(
  const MimeHeader & header
) : SIPHeaderD( "Authentication-Info", header )
{
}


////////////////////////////////////////////////////////////
//CREATE_COMMA_DELIMITED_PARAM_HEADER( Authorization, "Authorization" );
Authorization::Authorization() 
  : SIPHeaderD( "Authorization" )
{
}

Authorization::Authorization(
  const Authorization & header
) : SIPHeaderD( "Authorization", header )
{
}

Authorization::Authorization(
  const PString & header
) : SIPHeaderD( "Authorization", header )
{
}

Authorization::Authorization(
  const MimeHeader & header
) : SIPHeaderD( "Authorization", header )
{
}


////////////////////////////////////////////////////////////
//CREATE_COMMA_DELIMITED_PARAM_HEADER( ProxyAuthenticate, "Proxy-Authenticate" );
ProxyAuthenticate::ProxyAuthenticate() 
  : SIPHeaderD( "Proxy-Authenticate" )
{
}

ProxyAuthenticate::ProxyAuthenticate(
  const ProxyAuthenticate & header
) : SIPHeaderD( "Proxy-Authenticate", header )
{
}

ProxyAuthenticate::ProxyAuthenticate(
  const PString & header
) : SIPHeaderD( "Proxy-Authenticate", header )
{
}

ProxyAuthenticate::ProxyAuthenticate(
  const MimeHeader & header
) : SIPHeaderD( "Proxy-Authenticate", header )
{
}


////////////////////////////////////////////////////////////
//CREATE_COMMA_DELIMITED_PARAM_HEADER( ProxyAuthorization, "Proxy-Authorization" );
ProxyAuthorization::ProxyAuthorization() 
  : SIPHeaderD( "Proxy-Authorization" )
{
}

ProxyAuthorization::ProxyAuthorization(
  const ProxyAuthorization & header
) : SIPHeaderD( "Proxy-Authorization", header )
{
}

ProxyAuthorization::ProxyAuthorization(
  const PString & header
) : SIPHeaderD( "Proxy-Authorization", header )
{
}

ProxyAuthorization::ProxyAuthorization(
  const MimeHeader & header
) : SIPHeaderD( "Proxy-Authorization", header )
{
}


////////////////////////////////////////////////////////////
//CREATE_COMMA_DELIMITED_PARAM_HEADER( WWWAuthenticate, "WWW-Authenticate" );
WWWAuthenticate::WWWAuthenticate() 
  : SIPHeaderD( "WWW-Authenticate" )
{
}

WWWAuthenticate::WWWAuthenticate(
  const WWWAuthenticate & header
) : SIPHeaderD( "WWW-Authenticate", header )
{
}

WWWAuthenticate::WWWAuthenticate(
  const PString & header
) : SIPHeaderD( "WWW-Authenticate", header )
{
}

WWWAuthenticate::WWWAuthenticate(
  const MimeHeader & header
) : SIPHeaderD( "WWW-Authenticate", header )
{
}


////////////////////////////////////////////////////////////


Expires::Expires() 
  : SIPHeaderE( "Expires"  ) 
{ 
}

Expires::Expires(
  const PTimeInterval & interval
) : SIPHeaderE( "Expires", interval.GetSeconds(), 0, 86400, 3600 ) 
{
}

Expires::Expires( 
  PInt64 value 
) : SIPHeaderE( "Expires", value, 0, 86400, 3600 ) 
{
}

Expires::Expires( 
  const PString & header 
) : SIPHeaderE( "Expires", header, 0, 86400, 3600 )
{
}

Expires::Expires( 
  const MimeHeader & header 
) : SIPHeaderE( header, 0, 86400, 3600 )
{
}

Expires & Expires::operator=( const PTimeInterval & interval )
{
  PInt64 expire = interval.GetSeconds();
  SIPHeaderE::operator=( expire );
  return *this;
}
//////////////////////////////////////////////////////////////////////////////
/// RFC 4028 Header Requirement
MinSE::MinSE() 
  : SIPHeaderE( "Min-SE"  ) 
{ 
}

MinSE::MinSE(
  const PTimeInterval & interval
) : SIPHeaderE( "Min-SE", interval.GetSeconds(), 90, 86400, 90 ) 
{
}

MinSE::MinSE( 
  PInt64 value 
) : SIPHeaderE( "Min-SE", value, 90, 86400, 90 ) 
{
}

MinSE::MinSE( 
  const PString & header 
) : SIPHeaderE( "Min-SE", header, 90, 86400, 90 )
{
}

MinSE::MinSE( 
  const MimeHeader & header 
) : SIPHeaderE( header, 90, 86400, 90 )
{
}

MinSE & MinSE::operator=( const PTimeInterval & interval )
{
  PInt64 expire = interval.GetSeconds();
  SIPHeaderE::operator=( expire );
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
/// RFC 4028 Header Requirement
SessionExpires::SessionExpires() 
  : SIPHeaderE( "Session-Expires"  ) 
{ 
}

SessionExpires::SessionExpires(
  const PTimeInterval & interval
) : SIPHeaderE( "Session-Expires", interval.GetSeconds(), 90, 86400, 90 ) 
{
}

SessionExpires::SessionExpires( 
  PInt64 value 
) : SIPHeaderE( "Session-Expires", value, 90, 86400, 90 ) 
{
}

SessionExpires::SessionExpires( 
  const PString & header 
) : SIPHeaderE( "Session-Expires", header, 90, 86400, 90 )
{
}

SessionExpires::SessionExpires( 
  const MimeHeader & header 
) : SIPHeaderE( header, 90, 86400, 90 )
{
}

SessionExpires & SessionExpires::operator=( const PTimeInterval & interval )
{
  PInt64 expire = interval.GetSeconds();
  SIPHeaderE::operator=( expire );
  return *this;
}
////////////////////////////////////////////////////////////
//CREATE_INTEGER_HEADER( MaxForwards, "Max-Forwards", 0, 255, 70 );
MaxForwards::MaxForwards() 
  : SIPHeaderE( "Max-Forwards" ) 
{ 
}


MaxForwards::MaxForwards( 
  PInt64 value 
) : SIPHeaderE( "Max-Forwards", value, 0, 255, 70 ) 
{
}

MaxForwards::MaxForwards( 
  const PString & header 
) : SIPHeaderE( "Max-Forwards", header, 0, 255, 70 )
{
}

MaxForwards::MaxForwards( 
  const MimeHeader & header 
) : SIPHeaderE( header, 0, 255, 70 )
{
}

////////////////////////////////////////////////////////////
//CREATE_INTEGER_HEADER( MinExpires, "Min-Expires", 0, 86400, 60 );
MinExpires::MinExpires() 
  : SIPHeaderE( "Min-Expires"  ) 
{ 
}

MinExpires::MinExpires( 
  PInt64 value 
) : SIPHeaderE( "Min-Expires", value, 0, 86400, 60 ) 
{
}

MinExpires::MinExpires( 
  const PString & header 
) : SIPHeaderE( "Min-Expires", header, 0, 86400, 60 )
{
}

MinExpires::MinExpires( 
  const MimeHeader & header 
) : SIPHeaderE( header, 0, 86400, 60 )
{
}

////////////////////////////////////////////////////////////
//CREATE_INTEGER_HEADER( TimeStamp, "TimeStamp", 0, 0, 0 );
TimeStamp::TimeStamp() 
  : SIPHeaderE( "TimeStamp"  ) 
{ 
}

TimeStamp::TimeStamp( 
  PInt64 value 
) : SIPHeaderE( "Timestamp", value, 0, 0, 0 ) 
{
}

TimeStamp::TimeStamp( 
  const PString & header 
) : SIPHeaderE( "Timestamp", header, 0, 0, 0 )
{
}

TimeStamp::TimeStamp( 
  const MimeHeader & header 
) : SIPHeaderE( header, 0, 0, 0 )
{
}

////////////////////////////////////////////////////////////

//CREATE_ROUTE_HEADER( Route, "Route" );
Route::Route() 
  : SIPHeaderF("Route")
{
}

Route::Route(
  const SIPHeaderF & header
) : SIPHeaderF("Route", header)
{
}

Route::Route(
  const PString & header
) : SIPHeaderF("Route", header)
{
}

Route::Route(
  const MimeHeader & header
) : SIPHeaderF("Route", header)
{
}

Route::Route(
  const PStringArray & header
) : SIPHeaderF("Route", header)
{
}

Route::Route(
  const RouteURI::Collection & uri
) : SIPHeaderF("Route", uri)
{
}

RouteList::RouteList()
{
}

RouteList::RouteList(
  const SIPHeaderFList & list
) : SIPHeaderFList( list )
{
}

RouteList::RouteList(
  const SIPHeaderF::Collection & list
) : SIPHeaderFList( list )
{
}


////////////////////////////////////////////////////////////
//CREATE_ROUTE_HEADER( RecordRoute, "Record-Route" );
RecordRoute::RecordRoute() 
  : SIPHeaderF("Record-Route")
{
}

RecordRoute::RecordRoute(
  const SIPHeaderF & header
) : SIPHeaderF("Record-Route", header)
{
}

RecordRoute::RecordRoute(
  const PString & header
) : SIPHeaderF("Record-Route", header)
{
}

RecordRoute::RecordRoute(
  const MimeHeader & header
) : SIPHeaderF("Record-Route", header)
{
}

RecordRoute::RecordRoute(
  const PStringArray & header
) : SIPHeaderF("Record-Route", header)
{
}

RecordRoute::RecordRoute(
  const RouteURI::Collection & uri
) : SIPHeaderF("Record-Route", uri)
{
}

RecordRouteList::RecordRouteList()
{
}

RecordRouteList::RecordRouteList(
  const SIPHeaderFList & list
) : SIPHeaderFList( list )
{
}

RecordRouteList::RecordRouteList(
  const SIPHeaderF::Collection & list
) : SIPHeaderFList( list )
{
}


////////////////////////////////////////////////////////////
//PATH Header (RFC3327)
Path::Path() 
  : SIPHeaderF("Path")
{
}

Path::Path(
  const SIPHeaderF & header
) : SIPHeaderF("Path", header)
{
}

Path::Path(
  const PString & header
) : SIPHeaderF("Path", header)
{
}

Path::Path(
  const MimeHeader & header
) : SIPHeaderF("Path", header)
{
}

Path::Path(
  const PStringArray & header
) : SIPHeaderF("Path", header)
{
}

Path::Path(
  const RouteURI::Collection & uri
) : SIPHeaderF("Path", uri)
{
}

PathList::PathList()
{
}

PathList::PathList(
  const SIPHeaderFList & list
) : SIPHeaderFList( list )
{
}

PathList::PathList(
  const SIPHeaderF::Collection & list
) : SIPHeaderFList( list )
{
}
////////////////////////////////////////////////////////////

Event::Event() 
  : SIPHeader( "Event", "" ) 
{ 
}

Event::Event( 
  const PString & body
) : SIPHeader( "Event", body ) 
{ 
}

Event::Event( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

Event::Event( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}

PString Event::GetParameter( 
  const PString & paramName
)const
{
  PStringArray tokens = m_HeaderBody.Tokenise( ";" );
  if( tokens.GetSize() == 0 )
    return PString::Empty();

  for( PINDEX i = 1; i < tokens.GetSize(); i++ )
  {
    MimeParam param = tokens[i];
    if( param.GetParamName() *= paramName )
      return param.GetParamValue();
  }

  return PString::Empty();
}

void Event::SetParameter(
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

PString Event::GetEventName()const
{
  PStringArray tokens = m_HeaderBody.Tokenise( ";" );
  if( tokens.GetSize() <= 1 )
    return m_HeaderBody;

  return tokens[0];
}

void Event::SetEventName( 
  const PString & eventName
)
{
  PStringArray tokens = m_HeaderBody.Tokenise( ";" );
  if( tokens.GetSize() == 0 )
    return;

  tokens[0] = eventName;
  PStringStream newVal;
  for( PINDEX i = 0; i < tokens.GetSize(); i++ )
  {
     newVal << tokens[i];

     if( i < tokens.GetSize() - 1 )
       newVal << ";";
  }

  m_HeaderBody = newVal;
}

PString Event::GetEventId()const
{
  return GetParameter( "id" );
}

void Event::SetEventId( 
  const PString & eventId 
)
{
  SetParameter( "id", eventId );
}

///////////////////////////////////////////////////
SIPETag::SIPETag() 
  : SIPHeader( "SIP-ETag", "" ) 
{ 
}

SIPETag::SIPETag( 
  const PString & body
) : SIPHeader( "SIP-ETag", body ) 
{ 
}

SIPETag::SIPETag( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

SIPETag::SIPETag( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}
///////////////////////////////////////////////////
SIPIfMatch::SIPIfMatch() 
  : SIPHeader( "SIP-If-Match", "" ) 
{ 
}

SIPIfMatch::SIPIfMatch( 
  const PString & body
) : SIPHeader( "SIP-If-Match", body ) 
{ 
}

SIPIfMatch::SIPIfMatch( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

SIPIfMatch::SIPIfMatch( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}

///////////////////////////////////////////////////


AllowEvents::AllowEvents() 
  : SIPHeaderC( "Allow-Events" )
{
}

AllowEvents::AllowEvents( 
  const AllowEvents & header 
) : SIPHeaderC( "Allow-Events", (SIPHeaderC&)header )
{
}

AllowEvents::AllowEvents( 
  const PString & header 
) : SIPHeaderC( "Allow-Events", header )
{
}

AllowEvents::AllowEvents( 
  const MimeHeader & header 
) : SIPHeaderC( "Allow-Events", header )
{
}

AllowEvents::AllowEvents( 
  const PStringArray & header 
) : SIPHeaderC( "Allow-Events", header )
{
}

AllowEventsList::AllowEventsList()
{
}

AllowEventsList::AllowEventsList(
  const SIPHeaderCList & list
) : SIPHeaderCList( list )
{
}

AllowEventsList::AllowEventsList(
  const SIPHeaderC::Collection & list
) : SIPHeaderCList(list)
{
}

////////////////////////////////////////////////////////////

SubscriptionState::SubscriptionState() 
  : SIPHeader( "Subscription-State", "" ) 
{ 
}

SubscriptionState::SubscriptionState( 
  const PString & body
) : SIPHeader( "Subscription-State", body ) 
{ 
}

SubscriptionState::SubscriptionState( 
  const MimeHeader & value 
) : SIPHeader( value )
{
}

SubscriptionState::SubscriptionState( 
  const SIPHeader & header 
) : SIPHeader( header )
{
}

///////////////////////////////////////////////////////////


PAssertedIdentity::PAssertedIdentity() 
  : SIPHeaderF( "P-Asserted-Identity" )
{
}

PAssertedIdentity::PAssertedIdentity(const SIPHeaderF & header)
  : SIPHeaderF( "P-Asserted-Identity", header )
{
}

PAssertedIdentity::PAssertedIdentity(const PString & header)
  : SIPHeaderF( "P-Asserted-Identity", header )
{
}

PAssertedIdentity::PAssertedIdentity(const MimeHeader & header)
  : SIPHeaderF( "P-Asserted-Identity", header )
{
}

PAssertedIdentity::PAssertedIdentity(const PStringArray & header)
  : SIPHeaderF( "P-Asserted-Identity", header )
{
}

PAssertedIdentity::PAssertedIdentity(const RouteURI::Collection & uri)
  : SIPHeaderF( "P-Asserted-Identity", uri )
{
}

///////////////////////////////////////////////////////////


PAssertedIdentityList::PAssertedIdentityList()
{
}

PAssertedIdentityList::PAssertedIdentityList(const SIPHeaderFList & list)
  : SIPHeaderFList( list )
{
}

PAssertedIdentityList::PAssertedIdentityList(const SIPHeaderF::Collection & list)
  : SIPHeaderFList( list )
{
}


 
///////////////////////////////////////////////////////////

PPreferredIdentity::PPreferredIdentity()
  : SIPHeaderF( "P-Preferred-Identity" )
{
}

PPreferredIdentity::PPreferredIdentity(const SIPHeaderF & header)
  : SIPHeaderF( "P-Preferred-Identity", header )
{
}

PPreferredIdentity::PPreferredIdentity(const PString & header)
  : SIPHeaderF( "P-Preferred-Identity", header )
{
}

PPreferredIdentity::PPreferredIdentity(const MimeHeader & header)
  : SIPHeaderF( "P-Preferred-Identity", header )
{
}

PPreferredIdentity::PPreferredIdentity(const PStringArray & header)
  : SIPHeaderF( "P-Preferred-Identity", header )
{
}

PPreferredIdentity::PPreferredIdentity(const RouteURI::Collection & uri)
  : SIPHeaderF( "P-Preferred-Identity", uri )
{
}


PPreferredIdentityList::PPreferredIdentityList()
{
}

PPreferredIdentityList::PPreferredIdentityList(const SIPHeaderFList & list)
  : SIPHeaderFList( list )
{
}
PPreferredIdentityList::PPreferredIdentityList(const SIPHeaderF::Collection & list)
  : SIPHeaderFList( list )
{
}

////////////////////////////////////////////////////////////

/// RFC3891 Replaces Header


Replaces::Replaces()
 : SIPHeader( "Replaces", "unset;to-tag=unset;from-tag=unset" ) 
{
}

Replaces::Replaces( const PString & body )
 : SIPHeader( "Replaces", body ) 
{
}

Replaces::Replaces( const MimeHeader & value )
 : SIPHeader( value ) 
{
}

Replaces::Replaces( const SIPHeader & header )
 : SIPHeader( header ) 
{
}

PString Replaces::GetCallId()const
{
  PStringArray tokens = m_HeaderBody.Tokenise( ";" );
  if( tokens.GetSize() == 0 )
    return PString::Empty();

  return tokens[0];
}

PString Replaces::GetFromTag()const
{
  return GetParameter( "from-tag" );
}

PString Replaces::GetToTag()const
{
  return GetParameter( "to-tag" );
}

PString Replaces::GetParameter( 
  const PString & paramName
)const
{
  PStringArray tokens = m_HeaderBody.Tokenise( ";" );
  if( tokens.GetSize() == 0 )
    return PString::Empty();

  for( PINDEX i = 1; i < tokens.GetSize(); i++ )
  {
    MimeParam param = tokens[i];
    if( param.GetParamName() *= paramName )
      return param.GetParamValue();
  }

  return PString::Empty();
}

BOOL Replaces::IsEarlyDialogFlagSet()const
{
  return m_HeaderBody.ToLower().Find( "early-only" ) != P_MAX_INDEX;
}

void Replaces::SetCallId( 
  const PString & callId
)
{
  PStringArray tokens = m_HeaderBody.Tokenise( ";" );
  if( tokens.GetSize() == 0 )
    return;

  tokens[0] = callId;
  PStringStream newVal;
  for( PINDEX i = 0; i < tokens.GetSize(); i++ )
  {
     newVal << tokens[i];

     if( i < tokens.GetSize() - 1 )
       newVal << ";";
  }

  m_HeaderBody = newVal;
}

void Replaces::SetFromTag(
  const PString & fromTag
)
{
  SetParameter( "from-tag", fromTag );
}

void Replaces::SetToTag(
  const PString & toTag
)
{
  SetParameter( "to-tag", toTag );
}

void Replaces::SetParameter(
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

void Replaces::SetEarlyDialogFlag(
  BOOL early
)
{
  PStringArray tokens = m_HeaderBody.Tokenise( ";" );
  if( tokens.GetSize() == 0 )
   return;

  for( PINDEX i = 1; i < tokens.GetSize(); i++ )
  {
    if( tokens[i].Trim().ToLower() *= "early-only" )
    {
      if( !early )
        tokens.RemoveAt(i);
    }
  }

  if( early )
    tokens.AppendString( "early-only" );

  PStringStream newVal;
  for( PINDEX j = 0; j < tokens.GetSize(); j++ )
  {
     newVal << tokens[j];
     if( j < tokens.GetSize() - 1 )
       newVal << ";";
  }

  m_HeaderBody = newVal;
}

PString Replaces::EscapeAsRFC2396()const
{
  return ParserTools::EscapeAsRFC2396( m_HeaderBody );
}

