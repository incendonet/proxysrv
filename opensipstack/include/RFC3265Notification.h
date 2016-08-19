/*
 *
 * RFC3265Notification.h
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
 * $Log: RFC3265Notification.h,v $
 * Revision 1.4  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/10/30 02:42:26  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.2  2006/08/11 08:38:42  joegenbaclor
 * Initial upload of Presence package
 *
 * Revision 1.1  2006/05/03 03:28:54  joegenbaclor
 * Initial upload of RFC3265 and MWI files
 *
 *
 */


#ifndef RFC3265NOTIFICATION_H
#define RFC3265NOTIFICATION_H

#include "SIPMessage.h"

using namespace SIPParser;

namespace RFC3265
{
  class RFC3265NotificationEvent : public PObject
  {
    /*Notification: Notification is the act of a notifier sending a NOTIFY
      message to a subscriber to inform the subscriber of the state of a
      resource.
    */
    PCLASSINFO( RFC3265NotificationEvent, PObject );
  public:

    RFC3265NotificationEvent();

    RFC3265NotificationEvent(
      const RFC3265NotificationEvent & event
    );

    RFC3265NotificationEvent(
      const PString & eventPackage,
      const PString & subscriptionState,
      const PString & eventId = PString::Empty(),
      const PString & contentType = PString::Empty(),
      const PString & body = PString::Empty()
    );

    BOOL GetCustomHeader(
      const PCaselessString & name,
      SIPHeader & h,
      PINDEX i = 0
    )const;

    BOOL GetCustomHeader(
      SIPHeader & h,
      PINDEX i
    )const;


    void SetCustomHeader(
      const PCaselessString & name,
      const SIPHeader & h,
      PINDEX i = 0
    );

    void SetCustomHeader(
      const SIPHeader & h,
      PINDEX i
    );

    void AddCustomHeader(
      const SIPHeader & h
    );

    BOOL PopCustomHeader(
      const PCaselessString & name,
      SIPHeader & h,
      PINDEX i = 0
    );

    virtual void PrintOn( 
      ostream & strm 
    ) const;

    RFC3265NotificationEvent & operator=(
      const RFC3265NotificationEvent & event
    );

    PINLINE const PString & GetEventPackage()const{ return m_EventPackage; };
    PINLINE void SetEventPackage( const PString & name ){ m_EventPackage = name; };
    PINLINE const PString & GetSubscriptionState()const{ return m_SubscriptionState; };
    PINLINE void SetSubscriptionState( const PString & state ){ m_SubscriptionState = state; };
    PINLINE const PString & GetEventId()const{ return m_EventId; };
    PINLINE void SetEventId( const PString & id ){ m_EventId = id; };
    PINLINE const PString & GetContentType()const{ return m_ContentType; };
    PINLINE void SetContentType( const PString & type ){ m_ContentType = type; };
    PINLINE const PString & GetBody()const{ return m_Body; };
    PINLINE void SetBody( const PString & body ){ m_Body = body; };

  protected:

    PString m_EventPackage;
    PString m_SubscriptionState;
    PString m_EventId;
    PString m_ContentType;
    PString m_Body;

    /// Custom Headers
    PLIST( CustomHeaders, SIPHeader );
    CustomHeaders m_CustomHeaders;
  };

  ////////////////////////////////////////////////////

  class RFC3265Notifier : public PObject
  {
   /*
   Notifier: A notifier is a user agent which generates NOTIFY requests
      for the purpose of notifying subscribers of the state of a
      resource.  Notifiers typically also accept SUBSCRIBE requests to
      create subscriptions.
   */
  public:

    RFC3265Notifier();
    
    RFC3265Notifier(
      const PNotifier & notifier
    );

    RFC3265Notifier(
      const RFC3265Notifier & notifier
    );

    RFC3265Notifier & operator =(
      const PNotifier & notifier
    );

    RFC3265Notifier & operator =(
      const RFC3265Notifier & notifier
    );

    void operator()(
      RFC3265NotificationEvent & event,
      INT subscription
    );

  protected:
    PNotifier m_Notifier;
  };

};

#endif





