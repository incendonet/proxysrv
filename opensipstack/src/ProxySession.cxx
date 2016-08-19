/*
*
* ProxySession.cxx
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
* $Log: ProxySession.cxx,v $
* Revision 1.22  2007/01/22 10:01:58  joegenbaclor
* Fixed bug in handling Notifies
*
* Revision 1.21  2006/11/30 08:19:38  joegenbaclor
* Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
*
* Revision 1.20  2006/11/22 08:18:46  rcolobong
* Update method OnSessionEvent where OnCheckRoutePolicy was added during EventFinalResponse
*
* Revision 1.19  2006/08/28 07:18:35  joegenbaclor
* Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
*
* Revision 1.18  2006/08/26 14:31:44  joegenbaclor
* Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
*
* Revision 1.17  2006/08/14 01:29:08  joegenbaclor
* Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
*
* Revision 1.16  2006/07/11 13:59:36  joegenbaclor
* Completed Proxy functionalities for B2BUA
*
* Revision 1.15  2006/05/19 06:30:37  joegenbaclor
* 1.  Fixed bug in SIPHeaderB where tag may not be parsed properly if no < > enclosure is present
* 2.  Various enhancements to OpenSBC and proxy session
*
* Revision 1.14  2006/05/17 03:43:07  joegenbaclor
* Bulk commit after sourceforge cvs migration changes
*
* Revision 1.13  2006/04/05 00:26:04  joegenbaclor
* 1.  Completed support for STUN NAT traversal method and OPAL style address translation
* 2.  More bug fixes to ProxySession
* 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
* 4.  Started work on OPAL outbound
*
* Revision 1.12  2006/03/30 05:34:16  joegenbaclor
* 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
* 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
* 3.  Changed SDP to include a blank session name if it is not present.
* 4.  Worked on OPAL classes in preparation for openphone use
*
* Revision 1.11  2006/03/27 01:25:33  joegenbaclor
* Some improvements on NICT and NIST state machines
*
* Revision 1.10  2006/03/25 18:40:21  joegenbaclor
* 1.  Worked on some race conditions on Finite State Machines during heavy load.
* 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
* 3,  Some minor improvements to simple-proxy application
*
* Revision 1.9  2006/03/24 15:42:18  joegenbaclor
* Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
*
* Revision 1.8  2006/03/23 12:53:16  joegenbaclor
* Fixed missing Destroy() command after a SendAcceptByRejection()
*
* Revision 1.7  2006/03/23 01:36:45  joegenbaclor
* Added local routing through simple registrar
*
* Revision 1.6  2006/03/22 08:45:52  joegenbaclor
* More work on simple-proxy and affected classes
*
* Revision 1.5  2006/03/14 03:53:53  joegenbaclor
* Initial source upload for BETA Release Candidate
*
*
*
*/

#include "ProxySession.h"
#include "ProxySessionManager.h"

#define new PNEW

using namespace UACORE;

ProxySession::ProxySession(
						   ProxySessionManager & sessionManager,
						   const SIPMessage & request,
						   const PString & sessionId
						   ) : SIPSession( (SIPSessionManager &)sessionManager, request, sessionId )
{
	m_WillCreateDialog = FALSE;
	m_IsInviteSession = FALSE;
	m_LogTag = "PROXY";
	LOG_IF_DEBUG( LogInfo(), "*** CREATED *** PROXY Session " <<  m_SessionId );
}

ProxySession::~ProxySession()
{
	LOG_IF_DEBUG( LogInfo(), "*** DESTROYED *** PROXY Session " <<  m_SessionId );
}

/// Overrides from SIP Session
void ProxySession::OnIncomingSIPMessage(
										SIPMessageArrival & messageEvent
										)
{
	GCREF( "Auto" );

	SIPSession::OnIncomingSIPMessage( messageEvent );

	PWaitAndSignal lock( m_IncomingMessageMutex );

	SIPMessage msg = messageEvent.GetMessage();

	if( msg.IsRequest() )
	{
		if( msg.IsInvite() )
		{ 
			m_WillCreateDialog = m_IsInviteSession = TRUE;
			m_OriginalInvite = msg;
		}else if( msg.IsSubscribe() )
		{
			m_WillCreateDialog = TRUE;
		}

		EnqueueSessionEvent( new SIPSessionEvent( *this, EventValidateRequest, msg ) );
	}else if( msg.Is1xx() )
	{
		EnqueueSessionEvent( new SIPSessionEvent( *this, EventProvisionalResponse, msg ) );
	}else
	{
		EnqueueSessionEvent( new SIPSessionEvent( *this, EventFinalResponse, msg ) );
	}
}

