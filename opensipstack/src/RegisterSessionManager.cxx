/*
 *
 * RegisterSessionManager.cxx
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
 * $Log: RegisterSessionManager.cxx,v $
 * Revision 1.27  2007/01/17 00:12:19  joegenbaclor
 * 1.  Fixed bug in call session where CANCEL may not be sent if a call ahs been authenticated due to a misplaced SetState( StateDisconnected ) for 4xx events
 * 2.  Fixed register session to ignore authorization headers if allow all reg is flagged
 * 3.  Temporary hacks for syslog
 * 4.  More launcher code -  almost there!
 *
 * Revision 1.26  2006/12/20 16:24:46  joegenbaclor
 * Implemented keep-alives for upper reg.
 *
 * Revision 1.25  2006/12/07 09:12:21  joegenbaclor
 * 1.  Changed registration expire value from milliseconds to seconds
 *
 * Revision 1.24  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.23  2006/10/30 02:42:27  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.22  2006/10/23 20:22:28  joegenbaclor
 * Modified Registration Methods to include a reference to the Register request
 *
 * Revision 1.21  2006/09/04 02:58:53  joegenbaclor
 * 1.  More bug fixes to garbage collector
 * 2.  Added new Logging macro so call System Log for fatal errors
 *
 * Revision 1.20  2006/08/30 03:42:05  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.19  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.18  2006/08/26 14:31:44  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.17  2006/08/14 01:29:08  joegenbaclor
 * Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
 *
 * Revision 1.16  2006/07/24 07:33:56  joegenbaclor
 * Minor bug fixes allover
 *
 * Revision 1.15  2006/07/11 13:59:36  joegenbaclor
 * Completed Proxy functionalities for B2BUA
 *
 * Revision 1.14  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
 *
 * Revision 1.13  2006/06/08 04:54:20  joegenbaclor
 * !.  Added OnRegistrationTimeout Call back to Registrar classes
 * 2. Moved ECCIAPI to Tools namespace
 *
 * Revision 1.12  2006/05/22 06:20:57  joegenbaclor
 * Added Registrar Contact Recovery via SQLite
 *
 * Revision 1.11  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.10  2006/04/29 03:22:54  joegenbaclor
 * 1.  More Refer support code added to call session
 * 2.  Added StoreRegistrationInfo callback to Registrar classes
 *
 * Revision 1.9  2006/03/27 15:25:10  joegenbaclor
 * Last minute improvements before creating a new tarball for download including
 * 1.  Major bug fix in simple-proxy where events may leak for REGISTER transactions.
 * 2.  Used incremental logging in HTTP admin.
 * 3.  Removed unused parameters in HTTPAdmin.
 * 4.  Local domain calls are now record-routed.
 * 5.  Fixed bug where ACK routeset is not popped during relay.
 *
 * Revision 1.8  2006/03/23 05:37:58  joegenbaclor
 * Got rid of some compile warning in Visual Studio 2003
 *
 * Revision 1.7  2006/03/21 14:11:50  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.6  2006/03/21 09:01:53  joegenbaclor
 * Introduced  a Registration Database to cache REGISTER information
 *
 * Revision 1.5  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "RegisterSessionManager.h"

#define new PNEW

using namespace UACORE;


RegisterSessionManager::RegisterSessionManager(
  SIPUserAgent & ua,
  PINDEX sessionThreadCount,
  PINDEX stackSize
) : SIPSessionManager( ua, sessionThreadCount, stackSize, "Registrar" )
{
  m_RegisterSessionsByURI.DisallowDeleteObjects();
  m_RFC3265Agent = new RFC3265StateAgent( ua );
  m_RFC3265Agent->AddPackage( "reg", new RFC3680Package( *m_RFC3265Agent ) );
  m_RegEventNotifier = PCREATE_NOTIFIER( _OnRegistrationEvent );
  m_XORHashClientRegistrations = FALSE;
  m_WillAcceptAllRegistrations = FALSE;
}

RegisterSessionManager::~RegisterSessionManager()
{
  if( m_RFC3265Agent != NULL )
    delete m_RFC3265Agent;
}

BOOL RegisterSessionManager::RemoveSessionFromURIList(
  const PString & uri
)
{
  PWaitAndSignal lock( m_SessionsMutex );
  RegisterSession * sessionByURI = m_RegisterSessionsByURI.RemoveAt( uri );
  return sessionByURI != NULL;
}

RegisterSession * RegisterSessionManager::FindSessionByURI( 
  const PString & uri,
  SIPSession::GCRef & autoRef
)
{
  PWaitAndSignal lock( m_SessionsMutex );
  RegisterSession * sessionByURI = m_RegisterSessionsByURI.GetAt( uri );
  if( sessionByURI != NULL )
    autoRef = sessionByURI->GCCreateRef( "Auto" );
  return sessionByURI;
}

void RegisterSessionManager::OnIncomingSIPMessage(
  SIPMessageArrival & messageEvent,
  SIPSession * session
)
{
  const SIPMessage & msg = messageEvent.GetMessage();

  if( msg.IsRequest() && !msg.IsRegister() && !msg.IsSubscribe() && !msg.IsNotify()  )
  {
    // respond back with bad request
    SIPMessage response;
    msg.CreateResponse( response, SIPMessage::Code400_BadRequest );
    GetUserAgent().SendRequest( response );
    return;
  }

  if( msg.IsSubscribe() || msg.GetCSeqMethod() == "SUBSCRIBE" )
  {
    
    if( msg.IsRequest() )
    {
      SIPSessionManager::OnIncomingSIPMessage( messageEvent, session );
    }else
    {
      SIPSession::GCRef autoRef;
        RegisterSession * regSession = FindGCRefByCallId<RegisterSession>( msg.GetCallId(), autoRef );
      if( regSession != NULL )
        regSession->OnHandleResponse( msg );
    }
  }else if( msg.IsNotify() || msg.GetCSeqMethod() == "NOTIFY" )
  {
    if( msg.IsRequest() )
    {
      SIPSessionManager::OnIncomingSIPMessage( messageEvent, session );
    }else if( session != NULL )
    {
      SIPSession::GCRef autoRef;
        RegisterSession * regSession = FindGCRefByCallId<RegisterSession>( msg.GetCallId(), autoRef );
      if( regSession != NULL )
        regSession->OnHandleResponse( msg );
    }
  }else if( !msg.IsUnregister() )
  {
    /// check if the message is a 401 authentication required
    if( !msg.IsRequest() )
    {
      PWaitAndSignal lock( m_SessionsMutex );
      
      To to;
      if( !msg.GetTo( to ) )
        return;

      PString uri = to.GetURI().AsString();

      SIPSession::GCRef autoRef;
      RegisterSession * regSession = FindSessionByURI( uri, autoRef );

      if( regSession != NULL )
        regSession->OnHandleResponse( msg );
      else
      {
        /// check if we have a sessin by call-id
        SIPSession::GCRef autoRef;
        regSession = FindGCRefByCallId<RegisterSession>( msg.GetCallId(), autoRef );
        
        if( regSession != NULL )
          regSession->OnHandleResponse( msg );
      }

      return;
    }else
    {
      SIPSessionManager::OnIncomingSIPMessage( messageEvent, session );
      return;
    }
  }else if( msg.IsUnregister() )
  {
    /// this is an UnRegister
    PWaitAndSignal lock( m_SessionsMutex );
    
    To to;
    if( !msg.GetTo( to ) )
      return;

    PString uri = to.GetURI().AsString();

    SIPSession::GCRef autoRef;
    RegisterSession * sessionByURI = FindSessionByURI( uri, autoRef );

    if( sessionByURI != NULL )
    {
      sessionByURI->SendAcceptByOk( messageEvent.GetMessage() );
      PString uri = sessionByURI->GetURI().AsString();
      RemoveSessionFromURIList( uri );
      ///sessionByURI->GCCollect();
      sessionByURI->OnUnregistration( messageEvent.GetMessage() );
    }else
    {
      /// just send an OK
      LOG_IF_DEBUG( LogWarning(), "UN-Register without a session present " << msg );
      SIPMessage ok;
      msg.CreateResponse( ok, SIPMessage::Code200_Ok);
      TransactionId tid;
      GetUserAgent().SendRequest( ok, tid );
    }
  }
}

/// returns a new RegisterSession.  Users may override this function 
/// to return their own implementation of the RegisterSession object
SIPSession * RegisterSessionManager::OnCreateClientSession(
  const ProfileUA & profile,
  const PString & sessionId
)
{
  PWaitAndSignal lock( m_SessionsMutex );

  if( m_RegisterSessionsByURI.Contains( sessionId ) )
    return m_RegisterSessionsByURI.GetAt( sessionId );

  RegisterSession * regSession = new RegisterSession( *this, profile, sessionId  );
  regSession->EnableXORHash( m_XORHashClientRegistrations );
  PString uri = regSession->GetURI().AsString();
  m_RegisterSessionsByURI.SetAt( uri, regSession );
  return regSession;
}

SIPSession::GCRef  RegisterSessionManager::OnCreateServerSession(
  const SIPMessage & request
)
{
  if( !request.IsRegister() )
    return SIPSession::GCRef( NULL, "SIPSession" );

  PWaitAndSignal lock( m_SessionsMutex );

  CallId callId;
  request.GetCallId( callId );
  PString sessionId = callId.GetHeaderBody();
  RegisterSession * regSession = new RegisterSession( *this, request, sessionId );
  PString uri = regSession->GetURI().AsString();

  RegisterSession * regSessionByURI = m_RegisterSessionsByURI.RemoveAt( uri );
  if( regSessionByURI != NULL )
  {
    SIPSession::GCRef autoRef = regSessionByURI->GCCreateRef( "Auto" );
    regSessionByURI->Destroy();
  }

  m_RegisterSessionsByURI.SetAt( uri, regSession );
  return regSession->GCCreateRef( "RegisterSessionManager::OnCreateServerSession" );
}

RegisterSession::AcceptRegistrationResponse RegisterSessionManager::OnAcceptRegistration( 
  RegisterSession & /*session*/,
  const SIPMessage & /*request*/
)
{
  return RegisterSession::AcceptByRejection;
}

