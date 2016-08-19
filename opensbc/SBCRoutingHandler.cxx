/*
*
* SBCRoutingHandler.cxx
*
* Open SIP Stack ( OSS )
*
* Copyright (c) opensipstack.org.
*
* The contents of this file are subject to the Mozilla Public License
* Version 1.0 (the "License"); you may not use this file except in
* compliance with the License. You may obtain a copy of the License at
* http://www.mozilla.org/MPL/
*
* Software distributed under the License is distributed on an "AS IS"
* basis, WITHOUT WARRANTY OF ANY KIND, either expressed or implied. See
* the License for the specific language governing rights and limitations
* under the License.
*
* The Original Code is OpenSIPStack.
*
* The Initial Developer of the Original Code is opensipstack.org.
*
* The author of this code is Joegen E. Baclor
*
* $Log: SBCRoutingHandler.cxx,v $
* Revision 1.16  2007/01/22 13:12:11  joegenbaclor
* Updated VC710 project files
*
* Revision 1.15  2007/01/22 10:00:58  joegenbaclor
* Fixed ProxyRouteRequest
*
* Revision 1.14  2007/01/19 08:20:29  joegenbaclor
* Added check for none routable proxy request s
*
* Revision 1.13  2007/01/18 06:06:56  joegenbaclor
* Incremented Build Number for latest development release
*
* Revision 1.12  2007/01/17 00:09:02  joegenbaclor
* Added SysLog
*
* Revision 1.11  2007/01/15 10:06:57  joegenbaclor
* More on XML-RPC Registration Queries
*
* Revision 1.10  2007/01/15 09:24:47  joegenbaclor
* Added capability to find Registraton via XML-RPC
*
* Revision 1.9  2007/01/12 10:50:05  joegenbaclor
* minor proxy bug fixes
* more launcher code
*
* Revision 1.8  2007/01/11 10:00:26  joegenbaclor
* more launcher code
*
* Revision 1.7  2007/01/10 05:00:39  joegenbaclor
* Added B2BUpperReg mode.
*
* Revision 1.6  2006/12/21 08:56:59  joegenbaclor
* Added pid in b2bua log file name
* Fixed bug in proxy relay where startline URI is not rewritten if a static route is
*  not found
*
* Revision 1.5  2006/12/14 02:18:37  rcolobong
* Fix bug on OnRecordRouteShift causing an assertion failure during for loop
*
* Revision 1.4  2006/11/23 11:24:27  joegenbaclor
* Added capability to route none INVITE requests like instant messages
*
* Revision 1.3  2006/11/22 11:41:01  rcolobong
* 1. Update handling on Upper Registration
* 2. Override method on OnOrphanedMessage
* 3. Create new handling for route-shift in Record Routes Header
*
* Revision 1.2  2006/10/11 06:32:58  rcolobong
* Seperate method for routing using local registration.
*
* Revision 1.1  2006/08/14 10:05:01  rcolobong
* Convert B2BUA to SBC
* Support MP logging
*
* Revision 1.7  2006/08/04 05:48:58  rcolobong
* 1.  Add Getter for StaticRoutes
* 2.  Add virtual to method RefreshStaticRoutes
* 3.  RefreshStaticRoutes is not invoke during construction
*
* Revision 1.6  2006/07/19 02:19:13  joegenbaclor
* 1.  Bug fixes concerning B2BUA destruction during during 3xx, 4xx, 5xx, 6xx local response
* 2.  Fixed bug where Route header is never removed from outbound Invite resulting that may result to a loop when
*      Inbound Invite has a Route header present
* 3.  Added RemoveAllRoutes() in SIPMessage
*
* Revision 1.5  2006/07/13 06:53:03  joegenbaclor
* 1.  Introduced Sanity checks for incoming SIP Message
* 2.  Corrected bug in SIPURI where enum scheme is not getting recognized.
* 3.  Added ENUM support to routing
* 4.  Introduced global routing flag to indicate when and not to rewrite outbound To URI's
*
* Revision 1.4  2006/07/11 13:59:34  joegenbaclor
* Completed Proxy functionalities for B2BUA
*
* Revision 1.3  2006/07/10 06:29:36  joegenbaclor
* Completed crude Registration support for B2BUA
*
* Revision 1.2  2006/07/03 15:29:44  joegenbaclor
* Started merging OpenB2BUA into OpenSBC
*
* Revision 1.1  2006/06/20 09:58:11  joegenbaclor
* *** empty log message ***
*
*
*/

