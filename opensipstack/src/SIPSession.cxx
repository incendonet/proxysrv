/*
 *
 * SIPSession.cxx
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
 * $Log: SIPSession.cxx,v $
 * Revision 1.52  2007/01/22 10:01:58  joegenbaclor
 * Fixed bug in handling Notifies
 *
 * Revision 1.51  2007/01/14 13:12:56  joegenbaclor
 * Fixed compile warnings
 *
 * Revision 1.50  2007/01/12 10:49:10  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.49  2006/12/22 12:07:48  rcolobong
 * Fixed bug in InitializeDialogAsUAS where getting the via receive address should be on the response message instead on local via
 *
 * Revision 1.48  2006/12/22 03:17:57  joegenbaclor
 * Initialized m_InitialTargetAddress to 0 in session constructor.
 *
 * Revision 1.47  2006/12/21 04:14:47  joegenbaclor
 * Fixed bug in NAT traversal logic where NAT device port is not used when sending requests within dialog
 *
 * Revision 1.46  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.45  2006/11/22 07:58:20  rcolobong
 * Fix bug on IsRequestLooping method
 *
 * Revision 1.44  2006/11/22 00:05:52  joegenbaclor
 * Bug fixes for multi-listener support
 *
 * Revision 1.43  2006/11/21 06:55:19  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.42  2006/11/18 03:14:28  joegenbaclor
 * Implemented session loop detection mechanism using iid via parameter
 *
 * Revision 1.41  2006/11/17 11:23:24  joegenbaclor
 * 1.  Added Max-Forwards header to all outbound requests if not present
 * 2.  SetUACoreName() in SIPStack::OnReceivedMessage()
 *
 * Revision 1.40  2006/11/17 04:22:22  joegenbaclor
 * Added ability to remember core to use via m_UACoreName
 *
 * Revision 1.39  2006/11/08 06:46:20  joegenbaclor
 * Added session level NIC/Interface binding
 *
 * Revision 1.38  2006/10/23 20:22:28  joegenbaclor
 * Modified Registration Methods to include a reference to the Register request
 *
 * Revision 1.37  2006/10/23 15:44:58  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.36  2006/09/18 09:35:16  joegenbaclor
 * Completed initial implementation of RTP media aggregation
 *
 * Revision 1.35  2006/09/08 04:39:17  joegenbaclor
 * Implemented INFO Session Keep-alive
 *
 * Revision 1.34  2006/09/07 09:49:58  joegenbaclor
 * Fixed bug on possible race conditions when multiple thread calls Session::Destroy
 *  at the same time
 *
 * Revision 1.33  2006/09/06 07:36:38  joegenbaclor
 * Revised garbage collector so that each Garbage collectible objects will have its own
 *  instance of the GC.  This eliminates the need to cast to PObject
 *
 * Revision 1.32  2006/08/29 10:47:39  rcolobong
 * *** empty log message ***
 *
 * Revision 1.31  2006/08/29 03:56:44  joegenbaclor
 * 1.  Added GC Reference in SIP Event
 * 2.  Corrected buf in ProcessStackEvent where session is checked against NULL not allowing requests to create new sessions
 *
 * Revision 1.30  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.29  2006/08/26 14:31:44  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.28  2006/08/14 01:29:09  joegenbaclor
 * Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
 *
 * Revision 1.27  2006/08/13 06:49:08  joegenbaclor
 * introduced mutex for m_CurrentRequest
 *
 * Revision 1.26  2006/08/06 02:32:08  joegenbaclor
 * Corrected bug regarding blank call-ids for UnknownTransaction events
 *
 * Revision 1.25  2006/08/04 05:18:47  rcolobong
 * Specify Notifier in m_SelfDestructTimer
 *
 * Revision 1.24  2006/08/01 09:28:10  joegenbaclor
 * Corrected bug where InitializeDialogAsUAS may use 5060 as port even if the server is listening on a different port
 *
 * Revision 1.23  2006/07/24 07:33:56  joegenbaclor
 * Minor bug fixes allover
 *
 * Revision 1.22  2006/07/06 05:38:11  joegenbaclor
 * Final last minute changes to CBCOM hash support
 *
 * Revision 1.21  2006/06/28 14:15:00  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.20  2006/04/08 02:38:15  joegenbaclor
 * 1.  Corrected bug in call session where  OnConnected() is called befoe OnSDPOffe.
 * 2.  Corrected bug where Cancel syncpoint may not be signalled making calls to SendCancel block
 *
 * Revision 1.19  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.18  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.17  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.16  2006/03/25 18:40:21  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.15  2006/03/24 15:42:18  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.14  2006/03/21 14:11:51  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.13  2006/03/21 03:29:56  joegenbaclor
 * Added Logger functionality to log to PTrace
 *
 * Revision 1.12  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.11  2006/03/15 16:40:46  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.10  2006/03/14 17:00:18  joegenbaclor
 * Implemented Dialog methods in SIPSession object
 *
 * Revision 1.9  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPSessionManager.h"
#include "SIPSession.h"

#define new PNEW

using namespace UACORE;

PAtomicInteger SIPSession::m_InstanceCounter = 0;

GCCREATECOLLECTOR( SIPSession );

SIPSession::SIPSession(
  SIPSessionManager & sessionManager,
  const PString & sessionId
) : m_SessionManager( sessionManager ),
    m_SessionId( sessionId ),
    m_Type( MultiDirectional ),
    m_WillProcessEvents( TRUE )
{
  m_InstanceId = ++SIPSession::m_InstanceCounter;

  m_SessionRejectResponse = SIPMessage::Code404_NotFound;

  m_LogTag = "SESSION";
  m_DialogType = NotInDialog;
  m_CallId = sessionId;
  SetLogContextId( sessionId );
  m_EnableXORHash = FALSE;
  m_IsDestroyed = FALSE;
  m_InterfacePort = 0;
  m_InitialTargetAddress = 0;
  m_InitialTargetPort = 0;
  LOG_IF_DEBUG( LogInfo(), "Multidirectional Session CREATED" );

}

SIPSession::SIPSession(
  SIPSessionManager & sessionManager,
  const PString & sessionId,
  const ProfileUA & profile
) : m_SessionProfile( profile ),
    m_SessionManager( sessionManager ),
    m_SessionId( sessionId ),
    m_Type( Client ),
    m_WillProcessEvents( TRUE )
{
  m_InstanceId = ++SIPSession::m_InstanceCounter;

  m_SessionRejectResponse = SIPMessage::Code404_NotFound;

  /// IMPORTANT NOTE!!!
  /// this is a client session...  construction of the request is left to
  /// the descendant class since we do not know the request to construct at this point.
  /// Take note that the SIPSession manager will fetch
  /// the sessions callID after construction and therfore it is extremely important
  /// the the client request for this session is constructed and the m_CallId value set
   m_LogTag = "SESSION";
   m_DialogType = NotInDialog;
   m_CallId = sessionId;
   SetLogContextId( sessionId );
   m_EnableXORHash = FALSE;
   m_IsDestroyed = FALSE;
   m_InterfacePort = 0;
   m_InitialTargetAddress = 0;
   m_InitialTargetPort = 0;
   LOG_IF_DEBUG( LogInfo(), "Session CREATED" );

}

SIPSession::SIPSession(
  SIPSessionManager & sessionManager,
  const SIPMessage & request,
  const PString & sessionId
) : m_Request( request ),
    m_SessionManager( sessionManager ),
    m_SessionId( sessionId ),
    m_Type( Server ),
    m_WillProcessEvents( TRUE )
{
  m_InstanceId = ++SIPSession::m_InstanceCounter;

  m_SessionRejectResponse = SIPMessage::Code404_NotFound;

  m_LogTag = "SESSION";
  m_DialogType = NotInDialog;

  /// set the callId;
  CallId cid;
  m_Request.GetCallId( cid );
  m_CallId = cid.GetHeaderBody();

  /// set the URI
  To to;
  m_Request.GetTo( to );
  m_URI = to.GetURI();
  SetLogContextId( sessionId );
  m_EnableXORHash = m_Request.GetInternalHeader( "XORHASH" ) == "TRUE";
  m_IsDestroyed = FALSE;
  m_InterfacePort = 0;
  m_InitialTargetAddress = 0;
  m_InitialTargetPort = 0;
  LOG_IF_DEBUG( LogInfo(), "Session CREATED" );
}

SIPSession::~SIPSession()
{
  --SIPSession::m_InstanceCounter;
  LOG( LogInfo(), m_LogTag << ": Session DESTROYED" );
}

void SIPSession::PrintOn(
  ostream & strm
)const
{
  strm << m_SessionId;
}

void SIPSession::Cleanup()
{
  GCREF( "SIPSession::Cleanup" );
  m_SelfDestructTimer.Stop();
  m_NATKeepAliveTimer.Stop();
  m_SessionKeepAliveTimer.Stop();
  m_SessionKeepAliveTimeout.Stop();
}

BOOL SIPSession::SendRequest(
  const SIPMessage & request
)
{
  GCREF( "SIPSession::SendRequest" );

  SIPMessage out = request;

  /// inform the transport that this isnt a retransmission
  out.SetInternalHeader( "RETRANSMISSION", "FALSE"  );

  if( IsXORHashEnabled() )
    out.SetXORHash( TRUE );

  out.SetInterfaceAddress( m_InterfaceAddress );
  out.SetInterfacePort( m_InterfacePort );

  out.SetUACoreName( m_SessionManager.GetUACoreName() );

  out.SetViaParameter( "iid", PString( m_InstanceId ) );

  /// check if we have a Max-Forwards
  if( out.IsRequest() && !out.HasMaxForwards() )
    out.SetMaxForwards( 70 );

  if( !m_InitialTargetAddress.IsValid() )
  {
    /// set the initial target
    SIPTransport::Resolve( out, m_InitialTargetAddress, m_InitialTargetPort );
  }

  return m_SessionManager.SendRequest( out );
}

BOOL SIPSession::IsRequestLooping( const SIPMessage & request )const
{
  if( !request.IsRequest() )
    return FALSE;

  BOOL isLooping = FALSE;
  Via via;
  
  for( PINDEX i = request.GetViaSize() - 1; i >= 0; i-- )
  {
    if( request.GetViaAt( i, via ) )
    {
      PString iid;
      if( via.GetParameter( "iid", iid ) )
        if( iid.AsInteger() == m_InstanceId )
        {
          isLooping = TRUE;
          break;
        }
    }
  }

  return isLooping;
}

void SIPSession::SendAcceptByOk(
  const PString & body,
  const PString & contentType
)
{
  GCREF( "SIPSession::SendAcceptByOk" );
  PWaitAndSignal lock( m_CurrentUASRequestMutex );
  SendAcceptByOk( m_CurrentUASRequest, body, contentType );
}

void SIPSession::SendAcceptByOk(
  const SIPMessage & request,
  const PString & body,
  const PString & contentType
)
{
  GCREF( "SIPSession::SendAcceptByOk" );
  To to;
  request.GetTo( to );
  SIPMessage ok;
  request.CreateResponse( ok, SIPMessage::Code200_Ok, "" );
  ContentType ctype;
  ctype.SetHeaderBody( contentType );
  ok.SetContentType( ctype );
  ok.SetSDP( body );
  SendRequest( ok );
  LOG_IF_DEBUG( LogDetail(), "*** ACCEPTED ***" << " INCOMING request from " << to.GetURI().GetUser() );
}

BOOL SIPSession::SendAcceptByRejection(
  const SIPMessage & requestToReject,
  PINDEX statusCode,
  const PString & reasonPhrase,
  const PString & warning
)
{
  GCREF( "SIPSession::SendAcceptByRejection" );

  To to;
  requestToReject.GetTo( to );
  SIPMessage forbidden;
  requestToReject.CreateResponse( forbidden, (WORD)statusCode, reasonPhrase );

  if( !warning.IsEmpty() )
  {
    Warning h;
    h.SetHeaderBody( warning );
    forbidden.SetWarning( h );
  }

  LOG_IF_DEBUG( LogDebugVeryHigh(), "*** REJECTED ***" << " INCOMING request " << endl << requestToReject << " with " << endl << forbidden );
  return SendRequest( forbidden );
}

BOOL SIPSession::SendAcceptByRejection(
  PINDEX statusCode,
  const PString & reasonPhrase,
  const PString & warning
)
{
  PWaitAndSignal lock( m_CurrentUASRequestMutex );
  return SendAcceptByRejection( m_CurrentUASRequest, statusCode, reasonPhrase, warning );
}

BOOL SIPSession::SendAcceptByRedirection(
  const SIPMessage & requestToRedirect,
  const ContactList & contactList,
  PINDEX statusCode
)
{
  GCREF( "SIPSession::SendAcceptByRedirection" );
  To to;
  requestToRedirect.GetTo( to );
  SIPMessage redirect;
  requestToRedirect.CreateResponse( redirect, (WORD)statusCode );
  redirect.SetContactList( contactList );
  
  LOG_IF_DEBUG( LogDetail(), "*** REDIRECTING ***" << " INCOMING request from " << to.GetURI().GetUser() << " to "
    << contactList.GetAt(0) );

  return SendRequest( redirect );

}

BOOL SIPSession::SendAcceptByRedirection(
  const ContactList & contactList,
  PINDEX statusCode
)
{
  PWaitAndSignal lock( m_CurrentUASRequestMutex );
  return SendAcceptByRedirection( m_CurrentUASRequest, contactList, statusCode );
}

void SIPSession::OnIncomingSIPMessage(
  SIPMessageArrival & messageEvent
)
{
  GCREF( "SIPSession::OnIncomingSIPMessage" );
  

  if( messageEvent.GetMessage().IsRequest() )
  {
    if( !messageEvent.GetMessage().IsAck() )
    {
      PWaitAndSignal lock( m_CurrentUASRequestMutex );
      m_CurrentUASRequest = messageEvent.GetMessage();
    }
  }else
  {
    PWaitAndSignal lock( m_CurrentUACResponseMutex );
    m_CurrentUACResponse = messageEvent.GetMessage();
    
    /// Stop the keep alive timeout timer if its active
    PWaitAndSignal lock2( m_SessionKeepAliveMutex );
    if( m_SessionKeepAliveTimeout.IsRunning() )
    {
      m_SessionKeepAliveTimeout.Stop();
      m_SessionKeepAliveTimeout.SetInterval( static_cast<int>(m_SessionKeepAliveTimer.GetInterval() * .9 ));
      m_SessionKeepAliveTimeout.Resume();
    }
    
  }
}

void SIPSession::OnSIPMessageSent(
    SIPTransportWrite & messageEvent
)
{
  GCREF( "SIPSession::OnSIPMessageSent" );

  if( messageEvent.GetMessage().IsRequest() )
  {
    if( !messageEvent.GetMessage().IsAck() )
    {
      PWaitAndSignal lock( m_CurrentUACRequestMutex );
      m_CurrentUACRequest = messageEvent.GetMessage();
    }
  }else
  {
    PWaitAndSignal lock( m_CurrentUASResponseMutex );
    m_CurrentUASResponse = messageEvent.GetMessage();
  }
}

void SIPSession::OnTimerExpire(
  SIPTimerExpire & /*timerEvent*/
)
{
  GCREF( "SIPSession::OnTimerExpire" );
  Destroy();
}