void ProxySession::OnSessionEvent(
								  int event,
								  const SIPMessage & eventMsg
								  )
{
	GCREF( "Auto" );

	PWaitAndSignal lock1( m_SessionMutex );

	Via via;
	PString viaBranch;

	SIPMessage validatedRequest = eventMsg;
	PIPSocket::Address targetAddress;
	WORD targetPort;

//PTRACE(1, "### OnSessionEvent msg: " << eventMsg.AsString());
	switch( event )
	{
	case EventValidateRequest:
//PTRACE(1, "### OnSessionEvent EventValidateRequest");
		if( ValidateRequest( validatedRequest ) )
		{

//PTRACE(1, "### OnSessionEvent EventValidateRequest 1");
			switch( OnCheckRoutePolicy( validatedRequest ) )
			{
			case RouteByAuthentication:
//PTRACE(1, "### OnSessionEvent EventValidateRequest 2");
				SendAcceptByChallenge( validatedRequest );
				Destroy();
				break;
			case RouteByAuthorization:
//PTRACE(1, "### OnSessionEvent EventValidateRequest 3");
				SendAcceptByAuthorization( validatedRequest );
				Destroy();
				break;
			case RouteByRedirection:
//PTRACE(1, "### OnSessionEvent EventValidateRequest 4");
				if( m_RedirectContactList.GetCollection().GetSize() == 0 )
					SendAcceptByRejection( eventMsg, SIPMessage::Code500_InternalServerError );
				else
					SendAcceptByRedirection( eventMsg, m_RedirectContactList );
				Destroy();
				break;
			case RouteBySession:
//PTRACE(1, "### OnSessionEvent EventValidateRequest 5");
				/// Create the via
				if( SIPTransport::Resolve( validatedRequest, targetAddress, targetPort ) )
				{
					if( m_SessionManager.ConstructVia( targetAddress, via, SIPTransport::UDP ) )
					{
						viaBranch = m_RecordRouteBranch.AsString();
						via.SetBranch( viaBranch );
						via.AddParameter( "rport", "" );
						validatedRequest.AppendVia( via );

						if( validatedRequest.IsInvite() )
						{
							m_RoutedInvite = validatedRequest;

							/// Create a tuple for CANCELS
							//ProxySessionManager & manager = ((ProxySessionManager&)m_SessionManager);
							//manager.CreateTuple( m_OriginalInvite, m_RoutedInvite, 32 );
						}


						LOG_IF_DEBUG( LogDebug(), "Proxying request " << validatedRequest.GetStartLine() );
//PTRACE(1, "### OnSessionEvent EventValidateRequest 5.5, vR: " << validatedRequest.AsString());
						SendRequest( validatedRequest );
					}
				}
				break;
			case RouteReject:
//PTRACE(1, "### OnSessionEvent EventValidateRequest 6");
				SendAcceptByRejection( eventMsg, GetSessionRejectResponse() );
				Destroy();
				break;
			case RouteRejectByForbidden:
//PTRACE(1, "### OnSessionEvent EventValidateRequest 7");
				SendAcceptByRejection( eventMsg, SIPMessage::Code403_Forbidden );
				Destroy();
				break;
			case RouteByLocalAccept:
//PTRACE(1, "### OnSessionEvent EventValidateRequest 8");
				OnProcessRequestAsLocal( eventMsg );
				Destroy();
				break;
			case RouteIgnore:
//PTRACE(1, "### OnSessionEvent EventValidateRequest RouteIgnore");
				Destroy();
				break;

			}
		}
		break;
	case EventProvisionalResponse:
//PTRACE(1, "### OnSessionEvent EventProvisionalResponse");
		OnProvisionalResponse( eventMsg );
		break;
	case EventFinalResponse:
		if(OnCheckRoutePolicy(validatedRequest) == RouteIgnore)
		{
//PTRACE(1, "### EventFinalResponse (not 'Invite', RouteIgnore): ");
		}
		else
		{
//PTRACE(1, "### EventFinalResponse (not 'Invite'): " << validatedRequest.AsString());
			OnFinalResponse( validatedRequest );
		}
		break;
	}

}

