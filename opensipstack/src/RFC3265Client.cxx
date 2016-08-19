/*
 *
 * RFC3265Client.cxx
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
 * $Log: RFC3265Client.cxx,v $
 * Revision 1.6  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/11/30 00:51:14  joegenbaclor
 * 1.  Corrected bug in authorization construction where the URI can have a double port concatenation.
 * 2.  Modified AsString() method of SIPURI to allow not returning the URI parameters.
 * 3.  Added GetBasicURI method
 *
 * Revision 1.4  2006/11/24 03:21:16  joegenbaclor
 * Initial upload of RFC3680 Client
 *
 * Revision 1.3  2006/11/10 09:53:56  joegenbaclor
 * more implementations
 *
 * Revision 1.2  2006/11/09 10:03:44  joegenbaclor
 * Added code to start expire timer
 *
 * Revision 1.1  2006/11/07 07:48:04  joegenbaclor
 * Initial upload of RFC3265Client Files
 *
 */

#include "RFC3265Client.h"

using namespace RFC3265;

#define new PNEW

RFC3265Client::RFC3265Client(
  RFC3265ClientManager & manager,
  const PString & sessionId,
  const RFC3265ClientManager::SubscriptionInfo & profile
) : SIPSession( manager, sessionId, profile )
{
  m_SubscriptionProfile = profile;
  m_ExpireInterval = profile.GetExpireInterval();
}

RFC3265Client::~RFC3265Client()
{
}


void RFC3265Client::OnSessionEvent(
  int _event,
  const SIPMessage & eventMsg
)
{
  switch( _event )
  {
    case OnSendSubscriptionEvent:
      OnSendSubscription( eventMsg );
      break;
    case OnSendSubscriptionRefreshEvent:
      OnSendSubscriptionRefresh( eventMsg );
      break;
    case OnEventNotificationEvent:
      OnEventNotification( eventMsg );
      break;
    case OnAuthenticateSubscriptionEvent:
      OnAuthenticateSubscription( m_CurrentUACRequest, eventMsg );
      break;
    case OnSubscriptionAcceptedEvent:
      OnSubscriptionAccepted( m_CurrentUACRequest, eventMsg  );
      break;
    case OnSubscriptionRejectedEvent:
      OnSubscriptionRejected( m_CurrentUACRequest, eventMsg  );
      break;
    case OnRemoteTerminateSubscriptionEvent:
      break;
    case OnLocalTerminateSubscriptionEvent:
      break;
  }
}

void RFC3265Client::OnIncomingSIPMessage(
  SIPMessageArrival & messageEvent
)
{
  GCREF( "RFC3265Client::OnIncomingSIPMessage" );
  SIPSession::OnIncomingSIPMessage( messageEvent );
  const SIPMessage & msg = messageEvent.GetMessage();

  if( msg.GetCSeqMethod() *= "NOTIFY" )
  {
    EnqueueSessionEvent( new SIPSessionEvent( *this, RFC3265Client::OnEventNotificationEvent, msg ) );
  }else if( msg.GetCSeqMethod() *= "SUBSCRIBE" )
  {
    if( msg.IsRequest() )
    {
      /// UAS is sending a Subscribe???
      SIPMessage response;
      msg.CreateResponse( response, SIPMessage::Code400_BadRequest );
      SendRequest( response );
    }else
    {
      // we have a response, lets check what's going on
      if( msg.Is2xx() )
      {
        if( !this->IsDialogEstablished() )
          InitializeDialogAsUAC( m_CurrentUACRequest, msg );

        EnqueueSessionEvent( new SIPSessionEvent( *this, RFC3265Client::OnSubscriptionAcceptedEvent, msg ) );
      }else
      {
          if( msg.GetStatusCode() == SIPMessage::Code407_ProxyAuthenticationRequired || 
                msg.GetStatusCode() == SIPMessage::Code401_Unauthorized )
          {
            /// we are being challenged
            EnqueueSessionEvent( new SIPSessionEvent( *this, RFC3265Client::OnAuthenticateSubscriptionEvent, msg ) );
          }else if( msg.Is3xx() )
          {
            EnqueueSessionEvent( new SIPSessionEvent( *this, RFC3265Client::OnSubscribeRedirectionEvent, msg ) );
          }else /// oops!
          {
            EnqueueSessionEvent( new SIPSessionEvent( *this, RFC3265Client::OnSubscriptionRejectedEvent, msg ) );
          }
      }
    }
  }else
  {
    OnHandleNoneNotifyEvent( messageEvent );
  }
}

