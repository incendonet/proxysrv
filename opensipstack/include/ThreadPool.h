/*
 *
 * ThreadPool.h
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
 * $Log: ThreadPool.h,v $
 * Revision 1.2  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/09/15 09:55:51  joegenbaclor
 * Uploaded initial classes for ThreadPool implementation.
 *
 *
 */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <ptlib.h>
#include "EventQueue.h"

namespace Tools
{
  class ThreadPool : public PObject
  {
    PCLASSINFO( ThreadPool, PObject );
  public:
    ThreadPool(
      PINDEX threadCount,
      PINDEX stackSize,
      PThread::Priority priority,
      const PString & threadName
    );

    virtual ~ThreadPool();

    void GrowPool(
      PINDEX threadCount
    );

    void ShrinkPool(
      PINDEX threadCount
    );

    BOOL DoWork(
      const PNotifier & notifier,
      INT userData = 0
    );

    

  protected:
    PThread * WaitForIdleThread();

    void OnIdleThread(
      PThread * thread
    );

    PSyncPoint m_IdleThreadSync;

    PINDEX m_StackSize;
    PThread::Priority m_Priority;
    PString m_ThreadName;

    EQ::EventQueue * m_IdleThreadQueue;

    friend class Thread;

  private:

    class Thread : public PThread
    {
      PCLASSINFO( Thread, PThread );
    public:
      Thread(
        ThreadPool & threadPool,
        PINDEX stackSize, 
        PThread::Priority priority,
        const PString & threadName
      );

      ~Thread();

      void DoWork(
        const PNotifier & notifier,
        INT userData = 0
      );

      void Main();

    private:
      BOOL m_ExitFlag;
      PSyncPoint m_ThreadSync;
      PNotifier m_ThreadNotifier;
      INT m_UserData;
      ThreadPool & m_ThreadPool;
    };
  };
};

#endif
