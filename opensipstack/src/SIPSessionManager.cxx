/*
 *
 * SIPSessionManager.cxx
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
 * $Log: SIPSessionManager.cxx,v $
 * Revision 1.35  2007/01/22 10:01:58  joegenbaclor
 * Fixed bug in handling Notifies
 *
 * Revision 1.34  2007/01/12 10:49:10  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.33  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.32  2006/11/22 00:05:52  joegenbaclor
 * Bug fixes for multi-listener support
 *
 * Revision 1.31  2006/11/17 04:22:22  joegenbaclor
 * Added ability to remember core to use via m_UACoreName
 *
 * Revision 1.30  2006/11/07 07:50:32  joegenbaclor
 * Corrected bug in Session timer processing.
 *
 * Revision 1.29  2006/09/04 02:58:53  joegenbaclor
 * 1.  More bug fixes to garbage collector
 * 2.  Added new Logging macro so call System Log for fatal errors
 *
 * Revision 1.28  2006/09/02 14:30:27  joegenbaclor
 * using POrdinalKey as the GC Collector database key may result to pointer truncation
 *  in unix where void * is bigger than int.  The solution applied is to create a new
 *  GCOrdinalKey that wraps an INT
 *
 * Revision 1.27  2006/09/02 05:56:04  joegenbaclor
 * Improved ProcessStackEvent Logging
 *
 * Revision 1.26  2006/09/02 04:38:45  joegenbaclor
 * Added new Process Stack Event override to allow a GCRef as an argument
 *
 * Revision 1.25  2006/09/01 06:51:15  joegenbaclor
 * Bulk Commit:
 *
 * 1.  A bug is discovered in SIP Transaction Destructor where destruction of the SIPTimers may cause a deadlock on the timer manager effectivily freezing the entire message Queue System.  This was fixed by introducing a separate function to destroy the timers on receipt of the final event just before the transaction is queued for deletion.
 *
 * 2.  There is bug in Invite Server Transaction where ACK transaction is added to the Ack Transaction Pool after sending og none 2xx final response.  This new transaction is never removed from the pool after Timer_H has fired.  this may result to a bad pointer if ACK is received after the timer expired since the transaction may have been deleted by then.
 *
 * 3.  A few tweaks in GC GetName func to return the Class Name if GC NAme is empty.
 *
 * Revision 1.24  2006/08/30 03:42:05  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.23  2006/08/29 03:56:44  joegenbaclor
 * 1.  Added GC Reference in SIP Event
 * 2.  Corrected buf in ProcessStackEvent where session is checked against NULL not allowing requests to create new sessions
 *
 * Revision 1.22  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.21  2006/08/26 14:31:44  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.20  2006/08/18 07:12:24  joegenbaclor
 * 1.  added Function overload for GetContentType
 * 2.  Changed unix make files to not include opensbc in make process
 *
 * Revision 1.19  2006/08/09 12:17:48  joegenbaclor
 * Added RFC 3428 Support
 *
 * Revision 1.18  2006/08/06 02:32:08  joegenbaclor
 * Corrected bug regarding blank call-ids for UnknownTransaction events
 *
 * Revision 1.17  2006/06/22 08:27:57  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.16  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.15  2006/04/08 02:38:15  joegenbaclor
 * 1.  Corrected bug in call session where  OnConnected() is called befoe OnSDPOffe.
 * 2.  Corrected bug where Cancel syncpoint may not be signalled making calls to SendCancel block
 *
 * Revision 1.14  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.13  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the eventObject.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.12  2006/03/27 01:25:33  joegenbaclor
 * Some improvements on NICT and NIST state machines
 *
 * Revision 1.11  2006/03/25 18:40:21  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.10  2006/03/24 15:42:18  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.9  2006/03/15 16:40:46  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.8  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPSessionManager.h"


#define new PNEW

using namespace UACORE;

SIPSessionManager::SIPSessionManager(
  SIPUserAgent & ua,
  PINDEX sessionThreadCount,
  PINDEX stackSize,
  const PString & mgrName
) : m_UserAgent( ua )
{
  m_UACoreName = mgrName;
  m_SessionsBySessionId.DisallowDeleteObjects();
  m_SessionsByCallId.DisallowDeleteObjects();
  m_EventQueue = new EventQueue( PCREATE_NOTIFIER( OnSessionEvent ), sessionThreadCount, stackSize, P_MAX_INDEX, mgrName );
  m_ManagerEventQueue = NULL;
  SetLogContextId( "SIP Session Manager" );
}

SIPSessionManager::~SIPSessionManager()
{
  delete m_EventQueue;
}

SIPSession * SIPSessionManager::CreateMultiDirectionalSession(
  const ProfileUA & profile,
  const PString & sessionId
)
{
  PWaitAndSignal lock( m_SessionsMutex );
  SIPSession * session = OnCreateMultiDirectionalSession( profile, sessionId );

  if( session == NULL )
    return NULL;

  session->SetSessionProfile( profile );
  m_SessionsBySessionId.SetAt( session->GetSessionId(), session );
  m_SessionsByCallId.SetAt( session->GetCallId(), session );

  return session;
}

SIPSession * SIPSessionManager::OnCreateMultiDirectionalSession(
  const ProfileUA & /*profile*/,
  const PString & sessionId
)
{
  return new SIPSession( *this, sessionId );
}