BOOL ProxySession::ValidateRequest(
								   SIPMessage & request
								   )
{
	GCREF( "Auto" );


	/*
	16.3 Request Validation

	Before an element can proxy a request, it MUST verify the message's
	validity.  A valid message must pass the following checks:

	1. Reasonable Syntax
	2. URI scheme
	3. Max-Forwards
	4. (Optional) Loop Detection
	5. Proxy-Require
	6. Proxy-Authorization
	*/
	/// create a brach based on the original request
	m_RecordRouteBranch = RecordRouteBranch( request );

	OnCheckRouteInformation( request );

	if( !OnCheckURIScheme( request ) )
	{
		SendAcceptByRejection( request, SIPMessage::Code416_UnsupportedURIScheme );
		Destroy();
		return FALSE;
	}

	if( !OnCheckMaxForwards( request ) )
	{
		SendAcceptByRejection( request, SIPMessage::Code483_TooManyHops );
		Destroy();
		return FALSE;
	}

	if( !OnCheckLoop( request ) )
	{
		SendAcceptByRejection( request, SIPMessage::Code482_LoopDetected );
		Destroy();
		return FALSE;
	}



	if( request.IsInvite() )
	{
		if( !OnCheckProxyRequire( request ) )
		{
			SendAcceptByRejection( request, SIPMessage::Code420_BadExtension );
			Destroy();
			return FALSE;
		}

		AuthStatus authStatus = OnCheckAuthorization( request );

		if( authStatus == Unauthorized )
		{
			Destroy();
			return FALSE;
		}else if( authStatus == AuthRequired )
		{
			SendAcceptByChallenge( request );
			Destroy();
			return FALSE;
		}
	}else if( request.IsCancel() )
	{
		m_SessionManager.GetUserAgent().GetStack().CancelInviteServerTransaction(m_Request);
	}

	return TRUE;
}

void ProxySession::OnCheckRouteInformation(
	SIPMessage & request
	)
{

	/*If the first value in the Route header field indicates this proxy,
	the proxy MUST remove that value from the request.
	*/

	if( request.HasRoute() && request.GetRouteSize() > 0 )
	{
		Route route = request.GetRouteAt(0);
		RouteURI routeURI;
		route.GetURI( routeURI, 0 );

		if( GetTransportManager()->IsLocalAddressAndPort( routeURI.GetURI() ) )
		{
			LOG_IF_DEBUG( LogDetail(), "*** ROUTE-URI REMOVAL *** Route URI Resolves to this Proxy." );
			LOG_IF_DEBUG( LogInfo(), "*** ROUTE-URI REMOVAL *** " << request.PopTopRouteURI() );
		}
	}

	/*
	16.4 Route Information Preprocessing

	The proxy MUST inspect the Request-URI of the request.  If the
	Request-URI of the request contains a value this proxy previously
	placed into a Record-Route header field (see Section 16.6 item 4),
	the proxy MUST replace the Request-URI in the request with the last
	value from the Route header field, and remove that value from the
	Route header field.  The proxy MUST then proceed as if it received
	this modified request.

	This will only happen when the element sending the request to the
	proxy (which may have been an endpoint) is a strict router.  This
	rewrite on receive is necessary to enable backwards compatibility
	with those elements.  It also allows elements following this
	specification to preserve the Request-URI through strict-routing
	proxies (see Section 12.2.1.1).

	This requirement does not obligate a proxy to keep state in order
	to detect URIs it previously placed in Record-Route header fields.
	Instead, a proxy need only place enough information in those URIs
	to recognize them as values it provided when they later appear.
	*/


	RequestLine  requestLine;
	if( !request.GetRequestLine( requestLine ) )
		return;

	SIPURI requestURI = requestLine.GetRequestURI();

	if( m_RecordRouteURI.GetURI().GetHost() == requestURI.GetHost() )
	{
		if( requestURI.FindParameter( "lr" ) != P_MAX_INDEX )
		{
			if( request.HasRoute() && request.GetRouteSize() > 0 )
			{
				LOG_IF_DEBUG( LogInfo(), "*** REQUEST VALIDATION *** Detected strict router" );
				RouteURI routeURI = request.PopLastRouteURI();
				LOG_IF_DEBUG( LogDetail(), "*** REQUEST VALIDATION *** Old: " << requestLine );
				requestLine.SetRequestURI( routeURI.GetURI() );
				LOG_IF_DEBUG( LogDetail(), "*** REQUEST VALIDATION *** New: " << requestLine );
				request.SetStartLine( requestLine );
			}
		}
	}


	/*
	If the Request-URI contains a maddr parameter, the proxy MUST check
	to see if its value is in the set of addresses or domains the proxy
	is configured to be responsible for.  If the Request-URI has a maddr
	parameter with a value the proxy is responsible for, and the request
	was received using the port and transport indicated (explicitly or by
	default) in the Request-URI, the proxy MUST strip the maddr and any
	non-default port or transport parameter and continue processing as if
	those values had not been present in the request.

	A request may arrive with a maddr matching the proxy, but on a
	port or transport different from that indicated in the URI.  Such
	a request needs to be forwarded to the proxy using the indicated
	port and transport.

	*/


}