void RFC3265Client::OnHandleNoneNotifyEvent(
  SIPMessageArrival & noneNotifyEvent
)
{
  const SIPMessage & msg = noneNotifyEvent.GetMessage();
  if( msg.IsRequest() )
  {
    SIPMessage response;
    msg.CreateResponse( response, SIPMessage::Code400_BadRequest );
    SendRequest( response );
  }
}


BOOL RFC3265Client::SendSubscription(
  const SIPMessage &  subscribe
)
{
  GCREF( "RFC3265Client::SendSubscription" );
  EnqueueSessionEvent( new SIPSessionEvent( *this, RFC3265Client::OnSendSubscriptionEvent, subscribe ) );
  return TRUE;
}

BOOL RFC3265Client::OnSendSubscription(
  const SIPMessage &  subscribe
)
{
  GCREF( "RFC3265Client::OnSendSubscription" );

  if( !IsDialogEstablished() )
  {
    ///start the subscription request timer
    if( m_ExpireInterval.GetSeconds() < 90 )
      m_ExpireInterval = PTimeInterval( 0, 90 );

    m_ExpireTimer.SetInterval( m_ExpireInterval.GetMilliSeconds() / 2 );
    m_ExpireTimer.SetNotifier( PCREATE_NOTIFIER( OnSendSubscriptionRefresh ) );
    m_ExpireTimer.Resume();
  }

  return SendRequest( subscribe );
}

void RFC3265Client::OnSendSubscriptionRefresh(
  PTimer &,
  INT
)
{
  GCREF( "RFC3265Client::OnSendSubscriptionRefresh" );

  SIPMessage subscribe = m_CurrentUACRequest;
  CSeq cseq;
  subscribe.GetCSeq( cseq );
  subscribe.SetCSeq( ++cseq );
  EnqueueSessionEvent( new SIPSessionEvent( *this, RFC3265Client::OnSendSubscriptionRefreshEvent, subscribe ) );
}

BOOL RFC3265Client::OnSendSubscriptionRefresh(
  const SIPMessage & subscribe
)
{
  GCREF( "RFC3265Client::OnSendSubscriptionRefresh" );
  return SendRequest( subscribe );
}

void RFC3265Client::OnSubscribeRedirection(
  const SIPMessage & redirect
)
{
  if( redirect.GetContactSize() == 0 )
    return;

  RFC3265ClientManager & manager = dynamic_cast<RFC3265ClientManager &>(GetSessionManager());
  if( manager.OnSubscribeRedirection( this, redirect ) )
  {
    ContactURI contactURI;
    if( redirect.GetContactAt( 0 ).GetURI( contactURI, 0 ) )
    {
      RequestLine requestLine;
      m_CurrentUACRequest.GetRequestLine( requestLine );
      requestLine.SetRequestURI( contactURI.GetURI() );
      SendRequest( m_CurrentUACRequest );
    }
  }else
  {
    //TO DO: should destroy the session here
  }
}

void RFC3265Client::OnEventNotification(
  const SIPMessage & notify
)
{
  GCREF( "RFC3265Client::OnSendSubscription" );
  SIPMessage response;
  notify.CreateResponse( response, SIPMessage::Code200_Ok );
  SendRequest( response );

  /// TO DO:
  /// now lets figure out what the uas meant with this notify
}