SIPSession * SIPSessionManager::CreateClientSession(
  const ProfileUA & profile,
  const PString & sessionId
)
{
  PWaitAndSignal lock( m_SessionsMutex );
  SIPSession * session = OnCreateClientSession( profile, sessionId );

  if( session == NULL )
    return NULL;

  m_SessionsBySessionId.SetAt( session->GetSessionId(), session );
  m_SessionsByCallId.SetAt( session->GetCallId(), session );

  return session;

}

SIPSession * SIPSessionManager::OnCreateClientSession(
  const ProfileUA & /*profile*/,
  const PString & /*sessionId*/
)
{
  /// since we dont know what client session to create
  /// we must force implementors to return their own descendant here
  PAssertAlways( PUnimplementedFunction );
  return NULL;
}

SIPSession::GCRef SIPSessionManager::CreateServerSession(
  const SIPMessage & request
)
{
  PWaitAndSignal lock( m_SessionsMutex );
  SIPSession::GCRef session = OnCreateServerSession( request );

  if( session == NULL )
    return SIPSession::GCRef( NULL, "SIPSession" );

  m_SessionsBySessionId.SetAt( session->GetSessionId(), session.GetObject() );
  m_SessionsByCallId.SetAt( session->GetCallId(), session.GetObject() );

  return session;
}

SIPSession::GCRef  SIPSessionManager::OnCreateServerSession(
  const SIPMessage & request
)
{
  CallId callId;
  request.GetCallId( callId );
  PString sessionId = callId.GetHeaderBody();
  SIPSession * session = new SIPSession( *this, request, sessionId );
  return session->GCCreateRef( "SIPSessionManager::OnCreateServerSession" );
}

BOOL SIPSessionManager::DestroySession(
  const PString & sessionId
)
{
  PWaitAndSignal lock( m_SessionsMutex );

  SIPSession::GCRef session = FindGCRefBySessionId( sessionId ); 

  if( session == NULL )
    return FALSE;

  m_SessionsBySessionId.RemoveAt( sessionId );
  m_SessionsByCallId.RemoveAt( session->GetCallId() );

  session->m_WillProcessEvents = FALSE;

  session->OnDestroySession();

  LOG_IF_DEBUG( LogDetail(), "*** QUEUED FOR DELETION *** " << "SIPSession: " << sessionId );

  session->GCCollect();

  return TRUE;
}



SIPSession::GCRef SIPSessionManager::FindGCRefByCallId(
  const PString & callId
)
{
  PWaitAndSignal lock( m_SessionsMutex );
   
   if(callId.IsEmpty())
   {
     return SIPSession::GCRef( NULL, "SIPSession" );
   }

   SIPSession * session = m_SessionsByCallId.GetAt( callId );
   if( session != NULL )
   {
     return session->GCCreateRef( "SIPSessionManager::FindCRefByCallId" );
   }

  return SIPSession::GCRef( NULL, "SIPSession" );
}

SIPSession::GCRef SIPSessionManager::FindGCRefBySessionId(
  const PString & sessionId
)
{
  PWaitAndSignal lock( m_SessionsMutex );
  SIPSession * session = m_SessionsBySessionId.GetAt( sessionId );
  if( session != NULL )
  {
     return session->GCCreateRef( "SIPSessionManager::FindGCRefBySessionId" );
  }

  return SIPSession::GCRef( NULL, "SIPSession" );
}



