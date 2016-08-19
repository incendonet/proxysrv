/*
 *
 * SBCCallHandler.cxx
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
 * $Log: SBCCallHandler.cxx,v $
 * Revision 1.2  2006/08/29 07:45:42  rcolobong
 * Remove warning messages in Windows
 *
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


#include "SBCCallHandler.h"

SBCCallHandler::SBCCallHandler( 
  OpenSBC & ua 
  ) : B2BCallInterface( ua )
{
}

SBCCallHandler::~SBCCallHandler()
{
}

void SBCCallHandler::OnIncomingCall(
  B2BUAConnection &,
  B2BUACall &,
  const SIPMessage &
)
{
}

void SBCCallHandler::OnOutgoingCall(
  B2BUAConnection &,
  B2BUACall &,
  SIPMessage &
)
{
}

void SBCCallHandler::OnCallReinvite(
  B2BUAConnection &,
  B2BUACall &,
  const SIPMessage &
)
{
}

void SBCCallHandler::OnProceeding(
  B2BUAConnection &,
  B2BUACall &,
  const SIPMessage &
)
{
}

void SBCCallHandler::OnCallAbandoned(
  B2BUAConnection &,
  B2BUACall &,
  const SIPMessage &
)
{
}

void SBCCallHandler::OnAlerting(
  B2BUAConnection &,
  B2BUACall &,
  const SIPMessage &
)
{
}

void SBCCallHandler::OnProgress(
  B2BUAConnection &,
  B2BUACall &,
  const SIPMessage &
)
{
}

void SBCCallHandler::OnRejected(
  B2BUAConnection &,
  B2BUACall &,
  const SIPMessage &
)
{
}

void SBCCallHandler::OnConnected(
  B2BUAConnection &,
  B2BUACall &,
  const SIPMessage &
)
{
}

void SBCCallHandler::OnDisconnected(
  B2BUAConnection &,
  B2BUACall &,
  const SIPMessage &
)
{
}

void SBCCallHandler::OnInSessionMessage(
  B2BUAConnection &,
  B2BUACall &,
  const SIPMessage &
)
{
}