void SIPSession::OnTransportFailure(
  int /*failureType*/
)
{
  GCREF( "SIPSession::OnTransportFailure" );
  Destroy();
}

void SIPSession::EnqueueSessionEvent(
  SIPSessionEvent * event
)
{
  GCREF( "SIPSession::EnqueueSessionEvent" );
  m_SessionManager.EnqueueSessionEvent( *this, event );
}

void SIPSession::OnSessionEvent(
  int event,
  const SIPMessage & eventMsg
)
{
}

void SIPSession::Destroy()
{
  PWaitAndSignal lock( m_GCMutex );
  if( m_IsDestroyed )
    return;

  m_IsDestroyed = TRUE;;
  GCREF( "SIPSession::Destroy" );
  m_SessionManager.DestroySession( m_SessionId );
}

void SIPSession::OnDestroySession()
{
}

PString SIPSession::GetInternalHeader(
  const PCaselessString & name,
  PINDEX i
)const
{
  SIPHeader h("");
  if( !GetInternalHeader( name, h, i ) )
    return PString::Empty();

  return h.GetHeaderBody();
}

BOOL SIPSession::GetInternalHeader(
  const PCaselessString & name,
  SIPHeader & header,
  PINDEX idx
)const
{
  PWaitAndSignal lock( m_InternalHeadersMutex );
  ///ParseInternalHeaders();

  PINDEX currentIndex = 0;
  BOOL ok = FALSE;
  for( PINDEX i = 0; i < m_InternalHeaders.GetSize(); i++ )
  {
    if( m_InternalHeaders[i].GetHeaderName() *= name )
    {
      if( currentIndex == idx )
      {
        header = m_InternalHeaders[i];
        ok = TRUE;
        break;
      }else
        currentIndex++;
    }
  }

  return ok;
}

