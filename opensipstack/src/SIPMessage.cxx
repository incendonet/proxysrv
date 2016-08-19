/*
*
* SIPMessage.cxx
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
* $Log: SIPMessage.cxx,v $
* Revision 1.60  2006/11/30 08:19:38  joegenbaclor
* Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
*
* Revision 1.59  2006/11/23 09:30:48  joegenbaclor
* Fixed bug in Route header parsing where the order is reversed by the SIPParser
*
* Revision 1.58  2006/11/22 03:16:18  joegenbaclor
* Removed Record Routes from B2B Outbound invites
*
* Revision 1.57  2006/11/21 06:55:19  joegenbaclor
* Added ability to add/append additional listeners.
*
* Revision 1.56  2006/11/18 03:14:28  joegenbaclor
* Implemented session loop detection mechanism using iid via parameter
*
* Revision 1.55  2006/11/17 04:22:22  joegenbaclor
* Added ability to remember core to use via m_UACoreName
*
* Revision 1.54  2006/11/10 08:10:51  rcolobong
* Use ParserTools GetDefaultHashKey1 and GetDefaultHashKey2 in XORHash rather than using the MACRO GS_HASH_KEY_1/GS_HASH_KEY_2
*
* Revision 1.53  2006/11/08 06:46:20  joegenbaclor
* Added session level NIC/Interface binding
*
* Revision 1.52  2006/11/07 09:30:46  joegenbaclor
* Added new internal header "IFACE-ADDR" to allow transctions to remember the interface address for sending future responses within the transaction lifespan
*
* Revision 1.51  2006/11/02 22:53:46  joegenbaclor
* Added Path header support (RFC 3327)
*
* Revision 1.50  2006/10/30 02:42:27  joegenbaclor
* 1.  Various changes to support RFC 3680.
* 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
*
* Revision 1.49  2006/10/25 10:51:30  rcolobong
* Fix bug where m_SupportedList might be Null in GetSupported()
*
* Revision 1.48  2006/10/23 15:44:58  joegenbaclor
* 1.  Numerous bug fixes for REFER
* 2.  Added new SIP Headers for SIP timers and Attended call transfer
* 3.  Added loop sanity check to SIP UDP Transport
* 4.  Corrected parser bug for ProxyRequire header
*
* Revision 1.47  2006/10/17 16:20:26  joegenbaclor
* Fixed bug in parsing ProxyRequire header
*
* Revision 1.46  2006/10/17 00:12:24  joegenbaclor
* Added support for RFC 4028 SIP Session Timers header
*
* Revision 1.45  2006/09/02 04:44:05  joegenbaclor
* Modified FixViaNATParametes to not Pop the internal headers for RCVADDR and RCV Port
*
* Revision 1.44  2006/08/18 07:12:24  joegenbaclor
* 1.  added Function overload for GetContentType
* 2.  Changed unix make files to not include opensbc in make process
*
* Revision 1.43  2006/08/15 15:51:14  joegenbaclor
* Added sanity check to Contact header where some SIP Stacks may send blank contact in response after
* receiving a request without a contact presnt
*
* Revision 1.42  2006/08/14 01:29:08  joegenbaclor
* Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
*
* Revision 1.41  2006/08/11 08:38:42  joegenbaclor
* Initial upload of Presence package
*
* Revision 1.40  2006/08/09 12:17:48  joegenbaclor
* Added RFC 3428 Support
*
* Revision 1.39  2006/08/06 02:32:08  joegenbaclor
* Corrected bug regarding blank call-ids for UnknownTransaction events
*
* Revision 1.38  2006/07/28 04:14:21  joegenbaclor
* Added function to get the size of Accept header if available
*
* Revision 1.37  2006/07/19 02:19:21  joegenbaclor
* 1.  Bug fixes concerning B2BUA destruction during during 3xx, 4xx, 5xx, 6xx local response
* 2.  Fixed bug where Route header is never removed from outbound Invite resulting that may result to a loop when
*      Inbound Invite has a Route header present
* 3.  Added RemoveAllRoutes() in SIPMessage
*
* Revision 1.36  2006/07/13 06:53:05  joegenbaclor
* 1.  Introduced Sanity checks for incoming SIP Message
* 2.  Corrected bug in SIPURI where enum scheme is not getting recognized.
* 3.  Added ENUM support to routing
* 4.  Introduced global routing flag to indicate when and not to rewrite outbound To URI's
*
* Revision 1.35  2006/07/06 05:38:11  joegenbaclor
* Final last minute changes to CBCOM hash support
*
* Revision 1.34  2006/06/30 07:12:21  joegenbaclor
* *** empty log message ***
*
* Revision 1.33  2006/06/28 14:15:00  joegenbaclor
* Bulk commit for Media Proxy functionalities for OpenB2BUA
* Marks a minor version upgrade from 1.0.2 to 1.0.3
*
* Revision 1.32  2006/06/20 09:52:55  joegenbaclor
* Added Routing and SDP Handlers to B2BUA
*
* Revision 1.31  2006/06/09 07:37:30  joegenbaclor
* 1.  Added Publish, SIPETag & SIPIfMatch  support in SIPMessage
* 2.  Started work on StatusPage Resource for OSSAppConfig
*
* Revision 1.30  2006/06/07 09:18:33  joegenbaclor
* Added the ability to SIPMessage to read from a PChannel
*
* Revision 1.29  2006/05/19 06:30:37  joegenbaclor
* 1.  Fixed bug in SIPHeaderB where tag may not be parsed properly if no < > enclosure is present
* 2.  Various enhancements to OpenSBC and proxy session
*
* Revision 1.28  2006/05/17 03:43:07  joegenbaclor
* Bulk commit after sourceforge cvs migration changes
*
* Revision 1.27  2006/04/26 00:51:09  joegenbaclor
* 1.  Added IsMethod() and IsStatusCode() functions to get rid of repetitive code for determining Message class types
* 2.  Added IsRefer()
*
* Revision 1.26  2006/04/21 09:55:10  joegenbaclor
* Added RFC 3265 header requirements
*
* Revision 1.25  2006/04/21 04:12:02  joegenbaclor
* Added Refer-To header support
*
* Revision 1.24  2006/04/07 07:47:20  joegenbaclor
* More work on OpenPhone/OPAL
*
* Revision 1.23  2006/04/05 00:26:04  joegenbaclor
* 1.  Completed support for STUN NAT traversal method and OPAL style address translation
* 2.  More bug fixes to ProxySession
* 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
* 4.  Started work on OPAL outbound
*
* Revision 1.22  2006/03/27 01:25:33  joegenbaclor
* Some improvements on NICT and NIST state machines
*
* Revision 1.21  2006/03/25 18:40:21  joegenbaclor
* 1.  Worked on some race conditions on Finite State Machines during heavy load.
* 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
* 3,  Some minor improvements to simple-proxy application
*
* Revision 1.20  2006/03/24 15:42:18  joegenbaclor
* Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
*
* Revision 1.19  2006/03/23 01:36:45  joegenbaclor
* Added local routing through simple registrar
*
* Revision 1.18  2006/03/21 14:11:51  joegenbaclor
* 1.  Added rport support for NAT Traversal
* 2.  Added NAT Keep alive mechanism in SIPSession
* 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
*
* Revision 1.17  2006/03/17 13:51:00  joegenbaclor
* 1.  Implemented UAC Call Redirection
* 2.  Implemented SendCancel() method in CallSession
* 3.  Introduced ClearInternalHeaders() function in SIP Message to erase internal headers inheritted from previous transaction.
*
* Revision 1.16  2006/03/15 16:40:46  joegenbaclor
* 1.  More Dialog Support.
* 2.  Improved SIPUserAgent::AppendCommonHeaders().
* 3.  Fixed SDP parser bugs
* 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
* 4.  Worked on OpalOSSConnection RTP
*
* Revision 1.15  2006/03/14 05:53:58  joegenbaclor
* Modified SetBody() so that if the string is empty m_ContentType is also blanked out
*
* Revision 1.14  2006/03/14 03:53:53  joegenbaclor
* Initial source upload for BETA Release Candidate
*
*
*
*/

#include "SIPMessage.h"
#include "SIPTransport.h"
#include "SIPUserAgent.h"

#define new PNEW

using namespace SIPParser;
using namespace SIPTransports;

PAtomicInteger SIPMessage::m_InternalIdBase;
#if LOG_INSTANCE_COUNTER
PAtomicInteger SIPMessage::m_InstanceCounter = 0;
#endif

PMutex SIPMessage::m_GlobalMutex;
#define GlobalLock() PWaitAndSignal global_lock( SIPMessage::m_GlobalMutex )

SIPMessage::SIPMessage()
{
#if LOG_INSTANCE_COUNTER
	++SIPMessage::m_InstanceCounter;
#endif

	m_IsFinal = FALSE;
	m_IsUnique = TRUE;

	if( SIPMessage::m_InternalIdBase == INT_MAX)
		SIPMessage::m_InternalIdBase = 0;

	m_InternalId = ++SIPMessage::m_InternalIdBase;

	m_RefCounter = NULL;
	ZeroObjects();

}

SIPMessage::SIPMessage(
					   const SIPMessage & msg
					   )
{
#if LOG_INSTANCE_COUNTER
	++SIPMessage::m_InstanceCounter;
#endif

	GlobalLock();

	m_IsFinal = FALSE;
	m_IsUnique = FALSE;

	if( SIPMessage::m_InternalIdBase == INT_MAX )
		SIPMessage::m_InternalIdBase = 0;

	m_InternalId = ++SIPMessage::m_InternalIdBase;

	m_RefCounter = NULL;

	ZeroObjects();

	operator=( msg );
}

SIPMessage::SIPMessage(
					   const PString & msg
					   )
{
#if LOG_INSTANCE_COUNTER
	++SIPMessage::m_InstanceCounter;
#endif

	GlobalLock();

	m_IsFinal = FALSE;
	m_IsUnique = TRUE;

	if( SIPMessage::m_InternalIdBase == INT_MAX )
		SIPMessage::m_InternalIdBase = 0;

	m_InternalId = ++SIPMessage::m_InternalIdBase;

	m_RefCounter = NULL;
	ZeroObjects();
	operator=(msg);
}

SIPMessage::SIPMessage(
					   const PBYTEArray & msg
					   )
{
#if LOG_INSTANCE_COUNTER
	++SIPMessage::m_InstanceCounter;
#endif

	GlobalLock();

	m_IsFinal = FALSE;
	m_IsUnique = TRUE;

	if( SIPMessage::m_InternalIdBase == INT_MAX )
		SIPMessage::m_InternalIdBase = 0;

	m_InternalId = ++SIPMessage::m_InternalIdBase;

	m_RefCounter = NULL;
	ZeroObjects();
	operator=(msg);
}

SIPMessage::~SIPMessage()
{
#if LOG_INSTANCE_COUNTER
	--SIPMessage::m_InstanceCounter;
#endif

	GlobalLock();
	ReleaseRef();
}

BOOL SIPMessage::Read(
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
	MimeHeaderList headers;

	if (!inputStream.bad() && !inputStream.eof()) 
	{
		inputStream.SetReadTimeout(3000);
		inputStream >> startLine >> headers;
	}

	if (inputStream.bad()) 
	{
		PTRACE_IF(1, inputStream.GetErrorCode(PChannel::LastReadError) != PChannel::NoError,
			"SIP\tPDU Read failed: " << inputStream.GetErrorText(PChannel::LastReadError));
		return FALSE;
	}

	if(startLine.IsEmpty()) 
	{
		return FALSE;
	}


	PString body;
	// get the SDP content
	PINDEX contentLength = headers.GetInteger( "Content-Length", 0 );

	if (contentLength > 0)
	{
		inputStream.read(body.GetPointer(contentLength+1), contentLength);
		body[contentLength] = '\0';
	}

	PStringStream strm;
	strm << startLine << "\r\n";
	strm << headers << "\r\n";

	if( !body.IsEmpty() )
		strm << body << "\r\n\r\n"; 

	Parse( strm );

	return TRUE;
}

BOOL SIPMessage::Write(
					   PChannel & outputStream
					   )
{
	Finalize();
	PString str = AsString();
	return outputStream.WriteString( str );
}

void SIPMessage::Parse(
					   const PString & msg
					   )
{
	GlobalLock();

	PStringArray tokens;
	ParserTools::MsgTokenize( msg, tokens );

	if( tokens.GetSize() == 0 )
		return;

	PINDEX i;
	PINDEX bodyOffSet = 0;

	if( m_RawMessage != NULL )
		delete m_RawMessage;

	m_RawMessage = new PStringArray();

	if( m_StartLine != NULL )
		delete m_StartLine;

	m_StartLine = new StartLine( tokens[0] );

	tokens.RemoveAt(0);

	for( i = 0; i < tokens.GetSize(); i++ )
	{
		if( !tokens[i].IsEmpty() )
			m_RawMessage->AppendString( tokens[i] );
		else
			break;

		bodyOffSet = i;
	}

	if( m_RawMessageBody != NULL )
		delete m_RawMessageBody;

	m_RawMessageBody = new PString();

	BOOL hasBody = FALSE;

	for( i = ++bodyOffSet; i < tokens.GetSize(); i++ )
	{
		if( tokens[i].IsEmpty() )
			++bodyOffSet;
		else
			break;
	}

	for( i = bodyOffSet; i < tokens.GetSize(); i++ )
	{
		if( !tokens[i].IsEmpty() )
		{
			*m_RawMessageBody += tokens[i] + "\r\n";
			hasBody = TRUE;
		}else
			break;
	}

	if( !hasBody )
	{
		delete m_RawMessageBody;
		m_RawMessageBody = NULL;
	}

	return;
}

void SIPMessage::Parse(
					   const PBYTEArray & _msg
					   )
{
	PBYTEArray msg = _msg;
	msg.SetSize( msg.GetSize() + 1 );
	msg[msg.GetSize()-1] = '\0';
	PString msg2( (char*)msg.GetPointer() );
	msg2.MakeUnique();
	msg2.MakeMinimumSize();
	Parse( msg2 );
}

/*
BOOL SIPMessage::EnterCriticalSection()const
{

m_CriticalSection.Wait();

return TRUE;
}

void SIPMessage::LeaveCriticalSection()const
{
m_CriticalSection.Signal();
}
*/

SIPMessage & SIPMessage::operator=(
								   const SIPMessage & msg
								   )
{
	GlobalLock();

	/// object copying a ref to itself;
	if( msg.m_InternalId == m_InternalId )
		return *this;

	m_IsUnique = FALSE;


	/// we make sure that the referencing object is unique
	/// and has relinquished any previous references
	MakeUnique();

	/// we can now cleanup the object so we have a clean slate
	Cleanup();

	(const_cast< SIPMessage & >( msg )).AddRef( *this );

	return *this;
}


SIPMessage & SIPMessage::operator=(
								   const PString & msg
								   )
{
	GlobalLock();

	MakeUnique();
	Parse( msg );
	return *this;
}

SIPMessage & SIPMessage::operator=(
								   const PBYTEArray & msg
								   )
{
	GlobalLock();

	MakeUnique();
	Parse( msg );
	return *this;
}

/////////////////////////////////////////////////////////////


BOOL SIPMessage::GetStartLine(
							  StartLine & startLine
							  )const
{


	if( m_StartLine == NULL )
		return FALSE;

	startLine = *m_StartLine;
	return TRUE;
}

PString SIPMessage::GetMethod()const
{
	GlobalLock();
	if( !IsRequest() )
		return FALSE;

	StartLine startLine;
	GetStartLine( startLine );
	return startLine.GetRequestLine()->GetMethod();
}

PString SIPMessage::GetStartLine()const
{
	if( m_StartLine == NULL )
		return PString::Empty();

	return m_StartLine->AsString();
}

void SIPMessage::SetStartLine(
							  const StartLine & startLine
							  )
{


	MakeUnique();

	if( m_StartLine != NULL )
		delete m_StartLine;

	m_StartLine = (StartLine*)startLine.Clone();
}

void SIPMessage::SetStartLine(
							  const StatusLine & statusLine
							  )
{
	StartLine startLine( statusLine );
	SetStartLine( startLine );
}

void SIPMessage::SetStartLine(
							  const RequestLine & requestLine
							  )
{
	StartLine startLine( requestLine );
	SetStartLine( startLine );
}

WORD SIPMessage::GetStatusCode()const
{
	if( IsRequest() )
		return 0;

	StartLine startLine;
	GetStartLine( startLine );

	return startLine.GetStatusLine()->GetStatusCode();
}


BOOL SIPMessage::GetStatusLine(
							   StatusLine & statusLine
							   )const
{
	if( IsRequest() )
		return FALSE;

	StartLine startLine;
	if( !GetStartLine( startLine ) )
		return FALSE;

	StatusLine * sline = startLine.GetStatusLine();
	//PAssertNULL( sline );

	statusLine = *sline;
	return TRUE;
}

BOOL SIPMessage::GetRequestLine(
								RequestLine & requestLine
								)const
{
	if( !IsRequest() )
		return FALSE;

	StartLine startLine;
	if( !GetStartLine( startLine ) )
		return FALSE;

	RequestLine * rline = startLine.GetRequestLine();
	//PAssertNULL( rline );

	requestLine = *rline;
	return TRUE;
}

////////////////////////////////////////////////////////////

void SIPMessage::AddRef(
						SIPMessage & msg
						)const
{



	m_IsUnique = FALSE;

	(const_cast< SIPMessage * >( this ))->Finalize();

	msg.m_InternalId = m_InternalId;

	if( m_RefCounter == NULL )
		m_RefCounter = new PAtomicInteger();

	++(*m_RefCounter);

	msg.m_RefCounter = m_RefCounter;

	AssignContents( msg );

	msg.m_IsUnique = m_IsUnique = FALSE;


};

void SIPMessage::ReleaseRef()
{

	if( m_RefCounter == NULL )
	{
		//cout << m_InternalId << " CLEAN UP" << endl;
		Cleanup();
	}else if( *m_RefCounter == 0 )
	{
		//cout << m_InternalId << " CLEAN UP" << endl;
		Cleanup();
	}
	else
	{
		//cout << m_InternalId << " NO CLEAN UP" << endl;
		if( --(*m_RefCounter) == 0 )
			m_IsUnique = TRUE;
	}

}

