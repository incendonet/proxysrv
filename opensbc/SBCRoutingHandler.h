/*
*
* SBCRoutingHandler.h
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
* Contributor(s): ______________________________________.
*
* $Log: SBCRoutingHandler.h,v $
* Revision 1.6  2007/01/22 10:00:58  joegenbaclor
* Fixed ProxyRouteRequest
*
* Revision 1.5  2007/01/17 00:09:08  joegenbaclor
* Added SysLog
*
* Revision 1.4  2006/11/23 11:24:37  joegenbaclor
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
* Revision 1.4  2006/08/04 05:48:58  rcolobong
* 1.  Add Getter for StaticRoutes
* 2.  Add virtual to method RefreshStaticRoutes
* 3.  RefreshStaticRoutes is not invoke during construction
*
* Revision 1.3  2006/07/11 13:59:34  joegenbaclor
* Completed Proxy functionalities for B2BUA
*
* Revision 1.2  2006/07/03 15:29:46  joegenbaclor
* Started merging OpenB2BUA into OpenSBC
*
* Revision 1.1  2006/06/20 09:58:11  joegenbaclor
* *** empty log message ***
*
*
*/

#ifndef B2BROUTINGHANDLER_H
#define B2BROUTINGHANDLER_H

#include "OpenSBC.h"
#include "Router.h"

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
class PendingInvite : public PObject
{
	PCLASSINFO(PendingInvite, PObject);
public:
	SIPMessage		*m_pmInvite;		// Original invite.
	PString			m_sCallid;		// Copy for fast access.
	int				m_iRemainingRetries;	// Countdown from number of targets.

public:
					PendingInvite();
	virtual			~PendingInvite();

			void	Clear();
			void	Init(const SIPMessage & i_pmInvite, PString & i_sCallid, int i_iRemainingRetries);
};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
class SBCRoutingHandler : public B2BRoutingInterface
{
	PCLASSINFO( SBCRoutingHandler, B2BRoutingInterface );
public:

	SBCRoutingHandler(  
		OpenSBC & b2bua 
		);

	virtual ~SBCRoutingHandler();

	virtual BOOL B2BRouteCall(
		B2BUAConnection & connection,
		const SIPMessage & invite
		);

	virtual BOOL B2BRouteRequest(
		const SIPMessage & request,
		SIPURI  & route
		);

	virtual ProxySession::RoutePolicy RouteProxyRequest(
		ProxySession & session,
		SIPMessage & request
		);

	virtual ProxySession::RoutePolicy RouteProxyNISTRequest(
		ProxySession & session,
		SIPMessage & request
		);

	virtual void OnOrphanedMessage(
		SIPMessage & message
		);

	ProxySession::RoutePolicy RouteProxyInvite(
		ProxySession & session,
		SIPMessage & request,
		const SIPURI & target
		);

	ProxySession::RoutePolicy RouteProxyRegister(
		ProxySession & session,
		SIPMessage & request,
		const SIPURI & target
		);

	BOOL RouteLocalRegistration( 
		B2BUAConnection & connection,
		const SIPMessage & invite,
		const SIPMessage & registration
		);

	BOOL RouteLocalRegistration( 
		SIPURI & route,
		const SIPMessage & request,
		const SIPMessage & registration
		);

	BOOL ExtractTargetURI(
		SIPURI & route,
		const SIPMessage & registration
		);

	void OnRecordRouteShift(
		SIPMessage & msg
		);

	virtual void RefreshStaticRoutes();

	PINLINE OpenSBC & GetSBC(){ return (OpenSBC &)m_B2BUA; };
	PINLINE Router GetStaticRoutes() { return m_StaticRoutes; };
	PINLINE Router GetUpperRegistrationRoutes() { return m_UpperRegistrationRoutes; };
	PINLINE Router GetRelayRoutes() { return m_RelayRoutes; };
	PINLINE Router GetAppStaticRoutes() { return m_AppStaticRoutes; };
	PINLINE Router GetAppUpperRegistrationRoutes() { return m_AppUpperRegistrationRoutes; };
	PINLINE Router GetAppRelayRoutes() { return m_AppRelayRoutes; };
	PINLINE BOOL AppendAppRelayRoutes( const PString & route ){ return m_AppRelayRoutes.AppendRoute( route ); };

	void AddPendingInvite(const SIPMessage & i_msgInvite);
	BOOL FindAndRemovePendingInviteByCallId(const PString & i_sCallid);
	SIPMessage * FindPendingInviteByCallId(const PString & i_sCallid);

private:
	Router m_StaticRoutes;
	Router m_UpperRegistrationRoutes;
	Router m_RelayRoutes;

	Router m_AppStaticRoutes;
	Router m_AppUpperRegistrationRoutes;
	Router m_AppRelayRoutes;

	PLIST(PendingInviteList, PendingInvite);
	PendingInviteList	m_PendingInviteList;
	PMutex				m_PILMutex;
};


#endif


