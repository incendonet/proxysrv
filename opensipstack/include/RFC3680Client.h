/*
 *
 * RFC3680Client.h
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
 * $Log: RFC3680Client.h,v $
 * Revision 1.3  2006/12/04 07:32:49  joegenbaclor
 * Added RFC3680 methods to Opal classes
 *
 * Revision 1.2  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/11/24 03:21:16  joegenbaclor
 * Initial upload of RFC3680 Client
 *
 *
 */


#ifndef RFC3680CLIENT_H
#define RFC3680CLIENT_H

#include "RFC3265Client.h"

using namespace RFC3265;

namespace RFC3680
{
  class RFC3680Client : public RFC3265ClientManager
  {
    PCLASSINFO( RFC3680Client, RFC3265ClientManager );
  public:

    RFC3680Client(
      SIPUserAgent & ua,
      PINDEX sessionThreadCount = 2,
      PINDEX stackSize = 1024 * 2
    );

    virtual ~RFC3680Client();

    void WatchRegistrationState(
      const SIPURI & stateAgent,
      const SIPURI & resourceURI,
      const PTimeInterval & expires = PTimeInterval( 0, 90 ),
      const PString & authUser = PString::Empty(),
      const PString & authPassword = PString::Empty()
      
    );

    void StopWatchingRegistrationState(
      const SIPURI & resourceURI
    );

    virtual void OnRegistrationStateEvent(
      const SIPURI & resourceURI,
      const SIPMessage & notify
    );

    virtual void OnStateOnline(
      const SIPURI & resourceURI
    );

    virtual void OnStateOffLine(
      const SIPURI & resourceURI
    );

  protected:
    PDICTIONARY( RFC3680Subscriptions, SIPURI, SubscriptionInfo );
    PMutex m_SubscriptionsMutex;
    RFC3680Subscriptions m_Subscriptions;
  };
};

#endif