void SIPMessage::PrintOn( ostream & strm )const
{
	GlobalLock();

	///PAssert( m_IsFinal, "SIPMessage needs to call Finalize() before this operation" );
	(const_cast< SIPMessage * >( this ))->Finalize();

	if( m_StartLine != NULL )
		strm << *m_StartLine ;

	strm << "\r\n";

	if( m_From != NULL )
		strm << *m_From << "\r\n";

	if( m_To != NULL )
		strm << *m_To << "\r\n";

	if( m_ReferTo != NULL )
		strm << *m_ReferTo << "\r\n";

	/// special headers
	if( m_ViaList != NULL && m_ViaList->GetSize() != 0 )
	{
		for( PINDEX i = 0; i < m_ViaList->GetSize(); i++ )
			strm << (*m_ViaList)[i] << "\r\n";
	}

	if( m_CSeq != NULL )
		strm << *m_CSeq << "\r\n";

	if( m_CallId != NULL )
		strm << *m_CallId << "\r\n";

	if( m_ContactList != NULL && m_ContactList->GetSize() != 0 )
	{
		/// some broken stacks send blank Contact body if its now present in the request
		PStringStream c;
		c << *m_ContactList;  
		if( !c.Trim().IsEmpty() )
			strm << *m_ContactList << "\r\n";
	}

	/// Route Headers
	if( m_RouteList != NULL && m_RouteList->GetSize() != 0 )
		strm << *m_RouteList << "\r\n";

	if( m_RecordRouteList != NULL && m_RecordRouteList->GetSize() != 0 )
		strm << *m_RecordRouteList << "\r\n";

	if( m_PathList != NULL && m_PathList->GetSize() != 0 )
		strm << *m_RecordRouteList << "\r\n";

	if( m_AlertInfo != NULL )
		strm << *m_AlertInfo << "\r\n";

	if( m_CallInfo != NULL )
		strm << *m_CallInfo << "\r\n";

	if( m_ContentDisposition != NULL )
		strm << *m_ContentDisposition << "\r\n";

	if( m_ContentLanguage != NULL )
		strm << *m_ContentLanguage << "\r\n";

	if( m_Date != NULL )
		strm << *m_Date << "\r\n";

	if( m_ErrorInfo != NULL )
		strm << *m_ErrorInfo << "\r\n";

	if( m_MIMEVersion != NULL )
		strm << *m_MIMEVersion << "\r\n";

	if( m_Event != NULL )
		strm << *m_Event << "\r\n";

	if( m_SIPETag != NULL )
		strm << *m_SIPETag << "\r\n";

	if( m_SIPIfMatch != NULL )
		strm << *m_SIPIfMatch << "\r\n";

	if( m_SubscriptionState != NULL )
		strm << *m_SubscriptionState << "\r\n";

	if( m_Organization != NULL )
		strm << *m_Organization << "\r\n";

	if( m_Priority != NULL )
		strm << *m_Priority << "\r\n";

	if( m_RetryAfter != NULL )
		strm << *m_RetryAfter << "\r\n";

	if( m_Server != NULL )
		strm << *m_Server << "\r\n";

	if( m_Subject != NULL )
		strm << *m_Subject << "\r\n";

	if( m_UserAgent != NULL )
		strm << *m_UserAgent << "\r\n";

	if( m_Warning != NULL )
		strm << *m_Warning << "\r\n";

	/// URI headers
	if( m_ReplyTo != NULL )
		strm << *m_ReplyTo << "\r\n";

	/// Integer headers
	if( m_Expires != NULL )
		strm << *m_Expires << "\r\n";

	if( m_MaxForwards != NULL )
		strm << *m_MaxForwards << "\r\n";

	if( m_MinExpires != NULL )
		strm << *m_MinExpires << "\r\n";


	if( m_TimeStamp != NULL )
		strm << *m_TimeStamp << "\r\n";

	/// Comma delimited named parameters
	if( m_AuthenticationInfo != NULL )
		strm << *m_AuthenticationInfo << "\r\n";

	if( m_Authorization != NULL )
		strm << *m_Authorization << "\r\n";

	if( m_ProxyAuthenticate != NULL )
		strm << *m_ProxyAuthenticate << "\r\n";

	if( m_ProxyAuthorization != NULL )
		strm << *m_ProxyAuthorization << "\r\n";

	if( m_WWWAuthenticate != NULL )
		strm << *m_WWWAuthenticate << "\r\n";

	/// Comma delimited headers

	if( m_AcceptList != NULL && m_AcceptList->GetSize() != 0 )
		strm << *m_AcceptList << "\r\n";

	if( m_AllowEventsList != NULL && m_AllowEventsList->GetSize() != 0 )
		strm << *m_AllowEventsList << "\r\n";

	if( m_AcceptEncodingList != NULL && m_AcceptEncodingList->GetSize() != 0 )
		strm << *m_AcceptEncodingList << "\r\n";

	if( m_AcceptLanguageList != NULL && m_AcceptLanguageList->GetSize() != 0 )
		strm << *m_AcceptLanguageList << "\r\n";

	if( m_AllowList != NULL && m_AllowList->GetSize() != 0 )
		strm << *m_AllowList << "\r\n";

	if( m_ContentEncodingList != NULL && m_ContentEncodingList->GetSize() != 0 )
		strm << *m_ContentEncodingList << "\r\n";

	if( m_InReplyToList!= NULL && m_InReplyToList->GetSize() != 0 )
		strm << *m_InReplyToList << "\r\n";

	if( m_ProxyRequireList != NULL && m_ProxyRequireList->GetSize() != 0 )
		strm << *m_ProxyRequireList << "\r\n";

	if( m_RequireList != NULL && m_RequireList->GetSize() != 0 )
		strm << *m_RequireList << "\r\n";

	if( m_SupportedList != NULL && m_SupportedList->GetSize() != 0 )
		strm << *m_SupportedList << "\r\n";

	if( m_UnsupportedList != NULL && m_UnsupportedList->GetSize() != 0  )
		strm << *m_UnsupportedList << "\r\n";

	/// session timer headers
	if( m_SessionExpires != NULL )
		strm << *m_SessionExpires << "\r\n";

	if( m_MinSE != NULL )
		strm << *m_MinSE << "\r\n";
	/// Custom Headers

	if( m_CustomHeaders != NULL )
	{
		for( PINDEX i = 0; i < m_CustomHeaders->GetSize(); i++ )
			strm << (*m_CustomHeaders)[i] << "\r\n";
	};

	if( m_ContentType != NULL )
		strm << *m_ContentType << "\r\n";

	if( m_ContentLength != NULL )
	{
		strm << *m_ContentLength << "\r\n";
	}

	strm << "\r\n";


	if( m_RawMessageBody != NULL )
		strm << *m_RawMessageBody << "\r\n\r\n";


}

PObject * SIPMessage::Clone()const
{
	return new SIPMessage( *this );
}

PString SIPMessage::AsString()const
{
	PStringStream strm;
	PrintOn( strm );
	return strm;
}

void SIPMessage::Cleanup()
{
	GlobalLock();



	if( m_RawMessage != NULL )
		delete m_RawMessage;

	if( m_RawMessageBody != NULL )
		delete m_RawMessageBody;

	if( m_StartLine != NULL )
		delete m_StartLine;

	/// Standad SIP Headers
	if( m_AlertInfo != NULL )
		delete m_AlertInfo;

	if( m_CallId != NULL )
		delete m_CallId;

	if( m_CallInfo != NULL )
		delete m_CallInfo;

	if( m_ContentDisposition != NULL )
		delete m_ContentDisposition;

	if( m_ContentLanguage != NULL )
		delete m_ContentLanguage;

	if( m_ContentLength != NULL )
		delete m_ContentLength;

	if( m_ContentType != NULL )
		delete m_ContentType;

	if( m_Date != NULL )
		delete m_Date;

	if( m_ErrorInfo != NULL )
		delete m_ErrorInfo;

	if( m_MIMEVersion != NULL )
		delete m_MIMEVersion;

	if( m_Organization != NULL )
		delete m_Organization;

	if( m_Event != NULL )
		delete m_Event;

	if( m_SIPETag != NULL )
		delete m_SIPETag;

	if( m_SIPIfMatch != NULL )
		delete m_SIPIfMatch;

	if( m_SubscriptionState != NULL )
		delete m_SubscriptionState;

	if( m_Priority != NULL )
		delete m_Priority;

	if( m_RetryAfter != NULL )
		delete m_RetryAfter;

	if( m_Server != NULL )
		delete m_Server;

	if( m_Subject != NULL )
		delete m_Subject;

	if( m_UserAgent != NULL )
		delete m_UserAgent;

	if( m_Warning != NULL )
		delete m_Warning;

	/// URI headers
	if( m_From != NULL )
		delete m_From;

	if( m_To != NULL )
		delete m_To;

	if( m_ReferTo != NULL )
		delete m_ReferTo;

	if( m_Replaces != NULL )
		delete m_Replaces;

	if( m_ReplyTo != NULL )
		delete m_ReplyTo;

	/// Integer headers
	if( m_Expires != NULL )
		delete m_Expires;

	if( m_MaxForwards != NULL )
		delete m_MaxForwards;

	if( m_MinExpires != NULL )
		delete m_MinExpires;

	if( m_TimeStamp != NULL )
		delete m_TimeStamp;

	/// Comma delimited named parameters
	if( m_AuthenticationInfo != NULL )
		delete m_AuthenticationInfo;

	if( m_Authorization != NULL )
		delete m_Authorization;

	if( m_ProxyAuthenticate != NULL )
		delete m_ProxyAuthenticate;

	if( m_ProxyAuthorization != NULL )
		delete m_ProxyAuthorization;

	if( m_WWWAuthenticate != NULL )
		delete m_WWWAuthenticate;

	/// Comma delimited headers

	if( m_AcceptList != NULL )
		delete m_AcceptList;

	if( m_AllowEventsList != NULL )
		delete m_AllowEventsList;

	if( m_AcceptEncodingList != NULL )
		delete m_AcceptEncodingList;

	if( m_AcceptLanguageList != NULL )
		delete m_AcceptLanguageList;

	if( m_AllowList != NULL )
		delete m_AllowList;

	if( m_ContentEncodingList != NULL )
		delete m_ContentEncodingList;

	if( m_ContactList != NULL )
		delete m_ContactList;

	if( m_InReplyToList != NULL )
		delete m_InReplyToList;

	if( m_ProxyRequireList != NULL )
		delete m_ProxyRequireList;

	if( m_RequireList != NULL )
		delete m_RequireList;

	if( m_SupportedList != NULL )
		delete m_SupportedList;

	if( m_UnsupportedList != NULL )
		delete m_UnsupportedList;

	/// Route Headers
	if( m_RouteList != NULL )
		delete m_RouteList;

	if( m_RecordRouteList != NULL )
		delete m_RecordRouteList;

	if( m_PathList != NULL )
		delete m_PathList;

	/// special headers
	if( m_ViaList != NULL )
		delete m_ViaList;

	if( m_CSeq != NULL )
		delete m_CSeq;

	/// Custom Headers
	if( m_CustomHeaders != NULL )
		delete m_CustomHeaders;

	if( m_InternalHeaders != NULL )
		delete m_InternalHeaders;

	if( m_RefCounter != NULL )
		delete m_RefCounter;

	ZeroObjects();


}

void SIPMessage::MakeUnique()
{
	GlobalLock();

	if( m_IsUnique )
		return;

	///ReleaseRef();



	if( m_RefCounter == NULL )
	{
		m_IsUnique = TRUE;
		return;
	}else if( *m_RefCounter == 0 )
	{
		m_IsUnique = TRUE;
		return;
	}
	else
	{
		//cout << m_InternalId << " NO CLEAN UP" << endl;
		--(*m_RefCounter);
	}

	m_IsUnique = TRUE;

	if( SIPMessage::m_InternalIdBase == INT_MAX )
		SIPMessage::m_InternalIdBase = 0;

	m_InternalId = ++SIPMessage::m_InternalIdBase;

	if( m_RawMessage != NULL )
		m_RawMessage = new PStringArray( *m_RawMessage );

	if( m_RawMessageBody != NULL )
		m_RawMessageBody = new PString( *m_RawMessageBody );

	if( m_StartLine != NULL )
		m_StartLine = (StartLine*)m_StartLine->Clone();

	/// Standad SIP Headers
	if( m_AlertInfo != NULL )
		m_AlertInfo = (AlertInfo*)m_AlertInfo->Clone();

	if( m_CallId != NULL )
		m_CallId = (CallId*)m_CallId->Clone();

	if( m_CallInfo != NULL )
		m_CallInfo = (CallInfo*)m_CallInfo->Clone();

	if( m_ContentDisposition != NULL )
		m_ContentDisposition = (ContentDisposition*)m_ContentDisposition->Clone();

	if( m_ContentLanguage != NULL )
		m_ContentLanguage = (ContentLanguage*)m_ContentLanguage->Clone();

	if( m_ContentLength != NULL )
		m_ContentLength = (ContentLength*)m_ContentLength->Clone();

	if( m_ContentType != NULL )
		m_ContentType = (ContentType*)m_ContentType->Clone();

	if( m_Date != NULL )
		m_Date = (Date*)m_Date->Clone();

	if( m_ErrorInfo != NULL )
		m_ErrorInfo = (ErrorInfo*)m_ErrorInfo->Clone();

	if( m_MIMEVersion != NULL )
		m_MIMEVersion = (MIMEVersion*)m_MIMEVersion->Clone();

	if( m_Organization != NULL )
		m_Organization = (Organization*)m_Organization->Clone();

	if( m_Event != NULL )
		m_Event = (Event*)m_Event->Clone();

	if( m_SIPETag != NULL )
		m_SIPETag = (SIPETag*)m_SIPETag->Clone();

	if( m_SIPIfMatch != NULL )
		m_SIPIfMatch = (SIPIfMatch*)m_SIPIfMatch->Clone();

	if( m_SubscriptionState != NULL )
		m_SubscriptionState = (SubscriptionState*)m_SubscriptionState->Clone();

	if( m_Priority != NULL )
		m_Priority = (Priority*)m_Priority->Clone();

	if( m_RetryAfter != NULL )
		m_RetryAfter = (RetryAfter*)m_RetryAfter->Clone();

	if( m_Server != NULL )
		m_Server = (Server*)m_Server->Clone();

	if( m_Subject != NULL )
		m_Subject = (Subject*)m_Subject->Clone();

	if( m_UserAgent != NULL )
		m_UserAgent = (UserAgent*)m_UserAgent->Clone();

	if( m_Warning != NULL )
		m_Warning = (Warning*)m_Warning->Clone();

	/// URI headers
	if( m_From != NULL )
		m_From = (From*)m_From->Clone();

	if( m_To != NULL )
		m_To = (To*)m_To->Clone();

	if( m_ReferTo != NULL )
		m_ReferTo = (ReferTo*)m_ReferTo->Clone();

	if( m_Replaces != NULL )
		m_Replaces = (Replaces*)m_Replaces->Clone();

	if( m_ReplyTo != NULL )
		m_ReplyTo = (ReplyTo*)m_ReplyTo->Clone();

	/// Integer headers
	if( m_Expires != NULL )
		m_Expires = (Expires*)m_Expires->Clone();

	if( m_MaxForwards != NULL )
		m_MaxForwards = (MaxForwards*)m_MaxForwards->Clone();

	if( m_MinExpires != NULL )
		m_MinExpires = (MinExpires*)m_MinExpires->Clone();

	if( m_TimeStamp != NULL )
		m_TimeStamp = (TimeStamp*)m_TimeStamp->Clone();

	/// Comma delimited named parameters
	if( m_AuthenticationInfo != NULL )
		m_AuthenticationInfo = (AuthenticationInfo*)m_AuthenticationInfo->Clone();

	if( m_Authorization != NULL )
		m_Authorization = (Authorization*)m_Authorization->Clone();

	if( m_ProxyAuthenticate != NULL )
		m_ProxyAuthenticate = (ProxyAuthenticate*)m_ProxyAuthenticate->Clone();

	if( m_ProxyAuthorization != NULL )
		m_ProxyAuthorization = (ProxyAuthorization*)m_ProxyAuthorization->Clone();

	if( m_WWWAuthenticate != NULL )
		m_WWWAuthenticate = (WWWAuthenticate*)m_WWWAuthenticate->Clone();

	/// Comma delimited headers

	if( m_AcceptList != NULL )
		m_AcceptList = (AcceptList*)m_AcceptList->Clone();

	if( m_AllowEventsList != NULL )
		m_AllowEventsList = (AllowEventsList*)m_AllowEventsList->Clone();

	if( m_AcceptEncodingList != NULL )
		m_AcceptEncodingList = (AcceptEncodingList*)m_AcceptEncodingList->Clone();

	if( m_AcceptLanguageList != NULL )
		m_AcceptLanguageList = (AcceptLanguageList*)m_AcceptLanguageList->Clone();

	if( m_AllowList != NULL )
		m_AllowList = (AllowList*)m_AllowList->Clone();

	if( m_ContentEncodingList != NULL )
		m_ContentEncodingList = (ContentEncodingList*)m_ContentEncodingList->Clone();

	if( m_ContactList != NULL )
		m_ContactList = (ContactList*)m_ContactList->Clone();

	if( m_InReplyToList != NULL )
		m_InReplyToList = (InReplyToList*)m_InReplyToList->Clone();

	if( m_ProxyRequireList != NULL )
		m_ProxyRequireList = (ProxyRequireList*)m_ProxyRequireList->Clone();

	if( m_RequireList != NULL )
		m_RequireList = (RequireList*)m_RequireList->Clone();

	if( m_SupportedList != NULL )
		m_SupportedList = (SupportedList*)m_SupportedList->Clone();

	if( m_UnsupportedList != NULL )
		m_UnsupportedList = (UnsupportedList*)m_UnsupportedList->Clone();

	/// Route Headers
	if( m_RouteList != NULL )
		m_RouteList = (RouteList*)m_RouteList->Clone();

	if( m_RecordRouteList != NULL )
		m_RecordRouteList = (RecordRouteList*)m_RecordRouteList->Clone();

	if( m_PathList != NULL )
		m_PathList = (PathList*)m_PathList->Clone();


	/// special headers
	if( m_ViaList != NULL )
	{
		m_ViaList = new Via::Collection( *m_ViaList );
		m_ViaList->MakeUnique();
	}

	if( m_CSeq != NULL )
		m_CSeq = (CSeq*)m_CSeq->Clone();

	/// Custom Headers
	if( m_CustomHeaders != NULL )
		m_CustomHeaders = new CustomHeaders( *m_CustomHeaders );

	if( m_InternalHeaders != NULL )
		m_InternalHeaders = new InternalHeaders( *m_InternalHeaders );

	m_RefCounter = NULL;


}

