/*
 *
 * RFC3265Resource.cxx
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
 * $Log: RFC3265Resource.cxx,v $
 * Revision 1.4  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/10/30 02:42:27  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.2  2006/08/11 08:38:42  joegenbaclor
 * Initial upload of Presence package
 *
 * Revision 1.1  2006/05/03 03:40:16  joegenbaclor
 * Initial upload of RFC3265 and MWI files
 *
 *
 */

#include "RFC3265Resource.h"
#include "RFC3265EventPackage.h"

#define new PNEW

using namespace RFC3265;

GCCREATECOLLECTOR( RFC3265Resource );

RFC3265Resource::RFC3265Resource(
  const SIPURI & resourceURI,
  RFC3265EventPackage & eventPackage
) : m_Package( eventPackage )
{
  m_ResourceURI = resourceURI;
}

RFC3265Resource::~RFC3265Resource()
{
}

void RFC3265Resource::PrintOn( ostream & strm ) const
{
  strm << m_Package.GetPackageName() << "::" << m_ResourceURI;
}

BOOL RFC3265Resource::Subscribe( 
  RFC3265Subscription * subscription
)
{
  PWaitAndSignal lock( m_SubscriptionsMutex );
  
  /// add the subscription to our DB
  PStringStream subscriptionId;
  subscriptionId << *subscription;
  m_Subscriptions.SetAt( subscriptionId, new RFC3265Subscription::GCRef( subscription->GCCreateRef() ) );
  return TRUE;
}

void RFC3265Resource::Unsubscribe(
  RFC3265Subscription * subscription
)
{
  PWaitAndSignal lock( m_SubscriptionsMutex );
  
  /// add the subscription to our DB
  PStringStream subscriptionId;
  subscriptionId << (SIPSmartObject&)*subscription;
  m_Subscriptions.RemoveAt( subscriptionId );
}

BOOL RFC3265Resource::OnAuthorizeSubscription(
  RFC3265Subscription * subscription
)
{
  return m_Package.OnAuthorizeSubscription( *this, subscription ); 
}

BOOL RFC3265Resource::OnAuthenticateSubscription(
  RFC3265Subscription * subscription
)
{
  return m_Package.OnAuthenticateSubscription( *this, subscription );
}

BOOL RFC3265Resource::OnNewSubscription(
  RFC3265Subscription * subscription
)
{
  return TRUE;
}

BOOL RFC3265Resource::Notify(
  RFC3265NotificationEvent & event
)
{
  PWaitAndSignal lock( m_SubscriptionsMutex );
  
  for( PINDEX i = 0; i < m_Subscriptions.GetSize(); i++ )
  {
    RFC3265Subscription::GCRef subscription = m_Subscriptions.GetDataAt( i );
    RFC3265Notifier & notifier = dynamic_cast<RFC3265Notifier&>(*(subscription.GetObject()));   
    notifier( event, (INT)(subscription.GetObject()) );
  }

  return TRUE;
}



