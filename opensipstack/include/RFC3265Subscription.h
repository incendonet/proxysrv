/*
 *
 * RFC3265EventPackage.h
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
 * $Log: RFC3265Subscription.h,v $
 * Revision 1.5  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/11/08 06:01:03  joegenbaclor
 * added class keyword in GCObject friend declaration
 *
 * Revision 1.3  2006/10/30 03:42:42  joegenbaclor
 * Protected the destructor from getting deleted outside of the garbage collector
 *
 * Revision 1.2  2006/10/30 02:42:26  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.1  2006/05/03 03:28:54  joegenbaclor
 * Initial upload of RFC3265 and MWI files
 *
 *
 */


#ifndef RFC3265SUBSCRIPTION_H
#define RFC3265SUBSCRIPTION_H

#include "RFC3265Notification.h"
#include "SIPUserAgent.h"

using namespace SIP;

namespace RFC3265
{
  class RFC3265EventPackage;

  class RFC3265Subscription : public RFC3265Notifier
  {


   /*Subscriber: A subscriber is a user agent which receives NOTIFY
      requests from notifiers; these NOTIFY requests contain information
      about the state of a resource in which the subscriber is
      interested.  Subscribers typically also generate SUBSCRIBE
      requests and send them to notifiers to create subscriptions.

   Subscription: A subscription is a set of application state associated
      with a dialog.  This application state includes a pointer to the
      associated dialog, the event package name, and possibly an
      identification token.  Event packages will define additional
      subscription state information.  By definition, subscriptions
      exist in both a subscriber and a notifier.
    */

    GCREFINFO( RFC3265Subscription, "RFC3265Subscription" );
    PCLASSINFO( RFC3265Subscription, RFC3265Notifier );
  public:

    enum AuthStatus
    {
     AuthPending,
     AuthGranted,
     Unauthorized
    };

    RFC3265Subscription(
      const PString & callId,
      const SIPURI & subscriber,
      const SIPURI & resource,
      const PNotifier & notifier,
      RFC3265EventPackage & package
    );

  protected: 
    /// protect the destructor from 
    /// getting deleted outside of the garbage collector
    virtual ~RFC3265Subscription();
    friend class GCObject<RFC3265Subscription>;
  public:

    virtual void PrintOn( ostream & strm ) const;

    virtual void Cleanup();

    PINLINE const PString & GetCallId()const{ return m_CallId; };
    PINLINE const SIPURI & GetSubscriberURI()const{ return m_SubscriberURI; };
    PINLINE const SIPURI & GetResourceURI()const{ return m_ResourceURI; };
    PINLINE RFC3265EventPackage & GetPackage(){ return m_Package; };
    PINLINE AuthStatus GetAuthenticationStatus()const{ return m_AuthenticationStatus; };
    PINLINE AuthStatus GetAuthorizationStatus()const{ return m_AuthorizationStatus; };
  
  protected:
    //// protected accessors for RFC3265Resource
    PINLINE void SetAuthorizationStatus( AuthStatus auth ){ m_AuthorizationStatus = auth; };
    PINLINE void SetAuthenticationStatus( AuthStatus auth  ){ m_AuthenticationStatus = auth; };
    
    AuthStatus m_AuthorizationStatus;
    AuthStatus m_AuthenticationStatus;
    friend class RFC3265Resource;

  protected:

    PString m_CallId;
    SIPURI m_SubscriberURI;
    SIPURI m_ResourceURI;
    RFC3265EventPackage & m_Package;
  };
};

#endif





