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
 * $Log: StatefulEventQueue.h,v $
 * Revision 1.4  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:13:22  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef StatefulEventQueue_H
#define StatefulEventQueue_H

#include "EventQueue.h"


namespace EQ
{
  class StatefulEventQueue : public PThread
  {
    PCLASSINFO( StatefulEventQueue, PThread );
  public:

    class Context : public PObject
    {
      PCLASSINFO( Context, PObject );
    
    public:
      class SmartPtr : public PObject
      {
        PCLASSINFO( SmartPtr, PObject );
      public:
        PINLINE SmartPtr() : m_Obj( NULL ){};
        
        PINLINE SmartPtr(
          Context * obj
        ) : m_Obj( obj )
        {
          if( m_Obj != NULL )
            m_Obj->AddRef(); 
        };

        PINLINE SmartPtr(
          const SmartPtr & autoRef
        )
        {
          if( m_Obj != NULL )
          {
            m_Obj = autoRef.m_Obj;
            m_Obj->AddRef();
          }
        }

        PINLINE ~SmartPtr()
        {
          if( m_Obj != NULL )
            m_Obj->ReleaseRef(); 
        };

        PINLINE SmartPtr & operator=( Context * obj )
        { 
          m_Obj = obj;
          if( m_Obj != NULL )
            m_Obj->AddRef(); 
          return *this;
        };
        
        PINLINE SmartPtr & operator=( const SmartPtr & autoRef )
        { 
          m_Obj = autoRef.m_Obj;
          if( m_Obj != NULL )
            m_Obj->AddRef();
          return *this;
        };

        PINLINE Context * operator->(){ return m_Obj; };
        PINLINE Context * GetPointer(){ return m_Obj; };
        PINLINE BOOL operator==( Context * obj )const{ return m_Obj == obj; };
        PINLINE BOOL operator!=( Context * obj )const{ return m_Obj != obj; };

      protected:
        Context * m_Obj;
      };

      
      Context( 
        StatefulEventQueue & eventQueue,
        PINDEX contextID,
        int initialState
      );

    protected:
      virtual ~Context();
    public:

      virtual BOOL OnTransition(
        int newState,
        int event,
        PObject * userData
      ) = 0;

      virtual void OnEvent(
        int event,
        PObject * userData
      ) = 0;

      PINLINE int GetState()const{ return m_CurrentState; };
      PINLINE void SetState( int state ){ m_CurrentState = state; };
      PINLINE int GetContextId()const{ return m_ContextId; };
   
      PINLINE int AddRef(){ return ++m_RefCount; };
      PINLINE int ReleaseRef(){ return --m_RefCount; };
      PINLINE int GetRefCount()const{ return m_RefCount; };
  
    private:
      PAtomicInteger m_RefCount;
      int m_CurrentState;
      int m_ContextId;

    protected:
      PMutex m_EventMutex;
      StatefulEventQueue & m_EventQueue;
      friend class StatefulEventQueue;

    };

    StatefulEventQueue( 
      PINDEX stacksize 
    );

    virtual ~StatefulEventQueue();

    virtual void Main();

    BOOL AddState(
      int state,
      const PString & stateName, 
      PINDEX processorCount = 1,
      PINDEX processorStackSize = 1024 * 2,
      PINDEX maxQueueSize = P_MAX_INDEX
    );

    BOOL AddTransition(
      int event,
      int currentState,
      int nextState
    );

    /// returns -1 if there is no registered transition
    int GetTransition(
      int event,
      int currentState
    );

    BOOL AddContext(
      Context * context
    );

    Context::SmartPtr GetContext(
      int contextId
    );

    void DestroyContext(
      int contextId
    );

    BOOL EnqueueEvent(
      int contextId,
      int event,
      PObject * userData = 0
    );

    virtual void OnEvent(
      int contextId,
      int event,
      PObject * userData
    );

  protected:

    PDICTIONARY( StatefulEventQueueStateList, POrdinalKey, EventQueue );
    PMutex m_StateMutex;
    StatefulEventQueueStateList m_States;

    PDICTIONARY( ContextList, POrdinalKey, Context );
    PMutex m_ContextMutex;
    ContextList m_Contexts;

    PMutex m_TransitionsMutex;
    PDICTIONARY( TransitionList, PCaselessString, POrdinalKey );
    TransitionList m_Transitions;

    PDECLARE_NOTIFIER( EventQueue, StatefulEventQueue, _OnEvent );
    
    PQUEUE( ContextQueue, Context );
    PMutex m_GarbageQueueMutex;
    PSyncPoint m_GarbageQueueSync;
    BOOL m_ExitFlag;
    ContextQueue m_GarbageQueue;
    ContextQueue m_BackLogGarbageQueue;
   
  };
};

#endif

