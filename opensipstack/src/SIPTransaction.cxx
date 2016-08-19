/*
 *
 * SIPTransaction.cxx
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
 * $Log: SIPTransaction.cxx,v $
 * Revision 1.23  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.22  2006/11/21 06:55:19  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.21  2006/11/08 06:46:21  joegenbaclor
 * Added session level NIC/Interface binding
 *
 * Revision 1.20  2006/11/07 09:30:46  joegenbaclor
 * Added new internal header "IFACE-ADDR" to allow transctions to remember the interface address for sending future responses within the transaction lifespan
 *
 * Revision 1.19  2006/09/06 07:36:38  joegenbaclor
 * Revised garbage collector so that each Garbage collectible objects will have its own
 *  instance of the GC.  This eliminates the need to cast to PObject
 *
 * Revision 1.18  2006/09/04 02:58:53  joegenbaclor
 * 1.  More bug fixes to garbage collector
 * 2.  Added new Logging macro so call System Log for fatal errors
 *
 * Revision 1.17  2006/09/01 03:12:26  joegenbaclor
 * Bulk Commit:
 *
 * 1.  A bug is discovered in SIP Transaction Destructor where destruction of the SIPTimers may cause a deadlock on the timer manager effectivily freezing the entire message Queue System.  This was fixed by introducing a separate function to destroy the timers on receipt of the final event just before the transaction is queued for deletion.
 *
 * 2.  There is bug in Invite Server Transaction where ACK transaction is added to the Ack Transaction Pool after sending og none 2xx final response.  This new transaction is never removed from the pool after Timer_H has fired.  this may result to a bad pointer if ACK is received after the timer expired since the transaction may have been deleted by then.
 *
 * 3.  A few tweaks in GC GetName func to return the Class Name if GC NAme is empty.
 *
 * Revision 1.16  2006/08/29 03:56:44  joegenbaclor
 * 1.  Added GC Reference in SIP Event
 * 2.  Corrected buf in ProcessStackEvent where session is checked against NULL not allowing requests to create new sessions
 *
 * Revision 1.15  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.14  2006/08/14 01:29:09  joegenbaclor
 * Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
 *
 * Revision 1.13  2006/04/18 07:18:21  joegenbaclor
 * Fixed unix build environment errors and warnings
 *
 * Revision 1.12  2006/03/25 18:40:21  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.11  2006/03/24 15:42:18  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.10  2006/03/22 08:45:52  joegenbaclor
 * More work on simple-proxy and affected classes
 *
 * Revision 1.9  2006/03/17 13:51:00  joegenbaclor
 * 1.  Implemented UAC Call Redirection
 * 2.  Implemented SendCancel() method in CallSession
 * 3.  Introduced ClearInternalHeaders() function in SIP Message to erase internal headers inheritted from previous transaction.
 *
 * Revision 1.8  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPTransaction.h"
#include "SIPTransactionStage.h"
#include "SIPTransactionManager.h"
#include "ossbuildopts.h"

#define new PNEW

using namespace SIPTransactions;

PInt64 SIPTransaction::m_InternalIdCounter = 0;

#if LOG_INSTANCE_COUNTER
PAtomicInteger SIPTransaction::m_InstanceCounter = 0;
#endif

GCCREATECOLLECTOR( SIPTransaction );

SIPTransaction::SIPTransaction(
  const TransactionId & id,
  Type type,
  SIPTransactionManager & manager
) : SIPTimerInterface( manager ),
    m_Type( type ),
    m_Id( id ),
    m_State( 0 ),
    m_IsTerminating( FALSE ),
    m_IsTerminated( FALSE ),
    m_Manager( manager )
{
#if LOG_INSTANCE_COUNTER
  ++SIPTransaction::m_InstanceCounter;
#endif

  m_InterfacePort = 0;

  if( SIPTransaction::m_InternalIdCounter == 0 )
    SIPTransaction::m_InternalIdCounter = PTimer::Tick().GetMilliSeconds();

  ++SIPTransaction::m_InternalIdCounter;
  ///if( ++SIPTransaction::m_InternalIdCounter == INT_MAX )
  ///  SIPTransaction::m_InternalIdCounter = 0;

  switch( type )
  {
    case ICT:
      m_InternalId = "ICT(" + PString(SIPTransaction::m_InternalIdCounter) + ")";
      break;
    case NICT:
      m_InternalId = "NICT(" + PString(SIPTransaction::m_InternalIdCounter) + ")";
      break;
    case IST:
      m_InternalId = "IST(" + PString(SIPTransaction::m_InternalIdCounter) + ")";
      break;
    case NIST:
      m_InternalId = "NIST(" + PString(SIPTransaction::m_InternalIdCounter) + ")";
      break;
  }

  ((SIPTimerInterface*)this)->m_InternalId = m_InternalId;

  StartTransactionLifeSpanTimer( SIP_MAX_TRANSACTION_LIFE_SPAN );

  LOG_IF_DEBUG( LogDetail(), m_InternalId << "\t" << "*** CREATED *** - " << m_Id );
}

SIPTransaction::~SIPTransaction()
{
#if LOG_INSTANCE_COUNTER
  --SIPTransaction::m_InstanceCounter;
#endif

  LOG_IF_DEBUG( LogDetail(), m_InternalId << "\t" << "*** DESTROYED *** - " << m_Id );
}

void SIPTransaction::EnqueueEvent(
  SIPEvent * event
)
{
  PWaitAndSignal lock( m_EventQueueMutex );
  if( m_IsTerminating && !PIsDescendant( event, SIPFinalEvent ) )
  {
    delete event;
    return;
  }

  event->m_GCRef = new SIPTransaction::GCRef( GCCreateRef( "SIPTransaction::EnqueueEvent" ) );
  
  m_EventQueue.Enqueue( event );
  m_Manager.m_TransactionStage->Enqueue( this );
 
}

SIPEvent * SIPTransaction::DequeueEvent()
{
  PWaitAndSignal lock( m_EventQueueMutex );
  return m_EventQueue.Dequeue();
}

void SIPTransaction::FlushEvents()
{
  PWaitAndSignal lock( m_InternalLock );

  for( ;; )
  {
    SIPEvent * event = DequeueEvent();
    if( event == NULL )
      break;
    delete event;
  }
}

void SIPTransaction::ProcessEvents()
{

    PWaitAndSignal lock( m_InternalLock );

    SIPTransaction::GCRef autoRef = GCCreateRef( "SIPTransaction::ProcessEvents" );
    SIPEvent * event = DequeueEvent();

    if( event == NULL )
      return;

    if( m_IsTerminated )
    {
      delete event;
      return;
    }
    
    if( PIsDescendant( event->GetEvent(), SIPTimerEvent ) )
    {
      LOG_IF_DEBUG( LogDetail(), m_InternalId << "\t" << (*(event->GetEvent())) );
      OnTimerEvent( (SIPTimerEvent&)(*(event->GetEvent())) );
    }else if( PIsDescendant( event->GetEvent(), SIPMessage ) )
    {
      const SIPMessage & msg = (const SIPMessage&)(*(event->GetEvent()));
      StartLine startLine;
      msg.GetStartLine( startLine );
      
      PString sendStatus = msg.GetInternalHeader( "RETRANSMISSION" );

      LOG_IF_DEBUG( LogDetail(), m_InternalId << "\tEvent(SIPMessage) - " << startLine );

      if( sendStatus *= "FALSE" ) 
        OnSIPEvent( (SIPMessage&)(*(event->GetEvent())), FALSE );
      else if( sendStatus.IsEmpty() )
        OnSIPEvent( (SIPMessage&)(*(event->GetEvent())), FALSE );
      else if( sendStatus *= "TRUE" ) 
        OnSIPEvent( (SIPMessage&)(*(event->GetEvent())), TRUE );

    }else if( PIsDescendant( event, SIPFinalEvent ) )
    {
      LOG_IF_DEBUG( LogDetail(), m_InternalId << "\t" << (*(event->GetEvent())) );
      OnFinalEvent();
      m_Manager.m_TransactionStage->TerminateTransaction( this );
      m_IsTerminated = TRUE;

    }else if( PIsDescendant( event, SIPCancelEvent )  )
    {
      LOG_IF_DEBUG( LogDetail(), m_InternalId << "\t" << (*(event->GetEvent())) );
      OnCancelEvent();
    }
    else
    {
      LOG_IF_DEBUG( LogDetail(), m_InternalId << "\t" << (*(event->GetEvent())) );
      OnUnknownEvent( event );
      return;
    }

    delete event;

}

void SIPTransaction::OnTimerEvent( 
  SIPTimerEvent & 
)
{
}

void SIPTransaction::OnSIPEvent(
  SIPMessage &,
  BOOL
)
{
}

void SIPTransaction::OnFinalEvent()
{
  m_IsTerminating = TRUE;
  m_Manager.RemoveTransaction( GetIdentifier() );
  RemoveAllTimers();
  FlushEvents();
}

void SIPTransaction::OnCancelEvent()
{
}

void SIPTransaction::OnUnknownEvent( 
  SIPEvent * event 
)
{
  delete event;
}

void SIPTransaction::OnTransactionLifeSpanExpire(
  SIPTimer & timer
)
{
  SIPTimerEvent * event = new SIPTimerEvent( SIPTimerEvent::LifeSpanExpire );
  event->SetInterval( timer.GetInterval() );
  EnqueueEvent( new SIPEvent( event ) );
}

void SIPTransaction::OnTimerExpire_A( 
  SIPTimer & timer 
)
{
  SIPTimerEvent * event = new SIPTimerEvent( SIPTimerEvent::A );
  event->SetInterval( timer.GetInterval() );
  EnqueueEvent( new SIPEvent( event ) );
}

void SIPTransaction::OnTimerExpire_B( 
  SIPTimer & timer 
)
{
  SIPTimerEvent * event = new SIPTimerEvent( SIPTimerEvent::B );
  event->SetInterval( timer.GetInterval() );
  EnqueueEvent( new SIPEvent( event ) );
}

void SIPTransaction::OnTimerExpire_C( 
  SIPTimer & timer 
)
{
  SIPTimerEvent * event = new SIPTimerEvent( SIPTimerEvent::C );
  event->SetInterval( timer.GetInterval() );
  EnqueueEvent( new SIPEvent( event ) );
}

void SIPTransaction::OnTimerExpire_D( 
  SIPTimer & timer 
)
{
  SIPTimerEvent * event = new SIPTimerEvent( SIPTimerEvent::D );
  event->SetInterval( timer.GetInterval() );
  EnqueueEvent( new SIPEvent( event ) );
}

void SIPTransaction::OnTimerExpire_E( 
  SIPTimer & timer 
)
{
  SIPTimerEvent * event = new SIPTimerEvent( SIPTimerEvent::E );
  event->SetInterval( timer.GetInterval() );
  EnqueueEvent( new SIPEvent( event ) );
}

void SIPTransaction::OnTimerExpire_F( 
  SIPTimer & timer 
)
{
  SIPTimerEvent * event = new SIPTimerEvent( SIPTimerEvent::F );
  event->SetInterval( timer.GetInterval() );
  EnqueueEvent( new SIPEvent( event ) );
}

void SIPTransaction::OnTimerExpire_G( 
  SIPTimer & timer 
)
{
  SIPTimerEvent * event = new SIPTimerEvent( SIPTimerEvent::G );
  event->SetInterval( timer.GetInterval() );
  EnqueueEvent( new SIPEvent( event ) );
}

void SIPTransaction::OnTimerExpire_H( 
  SIPTimer & timer 
)
{
  SIPTimerEvent * event = new SIPTimerEvent( SIPTimerEvent::H );
  event->SetInterval( timer.GetInterval() );
  EnqueueEvent( new SIPEvent( event ) );
}

void SIPTransaction::OnTimerExpire_I( 
  SIPTimer & timer 
)
{
  SIPTimerEvent * event = new SIPTimerEvent( SIPTimerEvent::I );
  event->SetInterval( timer.GetInterval() );
  EnqueueEvent( new SIPEvent( event ) );
}

void SIPTransaction::OnTimerExpire_J( 
  SIPTimer & timer 
)
{
  SIPTimerEvent * event = new SIPTimerEvent( SIPTimerEvent::J );
  event->SetInterval( timer.GetInterval() );
  EnqueueEvent( new SIPEvent( event ) );
}

void SIPTransaction::OnTimerExpire_K( 
  SIPTimer & timer 
)
{
  SIPTimerEvent * event = new SIPTimerEvent( SIPTimerEvent::K );
  event->SetInterval( timer.GetInterval() );
  EnqueueEvent( new SIPEvent( event ) );
}

void SIPTransaction::Cleanup()
{
}

void SIPTransaction::Terminate()
{
  m_IsTerminating = TRUE;
  EnqueueEvent( new SIPFinalEvent() );
}

void SIPTransaction::SetOpeningRequest( const SIPMessage & message )
{ 
  m_OpeningRequest = message;
  m_InterfaceAddress = message.GetInterfaceAddress();
  m_InterfacePort = message.GetInterfacePort();
}