BOOL RegisterSessionManager::OnStoreRegistrationInfo(
  RegisterSession & session,
  const SIPURI & accountInfo
)
{
  return TRUE;
}

void RegisterSessionManager::OnRegistrationAccepted(
  RegisterSession & session,
  const SIPMessage & request
)
{
}

void RegisterSessionManager::OnRegistrationRejected(
  RegisterSession & session,
  const SIPMessage & request 
)
{
}

void RegisterSessionManager::OnRegistrationTimeout(
  RegisterSession & session 
)
{
}


void RegisterSessionManager::OnUnregistration(
  RegisterSession & session,
  const SIPMessage & request
)
{
}

/////////////////////////////////////////////////////////////////////////

RegistrationDatabase::RegistrationDatabase()
{
#if HAS_CPPSQLITE
  m_HasContactRecovery = PrepareContactRecoveryDB( "ContactRecovery.sqlite" );
  if( m_HasContactRecovery )
    RecoverRegistrations();
#endif
}

RegistrationDatabase::~RegistrationDatabase()
{
}

BOOL RegistrationDatabase::AddRegistration(
  const SIPURI & userURI,
  const SIPMessage & reg,
  int expires
)
{
  PWaitAndSignal lock( m_CacheMutex );
  CacheManager * cache = (CacheManager *)this;
  PString id = userURI.GetUser() + PString("@") + userURI.GetHost();
  cache->RemoveAt( id );
  cache->Append( new CacheObject( id, new SIPMessage( reg ), (int)(expires * 1.5) ) );

#if HAS_CPPSQLITE
  if( m_HasContactRecovery )
    AddRegistrationRecovery( userURI, reg, expires );
#endif

  return TRUE;
}

