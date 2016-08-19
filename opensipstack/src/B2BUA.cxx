/*
*
* B2BUserAgent.cxx
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
* $Log: B2BUA.cxx,v $
* Revision 1.43  2007/01/22 10:01:57  joegenbaclor
* Fixed bug in handling Notifies
*
* Revision 1.42  2007/01/17 00:12:19  joegenbaclor
* 1.  Fixed bug in call session where CANCEL may not be sent if a call ahs been authenticated due to a misplaced SetState( StateDisconnected ) for 4xx events
* 2.  Fixed register session to ignore authorization headers if allow all reg is flagged
* 3.  Temporary hacks for syslog
* 4.  More launcher code -  almost there!
*
* Revision 1.41  2007/01/10 08:07:16  rcolobong
* 1. Add new method for OnSendB2BConnect Call Interface.
* 2. OnSendB2BConnect is supported in B2BUserAgent, B2BUACall and B2BUAConnection
*
* Revision 1.40  2007/01/10 04:58:49  joegenbaclor
* Added B2BUpperReg mode.
*
* Revision 1.39  2007/01/08 07:12:58  joegenbaclor
* Added ability to run SBC in pure proxy or pure b2b modes
*
* Revision 1.38  2006/12/22 07:37:20  joegenbaclor
* Introduced separate funtion call  for starting of transport thread.  This is necessary
*  to make sure that the backdoor listener is already initialized prior to calling transport
*  reads
*
* Revision 1.37  2006/12/20 16:24:46  joegenbaclor
* Implemented keep-alives for upper reg.
*
* Revision 1.36  2006/12/17 13:30:58  joegenbaclor
* Added call attempt index in call-id string to avoid call-id collision during failovers
*
* Revision 1.35  2006/12/08 09:52:47  joegenbaclor
* Corrected bug in last commit
*
* Revision 1.34  2006/12/06 23:19:52  joegenbaclor
* Allowed REGISTER to be processed for backdoor requests
*
* Revision 1.33  2006/11/30 08:19:37  joegenbaclor
* Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
*
* Revision 1.32  2006/11/24 08:26:40  joegenbaclor
* Fixed bug in sending Instant messages
*
* Revision 1.31  2006/11/23 11:46:05  joegenbaclor
* Corrected bug in cosntructing B2B instant messages
*
* Revision 1.30  2006/11/23 11:00:12  joegenbaclor
* Added Instant Message B2B support
*
* Revision 1.29  2006/11/22 11:17:24  rcolobong
* 1. Add new Callback method for OnOrphanedMessage in B2BUA
* 2. Add new Handler method for OnOrphanedMessage in B2BRoutingInterface
*
* Revision 1.28  2006/11/21 06:55:19  joegenbaclor
* Added ability to add/append additional listeners.
*
* Revision 1.27  2006/11/18 03:14:28  joegenbaclor
* Implemented session loop detection mechanism using iid via parameter
*
* Revision 1.26  2006/11/17 04:22:21  joegenbaclor
* Added ability to remember core to use via m_UACoreName
*
* Revision 1.25  2006/11/16 01:27:05  rcolobong
* Update method ProcessEvent in handling existing callId during new request
*
* Revision 1.24  2006/11/10 08:16:21  rcolobong
* Modify OnSetupOutbound where it will handle Fork Calls or Not
*
* Revision 1.23  2006/10/30 02:42:27  joegenbaclor
* 1.  Various changes to support RFC 3680.
* 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
*
* Revision 1.22  2006/10/25 08:59:32  rcolobong
* Notify a method in CallHandler during failure in B2BRouteCall
*
* Revision 1.21  2006/10/23 20:22:28  joegenbaclor
* Modified Registration Methods to include a reference to the Register request
*
* Revision 1.20  2006/09/02 05:56:04  joegenbaclor
* Improved ProcessStackEvent Logging
*
* Revision 1.19  2006/09/02 04:38:45  joegenbaclor
* Added new Process Stack Event override to allow a GCRef as an argument
*
* Revision 1.18  2006/08/30 03:42:05  joegenbaclor
* Corrected bug in garbage collector loop.
*
* Revision 1.17  2006/08/29 10:47:23  rcolobong
* *** empty log message ***
*
* Revision 1.16  2006/08/28 07:18:35  joegenbaclor
* Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
*
* Revision 1.15  2006/08/26 14:31:44  joegenbaclor
* Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
*
* Revision 1.14  2006/08/06 02:32:08  joegenbaclor
* Corrected bug regarding blank call-ids for UnknownTransaction events
*
* Revision 1.13  2006/07/19 02:19:21  joegenbaclor
* 1.  Bug fixes concerning B2BUA destruction during during 3xx, 4xx, 5xx, 6xx local response
* 2.  Fixed bug where Route header is never removed from outbound Invite resulting that may result to a loop when
*      Inbound Invite has a Route header present
* 3.  Added RemoveAllRoutes() in SIPMessage
*
* Revision 1.12  2006/07/17 11:36:45  joegenbaclor
* 1.  More routing enhancements to B2BUA
*
* Revision 1.11  2006/07/11 13:59:36  joegenbaclor
* Completed Proxy functionalities for B2BUA
*
* Revision 1.10  2006/07/10 06:29:38  joegenbaclor
* Completed crude Registration support for B2BUA
*
* Revision 1.9  2006/06/29 07:40:53  joegenbaclor
* *** empty log message ***
*
* Revision 1.8  2006/06/29 05:09:13  joegenbaclor
* Changed OnOutgoingCall invite parameter from const to none const to give a
* chance for applications to modify the outbound invite before being sent to the transport
*
* Revision 1.7  2006/06/28 14:14:59  joegenbaclor
* Bulk commit for Media Proxy functionalities for OpenB2BUA
* Marks a minor version upgrade from 1.0.2 to 1.0.3
*
* Revision 1.6  2006/06/21 14:29:11  joegenbaclor
* This upload marks the first successful call by OpenB2BUA
*
* Revision 1.5  2006/06/20 09:52:55  joegenbaclor
* Added Routing and SDP Handlers to B2BUA
*
* Revision 1.4  2006/06/15 09:47:28  joegenbaclor
* More preliminary design work on B2BUA
*
* Revision 1.3  2006/06/14 08:43:38  joegenbaclor
* Initial upload of OpenB2BUA applcation and related classes
*
* Revision 1.2  2006/04/11 00:18:54  joegenbaclor
* More work on B2BUA class structures
*
* Revision 1.1  2006/04/10 01:07:37  joegenbaclor
* Initial upload
*
*
*/

