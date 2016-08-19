/*
 *
 * RFC3265Resource.h
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
 * $Log: RFC3265Resource.h,v $
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


#ifndef RFC3265EVENTRESOURCE_H
#define RFC3265EVENTRESOURCE_H

#include "SIPUserAgent.h"
#include "RFC3265Subscription.h"

using namespace SIP;

namespace RFC3265
{
  class RFC3265EventPackage;

  class RFC3265Resource : public Logger, public PObject
  {
    GCREFINFO( RFC3265Resource, "RFC3265Resource" );
    PCLASSINFO( RFC3265Resource, PObject );
  
  public:

    enum SubscriptionResponse
    {
      AcceptSubscription,
      AcceptByAuthorization,
      AcceptByAuthentication,
      AcceptByUnAuthorized,
      AcceptByNoResource,
      AcceptByUnsupported
    };
    
    RFC3265Resource(
      const SIPURI & resourceURI,
      RFC3265EventPackage & eventPackage
    );

    /// protect the destructor from 
    /// getting deleted outside of the garbage collector
    virtual ~RFC3265Resource();
    friend class GCObject<RFC3265Resource>;

  public:
    BOOL Subscribe( 
      RFC3265Subscription * subscription
    );

    void Unsubscribe(
      RFC3265Subscription * subscription
    );

    virtual BOOL OnAuthorizeSubscription(
      RFC3265Subscription * subscription
    );

    virtual BOOL OnAuthenticateSubscription(
      RFC3265Subscription * subscription
    );

    virtual BOOL OnNewSubscription(
      RFC3265Subscription * subscription
    );

    virtual void PrintOn( ostream & strm ) const;

    BOOL Notify(
      RFC3265NotificationEvent & event
    );

    virtual BOOL PollEvents() = 0;

    PINLINE const SIPURI & GetResourceURI()const{ return m_ResourceURI; };

  protected:

    PDICTIONARY( RFC3265SubscriptionList, PCaselessString, RFC3265Subscription::GCRef );
    PMutex m_SubscriptionsMutex;
    RFC3265SubscriptionList m_Subscriptions;

    RFC3265EventPackage & m_Package;
    SIPURI m_ResourceURI;

  };

  

};

#endif