BOOL SIPSession::GetInternalHeader(
  SIPHeader & h,
  PINDEX i
)const
{

  PWaitAndSignal lock( m_InternalHeadersMutex );
  ///ParseInternalHeaders();

  if( i > m_InternalHeaders.GetSize() - 1 )
    return FALSE;

  h = m_InternalHeaders[i];

  return TRUE;
}


void SIPSession::SetInternalHeader(
  const PCaselessString & name,
  const SIPHeader & header,
  PINDEX idx
)
{
  PWaitAndSignal lock( m_InternalHeadersMutex );

  PINDEX currentIndex = 0;
  BOOL ok = FALSE;
  for( PINDEX i = 0; i < m_InternalHeaders.GetSize(); i++ )
  {
    if( m_InternalHeaders[i].GetHeaderName() *= name )
    {
      if( currentIndex == idx )
      {
        m_InternalHeaders[i] = header;
        ok = TRUE;
        break;
      }else
        currentIndex++;
    }
  }
}

void SIPSession::SetInternalHeader(
  const SIPHeader & header,
  PINDEX i
)
{
  PWaitAndSignal lock( m_InternalHeadersMutex );

  if( i > m_InternalHeaders.GetSize() - 1 )
    return;

  m_InternalHeaders[i] == header;
}

