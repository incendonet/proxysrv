/*
 *
 * ProxySessionManager.cxx
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
 * $Log: ProxySessionManager.cxx,v $
 * Revision 1.17  2007/01/12 10:49:10  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.16  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.15  2006/08/30 03:42:05  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.14  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.13  2006/05/19 06:30:37  joegenbaclor
 * 1.  Fixed bug in SIPHeaderB where tag may not be parsed properly if no < > enclosure is present
 * 2.  Various enhancements to OpenSBC and proxy session
 *
 * Revision 1.12  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.11  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.10  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.9  2006/03/27 15:25:10  joegenbaclor
 * Last minute improvements before creating a new tarball for download including
 * 1.  Major bug fix in simple-proxy where events may leak for REGISTER transactions.
 * 2.  Used incremental logging in HTTP admin.
 * 3.  Removed unused parameters in HTTPAdmin.
 * 4.  Local domain calls are now record-routed.
 * 5.  Fixed bug where ACK routeset is not popped during relay.
 *
 * Revision 1.8  2006/03/27 01:25:33  joegenbaclor
 * Some improvements on NICT and NIST state machines
 *
 * Revision 1.7  2006/03/25 18:40:21  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.6  2006/03/24 15:42:18  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.5  2006/03/22 08:45:52  joegenbaclor
 * More work on simple-proxy and affected classes
 *
 * Revision 1.4  2006/03/14 17:00:18  joegenbaclor
 * Implemented Dialog methods in SIPSession object
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "ProxySessionManager.h"

using namespace UACORE;

#define new PNEW
///////////////////////////////////////////////////////////////////////////////

ProxySessionTupleManager::ProxySessionTupleManager()
{
}

BOOL ProxySessionTupleManager::RemoveTuple(
  const SIPMessage & routedMsg
)
{
  PWaitAndSignal lock( m_CacheMutex );
  PString callId = routedMsg.GetCallId();
  RemoveAt( callId );
  return TRUE;
}

BOOL ProxySessionTupleManager::CreateTuple(
  const SIPMessage & senderMsg,
  const SIPMessage & routedMsg,
  int expire
)
{
  PWaitAndSignal lock( m_CacheMutex );
  PString callId = senderMsg.GetCallId();
  RemoveAt( callId );
  return Append( new CacheObject( callId, new Tuple( senderMsg, routedMsg ), expire ) );
}

BOOL ProxySessionTupleManager::FindTuple(
  const PString & callId,
  SIPMessage & originalRequest,
  SIPMessage & routedRequest
)
{
  PWaitAndSignal lock( m_CacheMutex );

  CacheObject * cache = GetAt( callId );
  if( cache == NULL )
    return FALSE;

  Tuple * tuple = (Tuple *)cache->GetObject();
  if( tuple == NULL )
    return FALSE;

  SIPMessage o = tuple->GetOriginalRequest();
  SIPMessage r = tuple->GetRoutedRequest();

  originalRequest = o;
  routedRequest = r;

  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

ProxySessionTupleManager::Tuple::Tuple()
{
}

ProxySessionTupleManager::Tuple::Tuple( 
  const Tuple & tuple 
)
{
  operator=( tuple );
}

ProxySessionTupleManager::Tuple::Tuple(
  const SIPMessage & originalRequest,
  const SIPMessage & routedRequest
)
{
  m_OriginalRequest = originalRequest;
  m_RoutedRequest = routedRequest;
}

ProxySessionTupleManager::Tuple::~Tuple()
{
}

ProxySessionTupleManager::Tuple & 
ProxySessionTupleManager::Tuple::operator=( const ProxySessionTupleManager::Tuple & tuple )
{
  m_OriginalRequest = tuple.m_OriginalRequest;
  m_RoutedRequest = tuple.m_RoutedRequest;
  return *this;
}

///////////////////////////////////////////////////////////////////////////////

ProxySessionManager::ProxySessionManager(
  SIPUserAgent & ua,
  PINDEX sessionThreadCount,
  PINDEX stackSize
) : SIPSessionManager( ua, sessionThreadCount, stackSize, "Proxy" )
{
}

ProxySessionManager::~ProxySessionManager()
{
}

SIPSession::GCRef  ProxySessionManager::OnCreateServerSession(
  const SIPMessage & request
)
{
  PWaitAndSignal lock( m_SessionsMutex );
  CallId callId;
  request.GetCallId( callId );
  PString sessionId = callId.GetHeaderBody();
  ProxySession * proxySession = new ProxySession( *this, request, sessionId );
  return proxySession->GCCreateRef( "ProxySessionManager::OnCreateServerSession" );
}

ProxySession::RoutePolicy  ProxySessionManager::OnCheckRoutePolicy(
  ProxySession & session,
  SIPMessage & request
)
{
  return ProxySession::RouteBySession;
}

ProxySession::AuthStatus ProxySessionManager::OnCheckAuthorization(
  ProxySession & session,
  const SIPMessage & request
)
{
  return ProxySession::Authorized;
}

void ProxySessionManager::OnProvisionalResponse(
  ProxySession & session,
  const SIPMessage & request
)
{
}

void ProxySessionManager::OnFinalResponse(
  ProxySession & session,
  const SIPMessage & request
)
{
}

void ProxySessionManager::OnUnknownTransaction(
  SIPUnknownTransaction & event
)
{
  if( event.IsFromTransport() )
    OnOrphanedMessage( event.GetMessage() );
}

void ProxySessionManager::OnOrphanedMessage(
  const SIPMessage & message
)
{
  /// orphaned request?  This should never happen
  if( message.IsRequest()  && !message.IsAck() )
    return;

  /// we only relay INVITE transactions
  PString method = message.GetCSeqMethod().ToUpper().Trim();
  if(  method != "INVITE" && method != "ACK" )
    return;

  SIPMessage msg = message;
  PString t_id = msg.GetTransactionId().AsString();
  if( !msg.IsRequest() )
  {
    /// check the top via if its the same as our address
    Via via;
    if( msg.GetViaAt( 0, via ) )
    {
      if( GetTransportManager()->IsLocalAddressAndPort( via.GetURI() ) )
      {
        /// pop it if its not the only one left
        if( msg.GetViaSize() > 1 )
          msg.PopTopVia();
        else
          return;
      }
     
    }
  }else if( msg.IsAck() )
  {
    LOG( LogDebug(), "Received ACK without a Transaction.  Attempting relay" ); 
    PString callId = msg.GetCallId();
    SIPMessage  originalInvite, routedInvite;
    if( FindTuple( callId, originalInvite, routedInvite ) )
    {
    
      ////check if we have a tuple;
      if( !routedInvite.IsInvite() )
        return;
        
      StartLine startLine;
      routedInvite.GetStartLine( startLine );
      startLine.GetRequestLine()->SetMethod( "ACK" );
      msg.SetStartLine( startLine );
    }

    if( msg.HasRoute() && msg.GetRouteSize() > 0 )
    {
      Route route = msg.GetRouteAt(0);
      RouteURI routeURI;
      route.GetURI( routeURI, 0 );

      if( GetTransportManager()->IsLocalAddressAndPort( routeURI.GetURI() ) )
      {
        LOG_IF_DEBUG( LogDetail(), "*** ROUTE-URI REMOVAL *** Route URI Resolves to this Proxy." );
        LOG_IF_DEBUG( LogInfo(), "*** ROUTE-URI REMOVAL *** " << msg.PopTopRouteURI() );
      }
    }
  }

  /// check if the message will loop
  PIPSocket::Address addr;
  WORD  port;

  if( !SIPTransport::Resolve( msg, addr, port ) )
  {
    LOG_IF_DEBUG( LogInfo(), "*** DID NOT RESOLVE *** " );
    LOG_IF_DEBUG( LogDebugVeryHigh(),  msg );
    return;
  }

  if( GetTransportManager()->IsLocalAddressAndPort( addr, port ) /*&& 
      GetUserAgent().GetDefaultProfile().GetTransportProfile().GetUDPListenerPort()  == port*/ )
  {
    LOG_IF_DEBUG( LogInfo(), "*** LOOP DETECTED *** " );
    LOG_IF_DEBUG( LogDebugVeryHigh(),  msg );
    return;
  }

   /// relay the request directly
  LOG_IF_DEBUG( LogDebug(), "*** STATELESS RELAY *** " << endl << msg.GetStartLine() );
  LOG_IF_DEBUG( LogDebugVeryHigh(),  msg );
  
  GetUserAgent().SendRequest( msg );
}


void ProxySessionManager::OnProcessRequestAsLocal(
  ProxySession & session,
  const SIPMessage & request
)
{
  /// Just OK any request.  its up to implementors to incorporate 
  /// application specific herer

  session.SendAcceptByOk();
}