#include "SBCRoutingHandler.h"

#define new PNEW

static const char configKeySection[] = "ProxySrv Routes";
static const char configKeyUpperRegistrarSection[] = "Upper Registration Routes";
static const char configKeyRelayRouteSection[] = "Relay Routes";
static const char configKeyRoute[] = "Route";
static const char configKeyRewriteToURI[] = "Rewrite TO URI";

SBCRoutingHandler::SBCRoutingHandler(  
									 OpenSBC & b2bua 
									 ) : B2BRoutingInterface( b2bua )
{

}

SBCRoutingHandler::~SBCRoutingHandler()
{
}

void SBCRoutingHandler::RefreshStaticRoutes()
{
	OSSAppConfig * config = OSSAppConfig::GetInstance();
	PStringArray routes;
	for( PINDEX i = 0; i <  config->GetListSize( configKeySection, configKeyRoute ); i++ )
		routes.AppendString( config->GetListItem( configKeySection, configKeyRoute, i ) );

	if( routes.GetSize() > 0 )
		m_StaticRoutes.Parse( routes );

	PStringArray upperRegistrationRoutes;
	for( PINDEX j = 0; j <  config->GetListSize( configKeyUpperRegistrarSection, configKeyRoute ); j++ )
		upperRegistrationRoutes.AppendString( config->GetListItem( configKeyUpperRegistrarSection, configKeyRoute, j ) );

	if( upperRegistrationRoutes.GetSize() > 0 )
		m_UpperRegistrationRoutes.Parse( upperRegistrationRoutes );

	PStringArray relayRoutes;
	for( PINDEX x = 0; x <  config->GetListSize( configKeyRelayRouteSection, configKeyRoute ); x++ )
		relayRoutes.AppendString( config->GetListItem( configKeyRelayRouteSection, configKeyRoute, x ) );

	if( relayRoutes.GetSize() > 0 )
		m_RelayRoutes.Parse( relayRoutes );
}

BOOL SBCRoutingHandler::B2BRouteRequest(
										const SIPMessage & request,
										SIPURI  & route
										)
{
	To to;
	request.GetTo( to );
	SIPURI & uri = to.GetURI();
	PString user = uri.GetUser();

	SIPMessage registration;

	if( !GetB2BUA().GetRegistrar()->GetRegistrationDB().FindRegistration( uri, registration ) )
		return FALSE;

	return RouteLocalRegistration( route, request, registration );
}

BOOL SBCRoutingHandler::B2BRouteCall(
									 B2BUAConnection & connection,
									 const SIPMessage & invite
									 )
{
	To to;
	invite.GetTo( to );
	SIPURI & uri = to.GetURI();
	PString user = uri.GetUser();
	OSSAppConfig * config = OSSAppConfig::GetInstance();

	SIPMessage registration;

	if( GetB2BUA().GetUAMode() == B2BUserAgent::B2BOnlyMode && 
		OSSAppConfig::GetInstance()->GetType() == OSSAppConfig::XMLRPCClientProvider 
		)
	{

		OpenSBCDaemon * app = dynamic_cast<OpenSBCDaemon*>(OSSApplication::GetInstance());
		PString serverURL = app->GetExtendedArgs().GetParameter( "xml-rpc-registrar" );
		PTRACE( 1, "Using remote XML-RPC registrar at " << serverURL );
		BOOL ok = FALSE;
		if( !serverURL.IsEmpty() )
		{
			PArray<PStringToString>  cmd;
			PArray<PStringToString>  response;
			PStringToString * cmdParams = new PStringToString();
			cmdParams->SetAt( "URI", uri.AsString() );
			cmd.SetAt( 0, cmdParams );
			config->SendRequest( "Registrar.FindRegistration", cmd, response, serverURL );
			PStringToString * responseParams = static_cast<PStringToString*>(response.GetAt(0));
			if( responseParams != NULL )
			{
				PString * reg = responseParams->GetAt( "Registration" );
				if( reg == NULL || reg->IsEmpty() )
				{
					PTRACE( 1, "No Registration found" );
					ok = FALSE;
				}else
				{
					PTRACE( 1, "Got Register " << *reg );
					connection.AddRoute( SIPURI(*reg) );
					return TRUE;
				}
			}
		}

		if( !ok )
		{
			BOOL rewriteToURI = config->GetBoolean( 
				configKeySection, 
				configKeyRewriteToURI, FALSE );

			connection.SetRewriteToURI( rewriteToURI );

			return m_StaticRoutes.FindRoute( invite, connection );
		}
	}else if( !GetB2BUA().GetRegistrar()->GetRegistrationDB().FindRegistration( uri, registration ) )
	{
		OSSAppConfig * config = OSSAppConfig::GetInstance();
		BOOL rewriteToURI = config->GetBoolean( 
			configKeySection, 
			configKeyRewriteToURI, FALSE );

		connection.SetRewriteToURI( rewriteToURI );

		return m_StaticRoutes.FindRoute( invite, connection );
	}

	return RouteLocalRegistration( connection, invite, registration );
}

