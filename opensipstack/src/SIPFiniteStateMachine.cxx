/*
 *
 * SIPFiniteStateMachine.cxx
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
 * $Log: SIPFiniteStateMachine.cxx,v $
 * Revision 1.7  2007/01/01 05:12:53  joegenbaclor
 * Introduced termiantion flag to avoid crashes on exit.
 * TO DO: Maybe the best thing to do here is to have a WaitForTermination()
 * function exposed for the SIPStack
 *
 * Revision 1.6  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.4  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPFiniteStateMachine.h"
#include "InviteClientTransaction.h"
#include "InviteServerTransaction.h"
#include "NoneInviteClientTransaction.h"
#include "NoneInviteServerTransaction.h"


#define new PNEW

using namespace SIPFSM;

SIPFiniteStateMachine::SIPFiniteStateMachine()
{
}

SIPTransaction * SIPFiniteStateMachine::OnCreateTransaction(
  const SIPMessage & /*event*/,
  const TransactionId & transactionId,
  SIPTransaction::Type type,
  Logger * logger 
)
{
  if( m_IsTerminating )
    return NULL;

  PString id = transactionId.AsString();
  switch( type )
  {
  case SIPTransaction::ICT:
    return new InviteClientTransaction( id, *this );
  case SIPTransaction::IST:
    return new InviteServerTransaction( id, *this );
  case SIPTransaction::NICT:
    return new NoneInviteClientTransaction( id, *this );
  case SIPTransaction::NIST:
    return new NoneInviteServerTransaction( id, *this );
  default:
    PAssertAlways( PLogicError );
    return NULL;
  }

}

void SIPFiniteStateMachine::OnUnknownTransaction(
  const SIPMessage & /*event*/,
  BOOL /*fromTransport*/
)
{

}

