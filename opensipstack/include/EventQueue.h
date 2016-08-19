/*
 *
 * EventQueue.h
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
 * $Log: EventQueue.h,v $
 * Revision 1.7  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/07/24 07:33:56  joegenbaclor
 * Minor bug fixes allover
 *
 * Revision 1.5  2006/05/29 10:33:59  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.4  2006/05/26 06:29:28  joegenbaclor
 * Modifed EventQueue to allow external Dequeue()
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <ptlib.h>

namespace EQ
{
  class EventQueue : public PObject
  {
    PCLASSINFO( EventQueue, PObject );
  public:
    //---
    class QueueObject : public PObject
    {
      PCLASSINFO( QueueObject, PObject );
    
     public:
      QueueObject(
        PObject * eventObject
      );

      ~QueueObject();

   
      PINLINE PObject * GetEvent()const{ return m_EventObject; };

      PQUEUE( Queue, QueueObject );

    protected:
      PObject * m_EventObject;

      friend class EventQueue;
      friend class QueueProcessor;
    };

    //---

    class QueueProcessor : public PThread
    {
      PCLASSINFO( QueueProcessor, PThread );
     public:
      QueueProcessor(
        PINDEX stackSize,
        EventQueue & queue,
        const PString & threadName
      );

      ~QueueProcessor();

      virtual void Main();

      PLIST( Collection, QueueProcessor );
      EventQueue & m_EventQueue;
      friend class EventQueue;
    };

    //---

    EventQueue(
      BOOL blockIfEmpty = FALSE
    );

    EventQueue(
      const PNotifier & eventNotifier,
      PINDEX processorCount,
      PINDEX processorStackSize = 1024 * 2,
      PINDEX maxQueueSize = P_MAX_INDEX,
      const PString & queueName = "Event Queue" 
    );

    virtual ~EventQueue();

    /// will return FALSE if max queue size is reached

    BOOL EnqueueEvent( 
      PObject * eventObject 
    );

    void FlushEvents();

    PObject * DequeueEvent();

    
    PINLINE const PString & GetQueueName()const{ return m_QueueName; };

  protected:

    QueueObject * Dequeue();

    void OnEventDequeue( 
      PObject * eventObject 
    );

    
    QueueProcessor::Collection m_ProcessorList;
    PNotifier m_EventNotifier;
    QueueObject::Queue m_EventQueue;
    PSemaphore m_EventQueueSync;
    PMutex m_EventQueueMutex;
    PINDEX m_MaxQueueSize;
    PString m_QueueName;
    BOOL m_BlockIfEmpty;

    friend class QueueProcessor;
    
  };
};

#endif

