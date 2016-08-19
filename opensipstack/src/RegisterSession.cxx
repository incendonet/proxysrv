/*
 *
 * RegisterSession.cxx
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
 * $Log: RegisterSession.cxx,v $
 * Revision 1.44  2007/01/17 00:12:19  joegenbaclor
 * 1.  Fixed bug in call session where CANCEL may not be sent if a call ahs been authenticated due to a misplaced SetState( StateDisconnected ) for 4xx events
 * 2.  Fixed register session to ignore authorization headers if allow all reg is flagged
 * 3.  Temporary hacks for syslog
 * 4.  More launcher code -  almost there!
 *
 * Revision 1.43  2007/01/10 04:58:49  joegenbaclor
 * Added B2BUpperReg mode.
 *
 * Revision 1.42  2007/01/08 07:12:58  joegenbaclor
 * Added ability to run SBC in pure proxy or pure b2b modes
 *
 * Revision 1.41  2006/12/31 08:13:34  joegenbaclor
 * Set client NAT keep-alive to a lower value.
 * My NAT in particular doesn't keep the binding
 * with 30 seconds interval.
 *
 * Revision 1.40  2006/12/31 05:32:25  joegenbaclor
 * Added NAT keep alives to client sessions.
 *
 * Revision 1.39  2006/12/23 11:40:39  joegenbaclor
 * Set session expiration to 150 % of the original value
 *
 * Revision 1.38  2006/12/07 09:12:21  joegenbaclor
 * 1.  Changed registration expire value from milliseconds to seconds
 *
 * Revision 1.37  2006/12/06 06:40:56  joegenbaclor
 * STUN related fixes
 *
 * Revision 1.36  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.35  2006/11/30 00:51:15  joegenbaclor
 * 1.  Corrected bug in authorization construction where the URI can have a double port concatenation.
 * 2.  Modified AsString() method of SIPURI to allow not returning the URI parameters.
 * 3.  Added GetBasicURI method
 *
 * Revision 1.34  2006/11/17 11:23:24  joegenbaclor
 * 1.  Added Max-Forwards header to all outbound requests if not present
 * 2.  SetUACoreName() in SIPStack::OnReceivedMessage()
 *
 * Revision 1.33  2006/10/30 02:42:27  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.32  2006/10/23 20:22:28  joegenbaclor
 * Modified Registration Methods to include a reference to the Register request
 *
 * Revision 1.31  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.30  2006/08/26 14:31:44  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.29  2006/08/24 10:19:14  joegenbaclor
 * 1.  Couple of bug fixes in SIP Intitializarion and Disconnect cause detection
 * 2.  Added ProxyAuthentication call backs to Register Session
 *
 * Revision 1.28  2006/08/23 03:33:54  joegenbaclor
 * Some Opal related bug fixes discovered duriong intial development of ATLSIP
 *
 * Revision 1.27  2006/08/15 15:48:14  joegenbaclor
 * Removed unneeded LOG in register session
 *
 * Revision 1.26  2006/08/14 01:29:08  joegenbaclor
 * Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
 *
 * Revision 1.25  2006/08/13 06:49:08  joegenbaclor
 * introduced mutex for m_CurrentRequest
 *
 * Revision 1.24  2006/07/26 00:44:52  joegenbaclor
 * Added Voice File classes for upcoming IVR support
 *
 * Revision 1.23  2006/07/24 07:33:56  joegenbaclor
 * Minor bug fixes allover
 *
 * Revision 1.22  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.21  2006/06/08 09:02:40  joegenbaclor
 * Added SendUnregister() method to OpalOSSEndPoint
 *
 * Revision 1.20  2006/06/08 04:54:20  joegenbaclor
 * !.  Added OnRegistrationTimeout Call back to Registrar classes
 * 2. Moved ECCIAPI to Tools namespace
 *
 * Revision 1.19  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.18  2006/04/29 03:22:54  joegenbaclor
 * 1.  More Refer support code added to call session
 * 2.  Added StoreRegistrationInfo callback to Registrar classes
 *
 * Revision 1.17  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.16  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.15  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.14  2006/03/27 15:25:10  joegenbaclor
 * Last minute improvements before creating a new tarball for download including
 * 1.  Major bug fix in simple-proxy where events may leak for REGISTER transactions.
 * 2.  Used incremental logging in HTTP admin.
 * 3.  Removed unused parameters in HTTPAdmin.
 * 4.  Local domain calls are now record-routed.
 * 5.  Fixed bug where ACK routeset is not popped during relay.
 *
 * Revision 1.13  2006/03/23 05:37:58  joegenbaclor
 * Got rid of some compile warning in Visual Studio 2003
 *
 * Revision 1.12  2006/03/21 14:11:50  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.11  2006/03/21 03:29:56  joegenbaclor
 * Added Logger functionality to log to PTrace
 *
 * Revision 1.10  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
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

#include "RegisterSession.h"
#include "RegisterSessionManager.h"

#define new PNEW

using namespace UACORE;

RegisterSession::RegisterSession(
  RegisterSessionManager & sessionManager,
  const ProfileUA & profile,
  const PString & sessionId
) : SIPSession( sessionManager, sessionId, profile ),
    m_IsRegistered( FALSE ),
    m_IsBrandNewSession( TRUE )
{

  if( m_SessionProfile.RegistrationProfile().GetExpireInterval() < 10 )
  {
    m_SessionProfile.GetRegistrationProfile().SetExpireInterval( 3600 );
  }

  ConstructRegister( m_Request );

  CallId cid;
  m_Request.GetCallId( cid );
  m_CallId = cid.GetHeaderBody();

  /// set the URI
  To to;
  m_Request.GetTo( to );
  m_URI = to.GetURI();

  m_AuthorizationAttempts = 0;
  m_LogTag = "REGISTER";

  m_RFC3680Subscription = NULL;

  m_UseRegisterInKeepAlives = FALSE;
  
  LOG_IF_DEBUG( LogInfo(), "*** CREATED *** CLIENT REGISTER Session " <<  m_SessionId );
}

RegisterSession::RegisterSession(
  RegisterSessionManager & sessionManager,
  const SIPMessage & request,
  const PString & sessionId
) : SIPSession( sessionManager, request, sessionId ),
    m_IsRegistered( FALSE ),
    m_IsBrandNewSession( TRUE )
{
  m_AuthorizationAttempts = 0;
  if( request.IsSubscribe() )
  {
    m_LogTag = "RFC3680";
    m_IsRFC3680Session = TRUE;
  }else
  {
    m_LogTag = "REGISTER";
    m_IsRFC3680Session = FALSE;
  }

  m_RFC3680Subscription = NULL;

  m_UseRegisterInKeepAlives = FALSE;

  LOG_IF_DEBUG( LogInfo(), "*** CREATED *** SERVER REGISTER Session " <<  m_SessionId );
}

RegisterSession::~RegisterSession()
{
  LOG_IF_DEBUG( LogInfo(), "*** DESTROYED *** REGISTER Session " <<  m_SessionId  );
}

void RegisterSession::OnIncomingSIPMessage(
  SIPMessageArrival & messageEvent
)
{
  GCREF( "OnIncomingSIPMessage" );

  SIPSession::OnIncomingSIPMessage( messageEvent );

  SIPMessage msg = messageEvent.GetMessage();

  LOG_IF_DEBUG( LogDebugVeryHigh(), msg );

  if( !msg.IsRequest() && GetType() == Client )
  {
    OnHandleResponse( msg );
  }else if( msg.IsSubscribe() || msg.IsNotify()  )
  {
    Event eventHeader;
    SIPMessage response;
    if( !msg.GetEvent( eventHeader ) )
    {
      msg.CreateResponse( response, SIPMessage::Code400_BadRequest );
      SendRequest( response );
      return;
    }
    
    if( eventHeader.GetHeaderBody().Trim().ToLower() != "reg" )
    {
      msg.CreateResponse( response, SIPMessage::code489_BadEvent );
      SendRequest( response );
      return;
    }

    if( msg.IsUnsubscribe() )
    {
      EnqueueSessionEvent( new SIPSessionEvent( *this, RFC3680Unsubscribe, msg ) );
    }else if( msg.IsSubscribe() )
    {
      if( m_IsBrandNewSession )
        EnqueueSessionEvent( new SIPSessionEvent( *this, RFC3680NewSubscription, msg ) );
      else
        EnqueueSessionEvent( new SIPSessionEvent( *this, RFC3680SubscriptionRequest, msg ) );
    }else if( msg.IsNotify() )
    {
      EnqueueSessionEvent( new SIPSessionEvent( *this, RFC3680Notification, msg ) );
    }

  }else if( msg.IsUnregister() )
  {
    EnqueueSessionEvent( new SIPSessionEvent( *this, Unregister, msg ) );
  }else if( !m_IsBrandNewSession ) /// this is an existing session
  {
    EnqueueSessionEvent( new SIPSessionEvent( *this, RegistrationRequest, msg ) );
  }else  /// this is a brand new session
  {
    EnqueueSessionEvent( new SIPSessionEvent( *this, NewRegistration, msg ) );
  }
}

void RegisterSession::OnSessionEvent(
  int event,
  const SIPMessage & eventMsg
)
{
  GCREF( "OnSessionEvent" );


  switch( event )
  {
    case NewRegistration:
      OnNewRegistration( eventMsg );
      break;
    case Unregister:
      OnUnregistration( eventMsg );
      break;
    case RegistrationExpire:
      OnUnregistration( eventMsg );
      Destroy();
      break;
    case RegistrationRequest:
      if( !OnHandleRequest( eventMsg ) )
        Destroy();
      break;
    case RFC3680NewSubscription:
      OnRFC3680NewSubscription( eventMsg );
      break;
    case RFC3680SubscriptionRequest:
      OnRFC3680SubscriptionRequest( eventMsg );
      break;
    case RFC3680SubscriptionExpire:
      OnRFC3680SubscriptionExpire();
      break;
    case RFC3680Unsubscribe:
      OnRFC3680Unsubscribe( eventMsg );
      break;
    case RFC3680Notification:
      OnRFC3680Notification( eventMsg );
      break;
  }
}

void RegisterSession::SendRegister()
{
  GCREF( "SendRegister" );

  StopAutoDestructTimer();

  SIPMessage request = m_Request;
  request.ClearInternalHeaders();

  if( !m_SessionProfile.GetProxyProfile().GetServerAddress().IsEmpty() )
  {
    /// put the proxy as the static route
    SIPURI sendAddress( m_SessionProfile.GetProxyProfile().GetServerAddress() );
    request.SetSendAddress( sendAddress );
  }

  LOG_CONTEXT( LogDetail(), m_CallId, "REGISTER: Starting Registration Process" );
  LOG_IF_DEBUG( LogDetail(), request );

  SendRequest( request );
}

void RegisterSession::SendUnregister()
{
  GCREF( "SendUnregister" );
  
  StartAutoDestructTimer( 10000 );

  if( !IsRegistered() )
  {
    LOG_IF_DEBUG( LogDetail(), "Sending UNREGISTER Aborted because Session is not registered" );
    return;
  }

  SIPMessage unregister = GetCurrentUACRequest();
  /// increment the cseq;
  CSeq cseq;
  unregister.GetCSeq( cseq );
  unregister.SetCSeq( ++cseq );

  unregister.RemoveAllContact();
  Contact contact( "Contact: *" );
  unregister.AppendContact( contact );

  Expires expires;
  expires.SetHeaderBody( "0" );
  unregister.SetExpires( expires );

  Via via = unregister.PopTopVia();
  via.SetBranch( ParserTools::GenBranchParameter() );
  unregister.AppendVia( via );

  m_ExpireTimer.Stop();

  
  unregister.ClearInternalHeaders();
  
  LOG_CONTEXT( LogDetail(), m_CallId,  "REGISTER: Sending UNREGISTER" );
  LOG_IF_DEBUG( LogDetail(), unregister );

  if( !m_SessionProfile.GetProxyProfile().GetServerAddress().IsEmpty() )
  {
    /// put the proxy as the static route
    SIPURI sendAddress( m_SessionProfile.GetProxyProfile().GetServerAddress() );
    unregister.SetSendAddress( sendAddress );
  }

  SendRequest( unregister );
 
  m_IsRegistered = FALSE;
}

void RegisterSession::SendRegistrationRefresh( PTimer &, INT )
{
  GCREF( "SendRegistrationRefresh" );
  SIPMessage request = m_Request;
  request.ClearInternalHeaders();
  /// increment the cseq;
  CSeq cseq;
  request.GetCSeq( cseq );
  request.SetCSeq( ++cseq );

  if( !m_SessionProfile.GetProxyProfile().GetServerAddress().IsEmpty() )
  {
    /// put the proxy as the static route
    SIPURI sendAddress( m_SessionProfile.GetProxyProfile().GetServerAddress() );
    request.SetSendAddress( sendAddress );
  }

  if( SendRequest( request ) )
  {
    /// renew the timer
    m_ExpireTimer = PTimer( m_SessionProfile.RegistrationProfile().GetExpireInterval() * 800 );
    m_ExpireTimer.SetNotifier( PCREATE_NOTIFIER( SendRegistrationRefresh ) );
    m_ExpireTimer.Resume();
  }
}

void RegisterSession::OnRegistrationExpire( PTimer &, INT )
{
  GCREF( "OnRegistrationExpire" );

  LOG_IF_DEBUG( LogWarning(), "*** EXPIRED *** Registration " << GetCallId() );
  EnqueueSessionEvent( new SIPSessionEvent( *this, RegistrationExpire ) );
}



void RegisterSession::OnHandleResponse(
  const SIPMessage & response
)
{
  GCREF( "OnHandleResponse" );

  if( response.Is1xx() )
    return;

  PAssert( !response.IsRequest(), PLogicError );

  LOG_IF_DEBUG( LogDebugVeryHigh(), response );

  StartLine startLine;
  response.GetStartLine( startLine );

  if( response.Is2xx() )
  {
    m_AuthorizationAttempts = 0;
    OnSuccessful( response );
  }else if( response.GetStatusCode() == SIPMessage::Code401_Unauthorized || 
    response.GetStatusCode() == SIPMessage::Code407_ProxyAuthenticationRequired )
  {
    if( ++m_AuthorizationAttempts > 2 )
      OnFailure( response );
    else
      OnAuthenticate( response );
  }else
  {
    OnFailure( response );
  }

}

BOOL RegisterSession::OnHandleRequest( const SIPMessage & request )
{
  GCREF( "OnHandleRequest" );

  if( !request.HasAuthorization() || ((RegisterSessionManager&)GetSessionManager()).WillAcceptAllRegistrations() )
  {
    OnNewRegistration( request );
  }else
  {
    Authorization auth;
    if( !request.GetAuthorization( auth ) )
    {
      PTRACE( 1, "No Authorization Header in REGISTER" );
      SendAcceptByRejection( request, SIPMessage::Code403_Forbidden );
      OnRegistrationRejected( request );
      return FALSE;
    }

    // WWW-Authenticate: Digest realm=voip.homeunix.org, nonce="48818999e02f8258f3c15c88f7c904e7", opaque="8d99e89591b57e262601a894391e4fe8", algorithm=MD5
    // Authorization: Digest username="2222", realm="voip.homeunix.org", algorithm=MD5, uri="sip:voip.homeunix.org", nonce="48818999e02f8258f3c15c88f7c904e7", opaque="8d99e89591b57e262601a894391e4fe8", response="b06feaf34eea2bd75533bea986e5fab8"
    PString userName;
    PString realm;
    PString uri;
    PString nonce;
    PString opaque;
    PString response;

    if( !auth.GetParameter( "username", userName ) )
    {
      PTRACE( 1, "No User Name specified in REGISTER" );
      SendAcceptByRejection( request, SIPMessage::Code400_BadRequest );
      OnRegistrationRejected( request );
      return FALSE;
    }

    if( !auth.GetParameter( "realm", realm ) )
    {
      PTRACE( 1, "No Realm specified in REGISTER" );
      SendAcceptByRejection( request, SIPMessage::Code400_BadRequest );
      OnRegistrationRejected( request );
      return FALSE;
    }

    if( !auth.GetParameter( "uri", uri ) )
    {
      PTRACE( 1, "No URI specified in REGISTER" );
      SendAcceptByRejection( request, SIPMessage::Code400_BadRequest );
      OnRegistrationRejected( request );
      return FALSE;
    }

    if( !auth.GetParameter( "nonce", nonce ) )
    {
      PTRACE( 1, "No NONCE specified in REGISTER" );
      SendAcceptByRejection( request, SIPMessage::Code400_BadRequest );
      OnRegistrationRejected( request );
      return FALSE;
    }

    if( !auth.GetParameter( "response", response ) )
    {
      PTRACE( 1, "No Authorization response in REGISTER" );
      SendAcceptByRejection( request, SIPMessage::Code400_BadRequest );
      OnRegistrationRejected( request );
      return FALSE;
    }

    PString a1;
    PStringStream userURI;
    userURI << "sip:" << ParserTools::UnQuote( userName ) << "@" << ParserTools::UnQuote( realm );

    if( !((RegisterSessionManager&)GetSessionManager()).OnRequestA1Hash( userURI, a1, *this ) )
    {
      PTRACE( 1, "A1 Hash Error for REGISTER" );
      SendAcceptByRejection( request, SIPMessage::Code403_Forbidden );
      OnRegistrationRejected( request );
      return FALSE;
    }

    MD5::A2Hash a2( "REGISTER", uri );
    m_Authorization = MD5::MD5Authorization( a1, m_Nonce, a2 );
    
    PString md5Local =  m_Authorization.AsString();///MD5::MD5Authorization::Construct( a1.AsString(), ParserTools::UnQuote( nonce ), a2.AsString() );
    
    PString md5Remote = ParserTools::UnQuote( response ); 


    if( md5Local != md5Remote )
    {
      PTRACE( 1,  "Authorization token did not match: " 
        << " Local [" << md5Local << "]" << " Remote [" << md5Remote );

      LOG_IF_DEBUG( LogWarning(), "Authorization token did not match: " 
        << " Local [" << md5Local << "]" << " Remote [" << md5Remote );

      SendAcceptByRejection( request, SIPMessage::Code403_Forbidden );
      OnRegistrationRejected( request );
      return FALSE;
    }

  
    if( !OnStoreRegistrationInfo( userURI ) )
    {
      PTRACE( 1, "Unable to store REGISTRATION" );
      SendAcceptByRejection( request, SIPMessage::Code500_InternalServerError, "Internal Database Error" );
      OnRegistrationRejected( request );
      return FALSE;
    }

    
    SendAcceptRegistration( request );
  }

  return TRUE;
  /// check if this is a challenge response, refresh or an unregistration request;
}

BOOL RegisterSession::SetExpireFromRequest(
  const SIPMessage & registerRequest
)
{
  GCREF( "SetExpireFromRequest" );

  m_ExpireTimer.Stop();

  Expires expires;
  int expireValue = 3600;

  if( !registerRequest.GetExpires( expires ) )
  {

    if( registerRequest.GetContactSize() == 0 )
      return FALSE;

    Contact & contact = registerRequest.GetContactAt( 0 );
    ContactURI uri;
    contact.GetURI( uri );
    PString expireString;
    if( uri.GetParameter( "expires", expireString ) )
    {
      expireValue = expireString.AsInteger();
    }
  }else
  {
    expireValue = (int)expires.AsInt64();
  }

  m_Expires = PTimeInterval( 0, expireValue );
  m_ExpireTimer = PTimer( (int)((expireValue * 1000) * 1.5) );
  m_ExpireTimer.SetNotifier( PCREATE_NOTIFIER( OnRegistrationExpire ) );
  
  LOG_IF_DEBUG( LogDebugHigh(), "Setting expire = " << m_ExpireTimer.GetSeconds() );
  return TRUE;
}

void RegisterSession::OnFailure( 
  const SIPMessage & response 
)
{
  GCREF( "OnFailure" );
  m_IsRegistered = FALSE;
  ((RegisterSessionManager&)m_SessionManager).OnRegistrationRejected( *this, response );
}

void RegisterSession::OnSuccessful(
  const SIPMessage & response
)
{
  GCREF( "OnSuccessful" );

  int interval = m_SessionProfile.RegistrationProfile().GetExpireInterval() * 800;

  Expires expires;
  if( response.GetExpires( expires ) )
    interval = expires.GetHeaderBody().AsInteger() * 800;
  
  RegisterSessionManager& manager = (RegisterSessionManager&)m_SessionManager;
  SIPMessage request = GetCurrentUACRequest();
  if( !request.IsUnregister() )
  {
    //Start the NAT keep-alive heartbeat
    Via via;
    if( request.GetViaAt( request.GetViaSize() - 1, via ) )
    {
	  if( SIPTransport::IsPrivateNetwork( via.GetURI() ) )
      {
        if( !m_UseRegisterInKeepAlives )
        {
          PIPSocket::Address keepAliveAddress;
          WORD keepAlivePort = 5060;
          SIPURI keepAliveTarget;
          SIPTransport::Resolve( request, keepAliveAddress, keepAlivePort );
          keepAliveTarget.SetHost( keepAliveAddress);
          keepAliveTarget.SetPort( PString( keepAlivePort ) );
          StartNATKeepAlive( keepAliveTarget, 10000 );
        }else
        {
          if( interval > 10000 )
            interval = 10000;
        }
      }

      m_ExpireTimer.Stop();
      m_ExpireTimer = PTimer( (interval ) );
      m_ExpireTimer.SetNotifier( PCREATE_NOTIFIER( SendRegistrationRefresh ) );
      m_IsRegistered = TRUE;

      LOG_CONTEXT( LogDetail(), m_CallId,  "REGISTER: Registration Accepted" );
      manager.OnRegistrationAccepted( *this, request );
    }
  }else
  {
    m_ExpireTimer.Stop();
    m_IsRegistered = FALSE;
    LOG_CONTEXT( LogDetail(), m_CallId,  "REGISTER: UNREGISTRATION Accepted" );
    manager.OnUnregistration( *this, request );
  }
}

void RegisterSession::OnAuthenticate(
  const SIPMessage & response
)
{
  GCREF( "OnAuthenticate" );

  SIPMessage auth;

  SIPMessage request = GetCurrentUACRequest();
  BOOL ok = FALSE;

  LOG_CONTEXT( LogDetail(), m_CallId,  "REGISTER: Registration being authenticated" );

  if( response.GetStatusCode() == SIPMessage::Code401_Unauthorized )
  {
    ok = ConstructAuthorization(
      request,
      response,
      auth
    );
  }else if( SIPMessage::Code407_ProxyAuthenticationRequired )
  {
    ok = ConstructAuthentication(
      request,
      response,
      auth
    );
  }

  if( ok )
  {
    if( !m_SessionProfile.GetProxyProfile().GetServerAddress().IsEmpty() )
    {
      /// put the proxy as the static route
      SIPURI sendAddress( m_SessionProfile.GetProxyProfile().GetServerAddress() );
      auth.SetSendAddress( sendAddress );
    }
    SendRequest( auth );
  }else
  {
    OnFailure( response );
  }
}


void RegisterSession::Cleanup()
{
  GCREF( "Cleanup" );

  m_ExpireTimer.Stop();

  if( IsRegistered() )
    SendUnregister();

  SIPSession::Cleanup();
}

BOOL RegisterSession::ConstructRegister(
  SIPMessage & regMsg
)
{
  GCREF( "ConstructRegister" );

   /*
  10.2 Constructing the REGISTER Request

   REGISTER requests add, remove, and query bindings.  A REGISTER
   request can add a new binding between an address-of-record and one or
   more contact addresses.  Registration on behalf of a particular
   address-of-record can be performed by a suitably authorized third
   party.  A client can also remove previous bindings or query to
   determine which bindings are currently in place for an address-of-
   record.

   Except as noted, the construction of the REGISTER request and the
   behavior of clients sending a REGISTER request is identical to the
   general UAC behavior described in Section 8.1 and Section 17.1.

   A REGISTER request does not establish a dialog.  A UAC MAY include a
   Route header field in a REGISTER request based on a pre-existing
   route set as described in Section 8.1.  The Record-Route header field
   has no meaning in REGISTER requests or responses, and MUST be ignored
   if present.  In particular, the UAC MUST NOT create a new route set
   based on the presence or absence of a Record-Route header field in
   any response to a REGISTER request.

   The following header fields, except Contact, MUST be included in a
   REGISTER request.  A Contact header field MAY be included:

      Request-URI: The Request-URI names the domain of the location
           service for which the registration is meant (for example,
           "sip:chicago.com").  The "userinfo" and "@" components of the
           SIP URI MUST NOT be present.

      To: The To header field contains the address of record whose
           registration is to be created, queried, or modified.  The To
           header field and the Request-URI field typically differ, as
           the former contains a user name.  This address-of-record MUST
           be a SIP URI or SIPS URI.

      From: The From header field contains the address-of-record of the
           person responsible for the registration.  The value is the
           same as the To header field unless the request is a third-
           party registration.

      Call-ID: All registrations from a UAC SHOULD use the same Call-ID
           header field value for registrations sent to a particular
           registrar.

           If the same client were to use different Call-ID values, a
           registrar could not detect whether a delayed REGISTER request
           might have arrived out of order.

      CSeq: The CSeq value guarantees proper ordering of REGISTER
           requests.  A UA MUST increment the CSeq value by one for each
           REGISTER request with the same Call-ID.

      Contact: REGISTER requests MAY contain a Contact header field with
           zero or more values containing address bindings.

      expires: The "expires" parameter indicates how long the UA would
           like the binding to be valid.  The value is a number
           indicating seconds.  If this parameter is not provided, the
           value of the Expires header field is used instead.
           Implementations MAY treat values larger than 2**32-1
           (4294967295 seconds or 136 years) as equivalent to 2**32-1.
           Malformed values SHOULD be treated as equivalent to 3600.

   UAs MUST NOT send a new registration (that is, containing new Contact
   header field values, as opposed to a retransmission) until they have
   received a final response from the registrar for the previous one or
   the previous REGISTER request has timed out.

   */

  
  const ProfileRegister & profile = m_SessionProfile.RegistrationProfile();
  const ProfileTransport & transportProfile = m_SessionProfile.TransportProfile();

  /// Create the start line
  RequestLine requestLine;
  SIPURI registrarAddress = profile.GetServerAddress();
  if( profile.GetServerProtocol() != SIPTransport::Unknown )
  {
    if( profile.GetServerProtocol()  == SIPTransport::UDP )
    {
      if( !transportProfile.IsUDPEnabled() )
        return FALSE;

      registrarAddress.AddParameter( "transport", "udp" );
    }else if( profile.GetServerProtocol() == SIPTransport::TCP )
    {
      if( !transportProfile.IsTCPEnabled() )
        return FALSE;

      registrarAddress.AddParameter( "transport", "tcp" );
    }else if( profile.GetServerProtocol() == SIPTransport::TLS )
    {
      if( !transportProfile.IsTLSEnabled() )
        return FALSE;

      registrarAddress.AddParameter( "transport", "tls" );
    }
  }

  requestLine.SetMethod( "REGISTER" );
  requestLine.SetRequestURI( registrarAddress );
  regMsg.SetStartLine( requestLine );

  /// Create the from uri
  

  SIPURI domainURI(profile.GetDomain() );
  PString domain = domainURI.GetHost();
  if( domain.IsEmpty() )
  {
    SIPURI serverURI( profile.GetServerAddress() );
    domain = serverURI.GetHost();
  }

  /// Create the from uri
  From from;
  SIPURI fromURI;
  if( !m_SessionProfile.GetDisplayName().IsEmpty() )
    from.SetDisplayName( m_SessionProfile.GetDisplayName() );

  fromURI.SetUser( m_SessionProfile.GetUser() );
  fromURI.SetHost( domain );
  from.SetURI( fromURI );

  /// always add a new tag
  PString tag = ParserTools::GenTagParameter();
  from.AddParameter( "tag", tag );

  regMsg.SetFrom( from );

  /// Create the to uri
  /// To and from for registrations are identical
  To to;
  to.SetDisplayName( profile.GetDisplayName() );
  to.SetURI( from.GetURI() );
  regMsg.SetTo( to );

  /// Create the CSeq
  CSeq cseq;
  cseq.SetMethod( "REGISTER" );
  cseq.SetSequence( 1 );
  regMsg.SetCSeq( cseq );

  /// set the call-id
  CallId callId;
  PString sCallId = ParserTools::GenGUID();
  callId.SetHeaderBody( sCallId );
  regMsg.SetCallId( callId );

  regMsg.SetMaxForwards( 70 );


  /// Set the expires
  int expire = profile.GetExpireInterval();
  Expires expires( expire );
  regMsg.SetExpires( expires );

  /// Set the content length to zero
  ContentLength contentLength;
  contentLength.SetHeaderBody( "0" );
  regMsg.SetContentLength( contentLength );

    /// Create the via
  Via via;
  PIPSocket::Address targetAddress;
  WORD targetPort = 0;

  if( SIPTransport::Resolve( regMsg, targetAddress, targetPort ) )
  {
    if( m_SessionManager.ConstructVia( targetAddress, via, SIPTransport::UDP ) )
    {
      PString viaBranch = ParserTools::GenBranchParameter();
      via.SetBranch( viaBranch );
      via.AddParameter( "rport", "" );
      regMsg.AppendVia( via );
    }else
      return FALSE;
  }else
  {
    return FALSE;
  }

  for( PINDEX i = 0; i < profile.GetContactSize(); i++ )
  {
    if( transportProfile.IsUDPEnabled() )
    {
      SIPURI tempUri;
      tempUri.SetUser( m_SessionProfile.GetUser());
      tempUri.SetHost( via.GetAddress() );
      tempUri.SetPort( via.GetPort() );
      if( transportProfile.GetUDPListenerAddress() != 0 )
        tempUri.SetPort( transportProfile.GetUDPListenerPort() );
      tempUri.AddParameter( "transport", "udp" );

      Contact contact;
      ContactURI contactURI;
      contactURI.SetDisplayName( m_SessionProfile.GetDisplayName()  );
      contactURI.SetURI( tempUri );
      contact.AddURI( contactURI );
      regMsg.AppendContact( contact );
    }

#if 0
    if( transportProfile.IsTCPEnabled() )
    {
      SIPURI tempUri;
      tempUri.SetUser( m_SessionProfile.GetUser() );
      tempUri.SetHost( transportProfile.GetTCPListenerAddress().AsString() );
      
      if( transportProfile.GetTCPListenerAddress() != 0 )
        tempUri.SetPort( transportProfile.GetTCPListenerPort() );

      tempUri.AddParameter( "transport", "tcp" );

      Contact contact;
      ContactURI contactURI;
      contactURI.SetDisplayName( m_SessionProfile.GetDisplayName()  );
      contactURI.SetURI( tempUri );
      contact.AddURI( contactURI );
      regMsg.AppendContact( contact );
    }

    if( transportProfile.IsTLSEnabled() )
    {
      SIPURI tempUri;
      tempUri.SetScheme( "sips" );
      tempUri.SetUser( m_SessionProfile.GetUser() );
      tempUri.SetHost( transportProfile.GetTLSListenerAddress().AsString() );
      if( transportProfile.GetTLSListenerAddress() != 0 )
        tempUri.SetPort( transportProfile.GetTLSListenerPort() );

      Contact contact;
      ContactURI contactURI;
      contactURI.SetDisplayName( m_SessionProfile.GetDisplayName()  );
      contactURI.SetURI( tempUri );
      contact.AddURI( contactURI );
      regMsg.AppendContact( contact );
    }
#endif
    
  }

  return TRUE;
}