#include "B2BUA.h"
#include "B2BUAConnection.h"
#include "B2BUAForkCalls.h"

#define new PNEW

using namespace B2BUA;

B2BUserAgent::B2BUserAgent( 
						   UAMode uaMode 
						   )
{
	m_CallHandler = NULL;
	m_AuthHandler = NULL;
	m_RoutingHandler = NULL;
	m_MediaHandler = NULL;
	m_EndPoint = NULL;
	m_Registrar = NULL;
	m_Proxy = NULL;
	m_BackDoorPort = 5080;
	m_UAMode = uaMode;
}

B2BUserAgent::~B2BUserAgent()
{
	if( m_CallHandler != NULL )
		delete m_CallHandler;

	if( m_AuthHandler != NULL )
		delete m_AuthHandler;

	if( m_RoutingHandler != NULL )
		delete m_RoutingHandler;

	if( m_MediaHandler != NULL )
		delete m_MediaHandler;
}

BOOL B2BUserAgent::Initialize(
							  PINDEX threadCount
							  )
{
	if( m_CallHandler == NULL )
		m_CallHandler = OnCreateCallInterface();

	if( m_AuthHandler == NULL )
		m_AuthHandler = OnCreateAuthInterface();

	if( m_RoutingHandler == NULL )
		m_RoutingHandler = OnCreateRoutingInterface();

	if( m_MediaHandler == NULL )
		m_MediaHandler = OnCreateMediaInterface();

	ProfileTransport & transportProfile = GetDefaultProfile().GetTransportProfile();
	PIPSocket::Address listenerAddress = transportProfile.GetUDPListenerAddress();
	WORD listenerPort = transportProfile.GetUDPListenerPort();

	BOOL ok = SIPUserAgent::Initialize( threadCount );

	if( ok && m_UAMode == FullMode )
	{
		/// open the backdoor port
		m_BackDoorPort = listenerPort + 20;

		if( transportProfile.IsUDPEnabled() )
		{
			m_SIPStack.AppendTransport( 
				SIPTransport::UDP, 
				listenerAddress, 
				m_BackDoorPort );
		}

		if( transportProfile.IsTCPEnabled() )
		{
			m_SIPStack.AddTransport( 
				SIPTransport::TCP, 
				listenerAddress, 
				m_BackDoorPort );
		}

		if( transportProfile.IsTLSEnabled() )
		{
			m_SIPStack.AddTransport( 
				SIPTransport::TLS, 
				listenerAddress, 
				m_BackDoorPort );
		}
	}

	return StartTransportThreads();
}

B2BCallInterface * B2BUserAgent::OnCreateCallInterface()
{
	return new B2BCallInterface( *this );
}

B2BAuthInterface * B2BUserAgent::OnCreateAuthInterface()
{
	return new B2BAuthInterface( *this );
}

B2BMediaInterface * B2BUserAgent::OnCreateMediaInterface()
{
	return new B2BMediaInterface( *this );
}