ProxySession::RoutePolicy SBCRoutingHandler::RouteProxyRequest(
	ProxySession & session,
	SIPMessage & request
	)
{

	//// check to TO header if it resolves to this proxy
	To to;
	request.GetTo( to );
	SIPURI target = to.GetURI();
	SIPURI targetURI;

#if 0
	if( SIPTransport::IsLocalAddress( target ) )
	{
		/// we dont proxy local domain routing
		if( m_RelayRoutes.FindRoute( request, targetURI, TRUE ) )
			return ProxySession::RouteReject;
	}
#endif

	RequestLine requestLine;
	request.GetRequestLine( requestLine );
	SIPURI oldURI = requestLine.GetRequestURI();



	if( request.IsRegister() )
	{
		/// check if routable via upper registration
		if( m_UpperRegistrationRoutes.FindRoute( request, targetURI, FALSE ) )
		{
			PIPSocket::Address targetAddress, listenerAddress;
			WORD targetPort, listenerPort = 5060;

			if( !SIPTransport::Resolve( targetURI, targetAddress, targetPort ) )
			{
				LOG( LogError(), "Unable to resolve Upper Registrar URI for  " << request.GetStartLine() ); 
				PTRACE( 1, "Unable to resolve Upper Registrar URI for  " << request.GetStartLine() );
				return ProxySession::RouteReject; 
			}

			if( !GetB2BUA().GetStack().GetTransportManager()->GetListenerAddress(
				SIPTransport::UDP, targetAddress, listenerAddress, listenerPort ) )
			{
				LOG( LogError(), "Unable to identify a network interface for  " << request.GetStartLine() );
				PTRACE( 1, "Unable to identify a network interface for  " << request.GetStartLine() );
				return ProxySession::RouteReject; 
			}

			SIPURI tempUri = request.GetToURI();
			tempUri.SetHost( listenerAddress.AsString() );

			if( GetSBC().GetUAMode() == B2BUserAgent::FullMode )
				tempUri.SetPort( PString( GetSBC().GetBackDoorPort() ) );
			else
				tempUri.SetPort( PString( listenerPort ) );

			PTRACE( 1, "*** UPPER REGISTRATION ENABLE *** for " << request.GetToURI() );
			session.AddInternalHeader( "enable-upper-registration", tempUri.AsString() );

			Contact contact;
			ContactURI contactURI;
			contactURI.SetURI( tempUri );
			contact.AddURI( contactURI );
			request.RemoveAllContact();
			request.AppendContact( contact );

			oldURI.SetHost( targetURI.GetHost() );
			oldURI.SetPort( targetURI.GetPort() );

			requestLine.SetRequestURI( oldURI );
			request.SetStartLine( requestLine );

			return ProxySession::RouteBySession;
		}
	}
	else if (request.IsBusyHere())	// FIX - Also check that request.GetCSeq().GetMethod() is 'INVITE'?
	{
#if 0
		return RouteProxyNISTRequest( session, request );
#else
		// Do FindRoute to get next URI in Target array
		BOOL		bRes = FALSE;
		BOOL		relay = FALSE;
		//BOOL		allowRoundRobin = TRUE;
		SIPMessage	*pPrevInvite = 0, newRequest;
		Via			newVia, origVia;

//		PTRACE(1, "### BUSYHERE for CallId: " << request.GetCallId());

		// Find the matching Invite
		pPrevInvite = FindPendingInviteByCallId(request.GetCallId());
		if(pPrevInvite == 0)
		{
			// Didn't find the prev-invite, return the busyhere.
//			PTRACE(1, "### BUSYHERE (FindPendingInviteByCallId failed) returning BH for CallId: " << request.GetCallId());
			relay = FALSE;
		}
		else
		{
#if 0
//			PTRACE(1, "### BUSYHERE found prev-invite, about to FindRoute for CallId: " << request.GetCallId());
			if(!m_AppRelayRoutes.FindRoute(*pPrevInvite, targetURI, allowRoundRobin, TRUE ))
			{
				if(m_RelayRoutes.FindRoute(*pPrevInvite, targetURI, allowRoundRobin, TRUE ))
				{
					relay = TRUE;
				}
			}
			else
			{
				relay = TRUE;
			}
#else		// Don't bother with FindRoute, next pass through (after we've EnqueueSessionEvent() the new Invite event) will do it.
			relay = TRUE;
#endif
		}

		if(!relay)
		{
			// FIX - Should route call to overflow extension/address.
			// Do default routing.
//			PTRACE(1, "### BUSYHERE (FindRoute failed), returning BH for CallId: " << request.GetCallId());
			return RouteProxyNISTRequest( session, request );
		}
		else
		{
			//PTRACE(1, "### BUSYHERE attempting INVITE reroute for CallId: " << newRequest.GetCallId() << " to URI: " << targetURI);

			newRequest = pPrevInvite->AsString();	// Inefficient way to copy msg...

			// Reset the 'Via' (assumes 1'st line is what we'll work with.)
			bRes = newRequest.GetViaAt(0, origVia);
			newVia.SetProtocol(origVia.GetProtocol());
			newVia.SetAddress(origVia.GetAddress());
			newVia.SetPort(origVia.GetPort());
			newVia.SetBranch(origVia.GetBranch());
			newVia.SetRPort(origVia.GetRPort());
			newVia.SetReceiveAddress(origVia.GetReceiveAddress());

			newRequest.RemoveAllVias();
			newRequest.AppendVia(newVia);

			//
			// FIX - The following code is cut&pasted from the Invite section below.  Clean this up!!!
			//

			session.SetCreateDialog();
//PTRACE(1, "### BUSYHERE 1 AsString: " << newRequest.AsString());
			//session.SetDialogPeerAddress( targetURI.AsString() );

			RouteURI recordRoute;
			SIPURI routeURI;

			PIPSocket::Address targetAddress, listenerAddress;
			//WORD targetPort = 0, listenerPort = 0;

			/*
			if( !SIPTransport::Resolve( targetURI, targetAddress, targetPort ) )
			{
				PTRACE( 1, "(2) Unable to resolve target UA for  " << newRequest.GetStartLine() );
				LOG( LogError(), "(2) Unable to resolve target UA for  " << newRequest.GetStartLine() );
				return ProxySession::RouteReject;
			}

			if( !GetB2BUA().GetStack().GetTransportManager()->GetListenerAddress(
				SIPTransport::UDP, targetAddress, listenerAddress, listenerPort ) )
			{
				PTRACE( 1, "Unable to identify a network interface for  " << newRequest.GetStartLine() );
				LOG( LogError(), "Unable to identify a network interface for  " << newRequest.GetStartLine() );
				return ProxySession::RouteReject; 
			}

			routeURI.SetHost( listenerAddress.AsString() );
			routeURI.SetPort( PString( listenerPort ) );
//PTRACE(1, "### BUSYHERE 2 AsString: " << newRequest.AsString());


			if( SIPTransport::IsPrivateNetwork( listenerAddress ) && !SIPTransport::IsPrivateNetwork( newRequest.GetTopContactURI().GetAddress() ) )
			{
				/// public to private routing... lets tag the record route
				PIPSocket::Address shiftAddress;
				WORD shiftPort;

				GetB2BUA().GetStack().GetTransportManager()->GetListenerAddress(
					SIPTransport::UDP, newRequest.GetTopContactURI().GetAddress(), shiftAddress, shiftPort );
				PString routeShift = shiftAddress.AsString() + ":" + PString( shiftPort );

				routeURI.AddParameter( "route-shift", routeShift );
			}
//PTRACE(1, "### BUSYHERE 3 AsString: " << newRequest.AsString());

//			recordRoute.SetURI( routeURI );
//			recordRoute.SetLooseRouter( TRUE );
//			newRequest.AppendRecordRoute( recordRoute );
			oldURI.SetHost( targetURI.GetHost() );
			oldURI.SetPort( targetURI.GetPort() );

			To	tmpTo;
			newRequest.GetTo(tmpTo);						// FIX - Is this always correct?
			oldURI.SetUser( tmpTo.GetURI().GetUser() );

			requestLine.SetRequestURI( oldURI );
			*/
			requestLine.SetMethod("INVITE");
			newRequest.SetStartLine( requestLine );

//PTRACE(1, "### BUSYHERE 4 AsString: " << newRequest.AsString());

			session.EnqueueSessionEvent(new SIPSessionEvent(session, session.EventValidateRequest, newRequest));

			//return RouteProxyNISTRequest(session, request);
			return ProxySession::RouteIgnore;
		}
#endif
	} // if(busyhere)
	else if(request.IsOkToInvite())
	{
		BOOL	bRes = FALSE;

//		PTRACE(1, "### OK to INVITE, removing CallId: " << request.GetCallId());

		// FIX - Will it even get here, since an "OK" isn't a request?
		// Clear the invite entry from the list
		bRes = FindAndRemovePendingInviteByCallId(request.GetCallId());
		if(!bRes)
		{
//			PTRACE(1, "### No prev-invite found for CallId: " << request.GetCallId());
		}

		// Do default routing
		return RouteProxyNISTRequest( session, request );
	} // if(IsOkToInvite)
	else if( /*request.IsInvite()*/ !request.IsCancel() && !request.IsAck() )
	{
		BOOL relay = FALSE;
		BOOL allowRoundRobin = request.IsInvite();


//PTRACE(1, "### !IsCancel && !IsAck for CallId: " << request.GetCallId() << "starting with: " << request.AsString());
		if( !m_AppRelayRoutes.FindRoute( request, targetURI, allowRoundRobin, TRUE ) )
		{
			if( m_RelayRoutes.FindRoute( request, targetURI, allowRoundRobin, TRUE ) )
				relay = TRUE;
		}else
		{
			relay = TRUE;
		}

		if( relay )
		{
//PTRACE(1, "### !IsCancel && !IsAck for CallId: " << request.GetCallId() << ", found route: " << targetURI);
			session.SetCreateDialog();
			session.SetDialogPeerAddress( targetURI.AsString() );

			RouteURI recordRoute;
			SIPURI routeURI;

			PIPSocket::Address targetAddress, listenerAddress;
			WORD targetPort = 0, listenerPort = 0;

			if( !SIPTransport::Resolve( targetURI, targetAddress, targetPort ) )
			{
				PTRACE( 1, "(2) Unable to resolve target UA for  " << request.GetStartLine() );
				LOG( LogError(), "(2) Unable to resolve target UA for  " << request.GetStartLine() );
				return ProxySession::RouteReject;
			}

			if( !GetB2BUA().GetStack().GetTransportManager()->GetListenerAddress(
				SIPTransport::UDP, targetAddress, listenerAddress, listenerPort ) )
			{
				PTRACE( 1, "Unable to identify a network interface for  " << request.GetStartLine() );
				LOG( LogError(), "Unable to identify a network interface for  " << request.GetStartLine() );
				return ProxySession::RouteReject; 
			}

			routeURI.SetHost( listenerAddress.AsString() );
			routeURI.SetPort( PString( listenerPort ) );


			if( SIPTransport::IsPrivateNetwork( listenerAddress ) && !SIPTransport::IsPrivateNetwork( request.GetTopContactURI().GetAddress() ) )
			{
				/// public to private routing... lets tag the record route
				PIPSocket::Address shiftAddress;
				WORD shiftPort;

				GetB2BUA().GetStack().GetTransportManager()->GetListenerAddress(
					SIPTransport::UDP, request.GetTopContactURI().GetAddress(), shiftAddress, shiftPort );
				PString routeShift = shiftAddress.AsString() + ":" + PString( shiftPort );

				routeURI.AddParameter( "route-shift", routeShift );
			}
#if 0
			recordRoute.SetURI( routeURI );
			recordRoute.SetLooseRouter( TRUE );
			request.AppendRecordRoute( recordRoute );
#endif
			oldURI.SetHost( targetURI.GetHost() );
			oldURI.SetPort( targetURI.GetPort() );

			requestLine.SetRequestURI( oldURI );
			request.SetStartLine( requestLine );
//PTRACE(1, "### INVITE startline: " << request.GetStartLine());

			if(request.IsInvite())
			{	// Save the Invite in case it needs to be re-used if the UA was busy (see case above.)
				//PTRACE(1, "### Saving INVITE for CallId: " << request.GetCallId() << "  AsString: " << request.AsString());

				AddPendingInvite(request);
			}

//PTRACE(1, "### !IsCancel && !IsAck for CallId: " << request.GetCallId() << "ending with: " << request.AsString());
			return ProxySession::RouteBySession;
		}
		else
		{
			/// check if the request URI resolves to us
			RequestLine rq;
			if( request.GetRequestLine( rq ) )
			{
				if( session.GetTransportManager()->IsLocalAddressAndPort( rq.GetRequestURI() ) )
				{
					/// lets try to resolve the TO URI
					if( !session.GetTransportManager()->IsLocalAddressAndPort( request.GetToURI() ) )
					{
						SIPURI uri( request.GetToURI().AsString( FALSE, FALSE ) );
						uri.SetUser( rq.GetRequestURI().GetUser() );
						rq.SetRequestURI( uri );
						request.SetStartLine( rq );
					}else
					{
						return ProxySession::RouteReject;
					}
				}
			}
		}
	}
	else if(  ( request.IsCancel() || request.IsAck() ) )
	{
//PTRACE(1, "### IsCancel || IsAck for CallId: " << request.GetCallId() << "starting with: " << request.AsString());
		targetURI = session.GetDialogPeerAddress();
		targetURI.SetUser( oldURI.GetUser() );
		requestLine.SetRequestURI( targetURI );
		request.SetStartLine( requestLine );

		/// se if we could resolve the resulting request
		PIPSocket::Address newTarget;
		WORD newPort = 0;
		if( SIPTransport::Resolve( request, newTarget, newPort ) )
		{
			if( session.GetTransportManager()->IsLocalAddressAndPort( newTarget, newPort ) )
			{
				/// request resolves to us ....
				session.SetSessionRejectResponse( SIPMessage::Code481_TransactionDoesNotExist );
				return ProxySession::RouteReject;
			} 
		}else
		{
			/// we are unable to resolve this address
			session.SetSessionRejectResponse( SIPMessage::Code481_TransactionDoesNotExist );
			return ProxySession::RouteReject;
		}

	}
	else if( !request.IsRequest() )
	{
//PTRACE(1, "### Not IsRequest CallId: " << request.GetCallId() << "starting with: " << request.AsString());
		OnRecordRouteShift( request );
	}
	else
	{
//PTRACE(1, "### else for CallId: " << request.GetCallId() << "starting with: " << request.AsString());
		return RouteProxyNISTRequest( session, request );
	}

	return ProxySession::RouteBySession;
}