void SIPSession::AddInternalHeader(
  const PString & name,
  const PString & value
)
{
  SIPHeader h( name, value );
  AddInternalHeader( h );
}

void SIPSession::AddInternalHeader(
  const SIPHeader & header
)
{
  PWaitAndSignal lock( m_InternalHeadersMutex );
  m_InternalHeaders.Append( new SIPHeader( header ) );
}

BOOL SIPSession::PopInternalHeader(
  const PCaselessString & name,
  SIPHeader & h,
  PINDEX idx
)
{
  PWaitAndSignal lock( m_InternalHeadersMutex );
  ///ParseInternalHeaders();

  PINDEX currentIndex = 0;
  BOOL ok = FALSE;
  for( PINDEX i = 0; i < m_InternalHeaders.GetSize(); i++ )
  {
    if( m_InternalHeaders[i].GetHeaderName() *= name )
    {
      if( currentIndex == idx )
      {
        h = m_InternalHeaders[i];
        m_InternalHeaders.RemoveAt(i);
        ok = TRUE;
        break;
      }else
        currentIndex++;
    }
  }

  return ok;
}

BOOL SIPSession::GetInternalHeader(
  const PCaselessString & name,
  SIPHeader & h,
  PINDEX idx
)
{
  PWaitAndSignal lock( m_InternalHeadersMutex );
  ///ParseInternalHeaders();

  PINDEX currentIndex = 0;
  BOOL ok = FALSE;
  for( PINDEX i = 0; i < m_InternalHeaders.GetSize(); i++ )
  {
    if( m_InternalHeaders[i].GetHeaderName() *= name )
    {
      if( currentIndex == idx )
      {
        h = m_InternalHeaders[i];
        ok = TRUE;
        break;
      }else
        currentIndex++;
    }
  }

  return ok;
}

