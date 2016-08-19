/*
 *
 * MWIResource.cxx
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
 * $Log: MWIResource.cxx,v $
 * Revision 1.2  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/05/03 03:40:16  joegenbaclor
 * Initial upload of RFC3265 and MWI files
 *
 *
 */

#include "MWIResource.h"
#include "RFC3265EventPackage.h"
#include "RFC3265Notification.h"

#define new PNEW

using namespace MWI;

MWIResource::MWIResource(
  const SIPURI & resourceURI,
  RFC3265EventPackage & eventPackage
) : RFC3265Resource( resourceURI, eventPackage )
{
  m_NewMessageCount = 0;
  m_OldMessageCount = 0;
  m_NewUrgentMessageCount = 0;
  m_OldUrgentMessageCount = 0;

  m_NewFaxCount = 0;
  m_OldFaxCount = 0;
}

MWIResource::~MWIResource()
{
}

void MWIResource::OnVoiceMailStatus(
  const SIPURI & resourceURI,
  const SIPURI & accountURI,
  PINDEX newMessageCount,
  PINDEX oldMessageCount,
  PINDEX newUrgentMessageCount,
  PINDEX oldUrgentMessageCount
)
{
  m_NewMessageCount = newMessageCount;
  m_NewUrgentMessageCount = newUrgentMessageCount;
  m_OldMessageCount = oldMessageCount;
  m_OldUrgentMessageCount = oldUrgentMessageCount;

  /// prepare the headers
  PStringStream hbody;
  hbody << m_NewMessageCount << "/" << m_OldMessageCount 
        << " (" << m_NewUrgentMessageCount << "/" << m_OldUrgentMessageCount << ")";

  SIPHeader messagesWaiting( "Messages-Waiting", "yes" );
  SIPHeader messageAccount( "Message-Account", accountURI.AsString() );
  SIPHeader voiceMessage( "Voice-Message", hbody );

  RFC3265NotificationEvent event( m_Package.GetPackageName(), "active", "", "", "" );
  event.AddCustomHeader( messagesWaiting );
  event.AddCustomHeader( messageAccount );
  event.AddCustomHeader( voiceMessage );

  Notify( event );
}

void MWIResource::OnFacsimileStatus(
  const SIPURI & resourceURI,
  const SIPURI & accountURI,
  PINDEX newMessageCount,
  PINDEX oldMessageCount
)
{
  m_NewFaxCount = newMessageCount;
  m_OldFaxCount = oldMessageCount;


  /// prepare the headers
  PStringStream hbody;
  hbody << m_NewFaxCount << "/" << m_OldFaxCount; 

  SIPHeader messagesWaiting( "Messages-Waiting", "yes" );
  SIPHeader messageAccount( "Message-Account", accountURI.AsString() );
  SIPHeader faxMessage( "Fax-Message", hbody );

  RFC3265NotificationEvent event( m_Package.GetPackageName(), "active", "", "", "" );
  event.AddCustomHeader( messagesWaiting );
  event.AddCustomHeader( messageAccount );
  event.AddCustomHeader( faxMessage );

  Notify( event );
}


BOOL MWIResource::OnAuthorizeSubscription(
  RFC3265Subscription * subscription
)
{
  return RFC3265Resource::OnAuthorizeSubscription( subscription );
}

BOOL MWIResource::OnAuthenticateSubscription(
  RFC3265Subscription * subscription
)
{
  return RFC3265Resource::OnAuthenticateSubscription( subscription );
}

BOOL MWIResource::OnNewSubscription(
  RFC3265Subscription * subscription
)
{
  return RFC3265Resource::OnNewSubscription( subscription );
}

/// pure virtuals
void MWIResource::Cleanup()
{
}

BOOL MWIResource::PollEvents()
{
  PAssertAlways( PUnimplementedFunction );
  return FALSE;
}