BOOL RegisterSession::ConstructAuthorization(
  const SIPMessage & regMsg,
  const SIPMessage & challenge,
  SIPMessage & auth
)
{
  GCREF( "ConstructAuthorization" );

  ProfileRegister & profile = m_SessionProfile.GetRegistrationProfile();

  PString userName = profile.GetAccount();
  PString password = profile.GetPassword();

  if( !challenge.HasWWWAuthenticate() )
    return FALSE;

  WWWAuthenticate wwwAuth;
  challenge.GetWWWAuthenticate( wwwAuth );

  /// clone the original register message
  auth = regMsg;

  auth.SetFromTag( ParserTools::GenTagParameter() );

  /// increment cseq
  CSeq cseq;
  auth.GetCSeq( cseq );
  auth.SetCSeq( ++cseq );

  StartLine startLine;
  regMsg.GetStartLine( startLine );
  startLine.GetRequestLine()->GetRequestURI().AsString();
  PString uriText = startLine.GetRequestLine()->GetRequestURI().AsString( FALSE );

  PString realm;
  wwwAuth.GetParameter( "realm", realm );
  ParserTools::UnQuote( realm );
  PString nonce;
  wwwAuth.GetParameter( "nonce", nonce );
  ParserTools::UnQuote( nonce );

  MD5::A1Hash a1( userName,realm, password );
  MD5::A2Hash a2( startLine.GetRequestLine()->GetMethod(), uriText );
  PString hResponse = MD5::MD5Authorization::Construct( a1.AsString(), nonce, a2.AsString() );

  Authorization authorization;
  authorization.SetLeadString( "Digest" );
  authorization.AddParameter( "username", ParserTools::Quote( userName ) );
  authorization.AddParameter( "realm", ParserTools::Quote( realm ) );
  authorization.AddParameter( "nonce", ParserTools::Quote( nonce ) );
  authorization.AddParameter( "uri", ParserTools::Quote( uriText ) );
  authorization.AddParameter( "response", ParserTools::Quote( hResponse ) );
  authorization.AddParameter( "algorithm", "MD5" );

  auth.SetAuthorization( authorization );

  Via via;
  auth.GetViaAt( 0, via );
  via.AddParameter( "branch", ParserTools::GenBranchParameter(), TRUE );
  auth.SetViaAt( 0, via );

  return TRUE;
}

