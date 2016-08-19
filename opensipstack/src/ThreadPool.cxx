/*
 *
 * ThreadPool.cxx
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
 * $Log: ThreadPool.cxx,v $
 * Revision 1.2  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/09/15 09:55:51  joegenbaclor
 * Uploaded initial classes for ThreadPool implementation.
 *
 *
 */

#include "ThreadPool.h"

using namespace Tools;

#define new PNEW

ThreadPool::ThreadPool(
  PINDEX threadCount,
  PINDEX stackSize,
  PThread::Priority priority,
  const PString & threadName
)
{
  /*
  PSyncPoint m_IdleThreadSync;
  PDICTIONARY( ThreadList, POrdinalKey, PThread );
  PMutex m_BusyThreadsMutex;
  PMutex m_IdleThreadsMutex;
  ThreadList m_BusyThreads;
  ThreadList m_IdleThreads;
  */

  /// create a blocking event queue for idle threads
  m_IdleThreadQueue = new EQ::EventQueue( TRUE );

  m_StackSize = stackSize <= 1024 ? 1024 : stackSize;
  m_Priority = priority;
  m_ThreadName = threadName;

  GrowPool( threadCount < 1 ? 1 : threadCount );
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::GrowPool(
  PINDEX threadCount
)
{
  for( PINDEX i = 0; i < threadCount; i++ )
    OnIdleThread( new Thread( *this, m_StackSize, m_Priority, m_ThreadName + "-%0x" ) );
}

void ThreadPool::ShrinkPool(
  PINDEX threadCount
)
{
  for( PINDEX i = 0; i < threadCount; i++ )
  {
    PThread * thread = WaitForIdleThread();
    if( thread != NULL )
      delete thread;
  }
}

PThread * ThreadPool::WaitForIdleThread(
)
{
  return  dynamic_cast<PThread*>(m_IdleThreadQueue->DequeueEvent());
}

BOOL ThreadPool::DoWork(
  const PNotifier & notifier,
  INT userData
)
{
  Thread * thread = dynamic_cast<Thread*>(WaitForIdleThread());
  if( thread == NULL )
    return FALSE;
  thread->DoWork( notifier, userData );

  return TRUE;
}



void ThreadPool::OnIdleThread(
  PThread * thread
)
{
  if( thread != NULL )
    m_IdleThreadQueue->EnqueueEvent( thread );
}



////////////////////////////////////////////////////////////////

ThreadPool::Thread::Thread(
  ThreadPool & threadPool,
  PINDEX stackSize, 
  PThread::Priority priority,
  const PString & threadName
) : PThread( stackSize, NoAutoDeleteThread, priority, threadName ),
    m_ThreadPool( threadPool )
{
  m_ExitFlag = FALSE;
  Resume();
}

ThreadPool::Thread::~Thread()
{
  m_ExitFlag = TRUE;
  m_ThreadSync.Signal();
  WaitForTermination();
}

void ThreadPool::Thread::DoWork(
  const PNotifier & notifier,
  INT userData
)
{
  m_ThreadNotifier = notifier;
  m_UserData = userData;
  m_ThreadSync.Signal();
}

void ThreadPool::Thread::Main()
{
  for(;;)
  {
    m_ThreadSync.Wait();
    if( m_ExitFlag )
      break;
    m_ThreadNotifier( *this, m_UserData );
    m_ThreadPool.OnIdleThread( this );
  }
}