ProxySession::RoutePolicy SBCRoutingHandler::RouteProxyNISTRequest(
	ProxySession & session,
	SIPMessage & request
	)
{
	/// se if we could resolve the resulting request
	PIPSocket::Address newTarget;
	WORD newPort = 0;
	if( SIPTransport::Resolve( request, newTarget, newPort ) )
	{
		if( session.GetTransportManager()->IsLocalAddressAndPort( newTarget, newPort ) )
		{
			/// request resolves to us ....
			session.SetSessionRejectResponse( SIPMessage::Code481_TransactionDoesNotExist );
			return ProxySession::RouteReject;
		}
	}else
	{
		session.SetSessionRejectResponse( SIPMessage::Code404_NotFound );
		return ProxySession::RouteReject;
	}

	return ProxySession::RouteBySession;
}

void SBCRoutingHandler::OnOrphanedMessage(
	SIPMessage & message
	)
{
	if( !message.IsRequest() )
		OnRecordRouteShift( message );
}

BOOL SBCRoutingHandler::RouteLocalRegistration( 
	B2BUAConnection & connection,
	const SIPMessage & invite,
	const SIPMessage & registration
	)
{
	SIPURI targetURI;

	if( !RouteLocalRegistration( targetURI, invite, registration ) )
		return FALSE;

	connection.AddRoute( targetURI );

	return TRUE;
}

