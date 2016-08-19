/*
 *
 * RFC3265ClientManager.h
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
 * $Log: RFC3265ClientManager.h,v $
 * Revision 1.4  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/11/24 03:21:16  joegenbaclor
 * Initial upload of RFC3680 Client
 *
 * Revision 1.2  2006/11/10 09:53:55  joegenbaclor
 * more implementations
 *
 * Revision 1.1  2006/11/07 07:48:04  joegenbaclor
 * Initial upload of RFC3265Client Files
 *
 */

#ifndef RFC3265CLIENTMANAGER_H
#define RFC3265CLIENTMANAGER_H

#include "SIPSessionManager.h"
using namespace UACORE;


namespace RFC3265
{
  class RFC3265Client;
  class RFC3265ClientManager : public SIPSessionManager
  {
    PCLASSINFO( RFC3265ClientManager, PObject );
  public:

    class SubscriptionInfo : public ProfileUA
    {
      PCLASSINFO( SubscriptionInfo, ProfileUA );
    public:
      SubscriptionInfo(){};

      SubscriptionInfo(
        const PString & eventPackage,
        const SIPURI & resourceURI,
        const PString & contentType,
        const PString & authUserName = PString::Empty(),
        const PString & authPassword = PString::Empty(),
        const PTimeInterval & expireInterval = PTimeInterval( 0, 3600 ),
        const PString & callId = ParserTools::GenGUID()
      )
      {
        m_CallId = callId;
        m_EventPackage = eventPackage;
        m_ResourceURI = resourceURI;
        m_ExpireInterval = expireInterval;
        m_ContentType = contentType;
        m_AuthUserName = authUserName;
        m_AuthPassword = authPassword;
      }

      SubscriptionInfo( 
        const SubscriptionInfo & info
      ) : ProfileUA( info )
      {
        operator=( info );
      }

      SubscriptionInfo & operator=( 
        const SubscriptionInfo & info 
      )
      {
        m_CallId = info.m_CallId;
        m_EventPackage = info.m_EventPackage;
        m_ResourceURI = info.m_ResourceURI;
        m_ExpireInterval = info.m_ExpireInterval;
        m_ContentType = info.m_ContentType;
        ProfileUA::operator=( *this );
        return *this;
      }

      PINLINE const PString & GetCallId()const{ return m_CallId; };
      PINLINE const PString & GetEventPackage()const{ return m_EventPackage; };
      PINLINE const SIPURI & GetResourceURI()const{ return m_ResourceURI; };
      PINLINE const PTimeInterval & GetExpireInterval()const{ return m_ExpireInterval; };
      PINLINE const PString & GetContentType()const{ return m_ContentType; };
      PINLINE const PString & GetAuthUserName()const{ return m_AuthUserName; };
      PINLINE const PString & GetAuthPassword()const{ return m_AuthPassword; };

      PINLINE void SetCallId( const PString & callId ){ m_CallId = callId; };
      PINLINE void SetEventPackage( const PString & package ){ m_EventPackage = package; };
      PINLINE void SetResourceURI(const SIPURI & uri ){ m_ResourceURI = uri; };
      PINLINE void SetExpireInterval( const PTimeInterval & expires ){ m_ExpireInterval = expires; };
      PINLINE void SetContentType( const PString & contentType ){ m_ContentType = contentType; };
      PINLINE void SetAuthPassword( const PString & password ){ m_AuthPassword = password; };
      PINLINE void SetAuthUserName( const PString & userName ){ m_AuthUserName = userName; };
  
    protected:
      PString m_CallId;
      PString m_EventPackage;
      SIPURI m_ResourceURI;
      PTimeInterval m_ExpireInterval;
      PString m_ContentType;
      PString m_AuthUserName;
      PString m_AuthPassword;
    };

    RFC3265ClientManager(
      SIPUserAgent & ua,
      const PString & eventPackageName,
      PINDEX sessionThreadCount,
      PINDEX stackSize
    );

    virtual ~RFC3265ClientManager();

    virtual SIPSession * OnCreateMultiDirectionalSession(
      const ProfileUA & profile,
      const PString & sessionId
    );

    virtual SIPSession * OnCreateClientSession(
      const ProfileUA & profile,
      const PString & sessionId
    );

    virtual SIPSession::GCRef OnCreateServerSession(
      const SIPMessage & request
    );

    virtual BOOL CreateSubscription(
      const SubscriptionInfo & info,
      SIPMessage & subscribe,
      BOOL willCreateSession = TRUE
    );

    virtual BOOL SendSubscription(
      const SIPMessage & subscribe
    );

    virtual BOOL DestroySubscription(
      const SubscriptionInfo & info 
    );

    virtual void OnNotification(
      RFC3265Client * session,
      const SIPMessage & notify
    );

    virtual void OnUnsolicitedNotification(
      const SIPMessage & notify
    );

    virtual void OnOrphanedMessage(
      const SIPMessage & message
    );

    virtual BOOL OnSubscribeRedirection(
      RFC3265Client * session,
      const SIPMessage & redirect
    );

    virtual BOOL OnAuthenticateSubscription(
      RFC3265Client * session,
      const SIPMessage & subsribe,
      const SIPMessage & authenticator
    );

    PINLINE const PString & GetEventPackageName()const{ return m_EventPackageName; };
   
  protected:

    PString m_EventPackageName;
  };
};

#endif