BOOL RegistrationDatabase::RemoveRegistration(
  const SIPURI & userURI
)
{
  PWaitAndSignal lock( m_CacheMutex );
  CacheManager * cache = (CacheManager *)this;
  PString id = userURI.GetUser() + PString("@") + userURI.GetHost();
  cache->RemoveAt( id, TRUE );

#if HAS_CPPSQLITE
  if( m_HasContactRecovery )
    RemoveRegistrationRecovery( userURI );
#endif

  return TRUE;
}

BOOL RegistrationDatabase::FindRegistration(
  const SIPURI & userURI,
  SIPMessage & msg
)
{
  PWaitAndSignal lock( m_CacheMutex );
  CacheManager * cache = (CacheManager *)this;
  PString id = userURI.GetUser() + PString("@") + userURI.GetHost();
  CacheObject * co = cache->GetAt( id );
  if( co == NULL )
    return FALSE;

  SIPMessage * rec = (SIPMessage *)co->GetObject();
  
  PAssertNULL( rec );

  msg = *rec;

  return TRUE;

}

BOOL RegistrationDatabase::GetRegistration(
  PINDEX i,
  SIPMessage & reg
)
{
  PWaitAndSignal lock( m_CacheMutex );
  CacheManager * cache = dynamic_cast<CacheManager *>(this);
  CacheObject * co = cache->GetAt( i );

  if( co == NULL )
    return FALSE;

  SIPMessage * rec = dynamic_cast<SIPMessage *>(co->GetObject());
  
  PAssertNULL( rec );

  reg = *rec;
  return TRUE;
}