BOOL SIPSession::InitializeDialogAsUAC(
  const SIPMessage & request,
  const SIPMessage & response
)
{
  PWaitAndSignal lock( m_DialogMutex );
  if( m_DialogType != NotInDialog )
    return TRUE;

  m_DialogType = UAC;
  m_LocalTag = request.GetFromTag();
  m_RemoteTag = response.GetToTag();
  m_DialogId =  DialogId( request.GetCallId(), m_LocalTag, m_RemoteTag );
  RouteList rlist = response.GetRecordRouteList();
  if( rlist.GetSize() != 0 )
  {
    /// create the route set in reverse order
    for( PINDEX i = rlist.GetSize() - 1; i >= 0; i-- )
    {
      Route route = rlist[i].GetHeaderBody();
      m_RemoteRouteSet.Append( route );
    }
  }

  /// set the via
  request.GetViaAt( 0, m_LocalVia );

  CSeq cseq;
  request.GetCSeq( cseq );
  m_LocalSequence = cseq.GetSequence();
  m_RemoteSequence = 1;

  m_RemoteContactList = response.GetContactList();
  m_LocalContactList = request.GetContactList();

  if( response.GetContactSize() > 0 )
  {
    Contact contact = response.GetContactAt( 0 );
    ContactURI contactURI;
    contact.GetURI( contactURI );

    
    m_TargetURI = contactURI.GetURI();

    if( m_TargetURI.GetUser().IsEmpty() && !m_RemoteURI.GetUser().IsEmpty() )
      m_TargetURI.SetUser( m_RemoteURI.GetUser() );

	  if( SIPTransport::IsPrivateNetwork( m_TargetURI.GetAddress() ) )
	  {
      /// if the target URI is private, then lets use the Intial target
      /// this is a necessary HACK to support UAS behind NAT
      /// If there is a proxy in the middle, we can only hope that
      /// the proxy would be able to make the same routing desicion
      /// as it did when it received the original request

  		
      if( !SIPTransport::IsLocalAddress( m_TargetURI ) && 
        m_InitialTargetAddress.IsValid() )
      {
        m_TargetURI.SetHost( m_InitialTargetAddress.AsString() );
        if( m_InitialTargetPort > 0 )
          m_TargetURI.SetPort( PString( m_InitialTargetPort ) );
      }
	  }

  }else
  {
    return FALSE;
  }

  /// prepare the URIs
  To to;
  From from;
  request.GetTo( to );
  request.GetFrom( from );
  m_LocalURI = from.GetURI();
  m_RemoteURI = to.GetURI();

  return TRUE;
}