void RFC3265Client::OnAuthenticateSubscription(
  const SIPMessage & subscribe,
  const SIPMessage & authenticator
)
{
  GCREF( "RFC3265Client::OnSendSubscription" );
  RFC3265ClientManager & manager = dynamic_cast<RFC3265ClientManager &>(GetSessionManager());
  if( manager.OnAuthenticateSubscription( this, subscribe, authenticator ) )
  {
    PString proxyAccount = m_SubscriptionProfile.GetProxyProfile().GetAccount();
    PString proxyPassword = m_SubscriptionProfile.GetProxyProfile().GetPassword();
    PString registrarAccount = m_SubscriptionProfile.GetRegistrationProfile().GetAccount();
    PString registrarPassword = m_SubscriptionProfile.GetRegistrationProfile().GetPassword();

    if( authenticator.GetStatusCode() == SIPMessage::Code407_ProxyAuthenticationRequired )
    {
      if( !authenticator.HasProxyAuthenticate() )
      {
        /// TO DO Destroy the session
        return;
      }

      ProxyAuthenticate proxyAuth;
      authenticator.GetProxyAuthenticate( proxyAuth );
      
      if( proxyAccount.IsEmpty() )
      {
        LOG( LogWarning(), "*** Proxy Authentication User is EMPTY!!! ***" );
        LOG( LogWarning(), "*** Unable to authenticate Subscription!!! ***" );
        /// TO DO Destroy the session
        return;
      }

      if( proxyPassword.IsEmpty() )
      {
        LOG( LogWarning(), "Proxy authentication password is Empty" );
      }

      /// clear the internal headers from the previous transaction
      m_CurrentUACRequest.ClearInternalHeaders();

      /// increment cseq
      CSeq cseq;
      m_CurrentUACRequest.GetCSeq( cseq );
      m_CurrentUACRequest.SetCSeq( ++cseq );

      StartLine startLine;
      m_CurrentUACRequest.GetStartLine( startLine );
      startLine.GetRequestLine()->GetRequestURI().AsString();
      PString uriText = startLine.GetRequestLine()->GetRequestURI().AsString( FALSE );

      Via via;
      m_CurrentUACRequest.GetViaAt( 0, via );
      via.AddParameter( "branch", ParserTools::GenBranchParameter(), TRUE );
      m_CurrentUACRequest.SetViaAt( 0, via );

      PString realm;
      proxyAuth.GetParameter( "realm", realm );
      ParserTools::UnQuote( realm );
      PString nonce;
      proxyAuth.GetParameter( "nonce", nonce );
      ParserTools::UnQuote( nonce );

      MD5::A1Hash a1( proxyAccount, realm, proxyPassword );
      MD5::A2Hash a2( startLine.GetRequestLine()->GetMethod(), uriText );
      PString hResponse = MD5::MD5Authorization::Construct( a1.AsString(), nonce, a2.AsString() );

      ProxyAuthorization authorization;
      authorization.SetLeadString( "Digest" );
      authorization.AddParameter( "username", ParserTools::Quote( proxyAccount ) );
      authorization.AddParameter( "realm", ParserTools::Quote( realm ) );
      authorization.AddParameter( "nonce", ParserTools::Quote( nonce ) );
      authorization.AddParameter( "uri", ParserTools::Quote( uriText ) );
      authorization.AddParameter( "response", ParserTools::Quote( hResponse ) );
      authorization.AddParameter( "algorithm", "MD5" );

      m_CurrentUACRequest.SetProxyAuthorization( authorization );



    }else if( authenticator.GetStatusCode() == SIPMessage::Code401_Unauthorized )
    {
      WWWAuthenticate wwwAuth;
      if( !authenticator.HasWWWAuthenticate() )
      {
        /// TO DO Destroy the session
        return;
      }

      authenticator.GetWWWAuthenticate( wwwAuth );
    } 
  }else
  {
    /// TO DO:  We must enqueue the session for deletion here
  }
}
  
void RFC3265Client::OnSubscriptionAccepted(
  const SIPMessage & subscribe,
  const SIPMessage & response
)
{
  GCREF( "RFC3265Client::OnSendSubscription" );
}

void RFC3265Client::OnSubscriptionRejected(
  const SIPMessage & subscribe,
  const SIPMessage & response
)
{
  GCREF( "RFC3265Client::OnSendSubscription" );
}

void RFC3265Client::OnRemoteTerminateSubscription(
  const SIPMessage & notify
)
{
  GCREF( "RFC3265Client::OnSendSubscription" );
}

void RFC3265Client::OnLocalTerminateSubscription(
  const SIPMessage & subscribe
)
{
  GCREF( "RFC3265Client::OnSendSubscription" );
}

