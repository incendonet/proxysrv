/*
 *
 * SIPTimer.cxx
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
 * $Log: SIPTimer.cxx,v $
 * Revision 1.4  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPTimer.h"
#include "SIPTimerManager.h"

#define new PNEW

using namespace SIPTimers;

PAtomicInteger SIPTimer::m_CurrentId = 0;

SIPTimer::SIPTimer( 
  SIPTimerManager & manager
  ) : m_Manager( manager )
{
  m_IsRunning = FALSE;
  m_OldInterval = -1;
  m_TimeLeft = -1;

  if( ++SIPTimer::m_CurrentId >= INT_MAX )
    SIPTimer::m_CurrentId = 1;

  m_Id = SIPTimer::m_CurrentId;

}

SIPTimer::SIPTimer(
  PInt64 interval,
  PNotifier notifier,
  SIPTimerManager & manager
  ) : m_Manager( manager )
{
  PAssert( interval > 0, PLogicError );
  m_IsRunning = FALSE;
  m_OldInterval = interval;
  m_TimeLeft = interval;
  m_Notifier = notifier;
  
  if( ++SIPTimer::m_CurrentId >= INT_MAX )
    SIPTimer::m_CurrentId = 1;

  m_Id = SIPTimer::m_CurrentId;

}

SIPTimer::~SIPTimer()
{
}

void SIPTimer::Stop()
{
  if( m_IsRunning )
  {
    m_IsRunning = FALSE;
    m_Manager.StopTimer( this );
  }
}

void SIPTimer::Resume()
{
  m_IsRunning = TRUE;
  m_Manager.StartTimer( this );
}

void SIPTimer::Fire()
{
  if( m_IsRunning )
    m_Notifier( *this, 0 );
}

void SIPTimer::Destroy()
{
  if( !m_Manager.m_IsTerminating )
  m_Manager.DestroyTimer( this );
}