B2BRoutingInterface * B2BUserAgent::OnCreateRoutingInterface()
{
	return new B2BRoutingInterface( *this );
}

/// SIPUserAgent virtual func override
void B2BUserAgent::ProcessEvent(
								SIPStackEvent * event
								)
{
	TransactionId transactionId = event->GetTransactionId();
	PString method = transactionId.GetMethod().ToUpper();
	PString callId = transactionId.GetCallId();
	PString coreName = event->GetUACoreName();
	To to;
	PString domain;
	SIPMessage msg;

	if( event->GetType() == SIPStackEvent::Message )
	{
		SIPMessageArrival & msgEvent = (SIPMessageArrival&)*event;
		msg = msgEvent.GetMessage();
		msg.GetTo( to );
		domain = to.GetURI().GetHost();
		LOG( LogDebug(), "Event: B2BUserAgent::ProcessEvent( " << msg.GetStartLine() << " )" );
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent( " << msg.GetStartLine() << " )" );
	}

	if( m_UAMode == ProxyOnlyMode )
	{
		if( method != "REGISTER" )  ///  we need to delay evaluation because registration maybe local
			coreName = m_Proxy->GetUACoreName();
	}else if( m_UAMode == B2BOnlyMode  )
	{
		if( method != "REGISTER" )
			coreName = m_EndPoint->GetUACoreName();
		else
			coreName = m_Registrar->GetUACoreName();
	}else if( m_UAMode == B2BUpperRegMode  )
	{
		if( method != "REGISTER" )
		{
			coreName = m_EndPoint->GetUACoreName();
		}else
		{
			///check if this is a local registration
			if( SIPTransport::IsLocalAddress( to.GetURI() ) )
			{
				/// local registration handle it
				coreName = m_Registrar->GetUACoreName();
			}else
			{
				/// Upper reg.  Send it to the proxy
				if( m_UAMode != B2BOnlyMode )
					coreName = m_Proxy->GetUACoreName();  
			}
		}

	}else if( event->GetType() == SIPStackEvent::Message )
	{
		/// force backdoor calls as B2B
		if( msg.GetInterfacePort() == GetBackDoorPort() && method != "REGISTER"  )
			coreName = m_EndPoint->GetUACoreName();
	}

//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), ZERO" );
	/// check if the coreName is already set for this event
	if( !coreName.IsEmpty() )
	{
		LOG( LogInfo(), "Event: Setting UA Core [" << coreName << "] to handle event " << method );

		if( m_Registrar->GetUACoreName() == coreName )
		{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), ZERO - Registrar" );
			m_Registrar->ProcessStackEvent( event );
			return;
		}else if( m_Proxy->GetUACoreName() == coreName )
		{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), ZERO - Proxy" );
			m_Proxy->ProcessStackEvent( event );
			return;
		}else if( m_EndPoint->GetUACoreName() == coreName )
		{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), ZERO - EndPoint" );
			m_EndPoint->ProcessStackEvent( event );
			return;
		}
	}
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), ONE" );

	if( method == "REGISTER" )
	{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), method == register" );
		if( event->GetType() == SIPStackEvent::Message )
		{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), is SIPStackEvent::Message" );
			if( msg.IsRegister() )
			{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), IsRegister" );

				///check if this is a local registration
				if( SIPTransport::IsLocalAddress( to.GetURI() ) )
				{
					/// local registration handle it
					m_Registrar->ProcessStackEvent( event );
				}else
				{
					/// must be a relay.  Send it to the proxy
					if( m_UAMode != B2BOnlyMode )
						m_Proxy->ProcessStackEvent( event );
				}
			}else  /// Response to REGISTER.  check if we already have a session for this
			{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), restponse to Register" );
				SIPSession::GCRef autoRef = m_Registrar->FindGCRefByCallId( callId );
				if( autoRef != NULL )
				{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), existing session - m_Registrar->ProcessStackEvent" );
					/// handle it theres an existing session
					m_Registrar->ProcessStackEvent( autoRef, event );
				}else
				{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), autoRef==NULL" );
					if( m_UAMode != B2BOnlyMode )
						m_Proxy->ProcessStackEvent( event );
				}
			}

		}
		else  /// Not a SIPMessage event
		{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), not SIPMessage event" );
			SIPSession::GCRef autoRef =  m_Registrar->FindGCRefByCallId( callId );
			if( autoRef != NULL )
			{
				/// handle it theres an existing session
				m_Registrar->ProcessStackEvent( autoRef, event );
			}else
			{
				if( m_UAMode != B2BOnlyMode )
					m_Proxy->ProcessStackEvent( event );
			}
		}
	}else /// None REGISTER request
	{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), not register" );
		if( event->GetType() == SIPStackEvent::Message )
		{
			if( msg.IsRequest() )
			{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), IsRequest" );

				///check if this is a meant for local handling
				if( SIPTransport::IsLocalAddress( to.GetURI() ) || msg.IsXORHashSet()  )
				{
					/// local
					/// let the endpoint handle it
					SIPSession::GCRef autoRef = m_EndPoint->FindGCRefByCallId( callId ); 
					if( autoRef != NULL )
					{
						/// check if the requests is looping
						if( autoRef->IsRequestLooping( msg ) )
							m_Proxy->ProcessStackEvent( event );
						else
							m_EndPoint->ProcessStackEvent( autoRef, event );
					}else
					{
						m_EndPoint->ProcessStackEvent( event );
					}
				}else
				{
					SIPSession::GCRef autoRef = m_EndPoint->FindGCRefByCallId( callId ); 
					if( autoRef != NULL )
					{
						if( autoRef->IsRequestLooping( msg ) )
							m_Proxy->ProcessStackEvent( event );
						else
							m_EndPoint->ProcessStackEvent( autoRef, event );
					} else
					{
						/// must be a relay.  Send it to the proxy
						m_Proxy->ProcessStackEvent( event );
					}
				}
			}else  /// not a request.  check if we already have a session for this
			{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), NOT IsRequest" );
				SIPSession::GCRef autoRef = m_EndPoint->FindGCRefByCallId( callId ); 
				if( autoRef != NULL )
				{
					/// handle it if theres an existing session
					m_EndPoint->ProcessStackEvent( autoRef, event );
				}else
				{
					autoRef = m_Proxy->FindGCRefByCallId( callId );
					if( autoRef != NULL )
					{
						m_Proxy->ProcessStackEvent( autoRef, event );
					}else
					{
						/// retransmission.  just delete it.
						delete event;
					}
				}
			}
		}else  /// Not a SIPMessage event
		{
//PTRACE(5, "#####Event: B2BUserAgent::ProcessEvent(), not a SIPMessage event" );
			SIPSession::GCRef autoRef = m_EndPoint->FindGCRefByCallId( callId );
			if( autoRef != NULL )
			{
				/// handle it theres an existing session
				m_EndPoint->ProcessStackEvent( event );
			}else
			{
				m_Proxy->ProcessStackEvent( event );
			}
		}
	}
}