void SIPMessage::ZeroObjects()
{
	GlobalLock();

	m_RawMessage = NULL;
	m_RawMessageBody = NULL;
	m_StartLine = NULL;

	/// Standad SIP Headers
	m_AlertInfo = NULL;
	m_CallId = NULL;
	m_CallInfo = NULL;
	m_ContentDisposition = NULL;
	m_ContentLanguage = NULL;
	m_ContentLength = NULL;
	m_ContentType = NULL;
	m_Date = NULL;
	m_ErrorInfo = NULL;
	m_MIMEVersion = NULL;
	m_Organization = NULL;
	m_Event = NULL;
	m_SIPETag = NULL;
	m_SIPIfMatch = NULL;
	m_SubscriptionState = NULL;
	m_Priority = NULL;
	m_RetryAfter = NULL;
	m_Server = NULL;
	m_Subject = NULL;
	m_UserAgent = NULL;
	m_Warning = NULL;

	/// URI headers
	m_From = NULL;
	m_To = NULL;
	m_ReferTo = NULL;
	m_Replaces = NULL;
	m_ReplyTo = NULL;

	/// Integer headers
	m_Expires = NULL;
	m_SessionExpires = NULL;
	m_MinSE = NULL;
	m_MaxForwards = NULL;
	m_MinExpires = NULL;
	m_TimeStamp = NULL;

	/// Comma delimited named parameters
	m_AuthenticationInfo = NULL;
	m_Authorization = NULL;
	m_ProxyAuthenticate = NULL;
	m_ProxyAuthorization = NULL;
	m_WWWAuthenticate = NULL;

	/// Comma delimited headers

	m_AcceptList = NULL;
	m_AllowEventsList = NULL;
	m_AcceptEncodingList = NULL;
	m_AcceptLanguageList = NULL;
	m_AllowList = NULL;
	m_ContentEncodingList = NULL;
	m_ContactList = NULL;
	m_InReplyToList = NULL;
	m_ProxyRequireList = NULL;
	m_RequireList = NULL;
	m_SupportedList = NULL;
	m_UnsupportedList = NULL;

	/// Route Headers
	m_RouteList = NULL;
	m_RecordRouteList = NULL;
	m_PathList = NULL;

	/// special headers
	m_ViaList = NULL;
	m_CSeq = NULL;

	/// Custom Headers
	m_CustomHeaders = NULL;

	m_InternalHeaders = NULL;
}

void SIPMessage::AssignContents(
								SIPMessage & msg
								)const
{
	GlobalLock();



	msg.m_RawMessage = m_RawMessage;
	msg.m_RawMessageBody = m_RawMessageBody;
	msg.m_StartLine = m_StartLine;
	msg.m_IsFinal = m_IsFinal;
	/// Standad SIP Headers
	msg.m_AlertInfo = m_AlertInfo;
	msg.m_CallId = m_CallId;
	msg.m_CallInfo = m_CallInfo;
	msg.m_ContentDisposition = m_ContentDisposition;
	msg.m_ContentLanguage = m_ContentLanguage;
	msg.m_ContentLength = m_ContentLength;
	msg.m_ContentType = m_ContentType;
	msg.m_Date = m_Date;
	msg.m_ErrorInfo = m_ErrorInfo;
	msg.m_MIMEVersion = m_MIMEVersion;
	msg.m_Organization = m_Organization;
	msg.m_Event = m_Event;
	msg.m_SIPETag = m_SIPETag;
	msg.m_SIPIfMatch = m_SIPIfMatch;
	msg.m_SubscriptionState = m_SubscriptionState;
	msg.m_Priority = m_Priority;
	msg.m_RetryAfter = m_RetryAfter;
	msg.m_Server = m_Server;
	msg.m_Subject = m_Subject;
	msg.m_UserAgent = m_UserAgent;
	msg.m_Warning = m_Warning;

	/// URI headers
	msg.m_From = m_From;
	msg.m_To = m_To;
	msg.m_ReferTo = m_ReferTo;
	msg.m_Replaces = m_Replaces;
	msg.m_ReplyTo = m_ReplyTo;

	/// Integer headers
	msg.m_Expires = m_Expires;
	msg.m_SessionExpires = m_SessionExpires;
	msg.m_MinSE = m_MinSE;
	msg.m_MaxForwards = m_MaxForwards;
	msg.m_MinExpires = m_MinExpires;
	msg.m_TimeStamp = m_TimeStamp;

	/// Comma delimited named parameters
	msg.m_AuthenticationInfo = m_AuthenticationInfo;
	msg.m_Authorization = m_Authorization;
	msg.m_ProxyAuthenticate = m_ProxyAuthenticate;
	msg.m_ProxyAuthorization = m_ProxyAuthorization;
	msg.m_WWWAuthenticate = m_WWWAuthenticate;

	/// Comma delimited headers

	msg.m_AcceptList = m_AcceptList;
	msg.m_AllowEventsList = m_AllowEventsList;
	msg.m_AcceptEncodingList = m_AcceptEncodingList;
	msg.m_AcceptLanguageList = m_AcceptLanguageList;
	msg.m_AllowList = m_AllowList;
	msg.m_ContentEncodingList = m_ContentEncodingList;
	msg.m_ContactList = m_ContactList;
	msg.m_InReplyToList = m_InReplyToList;
	msg.m_ProxyRequireList = m_ProxyRequireList;
	msg.m_RequireList = m_RequireList;
	msg.m_SupportedList = m_SupportedList;
	msg.m_UnsupportedList = m_UnsupportedList;

	/// Route Headers
	msg.m_RouteList = m_RouteList;
	msg.m_RecordRouteList = m_RecordRouteList;
	msg.m_PathList = m_PathList;

	/// special headers
	msg.m_ViaList = m_ViaList;
	msg.m_CSeq = m_CSeq;

	/// Custom Headers
	msg.m_CustomHeaders = m_CustomHeaders;

	msg.m_InternalHeaders = m_InternalHeaders;

}

void SIPMessage::Finalize()
{
	GlobalLock();

	// cs( *this );

	if( m_IsFinal )
		return;

	m_IsFinal = TRUE;

	if( m_RawMessage == NULL )
		return;

	PStringArray & msg = *m_RawMessage;

	/// first parse the StartLine if its not set
	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		/// remove it so its not mistaken as a header
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		/// Standad SIP Headers

		if( hname == "alert-info" )
		{
			if( m_AlertInfo == NULL )
			{
				m_AlertInfo = new AlertInfo( h );
			}
		}else if( hname == "call-id" )
		{
			if( m_CallId == NULL )
			{
				m_CallId = new CallId( h );
			}
		}else if( hname == "call-info" )
		{
			if( m_CallInfo == NULL )
			{
				m_CallInfo = new CallInfo( h );
			}
		}else if( hname ==  "content-disposition" )
		{
			if( m_ContentDisposition == NULL )
			{
				m_ContentDisposition = new ContentDisposition( h );
			}
		}else if( hname == "content-language" )
		{
			if( m_ContentLanguage == NULL )
			{
				m_ContentLanguage = new ContentLanguage( h );
			}
		}else if( hname == "content-length" )
		{
			if( m_ContentLength == NULL )
			{
				m_ContentLength = new ContentLength( h );
			}
		}else if( hname == "content-type" )
		{
			if( m_ContentType == NULL )
			{
				m_ContentType = new ContentType( h );
			}
		}else if( hname == "date" )
		{
			if( m_Date == NULL )
			{
				m_Date = new Date( h );
			}
		}else if( hname == "error-info" )
		{
			if( m_ErrorInfo == NULL )
			{
				m_ErrorInfo = new ErrorInfo( h );
			}
		}else if( hname == "mime-version" )
		{
			if( m_MIMEVersion == NULL )
			{
				m_MIMEVersion = new MIMEVersion( h );
			}
		}else if( hname == "organization" )
		{
			if( m_Organization == NULL )
			{
				m_Organization = new Organization( h );
			}
		}else if( hname == "event" )
		{
			if( m_Event == NULL )
			{
				m_Event = new Event( h );
			}
		}else if( hname == "SIP-ETag" )
		{
			if( m_SIPETag == NULL )
			{
				m_SIPETag = new SIPETag( h );
			}
		}else if( hname == "SIP-If-Match" )
		{
			if( m_SIPIfMatch == NULL )
			{
				m_SIPIfMatch = new SIPIfMatch( h );
			}
		}else if( hname == "subscription-state" )
		{
			if( m_SubscriptionState == NULL )
			{
				m_SubscriptionState = new SubscriptionState( h );
			}
		}else if( hname == "priority" )
		{
			if( m_Priority == NULL )
			{
				m_Priority = new Priority( h );
			}
		}else if( hname == "retry-after" )
		{
			if( m_RetryAfter == NULL )
			{
				m_RetryAfter = new RetryAfter( h );
			}
		}else if( hname == "server" )
		{
			if( m_Server == NULL )
			{
				m_Server = new Server( h );
			}
		}else if( hname == "subject" )
		{
			if( m_Subject == NULL )
			{
				m_Subject = new Subject( h );
			}
		}else if( hname == "user-agent" )
		{
			if( m_UserAgent == NULL )
			{
				m_UserAgent = new UserAgent( h );
			}
		}else if( hname == "warning" )
		{
			if( m_Warning == NULL )
			{
				m_Warning = new Warning( h );
			}
		}else if( hname == "from" )
		{
			/// URI headers
			if( m_From == NULL )
			{
				m_From = new From( h );
			}
		}else if( hname == "to" )
		{
			if( m_To == NULL )
			{
				m_To = new To( h );
			}
		}else if( hname == "refer-to" )
		{
			if( m_ReferTo == NULL )
			{
				m_ReferTo = new ReferTo( h );
			}
		}else if( hname == "replaces" )
		{
			if( m_Replaces == NULL )
			{
				m_Replaces = new Replaces( h );
			}
		}else if( hname == "reply-to" )
		{
			if( m_ReplyTo == NULL )
			{
				m_ReplyTo = new ReplyTo( h );
			}
		}else if( hname == "expires" )
		{
			/// Integer headers
			if( m_Expires == NULL )
			{
				m_Expires = new Expires( h );
			}
		}else if( hname == "session-expires" )
		{
			/// Integer headers
			if( m_SessionExpires == NULL )
			{
				m_SessionExpires = new SessionExpires( h );
			}
		}else if( hname == "min-se" )
		{
			/// Integer headers
			if( m_MinSE == NULL )
			{
				m_MinSE = new MinSE( h );
			}
		}else if( hname == "max-forwards" )
		{
			if( m_MaxForwards == NULL )
			{
				m_MaxForwards = new MaxForwards( h );
			}
		}else if( hname == "min-expires" )
		{
			if( m_MinExpires == NULL )
			{
				m_MinExpires = new MinExpires( h );
			}
		}else if( hname == "time-stamp" )
		{
			if( m_TimeStamp == NULL )
			{
				m_TimeStamp = new TimeStamp( h );
			}
		}else if( hname == "authentication-info" )
		{
			/// Comma delimited named parameters
			if( m_AuthenticationInfo == NULL )
			{
				m_AuthenticationInfo = new AuthenticationInfo( h );
			}
		}else if( hname == "authorization" )
		{
			if( m_Authorization == NULL )
			{
				m_Authorization = new Authorization( h );
			}
		}else if( hname == "proxy-authenticate" )
		{
			if( m_ProxyAuthenticate == NULL )
			{
				m_ProxyAuthenticate = new ProxyAuthenticate( h );
			}
		}else if( hname == "proxy-authorization" )
		{
			if( m_ProxyAuthorization == NULL )
			{
				m_ProxyAuthorization = new ProxyAuthorization( h );
			}
		}else if( hname == "www-authenticate" )
		{
			if( m_WWWAuthenticate == NULL )
			{
				m_WWWAuthenticate = new WWWAuthenticate( h );
			}
		}else if( hname == "accept" )
		{
			/// Comma delimited headers
			if( m_AcceptList == NULL )
			{
				m_AcceptList = new AcceptList();
			}

			Accept accept( h );
			m_AcceptList->Append( accept );

		}else if( hname == "allow-events" )
		{
			/// Comma delimited headers
			if( m_AllowEventsList == NULL )
			{
				m_AllowEventsList = new AllowEventsList();
			}

			AllowEvents allowEvents( h );
			m_AllowEventsList->Append( allowEvents );

		}else if( hname == "accept-encoding" )
		{
			if( m_AcceptEncodingList == NULL )
			{
				m_AcceptEncodingList = new AcceptEncodingList();
			}

			AcceptEncoding acceptEncoding( h );
			m_AcceptEncodingList->Append( acceptEncoding );
		}else if( hname == "accept-language" )
		{
			if( m_AcceptLanguageList == NULL )
			{
				m_AcceptLanguageList = new AcceptLanguageList();
			}

			AcceptLanguage acceptLanguage( h );
			m_AcceptLanguageList->Append( acceptLanguage );

		}else if( hname == "allow" )
		{
			if( m_AllowList == NULL )
			{
				m_AllowList = new AllowList();
			}

			Allow allow(h);
			m_AllowList->Append( allow );

		}else if( hname == "content-encoding" )
		{
			if( m_ContentEncodingList == NULL )
			{
				m_ContentEncodingList = new ContentEncodingList();
			}

			ContentEncoding contentEncoding( h );
			m_ContentEncodingList->Append( contentEncoding );

		}else if( hname == "contact" )
		{
			if( m_ContactList == NULL )
			{
				m_ContactList = new ContactList();
			}

			Contact contact( h );
			m_ContactList->Append( contact );

		}else if( hname == "in-reply-to" )
		{
			if( m_InReplyToList == NULL )
			{
				m_InReplyToList = new InReplyToList();
			}

			InReplyTo inReplyTo( h );
			m_InReplyToList->Append( inReplyTo );

		}else if( hname == "proxy-require" )
		{
			if( m_ProxyRequireList == NULL )
			{
				m_ProxyRequireList = new ProxyRequireList();
			}

			ProxyRequire proxyRequire( h );
			m_ProxyRequireList->Append( proxyRequire );

		}else if( hname == "require" )
		{
			if( m_RequireList == NULL )
			{
				m_RequireList = new RequireList();
			}

			Require require( h );
			m_RequireList->Append( require );

		}else if( hname == "supported" )
		{
			if( m_SupportedList == NULL )
			{
				m_SupportedList = new SupportedList();
			}
			Supported supported( h );
			m_SupportedList->Append( supported );

		}else if( hname == "unsupported" )
		{
			if( m_UnsupportedList == NULL )
			{
				m_UnsupportedList = new UnsupportedList();
			}

			Unsupported unsupported( h );
			m_UnsupportedList->Append( unsupported );

		}else if( hname == "route" )
		{
			/// Route Headers
			if( m_RouteList == NULL )
			{
				m_RouteList = new RouteList();
			}

			Route route( h );
			m_RouteList->Append( route );

		}else if( hname == "record-route" )
		{
			if( m_RecordRouteList == NULL )
			{
				m_RecordRouteList = new RecordRouteList();
			}

			RecordRoute recordRoute( h );
			m_RecordRouteList->Append( recordRoute );

		}else if( hname == "path" )
		{
			if( m_PathList == NULL )
			{
				m_PathList = new PathList();
			}

			Path path( h );
			m_PathList->Append( path );

		}else if( hname == "via" )
		{
			/// special headers
			if( m_ViaList == NULL )
			{
				m_ViaList = new Via::Collection();
			}
			m_ViaList->Append( new Via( h ) );
		}else if( hname == "cseq" )
		{
			if( m_CSeq == NULL )
			{
				m_CSeq = new CSeq( h );
			}
		}else
		{
			/// Custom Headers
			if( m_CustomHeaders == NULL )
			{
				m_CustomHeaders = new CustomHeaders();
			}

			m_CustomHeaders->Append( new SIPHeader( h ) );
		}
	}



	delete m_RawMessage;
	m_RawMessage = NULL;

}


//////////////////////////////////////////////////////
///PString * m_RawMessageBody;
BOOL SIPMessage::GetBody(
						 PString & body
						 )const
{
	if( !HasBody() )
		return FALSE;
	body = *m_RawMessageBody;
	return TRUE;
}

PString SIPMessage::GetBody()const
{
	PString body;
	GetBody( body );
	return body;
}

void SIPMessage::SetSDP(
						const PString & sdp 
						)
{
	SetContentType( "application/sdp" );
	SetBody( sdp );
}

void SIPMessage::SetBody(
						 const PString & body
						 )
{
	// cs( *this );

	MakeUnique();

	if( m_RawMessageBody != NULL )
		delete m_RawMessageBody;

	if( body.IsEmpty() )
	{
		m_RawMessageBody = NULL;
		ContentLength contentLenth( "0" );
		SetContentLength( contentLenth );

		if( m_ContentType != NULL )
		{
			delete m_ContentType;
			m_ContentType = NULL;
		}
		return;
	}

	m_RawMessageBody = new PString( body );

	PString cl( body.GetLength() );
	ContentLength contentLenth( cl );
	SetContentLength( contentLenth );
}

BOOL SIPMessage::HasBody()const
{
	return m_RawMessageBody != NULL;
}

BOOL SIPMessage::HasSDP()const
{
	if( !HasBody() )
		return FALSE;

	ContentType ctype;

	if( !GetContentType( ctype ) )
		return FALSE;

	PString ct = ctype.GetHeaderBody();

	return ct *= "application/sdp";
}

BOOL SIPMessage::AppendRoute(
							 const RouteURI & uri,
							 PINDEX idx
							 )
{
	GlobalLock();

	// cs( *this );
	ParseRouteList();
	MakeUnique();

	if( m_RouteList == NULL )
		m_RouteList = new RouteList();

	if( m_RouteList->GetSize() == 0 )
	{
		Route route;
		route.AddURI( uri );
		return m_RouteList->Append( route );
	}else
	{
		return m_RouteList->Append( uri, idx );
	}
}

BOOL SIPMessage::AppendRoute(
							 const Route & header
							 )
{
	GlobalLock();

	// cs( *this );
	ParseRouteList();
	MakeUnique();

	if( m_RouteList == NULL )
		m_RouteList = new RouteList();

	return m_RouteList->Append( header );
}

