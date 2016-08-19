/*
 *
 * RegisterSessionManager.h
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
 * $Log: RegisterSessionManager.h,v $
 * Revision 1.21  2007/01/17 00:12:19  joegenbaclor
 * 1.  Fixed bug in call session where CANCEL may not be sent if a call ahs been authenticated due to a misplaced SetState( StateDisconnected ) for 4xx events
 * 2.  Fixed register session to ignore authorization headers if allow all reg is flagged
 * 3.  Temporary hacks for syslog
 * 4.  More launcher code -  almost there!
 *
 * Revision 1.20  2007/01/08 11:15:51  joegenbaclor
 * Added capability to add status pages
 *
 * Revision 1.19  2006/12/20 16:24:44  joegenbaclor
 * Implemented keep-alives for upper reg.
 *
 * Revision 1.18  2006/12/07 09:12:21  joegenbaclor
 * 1.  Changed registration expire value from milliseconds to seconds
 *
 * Revision 1.17  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.16  2006/10/30 02:42:26  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.15  2006/10/23 20:22:28  joegenbaclor
 * Modified Registration Methods to include a reference to the Register request
 *
 * Revision 1.14  2006/08/30 03:42:04  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.13  2006/08/26 14:31:40  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.12  2006/07/10 06:29:38  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.11  2006/06/08 04:54:20  joegenbaclor
 * !.  Added OnRegistrationTimeout Call back to Registrar classes
 * 2. Moved ECCIAPI to Tools namespace
 *
 * Revision 1.10  2006/05/22 06:20:57  joegenbaclor
 * Added Registrar Contact Recovery via SQLite
 *
 * Revision 1.9  2006/04/29 03:22:54  joegenbaclor
 * 1.  More Refer support code added to call session
 * 2.  Added StoreRegistrationInfo callback to Registrar classes
 *
 * Revision 1.8  2006/03/21 14:11:50  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.7  2006/03/21 09:01:49  joegenbaclor
 * Introduced  a Registration Database to cache REGISTER information
 *
 * Revision 1.6  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef REGSESSIONMANAGER_H
#define REGSESSIONMANAGER_H

#include "SIPSessionManager.h"
#include "RegisterSession.h"
#include "CacheManager.h"
#include "SQLiteDb.h"
#include "RFC3680Package.h"

using namespace SIP;
using namespace Profiles;
using namespace Cache;
using namespace RFC3680;

namespace UACORE
{

  class RegistrationDatabase : public CacheManager
  {
    PCLASSINFO( RegistrationDatabase, CacheManager );

  public:
    RegistrationDatabase();

    virtual ~RegistrationDatabase();

    BOOL AddRegistration(
      const SIPURI & userURI,
      const SIPMessage & reg,
      int expires
    );

    BOOL RemoveRegistration(
      const SIPURI & userURI
    );

    BOOL FindRegistration(
      const SIPURI & userURI,
      SIPMessage & reg
    );

    BOOL GetRegistration(
      PINDEX i,
      SIPMessage & reg
    );

    PINLINE PMutex & GetMutex(){ return m_CacheMutex; };

#if HAS_CPPSQLITE
  protected:
    BOOL PrepareContactRecoveryDB(
      const PFilePath & db
    );

    BOOL AddRegistrationRecovery(
      const SIPURI & userURI,
      const SIPMessage & reg,
      int expires
    );

    BOOL RemoveRegistrationRecovery(
      const SIPURI & userURI
    );

    BOOL RecoverRegistrations();

    CppSQLite3DB m_SQLiteDB;
    BOOL m_HasContactRecovery;
    PMutex m_SQLiteDBMutex;
#endif

  };

  /////////////////////////////////////////////////////////////////////////

  class RegisterSessionManager : public SIPSessionManager
  {
    PCLASSINFO( RegisterSessionManager, SIPSessionManager );
  public:


    RegisterSessionManager(
      SIPUserAgent & ua,
      PINDEX sessionThreadCount,
      PINDEX stackSize
    );

    virtual ~RegisterSessionManager();

    BOOL RemoveSessionFromURIList(
      const PString & uri
    );

    RegisterSession * FindSessionByURI(
      const PString & uri,
      SIPSession::GCRef & autoRef
    );

    virtual void OnIncomingSIPMessage(
      SIPMessageArrival & messageEvent,
      SIPSession * session
    );

    virtual RegisterSession::AcceptRegistrationResponse OnAcceptRegistration(
      RegisterSession & session,
      const SIPMessage & request
    );

    virtual BOOL OnRequestA1Hash(
      const SIPURI & userURL,
      PString & a1,
      RegisterSession & session
    ) = 0;

    /// this will be called before calling OnRegistrationAccepted
    /// Returning FALSE here will result to a rejection of the REGISTER
    /// request due to internal server error.  Default behavior
    /// returns TRUE
    virtual BOOL OnStoreRegistrationInfo(
      RegisterSession & session,
      const SIPURI & accountInfo
    );

    virtual void OnRegistrationAccepted(
      RegisterSession & session,
      const SIPMessage & request
    );

    virtual void OnRegistrationRejected(
      RegisterSession & session,
      const SIPMessage & request
    );

    virtual void OnRegistrationTimeout(
      RegisterSession & session
    );

    virtual void OnUnregistration(
      RegisterSession & session,
      const SIPMessage & request
    );


    /// returns a new Client RegisterSession.  Users may override this function
    /// to return their own implementation of the RegisterSession object
    virtual SIPSession * OnCreateClientSession(
      const ProfileUA & profile,
      const PString & sessionId
    );


    /// returns a new Client RegisterSession.  Users may override this function
    /// to return their own implementation of the RegisterSession object
    virtual SIPSession::GCRef OnCreateServerSession(
      const SIPMessage & request
    );

    PINLINE RegistrationDatabase & GetRegistrationDB(){ return m_RegistrationDB; };

    PDECLARE_NOTIFIER( RFC3265NotificationEvent, RegisterSessionManager, _OnRegistrationEvent );

    virtual void OnRegistrationEvent( 
      const RFC3265NotificationEvent & regEvent,
      const RFC3265Subscription * subscription
    );

    virtual void OnRFC3680NewSubscription(
      RegisterSession & session,
      const SIPMessage & request 
    );

    virtual void OnRFC3680SubscriptionRequest(
      RegisterSession & session,
      const SIPMessage & request
    );

    virtual void OnRFC3680SubscriptionExpire(
      RegisterSession & session
    );
    
    virtual void OnRFC3680Unsubscribe(
      RegisterSession & session,
      const SIPMessage & request
    );
    
    virtual void OnRFC3680Notification(
      RegisterSession & session,
      const SIPMessage & request
    );

    virtual BOOL IsKnownUser(
      const SIPURI & usrURI
    )const;

    PINLINE void SetEnableXORHash( BOOL hash = TRUE ){ m_XORHashClientRegistrations = hash; };
    PINLINE BOOL IsXORHashEnabled()const{ return m_XORHashClientRegistrations; };
    PINLINE BOOL WillAcceptAllRegistrations()const{ return m_WillAcceptAllRegistrations; };
    PINLINE void SetAcceptAllRegistrations( BOOL accept = TRUE ){ m_WillAcceptAllRegistrations = accept; };
    protected:
      BOOL m_WillAcceptAllRegistrations;
      RegisterSession::SortedCollection m_RegisterSessionsByURI;
      RegistrationDatabase m_RegistrationDB;
      RFC3265StateAgent * m_RFC3265Agent;
      PNotifier m_RegEventNotifier;
      BOOL m_XORHashClientRegistrations;
  };

  

  
};

#endif

