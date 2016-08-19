/*
 *
 * RFC3265Notification.cxx
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
 * $Log: RFC3265Notification.cxx,v $
 * Revision 1.3  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.2  2006/08/11 08:38:42  joegenbaclor
 * Initial upload of Presence package
 *
 * Revision 1.1  2006/05/03 03:40:16  joegenbaclor
 * Initial upload of RFC3265 and MWI files
 *
 *
 */

#include "RFC3265Notification.h"

#define new PNEW

using namespace RFC3265;

RFC3265NotificationEvent::RFC3265NotificationEvent()
{
}

RFC3265NotificationEvent::RFC3265NotificationEvent(
  const RFC3265NotificationEvent & event
)
{
  operator=( event );
}

RFC3265NotificationEvent::RFC3265NotificationEvent(
  const PString & eventPackage,
  const PString & subscriptionState,
  const PString & eventId,
  const PString & contentType,
  const PString & body
)
{
  m_EventPackage = eventPackage;
  m_SubscriptionState = subscriptionState;
  m_EventId = eventId;
  m_ContentType = contentType;
  m_Body = body;
}

RFC3265NotificationEvent & RFC3265NotificationEvent::operator=(
  const RFC3265NotificationEvent & event
)
{
  m_EventPackage = event.m_EventPackage;
  m_SubscriptionState = event.m_SubscriptionState;
  m_EventId = event.m_EventId;
  m_CustomHeaders = event.m_CustomHeaders;
  m_ContentType = event.m_ContentType;
  m_Body = event.m_Body;
  return *this;
}

BOOL RFC3265NotificationEvent::GetCustomHeader(
  const PCaselessString & name,
  SIPHeader & header,
  PINDEX idx
)const
{
  PINDEX currentIndex = 0;
  BOOL ok = FALSE;
  for( PINDEX i = 0; i < m_CustomHeaders.GetSize(); i++ )
  {
    if( (m_CustomHeaders)[i].GetHeaderName() *= name )
    {
      if( currentIndex == idx )
      {
        header = (m_CustomHeaders)[i];
        ok = TRUE;
        break;
      }else
        currentIndex++;
    }
  }

  return ok;
}

BOOL RFC3265NotificationEvent::GetCustomHeader(
  SIPHeader & h,
  PINDEX i
)const
{
  if( i > m_CustomHeaders.GetSize() - 1 )
    return FALSE;

  h = (m_CustomHeaders)[i];

  return TRUE;
}


void RFC3265NotificationEvent::SetCustomHeader(
  const PCaselessString & name,
  const SIPHeader & header,
  PINDEX idx
)
{
  PINDEX currentIndex = 0;
  BOOL ok = FALSE;
  for( PINDEX i = 0; i < m_CustomHeaders.GetSize(); i++ )
  {
    if( (m_CustomHeaders)[i].GetHeaderName() *= name )
    {
      if( currentIndex == idx )
      {
        (m_CustomHeaders)[i] = header;
        ok = TRUE;
        break;
      }else
        currentIndex++;
    }
  }
}

void RFC3265NotificationEvent::SetCustomHeader(
  const SIPHeader & header,
  PINDEX i
)
{
  if( i > m_CustomHeaders.GetSize() - 1 )
    return;

  (m_CustomHeaders)[i] == header;
}

void RFC3265NotificationEvent::AddCustomHeader(
  const SIPHeader & header
)
{
  m_CustomHeaders.Append( new SIPHeader( header ) );
}

BOOL RFC3265NotificationEvent::PopCustomHeader(
  const PCaselessString & name,
  SIPHeader & h,
  PINDEX idx
)
{
  PINDEX currentIndex = 0;
  BOOL ok = FALSE;
  for( PINDEX i = 0; i < m_CustomHeaders.GetSize(); i++ )
  {
    if( (m_CustomHeaders)[i].GetHeaderName() *= name )
    {
      if( currentIndex == idx )
      {
        h = (m_CustomHeaders)[i];
        m_CustomHeaders.RemoveAt(i);
        ok = TRUE;
        break;
      }else
        currentIndex++;
    }
  }

  return ok;
}

void RFC3265NotificationEvent::PrintOn( 
  ostream & strm 
) const
{
  strm <<  "Event: "  <<  m_EventPackage;
  if( !m_EventId.IsEmpty() )
    strm << ";id=" << m_EventId;

  strm << "\r\n";

  strm << "Subscription-State: " <<  m_SubscriptionState << "\r\n";

  for( PINDEX i = 0; i < m_CustomHeaders.GetSize(); i++ )
    strm << m_CustomHeaders[i] << "\r\n";

  if( !m_ContentType.IsEmpty() )
    strm << "Content-Type: " << m_ContentType << "\r\n";

  if( !m_Body.IsEmpty() )
    strm << "\r\n" << m_Body << "\r\n"; 

}

////////////////////////////////////////////////////////////


RFC3265Notifier::RFC3265Notifier()
{
}

RFC3265Notifier::RFC3265Notifier(
  const PNotifier & notifier
)
{
  operator=( notifier );
}

RFC3265Notifier::RFC3265Notifier(
  const RFC3265Notifier & notifier
)
{
  operator=( notifier );
}

RFC3265Notifier & RFC3265Notifier::operator =(
  const PNotifier & notifier
)
{
  m_Notifier = notifier;
  return *this;
}

RFC3265Notifier & RFC3265Notifier::operator =(
  const RFC3265Notifier & notifier
)
{
  m_Notifier = notifier.m_Notifier;
  return *this;
}

void RFC3265Notifier::operator()(
  RFC3265NotificationEvent & event,
  INT subscription
)
{
  m_Notifier( event, subscription );
}


