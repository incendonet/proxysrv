/*
 *
 * SIPTimerInterface.cxx
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
 * $Log: SIPTimerInterface.cxx,v $
 * Revision 1.10  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.9  2006/09/01 03:12:26  joegenbaclor
 * Bulk Commit:
 *
 * 1.  A bug is discovered in SIP Transaction Destructor where destruction of the SIPTimers may cause a deadlock on the timer manager effectivily freezing the entire message Queue System.  This was fixed by introducing a separate function to destroy the timers on receipt of the final event just before the transaction is queued for deletion.
 *
 * 2.  There is bug in Invite Server Transaction where ACK transaction is added to the Ack Transaction Pool after sending og none 2xx final response.  This new transaction is never removed from the pool after Timer_H has fired.  this may result to a bad pointer if ACK is received after the timer expired since the transaction may have been deleted by then.
 *
 * 3.  A few tweaks in GC GetName func to return the Class Name if GC NAme is empty.
 *
 * Revision 1.8  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.7  2006/03/22 08:45:52  joegenbaclor
 * More work on simple-proxy and affected classes
 *
 * Revision 1.6  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPTimerInterface.h"

#define new PNEW

using namespace SIPTimers;



SIPTimerInterface::SIPTimerInterface( 
  SIPTimerManager & manager,
  const PString & internalId 
) : m_TimerManager( manager )
{
  m_TransactionLifeSpan = NULL;
  m_TimerA = NULL;
  m_TimerB = NULL;
  m_TimerC = NULL;
  m_TimerD = NULL;
  m_TimerE = NULL;
  m_TimerF = NULL;
  m_TimerG = NULL;
  m_TimerH = NULL;
  m_TimerI = NULL;
  m_TimerJ = NULL;
  m_TimerK = NULL;
  m_InternalId = internalId;
}

SIPTimerInterface::~SIPTimerInterface()
{

}

void SIPTimerInterface::RemoveAllTimers()
{
    if( m_TransactionLifeSpan != NULL )
  {
    m_TransactionLifeSpan->Destroy();
    delete m_TransactionLifeSpan;
    m_TransactionLifeSpan = NULL;
  }

  if( m_TimerA != NULL )
  {
    m_TimerA->Destroy();
    delete m_TimerA;
    m_TimerA = NULL;
  }

  if( m_TimerB != NULL )
  {
    m_TimerB->Destroy();
    delete m_TimerB;
    m_TimerB = NULL;
  }

  if( m_TimerC != NULL )
  {
    m_TimerC->Destroy();
    delete m_TimerC;
    m_TimerC = NULL;
  }

  if( m_TimerD != NULL )
  {
    m_TimerD->Destroy();
    delete m_TimerD;
    m_TimerD = NULL;
  }

  if( m_TimerE != NULL )
  {
    m_TimerE->Destroy();
    delete m_TimerE;
    m_TimerE = NULL;
  }

  if( m_TimerF != NULL )
  {
    m_TimerF->Destroy();
    delete m_TimerF;
    m_TimerF = NULL;
  }

  if( m_TimerG != NULL )
  {
    m_TimerG->Destroy();
    delete m_TimerG;
    m_TimerG = NULL;
  }

  if( m_TimerH != NULL )
  {
    m_TimerH->Destroy();
    delete m_TimerH;
    m_TimerH = NULL;
  }

  if( m_TimerI != NULL )
  {
    m_TimerI->Destroy();
    delete m_TimerI;
    m_TimerI = NULL;
  }

  if( m_TimerJ != NULL )
  {
    m_TimerJ->Destroy();
    delete m_TimerJ;
    m_TimerJ = NULL;
  }

  if( m_TimerK != NULL )
  {
    m_TimerK->Destroy();
    delete m_TimerK;
    m_TimerK = NULL;
  }
}

void SIPTimerInterface::StartTransactionLifeSpanTimer(
  PInt64 interval
)
{
  if( m_TransactionLifeSpan != NULL )
    m_TransactionLifeSpan->Stop();
  else
    m_TransactionLifeSpan = new SIPTimer( interval, PCREATE_NOTIFIER( _OnTransactionLifeSpanExpire ), m_TimerManager );

  m_TransactionLifeSpan->SetInterval( interval );

  m_TransactionLifeSpan->Resume();
}

void SIPTimerInterface::StartTimer_A(
  PInt64 interval
)
{
  if( m_TimerA != NULL )
    m_TimerA->Stop();
  else
    m_TimerA = new SIPTimerA( PCREATE_NOTIFIER( _OnTimerExpire_A ), m_TimerManager );

  m_TimerA->SetInterval( interval );

  m_TimerA->Resume();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer A( " << interval << " ms ) STARTED" );
}

void SIPTimerInterface::StartTimer_B(
  PInt64 interval
)
{
  if( m_TimerB != NULL )
    m_TimerB->Stop();
  else
    m_TimerB = new SIPTimerB( PCREATE_NOTIFIER( _OnTimerExpire_B ), m_TimerManager );

  m_TimerB->SetInterval( interval );

  m_TimerB->Resume();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer B( " << interval << " ms ) STARTED" );

}

void SIPTimerInterface::StartTimer_C(
  PInt64 interval
)
{
  if( m_TimerC!= NULL )
    m_TimerC->Stop();
  else
    m_TimerC = new SIPTimerC( PCREATE_NOTIFIER( _OnTimerExpire_C ), m_TimerManager );

  m_TimerC->SetInterval( interval );

  m_TimerC->Resume();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer C( " << interval << " ms ) STARTED" );
}

void SIPTimerInterface::StartTimer_D(
  PInt64 interval
)
{
  if( m_TimerD != NULL )
    m_TimerD->Stop();
  else
    m_TimerD = new SIPTimerD( PCREATE_NOTIFIER( _OnTimerExpire_D), m_TimerManager );

  m_TimerD->SetInterval( interval );

  m_TimerD->Resume();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer D( " << interval << " ms ) STARTED" );
}

void SIPTimerInterface::StartTimer_E(
  PInt64 interval
)
{
  if( m_TimerE != NULL )
    m_TimerE->Stop();
  else
    m_TimerE = new SIPTimerE( PCREATE_NOTIFIER( _OnTimerExpire_E ), m_TimerManager );

  m_TimerE->SetInterval( interval );

  m_TimerE->Resume();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer E( " << interval << " ms ) STARTED" );
}

void SIPTimerInterface::StartTimer_F(
  PInt64 interval
)
{
  if( m_TimerF != NULL )
    m_TimerF->Stop();
  else
    m_TimerF = new SIPTimerF( PCREATE_NOTIFIER( _OnTimerExpire_F ), m_TimerManager );

  m_TimerF->SetInterval( interval );

  m_TimerF->Resume();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer F( " << interval << " ms ) STARTED" );
}

void SIPTimerInterface::StartTimer_G(
  PInt64 interval
)
{
  if( m_TimerG != NULL )
    m_TimerG->Stop();
  else
    m_TimerG = new SIPTimerG( PCREATE_NOTIFIER( _OnTimerExpire_G ), m_TimerManager );

  m_TimerG->SetInterval( interval );

  m_TimerG->Resume();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer G( " << interval << " ms ) STARTED" );
}

void SIPTimerInterface::StartTimer_H(
  PInt64 interval
)
{
  if( m_TimerH != NULL )
    m_TimerH->Stop();
  else
    m_TimerH = new SIPTimerH( PCREATE_NOTIFIER( _OnTimerExpire_H ), m_TimerManager );

  m_TimerH->SetInterval( interval );

  m_TimerH->Resume();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer H( " << interval << " ms ) STARTED" );
}

void SIPTimerInterface::StartTimer_I(
  PInt64 interval
)
{
  if( m_TimerI != NULL )
    m_TimerI->Stop();
  else
    m_TimerI = new SIPTimerI( PCREATE_NOTIFIER( _OnTimerExpire_I ), m_TimerManager );

  m_TimerI->SetInterval( interval );

  m_TimerI->Resume();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer I( " << interval << " ms ) STARTED" );
}

void SIPTimerInterface::StartTimer_J(
  PInt64 interval
)
{
  if( m_TimerJ != NULL )
    m_TimerJ->Stop();
  else
    m_TimerJ = new SIPTimerJ( PCREATE_NOTIFIER( _OnTimerExpire_J ), m_TimerManager );

  m_TimerJ->SetInterval( interval );

  m_TimerJ->Resume();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer J( " << interval << " ms ) STARTED" );
}

void SIPTimerInterface::StartTimer_K(
  PInt64 interval
)
{
  if( m_TimerK != NULL )
    m_TimerK->Stop();
  else
    m_TimerK = new SIPTimerK( PCREATE_NOTIFIER( _OnTimerExpire_K ), m_TimerManager );

  m_TimerK->SetInterval( interval );

  m_TimerK->Resume();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer K( " << interval << " ms ) STARTED" );
}

void SIPTimerInterface::StopTransactionLifeSpanTimer()
{
  if( m_TransactionLifeSpan != NULL )
    m_TransactionLifeSpan->Stop();
}

void SIPTimerInterface::StopTimer_A()
{
  if( m_TimerA != NULL )
    m_TimerA->Stop();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer A STOPPED" );
}

void SIPTimerInterface::StopTimer_B()
{
  if( m_TimerB != NULL )
    m_TimerB->Stop();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer B STOPPED" );
}

void SIPTimerInterface::StopTimer_C()
{
  if( m_TimerC != NULL )
    m_TimerC->Stop();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer C STOPPED" );
}

void SIPTimerInterface::StopTimer_D()
{
  if( m_TimerD != NULL )
    m_TimerD->Stop();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer D STOPPED" );
}

void SIPTimerInterface::StopTimer_E()
{
  if( m_TimerE != NULL )
    m_TimerE->Stop();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer E STOPPED" );
}

void SIPTimerInterface::StopTimer_F()
{
  if( m_TimerF != NULL )
    m_TimerF->Stop();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer F STOPPED" );
}

void SIPTimerInterface::StopTimer_G()
{
  if( m_TimerG != NULL )
    m_TimerG->Stop();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer G STOPPED" );
}

void SIPTimerInterface::StopTimer_H()
{
  if( m_TimerH != NULL )
    m_TimerH->Stop();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer H STOPPED" );
}

void SIPTimerInterface::StopTimer_I()
{
  if( m_TimerI != NULL )
    m_TimerI->Stop();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer I STOPPED" );
}

void SIPTimerInterface::StopTimer_J()
{
  if( m_TimerJ != NULL )
    m_TimerJ->Stop();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer J STOPPED" );
}

void SIPTimerInterface::StopTimer_K()
{
  if( m_TimerK != NULL )
    m_TimerK->Stop();

  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer K STOPPED" );
}

void SIPTimerInterface::OnTimerExpire_A(
  SIPTimer &
)
{
  PAssertAlways( PUnimplementedFunction );
}

void SIPTimerInterface::OnTimerExpire_B(
  SIPTimer &
)
{
  PAssertAlways( PUnimplementedFunction );
}

void SIPTimerInterface::OnTimerExpire_C(
  SIPTimer &
)
{
  PAssertAlways( PUnimplementedFunction );
}

void SIPTimerInterface::OnTimerExpire_D(
  SIPTimer &
)
{
  PAssertAlways( PUnimplementedFunction );
}

void SIPTimerInterface::OnTimerExpire_E(
  SIPTimer &
)
{
  PAssertAlways( PUnimplementedFunction );
}

void SIPTimerInterface::OnTimerExpire_F(
  SIPTimer &
)
{
  PAssertAlways( PUnimplementedFunction );
}

void SIPTimerInterface::OnTimerExpire_G(
  SIPTimer &
)
{
  PAssertAlways( PUnimplementedFunction );
}

void SIPTimerInterface::OnTimerExpire_H(
  SIPTimer &
)
{
  PAssertAlways( PUnimplementedFunction );
}

void SIPTimerInterface::OnTimerExpire_I(
  SIPTimer &
)
{
  PAssertAlways( PUnimplementedFunction );
}

void SIPTimerInterface::OnTimerExpire_J(
  SIPTimer &
)
{
  PAssertAlways( PUnimplementedFunction );
}

void SIPTimerInterface::OnTimerExpire_K(
  SIPTimer &
)
{
  PAssertAlways( PUnimplementedFunction );
}

void SIPTimerInterface::_OnTransactionLifeSpanExpire(
  SIPTimer & timer,
  INT
)
{
  OnTransactionLifeSpanExpire( timer );
}

void SIPTimerInterface::_OnTimerExpire_A(
  SIPTimer & timer,
  INT
)
{
  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer A( " << timer.GetInterval() << " ms ) EXPIRED" );
  OnTimerExpire_A( timer );
}

void SIPTimerInterface::_OnTimerExpire_B(
  SIPTimer & timer,
  INT
)
{
  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer B( " << timer.GetInterval() << " ms ) EXPIRED" );
  OnTimerExpire_B( timer );
}

void SIPTimerInterface::_OnTimerExpire_C(
  SIPTimer & timer,
  INT
)
{
  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer C( " << timer.GetInterval() << " ms ) EXPIRED" );
  OnTimerExpire_C( timer );
}

void SIPTimerInterface::_OnTimerExpire_D(
  SIPTimer & timer,
  INT
)
{
  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer D( " << timer.GetInterval() << " ms ) EXPIRED" );
  OnTimerExpire_D( timer );
}

void SIPTimerInterface::_OnTimerExpire_E(
  SIPTimer & timer,
  INT
)
{
  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer E( " << timer.GetInterval() << " ms ) EXPIRED" );
  OnTimerExpire_E( timer);
}

void SIPTimerInterface::_OnTimerExpire_F(
  SIPTimer & timer,
  INT
)
{
  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer F( " << timer.GetInterval() << " ms ) EXPIRED" );
  OnTimerExpire_F( timer );
}

void SIPTimerInterface::_OnTimerExpire_G(
  SIPTimer & timer,
  INT
)
{
  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer G( " << timer.GetInterval() << " ms ) EXPIRED" );
  OnTimerExpire_G( timer );
}

void SIPTimerInterface::_OnTimerExpire_H(
  SIPTimer & timer,
  INT
)
{
  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer H( " << timer.GetInterval() << " ms ) EXPIRED" );
  OnTimerExpire_H( timer );
}

void SIPTimerInterface::_OnTimerExpire_I(
  SIPTimer & timer,
  INT
)
{
  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer I( " << timer.GetInterval() << " ms ) EXPIRED" );
  OnTimerExpire_I( timer );
}

void SIPTimerInterface::_OnTimerExpire_J(
  SIPTimer & timer,
  INT
)
{
  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer J( " << timer.GetInterval() << " ms ) EXPIRED" );
  OnTimerExpire_J( timer );
}

void SIPTimerInterface::_OnTimerExpire_K(
  SIPTimer & timer,
  INT
)
{
  LOG_IF_DEBUG( m_TimerInterfaceLogger.LogDebugVeryHigh(), m_InternalId << "\tTimer K( " << timer.GetInterval() << " ms ) EXPIRED" );
  OnTimerExpire_K( timer );
}


