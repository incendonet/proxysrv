/*
 *
 * StatefulEventQueue.h
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
 * $Log: StatefulEventQueue.cxx,v $
 * Revision 1.7  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/04/25 03:41:10  joegenbaclor
 * 1.  Added REFER UAC subscription callbacks.
 * 2.  Renamed Event class in StatefulEventQueue to avoid collision with Event SIP Header
 *
 * Revision 1.5  2006/03/14 03:53:54  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

  
#include "StatefulEventQueue.h"

using namespace EQ;

#define new PNEW

////////////////////////////////////////////////////////////////////
class StatefulEvent : public PObject
{
  PCLASSINFO( StatefulEvent, PObject );
public:
  StatefulEvent(
    int contextId,
    int event,
    PObject * userData
  );

  int m_ContextId;
  int m_Event;
  PObject * m_UserData;
};

StatefulEvent::StatefulEvent(
  int contextId,
  int event,
  PObject * userData
)
{
  m_ContextId = contextId;
  m_Event = event;
  m_UserData = userData;
}

////////////////////////////////////////////////////////////////////

StatefulEventQueue::Context::Context( 
  StatefulEventQueue & eventQueue,
  PINDEX contextId,
  int initialState
) : m_EventQueue( eventQueue )
{
  m_ContextId = contextId;
  m_CurrentState = initialState;
}

StatefulEventQueue::Context::~Context()
{
}


///////////////////////////////////////////////////////////////      


StatefulEventQueue::StatefulEventQueue( PINDEX stackSize ) 
  : PThread( stackSize, NoAutoDeleteThread, NormalPriority, "Stale Object Collector" )
{
  m_Contexts.DisallowDeleteObjects();
  m_ExitFlag = FALSE;
  Resume();
}

StatefulEventQueue::~StatefulEventQueue()
{
  m_ExitFlag = TRUE;
  m_GarbageQueueSync.Signal();
}

BOOL StatefulEventQueue::AddState(
  int state,
  const PString & stateName, 
  PINDEX processorCount,
  PINDEX processorStackSize,
  PINDEX maxQueueSize
)
{
  if( m_States.Contains( state ) )
    return FALSE;

  m_States.SetAt( state, new EventQueue( 
    PCREATE_NOTIFIER( _OnEvent ), 
    processorCount, 
    processorStackSize, 
    maxQueueSize, 
    stateName 
  ) );

  return TRUE;
}

BOOL StatefulEventQueue::AddTransition(
  int event,
  int currentState,
  int nextState
)
{
  /*
  PMutex m_TransitionsMutex;
  PDICTIONARY( TransitionList, PCaselessString, POrdinalKey );
  TransitionList m_Transitions;
  */

  PWaitAndSignal lock( m_TransitionsMutex );

  PStringStream transition;
  transition << event << ":" << currentState;

  if( m_Transitions.Contains( transition ) )
    return FALSE;

  m_Transitions.SetAt( transition, new POrdinalKey( nextState ) );
  return TRUE;
}

int StatefulEventQueue::GetTransition(
  int event,
  int currentState
)
{
  PStringStream transition;
  transition << event << ":" << currentState;
  PWaitAndSignal lock( m_TransitionsMutex );
  POrdinalKey * newState = m_Transitions.GetAt( transition );

  if( newState == NULL )
    return -1;

  return *newState;
  
}

BOOL StatefulEventQueue::AddContext(
  Context * context
)
{
  if( context == NULL )
    return FALSE;

  int id = context->GetContextId();

  PWaitAndSignal lock( m_ContextMutex );
  m_Contexts.SetAt( id, context );

  return TRUE;
}

StatefulEventQueue::Context::SmartPtr StatefulEventQueue::GetContext(
  int contextId
)
{
  PWaitAndSignal lock( m_ContextMutex );
  return Context::SmartPtr( m_Contexts.GetAt( contextId ) );
}

void StatefulEventQueue::DestroyContext(
  int contextId
)
{
  PWaitAndSignal lock1( m_ContextMutex );
  PWaitAndSignal lock2( m_GarbageQueueMutex );

  Context * context = m_Contexts.GetAt( contextId );
  if( context != NULL )
  {
    m_GarbageQueue.Enqueue( context );
    m_GarbageQueueSync.Signal();
  }
}

BOOL StatefulEventQueue::EnqueueEvent(
  int contextId,
  int event,
  PObject * userData
)
{
  PWaitAndSignal lock( m_ContextMutex );
  Context::SmartPtr context = GetContext( contextId );

  if( context == NULL )
    return FALSE;

  int state = context->GetState();

  EventQueue * queue = m_States.GetAt( state );

  if( queue == NULL )
    return FALSE;

  return queue->EnqueueEvent(  new StatefulEvent( contextId, event, userData ) );
}

void StatefulEventQueue::_OnEvent(
  EventQueue & /*queue*/,
  INT userData
)
{
  if( userData == 0 )
    return;

  StatefulEvent * event = (StatefulEvent*)userData;

  OnEvent(
    event->m_ContextId,
    event->m_Event,
    event->m_UserData
  );

  delete event;
}

void StatefulEventQueue::OnEvent(
  int contextId,
  int event,
  PObject * userData
)
{
  Context::SmartPtr context = GetContext( contextId ); 

  if( context == NULL )
  {
    /// we will assume that a virtual destructor is defined for userData
    delete userData;
    return;
  }

  /// process one event at a time for this context
  PWaitAndSignal lock( context->m_EventMutex );

  /// first report to the context about the incoming event
  context->OnEvent( event, userData );

  int currentState = context->GetState();

  int nextState = GetTransition( event, currentState );

  if( nextState != -1 )
  {
    if( context->OnTransition(
        nextState,
        event,
        userData
    ) )context->SetState( nextState );
  }
}

void StatefulEventQueue::Main()
{
  while( TRUE )
  {
    m_GarbageQueueSync.Wait( 1000 ); 

    if( m_ExitFlag )
      return;

    PWaitAndSignal lock( m_GarbageQueueMutex );

    ContextQueue staleObjects;
    staleObjects.DisallowDeleteObjects();

    while( TRUE )
    {
      Context * context = m_BackLogGarbageQueue.Dequeue();

      if( context != NULL )
      {
        if( context->GetRefCount() == 0 )
        {
          delete context;
        }else
        {
          PTRACE( 4, "*** CONTEXT DELETION DELAYED *** " << context->GetContextId() << " Reference Count: " << context->GetRefCount() );
          staleObjects.Append( context );
        }
      }else
      {
        break;
      }
    }

    while( staleObjects.GetSize() != 0 )
    {
      m_BackLogGarbageQueue.Enqueue( (Context*)staleObjects.RemoveAt( 0 ) );
    }
 
    
    Context * context = (Context *)m_GarbageQueue.Dequeue();
    if( context != NULL )
    {
      if( context->GetRefCount() == 0 )
        delete context;
      else
      {
        PTRACE( 4, "*** CONTEXT DELETION DELAYED *** " << context->GetContextId() << " Reference Count: " << context->GetRefCount() );
        m_BackLogGarbageQueue.Enqueue( context );
      }
    }else
    {
      continue;
    }
  }
}

