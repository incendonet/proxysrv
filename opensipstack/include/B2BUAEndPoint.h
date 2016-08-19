/*
 *
 * B2BUAConnection.h
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
 * $Log: B2BUAEndPoint.h,v $
 * Revision 1.8  2007/01/11 02:11:40  rcolobong
 * Add setter for enable/disable proxying media
 *
 * Revision 1.7  2006/11/30 08:19:35  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/11/23 11:00:02  joegenbaclor
 * Added Instant Message B2B support
 *
 * Revision 1.5  2006/11/10 07:58:05  rcolobong
 * Add Method to disable/enable Session Timers in B2BUACall
 *
 * Revision 1.4  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
 *
 * Revision 1.3  2006/05/22 03:09:19  joegenbaclor
 * Added SQLite3 support
 *
 * Revision 1.2  2006/04/10 01:09:03  joegenbaclor
 * Implemented Create methods fo B2BUA sessions
 *
 * Revision 1.1  2006/04/08 06:27:13  joegenbaclor
 * Initial upload of B2BUA classes
 *
 *
 */

#ifndef B2BUAENDPOINT_H
#define B2BUAENDPOINT_H

#include "CallSessionManager.h"

using namespace UACORE;

namespace B2BUA
{
  class B2BUAConnection;
  class B2BUACall;

  class B2BUAEndPoint : public CallSessionManager
  {
    PCLASSINFO( B2BUAEndPoint, CallSessionManager );
  
  public:
    B2BUAEndPoint(
      SIPUserAgent & ua,
      PINDEX sessionThreadCount,
      PINDEX stackSize
    );

    virtual ~B2BUAEndPoint();

    virtual B2BUAConnection * CreateB2BUA(
      const SIPMessage & request,
      B2BUACall * session
    );

    virtual B2BUAConnection * OnCreateB2BUA(
      const SIPMessage & request,
      const PString & sessionId,
      B2BUACall * session
    );

    virtual CallSession * OnCreateServerCallSession(
      const SIPMessage & request,
      const PString & sessionId
    );

    virtual CallSession * OnCreateClientCallSession(
      const ProfileUA & profile,
      const PString & sessionId
    );

    virtual void OnReceivedInstantMessage(
      const SIPMessage & message
    );

    PINLINE void EnableSessionTimer( BOOL enable = TRUE ) { m_EnableSessionTimer = enable; }
    PINLINE void DisableSessionTimer() { m_EnableSessionTimer = FALSE; }
    PINLINE void SetMediaProxyIfPrivate( BOOL media = TRUE ) { m_MediaProxyIfPrivate = media; }

  private:
    BOOL m_EnableSessionTimer;
    BOOL m_MediaProxyIfPrivate;

  };

};

#endif