void SIPSessionManager::EnqueueSessionEvent(
  SIPSession & session,
  SIPSessionEvent * eventObject
)
{
  if( session.WillProcessEvents() )
  {
    m_EventQueue->EnqueueEvent( eventObject);
  }else
  {
    delete eventObject;
  }
}

void SIPSessionManager::EnqueueSessionManagerEvent(
  PObject * eventObject
)
{
  PWaitAndSignal lock( m_ManagerEventQueueMutex );

  if( m_ManagerEventQueue == NULL )
    m_ManagerEventQueue = new EventQueue( PCREATE_NOTIFIER( OnSessionManagerEvent ), 1, 1024 * 2, P_MAX_INDEX, "SIPSessionManager" );

  m_ManagerEventQueue->EnqueueEvent( eventObject );
}

void SIPSessionManager::OnSessionEvent(
  EventQueue &,
  INT _event
)
{
  if( _event == 0 )
    return;

  SIPSessionEvent * eventObject = (SIPSessionEvent*)_event;
  SIPSession & session = eventObject->GetSession();
  if( session.WillProcessEvents() )
    session.OnSessionEvent( eventObject->GetEvent(), eventObject->GetMessage() );
  delete eventObject;
}

void SIPSessionManager::OnSessionManagerEvent(
  EventQueue &,
  INT _event
)
{
  OnSessionManagerEvent( (PObject*)_event );
}

void SIPSessionManager::OnSessionManagerEvent(
  PObject * eventObject
)
{
  /// Note:  This method is meant to be overriden by implmentors.
  LOG_IF_DEBUG( LogWarning(), "SessionManagerEvent Received without an overriden handler" );
  if( eventObject != NULL )
    delete eventObject;
}

void SIPSessionManager::ProcessStackEvent(
  SIPStackEvent * eventObject
)
{
  SIPSession::GCRef gcRef = FindGCRefByCallId( eventObject->GetCallId() );
  ProcessStackEvent( gcRef, eventObject );
}

void SIPSessionManager::ProcessStackEvent(
  SIPSession::GCRef & gcRef,
  SIPStackEvent * eventObject 
)
{
  SIPSession * session = dynamic_cast<SIPSession *>( gcRef.GetObject() );

  if( eventObject->GetType() == SIPStackEvent::UnknownTransaction )
  {
    //if( session != NULL )
    //{
      LOG_CONTEXT( LogDetail(), eventObject->GetCallId(),
        "Event"
        << ": " << "---> UnknownTransaction - " 
        << eventObject->GetMethod() );
      OnUnknownTransaction( (SIPUnknownTransaction&)*eventObject );
    //}else
    //{
    //  delete eventObject;
    //  return;  /// just discard this event.  no session to handle it
    //}
  }else
  {
    if( session == NULL && eventObject->GetType() != SIPStackEvent::Message )
    {
      delete eventObject;
      return;  /// just discard this event.  no session to handle it
    }

    switch( eventObject->GetType() )
    {
      case SIPStackEvent::Message:
        LOG_CONTEXT( LogDetail(), eventObject->GetCallId(),
        "Event" 
        << ": " << "---> Inbound - " 
        << ((SIPMessageArrival&)*eventObject).GetMessage().GetStartLine() );

        OnIncomingSIPMessage( (SIPMessageArrival&)*eventObject, session );
        break;

      case SIPStackEvent::Timer:
 
        LOG_CONTEXT( LogDetail(), eventObject->GetCallId(),
        "Event" 
        << ": " << "---> TimerExpire - " 
        << eventObject->GetMethod() );
        OnTimerExpire( (SIPTimerExpire&)*eventObject, session );
        break;

      case SIPStackEvent::TransportError:

        LOG_CONTEXT( LogDetail(), eventObject->GetCallId(),
        "Event" 
        << ": " << "---> TransportError - " 
        << eventObject->GetMethod() );
        OnTransportFailure( SIPStackEvent::TransportError, session );
        break;

      case SIPStackEvent::TransportWrite:

        LOG_CONTEXT( LogDetail(), eventObject->GetCallId(),
        "Event" 
        << ": " << "---> TransportWrite - " 
        << eventObject->GetMethod() );
        OnSIPMessageSent( (SIPTransportWrite&)*eventObject, session );
        break;

      default:
        PAssertAlways( PLogicError );
        break;
    }

  }

  delete eventObject;
}

