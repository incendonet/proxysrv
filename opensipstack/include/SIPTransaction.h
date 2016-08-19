/*
 *
 * SIPTransaction.h
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
 * $Log: SIPTransaction.h,v $
 * Revision 1.21  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.20  2006/11/21 06:55:17  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.19  2006/11/17 04:22:21  joegenbaclor
 * Added ability to remember core to use via m_UACoreName
 *
 * Revision 1.18  2006/11/08 06:01:03  joegenbaclor
 * added class keyword in GCObject friend declaration
 *
 * Revision 1.17  2006/11/07 12:43:22  joegenbaclor
 * Corrected bug in GetListenerAddress() when binding to a single interface
 *
 * Revision 1.16  2006/11/07 09:30:28  joegenbaclor
 * Added new internal header "IFACE-ADDR" to allow transctions to remember the interface address for sending future responses within the transaction lifespan
 *
 * Revision 1.15  2006/10/30 03:42:42  joegenbaclor
 * Protected the destructor from getting deleted outside of the garbage collector
 *
 * Revision 1.14  2006/09/04 02:58:52  joegenbaclor
 * 1.  More bug fixes to garbage collector
 * 2.  Added new Logging macro so call System Log for fatal errors
 *
 * Revision 1.13  2006/09/01 03:00:32  joegenbaclor
 * Bulk Commit:
 *
 * 1.  A bug is discovered in SIP Transaction Destructor where destruction of the SIPTimers may cause a deadlock on the timer manager effectivily freezing the entire message Queue System.  This was fixed by introducing a separate function to destroy the timers on receipt of the final event just before the transaction is queued for deletion.
 *
 * 2.  There is bug in Invite Server Transaction where ACK transaction is added to the Ack Transaction Pool after sending og none 2xx final response.  This new transaction is never removed from the pool after Timer_H has fired.  this may result to a bad pointer if ACK is received after the timer expired since the transaction may have been deleted by then.
 *
 * 3.  A few tweaks in GC GetName func to return the Class Name if GC NAme is empty.
 *
 * Revision 1.12  2006/08/29 03:56:43  joegenbaclor
 * 1.  Added GC Reference in SIP Event
 * 2.  Corrected buf in ProcessStackEvent where session is checked against NULL not allowing requests to create new sessions
 *
 * Revision 1.11  2006/08/28 07:18:34  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.10  2006/03/25 18:40:13  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.9  2006/03/22 08:45:52  joegenbaclor
 * More work on simple-proxy and affected classes
 *
 * Revision 1.8  2006/03/17 13:50:54  joegenbaclor
 * 1.  Implemented UAC Call Redirection
 * 2.  Implemented SendCancel() method in CallSession
 * 3.  Introduced ClearInternalHeaders() function in SIP Message to erase internal headers inheritted from previous transaction.
 *
 * Revision 1.7  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPTRANSACTION_H
#define SIPTRANSACTION_H

#include <ptlib.h>
#include <ptlib/sockets.h>
#include "SIPTimerInterface.h"
#include "SIPEvent.h"
#include "SIPMessage.h"
#include "GCObject.h"
#include "Logger.h"

using namespace SIPTimers;
using namespace SIPParser;
using namespace Tools;
using namespace GC;

namespace SIPTransactions
{
  class SIPTransactionStage;
  class SIPTransactionManager;

  class SIPTransaction : public SIPTimerInterface, public Logger
  {
    PCLASSINFO( SIPTransaction, SIPTimerInterface );
    GCREFINFO( SIPTransaction, "SIPTransaction" );
  public:

    enum Type
    {
      ICT,
      IST,
      NICT,
      NIST
    };

    SIPTransaction(
      const TransactionId & id,
      Type type,
      SIPTransactionManager & manager
    );
    
    /// protect the destructor from 
    /// getting deleted outside of the garbage collector
  protected:
    virtual ~SIPTransaction();
    friend class GCObject<SIPTransaction>;
  public:

    void EnqueueEvent(
      SIPEvent * event 
    );

    SIPEvent * DequeueEvent();
  
    void FlushEvents();

    void ProcessEvents();

    virtual void OnTimerEvent(
      SIPTimerEvent & timerEvent  
    );

    virtual void OnSIPEvent(
      SIPMessage & message,
      BOOL fromTransport
    );

    virtual void OnFinalEvent();

    virtual void OnCancelEvent();

    virtual void OnUnknownEvent( 
      SIPEvent * event 
    );

    virtual void Cleanup();

    /// Timer callbacks
    virtual void OnTransactionLifeSpanExpire(
      SIPTimer & timer
    );

    virtual void OnTimerExpire_A( 
      SIPTimer & timer 
    );

    virtual void OnTimerExpire_B( 
      SIPTimer & timer 
    );

    virtual void OnTimerExpire_C( 
      SIPTimer & timer 
    );

    virtual void OnTimerExpire_D( 
      SIPTimer & timer 
    );

    virtual void OnTimerExpire_E( 
      SIPTimer & timer 
    );

    virtual void OnTimerExpire_F( 
      SIPTimer & timer 
    );

    virtual void OnTimerExpire_G( 
      SIPTimer & timer 
    );

    virtual void OnTimerExpire_H( 
      SIPTimer & timer 
    );

    virtual void OnTimerExpire_I( 
      SIPTimer & timer 
    );

    virtual void OnTimerExpire_J( 
      SIPTimer & timer 
    );

    virtual void OnTimerExpire_K( 
      SIPTimer & timer 
    );

    void SetOpeningRequest( const SIPMessage & message );

    PINLINE void SetIdentifier( const PString & id ){ m_Id = id; };
    PINLINE const PString & GetCallId()const{ return m_CallId; };
    PINLINE const TransactionId & GetIdentifier()const{ return m_Id; };
    PINLINE Type GetType(){ return m_Type; };
    PINLINE int GetState()const{ return m_State; };
    PINLINE void SetState( int state ){ m_State = state; };
    PINLINE const SIPMessage & GetOpeningRequest()const{ return m_OpeningRequest; };
    PINLINE const PString & GetInterfaceAddress()const{ return m_InterfaceAddress; };
    PINLINE WORD GetInterfacePort()const{ return m_InterfacePort; };
    PQUEUE( Queue, SIPTransaction );

    PDICTIONARY( Dictionary, PCaselessString, SIPTransaction );

    PINLINE const PString & GetInternalId()const{ return m_InternalId; };
    PINLINE void SetUACoreName( const PString & name ){ m_UACoreName = name; };
    PINLINE const PString & GetUACoreName()const{ return m_UACoreName; };

  protected:

  void Terminate();

  protected:

    Type m_Type;

    PMutex m_EventQueueMutex;
    PMutex m_InternalLock;
    SIPEvent::Queue m_EventQueue;
    TransactionId m_Id;
    PAtomicInteger m_State;
    SIPMessage m_OpeningRequest;
    BOOL m_IsTerminating;
    BOOL m_IsTerminated;
    PString m_InternalId;
    SIPTransactionManager & m_Manager;
    PString m_CallId;
    PString m_InterfaceAddress;
    WORD m_InterfacePort;
    PString m_UACoreName;

    static PInt64 m_InternalIdCounter;
    friend class SIPTransactionManager;
    friend class SIPTransactionStage;

    public:
#if LOG_INSTANCE_COUNTER
      static PAtomicInteger m_InstanceCounter;
#endif

  };
};

#endif
