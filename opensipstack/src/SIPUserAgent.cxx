/*
 *
 * SIPUserAgent.cxx
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
 * $Log: SIPUserAgent.cxx,v $
 * Revision 1.16  2007/01/14 13:12:56  joegenbaclor
 * Fixed compile warnings
 *
 * Revision 1.15  2007/01/06 07:30:33  joegenbaclor
 * Added mask to filter GetDefaultInterfaceAddress
 *
 * Revision 1.14  2006/12/22 07:37:21  joegenbaclor
 * Introduced separate funtion call  for starting of transport thread.  This is necessary
 *  to make sure that the backdoor listener is already initialized prior to calling transport
 *  reads
 *
 * Revision 1.13  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.12  2006/10/30 02:42:27  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.11  2006/06/14 08:43:38  joegenbaclor
 * Initial upload of OpenB2BUA applcation and related classes
 *
 * Revision 1.10  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.9  2006/03/27 15:25:10  joegenbaclor
 * Last minute improvements before creating a new tarball for download including
 * 1.  Major bug fix in simple-proxy where events may leak for REGISTER transactions.
 * 2.  Used incremental logging in HTTP admin.
 * 3.  Removed unused parameters in HTTPAdmin.
 * 4.  Local domain calls are now record-routed.
 * 5.  Fixed bug where ACK routeset is not popped during relay.
 *
 * Revision 1.8  2006/03/25 18:40:21  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.7  2006/03/15 16:40:46  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.6  2006/03/14 03:53:54  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPUserAgent.h"
#include "SIPSession.h"

#define new PNEW

using namespace SIP;
using namespace UACORE;


/*      
SIPUserAgent::StaleObjectCollector::StaleObjectCollector()
  : PThread( 2000, NoAutoDeleteThread, NormalPriority, "Stale Object Collector" )
{
  m_ExitFlag = FALSE;
  Resume();
}

SIPUserAgent::StaleObjectCollector::~StaleObjectCollector()
{
  m_ExitFlag = TRUE;
  m_SyncPoint.Signal();
  WaitForTermination();
}

void SIPUserAgent::StaleObjectCollector::Main()
{
  while( TRUE )
  {
    m_SyncPoint.Wait( 1000 ); 

    if( m_ExitFlag )
      return;

#if LOG_INSTANCE_COUNTER
    PTRACE( 1, 
      " MSG: " << SIPMessage::m_InstanceCounter - 1<< 
      " TRN: " << SIPTransaction::m_InstanceCounter <<
      " SSN: " << SIPSession::m_InstanceCounter
    );
#endif

    PWaitAndSignal lock( m_QueueMutex );

    SIPStackEvent::Collection m_RefedEvents;
    m_RefedEvents.DisallowDeleteObjects();
    while( TRUE )
    {
      SIPGarbageCollect * event = (SIPGarbageCollect *)m_BackLog.Dequeue();

      if( event != NULL )
      {
        if( event->GetStaleObject()->GetRefCount() == 0 )
        {
          delete event;
        }else
        {
          SIPSmartObject * obj = event->GetStaleObject();
          PTRACE( 4, "*** DELETION DELAYED *** " << *obj << " Reference Count: " << obj->GetRefCount() );
          m_RefedEvents.Append( event );
        }
      }else
      {
        break;
      }
    }

    while( m_RefedEvents.GetSize() != 0 )
    {
      m_BackLog.Enqueue( (SIPStackEvent*)m_RefedEvents.RemoveAt( 0 ) );
    }
 
    
    SIPGarbageCollect * event = (SIPGarbageCollect *)m_Queue.Dequeue();
    if( event != NULL )
    {
      if( event->GetStaleObject()->GetRefCount() == 0 )
        delete event;
      else
      {
        SIPSmartObject * obj = event->GetStaleObject();
        PTRACE( 4, "*** DELETION DELAYED *** " << *obj << " Reference Count: " << obj->GetRefCount() );
        m_BackLog.Enqueue( event );
      }
    }else
    {
      continue;
    }
  }
}

void SIPUserAgent::StaleObjectCollector::Enqueue(
  SIPStackEvent * event
)
{
  PWaitAndSignal lock( m_QueueMutex );
  m_Queue.Enqueue( event );
  m_SyncPoint.Signal();
}
      
*/


///////////////////////////////////////////////////////////////////////


SIPUserAgent::Worker::Worker( 
  SIPUserAgent & userAgent 
) : PThread( 2000, NoAutoDeleteThread, NormalPriority, "UserAgent" ),
    m_UserAgent( userAgent )
{
  Resume();
}

SIPUserAgent::Worker::~Worker()
{
  PAssert( WaitForTermination( 5000 ), "UA Thread failed to terminate!!!" );
}

