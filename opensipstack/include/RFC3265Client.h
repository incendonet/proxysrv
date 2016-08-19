/*
 *
 * RFC3265Client.h
 *
 * Open SIP Stack ( OSS )
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
 * $Log: RFC3265Client.h,v $
 * Revision 1.4  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/11/10 09:53:55  joegenbaclor
 * more implementations
 *
 * Revision 1.2  2006/11/09 10:03:44  joegenbaclor
 * Added code to start expire timer
 *
 * Revision 1.1  2006/11/07 07:48:04  joegenbaclor
 * Initial upload of RFC3265Client Files
 *
 */

#ifndef RFC3265CLIENT_H
#define RFC3265CLEINT_H

#include "RFC3265ClientManager.h"

using namespace UACORE;

namespace RFC3265
{
  class RFC3265Client : public SIPSession
  {
    PCLASSINFO( RFC3265Client, SIPSession );
  public:

    enum SessionEvents
    {
      OnSendSubscriptionEvent,
      OnSendSubscriptionRefreshEvent,
      OnEventNotificationEvent,
      OnAuthenticateSubscriptionEvent,
      OnSubscriptionAcceptedEvent,
      OnSubscriptionRejectedEvent,
      OnSubscribeRedirectionEvent,
      OnRemoteTerminateSubscriptionEvent,
      OnLocalTerminateSubscriptionEvent
    };

    RFC3265Client(
      RFC3265ClientManager & manager,
      const PString & sessionId,
      const RFC3265ClientManager::SubscriptionInfo & profile
    );

    virtual ~RFC3265Client();

    virtual void OnIncomingSIPMessage(
      SIPMessageArrival & messageEvent
    );

    virtual void OnHandleNoneNotifyEvent(
      SIPMessageArrival & noneNotifyEvent
    );

    virtual void OnSubscribeRedirection(
      const SIPMessage & redirect
    );

    virtual void OnSessionEvent(
      int _event,
      const SIPMessage & eventMsg
    );

    virtual BOOL SendSubscription(
      const SIPMessage &  subscribe
    );

    virtual BOOL OnSendSubscription(
      const SIPMessage &  subscribe
    );


    virtual BOOL OnSendSubscriptionRefresh(
      const SIPMessage & subscribe
    );
      
    virtual void OnEventNotification(
      const SIPMessage & notify
    );
      
    virtual void OnAuthenticateSubscription(
      const SIPMessage & subsribe,
      const SIPMessage & authenticator
    );
      
    virtual void OnSubscriptionAccepted(
      const SIPMessage & subscribe,
      const SIPMessage & response
    );

    virtual void OnSubscriptionRejected(
      const SIPMessage & subscribe,
      const SIPMessage & response
    );

    virtual void OnRemoteTerminateSubscription(
      const SIPMessage & notify
    );

    virtual void OnLocalTerminateSubscription(
      const SIPMessage & subscribe
    );


    PINLINE const PTimeInterval GetExpireInterval()const{ return m_ExpireInterval; };
    PINLINE RFC3265ClientManager::SubscriptionInfo & GetSubscriptionProfile(){ return m_SubscriptionProfile; };

  protected:
    PTimeInterval m_ExpireInterval;
    PTimer m_ExpireTimer;
    PDECLARE_NOTIFIER( PTimer, RFC3265Client, OnSendSubscriptionRefresh );
    RFC3265ClientManager::SubscriptionInfo m_SubscriptionProfile;

  };
};

#endif