BOOL RegisterSession::ConstructAuthentication(
  const SIPMessage & regMsg,
  const SIPMessage & challenge,
  SIPMessage & auth
)
{
  GCREF( "ConstructAuthentication" );

  ProfileRegister & profile = m_SessionProfile.GetRegistrationProfile();

  PString userName = profile.GetAccount();
  PString password = profile.GetPassword();

  if( !challenge.HasProxyAuthenticate() )
    return FALSE;

  ProxyAuthenticate proxyAuth;
  challenge.GetProxyAuthenticate( proxyAuth );

  /// clone the original register message
  auth = regMsg;

  auth.SetFromTag( ParserTools::GenTagParameter() );

  /// increment cseq
  CSeq cseq;
  auth.GetCSeq( cseq );
  auth.SetCSeq( ++cseq );

  StartLine startLine;
  regMsg.GetStartLine( startLine );
  startLine.GetRequestLine()->GetRequestURI().AsString();
  PString uriText = startLine.GetRequestLine()->GetRequestURI().AsString( FALSE );

  PString realm;
  proxyAuth.GetParameter( "realm", realm );
  ParserTools::UnQuote( realm );
  PString nonce;
  proxyAuth.GetParameter( "nonce", nonce );
  ParserTools::UnQuote( nonce );

  MD5::A1Hash a1( userName,realm, password );
  MD5::A2Hash a2( startLine.GetRequestLine()->GetMethod(), uriText );
  PString hResponse = MD5::MD5Authorization::Construct( a1.AsString(), nonce, a2.AsString() );

  ProxyAuthorization authorization;
  authorization.SetLeadString( "Digest" );
  authorization.AddParameter( "username", ParserTools::Quote( userName ) );
  authorization.AddParameter( "realm", ParserTools::Quote( realm ) );
  authorization.AddParameter( "nonce", ParserTools::Quote( nonce ) );
  authorization.AddParameter( "uri", ParserTools::Quote( uriText ) );
  authorization.AddParameter( "response", ParserTools::Quote( hResponse ) );
  authorization.AddParameter( "algorithm", "MD5" );


  auth.SetProxyAuthorization( authorization );

  Via via;
  auth.GetViaAt( 0, via );
  via.AddParameter( "branch", ParserTools::GenBranchParameter(), TRUE );
  auth.SetViaAt( 0, via );

  return TRUE;
}

