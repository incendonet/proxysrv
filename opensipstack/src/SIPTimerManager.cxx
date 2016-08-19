/*
 *
 * SIPTimerManager.cxx
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
 * $Log: SIPTimerManager.cxx,v $
 * Revision 1.8  2006/12/22 07:40:04  joegenbaclor
 * Remove PTRACE in constructor of manager
 *
 * Revision 1.7  2006/12/20 08:57:40  joegenbaclor
 * More cleanup beautification
 *
 * Revision 1.6  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPTimerManager.h"
#include "SIPTimer.h"

#define new PNEW

using namespace SIPTimers;

#define SIP_TIMER_RESOLUTION 100



SIPTimerManager::SIPTimerManager() :
  PThread( 100000, NoAutoDeleteThread, NormalPriority, "SIP Timer Thread" )
{
  m_IsTerminating = FALSE;
  m_IsRunning = FALSE;
  m_SIPTimers.DisallowDeleteObjects();
  Resume();
  ///PTRACE( 1, "*** CREATED *** SIPTimer Manager" );
}

SIPTimerManager::~SIPTimerManager()
{
  TerminateTimers();
  PTRACE( 1, "*** DESTROYED *** SIPTimer Manager" );
}

void SIPTimerManager::TerminateTimers()
{
  if( m_IsTerminating )
    return;

  m_IsTerminating = TRUE;
  m_SIPTimers.RemoveAll();
  PAssert( WaitForTermination( 2000 ), "SIPTimerManager Failed to Termiante" );
}

void SIPTimerManager::Main()
{
  while( !m_IsTerminating )
  {
    ProcessTimers();
    m_Delay.Delay( SIP_TIMER_RESOLUTION );
  }
}

void SIPTimerManager::StopTimer(
  SIPTimer * timer
)
{
  //PAssertNULL( timer );
  AutoCriticalSection cs(*this);

  if( m_SIPTimers.Contains( timer->m_Id ) )
    timer->m_IsRunning = FALSE;

}

void SIPTimerManager::StartTimer(
  SIPTimer * timer
)
{
  //PAssertNULL( timer );
  AutoCriticalSection cs(*this);

  if( m_SIPTimers.GetAt( timer->m_Id ) == NULL )
    m_SIPTimers.SetAt( timer->m_Id, timer );

  timer->m_IsRunning = TRUE;
}


void SIPTimerManager::DestroyTimer(
  SIPTimer * timer
)
{
  //PAssertNULL( timer );
  AutoCriticalSection cs(*this);
  timer->m_IsRunning = FALSE;
  m_SIPTimers.RemoveAt( timer->m_Id );
}

void SIPTimerManager::ProcessTimers()
{
  AutoCriticalSection cs(*this);
  PIntArray expireTimer;
  PINDEX currentIndex = 0, i = 0;

  PTimeInterval start = PTimer::Tick();

  for( i = 0; i < m_SIPTimers.GetSize(); i++ )
  {
    SIPTimer & currentTimer = m_SIPTimers.GetDataAt(i);
    if( currentTimer.IsRunning() )
    {
      currentTimer.m_TimeLeft -= SIP_TIMER_RESOLUTION;
      if( currentTimer.m_TimeLeft <= 0 )
        expireTimer[currentIndex++] = currentTimer.m_Id;
    }
  }

  /// remove the time from the list;
  for( i = 0; i < expireTimer.GetSize(); i++ )
  {
    SIPTimer * timer = m_SIPTimers.RemoveAt( expireTimer[i] );

    if( timer != NULL )
    {
      timer->Fire();
      timer->Stop();
    }
  }

  PTimeInterval skew = PTimer::Tick() - start;

  //// adjust the timer intervals
  if( skew >= 1 )
  {

    //PTRACE( 1, "Adjusting timer intervals by " << skew.GetMilliSeconds() << " ms" );
    for( i = 0; i < m_SIPTimers.GetSize(); i++ )
    {
      SIPTimer & currentTimer = m_SIPTimers.GetDataAt(i);
      if( currentTimer.IsRunning() )
        if( currentTimer.m_TimeLeft > 1 )
          currentTimer.m_TimeLeft -= skew.GetInterval();
    }
  }
}

void SIPTimerManager::Deinitialize()
{

}