BOOL SIPSession::InitializeDialogAsUAS(
  const SIPMessage & request,
  const SIPMessage & response
)
{
  PWaitAndSignal lock( m_DialogMutex );
  if( m_DialogType != NotInDialog )
    return TRUE;

  m_DialogType = UAS;
  m_LocalTag = response.GetToTag();
  m_RemoteTag = request.GetFromTag();
  m_DialogId =  DialogId( request.GetCallId(), m_LocalTag, m_RemoteTag );

  m_LocalContactList = response.GetContactList();
  m_RemoteContactList = request.GetContactList();

  RecordRouteList rlist = request.GetRecordRouteList();
  if( rlist.GetSize() != 0 )
  {
    /// create the route set;
    for( PINDEX i = 0; i < rlist.GetSize(); i++ )
    {
      Route route = rlist[i].GetHeaderBody();
      m_RemoteRouteSet.Append( route );
    }
  }

  /// create the via
  Via via;
  via.SetAddress( request.GetInterfaceAddress() );
  via.SetPort( request.GetInterfacePort() );
  via.SetBranch( ParserTools::GenBranchParameter() );
  via.AddParameter( "rport", "" );
  m_LocalVia = via;

  CSeq cseq;
  request.GetCSeq( cseq );
  m_RemoteSequence = cseq.GetSequence();
  m_LocalSequence = 1;

  /// prepare the URIs
  To to;
  From from;
  request.GetTo( to );
  request.GetFrom( from );
  m_LocalURI = to.GetURI();
  m_RemoteURI = from.GetURI();

  m_RemoteContactList = request.GetContactList();
  m_LocalContactList = response.GetContactList();

  if( request.GetContactSize() > 0 )
  {
    Contact contact = request.GetContactAt( 0 );
    ContactURI contactURI;
    contact.GetURI( contactURI );
    m_TargetURI = contactURI.GetURI();
    if( m_TargetURI.GetUser().IsEmpty() && !m_RemoteURI.GetUser().IsEmpty() )
      m_TargetURI.SetUser( m_RemoteURI.GetUser() );

    /// if target URI is private get it from the via received address
    if( !SIPTransport::IsLocalAddress( m_TargetURI ) && 
      SIPTransport::IsPrivateNetwork( m_TargetURI.GetAddress() ) )
	  {
      Via responseVia;
      response.GetViaAt( 0, responseVia );

      m_TargetURI.SetHost( responseVia.GetReceiveAddress().AsString() );
      if( via.GetRPort() > 0 )
      {
        m_TargetURI.SetPort( PString( responseVia.GetRPort() ) );
      }

	  }
  }else
  {
    return FALSE;
  }

  return TRUE;
}