BOOL SBCRoutingHandler::RouteLocalRegistration( 
	SIPURI & targetURI,
	const SIPMessage & request,
	const SIPMessage & registration
	)
{
	if( !ExtractTargetURI( targetURI, registration ) )
		return FALSE;

	PTRACE( 1, "Local Route -->> From: " << request.GetFromURI() 
		<< " Target: " << request.GetMethod() << " " << targetURI );

	return TRUE;
}

BOOL SBCRoutingHandler::ExtractTargetURI(
	SIPURI & targetURI,
	const SIPMessage & registration
	)
{
	if( registration.GetContactSize() == 0 )
		return FALSE;

	/// check the via if this is a NATted registration
	Via via;
	if( registration.GetViaAt( registration.GetViaSize() - 1, via ) 
		&& via.IsBehindNAT()  
		)
	{
		targetURI.SetHost( via.GetReceiveAddress().AsString() );
		targetURI.SetPort( PString( via.GetRPort() ) );
	}else
	{
		Contact & contact = registration.GetContactAt( 0 );
		ContactURI contactURI;
		contact.GetURI( contactURI );
		targetURI = contactURI.GetURI();
	}


	/*
	PIPSocket::Address targetAddress, listenerAddress;
	WORD targetPort = 0, listenerPort = 0;

	if( !SIPTransport::Resolve( targetURI, targetAddress, targetPort ) )
	{
	PTRACE( 1, "(1) Unable to resolve target UA for  " << targetAddress );
	return FALSE;
	}*/

	if( registration.IsXORHashSet() )
		targetURI.AddParameter( "XORHASH", "TRUE" );

	return TRUE;
}