BOOL ProxySession::OnCheckURIScheme(
									const SIPMessage & request
									)
{
	GCREF( "Auto" );
	/*
	URI scheme check

	If the Request-URI has a URI whose scheme is not understood by the
	proxy, the proxy SHOULD reject the request with a 416 (Unsupported
	URI Scheme) response.
	*/

	RequestLine requestLine;

	if( !request.GetRequestLine( requestLine ) )
		return FALSE;

	PString scheme = requestLine.GetRequestURI().GetScheme();

	if( scheme *= "sip" )
		return TRUE;

	LOG_IF_DEBUG( LogError(), "*** VALIDATION ERROR *** " << scheme << " not supported" );
	return FALSE;
}

BOOL ProxySession::OnCheckMaxForwards(
									  const SIPMessage & request
									  )
{
	/*Max-Forwards check

	The Max-Forwards header field (Section 20.22) is used to limit the
	number of elements a SIP request can traverse.

	If the request does not contain a Max-Forwards header field, this
	check is passed.

	If the request contains a Max-Forwards header field with a field
	value greater than zero, the check is passed.

	If the request contains a Max-Forwards header field with a field
	value of zero (0), the element MUST NOT forward the request.  If
	the request was for OPTIONS, the element MAY act as the final
	recipient and respond per Section 11.  Otherwise, the element MUST
	return a 483 (Too many hops) response. */

	GCREF( "Auto" );

	if( !request.HasMaxForwards() )
		return TRUE;

	MaxForwards maxForwards;
	request.GetMaxForwards( maxForwards );

	if( maxForwards.AsInt64() == 0 )
	{
		LOG_IF_DEBUG( LogError(), "*** VALIDATION ERROR *** Maximum Forward Reached" );
		return FALSE;
	}

	return TRUE;
}

BOOL ProxySession::OnCheckLoop(
							   const SIPMessage & request
							   )
{
	GCREF( "Auto" );
	/*
	Loop detection is performed by verifying that, when a request
	returns to a proxy, those fields having an impact on the
	processing of the request have not changed.  The value placed
	in this part of the branch parameter SHOULD reflect all of
	those fields (including any Route, Proxy-Require and Proxy-
	Authorization header fields).  This is to ensure that if the
	request is routed back to the proxy and one of those fields
	changes, it is treated as a spiral and not a loop (see Section
	16.3).  A common way to create this value is to compute a
	cryptographic hash of the To tag, From tag, Call-ID header
	field, the Request-URI of the request received (before
	translation), the topmost Via header, and the sequence number
	from the CSeq header field, in addition to any Proxy-Require
	and Proxy-Authorization header fields that may be present.  The

	algorithm used to compute the hash is implementation-dependent,
	but MD5 (RFC 1321 [35]), expressed in hexadecimal, is a
	reasonable choice.  (Base64 is not permissible for a token.)

	If a proxy wishes to detect loops, the "branch" parameter it
	supplies MUST depend on all information affecting processing of
	a request, including the incoming Request-URI and any header
	fields affecting the request's admission or routing.  This is
	necessary to distinguish looped requests from requests whose
	routing parameters have changed before returning to this
	server.

	The request method MUST NOT be included in the calculation of
	the branch parameter.  In particular, CANCEL and ACK requests
	(for non-2xx responses) MUST have the same branch value as the
	corresponding request they cancel or acknowledge.  The branch
	parameter is used in correlating those requests at the server
	handling them (see Sections 17.2.3 and 9.2).
	*/


	for( PINDEX i = 0; i < request.GetViaSize(); i++ )
	{
		Via via;
		if( request.GetViaAt(i, via ) )
		{
			PString branch = via.GetBranch();
			if( branch != PString::Empty() )
				if( m_RecordRouteBranch == branch )
				{
					RecordRouteBranch branchCheck( request );

					if( branchCheck *= m_RecordRouteBranch )
					{
						LOG_IF_DEBUG( LogError(), "*** VALIDATION ERROR *** Loop detected" );
						return FALSE;
					}else
					{
						LOG_IF_DEBUG( LogWarning(), "*** SPIRAL DETECTED ***" );
					}
				}
		}
	}

	return TRUE;
}

