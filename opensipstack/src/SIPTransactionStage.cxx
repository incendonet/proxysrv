/*
 *
 * SIPTransactionStage.cxx
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
 * $Log: SIPTransactionStage.cxx,v $
 * Revision 1.5  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/08/29 03:56:44  joegenbaclor
 * 1.  Added GC Reference in SIP Event
 * 2.  Corrected buf in ProcessStackEvent where session is checked against NULL not allowing requests to create new sessions
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPTransactionStage.h"

#define new PNEW

using namespace SIPTransactions;

SIPTransactionStage::Worker::Worker( 
  SIPTransactionStage & stage 
) : PThread( 100000, NoAutoDeleteThread, NormalPriority, "Transaction" ),
    m_Stage( stage )
{
  Resume();
}

void SIPTransactionStage::Worker::Main()
{
  PTRACE( 5, "Transaction Thread [" << GetThreadId() << "] Started" );
  for(;;)
  {
    SIPTransaction * transaction = m_Stage.Dequeue();
    
    if( transaction == NULL )
      break;
    
    transaction->ProcessEvents();
  }

  PTRACE( 5, "Transaction Thread [" << GetThreadId() << "] Terminated" );
}


/////////////////////////////////////////////////////////

SIPTransactionStage::Cleaner::Cleaner( 
  SIPTransactionStage & stage 
) : PThread( 100000, NoAutoDeleteThread, HighestPriority, "Transaction Cleaner" ),
    m_Stage( stage )
{
  Resume();
}

void SIPTransactionStage::Cleaner::Main()
{
  PTRACE( 5, "Transaction Cleaner Thread [" << GetThreadId() << "] Started" );
  for(;;)
  {
  
    SIPTransaction * transaction = m_Stage.DequeueTerminatedTransaction();
    
    if( transaction == NULL )
      break;

   
    /// process pending events if any
    transaction->ProcessEvents();

    /// allow descendants to cleanup before deletion
    transaction->Cleanup();

    transaction->GCCollect();

  }

  PTRACE( 5, "Transaction Cleaner Thread [" << GetThreadId() << "] Terminated" );
}

/////////////////////////////////////////////////////////
SIPTransactionStage::SIPTransactionStage(
  PINDEX threadCount,
  PINDEX cleanerCount
)  : m_EventQueueSync( 0, INT_MAX ),
     m_CleanerQueueSync( 0, INT_MAX )
{
  if( threadCount < 1 )
    threadCount = 1;

  if( cleanerCount < 1 )
    cleanerCount = 1;

  PINDEX i = 0;
  for( i = 0; i <  threadCount; i++ )
  {
    m_ThreadPool.Append( new Worker( *this ) );
  }

  for(  i = 0; i <  cleanerCount; i++ )
  {
    m_CleanerThreadPool.Append( new Cleaner( *this ) );
  }
}

SIPTransactionStage::~SIPTransactionStage()
{
  PINDEX i = 0;
  for( i = 0; i < 1000; i++ )
  {
    m_EventQueueSync.Signal();
    PThread::Yield();
  }

  for( i = 0; i < 1000; i++ )
  {
    m_CleanerQueueSync.Signal();
    PThread::Yield();
  }

  for( i = 0; i < m_ThreadPool.GetSize(); i++ )
  {
    PAssert( m_ThreadPool[i].WaitForTermination( 5000 ), "Transaction Thread failed to terminate" );
  }

  for( i = 0; i < m_CleanerThreadPool.GetSize(); i++ )
  {
    PAssert( m_CleanerThreadPool[i].WaitForTermination( 5000 ), "Transaction Cleaner Thread failed to terminate" );
  }
}

void SIPTransactionStage::Enqueue( 
  SIPTransaction * transaction 
)
{
  {
    PWaitAndSignal lock( m_EventQueueAccessMutex );
    m_EventQueue.Enqueue( transaction );
  }
  m_EventQueueSync.Signal();
}

SIPTransaction * SIPTransactionStage::Dequeue()
{
  m_EventQueueSync.Wait();
  PWaitAndSignal lock( m_EventQueueAccessMutex );
  return m_EventQueue.Dequeue();
}

void SIPTransactionStage::TerminateTransaction( 
  SIPTransaction * transaction 
)
{
  {
    PWaitAndSignal lock( m_CleanerQueueAccessMutex );
    m_CleanerQueue.Enqueue( transaction );
  }

  m_CleanerQueueSync.Signal();
}

SIPTransaction * SIPTransactionStage::DequeueTerminatedTransaction()
{
  m_CleanerQueueSync.Wait();
  PWaitAndSignal lock( m_CleanerQueueAccessMutex );
  return m_CleanerQueue.Dequeue();
}


