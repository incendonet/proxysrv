/*
 *
 * SBCCallHandler.h
 * 
 * Open SIP Stack ( OSS )
 *
 * Copyright (c) opensipstack.org.
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either expressed or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is OpenSIPStack.
 *
 * The Initial Developer of the Original Code is opensipstack.org.
 *
 * The author of this code is Joegen E. Baclor
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: SBCCallHandler.h,v $
 * Revision 1.1  2006/08/14 10:04:59  rcolobong
 * Convert B2BUA to SBC
 * Support MP logging
 *
 * Revision 1.2  2006/06/29 05:09:12  joegenbaclor
 * Changed OnOutgoingCall invite parameter from const to none const to give a
 * chance for applications to modify the outbound invite before being sent to the transport
 *
 * Revision 1.1  2006/06/20 09:58:11  joegenbaclor
 * *** empty log message ***
 *
 *
 */

#ifndef B2BCALLHANDLER_H
#define B2BCALLHANDLER_H

#include "OpenSBC.h"

class SBCCallHandler : public B2BCallInterface
{
  PCLASSINFO( SBCCallHandler, B2BCallInterface );

public:
  SBCCallHandler( 
    OpenSBC & ua 
  );

  ~SBCCallHandler();

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

  PINLINE OpenSBC & GetSBC(){ return (OpenSBC &)m_B2BUA; };
};

#endif