RouteList SIPMessage::GetRouteList()const
{
	GlobalLock();

	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseRouteList();

	RouteList routeList;

	if( m_RouteList != NULL )
	{
		routeList = RouteList( *m_RouteList );
		routeList.MakeUnique();
	}

	return routeList;
}

BOOL SIPMessage::SetRouteList(
							  const RouteList & list
							  )
{
	GlobalLock();

	RemoveAllRoutes();

	m_RouteList = new RouteList( list );

	return TRUE;
}

BOOL SIPMessage::RemoveAllRoutes()
{
	GlobalLock();

	// cs( *this );

	MakeUnique();

	if( m_RouteList != NULL )
	{
		delete m_RouteList;
		m_RouteList = NULL;
	}

	return TRUE;
}

Route SIPMessage::GetRouteAt(
							 PINDEX idx
							 )const
{
	GlobalLock();

	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseRouteList();
	return m_RouteList->GetAt( idx );
}

RouteURI SIPMessage::GetRouteTokenAt(
									 PINDEX idx1,
									 PINDEX idx2
									 )const
{
	GlobalLock();

	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseRouteList();
	return m_RouteList->GetAt( idx1, idx2 );
}

PINDEX SIPMessage::GetRouteSize()const
{
	GlobalLock();

	// cs( *this );
	if( !(const_cast< SIPMessage * >( this ))->ParseRouteList() )
		return 0;
	return m_RouteList->GetSize();
}

RouteURI SIPMessage::PopTopRouteURI()
{
	GlobalLock();

	// cs( *this );
	ParseRouteList();
	MakeUnique();
	return m_RouteList->PopTopURI();
}

RouteURI SIPMessage::PopLastRouteURI()
{
	GlobalLock();

	// cs( *this );
	ParseRouteList();
	MakeUnique();
	return m_RouteList->PopLastURI();
}

BOOL SIPMessage::HasRoute()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseRouteList();
}

BOOL SIPMessage::AppendRecordRoute(
								   const RouteURI & uri,
								   PINDEX idx
								   )
{
	GlobalLock();
	// cs( *this );
	ParseRecordRouteList();
	MakeUnique();
	if( m_RecordRouteList == NULL )
		m_RecordRouteList = new RecordRouteList();

	if( m_RecordRouteList->GetSize() == 0 )
	{
		RecordRoute route;
		route.AddURI( uri );
		return m_RecordRouteList->Append( route );
	}else
	{
		return m_RecordRouteList->Append( uri, idx );
	}
}

BOOL SIPMessage::RemoveAllRecordRoutes()
{
	GlobalLock();

	// cs( *this );

	MakeUnique();

	if( m_RecordRouteList != NULL )
	{
		delete m_RecordRouteList;
		m_RecordRouteList = NULL;
	}

	return TRUE;
}


BOOL SIPMessage::AppendRecordRoute(
								   const RecordRoute & header
								   )
{
	GlobalLock();
	// cs( *this );
	ParseRecordRouteList();
	MakeUnique();

	if( m_RecordRouteList == NULL )
		m_RecordRouteList = new RecordRouteList();

	return m_RecordRouteList->Append( header );
}

RecordRouteList SIPMessage::GetRecordRouteList()const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseRecordRouteList();

	RecordRouteList recordRouteList;

	if( m_RecordRouteList != NULL )
	{
		recordRouteList = RecordRouteList( *m_RecordRouteList );
		recordRouteList.MakeUnique();
	}

	return recordRouteList;
}

BOOL SIPMessage::SetRecordRouteList(
									const RecordRouteList & list
									)
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_RecordRouteList != NULL )
	{
		delete m_RecordRouteList;
		m_RecordRouteList = NULL;
	}

	m_RecordRouteList = new RecordRouteList( list );

	return TRUE;
}


RecordRoute SIPMessage::GetRecordRouteAt(
	PINDEX idx
	)const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseRecordRouteList();
	return m_RecordRouteList->GetAt( idx );
}

RouteURI SIPMessage::GetRecordRouteTokenAt(
	PINDEX idx1,
	PINDEX idx2
	)const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseRecordRouteList();
	return m_RecordRouteList->GetAt( idx1, idx2 );
}

PINDEX SIPMessage::GetRecordRouteSize()const
{
	GlobalLock();
	// cs( *this );
	if( !(const_cast< SIPMessage * >( this ))->ParseRecordRouteList() )
		return 0;

	return m_RecordRouteList->GetSize();
}

BOOL SIPMessage::CloneRecordRoutes(
								   SIPMessage & msg
								   )const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseRecordRouteList();

	if( m_RecordRouteList == NULL )
		return FALSE;

	if( m_RecordRouteList->GetSize() == 0 )
		return FALSE;

	msg.MakeUnique();

	if( msg.m_RecordRouteList != NULL )
		delete m_RecordRouteList;

	msg.m_RecordRouteList = (RecordRouteList*)m_RecordRouteList->Clone();

	return TRUE;
}

RouteURI SIPMessage::PopTopRecordRouteURI()
{
	GlobalLock();
	// cs( *this );
	ParseRecordRouteList();
	MakeUnique();
	return m_RecordRouteList->PopTopURI();
}

BOOL SIPMessage::AppendPath(
							const RouteURI & uri,
							PINDEX idx
							)
{
	GlobalLock();
	// cs( *this );
	ParsePathList();
	MakeUnique();
	if( m_PathList == NULL )
		m_PathList = new PathList();

	if( m_PathList->GetSize() == 0 )
	{
		Path route;
		route.AddURI( uri );
		return m_PathList->Append( route );
	}else
	{
		return m_PathList->Append( uri, idx );
	}
}

BOOL SIPMessage::AppendPath(
							const Path & header
							)
{
	GlobalLock();
	// cs( *this );
	ParsePathList();
	MakeUnique();

	if( m_PathList == NULL )
		m_PathList = new PathList();

	return m_PathList->Append( header );
}

PathList SIPMessage::GetPathList()const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParsePathList();

	PathList pathList;

	if( m_PathList != NULL )
	{
		pathList = PathList( *m_PathList );
		pathList.MakeUnique();
	}

	return pathList;
}

BOOL SIPMessage::SetPathList(
							 const PathList & list
							 )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_PathList != NULL )
	{
		delete m_PathList;
		m_PathList = NULL;
	}

	m_PathList = new PathList( list );

	return TRUE;
}


Path SIPMessage::GetPathAt(
						   PINDEX idx
						   )const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParsePathList();
	return m_PathList->GetAt( idx );
}

RouteURI SIPMessage::GetPathTokenAt(
									PINDEX idx1,
									PINDEX idx2
									)const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParsePathList();
	return m_PathList->GetAt( idx1, idx2 );
}

PINDEX SIPMessage::GetPathSize()const
{
	GlobalLock();
	// cs( *this );
	if( !(const_cast< SIPMessage * >( this ))->ParsePathList() )
		return 0;

	return m_PathList->GetSize();
}

BOOL SIPMessage::ClonePaths(
							SIPMessage & msg
							)const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParsePathList();

	if( m_PathList == NULL )
		return FALSE;

	if( m_PathList->GetSize() == 0 )
		return FALSE;

	msg.MakeUnique();

	if( msg.m_PathList != NULL )
		delete m_PathList;

	msg.m_PathList = (PathList*)m_PathList->Clone();

	return TRUE;
}

RouteURI SIPMessage::PopTopPathURI()
{
	GlobalLock();
	// cs( *this );
	ParsePathList();
	MakeUnique();
	return m_PathList->PopTopURI();
}

ViaList SIPMessage::GetViaList()const
{
	GlobalLock();

	(const_cast< SIPMessage * >( this ))->ParseViaList();

	ViaList viaList;

	if( m_ViaList != NULL )
	{
		viaList = *m_ViaList;
		viaList.MakeUnique();
	}

	return viaList;
}

BOOL SIPMessage::FixViaNATParameters()
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseViaList();
	if( m_ViaList == NULL )
		return FALSE;

	if( m_ViaList->GetSize() == 0 )
		return FALSE;

	Via & via = (*m_ViaList)[0];

	SIPHeader a("");
	if( GetInternalHeader( "RCVADDR", a ) && !a.GetHeaderBody().IsEmpty() )
		via.SetReceiveAddress( PIPSocket::Address( a.GetHeaderBody() ) );

	SIPHeader b("");
	if( GetInternalHeader( "RCVPORT", b ) && !b.GetHeaderBody().IsEmpty() )
		via.SetRPort( (WORD)b.GetHeaderBody().AsUnsigned() );

	return TRUE;
}

BOOL SIPMessage::SetViaList(
							const ViaList & list
							)
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_ViaList != NULL )
	{
		delete m_ViaList;
		m_ViaList = NULL;
	}

	m_ViaList = new ViaList( list );

	return TRUE;
}

//// VIA
BOOL SIPMessage::ParseViaList()
{
	GlobalLock();
	//  cs( *this );

	if( m_ViaList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	/// first parse the StartLine if its not set
	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		/// remove it so its not mistaken as a header
		msg.RemoveAt( 0 );
	}

	PIntArray viaIdx;
	PINDEX currentIdx = 0;
	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		/// Standad SIP Headers

		if( hname == "via" )
		{
			/// special headers
			if( m_ViaList == NULL )
			{
				m_ViaList = new Via::Collection();
			}
			m_ViaList->Append( new Via( h ) );
			viaIdx[currentIdx] = i;
			currentIdx++;
		}
	}

	for( PINDEX j = 0; j < viaIdx.GetSize(); j++ )
		msg.RemoveAt( viaIdx[j] );

	return m_ViaList != NULL;
}

BOOL SIPMessage::AppendVia(
						   const Via & header
						   )
{
	GlobalLock();
	// cs( *this );
	ParseViaList();

	MakeUnique();

	if( m_ViaList == NULL )
	{
		m_ViaList = new Via::Collection();
		m_ViaList->Append( new Via( header ) );
	}else
	{

		/// sanity check
		if( m_ViaList->GetSize() > 0 )
		{
			Via & topVia = (*m_ViaList)[0];
			SIPURI topViaURI = topVia.GetURI();
			SIPURI newURI = header.GetURI();
			if( SIPTransport::IsTheSameAddress( topViaURI, newURI, TRUE ) )
				return FALSE;
		}

		///we append it on top
		Via::Collection oldViaList = *m_ViaList;
		delete m_ViaList;

		m_ViaList = new Via::Collection();
		m_ViaList->Append( new Via( header ) );

		for( PINDEX i = 0; i < oldViaList.GetSize(); i++ )
			m_ViaList->Append( new Via( oldViaList[i] ) );
	}


	return TRUE;
}

BOOL SIPMessage::GetViaAt(
						  PINDEX idx,
						  Via & via
						  )const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseViaList();

	if( m_ViaList == NULL )
		return FALSE;

	if( idx > m_ViaList->GetSize() - 1 )
		return FALSE;

	via = (*m_ViaList)[idx];

	return TRUE;
}

BOOL SIPMessage::SetViaAt(
						  PINDEX idx,
						  const Via & header
						  )
{
	GlobalLock();
	// cs( *this );
	ParseViaList();
	MakeUnique();

	if( m_ViaList == NULL )
		return FALSE;

	if( idx > m_ViaList->GetSize() - 1 )
		return FALSE;

	(*m_ViaList)[idx] = header;

	return TRUE;
}

void SIPMessage::RemoveAllVias()
{
	GlobalLock();
	ParseViaList();

	if( m_ViaList != NULL )
	{
		delete m_ViaList;
		m_ViaList = NULL;
	}
}

Via SIPMessage::PopTopVia()
{
	GlobalLock();
	// cs( *this );
	ParseViaList();
	Via via;

	if( m_ViaList == NULL )
		return via;

	if( m_ViaList->GetSize() == 0 )
		return via;

	via = (*m_ViaList)[0];

	m_ViaList->RemoveAt(0);
	return via;
}

BOOL SIPMessage::SetViaParameter(
								 const PString & name,
								 const PString & value
								 )
{
	GlobalLock();
	// cs( *this );
	ParseViaList();

	if( m_ViaList == NULL )
		return FALSE;

	if( m_ViaList->GetSize() == 0 )
		return FALSE;

	Via & via = (*m_ViaList)[0];
	via.SetParameter( name, value );

	return TRUE;
}

PINDEX SIPMessage::GetViaSize()const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseViaList();

	if( m_ViaList == NULL )
		return 0;

	return m_ViaList->GetSize();
}

BOOL SIPMessage::CloneVias( SIPMessage & msg )const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseViaList();

	if( m_ViaList == NULL )
		return FALSE;

	if( m_ViaList->GetSize() == 0 )
		return FALSE;

	msg.MakeUnique();

	if( msg.m_ViaList != NULL )
		delete msg.m_ViaList;

	msg.m_ViaList = new Via::Collection( *m_ViaList );

	msg.m_ViaList->MakeUnique();

	return TRUE;
}

/// Custom Headers
///PLIST( CustomHeaders, SIPHeader );
///CustomHeaders * m_CustomHeaders;

BOOL SIPMessage::ParseCustomHeaders()
{
	GlobalLock();
	//  cs( *this );

	if( m_CustomHeaders != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	/// first parse the StartLine if its not set
	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		/// remove it so its not mistaken as a header
		msg.RemoveAt( 0 );
	}

	PIntArray cIdx;
	PINDEX currentIdx = 0;
	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		/// Standad SIP Headers

		if( !ParserTools::IsKnownHeader( hname ) )
		{
			/// Custom Headers
			if( m_CustomHeaders == NULL )
			{
				m_CustomHeaders = new CustomHeaders();
			}

			m_CustomHeaders->Append( new SIPHeader( h ) );
			cIdx[currentIdx++] = i;
		}

		for( PINDEX j = 0; j < cIdx.GetSize(); j++ )
			msg.RemoveAt( cIdx[j] );
	}

	return m_CustomHeaders != NULL;
}

BOOL SIPMessage::GetCustomHeader(
								 const PCaselessString & name,
								 SIPHeader & header,
								 PINDEX idx
								 )const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseCustomHeaders();

	if( m_CustomHeaders == NULL )
		return FALSE;

	PINDEX currentIndex = 0;
	BOOL ok = FALSE;
	for( PINDEX i = 0; i < m_CustomHeaders->GetSize(); i++ )
	{
		if( (*m_CustomHeaders)[i].GetHeaderName() *= name )
		{
			if( currentIndex == idx )
			{
				header = (*m_CustomHeaders)[i];
				ok = TRUE;
				break;
			}else
				currentIndex++;
		}
	}

	return ok;
}

BOOL SIPMessage::GetCustomHeader(
								 SIPHeader & h,
								 PINDEX i
								 )const
{

	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseCustomHeaders();

	if( m_CustomHeaders == NULL )
		return FALSE;

	if( i > m_CustomHeaders->GetSize() - 1 )
		return FALSE;

	h = (*m_CustomHeaders)[i];

	return TRUE;
}


void SIPMessage::SetCustomHeader(
								 const PCaselessString & name,
								 const SIPHeader & header,
								 PINDEX idx
								 )
{
	GlobalLock();
	// cs( *this );
	ParseCustomHeaders();
	MakeUnique();

	if( m_CustomHeaders == NULL )
		return;

	PINDEX currentIndex = 0;
	BOOL ok = FALSE;
	for( PINDEX i = 0; i < m_CustomHeaders->GetSize(); i++ )
	{
		if( (*m_CustomHeaders)[i].GetHeaderName() *= name )
		{
			if( currentIndex == idx )
			{
				(*m_CustomHeaders)[i] = header;
				ok = TRUE;
				break;
			}else
				currentIndex++;
		}
	}
}

void SIPMessage::SetCustomHeader(
								 const SIPHeader & header,
								 PINDEX i
								 )
{
	GlobalLock();
	// cs( *this );
	ParseCustomHeaders();
	MakeUnique();

	if( m_CustomHeaders == NULL )
		return;

	if( i > m_CustomHeaders->GetSize() - 1 )
		return;

	(*m_CustomHeaders)[i] == header;
}

void SIPMessage::AddCustomHeader(
								 const SIPHeader & header
								 )
{
	GlobalLock();
	// cs( *this );
	ParseCustomHeaders();
	MakeUnique();

	if( m_CustomHeaders == NULL )
		m_CustomHeaders = new CustomHeaders();

	m_CustomHeaders->Append( new SIPHeader( header ) );
}

BOOL SIPMessage::PopCustomHeader(
								 const PCaselessString & name,
								 SIPHeader & h,
								 PINDEX idx
								 )
{
	GlobalLock();
	// cs( *this );
	ParseCustomHeaders();

	if( m_CustomHeaders == NULL )
		return FALSE;

	PINDEX currentIndex = 0;
	BOOL ok = FALSE;
	for( PINDEX i = 0; i < m_CustomHeaders->GetSize(); i++ )
	{
		if( (*m_CustomHeaders)[i].GetHeaderName() *= name )
		{
			if( currentIndex == idx )
			{
				h = (*m_CustomHeaders)[i];
				m_CustomHeaders->RemoveAt(i);
				ok = TRUE;
				break;
			}else
				currentIndex++;
		}
	}

	return ok;
}

BOOL SIPMessage::HasInternalHeader(
								   const PCaselessString & name
								   )const
{
	return !GetInternalHeader( name ).IsEmpty();
}

PString SIPMessage::GetInternalHeader(
									  const PCaselessString & name,
									  PINDEX i
									  )const
{
	GlobalLock();
	SIPHeader h("");
	if( !GetInternalHeader( name, h, i ) )
		return PString::Empty();

	return h.GetHeaderBody();
}

BOOL SIPMessage::GetInternalHeader(
								   const PCaselessString & name,
								   SIPHeader & header,
								   PINDEX idx
								   )const
{
	GlobalLock();
	// cs( *this );
	///ParseInternalHeaders();

	if( m_InternalHeaders == NULL )
		return FALSE;

	PINDEX currentIndex = 0;
	BOOL ok = FALSE;
	for( PINDEX i = 0; i < m_InternalHeaders->GetSize(); i++ )
	{
		if( (*m_InternalHeaders)[i].GetHeaderName() *= name )
		{
			if( currentIndex == idx )
			{
				header = (*m_InternalHeaders)[i];
				ok = TRUE;
				break;
			}else
				currentIndex++;
		}
	}

	return ok;
}

BOOL SIPMessage::GetInternalHeader(
								   SIPHeader & h,
								   PINDEX i
								   )const
{

	GlobalLock();
	// cs( *this );
	///ParseInternalHeaders();

	if( m_InternalHeaders == NULL )
		return FALSE;

	if( i > m_InternalHeaders->GetSize() - 1 )
		return FALSE;

	h = (*m_InternalHeaders)[i];

	return TRUE;
}

