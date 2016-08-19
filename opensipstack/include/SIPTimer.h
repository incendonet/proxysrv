/*
 *
 * SIPTimer.h
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
 * $Log: SIPTimer.h,v $
 * Revision 1.5  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPTIMER_H
#define SIPTIMER_H

#include <ptlib.h>


namespace SIPTimers
{

  class SIPTimerManager;

  class SIPTimer : public PObject
  {
    PCLASSINFO( SIPTimer, PObject );
  public:

    SIPTimer(
      SIPTimerManager & manager
    );

    SIPTimer(
      PInt64 interval,
      PNotifier notifierm,
      SIPTimerManager & manager
    );

    virtual ~SIPTimer();

    void Stop();

    void Resume();

    PINLINE BOOL IsRunning(){ return m_IsRunning; };

    PINLINE void SetNotifier( PNotifier notifier ){ m_Notifier = notifier; };

    PINLINE void SetInterval( PInt64 interval ){ m_TimeLeft = m_OldInterval = interval; };

    PINLINE PInt64 GetInterval(){ return m_OldInterval; };

    PINLINE PInt64 GetTimeLeft(){ return m_TimeLeft; };

  protected:

    void Fire();

    void Destroy();

    int m_Id;
    BOOL m_IsRunning;
    PNotifier m_Notifier;
    PMutex m_CriticalSection;
    PInt64 m_OldInterval;
    PInt64 m_TimeLeft;
    SIPTimerManager & m_Manager;

    static PAtomicInteger m_CurrentId;
    PDICTIONARY( SortedCollection, POrdinalKey, SIPTimer );

    friend class SIPTimerManager;
    friend class SIPTimerInterface;
  };

  #define __SECONDS__(v)v*1000
  #define __MINUTE__(v)v*60000


  ///T1       500ms default    Section 17.1.1.1     RTT Estimate
  #define SIP_TIMER_T1 500

  class SIPTimerT1 : public SIPTimer
  {
    PCLASSINFO( SIPTimerT1, SIPTimer );
  public:
    SIPTimerT1( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_T1, n, manager ){};
  };

  ///T2       4s               Section 17.1.2.2     The maximum retransmit
  ///                                               interval for non-INVITE
  ///                                               requests and INVITE
  ///                                               responses
  #define SIP_TIMER_T2 __SECONDS__(4)

  class SIPTimerT2 : public SIPTimer
  {
    PCLASSINFO( SIPTimerT2, SIPTimer );
  public:
    SIPTimerT2( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_T2, n, manager ){};
  };


  ///T4       5s               Section 17.1.2.2     Maximum duration a
  ///                                               message will
  ///                                               remain in the network
  #define SIP_TIMER_T4 __SECONDS__(5)

  class SIPTimerT4 : public SIPTimer
  {
    PCLASSINFO( SIPTimerT4, SIPTimer );
  public:
    SIPTimerT4( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_T4, n, manager ){};
  };


  ///Timer A  initially T1     Section 17.1.1.2     INVITE request retransmit
  ///                                               interval, for UDP only
  #define SIP_TIMER_A SIP_TIMER_T1

  class SIPTimerA : public SIPTimer
  {
    PCLASSINFO( SIPTimerA, SIPTimer );
  public:
    SIPTimerA( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_A, n, manager ){};
  };

  ///Timer B  64*T1            Section 17.1.1.2     INVITE transaction
  ///                                               timeout timer
  #define SIP_TIMER_B SIP_TIMER_T1*64

  class SIPTimerB : public SIPTimer
  {
    PCLASSINFO( SIPTimerB, SIPTimer );
  public:
    SIPTimerB( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_B, n, manager ){};
  };


  ///Timer C  > 3min           Section 16.6         proxy INVITE transaction
  ///                           bullet 11            timeout
  #define SIP_TIMER_C __MINUTE__(3) + __SECONDS__(1)

  class SIPTimerC : public SIPTimer
  {
    PCLASSINFO( SIPTimerC, SIPTimer );
  public:
    SIPTimerC( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_C, n, manager ){};
  };


  ///Timer D  > 32s for UDP    Section 17.1.1.2     Wait time for response
  ///         0s for TCP/SCTP                       retransmits
  #define SIP_TIMER_D __SECONDS__(32)

  class SIPTimerD : public SIPTimer
  {
    PCLASSINFO( SIPTimerD, SIPTimer );
  public:
    SIPTimerD( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_D, n, manager ){};
  };


  ///Timer E  initially T1     Section 17.1.2.2     non-INVITE request
  ///                                               retransmit interval,
  ///                                               UDP only
  #define SIP_TIMER_E SIP_TIMER_T1

  class SIPTimerE : public SIPTimer
  {
    PCLASSINFO( SIPTimerE, SIPTimer );
  public:
    SIPTimerE( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_E, n, manager ){};
  };

  ///Timer F  64*T1            Section 17.1.2.2     non-INVITE transaction
  ///                                               timeout timer
  #define SIP_TIMER_F 64*SIP_TIMER_T1

  class SIPTimerF : public SIPTimer
  {
    PCLASSINFO( SIPTimerF, SIPTimer );
  public:
    SIPTimerF( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_F, n, manager ){};
  };

  ///Timer G  initially T1     Section 17.2.1       INVITE response
  ///                                               retransmit interval
  #define SIP_TIMER_G SIP_TIMER_T1

  class SIPTimerG : public SIPTimer
  {
    PCLASSINFO( SIPTimerG, SIPTimer );
  public:
    SIPTimerG( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_G, n, manager ){};
  };

  ///Timer H  64*T1            Section 17.2.1       Wait time for
  ///                                               ACK receipt
  #define SIP_TIMER_H 64*SIP_TIMER_T1

  class SIPTimerH : public SIPTimer
  {
    PCLASSINFO( SIPTimerH, SIPTimer );
  public:
    SIPTimerH( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_H, n, manager ){};
  };

  ///Timer I  T4 for UDP       Section 17.2.1       Wait time for
  ///         0s for TCP/SCTP                       ACK retransmits
  #define SIP_TIMER_I SIP_TIMER_T4


  class SIPTimerI : public SIPTimer
  {
    PCLASSINFO( SIPTimerI, SIPTimer );
  public:
    SIPTimerI( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_I, n, manager ){};
  };
  ///Timer J  64*T1 for UDP    Section 17.2.2       Wait time for
  ///         0s for TCP/SCTP                       non-INVITE request
  ///                                               retransmits
  #define SIP_TIMER_J 64*SIP_TIMER_T1

  class SIPTimerJ : public SIPTimer
  {
    PCLASSINFO( SIPTimerJ, SIPTimer );
  public:
    SIPTimerJ( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_J, n, manager ){};
  };

  ///Timer K  T4 for UDP       Section 17.1.2.2     Wait time for
  ///         0s for TCP/SCTP                       response retransmits
  #define SIP_TIMER_K SIP_TIMER_T4

  class SIPTimerK : public SIPTimer
  {
    PCLASSINFO( SIPTimerK, SIPTimer );
  public:
    SIPTimerK( PNotifier n, SIPTimerManager & manager ) : SIPTimer( SIP_TIMER_K, n, manager ){};
  };

};

#endif