/// Signaling Callbacks

BOOL  B2BUserAgent::OnIncomingCall(
								   B2BUAConnection & connection,
								   B2BUACall & call,
								   const SIPMessage & invite
								   )
{
	ProxyAuthenticate auth;
	SIPMessage authChallenge;
	switch( m_AuthHandler->B2BAuthenticate( connection, invite, auth ) )
	{
	case B2BAuthInterface::AuthAccept:
		break;
	case B2BAuthInterface::AuthReject:
		call.SetCallAnswerResponse( CallSession::DisconnectWithForbidden );
		connection.DestroyConnection( invite );
		return FALSE;

	case B2BAuthInterface::AuthChallenge:
		call.SetProxyAuthenticate( auth );
		call.SetCallAnswerResponse( CallSession::DisconnectWithUnauthorized );
		connection.DestroyConnection( invite );

		return FALSE;
	default:
		break;
	}

	m_CallHandler->OnIncomingCall( connection, call, invite );

	if( !m_RoutingHandler->B2BRouteCall( connection, invite ) 
		|| connection.GetRoutes().GetSize() == 0 )
	{
		call.SetCallAnswerResponse( CallSession::DisconnectWithNotFound );

		SIPMessage cancel;
		invite.CreateResponse( cancel, SIPMessage::Code404_NotFound ); 
		m_CallHandler->OnCallAbandoned( connection, call, cancel );

		connection.DestroyConnection( invite );
		return FALSE;
	}

	return TRUE;
}

void B2BUserAgent::OnReceivedInstantMessage(
	const SIPMessage & message
	)
{
	SIPURI uri;
	if( m_RoutingHandler->B2BRouteRequest( message, uri ) )
	{


		PIPSocket::Address targetAddress;
		WORD targetPort;

		if( !SIPTransport::Resolve( uri, targetAddress, targetPort ) )
			return;

		SIPMessage msg = message;

		msg.RemoveAllRoutes();

		BOOL willUseProxy = FALSE;

		/// create the request line
		RequestLine requestLine;
		requestLine.SetMethod( "MESSAGE" );
		willUseProxy = !m_DefaultProfile.GetProxyProfile().GetServerAddress().IsEmpty();
		requestLine.SetRequestURI( uri );
		msg.SetStartLine( requestLine );

		/// set the to uri;

		if( willUseProxy )
		{
			/// put the proxy as the static route
			SIPURI sendAddress( m_DefaultProfile.GetProxyProfile().GetServerAddress() );
			msg.SetSendAddress( sendAddress );
		}

		PString transport;
		if( !uri.GetParameter( "transport", transport ) )
			transport = "udp";


		Via via;
		m_EndPoint->ConstructVia( targetAddress, via, SIPTransport::UDP, message.GetInterfacePort() );
		via.SetBranch( ParserTools::GenBranchParameter() );
		via.AddParameter( "rport", "" );
		msg.RemoveAllVias();
		msg.AppendVia( via );

		/// Set the contact
		SIPURI contactURI;
		contactURI.SetHost( via.GetURI().GetHost() );
		contactURI.SetPort( via.GetURI().GetPort() );
		ContactURI contact( contactURI, m_DefaultProfile.GetDisplayName() );
		msg.RemoveAllContact();
		msg.AppendContact( contact );

		/// check if the route is to use an XOR Hash
		if( uri.GetParameter( "XORHASH" ) == "TRUE" )
		{
			msg.SetXORHash( TRUE );
		}

		msg.RemoveAllRecordRoutes();

		TransactionId tid;
		SendRequest( msg, tid );
	}
}

