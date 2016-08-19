/*
 *
 * EventQueue.cxx
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
 * $Log: EventQueue.cxx,v $
 * Revision 1.8  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.7  2006/07/24 07:33:56  joegenbaclor
 * Minor bug fixes allover
 *
 * Revision 1.6  2006/05/29 10:33:59  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.5  2006/05/26 06:29:29  joegenbaclor
 * Modifed EventQueue to allow external Dequeue()
 *
 * Revision 1.4  2006/03/25 18:40:21  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "EventQueue.h"

#define new PNEW

using namespace EQ;




/////////////////////////////////////////////

EventQueue::EventQueue( 
  BOOL blockIfEmpty
)
: m_EventQueueSync( 0, P_MAX_INDEX ),
  m_MaxQueueSize( P_MAX_INDEX )
{
  m_BlockIfEmpty = blockIfEmpty;
}

EventQueue::EventQueue(
  const PNotifier & eventNotifier,
  PINDEX processorCount,
  PINDEX processorStackSize,
  PINDEX maxQueueSize,
  const PString & queueName
) : m_EventQueueSync( 0, P_MAX_INDEX ),
    m_MaxQueueSize( maxQueueSize )
{
  m_BlockIfEmpty = TRUE;
  m_EventNotifier = eventNotifier;

  for( PINDEX i = 0; i < processorCount; i++ )
    m_ProcessorList.Append( new EventQueue::QueueProcessor( processorStackSize, *this, queueName ) );
}

EventQueue::~EventQueue()
{
  for( PINDEX i = 0; i < m_ProcessorList.GetSize(); i++ )
    m_EventQueueSync.Signal();

  for( PINDEX j = 0; j < m_ProcessorList.GetSize(); j++ )
    m_ProcessorList[j].WaitForTermination();
}

/// will return FALSE if max queue size is reached

BOOL EventQueue::EnqueueEvent( 
  PObject * eventObject 
)
{
  {
    PWaitAndSignal lock( m_EventQueueMutex );

    if( m_EventQueue.GetSize() >= m_MaxQueueSize )
      return FALSE;
      
    m_EventQueue.Enqueue( new QueueObject( eventObject ) );
  }

  if( m_BlockIfEmpty )
    m_EventQueueSync.Signal();

  return TRUE;
}

void  EventQueue::FlushEvents()
{
  PWaitAndSignal lock( m_EventQueueMutex );
  m_EventQueue.RemoveAll();
}

PObject * EventQueue::DequeueEvent()
{
  EventQueue::QueueObject * qObject = Dequeue();

  if( qObject == NULL )
    return NULL;
  
  return qObject->GetEvent();
}


#if defined(_MSC_VER)
#pragma warning( disable:4311 )
#endif

void EventQueue::OnEventDequeue( 
  PObject * eventObject 
)
{
  m_EventNotifier( *this, (INT)eventObject );
}


EventQueue::QueueObject * EventQueue::Dequeue()
{
  if( m_BlockIfEmpty )
    m_EventQueueSync.Wait();
  
  PWaitAndSignal lock( m_EventQueueMutex );
  return m_EventQueue.Dequeue();
}



///////////////////////////////////////


/////////////////////////////////////////////

EventQueue::QueueObject::QueueObject(
  PObject * eventObject
) : m_EventObject( eventObject )
{
}

EventQueue::QueueObject::~QueueObject()
{
}

/////////////////////////////////////////////

EventQueue::QueueProcessor::QueueProcessor(
  PINDEX stackSize,
  EventQueue & queue,
  const PString & threadName
) : PThread( stackSize, PThread::NoAutoDeleteThread, PThread::NormalPriority, threadName ),
    m_EventQueue( queue )
{
  Resume();
}

EventQueue::QueueProcessor::~QueueProcessor()
{
}

#if defined(_MSC_VER)
#pragma warning( disable:4127 )
#endif

void EventQueue::QueueProcessor::Main()
{
  while( TRUE )
  {
    QueueObject * obj = m_EventQueue.Dequeue();
    if( obj == NULL )
      return;
    
    m_EventQueue.OnEventDequeue( obj->GetEvent() );

    delete obj;

  };
}




