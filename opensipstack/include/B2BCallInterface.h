/*
 *
 * B2BCallInterface.h
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
 * $Log: B2BCallInterface.h,v $
 * Revision 1.5  2007/01/10 08:07:15  rcolobong
 * 1. Add new method for OnSendB2BConnect Call Interface.
 * 2. OnSendB2BConnect is supported in B2BUserAgent, B2BUACall and B2BUAConnection
 *
 * Revision 1.4  2006/11/30 08:19:35  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/06/29 05:09:13  joegenbaclor
 * Changed OnOutgoingCall invite parameter from const to none const to give a
 * chance for applications to modify the outbound invite before being sent to the transport
 *
 * Revision 1.2  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
 *
 * Revision 1.1  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 *
 */

#ifndef B2BCALLINTERFACE_H
#define B2BCALLINTERFACE_H

#include "B2BInterface.h"


namespace B2BUA
{
  class B2BCallInterface : public B2BInterface
  {
    PCLASSINFO( B2BCallInterface, B2BInterface );
  public: 
    B2BCallInterface(  
      B2BUserAgent & b2bua 
    );

    virtual ~B2BCallInterface();

    virtual void OnIncomingCall(
      B2BUAConnection & connection,
      B2BUACall & call,
      const SIPMessage & invite
    );

    virtual void OnOutgoingCall(
      B2BUAConnection & connection,
      B2BUACall & call,
      SIPMessage & invite
    );


    virtual void OnCallReinvite(
      B2BUAConnection & connection,
      B2BUACall & call,
      const SIPMessage & invite
    );

    virtual void OnProceeding(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & proceeding
    );

    virtual void OnCallAbandoned(
      B2BUAConnection & connection,
      B2BUACall & inboundCall,
      const SIPMessage & cancel
    );

    virtual void OnAlerting(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & alerting
    );

    virtual void OnProgress(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & progress
    );

    virtual void OnRejected(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & reject
    );

    virtual void OnConnected(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & connect
    );

    virtual void OnSendB2BConnect(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & connect
    );

    virtual void OnDisconnected(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & bye
    );


    virtual void OnInSessionMessage(
      B2BUAConnection & connection,
      B2BUACall & remoteCall,
      const SIPMessage & request
    );

  };

}

#endif

