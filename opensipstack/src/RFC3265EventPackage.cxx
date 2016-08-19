/*
 *
 * RFC3265EventPackage.cxx
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
 * $Log: RFC3265EventPackage.cxx,v $
 * Revision 1.3  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.2  2006/10/30 02:42:27  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.1  2006/05/03 03:40:16  joegenbaclor
 * Initial upload of RFC3265 and MWI files
 *
 *
 */

#include "RFC3265EventPackage.h"
#include "RFC3265StateAgent.h"

#define new PNEW

using namespace RFC3265;

RFC3265EventPackage::RFC3265EventPackage(
  RFC3265StateAgent & stateAgent
) : m_StateAgent( stateAgent )
{
  m_Subscriptions.DisallowDeleteObjects();
  m_Resources.DisallowDeleteObjects();
}

RFC3265EventPackage::~RFC3265EventPackage()
{
}

RFC3265Resource::SubscriptionResponse RFC3265EventPackage::Subscribe(
  RFC3265Subscription * subscription
)
{
  RFC3265Resource::GCRef resource = FindResource( subscription->GetResourceURI() );
  
  if( resource == NULL )
    return RFC3265Resource::AcceptByNoResource;

  /// check if the subscriber is authenticated
  if( !resource->OnAuthenticateSubscription( subscription ) )
    return RFC3265Resource::AcceptByUnAuthorized;
  else
  {
    if( subscription->GetAuthorizationStatus() == RFC3265Subscription::AuthPending )
      return RFC3265Resource::AcceptByAuthorization;
    else if( subscription->GetAuthorizationStatus() == RFC3265Subscription::Unauthorized )
      return RFC3265Resource::AcceptByUnAuthorized;
  }

  /// check if subscriber is authorized to use the resource
  if( !resource->OnAuthorizeSubscription( subscription ) )
    return RFC3265Resource::AcceptByUnAuthorized;
  else
  {
    if( subscription->GetAuthenticationStatus() == RFC3265Subscription::AuthPending )
      return RFC3265Resource::AcceptByAuthorization;
    else if( subscription->GetAuthenticationStatus() == RFC3265Subscription::Unauthorized )
      return RFC3265Resource::AcceptByUnAuthorized;
  }

  /// report the new subscription.
  if( !resource->OnNewSubscription( subscription ) ||
    !resource->Subscribe( subscription ) )
    return RFC3265Resource::AcceptByUnAuthorized;


  PWaitAndSignal lock( m_SubscriptionsMutex );
  
  /// add the subscription to our DB
  PStringStream subscriptionId;
  subscriptionId << *subscription;
  m_Subscriptions.SetAt( subscriptionId, new RFC3265Subscription::GCRef( subscription->GCCreateRef() ) );

  return RFC3265Resource::AcceptSubscription;
}

void RFC3265EventPackage::Unsubscribe(
  RFC3265Subscription * subscription
)
{
  RFC3265Resource::GCRef resource = FindResource( subscription->GetResourceURI() );
  
  if( resource != NULL )
  {
    resource->Unsubscribe( subscription );
  }

  PWaitAndSignal lock( m_SubscriptionsMutex );
  
  /// add the subscription to our DB
  PStringStream subscriptionId;
  subscriptionId << *subscription;
  m_Subscriptions.RemoveAt( subscriptionId );
}

BOOL RFC3265EventPackage::Notify(
  const SIPURI & resourceURI,
  RFC3265NotificationEvent & event
)
{
  RFC3265Resource::GCRef ptr = FindResource( resourceURI );
  
  if( ptr == NULL )
    return FALSE;

  return ptr->Notify( event );
}

BOOL RFC3265EventPackage::OnAuthorizeSubscription(
  RFC3265Resource & resource,
  RFC3265Subscription * subscription
)
{
  return TRUE;
}

BOOL RFC3265EventPackage::OnAuthenticateSubscription(
  RFC3265Resource & resource,
  RFC3265Subscription * subscription
)
{
  return TRUE;
}

BOOL RFC3265EventPackage::OnNewSubscription(
  RFC3265Resource & resource,
  RFC3265Subscription * subscription
)
{
  return TRUE;
}

RFC3265Resource::GCRef RFC3265EventPackage::CreateResource(
  const SIPURI & resourceURI
)
{
  RFC3265Resource * resource = OnCreateResource( resourceURI );

  if( resource == NULL )
    return RFC3265Resource::GCRef( NULL, "" );

  PWaitAndSignal lock( m_ResourcesMutex );
  PStringStream id;
  id << *resource;
  m_Resources.SetAt( id, new RFC3265Resource::GCRef( resource->GCCreateRef( "RFC3265EventPackage::CreateResource") ) );

  return resource->GCCreateRef();
}


BOOL RFC3265EventPackage::DestroyResource(
  const SIPURI & resourceURI
)
{
  PWaitAndSignal lock( m_ResourcesMutex );

  PStringStream id;
  id << GetPackageName() << "::" << resourceURI;
  
  RFC3265Resource::GCRef * resource = m_Resources.RemoveAt( id );

  if( resource == NULL )
    return FALSE;

  RFC3265Resource *obj = dynamic_cast<RFC3265Resource *>(resource->GetObject());

  if( obj != NULL )
    obj->GCCollect();
  
  return TRUE;
}

RFC3265Resource::GCRef RFC3265EventPackage::FindResource(
  const SIPURI & resourceURI
)
{
  PWaitAndSignal lock( m_ResourcesMutex );

  PStringStream id;
  id << GetPackageName() << "::" << resourceURI;
  
  RFC3265Resource::GCRef * resource = m_Resources.GetAt( id );
  if( resource != NULL )
    return *resource;

  return RFC3265Resource::GCRef( NULL, "RFC3265EventPackage::FindResource" );
}


void RFC3265EventPackage::PollEvents()
{
  PWaitAndSignal lock( m_ResourcesMutex );

  for( PINDEX i = 0; i < m_Resources.GetSize(); i++ )
  {
    RFC3265Resource::GCRef & resource = m_Resources.GetDataAt( i );
    resource->PollEvents();
  }

}

void RFC3265EventPackage::PollEvents( 
  const SIPURI & resourceURI 
)
{
  RFC3265Resource::GCRef ptr = FindResource( resourceURI );

  if( ptr != NULL )
    ptr->PollEvents();
}


