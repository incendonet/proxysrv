/*
 *
 * B2BRoutingInterface.h
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
 * $Log: B2BRoutingInterface.h,v $
 * Revision 1.7  2006/11/30 08:19:35  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/11/23 11:00:02  joegenbaclor
 * Added Instant Message B2B support
 *
 * Revision 1.5  2006/11/23 01:28:18  joegenbaclor
 * Corrected compile errors from ryan's last commit
 *
 * Revision 1.4  2006/07/11 13:59:35  joegenbaclor
 * Completed Proxy functionalities for B2BUA
 *
 * Revision 1.3  2006/07/03 15:29:47  joegenbaclor
 * Started merging OpenB2BUA into OpenSBC
 *
 * Revision 1.2  2006/06/21 14:29:11  joegenbaclor
 * This upload marks the first successful call by OpenB2BUA
 *
 * Revision 1.1  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 *
 */
#ifndef B2BROUTINGINTERFACE_H
#define B2BROUTINGINTERFACE_H

#include "B2BInterface.h"
#include "ProxySession.h"

using namespace UACORE;

namespace B2BUA
{

  class B2BRoutingInterface : public B2BInterface
  {
    PCLASSINFO( B2BRoutingInterface, B2BInterface );
  public:

    B2BRoutingInterface(  
      B2BUserAgent & b2bua 
    );

    virtual ~B2BRoutingInterface();

    virtual BOOL B2BRouteCall(
      B2BUAConnection & connection,
      const SIPMessage & invite
    );

    virtual BOOL B2BRouteRequest(
      const SIPMessage & request,
      SIPURI  & route
    );

    virtual ProxySession::RoutePolicy RouteProxyRequest(
      ProxySession & session,
      SIPMessage & request
    );

    virtual BOOL FindAndRemovePendingInviteByCallId(
      const PString & i_sCallid
    );

    virtual void OnRequireRedirectionId(
      B2BUAConnection & connection,
      PString & redirectionId
    );

    virtual void OnRequireCallTransferId(
      B2BUAConnection & connection,
      PString & redirectionId
    );

    virtual void OnOrphanedMessage(
      SIPMessage & message
    );

    PLIST( B2BRouteList, SIPURI );
  };
}

#endif