BOOL RegisterSession::OnNewRegistration( 
  const SIPMessage & request 
)
{
  GCREF( "OnNewRegistration" );

  m_IsBrandNewSession = FALSE;

  switch( OnAcceptRegistration( request ) )
  {
    case AcceptByOk:
      SendAcceptRegistration( request );
      break;
    case AcceptByChallenge:
      SendAcceptByChallenge( request );
      break;
    case AcceptByRejection:
      PTRACE( 1, "OnAcceptRegistration  returned FORBIDDEN for REGISTER" );
      SendAcceptByRejection( request, SIPMessage::Code403_Forbidden );
      OnRegistrationRejected( request );
      return FALSE;
  };
  return TRUE;
}

void RegisterSession::OnUnregistration( const SIPMessage & request )
{
  GCREF( "OnUnregistration" );
  ((RegisterSessionManager&)m_SessionManager).OnUnregistration( *this, request );
  /// we are about to be deleted by the useragent
  LOG_IF_DEBUG( LogInfo(),  "*** UNREGISTER *** " << GetSessionId() );
  SendAcceptByOk( request );
  PString uri = GetURI().AsString();
  Destroy();
}

RegisterSession::AcceptRegistrationResponse RegisterSession::OnAcceptRegistration( const SIPMessage & request )
{
  GCREF( "AcceptRegistrationResponse" );
  return ((RegisterSessionManager&)m_SessionManager).OnAcceptRegistration( *this, request );
}

