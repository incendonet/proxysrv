/*
 *
 * ProxySession.h
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
 * $Log: ProxySession.h,v $
 * Revision 1.15  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.14  2006/07/11 13:59:35  joegenbaclor
 * Completed Proxy functionalities for B2BUA
 *
 * Revision 1.13  2006/05/19 06:30:37  joegenbaclor
 * 1.  Fixed bug in SIPHeaderB where tag may not be parsed properly if no < > enclosure is present
 * 2.  Various enhancements to OpenSBC and proxy session
 *
 * Revision 1.12  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.11  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.10  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.9  2006/03/25 18:40:13  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.8  2006/03/24 15:42:13  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.7  2006/03/22 08:45:52  joegenbaclor
 * More work on simple-proxy and affected classes
 *
 * Revision 1.6  2006/03/14 17:00:00  joegenbaclor
 * Implemented Dialog methods in SIPSession object
 *
 * Revision 1.5  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef PROXYSESSION_H
#define PROXYSESSION_H

#include "SIPSession.h"
#include "SIPStack.h"
#include "ProfileUA.h"

using namespace SIP;
using namespace Profiles;

namespace UACORE
{
  class ProxySessionManager;

  class ProxySession : public SIPSession
  {
    PCLASSINFO( ProxySession, SIPSession );
  public:

    enum SessionEvent
    {
      EventValidateRequest = 1,
      EventCheckRoutePolicy,
      EventProvisionalResponse,
      EventFinalResponse
    };

    enum RoutePolicy
    {
      RouteByRedirection,
      RouteBySession,
      RouteReject,
      RouteRejectByForbidden,
      RouteByAuthentication,
      RouteByAuthorization,
      RouteByLocalAccept,
	  RouteIgnore,
    };

    enum AuthStatus
    {
      Authorized,
      AuthRequired,
      Unauthorized
    };

    ProxySession(
      ProxySessionManager & sessionManager,
      const SIPMessage & request,
      const PString & sessionId
    );

    virtual ~ProxySession();

    /// Overrides from SIP Session
    virtual void OnIncomingSIPMessage(
      SIPMessageArrival & messageEvent
    );

    virtual void OnSessionEvent(
      int event,
      const SIPMessage & eventMsg
    );

    void SendAcceptByChallenge(
      const SIPMessage & message
    );

    void SendAcceptByAuthorization(
      const SIPMessage & message
    );

    BOOL ValidateRequest(
      SIPMessage & request
    );

    void OnCheckRouteInformation(
      SIPMessage & request
    );

    virtual BOOL OnCheckURIScheme(
      const SIPMessage & request
    );

    virtual BOOL OnCheckMaxForwards(
      const SIPMessage & request
    );

    virtual BOOL OnCheckLoop(
      const SIPMessage & reques
    );

    virtual BOOL OnCheckProxyRequire(
      const SIPMessage & request
    );

    virtual AuthStatus OnCheckAuthorization(
      const SIPMessage & request
    );

    virtual RoutePolicy OnCheckRoutePolicy(
      SIPMessage & request
    );

    virtual void OnProvisionalResponse( 
      const SIPMessage & message 
    );

    virtual void OnFinalResponse(
      const SIPMessage & message
    );

    virtual void OnProcessRequestAsLocal(
      const SIPMessage & request
    );

    PINLINE const MD5::Nonce & GetNonce()const{ return m_Nonce; };
    PINLINE const RouteURI & GetRecordRouteURI()const{ return m_RecordRouteURI; };
    PINLINE const RecordRouteBranch & GetRecordRouteBranch()const{ return m_RecordRouteBranch; };
    PINLINE BOOL WillCreateDialog()const{ return m_WillCreateDialog; };
    PINLINE void SetCreateDialog( BOOL createDialog = TRUE ){ m_WillCreateDialog = createDialog; };
    PINLINE ContactList & GetRedirectContactList(){ return m_RedirectContactList; };
    PINLINE const PString & GetDialogPeerAddress()const{ return m_DialogPeerAddress; };
    PINLINE void SetDialogPeerAddress( const PString & peer ){ m_DialogPeerAddress = peer; };

    PINLINE BOOL IsInviteSession()const{ return m_IsInviteSession; };
    PINLINE const SIPMessage & GetOriginalInvite()const{ return m_OriginalInvite; };
    PINLINE const SIPMessage & GetRoutedInvite()const{ return m_RoutedInvite; };


  protected:

    BOOL m_WillCreateDialog;
    PString m_DialogPeerAddress;

    MD5::Nonce m_Nonce;
    RouteURI m_RecordRouteURI;
    RecordRouteBranch m_RecordRouteBranch;
    ContactList m_RedirectContactList;

    BOOL m_IsInviteSession;
    SIPMessage m_OriginalInvite;
    SIPMessage m_RoutedInvite;

    PMutex m_IncomingMessageMutex;
  };

};

#endif