BOOL SIPSession::CreateRequestWithinDialog(
  SIPMessage::Methods meth,
  SIPMessage & request
)const
{
  PWaitAndSignal lock( m_DialogMutex );
  if( m_DialogType == NotInDialog )
    return FALSE;

  PString method = SIPMessage::GetMethodString( meth );

  if( method.IsEmpty() )
    return FALSE;

  if( m_DialogType == NotInDialog )
    return FALSE;

  /// Create the start line
  RequestLine requestLine;
  requestLine.SetMethod( method );
  requestLine.SetRequestURI( m_TargetURI ); 
  request.SetStartLine( requestLine );

  /// Create the from uri
  From from( m_LocalURI );
  from.AddParameter( "tag", m_LocalTag, TRUE );
  request.SetFrom( from );


  /// Create the to uri
  To to( m_RemoteURI );
  to.AddParameter( "tag", m_RemoteTag, TRUE );
  request.SetTo( to );

  /// create the via
  Via via = m_LocalVia;
  via.SetBranch( ParserTools::GenBranchParameter() );
  request.AppendVia( via );
  
  /// only generate a Cseq for nonce-CANCEL and none-ACK requests

  /*
   Requests within a dialog MUST contain strictly monotonically
   increasing and contiguous CSeq sequence numbers (increasing-by-one)
   in each direction (excepting ACK and CANCEL of course, whose numbers
   equal the requests being acknowledged or cancelled).  Therefore, if
   the local sequence number is not empty, the value of the local
   sequence number MUST be incremented by one, and this value MUST be
   placed into the CSeq header field.  If the local sequence number is
   empty, an initial value MUST be chosen using the guidelines of
   Section 8.1.1.5.  The method field in the CSeq header field value
   MUST match the method of the request.
   */
  if( meth != SIPMessage::Method_ACK && meth != SIPMessage::Method_CANCEL )
    ++m_LocalSequence;
  
  /// Create the CSeq
  CSeq cseq;
  cseq.SetMethod( method );
  cseq.SetSequence( m_LocalSequence );
  request.SetCSeq( cseq );


  /// set the call-id
  request.SetCallId( m_CallId );
  
  /// Set the contact URI
  request.SetContactList( m_LocalContactList );

  /// Set the route set )
  if( m_RemoteRouteSet.GetSize() != 0 )
  {
    request.SetRouteList( m_RemoteRouteSet );
  }

  request.SetBody( "" );

  if( m_EnableXORHash )
    request.SetXORHash( TRUE );

  return TRUE;
}


BOOL SIPSession::CloneRequestWithinDialog(
  const SIPMessage & oldRequest,
  SIPMessage & request
)const
{
  request = oldRequest;

  PWaitAndSignal lock( m_DialogMutex );
  if( m_DialogType == NotInDialog )
    return FALSE;

  if( !request.IsRequest() )
    return FALSE;

  PString method = request.GetMethod().Trim().ToUpper();

  if( method.IsEmpty() )
    return FALSE;

  if( m_DialogType == NotInDialog )
    return FALSE;

  /// Create the start line
  RequestLine requestLine;
  requestLine.SetMethod( method );
  requestLine.SetRequestURI( m_TargetURI ); 
  request.SetStartLine( requestLine );

  /// Create the from uri
  From from( m_LocalURI );
  from.AddParameter( "tag", m_LocalTag, TRUE );
  request.SetFrom( from );


  /// Create the to uri
  To to( m_RemoteURI );
  to.AddParameter( "tag", m_RemoteTag, TRUE );
  request.SetTo( to );

  /// create the via
  request.RemoveAllVias();
  Via via = m_LocalVia;
  via.SetBranch( ParserTools::GenBranchParameter() );
  request.AppendVia( via );
  
  /// only generate a Cseq for nonce-CANCEL and none-ACK requests

  /*
   Requests within a dialog MUST contain strictly monotonically
   increasing and contiguous CSeq sequence numbers (increasing-by-one)
   in each direction (excepting ACK and CANCEL of course, whose numbers
   equal the requests being acknowledged or cancelled).  Therefore, if
   the local sequence number is not empty, the value of the local
   sequence number MUST be incremented by one, and this value MUST be
   placed into the CSeq header field.  If the local sequence number is
   empty, an initial value MUST be chosen using the guidelines of
   Section 8.1.1.5.  The method field in the CSeq header field value
   MUST match the method of the request.
   */
  if( method != "ACK" && method != "CANCEL" )
    ++m_LocalSequence;
  
  /// Create the CSeq
  CSeq cseq;
  cseq.SetMethod( method );
  cseq.SetSequence( m_LocalSequence );
  request.SetCSeq( cseq );


  /// set the call-id
  request.SetCallId( m_CallId );
  
  /// Set the contact URI
  request.SetContactList( m_LocalContactList );

  /// Set the route set )
  if( m_RemoteRouteSet.GetSize() != 0 )
  {
    request.SetRouteList( m_RemoteRouteSet );
  }

  request.SetBody( "" );

  if( m_EnableXORHash )
    request.SetXORHash( TRUE );

  return TRUE;
}