BOOL ProxySession::OnCheckProxyRequire(
									   const SIPMessage & /*request*/
									   )
{
	GCREF( "Auto" );
	return TRUE;
}

ProxySession::AuthStatus ProxySession::OnCheckAuthorization(
	const SIPMessage & request
	)
{
	GCREF( "Auto" );
	return ((ProxySessionManager&)m_SessionManager).OnCheckAuthorization( *this, request );
}

ProxySession::RoutePolicy ProxySession::OnCheckRoutePolicy(
	SIPMessage & request
	)
{
	GCREF( "Auto" );
	return ((ProxySessionManager&)m_SessionManager).OnCheckRoutePolicy( *this, request );
}




void ProxySession::OnProvisionalResponse( const SIPMessage & message )
{
	GCREF( "Auto" );
	SIPMessage response = message;

	Via via;
	response.GetViaAt( 0, via );

	if( GetTransportManager()->IsLocalAddressAndPort( via.GetURI() ) )
		response.PopTopVia();

	SendRequest( response );
	((ProxySessionManager&)m_SessionManager).OnProvisionalResponse( *this, response );
}

void ProxySession::OnFinalResponse( const SIPMessage & message )
{
	GCREF( "Auto" );

	SIPMessage response = message;

	Via via;
	response.GetViaAt( 0, via );

	if( GetTransportManager()->IsLocalAddressAndPort( via.GetURI() ) )
		response.PopTopVia();

	SendRequest( response );

	ProxySessionManager & manager = ((ProxySessionManager&)m_SessionManager);

	manager.OnFinalResponse( *this, response );

	if( response.GetCSeqMethod() == "INVITE" )
	{
		/// create a tuple for ACKs
		if( response.Is2xx() )
			manager.CreateTuple( m_OriginalInvite, m_RoutedInvite, 10 );
		else
			manager.RemoveTuple( m_RoutedInvite );
	}

	Destroy();
}

void ProxySession::SendAcceptByAuthorization(
	const SIPMessage & message
	)
{
	SIPMessage request = message;
	To to;
	request.GetTo( to );
	SIPMessage unauthorized;
	request.CreateResponse( unauthorized, SIPMessage::Code401_Unauthorized, "" );

	WWWAuthenticate auth;
	MD5::Nonce opaque;
	auth.SetLeadString( "Digest" );
	auth.AddParameter( "realm", ParserTools::Quote( m_URI.GetHost() ) );
	auth.AddParameter( "nonce", m_Nonce.AsQuotedString() );
	auth.AddParameter( "opaque", opaque.AsQuotedString() );
	auth.AddParameter( "algorithm", "MD5" );
	unauthorized.SetWWWAuthenticate( auth );

	SendRequest( unauthorized );

	LOG_IF_DEBUG( LogDetail(), message.GetCSeqMethod() << " request from " << to.GetURI().GetUser() << " *** Unauthorized ***" );

}

void ProxySession::SendAcceptByChallenge( const SIPMessage & message )
{
	SIPMessage request = message;
	To to;
	request.GetTo( to );
	SIPMessage unauthorized;
	request.CreateResponse( unauthorized, SIPMessage::Code407_ProxyAuthenticationRequired, "" );

	ProxyAuthenticate auth;
	MD5::Nonce opaque;
	auth.SetLeadString( "Digest" );
	auth.AddParameter( "realm", ParserTools::Quote( m_URI.GetHost() ) );
	auth.AddParameter( "nonce", m_Nonce.AsQuotedString() );
	auth.AddParameter( "opaque", opaque.AsQuotedString() );
	auth.AddParameter( "algorithm", "MD5" );
	unauthorized.SetProxyAuthenticate( auth );

	SendRequest( unauthorized );

	LOG_IF_DEBUG( LogDetail(), message.GetCSeqMethod() << " request from " << to.GetURI().GetUser() << " *** Unauthorized ***" );
}

void ProxySession::OnProcessRequestAsLocal(
	const SIPMessage & request
	)
{
	ProxySessionManager & manager = ((ProxySessionManager&)m_SessionManager);
	manager.OnProcessRequestAsLocal( *this, request );
}