#if HAS_CPPSQLITE

BOOL RegistrationDatabase::PrepareContactRecoveryDB(
  const PFilePath & db
)
{
  if( PFile::Exists( db ) )
  {
    m_SQLiteDB.open( db );
    return TRUE;
  }

  PString sql = "CREATE TABLE ContactRecovery( timestamp int, uri char(256), register char( 1300 ), expires int);";
  try
  {
    m_SQLiteDB.open( db );
    m_SQLiteDB.execDML( sql );
  }catch (CppSQLite3Exception& e)
  {
    PTRACE( 1, "PrepareContactRecoveryDB - SQLite\t" << e.errorCode() << ":" << e.errorMessage() );
    PFile::Remove( db, TRUE );
    return FALSE;
  }

  return TRUE;
}

BOOL RegistrationDatabase::AddRegistrationRecovery(
  const SIPURI & userURI,
  const SIPMessage & reg,
  int expires
)
{
  PWaitAndSignal lock( m_SQLiteDBMutex );
  PTime now;
  PStringStream sql;
  sql << "INSERT INTO ContactRecovery values(" 
      << now.GetTimeInSeconds() << ", "
      << "\"" << userURI << "\"" << ", "
      << "\"" << PBase64::Encode( reg.AsString() ) << "\"" << ", "
      << expires << ");";

  PStringStream del;
  del << "DELETE FROM ContactRecovery WHERE uri = \"" << userURI << "\";";

  try
  {
    m_SQLiteDB.execDML( "BEGIN TRANSACTION;" );
    m_SQLiteDB.execDML( del );
    m_SQLiteDB.execDML( "COMMIT TRANSACTION;" );
    m_SQLiteDB.execDML( "BEGIN TRANSACTION;" );
    m_SQLiteDB.execDML( sql );
    m_SQLiteDB.execDML( "COMMIT TRANSACTION;" );
  }catch (CppSQLite3Exception& e)
  {
    PTRACE( 1, "AddRegistrationRecovery - SQLite\t" << e.errorCode() << ":" << e.errorMessage() );
    return FALSE;
  }

  return TRUE;
}

BOOL RegistrationDatabase::RemoveRegistrationRecovery(
  const SIPURI & userURI
)
{
  PWaitAndSignal lock( m_SQLiteDBMutex );
  PStringStream del;
  del << "DELETE FROM ContactRecovery WHERE uri = \"" << userURI << "\";";

  try
  {
    m_SQLiteDB.execDML( "BEGIN TRANSACTION;" );
    m_SQLiteDB.execDML( del );
    m_SQLiteDB.execDML( "COMMIT TRANSACTION;" );
  }catch (CppSQLite3Exception& e)
  {
    PTRACE( 1, "AddRegistrationRecovery - SQLite\t" << e.errorCode() << ":" << e.errorMessage() );
    return FALSE;
  }

  return TRUE;
}