BOOL RegisterSession::OnStoreRegistrationInfo( const SIPURI & accountInfo )
{
  return ((RegisterSessionManager&)m_SessionManager).OnStoreRegistrationInfo( *this, accountInfo );
}

void RegisterSession::OnRegistrationAccepted( const SIPMessage & request )
{
  GCREF( "OnRegistrationAccepted" );

  /// preserve the URI
  To to;
  request.GetTo( to );
  SetToURI( to );

  /// Preserve the contact list
  ContactList clist = request.GetContactList();
  SetContactList( clist );

  RegisterSessionManager& manager = (RegisterSessionManager&)m_SessionManager;
  manager.GetRegistrationDB().AddRegistration( to.GetURI(), request, m_ExpireTimer.GetSeconds());

  return ((RegisterSessionManager&)m_SessionManager).OnRegistrationAccepted( *this, request );
}

void RegisterSession::OnRegistrationRejected( const SIPMessage & request )
{
  GCREF( "OnRegistrationRejected" );
  LOG_CONTEXT( LogDetail(), m_CallId,  "REGISTER: Registration REJECTED" );
  ((RegisterSessionManager&)m_SessionManager).OnRegistrationRejected( *this, request );
}

void RegisterSession::OnRegistrationTimeout()
{
  GCREF( "OnRegistrationTimeout" );
  ((RegisterSessionManager&)m_SessionManager).OnRegistrationTimeout( *this );
}

