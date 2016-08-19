/*
 *
 * RFC3265ClientManager.h
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
 * $Log: RFC3265ClientManager.cxx,v $
 * Revision 1.5  2007/01/14 13:12:55  joegenbaclor
 * Fixed compile warnings
 *
 * Revision 1.4  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/11/10 09:53:59  joegenbaclor
 * more implementations
 *
 * Revision 1.2  2006/11/09 10:03:44  joegenbaclor
 * Added code to start expire timer
 *
 * Revision 1.1  2006/11/07 07:48:04  joegenbaclor
 * Initial upload of RFC3265Client Files
 *
 */
  
#include "RFC3265ClientManager.h"
#include "RFC3265Client.h"

#define new PNEW

using namespace RFC3265;

RFC3265ClientManager::RFC3265ClientManager(
  SIPUserAgent & ua,
  const PString & eventPackageName,
  PINDEX sessionThreadCount,
  PINDEX stackSize
) : SIPSessionManager( ua, sessionThreadCount, stackSize, "RFC3265ClientManager" )
{
  m_EventPackageName = eventPackageName;
}

RFC3265ClientManager::~RFC3265ClientManager()
{
}

SIPSession * RFC3265ClientManager::OnCreateMultiDirectionalSession(
  const ProfileUA & profile,
  const PString & sessionId
)
{
  PAssertAlways( PLogicError );
  return NULL;
}

SIPSession::GCRef RFC3265ClientManager::OnCreateServerSession(
  const SIPMessage & request
)
{
  PAssertAlways( PLogicError );
  return SIPSession::GCRef( NULL, "" );
}

SIPSession * RFC3265ClientManager::OnCreateClientSession(
  const ProfileUA & profile,
  const PString & sessionId
)
{
  if( dynamic_cast<const RFC3265ClientManager::SubscriptionInfo*>(&profile) == NULL )
  {
    PAssertAlways( PInvalidCast );
    return NULL;
  }

  return new RFC3265Client( *this, sessionId, dynamic_cast<const RFC3265ClientManager::SubscriptionInfo&>(profile)  ) ;
}



