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
 * $Log: RFC3265StateAgent.cxx,v $
 * Revision 1.5  2007/01/14 13:12:56  joegenbaclor
 * Fixed compile warnings
 *
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

#include "RFC3265StateAgent.h"

#define new PNEW

using namespace RFC3265;

////////////////////////////////////////////////////////////

RFC3265StateAgent::RFC3265StateAgent(
  SIPUserAgent & ua
) : m_UserAgent( ua )
{
  m_Packages.DisallowDeleteObjects();
}

RFC3265StateAgent::~RFC3265StateAgent()
{
}

BOOL RFC3265StateAgent::AddPackage(
  const PCaselessString & name,
  RFC3265EventPackage * package
)
{
  PWaitAndSignal lock( m_PackagesMutex );
  
  if( m_Packages.Contains( name ) )
    return FALSE;

  return m_Packages.SetAt( name, package );
}

RFC3265EventPackage * RFC3265StateAgent::RemovePackage(
  const PCaselessString & name,
  BOOL autoDelete
)
{
  PWaitAndSignal lock( m_PackagesMutex );
  RFC3265EventPackage * package = m_Packages.RemoveAt( name );

  if( autoDelete )
  {
    delete package;
    package = NULL;
  }

  return package;
}

RFC3265EventPackage * RFC3265StateAgent::FindPackage(
  const PCaselessString & name
)
{
  PWaitAndSignal lock( m_PackagesMutex );
  return m_Packages.GetAt( name );
}

RFC3265Subscription::GCRef RFC3265StateAgent::Subscribe(
  const PNotifier & notifier,
  const PString & event,
  const PString & callId,
  const SIPURI & subscriber,
  const SIPURI & resource
)
{
  /// check if we have a package
  RFC3265EventPackage * package = FindPackage( event );
  
  if( package == NULL )
    return RFC3265Subscription::GCRef( NULL, "" );

  /// create the subscription
  RFC3265Subscription * subscription = new RFC3265Subscription(
      callId,
      subscriber,
      resource,
      notifier,
      *package
  );
 
  RFC3265Subscription::GCRef ptr = subscription->GCCreateRef();
  
  if( package->Subscribe( subscription ) != RFC3265Resource::AcceptSubscription )
  {
    subscription->GCCollect();
    return RFC3265Subscription::GCRef( NULL, "" );
  }

  return ptr;

}

BOOL RFC3265StateAgent::Unsubscribe(
  RFC3265Subscription * subscription,
  BOOL autoDelete
)
{
  subscription->GetPackage().Unsubscribe( subscription );

  if( autoDelete )
    subscription->GCCollect();

  return TRUE;
}


////////////////////////////////////////////////////////////