void SIPSessionManager::OnIncomingSIPMessage(
  SIPMessageArrival & messageEvent,
  SIPSession * session
)
{
  const TransactionId & transactionId = messageEvent.GetTransactionId();
  PString callId = transactionId.GetCallId();
  SIPMessage request = messageEvent.GetMessage();

  if( session != NULL )
  {

    LOG_IF_DEBUG( session->LogDetail(), "*** MESSAGE ARRIVAL *** for SIP Session " << session->GetSessionId() );
    LOG_IF_DEBUG( session->LogDebugVeryHigh(), request );
    session->OnIncomingSIPMessage( messageEvent );
    return;
  }else
  {
    if( request.IsRequest() )
    {
      if( !request.IsAck())
      {
        SIPSession::GCRef ref = CreateServerSession( request );
        session = ref.GetObject();

        if( session != NULL )
        {
          LOG( session->LogDetail(), "*** MESSAGE ARRIVAL *** for SIP Session " << session->GetSessionId() );
          LOG_IF_DEBUG( session->LogDebugVeryHigh(), request );
          session->OnIncomingSIPMessage( messageEvent );
          return;
        }else
        {
          LOG( LogDetail(), "*** MESSAGE ARRIVAL *** No Session available to handle " << request.GetStartLine() );
          LOG_IF_DEBUG( LogDebugVeryHigh(), request );
          OnOrphanedMessage( request );
        }
      }else
      {
        OnOrphanedMessage( request );
      }

    }else  /// this is a response 
    {
        LOG_IF_DEBUG( LogWarning(), "*** Received a response without a session present!!! *** "  );
        LOG_IF_DEBUG( LogDebugVeryHigh(), request );
        OnOrphanedMessage( request );
        return;
    }
  }
  
}

void SIPSessionManager::OnSIPMessageSent(
  SIPTransportWrite & messageEvent ,
  SIPSession * session
)
{
  const TransactionId & transactionId = messageEvent.GetTransactionId();
  PString callId = transactionId.GetCallId();
  SIPMessage request = messageEvent.GetMessage();

  if( session != NULL )
  {
    SIPMessage newArrival = messageEvent.GetMessage();
    LOG_IF_DEBUG( session->LogDetail(), "*** MESSAGE SENT *** for SIP Session " << session->GetSessionId() );
    LOG_IF_DEBUG( session->LogDebugVeryHigh(), newArrival );

    session->OnSIPMessageSent( messageEvent );
    return;
  }
}

void SIPSessionManager::OnUnknownTransaction(
  SIPUnknownTransaction & eventObject
)
{
  LOG_IF_DEBUG( LogWarning(), "*** UNHANDLED *** " << eventObject );
}

void SIPSessionManager::OnOrphanedMessage(
  const SIPMessage & message
)
{
  LOG_IF_DEBUG( LogWarning(), "*** UNHANDLED *** " << endl << message.GetStartLine() );

  if( message.IsRequest() )
  {
    SIPMessage response;
    message.CreateResponse( response, SIPMessage::Code489_BadRequest );
    SendRequest( response );
  }
}


void SIPSessionManager::OnTimerExpire(
  SIPTimerExpire & timerEvent,
  SIPSession * session
)
{
  if( session != NULL )
  {
     LOG_IF_DEBUG( LogWarning(), "*** TIMER EXPIRATION *** for SIP Session " << session->GetSessionId() );
    session->OnTimerExpire( timerEvent );
  }
}

void SIPSessionManager::OnTransportFailure(
  int failureType,
  SIPSession * session
)
{
  if( session != NULL )
    session->OnTransportFailure( failureType );
}


