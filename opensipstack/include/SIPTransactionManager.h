/*
 *
 * SIPTransactionManager.h
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
 * $Log: SIPTransactionManager.h,v $
 * Revision 1.9  2007/01/01 05:12:52  joegenbaclor
 * Introduced termiantion flag to avoid crashes on exit.
 * TO DO: Maybe the best thing to do here is to have a WaitForTermination()
 * function exposed for the SIPStack
 *
 * Revision 1.8  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.7  2006/09/01 03:00:32  joegenbaclor
 * Bulk Commit:
 *
 * 1.  A bug is discovered in SIP Transaction Destructor where destruction of the SIPTimers may cause a deadlock on the timer manager effectivily freezing the entire message Queue System.  This was fixed by introducing a separate function to destroy the timers on receipt of the final event just before the transaction is queued for deletion.
 *
 * 2.  There is bug in Invite Server Transaction where ACK transaction is added to the Ack Transaction Pool after sending og none 2xx final response.  This new transaction is never removed from the pool after Timer_H has fired.  this may result to a bad pointer if ACK is received after the timer expired since the transaction may have been deleted by then.
 *
 * 3.  A few tweaks in GC GetName func to return the Class Name if GC NAme is empty.
 *
 * Revision 1.6  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPTRANSACTIONMANAGER_H
#define SIPTRANSACTIONMANAGER_H

#include "SIPTransaction.h"
#include "SIPTimerManager.h"
#include "Logger.h"

using namespace SIPTimers;
using namespace Tools;

namespace SIPTransactions
{
  class SIPTransationStage;

  class SIPTransactionManager : public SIPTimerManager, public Logger
  {
    PCLASSINFO( SIPTransactionManager, SIPTimerManager );
  public:
    SIPTransactionManager(
      PINDEX threadCount = 10
    );

    virtual ~SIPTransactionManager();

    virtual SIPTransaction * OnCreateTransaction(
      const SIPMessage & event,
      const TransactionId & transactionId,
      SIPTransaction::Type type,
      Logger * logger = NULL
    );

    virtual void OnUnknownTransaction(
      const SIPMessage & event,
      BOOL fromTransport
    );

    BOOL CancelInviteClientTransaction(
      const SIPMessage & invite
    );

    BOOL CancelInviteServerTransaction(
      const SIPMessage & cancel
    );


    BOOL FindTransactionAndAddEvent(
      SIPMessage & event,
      TransactionId & id,
      BOOL fromTransport = FALSE,
      Logger * logger = NULL
    );

    BOOL FindTransactionAndAddEvent(
      const TransactionId & id,
      SIPEvent * event,
      Logger * logger = NULL
    );

    BOOL FindAckTransactionAndAddEvent(
      const SIPMessage & ack,
      TransactionId & id,
      Logger * logger = NULL
    );

    BOOL AddAckTransaction(
      const SIPMessage & response,
      SIPTransaction * transaction
    );

    BOOL RemoveAckTransaction(
      const SIPMessage & response,
      SIPTransaction * transaction
    );

    virtual void OnReceivedMessage(
      const SIPMessage & message,
      SIPTransaction & transaction
    ) = 0;

    virtual void OnSendMessageToTransport(
      const SIPMessage & message,
      SIPTransaction & transaction
    ) = 0;

    virtual void OnTimerExpire(
      SIPTimerEvent & timer,
      SIPTransaction & transaction
    ) = 0;
   
  protected:

    BOOL RemoveTransaction( 
      const TransactionId & transactionId 
    );

    BOOL CreateTransaction( 
      const SIPMessage & event,
      BOOL fromTransport,
      Logger * logger = NULL
    );

  protected:
    SIPTransactionStage * m_TransactionStage;
    BOOL m_IsTerminating;
  private:

    PMutex m_TransactionPoolMutex;
    SIPTransaction::Dictionary m_TransactionPool;
    
    PMutex m_AckTransactionPoolMutex;
    SIPTransaction::Dictionary m_AckTransactionPool;

    friend class SIPTransaction;
  };
};

#endif


