/*
 *
 * InviteClientTransaction.h
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
 * $Log: InviteClientTransaction.h,v $
 * Revision 1.8  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.7  2006/07/06 05:38:10  joegenbaclor
 * Final last minute changes to CBCOM hash support
 *
 * Revision 1.6  2006/03/20 03:51:12  joegenbaclor
 * This upload marks the first successful OPAL call using opensipstack!!!   There is still some pending issues
 * in 183 Progress handling in OPAL classes but a fix should with the next revision.
 *
 * Revision 1.5  2006/03/14 06:38:51  joegenbaclor
 * Introduced State Connected to ICT to catch 200 ok retransmissions
 *
 * Revision 1.4  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef INVITECLIENTTRANSACTION_H
#define INVITECLIENTTRANSACTION_H

#include "SIPTransaction.h" 

using namespace SIPTransactions;
class SIPTransactions::SIPTransactionManager;

namespace SIPFSM
{

  class InviteClientTransaction : public SIPTransaction
  {
    PCLASSINFO( InviteClientTransaction, SIPTransaction );
  public:
    
    enum State
    {
      Idle = 0,
      Calling,
      Proceeding,
      Completed,
      Connected,
      Terminated
    };

    InviteClientTransaction(
      const PString & id,
      SIPTransactionManager & manager
    );

    

    virtual ~InviteClientTransaction();

    void SendACK( 
      const SIPMessage & response 
    );

    virtual void OnTimerEvent(
      SIPTimerEvent & timerEvent  
    );

    virtual void OnSIPEvent(
      SIPMessage & message,
      BOOL isSendEvent
    );

    virtual void OnUnknownEvent( 
      SIPEvent * event 
    );

    virtual void HandleStateIdle(
      SIPMessage & message,
      BOOL isSendEvent
    );

    virtual void HandleStateCalling(
      SIPMessage & message,
      BOOL isSendEvent
    );

    virtual void HandleStateProceeding(
      SIPMessage & message,
      BOOL isSendEvent
    );

    virtual void HandleStateCompleted(
      SIPMessage & message,
      BOOL isSendEvent
    );

    virtual void HandleStateConnected(
      SIPMessage & message,
      BOOL isSendEvent
    );

  protected:
    BOOL m_HasSentInitialRequest;
    BOOL m_EnableXORHash;

  };

};

#endif