/// Appends a via based on the ua transport 
BOOL SIPSessionManager::ConstructVia(
  const PIPSocket::Address & target,
  Via & via,
  SIPTransport::Protocol proto,
  WORD staticPort
)
{
  PIPSocket::Address addr;
  WORD port = 0;
  PString protocol;

  SIPTransportManager * transportManager = GetUserAgent().GetStack().GetTransportManager();

  if( proto == SIPTransport::UDP )
  {
    if( transportManager->GetUDPTransport() == NULL )
      return FALSE;

    if( !transportManager->GetListenerAddress(
      SIPTransport::UDP, target, addr, port ))
      return FALSE;

	
    protocol = "UDP";

  }else if( proto == SIPTransport::TCP )
  {
    if( transportManager->GetTCPTransport() == NULL )
      return FALSE;

    if( !transportManager->GetListenerAddress(
      SIPTransport::TCP, target, addr, port ))
      return FALSE;

    protocol = "TCP";

  }else if( proto == SIPTransport::TLS )
  {
    if( transportManager->GetTLSTransport() == NULL )
      return FALSE;

    if( !transportManager->GetListenerAddress(
      SIPTransport::TLS, target, addr, port ))
      return FALSE;

    protocol = "TLS";
  }

  LOG_IF_DEBUG( LogDebug(), "CREATED via=" << addr << ":" << port << " for target=" << target << " protocol=" << protocol  );

  via.SetAddress( addr );
  via.SetPort( staticPort == 0 ? port : staticPort );
  via.SetProtocol( protocol );

  return TRUE;
}

SIPTransportManager * SIPSessionManager::GetTransportManager()
{
  return GetUserAgent().GetStack().GetTransportManager();
}

BOOL SIPSessionManager::SendRequest(
  const SIPMessage & request
)
{
  TransactionId tid;
  return GetUserAgent().SendRequest( request, tid );
}

BOOL SIPSessionManager::Authenticate(
  const SIPMessage & request
)
{
  PString userName;
  PString realm;
  PString uri;
  PString nonce;
  PString opaque;
  PString response;

  ProxyAuthorization auth;

  if( !request.GetProxyAuthorization( auth ) )
  {
    if( request.HasAuthorization() )
      return Authorize( request );
    else
      return FALSE;
  }

  if( !auth.GetParameter( "username", userName ) )
    return FALSE;

  if( !auth.GetParameter( "realm", realm ) )
    return FALSE;

  if( !auth.GetParameter( "uri", uri ) )
    return FALSE;

  if( !auth.GetParameter( "nonce", nonce ) )
    return FALSE;


  if( !auth.GetParameter( "response", response ) )
    return FALSE;

  PString a1;
  PStringStream userURI;
  userURI << "sip:" << ParserTools::UnQuote( userName ) << "@" << ParserTools::UnQuote( realm );

  if( !OnRequestA1Hash( userName, realm, request.GetMethod(), a1 ) )
    return FALSE;

  MD5::A2Hash a2( "REGISTER", uri );
  MD5::MD5Authorization authorization = MD5::MD5Authorization( a1, nonce, a2 );
    
  PString md5Local =  authorization.AsString();
    
  PString md5Remote = ParserTools::UnQuote( response ); 

  return ( md5Local == md5Remote );
}

BOOL SIPSessionManager::Authorize(
  const SIPMessage & request
)
{
  PString userName;
  PString realm;
  PString uri;
  PString nonce;
  PString opaque;
  PString response;

  Authorization auth;

  if( !request.GetAuthorization( auth ) )
  {
    if( request.HasProxyAuthorization() )
      return Authenticate( request );
    else
      return FALSE;
  }

  if( !auth.GetParameter( "username", userName ) )
    return FALSE;

  if( !auth.GetParameter( "realm", realm ) )
    return FALSE;

  if( !auth.GetParameter( "uri", uri ) )
    return FALSE;

  if( !auth.GetParameter( "nonce", nonce ) )
    return FALSE;


  if( !auth.GetParameter( "response", response ) )
    return FALSE;

  ParserTools::UnQuote( uri );
  ParserTools::UnQuote( nonce );
  ParserTools::UnQuote( userName );
  ParserTools::UnQuote( realm );

  PString a1;
  PStringStream userURI;
  userURI << "sip:" << userName << "@" << realm;

  if( !OnRequestA1Hash( userName, realm, request.GetMethod(), a1 ) )
    return FALSE;

  MD5::A2Hash a2( "REGISTER", uri );
  MD5::MD5Authorization authorization = MD5::MD5Authorization( a1, nonce, a2 );
    
  PString md5Local =  authorization.AsString();
    
  PString md5Remote = ParserTools::UnQuote( response ); 

  return ( md5Local == md5Remote );
}

BOOL SIPSessionManager::OnRequestA1Hash(
  const PString & userName,
  const PString & realm,
  const PString & method,
  PString & hash
)
{
  return FALSE;
}