void B2BUserAgent::OnSetupOutbound(
								   const SIPMessage & invite,
								   B2BUAConnection & connection
								   )
{
	SIPMessage outboundInvite = invite;
	if( connection.IsForkCalls() )
	{
		B2BUAForkCalls * forkCalls = connection.GetForkCalls();
		if( forkCalls )
			forkCalls->CreateForkCalls( outboundInvite );
	} else
	{
		PString callId = invite.GetCallId() + "-" + PString( ++connection.m_OutboundAttempts ) + PString( "-outbound" );
		ProfileUA defaultProfile = GetDefaultProfile();
		B2BUACall * call = (B2BUACall*)m_EndPoint->CreateClientSession( defaultProfile, callId );
		SIPSession::GCRef autoRef = call->GCCreateRef( "B2BUserAgent::OnSetupOutbound" );;
		connection.AttachCall( callId, call );
		m_CallHandler->OnOutgoingCall( connection, *call, outboundInvite );
		call->OnSetupOutbound( outboundInvite, connection );
	}
}

BOOL B2BUserAgent::OnIncomingSDPOffer(
									  B2BUAConnection & connection,
									  B2BUACall & call,
									  const SIPMessage & sdp
									  )
{
	if( sdp.GetBody().IsEmpty() )
		return FALSE;

	return m_MediaHandler->OnIncomingSDPOffer( connection, call, sdp );
}

BOOL B2BUserAgent::OnRequireSDPOffer(
									 B2BUAConnection & connection,
									 B2BUACall & call,
									 SIPMessage & offer
									 )
{
	return m_MediaHandler->OnRequireSDPOffer( connection, call, offer );
}

BOOL B2BUserAgent::OnRequireSDPAnswer(
									  B2BUAConnection & connection,
									  B2BUACall & call,
									  const SIPMessage & offer,
									  SIPMessage & answer
									  )
{
	return m_MediaHandler->OnRequireSDPAnswer( connection, call, offer, answer );
}

BOOL  B2BUserAgent::OnOutgoingCall(
								   B2BUAConnection & connection,
								   B2BUACall & call,
								   SIPMessage & invite
								   )
{
	m_CallHandler->OnOutgoingCall( connection, call, invite );
	return TRUE;
}

BOOL  B2BUserAgent::OnCallReinvite(
								   B2BUAConnection & connection,
								   B2BUACall & call,
								   const SIPMessage & invite
								   )
{
	m_CallHandler->OnCallReinvite( connection, call, invite );
	return TRUE;
}

BOOL  B2BUserAgent::OnProceeding(
								 B2BUAConnection & connection,
								 B2BUACall & outboundCall,
								 const SIPMessage & proceeding
								 )
{
	m_CallHandler->OnProceeding( connection, outboundCall, proceeding );
	return TRUE;
}

BOOL  B2BUserAgent::OnCallAbandoned(
									B2BUAConnection & connection,
									B2BUACall & inboundCall,
									const SIPMessage & cancel
									)
{
	m_CallHandler->OnCallAbandoned( connection, inboundCall, cancel );
	return TRUE; 
}

BOOL  B2BUserAgent::OnAlerting(
							   B2BUAConnection & connection,
							   B2BUACall & outboundCall,
							   const SIPMessage & alerting
							   )
{
	m_CallHandler->OnAlerting( connection, outboundCall, alerting );
	return TRUE;
}

BOOL  B2BUserAgent::OnProgress(
							   B2BUAConnection & connection,
							   B2BUACall & outboundCall,
							   const SIPMessage & progress
							   )
{
	m_CallHandler->OnProgress( connection, outboundCall, progress );
	return TRUE;
}

