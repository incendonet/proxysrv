/*
 *
 * SIPEvent.h
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
 * $Log: SIPEvent.h,v $
 * Revision 1.7  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/08/29 03:56:43  joegenbaclor
 * 1.  Added GC Reference in SIP Event
 * 2.  Corrected buf in ProcessStackEvent where session is checked against NULL not allowing requests to create new sessions
 *
 * Revision 1.5  2006/03/22 08:45:52  joegenbaclor
 * More work on simple-proxy and affected classes
 *
 * Revision 1.4  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPEVENT_H
#define SIPEVENT_H

#include <ptlib.h>
#include "Logger.h"
#include "GCObject.h"


using namespace Tools;

namespace SIPTransactions
{
  class SIPTransaction;

  class SIPEvent : public PObject
  {
    PCLASSINFO( SIPEvent, PObject );
  public:
      
    SIPEvent(
      PObject * eventObject = NULL
    ); 
    
    virtual ~SIPEvent();

    virtual void PrintOn(
      ostream & strm 
    )const;
    
    PINLINE PObject * GetEvent(){ return m_EventObject; };

    PQUEUE( Queue, SIPEvent );

  private:
    /// hide copy constructor
    SIPEvent( 
      const SIPEvent & 
    );

    SIPEvent & operator=(
      const SIPEvent & 
    );
  public:
    PINLINE Logger * GetLogger(){ return m_Logger; };
    PINLINE void SetLogger( Logger * logger ){ m_Logger = logger; }; 

  protected:
    PObject * m_EventObject;
    Logger * m_Logger;
    GC::GCReference<SIPTransaction> * m_GCRef;

    friend class SIPTransaction;
    
  };


//// Timers

  class SIPTimerEvent : public PObject
  {
    PCLASSINFO( SIPTimerEvent, PObject );
  public:
    
    enum Type { None, A, B, C, D, E, F, G, H, I, J, K, T1, T2, T4, LifeSpanExpire };

    SIPTimerEvent();

    SIPTimerEvent(
      const SIPTimerEvent & event
    );

    SIPTimerEvent(
      SIPTimerEvent::Type type,
      PInt64 interval = 0
    );

    virtual ~SIPTimerEvent();

    SIPTimerEvent & operator=(
      const SIPTimerEvent & event
    );

    SIPTimerEvent & operator=(
      SIPTimerEvent::Type type
    );

    BOOL operator==( 
      SIPTimerEvent::Type type 
    );

    virtual void PrintOn(
      ostream & strm
    )const;

    PINLINE const Type GetType()const{ return m_Type; };

    PINLINE const PInt64 GetInterval()const{ return m_Interval; };

    PINLINE void SetInterval( PInt64 interval ){ m_Interval = interval; };

  protected:

    Type m_Type;
    PInt64 m_Interval;

  };


 /// Transaction termination event
  class SIPFinalEvent : public SIPEvent
  {
    PCLASSINFO( SIPFinalEvent, SIPEvent );
  public:
    SIPFinalEvent() : SIPEvent( new PString( "Event(Final)" ) ){}
  };

  /// cancel an IST transaction
  class SIPCancelEvent : public SIPEvent
  {
    PCLASSINFO( SIPCancelEvent, SIPEvent );
  public:
    SIPCancelEvent() : SIPEvent( new PString( "Event(Cancel)" ) ){}
  };

  
}

#endif