void SIPMessage::SetInternalHeader( 
								   const PCaselessString & name,
								   const PString & value,
								   PINDEX index
								   )
{
	GlobalLock();
	SIPHeader h( name );
	h.SetHeaderBody( value );
	SetInternalHeader( name, h, index );
}

void SIPMessage::SetInternalHeader(
								   const PCaselessString & name,
								   const SIPHeader & header,
								   PINDEX idx
								   )
{
	GlobalLock();
	// cs( *this ) ;

	MakeUnique();

	if( m_InternalHeaders == NULL )
		m_InternalHeaders = new InternalHeaders();

	PINDEX currentIndex = 0;
	BOOL ok = FALSE;
	for( PINDEX i = 0; i < m_InternalHeaders->GetSize(); i++ )
	{
		if( (*m_InternalHeaders)[i].GetHeaderName() *= name )
		{
			if( currentIndex == idx )
			{
				(*m_InternalHeaders)[i] = header;
				ok = TRUE;
				break;
			}else
				currentIndex++;
		}
	}

	PString value = header.GetHeaderBody();
	if( !ok )
		AddInternalHeader( name, value );
}

void SIPMessage::SetInternalHeader(
								   const SIPHeader & header,
								   PINDEX i
								   )
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_InternalHeaders == NULL )
		m_InternalHeaders = new InternalHeaders();

	if( i > m_InternalHeaders->GetSize() - 1 )
		return;

	(*m_InternalHeaders)[i] == header;
}

void SIPMessage::AddInternalHeader(
								   const PString & name,
								   const PString & value
								   )
{
	GlobalLock();
	SIPHeader h( name, value );
	AddInternalHeader( h );
}

void SIPMessage::ClearInternalHeaders()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_InternalHeaders != NULL )
		m_InternalHeaders->RemoveAll();
}

void SIPMessage::AddInternalHeader(
								   const SIPHeader & header
								   )
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_InternalHeaders == NULL )
		m_InternalHeaders = new InternalHeaders();

	m_InternalHeaders->Append( new SIPHeader( header ) );
}

BOOL SIPMessage::PopInternalHeader(
								   const PCaselessString & name,
								   SIPHeader & h,
								   PINDEX idx
								   )
{
	GlobalLock();
	// cs( *this );
	///ParseInternalHeaders();

	if( m_InternalHeaders == NULL )
		return FALSE;

	PINDEX currentIndex = 0;
	BOOL ok = FALSE;
	for( PINDEX i = 0; i < m_InternalHeaders->GetSize(); i++ )
	{
		PString hname = (*m_InternalHeaders)[i].GetHeaderName();
		if( hname == name )
		{
			if( currentIndex == idx )
			{
				h = (*m_InternalHeaders)[i];
				m_InternalHeaders->RemoveAt(i);
				ok = TRUE;
				break;
			}else
				currentIndex++;
		}
	}

	return ok;
}

BOOL SIPMessage::GetInternalHeader(
								   const PCaselessString & name,
								   SIPHeader & h,
								   PINDEX idx
								   )
{
	GlobalLock();
	// cs( *this );
	///ParseInternalHeaders();

	if( m_InternalHeaders == NULL )
		return FALSE;

	PINDEX currentIndex = 0;
	BOOL ok = FALSE;
	for( PINDEX i = 0; i < m_InternalHeaders->GetSize(); i++ )
	{
		if( (*m_InternalHeaders)[i].GetHeaderName() == name )
		{
			if( currentIndex == idx )
			{
				h = (*m_InternalHeaders)[i];
				ok = TRUE;
				break;
			}else
				currentIndex++;
		}
	}

	return ok;
}


TransactionId SIPMessage::GetTransactionId()const
{
	GlobalLock();
	return TransactionId( *this );
}


BOOL SIPMessage::IsRequest()const
{
	GlobalLock();
	StartLine startLine;
	GetStartLine( startLine );
	return startLine.IsRequest();
}

BOOL SIPMessage::IsMethod(
						  const PCaselessString & meth
						  ) const
{
	GlobalLock();
	StartLine startLine;
	GetStartLine( startLine );
	if( !startLine.IsRequest() )
		return FALSE;

	if( startLine.GetRequestLine() == NULL )
		return FALSE;

	return startLine.GetRequestLine()->GetMethod() == meth;
}

BOOL SIPMessage::IsInvite()const
{
	return IsMethod("INVITE" );
}

BOOL SIPMessage::IsAck()const
{
	return IsMethod( "ACK" );
}

BOOL SIPMessage::IsBye()const
{
	return IsMethod( "BYE" );
}

BOOL SIPMessage::IsOptions()const
{
	return IsMethod( "OPTIONS" );
}

BOOL SIPMessage::IsInfo()const
{
	return IsMethod( "INFO" );
}

BOOL SIPMessage::IsRegister()const
{
	return IsMethod( "REGISTER" );
}

BOOL SIPMessage::IsSubscribe()const
{
	return IsMethod( "SUBSCRIBE" );
}

BOOL SIPMessage::IsUnsubscribe()const
{
	GlobalLock();

	if( GetCSeqMethod().ToUpper().Trim() != "SUBSCRIBE" )
		return FALSE;

	if( HasExpires() )
	{
		Expires expires;
		GetExpires( expires );
		if( expires.AsInt64() == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL SIPMessage::IsNotify()const
{
	return IsMethod( "NOTIFY" );
}

BOOL SIPMessage::IsPublish()const
{
	return IsMethod( "PUBLISH" );
}


BOOL SIPMessage::IsRefer()const
{
	return IsMethod( "REFER" );
}

BOOL SIPMessage::IsMessage()const
{
	return IsMethod( "MESSAGE" );
}

BOOL SIPMessage::IsUpdate()const
{
	return IsMethod( "UPDATE" );
}


BOOL SIPMessage::IsUnregister()const
{
	GlobalLock();

	///if( !IsRequest() || !IsRegister() )
	///  return FALSE;

	/// only check the CSeq method to include evaluation
	/// of 200 Ok response for unregistration.  This
	/// is beneficial for proxies hi-jacking Upper Registrations

	if( GetCSeqMethod().ToUpper().Trim() != "REGISTER" )
		return FALSE;

	if( GetContactSize() > 0 )
	{
		Contact contact = GetContactAt( 0 );
		if( contact.GetHeaderBody() *= "*" )
		{
			return TRUE;
		}else /// check if expires == 0
		{
			ContactURI contactURI;
			if( contact.GetURI( contactURI ) )
			{
				PString paramValue;
				contactURI.GetParameter( "expires", paramValue );
				if( paramValue *= "0" )
					return TRUE;
			}
		}
	}

	if( HasExpires() )
	{
		Expires expires;
		GetExpires( expires );
		if( expires.AsInt64() == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL SIPMessage::IsUnpublish()const
{
	GlobalLock();

	if( !IsPublish() )
		return FALSE;

	//check Expires
	Expires expires;
	GetExpires( expires );
	if( expires.AsInt64() == 0 )
		return TRUE;

	return FALSE;
}

BOOL SIPMessage::IsCancel()const
{
	return IsMethod( "CANCEL" );
}

BOOL SIPMessage::IsKeepAlive()const
{
	return IsMethod( "KEEP-ALIVE" );
}

BOOL SIPMessage::IsBusyHere() const
{
	//return IsMethod( "BUSY HERE" );

	GlobalLock();

	StartLine	startLine;
	WORD		statusCode;

	GetStartLine(startLine);
	if(startLine.IsRequest())
	{
		return FALSE;
	}

	if(startLine.GetStatusLine() == NULL)
	{
		return FALSE;
	}

	statusCode = startLine.GetStatusLine()->GetStatusCode();
	return(statusCode == 486);
}

BOOL SIPMessage::IsStatusCode(
							  int baseCode
							  ) const
{
	GlobalLock();
	StartLine startLine;
	GetStartLine( startLine );
	if( startLine.IsRequest() )
		return FALSE;

	if( startLine.GetStatusLine() == NULL )
		return FALSE;

	WORD statusCode = startLine.GetStatusLine()->GetStatusCode();

	return statusCode >= baseCode && statusCode < baseCode + 100;
}

BOOL SIPMessage::IsOkToInvite() const
{
	BOOL	bRet = FALSE;

	GlobalLock();

	if(IsMethod("OK"))
	{
		if(GetCSeqMethod() *= "INVITE")
		{
			bRet = TRUE;
		}
	}

	return(bRet);
}

BOOL SIPMessage::Is1xx()const
{
	return IsStatusCode( 100 );
}


BOOL SIPMessage::Is2xx()const
{
	return IsStatusCode( 200 );
}

BOOL SIPMessage::Is3xx()const
{
	return IsStatusCode( 300 );
}

BOOL SIPMessage::Is4xx()const
{
	return IsStatusCode( 400 );
}

BOOL SIPMessage::Is5xx()const
{
	return IsStatusCode( 500 );
}

BOOL SIPMessage::Is6xx()const
{
	return IsStatusCode( 600 );
}


///////////////////////////////////////////////////////////


///CREATE_STANDARD_PARSER( Authorization, "authorization" );

BOOL SIPMessage::ParseAuthorization()
{
	GlobalLock();
	// cs( *this );

	if( m_Authorization != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "authorization" )
		{
			if( m_Authorization == NULL )
			{
				m_Authorization = new Authorization( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_Authorization != NULL;
}

BOOL SIPMessage::GetAuthorization(
								  Authorization & h
								  )const
{
	GlobalLock();
	//if( !ParseAuthorization() )
	if( !(const_cast< SIPMessage * >( this ))->ParseAuthorization() )
		return FALSE;

	if( !HasAuthorization() )
		return FALSE;

	h = *m_Authorization;

	return TRUE;
}

void SIPMessage::SetAuthorization(
								  const Authorization & h
								  )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_Authorization != NULL )
		delete m_Authorization;
	m_Authorization = (Authorization*)h.Clone();
}

BOOL SIPMessage::HasAuthorization()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseAuthorization();
}


////////////////////////////////////////////////////////////


BOOL SIPMessage::ParseAlertInfo()
{
	GlobalLock();
	// cs( *this );

	if( m_AlertInfo != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "alert-info" )
		{
			if( m_AlertInfo == NULL )
			{
				m_AlertInfo = new AlertInfo( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_AlertInfo != NULL;
}

BOOL SIPMessage::GetAlertInfo(
							  AlertInfo & h
							  )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseAlertInfo() )
		return FALSE;

	if( !HasAlertInfo() )
		return FALSE;

	h = *m_AlertInfo;

	return TRUE;
}

void SIPMessage::SetAlertInfo(
							  const AlertInfo & h
							  )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_AlertInfo != NULL )
		delete m_AlertInfo;
	m_AlertInfo = (AlertInfo*)h.Clone();
}

BOOL SIPMessage::HasAlertInfo()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseAlertInfo();
}

////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseCallId()
{
	GlobalLock();
	// cs( *this );

	if( m_CallId != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "call-id" )
		{
			if( m_CallId == NULL )
			{
				m_CallId = new CallId( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_CallId != NULL;
}

BOOL SIPMessage::GetCallId(
						   CallId & h
						   )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseCallId() )
		return FALSE;

	if( !HasCallId() )
		return FALSE;

	h = *m_CallId;

	return TRUE;
}

PString SIPMessage::GetCallId()const
{
	GlobalLock();
	CallId callId;
	if( !GetCallId( callId ) )
		return PString::Empty();

	return callId.GetHeaderBody();
}

void SIPMessage::SetCallId(
						   const CallId & h
						   )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_CallId != NULL )
		delete m_CallId;
	m_CallId = (CallId*)h.Clone();
}

BOOL SIPMessage::HasCallId()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseCallId();
}

/////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseCallInfo()
{
	GlobalLock();
	// cs( *this );

	if( m_CallInfo != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "call-info" )
		{
			if( m_CallInfo == NULL )
			{
				m_CallInfo = new CallInfo( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_CallInfo != NULL;
}

BOOL SIPMessage::GetCallInfo(
							 CallInfo & h
							 )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseCallInfo() )
		return FALSE;

	if( !HasCallInfo() )
		return FALSE;

	h = *m_CallInfo;

	return TRUE;
}

void SIPMessage::SetCallInfo(
							 const CallInfo & h
							 )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_CallInfo != NULL )
		delete m_CallInfo;
	m_CallInfo = (CallInfo*)h.Clone();
}

BOOL SIPMessage::HasCallInfo()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseCallInfo();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseContentDisposition()
{
	GlobalLock();
	// cs( *this );

	if( m_ContentDisposition != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "content-disposition" )
		{
			if( m_ContentDisposition == NULL )
			{
				m_ContentDisposition = new ContentDisposition( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_ContentDisposition != NULL;
}

BOOL SIPMessage::GetContentDisposition(
									   ContentDisposition & h
									   )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseContentDisposition() )
		return FALSE;

	if( !HasContentDisposition() )
		return FALSE;

	h = *m_ContentDisposition;

	return TRUE;
}

void SIPMessage::SetContentDisposition(
									   const ContentDisposition & h
									   )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_ContentDisposition != NULL )
		delete m_ContentDisposition;
	m_ContentDisposition = (ContentDisposition*)h.Clone();
}

BOOL SIPMessage::HasContentDisposition()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseContentDisposition();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseContentLanguage()
{
	GlobalLock();
	// cs( *this );

	if( m_ContentLanguage != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "content-language" )
		{
			if( m_ContentLanguage == NULL )
			{
				m_ContentLanguage = new ContentLanguage( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_ContentLanguage != NULL;
}

BOOL SIPMessage::GetContentLanguage(
									ContentLanguage & h
									)const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseContentLanguage() )
		return FALSE;

	if( !HasContentLanguage() )
		return FALSE;

	h = *m_ContentLanguage;

	return TRUE;
}

void SIPMessage::SetContentLanguage(
									const ContentLanguage & h
									)
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_ContentLanguage != NULL )
		delete m_ContentLanguage;
	m_ContentLanguage = (ContentLanguage*)h.Clone();
}

BOOL SIPMessage::HasContentLanguage()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseContentLanguage();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseContentLength()
{
	GlobalLock();
	// cs( *this );

	if( m_ContentLength != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "content-length" )
		{
			if( m_ContentLength == NULL )
			{
				m_ContentLength = new ContentLength( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_ContentLength != NULL;
}

BOOL SIPMessage::GetContentLength(
								  ContentLength & h
								  )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseContentLength() )
		return FALSE;

	if( !HasContentLength() )
		return FALSE;

	h = *m_ContentLength;

	return TRUE;
}

void SIPMessage::SetContentLength(
								  DWORD len
								  )
{
	GlobalLock();
	ContentLength h;
	h.SetHeaderBody( PString( len ));
	SetContentLength( h );
}

void SIPMessage::SetContentLength(
								  const ContentLength & h
								  )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_ContentLength != NULL )
		delete m_ContentLength;
	m_ContentLength = (ContentLength*)h.Clone();
}

BOOL SIPMessage::HasContentLength()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseContentLength();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseContentType()
{
	GlobalLock();
	// cs( *this );

	if( m_ContentType != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "content-type" )
		{
			if( m_ContentType == NULL )
			{
				m_ContentType = new ContentType( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_ContentType != NULL;
}

PString SIPMessage::GetContentType()const
{
	ContentType h;
	if( !GetContentType( h ) )
		return PString::Empty();

	return h.GetHeaderBody();
}

BOOL SIPMessage::GetContentType(
								ContentType & h
								)const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseContentType() )
		return FALSE;

	if( !HasContentType() )
		return FALSE;

	h = *m_ContentType;

	return TRUE;
}

void SIPMessage::SetContentType(
								const PString & type
								)
{
	GlobalLock();
	ContentType cType( type );
	SetContentType( cType );
}

void SIPMessage::SetContentType(
								const ContentType & h
								)
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_ContentType != NULL )
		delete m_ContentType;
	m_ContentType = (ContentType*)h.Clone();
}

BOOL SIPMessage::HasContentType()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseContentType();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseCSeq()
{
	GlobalLock();
	// cs( *this );

	if( m_CSeq != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "cseq" )
		{
			if( m_CSeq == NULL )
			{
				m_CSeq = new CSeq( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_CSeq != NULL;
}

BOOL SIPMessage::GetCSeq(
						 CSeq & h
						 )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseCSeq() )
		return FALSE;

	if( !HasCSeq() )
		return FALSE;

	h = *m_CSeq;

	return TRUE;
}

PString SIPMessage::GetCSeqMethod()const
{
	GlobalLock();
	CSeq cseq;
	GetCSeq( cseq );
	return cseq.GetMethod();
}

DWORD SIPMessage::GetCSeqNumber()const
{
	GlobalLock();
	CSeq cseq;
	GetCSeq( cseq );
	return cseq.GetSequence();
}

void SIPMessage::SetCSeq(
						 const CSeq & h
						 )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_CSeq != NULL )
		delete m_CSeq;
	m_CSeq = (CSeq*)h.Clone();
}

BOOL SIPMessage::HasCSeq()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseCSeq();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseDate()
{
	GlobalLock();
	// cs( *this );

	if( m_Date != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "date" )
		{
			if( m_Date == NULL )
			{
				m_Date = new Date( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_Date != NULL;
}

BOOL SIPMessage::GetDate(
						 Date & h
						 )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseDate() )
		return FALSE;

	if( !HasDate() )
		return FALSE;

	h = *m_Date;

	return TRUE;
}

void SIPMessage::SetDate(
						 const Date & h
						 )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_Date != NULL )
		delete m_Date;
	m_Date = (Date*)h.Clone();
}

BOOL SIPMessage::HasDate()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseDate();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseErrorInfo()
{
	GlobalLock();
	// cs( *this );

	if( m_ErrorInfo != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "error-info" )
		{
			if( m_ErrorInfo == NULL )
			{
				m_ErrorInfo = new ErrorInfo( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_ErrorInfo != NULL;
}

BOOL SIPMessage::GetErrorInfo(
							  ErrorInfo & h
							  )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseErrorInfo() )
		return FALSE;

	if( !HasErrorInfo() )
		return FALSE;

	h = *m_ErrorInfo;

	return TRUE;
}

void SIPMessage::SetErrorInfo(
							  const ErrorInfo & h
							  )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_ErrorInfo != NULL )
		delete m_ErrorInfo;
	m_ErrorInfo = (ErrorInfo*)h.Clone();
}