BOOL  B2BUserAgent::OnRejected(
							   B2BUAConnection & connection,
							   B2BUACall & outboundCall,
							   const SIPMessage & reject
							   )
{
	m_CallHandler->OnRejected( connection, outboundCall, reject );
	return TRUE;
}

BOOL  B2BUserAgent::OnConnected(
								B2BUAConnection & connection,
								B2BUACall & outboundCall,
								const SIPMessage & connect
								)
{
	m_CallHandler->OnConnected( connection, outboundCall, connect );
	return TRUE;
}

BOOL B2BUserAgent::OnSendB2BConnect(
									B2BUAConnection & connection,
									B2BUACall & outboundCall,
									const SIPMessage & connect
									)
{
	m_CallHandler->OnSendB2BConnect( connection, outboundCall, connect );
	return TRUE;
}

BOOL  B2BUserAgent::OnDisconnected(
								   B2BUAConnection & connection,
								   B2BUACall & outboundCall,
								   const SIPMessage & bye
								   )
{
	m_CallHandler->OnDisconnected( connection, outboundCall, bye );
	return TRUE;
}


BOOL  B2BUserAgent::OnInSessionMessage(
									   B2BUAConnection & connection,
									   B2BUACall & remoteCall,
									   const SIPMessage & request
									   )
{
	m_CallHandler->OnInSessionMessage( connection, remoteCall, request );
	return TRUE;
}


void B2BUserAgent::OnRequireRedirectionId(
	B2BUAConnection & /*connection*/,
	B2BUACall & /*call*/,
	PString & redirectionId
	)
{
	redirectionId = "oss";
}

void B2BUserAgent::OnRequireCallTransferId(
	B2BUAConnection & /*connection*/,
	B2BUACall & /*call*/,
	PString & redirectionId
	)
{
	redirectionId = "oss";
}

RegisterSession::AcceptRegistrationResponse 
B2BUserAgent::OnAcceptRegistration( 
								   RegisterSession & session,
								   const SIPMessage & request
								   )
{
	return RegisterSession::AcceptByChallenge;
}

/// pure virtuals from SIPRegistrar
BOOL B2BUserAgent::OnRequireAuthorizationHash(
	const SIPURI & userURL,
	PString & a1,
	RegisterSession & session 
	)
{
	return m_AuthHandler->RequireAuthorizationHash( userURL, a1, session );
}

BOOL B2BUserAgent::OnStoreRegistrationInfo(
	RegisterSession& session,
	const SIPURI& accountInfo
	)
{
	return TRUE;
}

BOOL B2BUserAgent::OnStoreUpperRegistrationInfo(
	ProxySession & session,
	const SIPMessage & message
	)
{
	return m_Registrar->OnStoreUpperRegistrationInfo( session, message );
}


void B2BUserAgent::OnRegistrationAccepted(
	RegisterSession & session,
	const SIPMessage & request
	)
{
}

void B2BUserAgent::OnRegistrationRejected(
	RegisterSession & session,
	const SIPMessage & request
	)
{
}

void B2BUserAgent::OnUnregistration(
									RegisterSession & session,
									const SIPMessage & request
									)
{
}





///////////////////////////////////////////////////////////////////////

class UpperRegKeepAliveThread : public PThread
{
	PCLASSINFO( UpperRegKeepAliveThread, PThread );
public:
	UpperRegKeepAliveThread( 
		B2BUserAgent::Registrar & registrar 
		) : PThread( 1024 * 2, NoAutoDeleteThread, NormalPriority, "UpperRegKeepAliveThread" ),
		m_Registrar( registrar )
	{
		Resume();
	}

	void Main()
	{
		m_Registrar.ProcessUpperRegKeepAlive();
	}

	B2BUserAgent::Registrar & m_Registrar;

};

B2BUserAgent::Registrar::Registrar(
								   B2BUserAgent & ua,
								   PINDEX sessionThreadCount = 5,
								   PINDEX stackSize = 1024 * 2
								   ) : RegisterSessionManager( ua, sessionThreadCount, stackSize )
{
	m_UpperRegKeepAliveThread = new UpperRegKeepAliveThread( *this );
}

B2BUserAgent::Registrar::~Registrar()
{
	m_UpperRegSync.Signal();
	PAssert( m_UpperRegKeepAliveThread->WaitForTermination( 5000 ), "UpperRegKeepAliveThread failed to terminate!" );
	delete m_UpperRegKeepAliveThread;
}

RegisterSession::AcceptRegistrationResponse B2BUserAgent::Registrar::OnAcceptRegistration( 
	RegisterSession & session,
	const SIPMessage & request
	)
{
	B2BUserAgent & b2bua = (B2BUserAgent &)GetUserAgent();
	return b2bua.OnAcceptRegistration( session, request );
}