BOOL RFC3265ClientManager::CreateSubscription(
  const SubscriptionInfo & _info,
  SIPMessage & subscribe,
  BOOL willCreateSession
)
{
  SubscriptionInfo info (_info);
  /// check if a subcription dialog is already present;
  if( info.GetCallId().IsEmpty() )
    info.SetCallId( ParserTools::GenGUID() );

  PString callId = info.GetCallId();
  {
    SIPSession::GCRef session = FindGCRefByCallId( callId );
    if( session != NULL )
    {
      if( !session->CreateRequestWithinDialog(
        SIPMessage::Method_SUBSCRIBE,
        subscribe
      ) ) return FALSE;

      return TRUE;
    }
  }

  
  if( willCreateSession && CreateClientSession( info, callId ) == NULL )
    return FALSE;

  /// we dont have a session yet so create a new Request
  SIPURI uri = info.GetResourceURI();
  PIPSocket::Address targetAddress = uri.GetAddress();

  /// create the request line
  RequestLine requestLine;
  requestLine.SetMethod( "SUBSCRIBE" );
  BOOL willUseProxy = !info.GetProxyProfile().GetServerAddress().IsEmpty();
  requestLine.SetRequestURI( uri );
  subscribe.SetStartLine( requestLine );


  if( willUseProxy )
  {
    /// put the proxy as the static route
    SIPURI sendAddress( info.GetProxyProfile().GetServerAddress() );
    subscribe.SetSendAddress( sendAddress );
    targetAddress = sendAddress.GetAddress();
  }

  PString transport;
  if( !uri.GetParameter( "transport", transport ) )
    transport = "udp";

  Via via;
  ConstructVia( targetAddress, via, SIPTransport::UDP );
  
  via.SetBranch( ParserTools::GenBranchParameter() );
  via.AddParameter( "rport", "" );
  subscribe.AppendVia( via );

  /// Set From header
  From from;
  PStringStream userURI; 

  if( !info.GetDisplayName().IsEmpty() )
    from.SetDisplayName( info.GetDisplayName() );

  PString userName = info.GetUser();
  
  if( !userName.IsEmpty() )
    userURI << userName << "@"; 

  PString localHost;

  if( willUseProxy )
  {
    localHost = info.GetProxyProfile().GetDomain();
    if( localHost.IsEmpty() )
    {
      ///get the domain from the proxy address
      SIPURI domainURI( info.GetRegistrationProfile().GetServerAddress() );
      localHost = domainURI.GetHost();
    }
  }

  if( localHost.IsEmpty() )
  {
    localHost = via.GetAddress();
  }

  userURI << localHost;

  SIPURI fromURI( userURI );
  from.SetURI( fromURI );
  from.AddParameter( "tag", ParserTools::GenTagParameter() );

  subscribe.SetFrom( from );

  /// Set To header
  To to;
  to.SetURI( uri );
  subscribe.SetTo( to );

  /// Set the call Id
  CallId cid;
  cid.SetHeaderBody( callId );
  subscribe.SetCallId( cid );

  /// Set the CSeq
  CSeq cSeq;
  cSeq.SetMethod( "SUBSCRIBE" );
  cSeq.SetSequence( 4711 );
  subscribe.SetCSeq( cSeq );

  /// Set the contact
  SIPURI contactURI = fromURI;
  contactURI.SetHost( via.GetURI().GetHost() );
  contactURI.SetPort( via.GetURI().GetPort() );

  ContactURI contact( contactURI, info.GetDisplayName() );
  subscribe.AppendContact( contact );

  ContentType ct;
  ct.SetHeaderBody( info.GetContentType() );
  subscribe.SetContentType( ct );

  Expires expires( info.GetExpireInterval().GetSeconds() );
  subscribe.SetExpires( expires );

  Event event( GetEventPackageName() );
  subscribe.SetEvent( event );

  return FALSE;
}

BOOL RFC3265ClientManager::SendSubscription(
  const SIPMessage & subscribe
)
{
  SIPSession::GCRef session = FindGCRefByCallId( subscribe.GetCallId() );
  if( session == NULL )
    return FALSE;

  RFC3265Client * client = dynamic_cast<RFC3265Client*>(session.GetObject());
  if( client == NULL )
  {
    PAssertAlways( PInvalidCast );
    return FALSE;
  }

  return client->SendSubscription( subscribe );
}

BOOL RFC3265ClientManager::DestroySubscription(
  const SubscriptionInfo & info 
)
{
  SIPMessage subscribe;
  if( !CreateSubscription( info, subscribe, FALSE ) )
    return FALSE;

  Expires expires(0);
  subscribe.SetExpires( expires );

  return SendSubscription( subscribe );
}

void RFC3265ClientManager::OnNotification(
  RFC3265Client * session,
  const SIPMessage & notify
)
{
}

/// return FALSE here will automatically destroy the subscription
BOOL RFC3265ClientManager::OnSubscribeRedirection(
  RFC3265Client * session,
  const SIPMessage & redirect
)
{
  return TRUE;
}
/// return FALSE here will automatically destroy the subscription
/// this is the place where you can set the account and password in the session profile
/// if it hasnt been set yet during creatin of the session
BOOL RFC3265ClientManager::OnAuthenticateSubscription(
  RFC3265Client * session,
  const SIPMessage & subsribe,
  const SIPMessage & authenticator
)
{
  return TRUE;
}

void RFC3265ClientManager::OnOrphanedMessage(
  const SIPMessage & message
)
{
  if( message.IsNotify() )
    OnUnsolicitedNotification( message );
  else
    SIPSessionManager::OnOrphanedMessage( message );

}

void RFC3265ClientManager::OnUnsolicitedNotification(
  const SIPMessage & notify
)
{
  SIPMessage response;
  notify.CreateResponse( response, SIPMessage::Code200_Ok );
  SendRequest( response );
}