BOOL RegistrationDatabase::RecoverRegistrations()
{
  PString sql = "SELECT * from ContactRecovery;";
  PTime now;

  try
  {
    CppSQLite3Query q = m_SQLiteDB.execQuery( sql );
    while (!q.eof())
    {
      
      /// time stamp
      if( q.fieldIsNull( 0 ) || q.fieldIsNull( 1 ) || q.fieldIsNull( 2 ) || q.fieldIsNull( 3 ) )
      {
        q.nextRow();
        continue;
      }

      PString timeStamp = q.fieldValue( 0 );
      PString uri = q.fieldValue( 1 );
      SIPURI userURI( uri );
      PString reg = PBase64::Decode( q.fieldValue( 2 ) );
      PString expires = q.fieldValue( 3 );
     
      if( now.GetTimeInSeconds() - timeStamp.AsInteger() < 3600 )
      {

        PWaitAndSignal lock( m_CacheMutex );
        
        PTRACE( 1, "*** CONTACT RECOVERY *** " << uri << ";expires=" << expires );
        CacheManager * cache = (CacheManager *)this;
        PString id = userURI.GetUser() + PString("@") + userURI.GetHost();
        cache->RemoveAt( id );
        cache->Append( new CacheObject( id, new SIPMessage( reg ), (int)(expires.AsInteger() * 1.1) ) );
      }else
      {
        RemoveRegistrationRecovery( userURI );
      }

      q.nextRow();
    }
  }catch( CppSQLite3Exception& e)
  {
    PTRACE( 1, "RecoverRegistrations - SQLite\t" << e.errorCode() << ":" << e.errorMessage() );
    return FALSE;
  }
  return TRUE;
}

#endif

BOOL RegisterSessionManager::IsKnownUser(
  const SIPURI & usrURI
)const
{
  PAssertAlways( PUnimplementedFunction );
  return FALSE;
}

void RegisterSessionManager::_OnRegistrationEvent( 
  RFC3265NotificationEvent & regEvent,
  INT subscription
)
{
  if( subscription == 0 )
    return;

  OnRegistrationEvent( regEvent, reinterpret_cast<RFC3265Subscription*>(subscription) );
}

void RegisterSessionManager::OnRegistrationEvent( 
  const RFC3265NotificationEvent & regEvent,
  const RFC3265Subscription * subscription
)
{
  SIPURI subscriber = subscription->GetSubscriberURI();
  /// see if subscriber is registered;
  SIPMessage registration;
  SIPURI targetURI;

  if( GetRegistrationDB().FindRegistration( subscriber, registration ) )
  {
    /// check the via if this is a NATted registration
    Via via;
    if( registration.GetViaAt( registration.GetViaSize() - 1, via ) 
      && via.IsBehindNAT()  
    )
    {
      targetURI.SetHost( via.GetReceiveAddress().AsString() );
      targetURI.SetPort( PString( via.GetRPort() ) );
    }else
    {
      Contact & contact = registration.GetContactAt( 0 );
      ContactURI contactURI;
      contact.GetURI( contactURI );
      targetURI = contactURI.GetURI();
    }

  }
}

void RegisterSessionManager::OnRFC3680NewSubscription(
  RegisterSession & session,
  const SIPMessage & request 
)
{
    RFC3680Package * regPackage = 
      dynamic_cast<RFC3680Package *>(m_RFC3265Agent->FindPackage( "reg" ));
    SIPMessage response;
    if( regPackage != NULL )
    {
      request.CreateResponse( response, SIPMessage::Code200_Ok );
      session.SendRequest( response );

      SIPURI resourceURI;
      SIPURI toURI = request.GetToURI();
      resourceURI.SetUser( toURI.GetUser() );
      resourceURI.SetHost( toURI.GetHost() );

      /// check if theres a resource
      RFC3265Resource::GCRef resource = regPackage->FindResource( resourceURI );
      if( resource == NULL && IsKnownUser( resourceURI ) )
        resource = regPackage->CreateResource( resourceURI );

      if( resource != NULL )
      {

          RFC3265Subscription * subscription = new RFC3265Subscription( 
            request.GetCallId(),
            request.GetFromURI(),
            resourceURI,
            m_RegEventNotifier,
            *regPackage );

          resource->Subscribe( subscription );
      }
    }else
    {
      request.CreateResponse( response, SIPMessage::Code500_InternalServerError );
      session.SendRequest( response );
    }
}

void RegisterSessionManager::OnRFC3680SubscriptionRequest(
  RegisterSession & session,
  const SIPMessage & request
)
{
  SIPMessage response;
  request.CreateResponse( response, SIPMessage::Code200_Ok );
  session.SendRequest( response );
}

void RegisterSessionManager::OnRFC3680SubscriptionExpire(
  RegisterSession & session
)
{
}

void RegisterSessionManager::OnRFC3680Unsubscribe(
  RegisterSession & session,
  const SIPMessage & request
)
{
}

void RegisterSessionManager::OnRFC3680Notification(
  RegisterSession & session,
  const SIPMessage & request
)
{
}