/// pure virtuals from SIPRegistrar
BOOL B2BUserAgent::Registrar::OnRequestA1Hash(
	const SIPURI & userURL,
	PString & a1,
	RegisterSession & session 
	)
{
	B2BUserAgent & b2bua = (B2BUserAgent &)GetUserAgent();
	return b2bua.OnRequireAuthorizationHash( userURL, a1, session );
}

BOOL B2BUserAgent::Registrar::IsKnownUser(
	const SIPURI & usrURI
	)const
{
	B2BUserAgent & b2bua = dynamic_cast<B2BUserAgent &>(const_cast<B2BUserAgent::Registrar*>(this)->GetUserAgent());
	return b2bua.IsKnownUser( usrURI );
}

BOOL B2BUserAgent::IsKnownUser(
							   const SIPURI & usrURI
							   )const
{
	PAssertAlways( PUnimplementedFunction );
	return FALSE;
}


BOOL B2BUserAgent::Registrar::OnStoreRegistrationInfo(
	RegisterSession& session,
	const SIPURI& accountInfo
	)
{
	B2BUserAgent & b2bua = (B2BUserAgent &)GetUserAgent();
	return b2bua.OnStoreRegistrationInfo( session, accountInfo );
}

BOOL B2BUserAgent::Registrar::OnStoreUpperRegistrationInfo(
	ProxySession & session,
	const SIPMessage & message
	)
{
	SIPMessage reg = session.GetRequest();

	/// hijack the registration
	PString userName;
	PString realm;
	To to;
	message.GetTo( to );
	SIPURI uri = to.GetURI();
	SIPURI contact = reg.GetTopContactURI();
	int expires = 3600;


	Authorization auth;
	if( reg.GetAuthorization( auth ) )
	{
		auth.GetParameter( "username", userName );
		auth.GetParameter( "realm", realm );
	}

	ParserTools::UnQuote(userName);
	ParserTools::UnQuote(realm);

	if( message.HasExpires() )
	{
		Expires h;
		message.GetExpires( h );
		expires = h.AsInt64();
	}else
	{
		if( contact.FindParameter( "expires" ) != P_MAX_INDEX )
		{
			PString temp;
			if( contact.GetParameter( "expires", temp ) && !temp.IsEmpty() )
				expires = temp.AsInteger();
		}
	}

	PStringStream account;
	if( !userName.IsEmpty() && !realm.IsEmpty() )
		account << "sip:" << userName << "@" << realm;
	else
		account << "sip:" << uri.GetUser() << "@" << uri.GetHost();

	PString c = contact.AsString();

	if( !reg.IsUnregister() )
	{
		PString upperReg = session.GetInternalHeader( "enable-upper-registration" );
		if( !upperReg.IsEmpty() )
		{

			uri.AddParameter( "expires", expires, TRUE );
			uri.AddParameter( "status", "online" );

			PString t = uri.AsString();

			PTRACE( 1, "*** UPPER REGISTRATION ***" );
			PTRACE( 1,  "User: " << account  );
			PTRACE( 1,  "Registration: " << reg.GetTopContactURI()  );
			PTRACE( 1,  "Upper Registration: " << upperReg  );
			PTRACE( 1,  to );

			/// add the modified URI to the DB
			SIPHeader h( "upper-reg", "TRUE" );
			reg.AddInternalHeader( h );
			GetRegistrationDB().AddRegistration( upperReg, reg, expires );

			/// add the original URI to the DB
			GetRegistrationDB().AddRegistration( uri, reg, expires );
		}
	}else
	{
		GetRegistrationDB().RemoveRegistration( uri );

		PString upperReg = session.GetInternalHeader( "enable-upper-registration" );
		if( !upperReg.IsEmpty() )
			GetRegistrationDB().RemoveRegistration( upperReg );


		// default expiration to 0, to force expiration 
		uri.AddParameter( "expires", /*expires*/ "0", TRUE );
		uri.AddParameter( "status", "offline" );
		PString t = uri.AsString();

		PTRACE( 1, "Unregister \"" << t << "\"");

#if ENABLE_SOLEGY_PAM
		SolegyPAM::UpdatePAMIdentityPresenceState( t, c, TRUE );
		SolegyPAM::UpdatePAMAgentPresenceState( t, c, TRUE );
#endif
	}

	return TRUE;
}