void RegisterSession::SendAcceptByChallenge(
  const SIPMessage & request
)
{
  GCREF( "SendAcceptByChallenge" );

  To to;

  request.GetTo( to );
  SIPMessage unauthorized;
  request.CreateResponse( unauthorized, SIPMessage::Code401_Unauthorized );

 // "WWW-Authenticate: Digest realm="voip.homeunix.org", nonce="88ec79faf65b92e4263829498821749a", opaque="a2d30d6ab6eb1a435c86c832c99cec21", algorithm=MD5"
  WWWAuthenticate auth;
  MD5::Nonce opaque;
  auth.SetLeadString( "Digest" );
  auth.AddParameter( "realm", ParserTools::Quote( m_URI.GetHost() ) );
  auth.AddParameter( "nonce", m_Nonce.AsQuotedString() );
  auth.AddParameter( "opaque", opaque.AsQuotedString() );
  auth.AddParameter( "algorithm", "MD5" );
  unauthorized.SetWWWAuthenticate( auth );

  SendRequest( unauthorized );
 
  LOG_IF_DEBUG( LogDetail(), "REGISTER request from " << to.GetURI().GetUser() << " *** Unauthorized ***" );
  //PTRACE( 1, unauthorized.AsString() );
}

void RegisterSession::SendAcceptRegistration( 
  const SIPMessage & request 
)
{

  GCREF( "SendAcceptRegistration" );
  
  if( !SetExpireFromRequest( request ) )
  {
    PTRACE( 1, "Unable to set Expire for REGISTRATION" );
    SendAcceptByRejection( request, SIPMessage::Code403_Forbidden );
    OnRegistrationRejected( request );
    return;
  }

  To to;
  request.GetTo( to );
  SetToURI( to );

  SIPMessage ok;
  request.CreateResponse( ok, SIPMessage::Code200_Ok );
  
  Expires expires( (int)( m_Expires.GetSeconds() ) );
  ok.SetExpires( expires );

  ContactList contactList = request.GetContactList();
  ok.SetContactList( contactList );
  SetContactList( contactList );
  
  /// supported extensions
  Supported supported;
  supported.AddEntry( "reg" );
  supported.AddEntry( "timer" );

  SendRequest( ok );

  /// Check the last via if its from a private IP
  Via via;
  if( request.GetViaAt( request.GetViaSize() - 1, via ) )
  {
    if( via.IsBehindNAT() )
    {
      SIPURI target;
      target.SetHost( via.GetReceiveAddress().AsString() );
      target.SetPort( via.GetRPort() );
      StartNATKeepAlive( target, 30000 );
    }
  }

  OnRegistrationAccepted( request );

  LOG_IF_DEBUG( LogInfo(), "*** REGISTERED *** " << to.GetURI().AsString() << " Session: " << GetSessionId() );
}