void SBCRoutingHandler::OnRecordRouteShift(
	SIPMessage & msg
	)
{
	if( !msg.IsRequest() )
	{
		PINDEX routeCount = msg.GetRecordRouteSize();
		RecordRouteList oldList = msg.GetRecordRouteList();
		RecordRouteList newList;
		BOOL willReplaceRoutes = FALSE;

		for( PINDEX i = routeCount - 1; i >= 0; i-- )
		{
			RecordRoute recordRoute = msg.GetRecordRouteAt( i );
			RecordRoute newRoute;
			BOOL willReplaceEntry = FALSE;
			for( PINDEX j = 0; j < recordRoute.GetSize(); j++ )
			{
				RouteURI routeURI;
				PString routeShift;
				recordRoute.GetURI( routeURI, j );
				if( routeURI.GetURI().GetParameter( "route-shift", routeShift ) && !routeShift.IsEmpty() )
				{ 
					willReplaceEntry = willReplaceRoutes = TRUE;
					RouteURI ruri;
					SIPURI suri( routeShift );
					ruri.SetURI( suri );
					ruri.SetLooseRouter( TRUE );
					newRoute.AddURI( ruri );
				}else
				{
					newRoute.AddURI( routeURI );
				}
			}
			newList.Append( newRoute );
		}

		if( willReplaceRoutes )
			msg.SetRecordRouteList( newList );
	}
}


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void SBCRoutingHandler::AddPendingInvite(const SIPMessage & i_msgInvite)
{
	PendingInvite		*pPI = 0;

	PWaitAndSignal		lock(m_PILMutex);

	PString				sCallid;

	pPI = new PendingInvite();
	sCallid = i_msgInvite.GetCallId();
#if 0
	pPI->Init(i_msgInvite, sCallid, (m_AppRelayRoutes.GetRoutes().GetSize() + m_RelayRoutes.GetRoutes().GetSize()));
#else
	int		iAppRelayRoutes, iRelayRoutes, iNumTargets;

	iAppRelayRoutes = m_AppRelayRoutes.GetRoutes().GetSize();
	iRelayRoutes = m_RelayRoutes.GetRoutes().GetSize();
	
	// Get the number of targets in the first route, assume all routes have the same number.
	iNumTargets = m_RelayRoutes.GetRoutes()[0].GetTargetURICount();
	
//PTRACE(5, "###### AppRelayRoutes: " << iAppRelayRoutes << ", RelayRoutes: " << iRelayRoutes << ", NumTargets: " << iNumTargets);
	pPI->Init(i_msgInvite, sCallid, iNumTargets);
#endif

	m_PendingInviteList.Append(pPI);
}