void B2BUserAgent::Registrar::ProcessUpperRegKeepAlive()
{
	while( !m_UpperRegSync.Wait( 30000 ) )
	{
		///process the keep alives here
		for( PINDEX i = 0; i < GetRegistrationDB().GetSize(); i++ )
		{
			SIPMessage reg;
			if( GetRegistrationDB().GetRegistration( i, reg ) )
			{
				if( reg.HasInternalHeader( "upper-reg" ) )
				{
					/// this is an upper reg, send a keep-alive
					/// Check the last via if its from a private IP
					Via via;
					if( reg.GetViaAt( reg.GetViaSize() - 1, via ) )
					{
						if( via.IsBehindNAT() )
						{
							SIPURI target;
							target.SetHost( via.GetReceiveAddress().AsString() );
							target.SetPort( via.GetRPort() );
							SIPMessage keepAlive;
							RequestLine requestLine;
							requestLine.SetMethod( "KEEP-ALIVE" );
							requestLine.SetRequestURI( target );
							keepAlive.SetStartLine( requestLine );
							GetUserAgent().SendRequest( keepAlive );
						}
					}
				}
			}
		}
	}
}

void B2BUserAgent::Registrar::OnRegistrationAccepted(
	RegisterSession & session,
	const SIPMessage & request
	)
{
	B2BUserAgent & b2bua = (B2BUserAgent &)GetUserAgent();
	b2bua.OnRegistrationAccepted( session, request );

	SIPMessage reg = session.GetRequest();
	PString c = reg.GetContactTopURI().AsString(); 
	PString expires = PString( session.GetExpires().GetSeconds() );

	To to;
	reg.GetTo(to);
	SIPURI toURI = to.GetURI();
	PString t = toURI.AsString();

	LOG_IF_DEBUG( LogDetail(), "Registration accepted for " << t
		<< " contact=\"" << c 
		<< "\" expires=" << expires
		);
}

void B2BUserAgent::Registrar::OnRegistrationRejected(
	RegisterSession & session,
	const SIPMessage & request
	)
{
	B2BUserAgent & b2bua = (B2BUserAgent &)GetUserAgent();
	b2bua.OnRegistrationRejected( session, request );

	SIPMessage reg = session.GetRequest();
	To to;
	reg.GetTo(to);
	SIPURI toURI = to.GetURI();
	PString t = toURI.AsString();
	PTRACE( 1, "Registration rejected for " << t);
	LOG_IF_DEBUG( LogDetail(), "Registration rejected for \"" 
		<< t 
		<< "\"" );
}

void B2BUserAgent::Registrar::OnUnregistration(
	RegisterSession & session,
	const SIPMessage & request
	)
{
	B2BUserAgent & b2bua = (B2BUserAgent &)GetUserAgent();
	b2bua.OnUnregistration( session, request );
}


///////////////////////////////////////////////////////////////////


B2BUserAgent::Proxy::Proxy(
						   B2BUserAgent & ua,
						   PINDEX sessionThreadCount,
						   PINDEX stackSize
						   ) : ProxySessionManager( ua, sessionThreadCount, stackSize )
{
}

ProxySession::RoutePolicy B2BUserAgent::RouteProxyRequest(
	ProxySession & session,
	SIPMessage & request
	)
{
	return m_RoutingHandler->RouteProxyRequest( session, request );
}

BOOL B2BUserAgent::FindAndRemovePendingInviteByCallId(
	const PString & i_sCallid
	)
{
	return m_RoutingHandler->FindAndRemovePendingInviteByCallId(i_sCallid);
}

void B2BUserAgent::OnOrphanedMessage(
									 SIPMessage & message
									 )
{
PTRACE(2, "##### OnOrphanedMessage for CallId: " << message.GetCallId() << "starting with: " << message.AsString());
	m_RoutingHandler->OnOrphanedMessage( message );
}

ProxySession::RoutePolicy B2BUserAgent::Proxy::OnCheckRoutePolicy(
	ProxySession & session,
	SIPMessage & request
	) 
{
	B2BUserAgent & b2bua = (B2BUserAgent &)GetUserAgent();
	return b2bua.RouteProxyRequest( session, request );
}


void B2BUserAgent::Proxy::OnFinalResponse(
	ProxySession & session,
	const SIPMessage & request
	)
{
	if( request.Is2xx() )
	{
		if( request.GetCSeqMethod() *= "REGISTER" )
		{
			B2BUserAgent & b2bua = (B2BUserAgent &)GetUserAgent();
			b2bua.OnStoreUpperRegistrationInfo( session, request );
		}
		else if(request.GetCSeqMethod() *= "INVITE")
		{
PTRACE(2, "##### OnFinalResponse-Invite, CallID:  " << request.GetCallId());
			B2BUserAgent & b2bua = (B2BUserAgent &)GetUserAgent();
			b2bua.FindAndRemovePendingInviteByCallId(request.GetCallId());
		}
	}

	ProxySessionManager::OnFinalResponse( session, request );
}

void B2BUserAgent::Proxy::OnOrphanedMessage(
	const SIPMessage & message
	)
{
	B2BUserAgent & b2bua = (B2BUserAgent &)GetUserAgent();

	SIPMessage msg = message;
	b2bua.OnOrphanedMessage( msg );

	ProxySessionManager::OnOrphanedMessage( msg );
}