void SIPUserAgent::Worker::Main()
{
    PTRACE( 5, "UA Thread [" << GetThreadId() << "] Started" );
  for(;;)
  {

    SIPStackEvent * event = m_UserAgent.ReadEvent();
    
    if( event != NULL )
    {
      if( event->GetType() == SIPStackEvent::Final )
      {
        delete event;
        break;
      }else//else if( event->GetType() == SIPStackEvent::GarbageCollect )
      //{
      //  if( ((SIPGarbageCollect*)event)->GetStaleObject()->GetRefCount() == 0 )
      //   delete event;
      //  else
      //    m_UserAgent.m_StaleObjectCollector.Enqueue( event );

      //}else
      {
        m_UserAgent.ProcessEvent( event );
      }
    }
  }

  PTRACE( 5, "UA Thread [" << GetThreadId() << "] Terminated" );
}



///////////////////////////////////////////////////////////////////////
SIPUserAgent::SIPUserAgent() 
  : m_EventQueueSync( 0, INT_MAX )
{
}

SIPUserAgent::SIPUserAgent(
  ProfileUA & profile
  ) : m_DefaultProfile( profile ),
      m_EventQueueSync( 0, INT_MAX )
{
}

SIPUserAgent::~SIPUserAgent()
{
}

BOOL SIPUserAgent::Initialize(
  PINDEX threadCount
)
{
  if( m_ThreadPool.GetSize() == 0 )
  {
    if( threadCount < 1 )
      threadCount = 1;

    PINDEX i = 0;
    for( i = 0; i <  threadCount; i++ )
    {
      m_ThreadPool.Append( new Worker( *this ) );
    }
  }

  ProfileTransport & transportProfile = GetDefaultProfile().GetTransportProfile();
  BOOL hasTransport = FALSE;
  PIPSocket::Address defaultAddress = SIPTransportManager::GetDefaultInterfaceAddress();

  if( !transportProfile.GetSTUNServerAddress().IsEmpty() )
  {
    m_SIPStack.GetTransportManager()->SetSTUNServer( transportProfile.GetSTUNServerAddress() );
  }

  if( transportProfile.IsUDPEnabled() )
  {
    if( m_SIPStack.AddTransport( 
        SIPTransport::UDP, 
        transportProfile.GetUDPListenerAddress(), 
        transportProfile.GetUDPListenerPort() ) )
    {
      hasTransport = m_SIPStack.GetTransportManager()->HasListener();

      if( !transportProfile.GetUDPListenerAddress().IsValid() )
      {
        transportProfile.SetUDPListenerAddress( defaultAddress );
      }
    }
  }

  if( transportProfile.IsTCPEnabled() )
  {
    if( m_SIPStack.AddTransport( 
        SIPTransport::TCP, 
        transportProfile.GetTCPListenerAddress(), 
        transportProfile.GetTCPListenerPort() ) )
    {
      hasTransport = TRUE;
      if( !transportProfile.GetTCPListenerAddress().IsValid() )
      {
        transportProfile.SetTCPListenerAddress( defaultAddress );
      }
    }
  }

  if( transportProfile.IsTLSEnabled() )
  {
    if( m_SIPStack.AddTransport( 
        SIPTransport::TLS, 
        transportProfile.GetTLSListenerAddress(), 
        transportProfile.GetTLSListenerPort() ) )
    {
      hasTransport = TRUE;
      if( !transportProfile.GetTLSListenerAddress().IsValid() )
      {
        transportProfile.SetTLSListenerAddress( defaultAddress );
      }
    }
  }


  return hasTransport;
}

void SIPUserAgent::Terminate()
{
  m_SIPStack.Terminate( m_ThreadPool.GetSize() );
}

void SIPUserAgent::AppendCommonHeaders(
  SIPMessage & msg,
  const ProfileUA & profile
)
{
  /// set the user agent name
  UserAgent userAgent;
  userAgent.SetHeaderBody( profile.GetUserAgentName() );
  msg.SetUserAgent( userAgent );

  /// set the date
  Date date;
  date.SetHeaderBody( ParserTools::GetRFC1123Date() );
  msg.SetDate( date );

  /// set the allowed methods
  if( profile.GetAllowedMethodSize() > 0 )
  {
    Allow allow( profile.GetAllowedMethods() );
    msg.AppendAllow( allow );
  }

  /// Set max forwards
  msg.SetMaxForwards( profile.GetMaxForwards() );

}



BOOL SIPUserAgent::SendRequest(
  const SIPMessage & request,
  TransactionId & transactionId
)
{
  SIPMessage msg = request;
  return m_SIPStack.FindTransactionAndAddEvent( msg, transactionId );
}

void SIPUserAgent::SendRequest(
  const SIPMessage & request
)
{
  return m_SIPStack.EnqueueTransportWriteEvent( request );
}

BOOL SIPUserAgent::StartTransportThreads()
{
  return GetStack().GetTransportManager()->StartThreads();
}



