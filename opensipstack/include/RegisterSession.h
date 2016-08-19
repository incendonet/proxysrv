/*
 *
 * RegisterSession.h
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
 * $Log: RegisterSession.h,v $
 * Revision 1.19  2006/12/31 05:32:23  joegenbaclor
 * Added NAT keep alives to client sessions.
 *
 * Revision 1.18  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.17  2006/10/30 02:42:26  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.16  2006/10/23 20:22:28  joegenbaclor
 * Modified Registration Methods to include a reference to the Register request
 *
 * Revision 1.15  2006/08/24 10:19:10  joegenbaclor
 * 1.  Couple of bug fixes in SIP Intitializarion and Disconnect cause detection
 * 2.  Added ProxyAuthentication call backs to Register Session
 *
 * Revision 1.14  2006/08/15 15:48:14  joegenbaclor
 * Removed unneeded LOG in register session
 *
 * Revision 1.13  2006/06/08 04:54:20  joegenbaclor
 * !.  Added OnRegistrationTimeout Call back to Registrar classes
 * 2. Moved ECCIAPI to Tools namespace
 *
 * Revision 1.12  2006/04/29 03:22:54  joegenbaclor
 * 1.  More Refer support code added to call session
 * 2.  Added StoreRegistrationInfo callback to Registrar classes
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
 * Revision 1.9  2006/03/21 14:11:50  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.8  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.7  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef REGISTERSESSION_H
#define REGISTERSESSION_H

#include "SIPSession.h"
#include "SIPStack.h"
#include "ProfileUA.h"
#include "RFC3265Subscription.h"

using namespace RFC3265;
using namespace SIP;
using namespace Profiles;




namespace UACORE
{
  class RegisterSessionManager;

  class RegisterSession : public SIPSession
  {
    PCLASSINFO( RegisterSession, SIPSession );
  public:

    enum AcceptRegistrationResponse
    {
      AcceptByOk,
      AcceptByChallenge,
      AcceptByRejection
    };

    enum SessionEvent
    {
      NewRegistration,
      RegistrationRequest,
      RegistrationExpire,
      Unregister,
      RFC3680NewSubscription,
      RFC3680SubscriptionRequest,
      RFC3680SubscriptionExpire,
      RFC3680Unsubscribe,
      RFC3680Notification
    };


    /// Client REGISTRATION, Make use of profiles
    RegisterSession(
      RegisterSessionManager & sessionManager,
      const ProfileUA & profile,
      const PString & sessionId
    );

    /// Server registration, does NOT use profiles
    RegisterSession(
      RegisterSessionManager & sessionManager,
      const SIPMessage & request,
      const PString & sessionId
    );

    virtual ~RegisterSession();

    virtual void OnIncomingSIPMessage(
      SIPMessageArrival & messageEvent
    );

    virtual void OnSessionEvent(
      int event,
      const SIPMessage & eventMsg
    );

    virtual void OnDestroySession();

    void SendRegister();

    void SendUnregister();

    void SendAcceptByChallenge( 
      const SIPMessage & request 
    );

    void SendAcceptRegistration( 
      const SIPMessage & request 
    );

    BOOL ConstructRegister(
      SIPMessage & request
    );

    BOOL ConstructAuthorization(
      const SIPMessage & registerRequest,
      const SIPMessage & challengeResponse,
      SIPMessage & wwwAuthenticate
    );

    BOOL ConstructAuthentication(
      const SIPMessage & registerRequest,
      const SIPMessage & challengeResponse,
      SIPMessage & wwwAuthenticate
    );

    BOOL SetExpireFromRequest(
      const SIPMessage & registerRequest
    );

    virtual void OnHandleResponse(
      const SIPMessage & response
    );

    /// server callbacks
    virtual BOOL OnHandleRequest( 
      const SIPMessage & request 
    );

	virtual BOOL OnNewRegistration( 
		const SIPMessage & request 
	);

    virtual void OnUnregistration( const SIPMessage & request );

    void OnTimerExpire(
      SIPTimerExpire & /*timerEvent*/
    );

    AcceptRegistrationResponse OnAcceptRegistration( 
      const SIPMessage & request
    );

    /// this will be called before calling OnRegistrationAccepted
    /// Returning FALSE here will result to a rejection of the REGISTER
    /// request due to internal server error.  Default behavior
    /// calls RegisterSessionManager::OnStoreRegistrationInfo()
    virtual BOOL OnStoreRegistrationInfo( 
      const SIPURI & accountInfo
    );

    virtual void OnRegistrationAccepted( const SIPMessage & request );

    virtual void OnRegistrationRejected( const SIPMessage & request );

    virtual void OnRegistrationTimeout();

    virtual void OnFailure(
      const SIPMessage & response
    );

    virtual void OnSuccessful(
      const SIPMessage & response
    );

    virtual void OnAuthenticate(
      const SIPMessage & response
    );

    virtual void Cleanup();

    virtual void OnRFC3680NewSubscription( 
      const SIPMessage & request 
    );

    virtual void OnRFC3680SubscriptionRequest(
      const SIPMessage & request
    );

    virtual void OnRFC3680SubscriptionExpire();
    
    virtual void OnRFC3680Unsubscribe(
      const SIPMessage & request
    );
    
    virtual void OnRFC3680Notification(
      const SIPMessage & request
    );

    PDECLARE_NOTIFIER( PTimer, RegisterSession, SendRegistrationRefresh );
    PDECLARE_NOTIFIER( PTimer, RegisterSession, OnRegistrationExpire );

    PINLINE BOOL IsRegistered()const{ return m_IsRegistered; };
    PINLINE const MD5::Nonce GetNonce()const{ return m_Nonce; };
    PINLINE const MD5::MD5Authorization & GetAuthorization()const{ return m_Authorization; };
    PINLINE void SetContactList( const ContactList & contactList ){ m_ContactList = contactList; };
    PINLINE const ContactList & GetContactList()const{ return m_ContactList; };
    PINLINE void SetToURI( const To & uri ){ m_ToURI = uri; };
    PINLINE const To & GetToURI()const{ return m_ToURI; };
    PINLINE const PTimeInterval & GetExpires()const{ return m_Expires; };
    PINLINE BOOL IsRFC3680Session()const{ return m_IsRFC3680Session; };
    PINLINE void SetRFC3680Subscription( RFC3265Subscription * s ){ m_RFC3680Subscription = s; };
    PINLINE RFC3265Subscription * GetRFC3680Subcription(){ return m_RFC3680Subscription; };
    PDICTIONARY( SortedCollection, PCaselessString, RegisterSession );
    PLIST( Collection, RegisterSession );

  protected:

    PTimer m_ExpireTimer;
    PTimeInterval m_Expires;
    BOOL m_IsRegistered;
    BOOL m_IsBrandNewSession;
    MD5::Nonce m_Nonce;
    MD5::MD5Authorization m_Authorization;
    ContactList m_ContactList;
    To m_ToURI;
    int m_AuthorizationAttempts;
    BOOL m_IsRFC3680Session;
    RFC3265Subscription * m_RFC3680Subscription;
    BOOL m_UseRegisterInKeepAlives;
  };
};

#endif


