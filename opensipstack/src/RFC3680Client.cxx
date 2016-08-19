/*
 *
 * RFC3680Client.cxx
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
 * $Log: RFC3680Client.cxx,v $
 * Revision 1.2  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/11/24 03:21:16  joegenbaclor
 * Initial upload of RFC3680 Client
 *
 *
 */


#include "RFC3680Client.h"

#define new PNEW

using namespace RFC3680;


RFC3680Client::RFC3680Client(
  SIPUserAgent & ua,
  PINDEX sessionThreadCount,
  PINDEX stackSize
  ) : RFC3265ClientManager( ua, "reg", sessionThreadCount, stackSize )
{
}


RFC3680Client::~RFC3680Client()
{
}

void RFC3680Client::WatchRegistrationState(
  const SIPURI & stateAgent,
  const SIPURI & resourceURI,
  const PTimeInterval & expires,
  const PString & authUser,
  const PString & authPassword
)
{
  PWaitAndSignal lock( m_SubscriptionsMutex );
  if( m_Subscriptions.Contains( resourceURI ) )
    return;

  PString callId = ParserTools::GenGUID();
  SubscriptionInfo * subscription = new SubscriptionInfo(
    "reg",
    resourceURI,
    "application/reginfo+xml",
    authUser,
    authPassword,
    expires,
    callId );

  m_Subscriptions.SetAt( resourceURI, subscription );

  /*
  SubscriptionInfo(
        const PString & eventPackage,
        const SIPURI & resourceURI,
        const PString & contentType,
        const PString & authUserName = PString::Empty(),
        const PString & authPassword = PString::Empty(),
        const PTimeInterval & expireInterval = PTimeInterval( 0, 3600 ),
        const PString & callId = ParserTools::GenGUID()
   */


}

void RFC3680Client::StopWatchingRegistrationState(
  const SIPURI & resourceURI
)
{
}

void RFC3680Client::OnRegistrationStateEvent(
  const SIPURI & resourceURI,
  const SIPMessage & notify
)
{
}

void RFC3680Client::OnStateOnline(
  const SIPURI & resourceURI
)
{
}

void RFC3680Client::OnStateOffLine(
  const SIPURI & resourceURI
)
{
}

