/*
 *
 * SIPTimerInterface.h
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
 * $Log: SIPTimerInterface.h,v $
 * Revision 1.9  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.8  2006/09/01 03:00:32  joegenbaclor
 * Bulk Commit:
 *
 * 1.  A bug is discovered in SIP Transaction Destructor where destruction of the SIPTimers may cause a deadlock on the timer manager effectivily freezing the entire message Queue System.  This was fixed by introducing a separate function to destroy the timers on receipt of the final event just before the transaction is queued for deletion.
 *
 * 2.  There is bug in Invite Server Transaction where ACK transaction is added to the Ack Transaction Pool after sending og none 2xx final response.  This new transaction is never removed from the pool after Timer_H has fired.  this may result to a bad pointer if ACK is received after the timer expired since the transaction may have been deleted by then.
 *
 * 3.  A few tweaks in GC GetName func to return the Class Name if GC NAme is empty.
 *
 * Revision 1.7  2006/03/22 08:45:52  joegenbaclor
 * More work on simple-proxy and affected classes
 *
 * Revision 1.6  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPTIMERINTERFACE_H
#define SIPTIMERINTERFACE_H

#include "SIPTimer.h"
#include "SIPTimerManager.h"
#include "Logger.h"

using namespace Tools;

namespace SIPTimers
{

  class SIPTimerInterface : public PObject
  {
    PCLASSINFO( SIPTimerInterface, PObject );
  public:

    SIPTimerInterface(
      SIPTimerManager & manager,
      const PString & internalId = PString::Empty() 
    );

    virtual ~SIPTimerInterface();

    void StartTransactionLifeSpanTimer(
      PInt64 inteval = 120000 /// 2 minutes
    );

    void StartTimer_A(
      PInt64 interval = SIP_TIMER_A
    );

    void StartTimer_B(
      PInt64 interval = SIP_TIMER_B
    );

    void StartTimer_C(
      PInt64 interval = SIP_TIMER_C
    );

    void StartTimer_D(
      PInt64 interval = SIP_TIMER_D
    );

    void StartTimer_E(
      PInt64 interval = SIP_TIMER_E
    );

    void StartTimer_F(
      PInt64 interval = SIP_TIMER_F
    );

    void StartTimer_G(
      PInt64 interval = SIP_TIMER_G
    );

    void StartTimer_H(
      PInt64 interval = SIP_TIMER_H
    );

    void StartTimer_I(
      PInt64 interval = SIP_TIMER_I
    );

    void StartTimer_J(
      PInt64 interval = SIP_TIMER_J
    );

    void StartTimer_K(
      PInt64 interval = SIP_TIMER_K
    );

    void StopTransactionLifeSpanTimer();

    void StopTimer_A();

    void StopTimer_B();

    void StopTimer_C();

    void StopTimer_D();

    void StopTimer_E();

    void StopTimer_F();

    void StopTimer_G();

    void StopTimer_H();

    void StopTimer_I();

    void StopTimer_J();

    void StopTimer_K();

    virtual void OnTransactionLifeSpanExpire(
      SIPTimer & timer
    ) = 0;

    virtual void OnTimerExpire_A(
      SIPTimer & timer
    );

    virtual void OnTimerExpire_B(
      SIPTimer & timer
    );

    virtual void OnTimerExpire_C(
      SIPTimer & timer
    );

    virtual void OnTimerExpire_D(
      SIPTimer & timer
    );

    virtual void OnTimerExpire_E(
      SIPTimer & timer
    );

    virtual void OnTimerExpire_F(
      SIPTimer & timer
    );

    virtual void OnTimerExpire_G(
      SIPTimer & timer
    );

    virtual void OnTimerExpire_H(
      SIPTimer & timer
    );

    virtual void OnTimerExpire_I(
      SIPTimer & timer
    );

    virtual void OnTimerExpire_J(
      SIPTimer & timer
    );

    virtual void OnTimerExpire_K(
      SIPTimer & timer
    );

    virtual void RemoveAllTimers();
  protected:

    SIPTimer  *m_TransactionLifeSpan;
    SIPTimerA *m_TimerA;
    SIPTimerB *m_TimerB;
    SIPTimerC *m_TimerC;
    SIPTimerD *m_TimerD;
    SIPTimerE *m_TimerE;
    SIPTimerF *m_TimerF;
    SIPTimerG *m_TimerG;
    SIPTimerH *m_TimerH;
    SIPTimerI *m_TimerI;
    SIPTimerJ *m_TimerJ;
    SIPTimerK *m_TimerK;
    

    PDECLARE_NOTIFIER( SIPTimer, SIPTimerInterface, _OnTransactionLifeSpanExpire );
    PDECLARE_NOTIFIER( SIPTimer, SIPTimerInterface, _OnTimerExpire_A );
    PDECLARE_NOTIFIER( SIPTimer, SIPTimerInterface, _OnTimerExpire_B );
    PDECLARE_NOTIFIER( SIPTimer, SIPTimerInterface, _OnTimerExpire_C );
    PDECLARE_NOTIFIER( SIPTimer, SIPTimerInterface, _OnTimerExpire_D );
    PDECLARE_NOTIFIER( SIPTimer, SIPTimerInterface, _OnTimerExpire_E );
    PDECLARE_NOTIFIER( SIPTimer, SIPTimerInterface, _OnTimerExpire_F );
    PDECLARE_NOTIFIER( SIPTimer, SIPTimerInterface, _OnTimerExpire_G );
    PDECLARE_NOTIFIER( SIPTimer, SIPTimerInterface, _OnTimerExpire_H );
    PDECLARE_NOTIFIER( SIPTimer, SIPTimerInterface, _OnTimerExpire_I );
    PDECLARE_NOTIFIER( SIPTimer, SIPTimerInterface, _OnTimerExpire_J );
    PDECLARE_NOTIFIER( SIPTimer, SIPTimerInterface, _OnTimerExpire_K );

  public:
    PString m_InternalId;
    SIPTimerManager & m_TimerManager;
    Logger m_TimerInterfaceLogger;
  };

};

#endif