void RegisterSession::OnDestroySession()
{
  PString uri = GetURI().AsString();
  RegisterSessionManager& manager = (RegisterSessionManager&)m_SessionManager;
  manager.RemoveSessionFromURIList( uri );
  StopNATKeepAlive();

  if( GetType() == Server )
  {
    To to = GetToURI();
    manager.GetRegistrationDB().RemoveRegistration( to.GetURI() );
  }
}

void RegisterSession::OnTimerExpire(
  SIPTimerExpire & /*timerEvent*/
)
{
  GCREF( "OnTimerExpire" );
  OnRegistrationTimeout();
  Destroy();
}

void RegisterSession::OnRFC3680NewSubscription( 
  const SIPMessage & request 
)
{
  GCREF( "OnRFC3680NewSubscription" );
  m_IsBrandNewSession = FALSE;
  dynamic_cast<RegisterSessionManager&>(GetSessionManager()).OnRFC3680NewSubscription( *this, request );
}

void RegisterSession::OnRFC3680SubscriptionRequest(
  const SIPMessage & request
)
{
  GCREF( "OnRFC3680SubscriptionRequest" );
  dynamic_cast<RegisterSessionManager&>(GetSessionManager()).OnRFC3680SubscriptionRequest( *this, request );
}

void RegisterSession::OnRFC3680SubscriptionExpire()
{
  GCREF( "OnRFC3680SubscriptionExpire" );
  dynamic_cast<RegisterSessionManager&>(GetSessionManager()).OnRFC3680SubscriptionExpire( *this );
}

void RegisterSession::OnRFC3680Unsubscribe(
  const SIPMessage & request
)
{
  GCREF( "OnRFC3680Unsubscribe" );
  SIPMessage response;
  request.CreateResponse( response, SIPMessage::Code200_Ok );
  SendRequest( response );
  dynamic_cast<RegisterSessionManager&>(GetSessionManager()).OnRFC3680Unsubscribe( *this, request );
}

void RegisterSession::OnRFC3680Notification(
  const SIPMessage & request
)
{
  GCREF( "OnRFC3680Notification" );
  SIPMessage response;
  request.CreateResponse( response, SIPMessage::Code200_Ok );
  SendRequest( response );
  dynamic_cast<RegisterSessionManager&>(GetSessionManager()).OnRFC3680Notification( *this, request );
}