BOOL SIPMessage::HasErrorInfo()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseErrorInfo();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseMIMEVersion()
{
	GlobalLock();
	// cs( *this );

	if( m_MIMEVersion != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "mime-version" )
		{
			if( m_MIMEVersion == NULL )
			{
				m_MIMEVersion = new MIMEVersion( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_MIMEVersion != NULL;
}

BOOL SIPMessage::GetMIMEVersion(
								MIMEVersion & h
								)const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseMIMEVersion() )
		return FALSE;

	if( !HasMIMEVersion() )
		return FALSE;

	h = *m_MIMEVersion;

	return TRUE;
}

void SIPMessage::SetMIMEVersion(
								const MIMEVersion & h
								)
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_MIMEVersion != NULL )
		delete m_MIMEVersion;
	m_MIMEVersion = (MIMEVersion*)h.Clone();
}

BOOL SIPMessage::HasMIMEVersion()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseMIMEVersion();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseOrganization()
{
	GlobalLock();
	// cs( *this );

	if( m_Organization != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "organization" )
		{
			if( m_Organization == NULL )
			{
				m_Organization = new Organization( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_Organization != NULL;
}

BOOL SIPMessage::GetOrganization(
								 Organization & h
								 )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseOrganization() )
		return FALSE;

	if( !HasOrganization() )
		return FALSE;

	h = *m_Organization;

	return TRUE;
}

void SIPMessage::SetOrganization(
								 const Organization & h
								 )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_Organization != NULL )
		delete m_Organization;
	m_Organization = (Organization*)h.Clone();
}

BOOL SIPMessage::HasOrganization()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseOrganization();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParsePriority()
{
	GlobalLock();
	// cs( *this );

	if( m_Priority != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "priority" )
		{
			if( m_Priority == NULL )
			{
				m_Priority = new Priority( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_Priority != NULL;
}

BOOL SIPMessage::GetPriority(
							 Priority & h
							 )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParsePriority() )
		return FALSE;

	if( !HasPriority() )
		return FALSE;

	h = *m_Priority;

	return TRUE;
}

void SIPMessage::SetPriority(
							 const Priority & h
							 )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_Priority != NULL )
		delete m_Priority;
	m_Priority = (Priority*)h.Clone();
}

BOOL SIPMessage::HasPriority()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParsePriority();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseRetryAfter()
{
	GlobalLock();
	// cs( *this );

	if( m_RetryAfter != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "retry-after" )
		{
			if( m_RetryAfter == NULL )
			{
				m_RetryAfter = new RetryAfter( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_RetryAfter != NULL;
}

BOOL SIPMessage::GetRetryAfter(
							   RetryAfter & h
							   )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseRetryAfter() )
		return FALSE;

	if( !HasRetryAfter() )
		return FALSE;

	h = *m_RetryAfter;

	return TRUE;
}

void SIPMessage::SetRetryAfter(
							   const RetryAfter & h
							   )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_RetryAfter != NULL )
		delete m_RetryAfter;
	m_RetryAfter = (RetryAfter*)h.Clone();
}

BOOL SIPMessage::HasRetryAfter()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseRetryAfter();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseServer()
{
	GlobalLock();
	// cs( *this );

	if( m_Server != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "server" )
		{
			if( m_Server == NULL )
			{
				m_Server = new Server( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_Server != NULL;
}

BOOL SIPMessage::GetServer(
						   Server & h
						   )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseServer() )
		return FALSE;

	if( !HasServer() )
		return FALSE;

	h = *m_Server;

	return TRUE;
}

void SIPMessage::SetServer(
						   const Server & h
						   )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_Server != NULL )
		delete m_Server;
	m_Server = (Server*)h.Clone();
}

BOOL SIPMessage::HasServer()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseServer();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseSubject()
{
	GlobalLock();
	// cs( *this );

	if( m_Subject != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "subject" )
		{
			if( m_Subject == NULL )
			{
				m_Subject = new Subject( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_Subject != NULL;
}

BOOL SIPMessage::GetSubject(
							Subject & h
							)const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseSubject() )
		return FALSE;

	if( !HasSubject() )
		return FALSE;

	h = *m_Subject;

	return TRUE;
}

void SIPMessage::SetSubject(
							const Subject & h
							)
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_Subject != NULL )
		delete m_Subject;
	m_Subject = (Subject*)h.Clone();
}

BOOL SIPMessage::HasSubject()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseSubject();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseUserAgent()
{
	GlobalLock();
	// cs( *this );

	if( m_UserAgent != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "user-agent" )
		{
			if( m_UserAgent == NULL )
			{
				m_UserAgent = new UserAgent( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_UserAgent != NULL;
}

BOOL SIPMessage::GetUserAgent(
							  UserAgent & h
							  )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseUserAgent() )
		return FALSE;

	if( !HasUserAgent() )
		return FALSE;

	h = *m_UserAgent;

	return TRUE;
}

void SIPMessage::SetUserAgent(
							  const UserAgent & h
							  )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_UserAgent != NULL )
		delete m_UserAgent;
	m_UserAgent = (UserAgent*)h.Clone();
}

BOOL SIPMessage::HasUserAgent()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseUserAgent();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseWarning()
{
	GlobalLock();
	// cs( *this );

	if( m_Warning != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "warning" )
		{
			if( m_Warning == NULL )
			{
				m_Warning = new Warning( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_Warning != NULL;
}

BOOL SIPMessage::GetWarning(
							Warning & h
							)const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseWarning() )
		return FALSE;

	if( !HasWarning() )
		return FALSE;

	h = *m_Warning;

	return TRUE;
}

void SIPMessage::SetWarning(
							const Warning & h
							)
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_Warning != NULL )
		delete m_Warning;
	m_Warning = (Warning*)h.Clone();
}

BOOL SIPMessage::HasWarning()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseWarning();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseFrom()
{
	GlobalLock();
	// cs( *this );

	if( m_From != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "from" )
		{
			if( m_From == NULL )
			{
				m_From = new From( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_From != NULL;
}

BOOL SIPMessage::GetFrom(
						 From & h
						 )const
{
	GlobalLock();
	// cs( *this );
	if( !(const_cast< SIPMessage * >( this ))->ParseFrom() )
		return FALSE;

	if( !HasFrom() )
		return FALSE;

	h = *m_From;

	return TRUE;
}

SIPURI SIPMessage::GetFromURI()const
{
	GlobalLock();
	// cs( *this );
	From from;

	if( !(const_cast< SIPMessage * >( this ))->GetFrom( from ) )
		return SIPURI();

	return from.GetURI();
}

PString SIPMessage::GetFromTag()const
{
	GlobalLock();
	// cs( *this );
	From from;

	if( !(const_cast< SIPMessage * >( this ))->GetFrom( from ) )
		return PString::Empty();

	PString tag;
	from.GetParameter( "tag", tag );

	/// some implementations dont have <> enclosures so try the uri if its there
	if( tag.IsEmpty() )
		from.GetURI().GetParameter( "tag", tag );

	return tag;
}

void SIPMessage::SetFromTag( 
							const PString & tag 
							)
{
	GlobalLock();
	// cs( *this );
	From from;
	GetFrom( from );
	from.SetParameter( "tag", tag );
	SetFrom( from );
}

void SIPMessage::SetFrom(
						 const From & h
						 )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_From != NULL )
		delete m_From;
	m_From = (From*)h.Clone();
}

BOOL SIPMessage::HasFrom()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseFrom();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseTo()
{
	GlobalLock();
	// cs( *this );

	if( m_To != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "to" )
		{
			if( m_To == NULL )
			{
				m_To = new To( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_To != NULL;
}

BOOL SIPMessage::GetTo(
					   To & h
					   )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseTo() )
		return FALSE;

	if( !HasTo() )
		return FALSE;

	h = *m_To;

	return TRUE;
}

SIPURI SIPMessage::GetToURI()const
{
	GlobalLock();
	To to;

	if( !GetTo( to ) )
		return SIPURI();

	return to.GetURI();
}

PString SIPMessage::GetToTag()const
{
	GlobalLock();
	To to;

	if( !GetTo( to ) )
		return PString::Empty();

	PString tag;
	to.GetParameter( "tag", tag );

	/// some implementations dont have <> enclosures so try the uri if its there
	if( tag.IsEmpty() )
		to.GetURI().GetParameter( "tag", tag );

	return tag;
}

void SIPMessage::SetToTag( 
						  const PString & tag 
						  )
{
	GlobalLock();
	// cs( *this );
	To to;
	GetTo( to );
	to.SetParameter( "tag", tag );
	SetTo( to );
}

void SIPMessage::SetTo(
					   const To & h
					   )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_To != NULL )
		delete m_To;
	m_To = (To*)h.Clone();
}

BOOL SIPMessage::HasTo()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseTo();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseReplyTo()
{
	GlobalLock();
	// cs( *this );

	if( m_ReplyTo != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "reply-to" )
		{
			if( m_ReplyTo == NULL )
			{
				m_ReplyTo = new ReplyTo( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_ReplyTo != NULL;
}

BOOL SIPMessage::GetReplyTo(
							ReplyTo & h
							)const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseReplyTo() )
		return FALSE;

	if( !HasReplyTo() )
		return FALSE;

	h = *m_ReplyTo;

	return TRUE;
}

void SIPMessage::SetReplyTo(
							const ReplyTo & h
							)
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_ReplyTo != NULL )
		delete m_ReplyTo;
	m_ReplyTo = (ReplyTo*)h.Clone();
}

BOOL SIPMessage::HasReplyTo()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseReplyTo();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseExpires()
{
	GlobalLock();
	// cs( *this );

	if( m_Expires != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "expires" )
		{
			if( m_Expires == NULL )
			{
				m_Expires = new Expires( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_Expires != NULL;
}

BOOL SIPMessage::GetExpires(
							Expires & h
							)const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseExpires() )
		return FALSE;

	if( !HasExpires() )
		return FALSE;

	h = *m_Expires;

	return TRUE;
}

void SIPMessage::SetExpires(
							const Expires & h
							)
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_Expires != NULL )
		delete m_Expires;
	m_Expires = (Expires*)h.Clone();
}

BOOL SIPMessage::HasExpires()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseExpires();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseMaxForwards()
{
	GlobalLock();
	// cs( *this );

	if( m_MaxForwards != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "max-forwards" )
		{
			if( m_MaxForwards == NULL )
			{
				m_MaxForwards = new MaxForwards( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_MaxForwards != NULL;
}

BOOL SIPMessage::GetMaxForwards(
								MaxForwards & h
								)const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseMaxForwards() )
		return FALSE;

	if( !HasMaxForwards() )
		return FALSE;

	h = *m_MaxForwards;

	return TRUE;
}

void SIPMessage::SetMaxForwards( 
								WORD maxForwards
								)
{
	GlobalLock();
	MaxForwards maxF;
	maxF.SetHeaderBody( PString( maxForwards ) );
	SetMaxForwards( maxF );
}

void SIPMessage::SetMaxForwards(
								const MaxForwards & h
								)
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_MaxForwards != NULL )
		delete m_MaxForwards;
	m_MaxForwards = (MaxForwards*)h.Clone();
}

BOOL SIPMessage::HasMaxForwards()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseMaxForwards();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseMinExpires()
{
	GlobalLock();
	// cs( *this );

	if( m_MinExpires != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "min-expires" )
		{
			if( m_MinExpires == NULL )
			{
				m_MinExpires = new MinExpires( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_MinExpires != NULL;
}

BOOL SIPMessage::GetMinExpires(
							   MinExpires & h
							   )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseMinExpires() )
		return FALSE;

	if( !HasMinExpires() )
		return FALSE;

	h = *m_MinExpires;

	return TRUE;
}

void SIPMessage::SetMinExpires(
							   const MinExpires & h
							   )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_MinExpires != NULL )
		delete m_MinExpires;
	m_MinExpires = (MinExpires*)h.Clone();
}

BOOL SIPMessage::HasMinExpires()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseMinExpires();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseTimeStamp()
{
	GlobalLock();
	// cs( *this );

	if( m_TimeStamp != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "time-stamp" )
		{
			if( m_TimeStamp == NULL )
			{
				m_TimeStamp = new TimeStamp( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_TimeStamp != NULL;
}

BOOL SIPMessage::GetTimeStamp(
							  TimeStamp & h
							  )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseTimeStamp() )
		return FALSE;

	if( !HasTimeStamp() )
		return FALSE;

	h = *m_TimeStamp;

	return TRUE;
}

void SIPMessage::SetTimeStamp(
							  const TimeStamp & h
							  )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_TimeStamp != NULL )
		delete m_TimeStamp;
	m_TimeStamp = (TimeStamp*)h.Clone();
}

BOOL SIPMessage::HasTimeStamp()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseTimeStamp();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseAuthenticationInfo()
{
	GlobalLock();
	// cs( *this );

	if( m_AuthenticationInfo != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "authentication-info" )
		{
			if( m_AuthenticationInfo == NULL )
			{
				m_AuthenticationInfo = new AuthenticationInfo( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_AuthenticationInfo != NULL;
}

BOOL SIPMessage::GetAuthenticationInfo(
									   AuthenticationInfo & h
									   )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseAuthenticationInfo() )
		return FALSE;

	if( !HasAuthenticationInfo() )
		return FALSE;

	h = *m_AuthenticationInfo;

	return TRUE;
}

void SIPMessage::SetAuthenticationInfo(
									   const AuthenticationInfo & h
									   )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_AuthenticationInfo != NULL )
		delete m_AuthenticationInfo;
	m_AuthenticationInfo = (AuthenticationInfo*)h.Clone();
}

BOOL SIPMessage::HasAuthenticationInfo()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseAuthenticationInfo();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseProxyAuthenticate()
{
	GlobalLock();
	// cs( *this );

	if( m_ProxyAuthenticate != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "proxy-authenticate" )
		{
			if( m_ProxyAuthenticate == NULL )
			{
				m_ProxyAuthenticate = new ProxyAuthenticate( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_ProxyAuthenticate != NULL;
}

BOOL SIPMessage::GetProxyAuthenticate(
									  ProxyAuthenticate & h
									  )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseProxyAuthenticate() )
		return FALSE;

	if( !HasProxyAuthenticate() )
		return FALSE;

	h = *m_ProxyAuthenticate;

	return TRUE;
}

void SIPMessage::SetProxyAuthenticate(
									  const ProxyAuthenticate & h
									  )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_ProxyAuthenticate != NULL )
		delete m_ProxyAuthenticate;
	m_ProxyAuthenticate = (ProxyAuthenticate*)h.Clone();
}

BOOL SIPMessage::HasProxyAuthenticate()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseProxyAuthenticate();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseProxyAuthorization()
{
	GlobalLock();
	// cs( *this );

	if( m_ProxyAuthorization != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "proxy-authorization" )
		{
			if( m_ProxyAuthorization == NULL )
			{
				m_ProxyAuthorization = new ProxyAuthorization( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_ProxyAuthorization != NULL;
}

BOOL SIPMessage::GetProxyAuthorization(
									   ProxyAuthorization & h
									   )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseProxyAuthorization() )
		return FALSE;

	if( !HasProxyAuthorization() )
		return FALSE;

	h = *m_ProxyAuthorization;

	return TRUE;
}

void SIPMessage::SetProxyAuthorization(
									   const ProxyAuthorization & h
									   )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_ProxyAuthorization != NULL )
		delete m_ProxyAuthorization;
	m_ProxyAuthorization = (ProxyAuthorization*)h.Clone();
}

BOOL SIPMessage::HasProxyAuthorization()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseProxyAuthorization();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseWWWAuthenticate()
{
	GlobalLock();
	// cs( *this );

	if( m_WWWAuthenticate != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "www-authenticate" )
		{
			if( m_WWWAuthenticate == NULL )
			{
				m_WWWAuthenticate = new WWWAuthenticate( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_WWWAuthenticate != NULL;
}

BOOL SIPMessage::GetWWWAuthenticate(
									WWWAuthenticate & h
									)const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseWWWAuthenticate() )
		return FALSE;

	if( !HasWWWAuthenticate() )
		return FALSE;

	h = *m_WWWAuthenticate;

	return TRUE;
}

void SIPMessage::SetWWWAuthenticate(
									const WWWAuthenticate & h
									)
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_WWWAuthenticate != NULL )
		delete m_WWWAuthenticate;
	m_WWWAuthenticate = (WWWAuthenticate*)h.Clone();
}

BOOL SIPMessage::HasWWWAuthenticate()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseWWWAuthenticate();
}


////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseAcceptList()
{
	GlobalLock();
	// cs( *this );

	if( m_AcceptList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "accept" )
		{
			if( m_AcceptList == NULL )
			{
				m_AcceptList = new AcceptList();
			}

			Accept nh( h );

			m_AcceptList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_AcceptList != NULL;
}

BOOL SIPMessage::AppendAccept(
							  const PString & token,
							  PINDEX idx
							  )
{
	GlobalLock();
	ParseAcceptList();
	return FALSE;

	MakeUnique();

	if( m_AcceptList == NULL )
	{
		m_AcceptList = new AcceptList();
	}

	return m_AcceptList->Append( token, idx );
}

BOOL SIPMessage::AppendAccept(
							  const Accept & header
							  )
{
	GlobalLock();
	ParseAcceptList();

	MakeUnique();

	if( m_AcceptList == NULL )
	{
		m_AcceptList = new AcceptList();
	}

	m_AcceptList->Append( header );

	return TRUE;
}

PINDEX SIPMessage::GetAcceptSize()const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseAcceptList();
	return m_AcceptList->GetSize();
}

Accept & SIPMessage::GetAcceptAt(
								 PINDEX idx
								 )const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseAcceptList();
	return (Accept&)m_AcceptList->GetAt( idx );
}

PString SIPMessage::GetAcceptTokenAt(
									 PINDEX idx1,
									 PINDEX idx2
									 )const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseAcceptList();

	if( m_AcceptList == NULL )
	{
		return PString::Empty();
	}

	return m_AcceptList->GetTokenAt( idx1, idx2 );
}