void SIPSession::StartNATKeepAlive(
  const SIPURI & target,
  int expires
)
{
  m_NATKeepAliveTarget = target;
  m_NATKeepAliveTimer = PTimer( expires );
  m_NATKeepAliveTimer.SetNotifier( PCREATE_NOTIFIER( OnNATKeepAlive ) );
	m_NATKeepAliveTimer.Resume(); 
}

void SIPSession::StopNATKeepAlive()
{
  m_NATKeepAliveTimer.Stop();
}

void SIPSession::OnNATKeepAlive( PTimer & timer, INT )
{
  GCREF( "SIPSession::OnNATKeepAlive" );
  SIPMessage keepAlive;
  RequestLine requestLine;
  requestLine.SetMethod( "KEEP-ALIVE" );
  requestLine.SetRequestURI( m_NATKeepAliveTarget.AsString() );
  keepAlive.SetStartLine( requestLine );
  GetSessionManager().GetUserAgent().SendRequest( keepAlive );
  timer.Reset();
  timer.Resume();
}

void SIPSession::StartSessionKeepAlive(
  const PTimeInterval & expires
)
{
  PWaitAndSignal lock( m_SessionKeepAliveMutex );
  m_SessionKeepAliveTimer = PTimer( expires );
  m_SessionKeepAliveTimer.SetNotifier( PCREATE_NOTIFIER( OnSessionKeepAlive ) );
	m_SessionKeepAliveTimer.Resume();

  m_SessionKeepAliveTimeout = PTimer( expires + 30000 );
  m_SessionKeepAliveTimeout.SetNotifier( PCREATE_NOTIFIER( OnSessionKeepAliveTimeout ) );
  m_SessionKeepAliveTimeout.Resume();
}

void SIPSession::StopSessionKeepAlive()
{
  PWaitAndSignal lock( m_SessionKeepAliveMutex );
  m_SessionKeepAliveTimer.Stop();
  m_SessionKeepAliveTimeout.Stop();
}

void SIPSession::OnSessionKeepAlive( PTimer & timer, INT )
{
  GCREF( "SIPSession::OnSessionKeepAlive" );
  PWaitAndSignal lock( m_SessionKeepAliveMutex );
  if( m_DialogType == NotInDialog )
    return;

  SIPMessage keepAlive;
  if( CreateRequestWithinDialog( SIPMessage::Method_INFO, keepAlive ) )
    SendRequest( keepAlive );

  m_SessionKeepAliveTimer.Reset();
  m_SessionKeepAliveTimer.Resume();

  LOG( LogDebug(), GetLogTag() << ":  Send Session Keep-Alive Request" ); 
}

void SIPSession::OnSessionKeepAliveTimeout( PTimer & timer, INT )
{
  GCREF( "SIPSession::OnSessionKeepAlive" );
  PWaitAndSignal lock( m_SessionKeepAliveMutex );
  m_SessionKeepAliveTimer.Stop();
  m_SessionKeepAliveTimeout.Stop();
  OnSessionKeepAliveTimeout();
}

void SIPSession::OnSessionKeepAliveTimeout()
{
  // this callback is left for implementors.  The may opt to disconnect the call
  // Its not up to us to decide what to do at the session level
}



void SIPSession::StartAutoDestructTimer(
  int expires
)
{
  m_SelfDestructTimer = PTimer( expires );

  m_SelfDestructTimer.SetNotifier( PCREATE_NOTIFIER( OnSelfDestructExpire ) );
  m_SelfDestructTimer.Resume();
}

void SIPSession::StopAutoDestructTimer()
{
  m_SelfDestructTimer.Stop();
}

void SIPSession::OnSelfDestructExpire( PTimer & timer, INT )
{
  Destroy();
}

SIPTransportManager * SIPSession::GetTransportManager()
{
  return GetSessionManager().GetTransportManager();
}