/////////////////////////////////////////////////////////////////////////////////////
//	Find and remove previous Invite that has a matching CallId
/////////////////////////////////////////////////////////////////////////////////////
BOOL SBCRoutingHandler::FindAndRemovePendingInviteByCallId(const PString & i_sCallid)
{
	PINDEX		ii, iIndex = -1, iEnd = 0;

	PWaitAndSignal lock(m_PILMutex);

	iEnd = m_PendingInviteList.GetSize();

	// FIX - Linear search time, yuck...  Use a Map instead?
	for(ii = 0; ( (ii < iEnd) && (iIndex == -1) ); ii++)
	{
		if(m_PendingInviteList[ii].m_sCallid == i_sCallid)
		{
PTRACE(2, "#####FindAndRemovePendingInviteByCallId Found callid at index:   " << ii << ".  Total remaining:  " << m_PendingInviteList.GetSize());
			iIndex = ii;
			m_PendingInviteList.RemoveAt(iIndex);
		}
	}

	return(iIndex != -1);
}

/////////////////////////////////////////////////////////////////////////////////////
//	Find previous Invite that has a matching CallId
/////////////////////////////////////////////////////////////////////////////////////
SIPMessage * SBCRoutingHandler::FindPendingInviteByCallId(const PString & i_sCallid)
{
	SIPMessage	*pRet = 0;
	PINDEX		ii, iEnd = 0;
	BOOL		bDone = FALSE;

	PWaitAndSignal lock(m_PILMutex);

	iEnd = m_PendingInviteList.GetSize();
PTRACE(2, "### FindPendingInviteByCallId: " << i_sCallid << ", PendingInvites: " << iEnd);

	// FIX - Linear search time, yuck...  Use a Map instead?
	for(ii = 0; ( (ii < iEnd) && (bDone == FALSE) ); ii++)
	{
		if(m_PendingInviteList[ii].m_sCallid == i_sCallid)
		{
			if(m_PendingInviteList[ii].m_iRemainingRetries > 0)
			{
				// If we found it, decrement the retries and return it.
				pRet = m_PendingInviteList[ii].m_pmInvite;
				m_PendingInviteList[ii].m_iRemainingRetries--;
			}
			else
			{
				PTRACE(2, "### FoundPendingInviteByCallId: " << i_sCallid);
				// If we've run out of retries, remove it from the list and return.
				m_PendingInviteList.RemoveAt(ii);
			}
			bDone = TRUE;
		}
	}

	return(pRet);
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
PendingInvite::PendingInvite()
{
	m_pmInvite = 0;
	Clear();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
PendingInvite::~PendingInvite()
{
	Clear();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void PendingInvite::Clear()
{
	if(m_pmInvite != 0)
	{
		delete(m_pmInvite);
		m_pmInvite = 0;
	}
	m_sCallid = "";
	m_iRemainingRetries = 0;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void PendingInvite::Init(const SIPMessage & i_msgInvite, PString & i_sCallid, int i_iRemainingRetries)
{
	m_pmInvite = new SIPMessage(i_msgInvite.AsString());
	m_sCallid = i_sCallid;
	m_iRemainingRetries = i_iRemainingRetries;
}