BOOL SIPMessage::RemoveAllAccept()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_AcceptList != NULL )
	{
		delete m_AcceptList;
		m_AcceptList = NULL;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseAcceptEncodingList()
{
	GlobalLock();
	// cs( *this );

	if( m_AcceptEncodingList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "accept-encoding" )
		{
			if( m_AcceptEncodingList == NULL )
			{
				m_AcceptEncodingList = new AcceptEncodingList();
			}

			AcceptEncoding nh( h );

			m_AcceptEncodingList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_AcceptEncodingList != NULL;
}

BOOL SIPMessage::AppendAcceptEncoding(
									  const PString & token,
									  PINDEX idx
									  )
{
	GlobalLock();
	ParseAcceptEncodingList();
	return FALSE;

	MakeUnique();

	if( m_AcceptEncodingList == NULL )
	{
		m_AcceptEncodingList = new AcceptEncodingList();
	}

	return m_AcceptEncodingList->Append( token, idx );
}

BOOL SIPMessage::AppendAcceptEncoding(
									  const AcceptEncoding & header
									  )
{
	GlobalLock();
	ParseAcceptEncodingList();

	MakeUnique();

	if( m_AcceptEncodingList == NULL )
	{
		m_AcceptEncodingList = new AcceptEncodingList();
	}

	m_AcceptEncodingList->Append( header );

	return TRUE;
}

AcceptEncoding & SIPMessage::GetAcceptEncodingAt(
	PINDEX idx
	)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseAcceptEncodingList();
	return (AcceptEncoding&)m_AcceptEncodingList->GetAt( idx );
}

PString SIPMessage::GetAcceptEncodingTokenAt(
	PINDEX idx1,
	PINDEX idx2
	)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseAcceptEncodingList();

	if( m_AcceptEncodingList == NULL )
	{
		return PString::Empty();
	}

	return m_AcceptEncodingList->GetTokenAt( idx1, idx2 );
}

BOOL SIPMessage::RemoveAllAcceptEncoding()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_AcceptEncodingList != NULL )
	{
		delete m_AcceptEncodingList;
		m_AcceptEncodingList = NULL;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseAcceptLanguageList()
{
	GlobalLock();
	// cs( *this );

	if( m_AcceptLanguageList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "accept-language" )
		{
			if( m_AcceptLanguageList == NULL )
			{
				m_AcceptLanguageList = new AcceptLanguageList();
			}

			AcceptLanguage nh( h );

			m_AcceptLanguageList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_AcceptLanguageList != NULL;
}

BOOL SIPMessage::AppendAcceptLanguage(
									  const PString & token,
									  PINDEX idx
									  )
{
	GlobalLock();
	ParseAcceptLanguageList();
	return FALSE;

	MakeUnique();

	if( m_AcceptLanguageList == NULL )
	{
		m_AcceptLanguageList = new AcceptLanguageList();
	}

	return m_AcceptLanguageList->Append( token, idx );
}

BOOL SIPMessage::AppendAcceptLanguage(
									  const AcceptLanguage & header
									  )
{
	GlobalLock();
	ParseAcceptLanguageList();

	MakeUnique();

	if( m_AcceptLanguageList == NULL )
	{
		m_AcceptLanguageList = new AcceptLanguageList();
	}

	m_AcceptLanguageList->Append( header );

	return TRUE;
}

AcceptLanguage & SIPMessage::GetAcceptLanguageAt(
	PINDEX idx
	)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseAcceptLanguageList();
	return (AcceptLanguage&)m_AcceptLanguageList->GetAt( idx );
}

PString SIPMessage::GetAcceptLanguageTokenAt(
	PINDEX idx1,
	PINDEX idx2
	)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseAcceptLanguageList();

	if( m_AcceptLanguageList == NULL )
	{
		return PString::Empty();
	}

	return m_AcceptLanguageList->GetTokenAt( idx1, idx2 );
}

BOOL SIPMessage::RemoveAllAcceptLanguage()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_AcceptLanguageList != NULL )
	{
		delete m_AcceptLanguageList;
		m_AcceptLanguageList = NULL;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseAllowList()
{
	GlobalLock();
	// cs( *this );

	if( m_AllowList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "allow" )
		{
			if( m_AllowList == NULL )
			{
				m_AllowList = new AllowList();
			}

			Allow nh( h );

			m_AllowList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_AllowList != NULL;
}

BOOL SIPMessage::AppendAllow(
							 const PString & token,
							 PINDEX idx
							 )
{
	GlobalLock();
	ParseAllowList();
	return FALSE;

	MakeUnique();

	if( m_AllowList == NULL )
	{
		m_AllowList = new AllowList();
	}

	return m_AllowList->Append( token, idx );
}

BOOL SIPMessage::AppendAllow(
							 const Allow & header
							 )
{
	GlobalLock();
	ParseAllowList();

	MakeUnique();

	if( m_AllowList == NULL )
	{
		m_AllowList = new AllowList();
	}

	m_AllowList->Append( header );

	return TRUE;
}

Allow & SIPMessage::GetAllowAt(
							   PINDEX idx
							   )const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseAllowList();
	return (Allow&)m_AllowList->GetAt( idx );
}

PString SIPMessage::GetAllowTokenAt(
									PINDEX idx1,
									PINDEX idx2
									)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseAllowList();

	if( m_AllowList == NULL )
	{
		return PString::Empty();
	}

	return m_AllowList->GetTokenAt( idx1, idx2 );
}

BOOL SIPMessage::RemoveAllAllow()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_AllowList != NULL )
	{
		delete m_AllowList;
		m_AllowList = NULL;
	}

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseContentEncodingList()
{
	GlobalLock();
	// cs( *this );

	if( m_ContentEncodingList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "content-encoding" )
		{
			if( m_ContentEncodingList == NULL )
			{
				m_ContentEncodingList = new ContentEncodingList();
			}

			ContentEncoding nh( h );

			m_ContentEncodingList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_ContentEncodingList != NULL;
}

BOOL SIPMessage::AppendContentEncoding(
									   const PString & token,
									   PINDEX idx
									   )
{
	GlobalLock();
	ParseContentEncodingList();
	return FALSE;

	MakeUnique();

	if( m_ContentEncodingList == NULL )
	{
		m_ContentEncodingList = new ContentEncodingList();
	}

	return m_ContentEncodingList->Append( token, idx );
}

BOOL SIPMessage::AppendContentEncoding(
									   const ContentEncoding & header
									   )
{
	GlobalLock();
	ParseContentEncodingList();

	MakeUnique();

	if( m_ContentEncodingList == NULL )
	{
		m_ContentEncodingList = new ContentEncodingList();
	}

	m_ContentEncodingList->Append( header );

	return TRUE;
}


ContentEncoding & SIPMessage::GetContentEncodingAt(
	PINDEX idx
	)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseContentEncodingList();
	return (ContentEncoding&)m_ContentEncodingList->GetAt( idx );
}

PString SIPMessage::GetContentEncodingTokenAt(
	PINDEX idx1,
	PINDEX idx2
	)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseContentEncodingList();

	if( m_ContentEncodingList == NULL )
	{
		return PString::Empty();
	}

	return m_ContentEncodingList->GetTokenAt( idx1, idx2 );
}

BOOL SIPMessage::RemoveAllContentEncoding()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_ContentEncodingList != NULL )
	{
		delete m_ContentEncodingList;
		m_ContentEncodingList = NULL;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseContactList()
{
	GlobalLock();
	// cs( *this );

	if( m_ContactList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "contact" )
		{
			if( m_ContactList == NULL )
			{
				m_ContactList = new ContactList();
			}

			Contact nh( h );

			m_ContactList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_ContactList != NULL;
}

BOOL SIPMessage::AppendContact(
							   const PString & token,
							   PINDEX idx
							   )
{
	GlobalLock();
	ParseContactList();
	return FALSE;

	MakeUnique();

	if( m_ContactList == NULL )
	{
		m_ContactList = new ContactList();
	}

	return m_ContactList->Append( token, idx );
}

BOOL SIPMessage::AppendContact(
							   const ContactURI & contactURI
							   )
{
	GlobalLock();
	Contact contact;
	contact.AddURI( contactURI );
	return AppendContact( contact );
}

BOOL SIPMessage::AppendContact(
							   const Contact & header
							   )
{
	GlobalLock();
	ParseContactList();

	MakeUnique();

	if( m_ContactList == NULL )
	{
		m_ContactList = new ContactList();
	}

	m_ContactList->Append( header );

	return TRUE;
}

SIPURI SIPMessage::GetTopContactURI()const
{
	GlobalLock();
	Contact contact = GetContactAt( 0 );
	ContactURI curi;
	contact.GetURI( curi );
	return curi.GetURI();
}

SIPURI SIPMessage::GetContactTopURI()const
{
	GlobalLock();
	Contact & c = GetContactAt( 0 );
	ContactURI curi;
	c.GetURI( curi, 0 );
	return curi.GetURI();
}

Contact & SIPMessage::GetContactAt(
								   PINDEX idx
								   )const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseContactList();
	return (Contact&)m_ContactList->GetAt( idx );
}

PINDEX SIPMessage::GetContactSize()const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseContactList();

	if( m_ContactList == NULL )
		return 0;

	SIPHeaderC::Collection & contactList = m_ContactList->GetCollection();
	return contactList.GetSize();
}

PString SIPMessage::GetContactTokenAt(
									  PINDEX idx1,
									  PINDEX idx2
									  )const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseContactList();

	if( m_ContactList == NULL )
	{
		return PString::Empty();
	}

	return m_ContactList->GetTokenAt( idx1, idx2 );
}

BOOL SIPMessage::RemoveAllContact()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_ContactList != NULL )
	{
		delete m_ContactList;
		m_ContactList = NULL;
	}

	return TRUE;
}

BOOL SIPMessage::SetContactList(
								const ContactList & contactList
								)
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_ContactList != NULL )
	{
		delete m_ContactList;
		m_ContactList = NULL;
	}

	m_ContactList = new ContactList( contactList );

	return TRUE;
}

ContactList SIPMessage::GetContactList()const
{
	GlobalLock();
	// cs( *this );
	(const_cast< SIPMessage * >( this ))->ParseContactList();

	ContactList contactList;

	if( m_ContactList != NULL )
	{
		contactList = ContactList( *m_ContactList );
		contactList.MakeUnique();
	}

	return contactList;
}


///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseInReplyToList()
{
	GlobalLock();
	// cs( *this );

	if( m_InReplyToList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "in-reply-to" )
		{
			if( m_InReplyToList == NULL )
			{
				m_InReplyToList = new InReplyToList();
			}

			InReplyTo nh( h );

			m_InReplyToList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_InReplyToList != NULL;
}

BOOL SIPMessage::AppendInReplyTo(
								 const PString & token,
								 PINDEX idx
								 )
{
	GlobalLock();
	ParseInReplyToList();
	return FALSE;

	MakeUnique();

	if( m_InReplyToList == NULL )
	{
		m_InReplyToList = new InReplyToList();
	}

	return m_InReplyToList->Append( token, idx );
}

BOOL SIPMessage::AppendInReplyTo(
								 const InReplyTo & header
								 )
{
	GlobalLock();
	ParseInReplyToList();

	MakeUnique();

	if( m_InReplyToList == NULL )
	{
		m_InReplyToList = new InReplyToList();
	}

	m_InReplyToList->Append( header );

	return TRUE;
}

InReplyTo & SIPMessage::GetInReplyToAt(
									   PINDEX idx
									   )const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseInReplyToList();
	return (InReplyTo&)m_InReplyToList->GetAt( idx );
}

PString SIPMessage::GetInReplyToTokenAt(
										PINDEX idx1,
										PINDEX idx2
										)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseInReplyToList();

	if( m_InReplyToList == NULL )
	{
		return PString::Empty();
	}

	return m_InReplyToList->GetTokenAt( idx1, idx2 );
}

BOOL SIPMessage::RemoveAllInReplyTo()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_InReplyToList != NULL )
	{
		delete m_InReplyToList;
		m_InReplyToList = NULL;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseProxyRequireList()
{
	GlobalLock();
	// cs( *this );

	if( m_ProxyRequireList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "proxy-require" )
		{
			if( m_ProxyRequireList == NULL )
			{
				m_ProxyRequireList = new ProxyRequireList();
			}

			ProxyRequire nh( h );

			m_ProxyRequireList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_ProxyRequireList != NULL;
}

BOOL SIPMessage::AppendProxyRequire(
									const PString & token,
									PINDEX idx
									)
{
	GlobalLock();
	ParseProxyRequireList();
	return FALSE;

	MakeUnique();

	if( m_ProxyRequireList == NULL )
	{
		m_ProxyRequireList = new ProxyRequireList();
	}

	return m_ProxyRequireList->Append( token, idx );
}

BOOL SIPMessage::AppendProxyRequire(
									const ProxyRequire & header
									)
{
	GlobalLock();
	ParseProxyRequireList();

	MakeUnique();

	if( m_ProxyRequireList == NULL )
	{
		m_ProxyRequireList = new ProxyRequireList();
	}

	m_ProxyRequireList->Append( header );

	return TRUE;
}

ProxyRequire & SIPMessage::GetProxyRequireAt(
	PINDEX idx
	)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseProxyRequireList();
	return (ProxyRequire&)m_ProxyRequireList->GetAt( idx );
}

PString SIPMessage::GetProxyRequireTokenAt(
	PINDEX idx1,
	PINDEX idx2
	)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseProxyRequireList();

	if( m_ProxyRequireList == NULL )
	{
		return PString::Empty();
	}

	return m_ProxyRequireList->GetTokenAt( idx1, idx2 );
}

BOOL SIPMessage::RemoveAllProxyRequire()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_ProxyRequireList != NULL )
	{
		delete m_ProxyRequireList;
		m_ProxyRequireList = NULL;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseRequireList()
{
	GlobalLock();
	// cs( *this );

	if( m_RequireList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "require" )
		{
			if( m_RequireList == NULL )
			{
				m_RequireList = new RequireList();
			}

			Require nh( h );

			m_RequireList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_RequireList != NULL;
}

BOOL SIPMessage::AppendRequire(
							   const PString & token,
							   PINDEX idx
							   )
{
	GlobalLock();
	ParseRequireList();
	return FALSE;

	MakeUnique();

	if( m_RequireList == NULL )
	{
		m_RequireList = new RequireList();
	}

	return m_RequireList->Append( token, idx );
}

BOOL SIPMessage::AppendRequire(
							   const Require & header
							   )
{
	GlobalLock();
	ParseRequireList();

	MakeUnique();

	if( m_RequireList == NULL )
	{
		m_RequireList = new RequireList();
	}

	m_RequireList->Append( header );

	return TRUE;
}

Require & SIPMessage::GetRequireAt(
								   PINDEX idx
								   )const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseRequireList();
	return (Require&)m_RequireList->GetAt( idx );
}

PString SIPMessage::GetRequireTokenAt(
									  PINDEX idx1,
									  PINDEX idx2
									  )const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseRequireList();

	if( m_RequireList == NULL )
	{
		return PString::Empty();
	}

	return m_RequireList->GetTokenAt( idx1, idx2 );
}

BOOL SIPMessage::RemoveAllRequire()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_RequireList != NULL )
	{
		delete m_RequireList;
		m_RequireList = NULL;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseSupportedList()
{
	GlobalLock();
	// cs( *this );

	if( m_SupportedList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "supported" )
		{
			if( m_SupportedList == NULL )
			{
				m_SupportedList = new SupportedList();
			}

			Supported nh( h );

			m_SupportedList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_SupportedList != NULL;
}

BOOL SIPMessage::AppendSupported(
								 const PString & token,
								 PINDEX idx
								 )
{
	GlobalLock();
	ParseSupportedList();
	return FALSE;

	MakeUnique();

	if( m_SupportedList == NULL )
	{
		m_SupportedList = new SupportedList();
	}

	return m_SupportedList->Append( token, idx );
}

BOOL SIPMessage::AppendSupported(
								 const Supported & header
								 )
{
	GlobalLock();
	ParseSupportedList();

	MakeUnique();

	if( m_SupportedList == NULL )
	{
		m_SupportedList = new SupportedList();
	}

	m_SupportedList->Append( header );

	return TRUE;
}

PStringArray SIPMessage::GetSupported()const
{
	GlobalLock();

	(const_cast< SIPMessage * >( this ))->ParseSupportedList();

	PStringArray supportedList;

	if( m_SupportedList == NULL ) 
		return supportedList; // return empty array

	for( PINDEX i = 0; i < m_SupportedList->GetSize(); i++ )
	{
		Supported & s = GetSupportedAt( i );
		for( PINDEX j = 0; j < s.GetSize(); j++ )
		{
			supportedList.AppendString( s.GetEntry( j ) );
		}
	}

	return supportedList;
}

BOOL SIPMessage::IsSupportedExtension( 
									  const PString & extensionName 
									  )const
{
	GlobalLock();

	PStringArray supported = GetSupported();
	if( supported.GetSize() > 0 )
	{
		for( PINDEX i = 0; i < supported.GetSize(); i++ )
		{
			if( supported[i] *= extensionName )
				return TRUE;
		}
	}

	return FALSE;
}

Supported & SIPMessage::GetSupportedAt(
									   PINDEX idx
									   )const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseSupportedList();
	return (Supported&)m_SupportedList->GetAt( idx );
}

PString SIPMessage::GetSupportedTokenAt(
										PINDEX idx1,
										PINDEX idx2
										)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseSupportedList();

	if( m_SupportedList == NULL )
	{
		return PString::Empty();
	}

	return m_SupportedList->GetTokenAt( idx1, idx2 );
}

BOOL SIPMessage::RemoveAllSupported()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_SupportedList != NULL )
	{
		delete m_SupportedList;
		m_SupportedList = NULL;
	}

	return TRUE;
}





///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseUnsupportedList()
{
	GlobalLock();
	// cs( *this );

	if( m_UnsupportedList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "unsupported" )
		{
			if( m_UnsupportedList == NULL )
			{
				m_UnsupportedList = new UnsupportedList();
			}

			Unsupported nh( h );

			m_UnsupportedList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_UnsupportedList != NULL;
}

BOOL SIPMessage::AppendUnsupported(
								   const PString & token,
								   PINDEX idx
								   )
{
	GlobalLock();
	ParseUnsupportedList();
	return FALSE;

	MakeUnique();

	if( m_UnsupportedList == NULL )
	{
		m_UnsupportedList = new UnsupportedList();
	}

	return m_UnsupportedList->Append( token, idx );
}

BOOL SIPMessage::AppendUnsupported(
								   const Unsupported & header
								   )
{
	GlobalLock();
	ParseUnsupportedList();

	MakeUnique();

	if( m_UnsupportedList == NULL )
	{
		m_UnsupportedList = new UnsupportedList();
	}

	m_UnsupportedList->Append( header );

	return TRUE;
}

Unsupported & SIPMessage::GetUnsupportedAt(
	PINDEX idx
	)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseUnsupportedList();
	return (Unsupported&)m_UnsupportedList->GetAt( idx );
}

