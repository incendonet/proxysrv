/*
 *
 * SIPMessage.h
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
 * $Log: SIPMessage.h,v $
 * Revision 1.49  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.48  2006/11/22 03:16:13  joegenbaclor
 * Removed Record Routes from B2B Outbound invites
 *
 * Revision 1.47  2006/11/21 06:55:17  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.46  2006/11/18 03:14:26  joegenbaclor
 * Implemented session loop detection mechanism using iid via parameter
 *
 * Revision 1.45  2006/11/17 04:22:21  joegenbaclor
 * Added ability to remember core to use via m_UACoreName
 *
 * Revision 1.44  2006/11/08 06:46:20  joegenbaclor
 * Added session level NIC/Interface binding
 *
 * Revision 1.43  2006/11/07 09:30:28  joegenbaclor
 * Added new internal header "IFACE-ADDR" to allow transctions to remember the interface address for sending future responses within the transaction lifespan
 *
 * Revision 1.42  2006/11/02 22:53:46  joegenbaclor
 * Added Path header support (RFC 3327)
 *
 * Revision 1.41  2006/10/30 02:42:26  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.40  2006/10/23 15:44:54  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.39  2006/10/17 00:12:23  joegenbaclor
 * Added support for RFC 4028 SIP Session Timers header
 *
 * Revision 1.38  2006/08/18 07:12:23  joegenbaclor
 * 1.  added Function overload for GetContentType
 * 2.  Changed unix make files to not include opensbc in make process
 *
 * Revision 1.37  2006/08/14 01:29:06  joegenbaclor
 * Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
 *
 * Revision 1.36  2006/08/11 08:38:42  joegenbaclor
 * Initial upload of Presence package
 *
 * Revision 1.35  2006/08/09 12:17:46  joegenbaclor
 * Added RFC 3428 Support
 *
 * Revision 1.34  2006/07/28 04:14:17  joegenbaclor
 * Added function to get the size of Accept header if available
 *
 * Revision 1.33  2006/07/19 02:19:20  joegenbaclor
 * 1.  Bug fixes concerning B2BUA destruction during during 3xx, 4xx, 5xx, 6xx local response
 * 2.  Fixed bug where Route header is never removed from outbound Invite resulting that may result to a loop when
 *      Inbound Invite has a Route header present
 * 3.  Added RemoveAllRoutes() in SIPMessage
 *
 * Revision 1.32  2006/07/17 11:36:45  joegenbaclor
 * 1.  More routing enhancements to B2BUA
 *
 * Revision 1.31  2006/07/13 06:53:04  joegenbaclor
 * 1.  Introduced Sanity checks for incoming SIP Message
 * 2.  Corrected bug in SIPURI where enum scheme is not getting recognized.
 * 3.  Added ENUM support to routing
 * 4.  Introduced global routing flag to indicate when and not to rewrite outbound To URI's
 *
 * Revision 1.30  2006/07/06 05:38:10  joegenbaclor
 * Final last minute changes to CBCOM hash support
 *
 * Revision 1.29  2006/06/28 14:14:57  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.28  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
 *
 * Revision 1.27  2006/06/09 07:37:30  joegenbaclor
 * 1.  Added Publish, SIPETag & SIPIfMatch  support in SIPMessage
 * 2.  Started work on StatusPage Resource for OSSAppConfig
 *
 * Revision 1.26  2006/06/07 09:18:33  joegenbaclor
 * Added the ability to SIPMessage to read from a PChannel
 *
 * Revision 1.25  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.24  2006/04/26 00:51:09  joegenbaclor
 * 1.  Added IsMethod() and IsStatusCode() functions to get rid of repetitive code for determining Message class types
 * 2.  Added IsRefer()
 *
 * Revision 1.23  2006/04/21 09:55:09  joegenbaclor
 * Added RFC 3265 header requirements
 *
 * Revision 1.22  2006/04/21 04:12:01  joegenbaclor
 * Added Refer-To header support
 *
 * Revision 1.21  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.20  2006/03/27 01:25:33  joegenbaclor
 * Some improvements on NICT and NIST state machines
 *
 * Revision 1.19  2006/03/25 18:40:13  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.18  2006/03/21 14:11:50  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.17  2006/03/17 13:50:54  joegenbaclor
 * 1.  Implemented UAC Call Redirection
 * 2.  Implemented SendCancel() method in CallSession
 * 3.  Introduced ClearInternalHeaders() function in SIP Message to erase internal headers inheritted from previous transaction.
 *
 * Revision 1.16  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.15  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPMESSAGE_H
#define SIPMESSAGE_H

#include "MessageHeaders.h"
#include "ossbuildopts.h"

namespace SIPParser
{
  class SIPMessage : public PObject
  {
    PCLASSINFO( SIPMessage, PObject );
  public:

    enum StatusCodes
    {
      CodeUknown = 0,
      Code100_Trying = 100,
      Code_MinCode = Code100_Trying,
      Code180_Ringing = 180,
      Code181_BeingForwarded = 181,
      Code182_Queued = 182,
      Code183_SessionProgress = 183,
      Code200_Ok = 200,
      Code202_Accepted = 202,
      Code300_MultipleChoices = 300,
      Code301_MovedPermanently = 301,
      Code302_MovedTemporarily = 302,
      Code305_UseProxy = 305,
      Code380_AlternativeService = 380,
      Code400_BadRequest = 400,
      Code401_Unauthorized = 401,
      Code402_PaymentRequired = 402,
      Code403_Forbidden = 403,
      Code404_NotFound = 404,
      Code405_MethodNotAllowed = 405,
      Code406_NotAcceptable = 406,
      Code407_ProxyAuthenticationRequired = 407,
      Code408_RequestTimeout = 408,
      Code409_Conflict = 409,
      Code410_Gone = 410,
      Code411_LengthRequired = 411,
      Code412_ConditionalRequestFailed = 412,
      Code413_RequestEntityTooLarge = 413,
      Code414_RequestURITooLarge = 414,
      Code415_UnsupportedMedia = 415,
      Code416_UnsupportedURIScheme = 416,
      Code420_BadExtension = 420,
      Code422_SessionIntervalTooSmall = 422,
      Code423_IntervalTooShort = 423,
      Code480_TemporarilyNotAvailable = 480,
      Code481_TransactionDoesNotExist = 481,
      Code482_LoopDetected = 482,
      Code483_TooManyHops = 483,
      Code484_AddressIncomplete = 484,
      Code485_Ambiguous = 485,
      Code486_BusyHere = 486,
      Code487_RequestCancelled = 487,
      Code488_NotAcceptableHere = 488,
      Code489_BadRequest = 489,
      code489_BadEvent = Code489_BadRequest,
      Code500_InternalServerError = 500,
      Code501_NotImplemented = 501,
      Code502_BadGateway = 502,
      Code503_ServiceUnavailable = 503,
      Code504_GatewayTimeout = 504,
      Code505_VersionNotSupported = 505,
      Code600_BusyEverywhere = 600,
      Code603_Decline = 603,
      Code604_DoesNotExistAnywhere = 604,
      Code606_NotAcceptable = 606,
      Code_MaxCode = 699
    };

    enum Methods
    {
      Method_ACK,
      Method_BYE,
      Method_CANCEL,
      Method_INFO,
      Method_INVITE,
      Method_MESSAGE,
      Method_NOTIFY,
      Method_OPTIONS,
      Method_PRACK,
      Method_PUBLISH,
      Method_REFER,
      Method_REGISTER,
      Method_SUBSCRIBE,
      Method_UPDATE,
      NumMethods
    };

    PQUEUE( Queue, SIPMessage );

    /*
    class AutoCriticalSection
    {
    public:
      AutoCriticalSection( const SIPMessage & c ) : m_C(c) { m_C.EnterCriticalSection(); }
      virtual ~AutoCriticalSection(){ m_C.LeaveCriticalSection(); };
    private:
      const SIPMessage & m_C;
    };
    */

    SIPMessage();

    SIPMessage(
      const SIPMessage & msg
    );

    SIPMessage(
      const PString & msg
    );

    SIPMessage(
      const PBYTEArray & msg
    );

    virtual ~SIPMessage();

    SIPMessage & operator=(
      const SIPMessage & msg
    );

    SIPMessage & operator=(
      const PString & msg
    );

    SIPMessage & operator=(
      const PBYTEArray & msg
    );


    BOOL CreateResponse(
      SIPMessage & response,
      WORD statusCode,
      const PString & reasonPhrase = PString::Empty()
    )const;

    ///////////////////////////////////////////////
    /// StartLine Accessors
    BOOL GetStartLine(
      StartLine & startLine
    )const;

    PString GetStartLine()const;

    PString GetMethod()const;

    void SetStartLine(
      const StartLine & startLine
    );

    void SetStartLine(
      const StatusLine & statusLine
    );

    void SetStartLine(
      const RequestLine & requestLine
    );

    WORD GetStatusCode()const;

    BOOL GetStatusLine(
      StatusLine & statusLine
    )const;

    BOOL GetRequestLine(
      RequestLine & requestLine
    )const;

    PString GetBody()const;

    BOOL GetBody(
      PString & body
    )const;

    void SetBody(
      const PString & body
    );

    void SetSDP(
      const PString & sdp 
    );


    BOOL HasBody()const;

    BOOL HasSDP()const;


  /// Standad SIP Headers
    ///AlertInfo * m_AlertInfo;
    BOOL GetAlertInfo(
      AlertInfo & h
    )const;

    void SetAlertInfo(
      const AlertInfo & h
    );

    BOOL HasAlertInfo()const;

    ///CallId * m_CallId;
    BOOL GetCallId(
      CallId & h
    )const;

    PString GetCallId()const;

    void SetCallId(
      const CallId & h
    );

    BOOL HasCallId()const;

    ///CallInfo * m_CallInfo;
    BOOL GetCallInfo(
      CallInfo & h
    )const;

    void SetCallInfo(
      const CallInfo & h
    );

    BOOL HasCallInfo()const;

    ///ContentDisposition * m_ContentDisposition;
    BOOL GetContentDisposition(
      ContentDisposition & h
    )const;

    void SetContentDisposition(
      const ContentDisposition & h
    );

    BOOL HasContentDisposition()const;

    ///ContentLanguage * m_ContentLanguage;
    BOOL GetContentLanguage(
      ContentLanguage & h
    )const;

    void SetContentLanguage(
      const ContentLanguage & h
    );

    BOOL HasContentLanguage()const;

    ///ContentLength * m_ContentLength;
    BOOL GetContentLength(
      ContentLength & h
    )const;

    void SetContentLength(
      const ContentLength & h
    );

    void SetContentLength(
      DWORD len
    );

    BOOL HasContentLength()const;

    ///CSeq
    BOOL GetCSeq(
      CSeq & h
    )const;

    PString GetCSeqMethod()const;

    DWORD GetCSeqNumber()const;

    void SetCSeq(
      const CSeq & h
    );

    BOOL HasCSeq()const;

    ///ContentType * m_ContentType;
    PString GetContentType()const;

    BOOL GetContentType(
      ContentType & h
    )const;

    void SetContentType(
      const PString & contentType
    );

    void SetContentType(
      const ContentType & h
    );

    BOOL HasContentType()const;

    ///Date * m_Date;
    BOOL GetDate(
      Date & h
    )const;

    void SetDate(
      const Date & h
    );

    BOOL HasDate()const;

    ///ErrorInfo * m_ErrorInfo;
    BOOL GetErrorInfo(
      ErrorInfo & h
    )const;

    void SetErrorInfo(
      const ErrorInfo & h
    );

    BOOL HasErrorInfo()const;

    ///MIMEVersion * m_MIMEVersion;
    BOOL GetMIMEVersion(
      MIMEVersion & h
    )const;

    void SetMIMEVersion(
      const MIMEVersion & h
    );

    BOOL HasMIMEVersion()const;

    ///Event * m_Event;
    BOOL GetEvent(
      Event & h
    )const;

    void SetEvent(
      const Event & h
    );

    BOOL HasEvent()const;

    ///SIPETag;
    BOOL GetSIPETag(
      SIPETag & h
    )const;

    PString GetSIPETag()const;

    void SetSIPETag(
      const SIPETag & h
    );

    void SetSIPETag(
      const PString & etag
    );

    BOOL HasSIPETag()const;

    ///SIPIfMatch;
    BOOL GetSIPIfMatch(
      SIPIfMatch & h
    )const;

    PString GetSIPIfMatch()const;

    void SetSIPIfMatch(
      const SIPIfMatch & h
    );

    void SetSIPIfMatch(
      const PString & etag
    );

    BOOL HasSIPIfMatch()const;

    ///SubscriptionState * m_SubscriptionState;
    BOOL GetSubscriptionState(
      SubscriptionState & h
    )const;

    void SetSubscriptionState(
      const SubscriptionState & h
    );

    BOOL HasSubscriptionState()const;

    ///Organization * m_Organization;
    BOOL GetOrganization(
      Organization & h
    )const;

    void SetOrganization(
      const Organization & h
    );

    BOOL HasOrganization()const;

    ///Priority * m_Priority;
    BOOL GetPriority(
      Priority & h
    )const;

    void SetPriority(
      const Priority & h
    );

    BOOL HasPriority()const;

    ///RetryAfter * m_RetryAfter;
    BOOL GetRetryAfter(
      RetryAfter & h
    )const;

    void SetRetryAfter(
      const RetryAfter & h
    );

    BOOL HasRetryAfter()const;

    ///Server * m_Server;
    BOOL GetServer(
      Server & h
    )const;

    void SetServer(
      const Server & h
    );

    BOOL HasServer()const;

    ///Subject * m_Subject;
    BOOL GetSubject(
      Subject & h
    )const;

    void SetSubject(
      const Subject &h
    );

    BOOL HasSubject()const;

    ///UserAgent * m_UserAgent;
    BOOL GetUserAgent(
      UserAgent & h
    )const;

    void SetUserAgent(
      const UserAgent & h
    );

    BOOL HasUserAgent()const;

    ///Warning * m_Warning;
    BOOL GetWarning(
      Warning & h
    )const;

    void SetWarning(
      const Warning & h
    );

    BOOL HasWarning()const;

    /// URI headers
    ///From * m_From;
    BOOL GetFrom(
      From & h
    )const;

    void SetFromTag( 
      const PString & tag 
    );

    PString GetFromTag()const;

    SIPURI GetFromURI()const;

    void SetFrom(
      const From & h
    );

    BOOL HasFrom()const;

    ///To * m_To;
    BOOL GetTo(
      To & h
    )const;

    PString GetToTag()const;

    SIPURI GetToURI()const;

    void SetToTag( 
      const PString & tag
    );

    void SetTo(
      const To & h
    );

    BOOL HasTo()const;

    ///ReferTo * m_ReferTo;
    BOOL GetReferTo(
      ReferTo & h
    )const;


    void SetReferTo(
      const ReferTo & h
    );

    BOOL HasReferTo()const;

    /////////////////////////
    BOOL GetReplaces(
      Replaces & h
    )const;


    void SetReplaces(
      const Replaces & h
    );

    BOOL HasReplaces()const;

    ///ReplyTo * m_ReplyTo;
    BOOL GetReplyTo(
      ReplyTo & to
    )const;

    void SetReplyTo(
      const ReplyTo & h
    );

    BOOL HasReplyTo()const;

    /// Integer headers
    ///Expires * m_Expires;
    BOOL GetExpires(
      Expires & h
    )const;

    void SetExpires(
      const Expires & h
    );

    BOOL HasExpires()const;

    ///MaxForwards * m_MaxForwards;
    BOOL GetMaxForwards(
      MaxForwards & h
    )const;

    void SetMaxForwards(
      const MaxForwards & h
    );

    void SetMaxForwards( 
      WORD maxForward 
    );

    BOOL HasMaxForwards()const;


    BOOL GetMinExpires(
      MinExpires & h
    )const;

    void SetMinExpires(
      const MinExpires & h
    );

    BOOL HasMinExpires()const;

    BOOL GetMinSE(
      MinSE & h
    )const;

    void SetMinSE(
      const MinSE & h
    );

    BOOL HasMinSE()const;

    BOOL GetSessionExpires(
      SessionExpires & h
    )const;

    void SetSessionExpires(
      const SessionExpires & h
    );

    BOOL HasSessionExpires()const;

    ///TimeStamp * m_TimeStamp;
    BOOL GetTimeStamp(
      TimeStamp & h
    )const;

    void SetTimeStamp(
      const TimeStamp & h
    );

    BOOL HasTimeStamp()const;

    /// Comma delimited named parameters
    ///AuthenticationInfo * m_AuthenticationInfo;
    BOOL GetAuthenticationInfo(
      AuthenticationInfo & h
    )const;

    void SetAuthenticationInfo(
      const AuthenticationInfo & h
    );

    BOOL HasAuthenticationInfo()const;

    ///Authorization * m_Authorization;
    BOOL GetAuthorization(
      Authorization & h
    )const;

    void SetAuthorization(
      const Authorization & h
    );

    BOOL HasAuthorization()const;

    ///ProxyAuthenticate * m_ProxyAuthenticate;
    BOOL GetProxyAuthenticate(
      ProxyAuthenticate & h
    )const;

    void SetProxyAuthenticate(
      const ProxyAuthenticate & h
    );

    BOOL HasProxyAuthenticate()const;

    ///ProxyAuthorization * m_ProxyAuthorization;
    BOOL GetProxyAuthorization(
      ProxyAuthorization & h
    )const;

    void SetProxyAuthorization(
      const ProxyAuthorization & h
    );

    BOOL HasProxyAuthorization()const;

    ///WWWAuthenticate * m_WWWAuthenticate;

    BOOL GetWWWAuthenticate(
      WWWAuthenticate & h
    )const;

    void SetWWWAuthenticate(
      const WWWAuthenticate & h
    );

    BOOL HasWWWAuthenticate()const;

    /// Comma delimited headers


    /// appends a new token at the header specified by idx
    BOOL AppendAccept(
      const PString & token,
      PINDEX idx = 0
    );

    /// appends a new header at the end of the list
    BOOL AppendAccept(
      const Accept & header
    );

    /// returns the number of accept
    /// headers.  Take note that a single
    /// accept header may contain more
    /// than one comma delimited values
    PINDEX GetAcceptSize()const;

    /// returns the header from the address specified by idx
    Accept & GetAcceptAt(
      PINDEX idx
    )const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetAcceptTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    /// Clears the comma delimited list
    BOOL RemoveAllAccept();

    //$$$$$$$$$$$$$
       /// appends a new token at the header specified by idx
    BOOL AppendAllowEvents(
      const PString & token,
      PINDEX idx = 0
    );

    /// appends a new header at the end of the list
    BOOL AppendAllowEvents(
      const AllowEvents & header
    );

    /// returns the header from the address specified by idx
    AllowEvents & GetAllowEventsAt(
      PINDEX idx
    )const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetAllowEventsTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    /// Clears the comma delimited list
    BOOL RemoveAllAllowEvents();
    //$$$$$$$$$$$$$

    //////////
    /// appends a new token at the header specified by idx
    BOOL AppendAcceptEncoding(
      const PString & token,
      PINDEX idx = 0
    );

    /// appends a new header at the end of the list
    BOOL AppendAcceptEncoding(
      const AcceptEncoding & header
    );

    /// returns the header from the address specified by idx
    AcceptEncoding & GetAcceptEncodingAt(
      PINDEX idx
    )const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetAcceptEncodingTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    /// Clears the comma delimited list
    BOOL RemoveAllAcceptEncoding();

    ///AcceptLanguageList * m_AcceptLanguageList;
    /// appends a new token at the header specified by idx
    BOOL AppendAcceptLanguage(
      const PString & token,
      PINDEX idx = 0
    );

    /// appends a new header at the end of the list
    BOOL AppendAcceptLanguage(
      const AcceptLanguage & header
    );

    /// returns the header from the address specified by idx
    AcceptLanguage & GetAcceptLanguageAt(
      PINDEX idx
    )const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetAcceptLanguageTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    /// Clears the comma delimited list
    BOOL RemoveAllAcceptLanguage();

    ///AllowList * m_AllowList;
    BOOL AppendAllow(
      const PString & token,
      PINDEX idx = 0
    );

    /// appends a new header at the end of the list
    BOOL AppendAllow(
      const Allow & header
    );

    /// returns the header from the address specified by idx
    Allow & GetAllowAt(
      PINDEX idx
    )const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetAllowTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    /// Clears the comma delimited list
    BOOL RemoveAllAllow();

    ///ContentEncodingList * m_ContentEncodingList;
    BOOL AppendContentEncoding(
      const PString & token,
      PINDEX idx = 0
    );

    /// appends a new header at the end of the list
    BOOL AppendContentEncoding(
      const ContentEncoding & header
    );

    /// returns the header from the address specified by idx
    ContentEncoding & GetContentEncodingAt(
      PINDEX idx
    )const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetContentEncodingTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    /// Clears the comma delimited list
    BOOL RemoveAllContentEncoding();

    ///ContactList * m_ContactList;
    BOOL AppendContact(
      const PString & token,
      PINDEX idx = 0
    );



    BOOL SetContactList(
      const ContactList & contactList
    );

    ContactList GetContactList()const;

    /// appends a new header at the end of the list
    BOOL AppendContact(
      const Contact & header
    );

    BOOL AppendContact(
      const ContactURI & contactURI
    );

    /// returns the top most contact URI
    SIPURI GetTopContactURI()const;

    /// returns the header from the address specified by idx
    Contact & GetContactAt(
      PINDEX idx
    )const;

    SIPURI GetContactTopURI()const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetContactTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    PINDEX GetContactSize()const;

    BOOL RemoveAllContact();

    ///InReplyToList * m_InReplyToList;
    BOOL AppendInReplyTo(
      const PString & token,
      PINDEX idx = 0
    );

    /// appends a new header at the end of the list
    BOOL AppendInReplyTo(
      const InReplyTo & header
    );

    /// returns the header from the address specified by idx
    InReplyTo & GetInReplyToAt(
      PINDEX idx
    )const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetInReplyToTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    /// Clears the comma delimited list
    BOOL RemoveAllInReplyTo();

    ///ProxyRequireList * m_ProxyRequireList;
    BOOL AppendProxyRequire(
      const PString & token,
      PINDEX idx = 0
    );

    /// appends a new header at the end of the list
    BOOL AppendProxyRequire(
      const ProxyRequire & header
    );

    /// returns the header from the address specified by idx
    ProxyRequire & GetProxyRequireAt(
      PINDEX idx
    )const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetProxyRequireTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    /// Clears the comma delimited list
    BOOL RemoveAllProxyRequire();


    ///RequireList * m_RequireList;
    BOOL AppendRequire(
      const PString & token,
      PINDEX idx = 0
    );

    /// appends a new header at the end of the list
    BOOL AppendRequire(
      const Require & header
    );

    /// returns the header from the address specified by idx
    Require & GetRequireAt(
      PINDEX idx
    )const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetRequireTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    /// Clears the comma delimited list
    BOOL RemoveAllRequire();

    ///SupportedList * m_SupportedList;
    BOOL AppendSupported(
      const PString & token,
      PINDEX idx = 0
    );

    /// appends a new header at the end of the list
    BOOL AppendSupported(
      const Supported & header
    );

    /// returns supported extensions as a string array
    PStringArray GetSupported()const;

    /// returns TRUE if extension is listend in the supported header
    BOOL IsSupportedExtension( 
      const PString & extensionName 
    )const;

    /// returns the header from the address specified by idx
    Supported & GetSupportedAt(
      PINDEX idx
    )const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetSupportedTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    /// Clears the comma delimited list
    BOOL RemoveAllSupported();

    ///UnsupportedList * m_UnsupportedList;
    BOOL AppendUnsupported(
      const PString & token,
      PINDEX idx = 0
    );

    /// appends a new header at the end of the list
    BOOL AppendUnsupported(
      const Unsupported & header
    );

    /// returns the header from the address specified by idx
    Unsupported & GetUnsupportedAt(
      PINDEX idx
    )const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetUnsupportedTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    /// Clears the comma delimited list
    BOOL RemoveAllUnsupported();

    BOOL AppendRoute(
      const RouteURI & uri,
      PINDEX idx = 0
    );

    BOOL AppendRoute(
      const Route & header
    );

    RouteList GetRouteList()const;

    BOOL SetRouteList(
      const RouteList & list
    );

    Route GetRouteAt(
      PINDEX idx
    )const;

    RouteURI GetRouteTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    RouteURI PopTopRouteURI();

    RouteURI PopLastRouteURI();

    BOOL HasRoute()const;

    PINDEX GetRouteSize()const;

    BOOL RemoveAllRoutes();

    BOOL AppendRecordRoute(
      const RouteURI & uri,
      PINDEX idx = 0
    );

    BOOL AppendRecordRoute(
      const RecordRoute & header
    );

    BOOL RemoveAllRecordRoutes();

    RecordRouteList GetRecordRouteList()const;

    BOOL SetRecordRouteList(
      const RecordRouteList & list
    );

    RecordRoute GetRecordRouteAt(
      PINDEX idx
    )const;

    RouteURI GetRecordRouteTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    RouteURI PopTopRecordRouteURI();

    PINDEX GetRecordRouteSize()const;

    BOOL CloneRecordRoutes(
      SIPMessage & msg
    )const;

    ///PATH
    BOOL AppendPath(
      const RouteURI & uri,
      PINDEX idx = 0
    );

    BOOL AppendPath(
      const Path & header
    );

    PathList GetPathList()const;

    BOOL SetPathList(
      const PathList & list
    );

    Path GetPathAt(
      PINDEX idx
    )const;

    RouteURI GetPathTokenAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    RouteURI PopTopPathURI();

    PINDEX GetPathSize()const;

    BOOL ClonePaths(
      SIPMessage & msg
    )const;

    /// Via

    BOOL FixViaNATParameters();

    ViaList GetViaList()const;

    BOOL SetViaList(
      const ViaList & list
    );

    BOOL AppendVia(
      const Via & header
    );

    BOOL GetViaAt(
      PINDEX idx,
      Via & via
    )const;

    BOOL SetViaAt(
      PINDEX idx,
      const Via & header
    );

    Via PopTopVia();

    void RemoveAllVias();

    PINDEX GetViaSize()const;

    BOOL CloneVias(
      SIPMessage & msg
    )const;

    BOOL SetViaParameter(
      const PString & name,
      const PString & value
    );

    /// Custom Headers
    ///PLIST( CustomHeaders, SIPHeader );
    ///CustomHeaders * m_CustomHeaders;
    BOOL GetCustomHeader(
      const PCaselessString & name,
      SIPHeader & h,
      PINDEX i = 0
    )const;

    BOOL GetCustomHeader(
      SIPHeader & h,
      PINDEX i
    )const;


    void SetCustomHeader(
      const PCaselessString & name,
      const SIPHeader & h,
      PINDEX i = 0
    );

    void SetCustomHeader(
      const SIPHeader & h,
      PINDEX i
    );

    void AddCustomHeader(
      const SIPHeader & h
    );

    BOOL PopCustomHeader(
      const PCaselessString & name,
      SIPHeader & h,
      PINDEX i = 0
    );

    ///////
    BOOL HasInternalHeader(
      const PCaselessString & name
    )const;

    BOOL GetInternalHeader(
      const PCaselessString & name,
      SIPHeader & h,
      PINDEX i = 0
    )const;

    PString GetInternalHeader(
      const PCaselessString & name,
      PINDEX i = 0
    )const;

    BOOL GetInternalHeader(
      SIPHeader & h,
      PINDEX i
    )const;

    void SetInternalHeader( 
      const PCaselessString & name,
      const PString & value,
      PINDEX index = 0
    );

    void SetInternalHeader(
      const PCaselessString & name,
      const SIPHeader & h,
      PINDEX i = 0
    );

    void SetInternalHeader(
      const SIPHeader & h,
      PINDEX i
    );

    void ClearInternalHeaders();

    void AddInternalHeader(
      const SIPHeader & h
    );

    void AddInternalHeader(
      const PString & name,
      const PString & value
    );

    BOOL PopInternalHeader(
      const PCaselessString & name,
      SIPHeader & h,
      PINDEX i = 0
    );

    BOOL GetInternalHeader(
      const PCaselessString & name,
      SIPHeader & h,
      PINDEX i = 0
    );

    BOOL SetSendAddress(
      const SIPURI & address
    );
    ///////

    TransactionId GetTransactionId()const;

    BOOL IsRequest()const;

    BOOL IsMethod(
      const PCaselessString & meth
    ) const;

    BOOL IsStatusCode(
      int baseCode
    ) const;
    
    BOOL IsInvite()const;

    BOOL IsAck()const;

    BOOL IsBye()const;

    BOOL IsOptions()const;

    BOOL IsInfo()const;

    BOOL IsRegister()const;

    BOOL IsSubscribe()const;

    BOOL IsUnsubscribe()const;

    BOOL IsNotify()const;

    BOOL IsRefer()const;

    BOOL IsMessage()const;

    BOOL IsPublish()const;

    BOOL IsUnregister()const;

    BOOL IsUnpublish()const;

    BOOL IsUpdate()const;

    BOOL IsCancel()const;

    BOOL IsKeepAlive()const;

	BOOL IsBusyHere()const;

	BOOL IsOkToInvite()const;

    BOOL Is1xx()const;

    BOOL Is2xx()const;

    BOOL Is3xx()const;

    BOOL Is4xx()const;

    BOOL Is5xx()const;

    BOOL Is6xx()const;

    BOOL IsValid( 
      BOOL minimal = TRUE 
    )const;

    BOOL IsXORHashSet()const;

    BOOL SetXORHash( 
      BOOL willHash
    );

    BOOL GetXORHash( 
      PBYTEArray & out 
    )const;


	  PString GetInterfaceAddress()const;

	  void SetInterfaceAddress(
		  const PString & ifaceAddress
	  );

    WORD GetInterfacePort()const;

	  void SetInterfacePort(
		  WORD port
	  );

    PString GetUACoreName()const;

	  void SetUACoreName(
		  const PString & name
	  );


    //////////////////////////////////////////////
    void Finalize();

    virtual void PrintOn( 
      ostream & strm 
    )const;

    BOOL Read(
      PChannel & inputStream
    );

    BOOL Write(
      PChannel & outputStream
    );

    virtual PObject * Clone()const;

    PString AsString()const;

    void MakeUnique();

    PINLINE BOOL IsUnique()const{ return m_IsUnique; };

    PINLINE BOOL IsFinal()const{ return m_IsFinal; };

  protected:

    void Parse(
      const PString & msg
    );

    void Parse(
      const PBYTEArray & msg
    );

    void AddRef(
      SIPMessage & msg
    )const;

    void AssignContents(
      SIPMessage & msg
    )const;

    void ReleaseRef();

    void Cleanup();

    void ZeroObjects();

    
  protected:

    BOOL ParseAlertInfo();
    BOOL ParseCallId();
    BOOL ParseCallInfo();
    BOOL ParseContentDisposition();
    BOOL ParseContentLanguage();
    BOOL ParseContentLength();
    BOOL ParseContentType();
    BOOL ParseCSeq();
    BOOL ParseDate();
    BOOL ParseErrorInfo();
    BOOL ParseMIMEVersion();
    BOOL ParseEvent();
    BOOL ParseSIPETag();
    BOOL ParseSIPIfMatch();
    BOOL ParseSubscriptionState();
    BOOL ParseOrganization();
    BOOL ParsePriority();
    BOOL ParseRetryAfter();
    BOOL ParseServer();
    BOOL ParseSubject();
    BOOL ParseUserAgent();
    BOOL ParseWarning();

    /// URI headers
    BOOL ParseFrom();
    BOOL ParseTo();
    BOOL ParseReplyTo();
    BOOL ParseReplaces();
    BOOL ParseReferTo();

    /// Integer headers
    BOOL ParseExpires();
    BOOL ParseMaxForwards();
    BOOL ParseMinExpires();
    BOOL ParseTimeStamp();
    BOOL ParseMinSE();
    BOOL ParseSessionExpires();

    /// Comma delimited named parameters
    BOOL ParseAuthenticationInfo();
    BOOL ParseAuthorization();
    BOOL ParseProxyAuthenticate();
    BOOL ParseProxyAuthorization();
    BOOL ParseWWWAuthenticate();

    /// Comma delimited headers

    BOOL ParseAcceptList();
    BOOL ParseAllowEventsList();
    BOOL ParseAcceptEncodingList();
    BOOL ParseAcceptLanguageList();
    BOOL ParseAllowList();
    BOOL ParseContentEncodingList();
    BOOL ParseContactList();
    BOOL ParseInReplyToList();
    BOOL ParseProxyRequireList();
    BOOL ParseRequireList();
    BOOL ParseSupportedList();
    BOOL ParseUnsupportedList();

    /// Route Headers
    BOOL ParseRouteList();
    BOOL ParseRecordRouteList();
    BOOL ParsePathList();

    /// special headers
    BOOL ParseViaList();
    BOOL ParseCustomHeaders();

  public:
    /// static mathods
    static PString GetMethodString( Methods method );
  protected:

    mutable BOOL m_IsUnique;
    mutable BOOL m_IsFinal;
    int m_InternalId;
    mutable PAtomicInteger * m_RefCounter;

    PStringArray * m_RawMessage;
    PString * m_RawMessageBody;

    StartLine * m_StartLine;

  /// Standad SIP Headers
    AlertInfo * m_AlertInfo;
    CallId * m_CallId;
    CallInfo * m_CallInfo;
    ContentDisposition * m_ContentDisposition;
    ContentLanguage * m_ContentLanguage;
    ContentLength * m_ContentLength;
    ContentType * m_ContentType;
    Date * m_Date;
    ErrorInfo * m_ErrorInfo;
    MIMEVersion * m_MIMEVersion;
    Organization * m_Organization;
    Priority * m_Priority;
    RetryAfter * m_RetryAfter;
    Server * m_Server;
    Subject * m_Subject;
    UserAgent * m_UserAgent;
    Warning * m_Warning;

    /// URI headers
    From * m_From;
    To * m_To;
    ReplyTo * m_ReplyTo;

    /// Integer headers
    Expires * m_Expires;
    MaxForwards * m_MaxForwards;
    MinExpires * m_MinExpires;
    TimeStamp * m_TimeStamp;
    MinSE * m_MinSE;
    SessionExpires * m_SessionExpires;

    /// Comma delimited named parameters
    AuthenticationInfo * m_AuthenticationInfo;
    Authorization * m_Authorization;
    ProxyAuthenticate * m_ProxyAuthenticate;
    ProxyAuthorization * m_ProxyAuthorization;
    WWWAuthenticate * m_WWWAuthenticate;

    /// Comma delimited headers

    AcceptList * m_AcceptList;
    AcceptEncodingList * m_AcceptEncodingList;
    AcceptLanguageList * m_AcceptLanguageList;
    AllowList * m_AllowList;
    ContentEncodingList * m_ContentEncodingList;
    ContactList * m_ContactList;
    InReplyToList * m_InReplyToList;
    ProxyRequireList * m_ProxyRequireList;
    RequireList * m_RequireList;
    SupportedList * m_SupportedList;
    UnsupportedList * m_UnsupportedList;

    /// Route Headers
    RouteList * m_RouteList;
    RecordRouteList * m_RecordRouteList;
    PathList * m_PathList;

    /// special headers
    Via::Collection * m_ViaList;
    CSeq * m_CSeq;

    /// Custom Headers
    PLIST( CustomHeaders, SIPHeader );
    CustomHeaders * m_CustomHeaders;

    /// Custom Headers
    PLIST( InternalHeaders, SIPHeader );
    InternalHeaders * m_InternalHeaders;

    /// Extension Headers
    ReferTo * m_ReferTo;
    Replaces * m_Replaces;
    Event * m_Event;
    SIPETag * m_SIPETag;
    SIPIfMatch * m_SIPIfMatch;
    SubscriptionState * m_SubscriptionState;
    AllowEventsList * m_AllowEventsList;

    mutable PMutex m_InternalMutex;
    static PMutex m_GlobalMutex;
    static PAtomicInteger m_InternalIdBase;

  public:
#if LOG_INSTANCE_COUNTER
    static PAtomicInteger m_InstanceCounter;
#endif
  };


  
};


#endif