PString SIPMessage::GetUnsupportedTokenAt(
	PINDEX idx1,
	PINDEX idx2
	)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseUnsupportedList();

	if( m_UnsupportedList == NULL )
	{
		return PString::Empty();
	}

	return m_UnsupportedList->GetTokenAt( idx1, idx2 );
}

BOOL SIPMessage::RemoveAllUnsupported()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_UnsupportedList != NULL )
	{
		delete m_UnsupportedList;
		m_UnsupportedList = NULL;
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseRouteList()
{
	GlobalLock();
	// cs( *this );

	if( m_RouteList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = msg.GetSize() - 1; i >= 0; i-- )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "route" )
		{
			if( m_RouteList == NULL )
			{
				m_RouteList = new RouteList();
			}

			Route nh( h );

			m_RouteList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_RouteList != NULL;
}

///////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseRecordRouteList()
{
	GlobalLock();
	// cs( *this );

	if( m_RecordRouteList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = msg.GetSize() - 1; i >= 0; i-- )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "record-route" )
		{
			if( m_RecordRouteList == NULL )
			{
				m_RecordRouteList = new RecordRouteList();
			}

			RecordRoute nh( h );

			m_RecordRouteList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_RecordRouteList != NULL;
}

////////////////////////////////////////////////////

BOOL SIPMessage::ParsePathList()
{
	GlobalLock();
	// cs( *this );

	if( m_PathList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "record-route" )
		{
			if( m_PathList == NULL )
			{
				m_PathList = new PathList();
			}

			Path nh( h );

			m_PathList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_PathList != NULL;
}

////////////////////////////////////////////////////
BOOL SIPMessage::CreateResponse(
								SIPMessage & response,
								WORD statusCode,
								const PString & reasonPhrase
								)const
{
	GlobalLock();
	PAssert( IsRequest(), "Attempting to create a response to a response!!!" );

	StatusLine statusLine( statusCode );
	if( !reasonPhrase.IsEmpty() )
		statusLine.SetReasonPhrase( reasonPhrase );
	StartLine startLine( statusLine );
	response.SetStartLine( startLine );

	To to;
	if( !GetTo(to) )
		return FALSE;
	if( statusCode != 100 )
	{
		if( GetToTag().IsEmpty() )
		{
			/// only set the tag parameter if its empty
			PString tag = ParserTools::GenTagParameter();
			to.AddParameter( "tag", tag );
		}
	}
	response.SetTo( to );


	/** Clone the from header */
	From from;
	if( !GetFrom( from ) )
		return FALSE;
	response.SetFrom( from );

	/** Clone the CSEQ */
	CSeq cseq;
	if( !GetCSeq( cseq ) )
		return FALSE;
	response.SetCSeq( cseq );

	/** Clone the call ID */
	CallId callId;
	if( !GetCallId( callId ) )
		return FALSE;
	response.SetCallId( callId );

	/*
	Via via;
	if( GetViaSize() == 0 )
	return FALSE;
	GetViaAt( 0, via );

	response.AppendVia( via );
	*/

	ViaList viaList = GetViaList();
	if( viaList.GetSize() == 0 )
		return FALSE;
	response.SetViaList( viaList );

	/// clone the record route if there is any
	RecordRouteList recordRouteList = GetRecordRouteList();
	if( recordRouteList.GetSize() > 0 )
		response.SetRecordRouteList( recordRouteList );

	response.SetContentLength( 0 );

	response.SetXORHash( IsXORHashSet() );

	response.Finalize();

	return TRUE;
}


PString SIPMessage::GetMethodString( Methods meth )
{
	static const char * methods[NumMethods] =
	{
		"ACK",
			"BYE",
			"CANCEL",
			"INFO",
			"INVITE",
			"MESSAGE",
			"NOTIFY",
			"OPTIONS",
			"PRACK",
			"PUBLISH",
			"REFER",
			"REGISTER",
			"SUBSCRIBE",
			"UPDATE"
	};

	return methods[meth];
}

BOOL SIPMessage::SetSendAddress(
								const SIPURI & address
								)
{
	GlobalLock(); 
	PString port = "5060";
	if( !address.GetPort().IsEmpty() )
		port = address.GetPort();

	PString proto;
	if( !address.GetParameter( "transport", proto ) )
		proto = "udp";

	SetInternalHeader( "SENDADDR", address.GetHost() );
	SetInternalHeader( "SENDPORT", port );
	SetInternalHeader( "SENDPROTO", proto );

	return TRUE;
}


/////////////////////////////////////////////

/////////////////////////
BOOL SIPMessage::ParseReferTo()
{
	GlobalLock();
	// cs( *this );

	if( m_ReferTo != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "Refer-To" )
		{
			if( m_ReferTo == NULL )
			{
				m_ReferTo = new ReferTo( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_ReferTo != NULL;
}

BOOL SIPMessage::GetReferTo(
							ReferTo & h
							)const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseReferTo() )
		return FALSE;

	if( !HasReferTo() )
		return FALSE;

	h = *m_ReferTo;

	return TRUE;
}

void SIPMessage::SetReferTo(
							const ReferTo & h
							)
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_ReferTo != NULL )
		delete m_ReferTo;
	m_ReferTo = (ReferTo*)h.Clone();
}

BOOL SIPMessage::HasReferTo()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseReferTo();
}

//////////////////////////////////////////////////////////////////////////////

BOOL SIPMessage::ParseEvent()
{
	GlobalLock();
	// cs( *this );

	if( m_Event != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "event" )
		{
			if( m_Event == NULL )
			{
				m_Event = new Event( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_Event != NULL;
}

BOOL SIPMessage::GetEvent(
						  Event & h
						  )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseEvent() )
		return FALSE;

	if( !HasEvent() )
		return FALSE;

	h = *m_Event;

	return TRUE;
}

void SIPMessage::SetEvent(
						  const Event & h
						  )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_Event != NULL )
		delete m_Event;
	m_Event = (Event*)h.Clone();
}

BOOL SIPMessage::HasEvent()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseEvent();
}

//////////////////////////////////////////////////////////////////

BOOL SIPMessage::GetSIPETag(
							SIPETag & h
							)const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseSIPETag() )
		return FALSE;

	if( !HasSIPETag() )
		return FALSE;

	h = *m_SIPETag;

	return TRUE;
}

PString SIPMessage::GetSIPETag()const
{
	SIPETag h;
	if( !GetSIPETag( h ) )
		return PString::Empty();

	return h.GetHeaderBody();
}

void SIPMessage::SetSIPETag(
							const SIPETag & h
							)
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_SIPETag != NULL )
		delete m_SIPETag;
	m_SIPETag = (SIPETag*)h.Clone();
}

void SIPMessage::SetSIPETag(
							const PString & etag
							)
{
	SIPETag h( etag );
	SetSIPETag( h );
}

BOOL SIPMessage::HasSIPETag()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseSIPETag();
}
///////////////////////////////////////////////////////////////////////////
BOOL SIPMessage::GetSIPIfMatch(
							   SIPIfMatch & h
							   )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseSIPIfMatch() )
		return FALSE;

	if( !HasSIPIfMatch() )
		return FALSE;

	h = *m_SIPIfMatch;

	return TRUE;
}

PString SIPMessage::GetSIPIfMatch()const
{
	SIPIfMatch h;
	if( !GetSIPIfMatch( h ) )
		return PString::Empty();

	return h.GetHeaderBody();
}

void SIPMessage::SetSIPIfMatch(
							   const SIPIfMatch & h
							   )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_SIPIfMatch != NULL )
		delete m_SIPIfMatch;
	m_SIPIfMatch = (SIPIfMatch*)h.Clone();
}

void SIPMessage::SetSIPIfMatch(
							   const PString & etag
							   )
{
	SIPIfMatch h( etag );
	SetSIPIfMatch( h );
}

BOOL SIPMessage::HasSIPIfMatch()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseSIPIfMatch();
}
///////////////////////////////////////////////////////////////////////////
BOOL SIPMessage::ParseAllowEventsList()
{
	GlobalLock();
	// cs( *this );

	if( m_AllowEventsList != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname == "allow-events" )
		{
			if( m_AllowEventsList == NULL )
			{
				m_AllowEventsList = new AllowEventsList();
			}

			AllowEvents nh( h );

			m_AllowEventsList->Append( nh );
			msg.RemoveAt( i );
			break;
		}
	}

	return m_AcceptList != NULL;
}

BOOL SIPMessage::AppendAllowEvents(
								   const PString & token,
								   PINDEX idx
								   )
{
	GlobalLock();
	ParseAllowEventsList();
	return FALSE;

	MakeUnique();

	if( m_AllowEventsList == NULL )
	{
		m_AllowEventsList = new AllowEventsList();
	}

	return m_AllowEventsList->Append( token, idx );
}

BOOL SIPMessage::AppendAllowEvents(
								   const AllowEvents & header
								   )
{
	GlobalLock();
	ParseAllowEventsList();

	MakeUnique();

	if( m_AllowEventsList == NULL )
	{
		m_AllowEventsList = new AllowEventsList();
	}

	m_AllowEventsList->Append( header );

	return TRUE;
}

AllowEvents & SIPMessage::GetAllowEventsAt(
	PINDEX idx
	)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseAllowEventsList();
	return (AllowEvents&)m_AllowEventsList->GetAt( idx );
}

PString SIPMessage::GetAllowEventsTokenAt(
	PINDEX idx1,
	PINDEX idx2
	)const
{
	GlobalLock();
	(const_cast< SIPMessage * >( this ))->ParseAllowEventsList();

	if( m_AllowEventsList == NULL )
	{
		return PString::Empty();
	}

	return m_AllowEventsList->GetTokenAt( idx1, idx2 );
}

BOOL SIPMessage::RemoveAllAllowEvents()
{
	GlobalLock();
	// cs( *this );

	MakeUnique();

	if( m_AllowEventsList != NULL )
	{
		delete m_AllowEventsList;
		m_AllowEventsList = NULL;
	}

	return TRUE;
}

///////////////////////////////////////////////////////

BOOL SIPMessage::ParseSubscriptionState()
{
	GlobalLock();
	// cs( *this );

	if( m_SubscriptionState != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "subscription-state" )
		{
			if( m_SubscriptionState == NULL )
			{
				m_SubscriptionState = new SubscriptionState( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_SubscriptionState != NULL;
}

BOOL SIPMessage::GetSubscriptionState(
									  SubscriptionState & h
									  )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseSubscriptionState() )
		return FALSE;

	if( !HasSubscriptionState() )
		return FALSE;

	h = *m_SubscriptionState;

	return TRUE;
}

void SIPMessage::SetSubscriptionState(
									  const SubscriptionState & h
									  )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_SubscriptionState != NULL )
		delete m_SubscriptionState;
	m_SubscriptionState = (SubscriptionState*)h.Clone();
}

BOOL SIPMessage::HasSubscriptionState()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseSubscriptionState();
}


BOOL SIPMessage::ParseSIPETag()
{
	GlobalLock();
	// cs( *this );

	if( m_SIPETag != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "sip-etag" )
		{
			if( m_SIPETag == NULL )
			{
				m_SIPETag = new SIPETag( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_SIPETag != NULL;
}

BOOL SIPMessage::ParseSIPIfMatch()
{
	GlobalLock();
	// cs( *this );

	if( m_SIPIfMatch != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "sip-if-match" )
		{
			if( m_SIPIfMatch == NULL )
			{
				m_SIPIfMatch = new SIPIfMatch( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_SIPIfMatch != NULL;
}

BOOL SIPMessage::IsXORHashSet()const
{
	return GetInternalHeader( "XORHASH" ) == "TRUE";
}

BOOL SIPMessage::SetXORHash( 
							BOOL willHash
							)
{
	if( GetInternalHeader( "XORHASH" ) != "TRUE" && willHash )
	{
		SetInternalHeader( "XORHASH", "TRUE"  );
	}else if( GetInternalHeader( "XORHASH" ) == "TRUE" && !willHash )
	{
		SIPHeader xorHash( "XORHASH" );
		PopInternalHeader( "XORHASH", xorHash );
	}

	return TRUE;
}

BOOL SIPMessage::GetXORHash( 
							PBYTEArray & out 
							)const
{
	PStringStream buff;
	buff << *this;

	PBYTEArray in;
	in.SetSize( buff.GetSize() );
	::memcpy( in.GetPointer(), buff.GetPointer(), buff.GetSize() );

	return ParserTools::XORHash( in, out, 
		ParserTools::GetDefaultXORHashKey1(), 
		ParserTools::GetDefaultXORHashKey2() );
}

PString SIPMessage::GetInterfaceAddress()const
{
	PString ifaceAddress;
	if( HasInternalHeader( "IFACE-ADDR" ) )
		ifaceAddress = GetInternalHeader( "IFACE-ADDR" );
	return ifaceAddress;
}

void SIPMessage::SetInterfaceAddress(
									 const PString & ifaceAddress
									 )
{
	if( ifaceAddress.IsEmpty() )
		return;

	if( HasInternalHeader( "IFACE-ADDR" ) )
		SetInternalHeader( "IFACE-ADDR", ifaceAddress );
	else
		AddInternalHeader( "IFACE-ADDR", ifaceAddress );
}

WORD SIPMessage::GetInterfacePort()const
{
	PString port = "5060";
	if( HasInternalHeader( "IFACE-PORT" ) )
		port = GetInternalHeader( "IFACE-PORT" );
	return port.AsUnsigned();
}

void SIPMessage::SetInterfacePort(
								  WORD port
								  )
{
	if( port == 0 )
		return;

	if( HasInternalHeader( "IFACE-PORT" ) )
		SetInternalHeader( "IFACE-PORT", PString( port ) );
	else
		AddInternalHeader( "IFACE-PORT", PString( port ) );
}

PString SIPMessage::GetUACoreName()const
{
	PString coreName;
	if( HasInternalHeader( "UA-CORE-NAME" ) && coreName.IsEmpty() )
		coreName = GetInternalHeader( "UA-CORE-NAME" );
	return coreName;
}

void SIPMessage::SetUACoreName(
							   const PString & coreName
							   )
{
	if( coreName.IsEmpty() )
		return;

	if( HasInternalHeader( "UA-CORE-NAME" ) )
		SetInternalHeader( "UA-CORE-NAME", coreName );
	else
		AddInternalHeader( "UA-CORE-NAME", coreName );
}


BOOL SIPMessage::IsValid( BOOL minimal )const
{
	GlobalLock();

	StartLine startLine;
	if( !GetStartLine( startLine ) )
		return FALSE;

	if( GetCSeqMethod().IsEmpty() )
		return FALSE;

	if( GetCallId().IsEmpty() )
		return FALSE;

	if( !minimal )
	{
		From from;
		if( !GetFrom( from ) )
			return FALSE;
		else if( from.GetURI().GetHost().IsEmpty() )
			return FALSE;

		To to;
		if( !GetTo( to ) )
			return FALSE;
		else if( to.GetURI().GetHost().IsEmpty() )
			return FALSE;

		if( GetViaSize() == 0 )
			return FALSE;

	}

	return TRUE;

}


BOOL SIPMessage::GetMinSE(
						  MinSE & h
						  )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseMinSE() )
		return FALSE;

	if( !HasMinSE() )
		return FALSE;

	h = *m_MinSE;

	return TRUE;
}

void SIPMessage::SetMinSE(
						  const MinSE & h
						  )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_MinSE != NULL )
		delete m_MinSE;

	m_MinSE = (MinSE*)h.Clone();
}

BOOL SIPMessage::HasMinSE()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseMinSE();
}

BOOL SIPMessage::GetSessionExpires(
								   SessionExpires & h
								   )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseSessionExpires() )
		return FALSE;

	if( !HasSessionExpires() )
		return FALSE;

	h = *m_SessionExpires;

	return TRUE;
}

void SIPMessage::SetSessionExpires(
								   const SessionExpires & h
								   )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_SessionExpires != NULL )
		delete m_SessionExpires;

	m_SessionExpires = (SessionExpires*)h.Clone();
}

BOOL SIPMessage::HasSessionExpires()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseSessionExpires();
}

BOOL SIPMessage::ParseMinSE()
{
	GlobalLock();

	if( m_MinSE != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "Min-SE" )
		{
			if( m_MinSE== NULL )
			{
				m_MinSE = new MinSE( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_MinSE!= NULL;
}

BOOL SIPMessage::ParseSessionExpires()
{
	GlobalLock();

	if( m_SessionExpires != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "Session-Expires" )
		{
			if( m_SessionExpires== NULL )
			{
				m_SessionExpires= new SessionExpires( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_SessionExpires!= NULL;
}


/////////////////////////
BOOL SIPMessage::ParseReplaces()
{
	GlobalLock();
	// cs( *this );

	if( m_Replaces != NULL )
		return TRUE;

	if( m_RawMessage == NULL )
		return FALSE;

	PStringArray & msg = *m_RawMessage;

	if( m_StartLine == NULL )
	{
		m_StartLine = new StartLine( msg[0] );
		msg.RemoveAt( 0 );
	}

	for( PINDEX i = 0; i < msg.GetSize(); i++ )
	{
		MimeHeader h( msg[i] );
		PCaselessString hname = h.GetMimeName();
		if( hname *= "replaces" )
		{
			if( m_Replaces == NULL )
			{
				m_Replaces = new Replaces( h );
			}

			msg.RemoveAt( i );
			break;
		}
	}

	return m_Replaces != NULL;
}

BOOL SIPMessage::GetReplaces(
							 Replaces & h
							 )const
{
	GlobalLock();
	if( !(const_cast< SIPMessage * >( this ))->ParseReplaces() )
		return FALSE;

	if( !HasReplaces() )
		return FALSE;

	h = *m_Replaces;

	return TRUE;
}


void SIPMessage::SetReplaces(
							 const Replaces & h
							 )
{
	GlobalLock();
	// cs( *this );
	MakeUnique();

	if( m_Replaces != NULL )
		delete m_Replaces;
	m_Replaces = (Replaces*)h.Clone();
}

BOOL SIPMessage::HasReplaces()const
{
	GlobalLock();
	return (const_cast< SIPMessage * >( this ))->ParseReplaces();
}

