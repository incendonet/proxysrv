/*
 *
 * SIPSession.h
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
 * $Log: SIPSession.h,v $
 * Revision 1.40  2007/01/22 10:01:57  joegenbaclor
 * Fixed bug in handling Notifies
 *
 * Revision 1.39  2006/12/21 04:14:47  joegenbaclor
 * Fixed bug in NAT traversal logic where NAT device port is not used when sending requests within dialog
 *
 * Revision 1.38  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.37  2006/11/21 06:55:17  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.36  2006/11/18 03:14:26  joegenbaclor
 * Implemented session loop detection mechanism using iid via parameter
 *
 * Revision 1.35  2006/11/09 06:26:00  joegenbaclor
 * Added IsDialogEstablished method
 *
 * Revision 1.34  2006/11/08 06:01:03  joegenbaclor
 * added class keyword in GCObject friend declaration
 *
 * Revision 1.33  2006/10/30 03:42:42  joegenbaclor
 * Protected the destructor from getting deleted outside of the garbage collector
 *
 * Revision 1.32  2006/10/23 20:22:28  joegenbaclor
 * Modified Registration Methods to include a reference to the Register request
 *
 * Revision 1.31  2006/10/23 15:44:54  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.30  2006/10/05 12:50:43  rcolobong
 * 1. Add a template method to store a copy of an object.
 *
 * Revision 1.29  2006/09/18 09:35:15  joegenbaclor
 * Completed initial implementation of RTP media aggregation
 *
 * Revision 1.28  2006/09/08 04:39:16  joegenbaclor
 * Implemented INFO Session Keep-alive
 *
 * Revision 1.27  2006/09/07 09:49:56  joegenbaclor
 * Fixed bug on possible race conditions when multiple thread calls Session::Destroy
 *  at the same time
 *
 * Revision 1.26  2006/08/30 03:42:04  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.25  2006/08/28 07:18:34  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.24  2006/08/26 14:31:40  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.23  2006/08/14 01:29:06  joegenbaclor
 * Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
 *
 * Revision 1.22  2006/08/13 06:49:07  joegenbaclor
 * introduced mutex for m_CurrentRequest
 *
 * Revision 1.21  2006/07/24 07:33:56  joegenbaclor
 * Minor bug fixes allover
 *
 * Revision 1.20  2006/07/06 07:05:59  joegenbaclor
 * Changed return value of EnableXORHash from BOOL to void
 *
 * Revision 1.19  2006/07/06 05:38:10  joegenbaclor
 * Final last minute changes to CBCOM hash support
 *
 * Revision 1.18  2006/04/08 02:38:15  joegenbaclor
 * 1.  Corrected bug in call session where  OnConnected() is called befoe OnSDPOffe.
 * 2.  Corrected bug where Cancel syncpoint may not be signalled making calls to SendCancel block
 *
 * Revision 1.17  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.16  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.15  2006/03/27 01:25:33  joegenbaclor
 * Some improvements on NICT and NIST state machines
 *
 * Revision 1.14  2006/03/25 18:40:13  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.13  2006/03/24 15:42:13  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.12  2006/03/21 14:11:50  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.11  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.10  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.9  2006/03/14 17:00:00  joegenbaclor
 * Implemented Dialog methods in SIPSession object
 *
 * Revision 1.8  2006/03/14 05:17:12  joegenbaclor
 * Implemented CallSessionManager::OnRequireSDPOffer() to generate an interim Serssion Description
 *
 * Revision 1.7  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPSESSION_H
#define SIPSESSION_H

#include "SIPStack.h"
#include "ProfileUA.h"
#include "SDPSessionDescription.h"
#include "EventQueue.h"
#include "DialogId.h"
#include "Logger.h"
#include "GCObject.h"

using namespace SIP;
using namespace Profiles;
using namespace SDP;
using namespace EQ;
using namespace Tools;

namespace UACORE
{
  class SIPSessionManager;
  class SIPSessionEvent;

  class SIPSession : public SIPSmartObject, public Logger
  {
    GCREFINFO( SIPSession, "SIPSession" );
    PCLASSINFO( SIPSession, SIPSmartObject );
  
  public:
    enum Type
    {
      Client,
      Server,
      MultiDirectional
    };

    enum DialogType
    {
      NotInDialog,
      UAC,
      UAS
    };

    enum SessionKeepAliveMethod
    {
      SessionTimer,
      Notify,
      Options
    };

    SIPSession(
      SIPSessionManager & manager,
      const PString & sessionId
    );

    SIPSession(
      SIPSessionManager & manager,
      const PString & sessionId,
      const ProfileUA & profile
    );

    SIPSession(
      SIPSessionManager & manager,
      const SIPMessage & reqeust,
      const PString & sessionId
    );

  protected:
    /// protect the destructor from 
    /// getting deleted outside of the garbage collector
    virtual ~SIPSession();
    friend class GCObject<SIPSession>;
  public:

    virtual void Cleanup();

    virtual void OnDestroySession();

    void Destroy();

    BOOL SendRequest(
      const SIPMessage & request
    );

    void SendAcceptByOk(
      const SIPMessage & request,
      const PString & body = PString::Empty(),
      const PString & contentType = "text/plain"
    );

    void SendAcceptByOk(
      const PString & body = PString::Empty(),
      const PString & contentType = "text/plain"
    );

    BOOL SendAcceptByRejection(
      const SIPMessage & requestToReject,
      PINDEX statusCode = SIPMessage::Code403_Forbidden,
      const PString & resonPhrase = PString::Empty(),
      const PString & warning = PString::Empty()
    );

    BOOL SendAcceptByRejection(
      PINDEX statusCode = SIPMessage::Code403_Forbidden,
      const PString & resonPhrase = PString::Empty(),
      const PString & warning = PString::Empty()
    );

    BOOL SendAcceptByRedirection(
      const SIPMessage & requestToRedirect,
      const ContactList & contactList,
      PINDEX statusCode = SIPMessage::Code302_MovedTemporarily
    );

    BOOL SendAcceptByRedirection(
      const ContactList & contactList,
      PINDEX statusCode = SIPMessage::Code302_MovedTemporarily
    );

    virtual void OnIncomingSIPMessage(
      SIPMessageArrival & messageEvent
    );

    virtual void OnSIPMessageSent(
      SIPTransportWrite & messageEvent
    );

    virtual void OnTimerExpire(
      SIPTimerExpire & timerEvent
    );

    virtual void OnTransportFailure(
      int failureType
    );

    virtual void OnSessionEvent(
      int event,
      const SIPMessage & eventMsg
    );

    void EnqueueSessionEvent(
      SIPSessionEvent * event
    );

    virtual void PrintOn(
      ostream & strm
    )const;

            /// Internal header manipulators
    ///////
    BOOL GetInternalHeader(
      const PCaselessString & name,
      SIPHeader & h,
      PINDEX i = 0
    )const;

    PString GetInternalHeader(
      const PCaselessString & name,
      PINDEX i = 0
    )const;

    BOOL GetInternalHeader(
      SIPHeader & h,
      PINDEX i
    )const;

    void SetInternalHeader(
      const PCaselessString & name,
      const SIPHeader & h,
      PINDEX i = 0
    );

    void SetInternalHeader(
      const SIPHeader & h,
      PINDEX i
    );

    void AddInternalHeader(
      const SIPHeader & h
    );

    void AddInternalHeader(
      const PString & name,
      const PString & value
    );

    BOOL PopInternalHeader(
      const PCaselessString & name,
      SIPHeader & h,
      PINDEX i = 0
    );

    BOOL GetInternalHeader(
      const PCaselessString & name,
      SIPHeader & h,
      PINDEX i = 0
    );

    virtual BOOL InitializeDialogAsUAC(
      const SIPMessage & request,
      const SIPMessage & response
    );

    virtual BOOL InitializeDialogAsUAS(
      const SIPMessage & request,
      const SIPMessage & response
    );

    virtual BOOL CreateRequestWithinDialog(
      SIPMessage::Methods method,
      SIPMessage & request
    )const;

    virtual BOOL CloneRequestWithinDialog(
      const SIPMessage & request,
      SIPMessage & clone
    )const;

    void StartNATKeepAlive(
      const SIPURI & target,
      int expires
    );

    void StopNATKeepAlive();

    void StartSessionKeepAlive(
      const PTimeInterval & expires = 10000
    );

    void StopSessionKeepAlive();

    void StartAutoDestructTimer(
      int expires
    );

    virtual void OnSessionKeepAliveTimeout();

    void StopAutoDestructTimer();

    SIPTransportManager * GetTransportManager();
    
    template< class T >
    void AddInternalPObject( const T& object ) 
    { 
      PWaitAndSignal scopedLock( m_InternalPObjectsMutex ); 
      m_InternalPObjects.Append( new PObject( object ) ); 
    }

    BOOL IsRequestLooping( const SIPMessage & request )const;

    PINLINE const SIPMessage & GetRequest()const{ return m_Request; };
    PINLINE void SetRequest( const SIPMessage & request ){ m_Request = request; };
    PINLINE const PString & GetCallId()const{ return m_CallId; };
    PINLINE const SIPURI & GetURI()const{ return m_URI; };
    PINLINE SIPSessionManager & GetSessionManager(){ return m_SessionManager; };
    PINLINE const PString & GetSessionId()const{ return m_SessionId; };
    PINLINE Type GetType()const{ return m_Type; };
    PINLINE ProfileUA & GetSessionProfile(){ return m_SessionProfile; };
    PINLINE void SetSessionProfile( const ProfileUA & profile ){ m_SessionProfile = profile; };
    PINLINE BOOL WillProcessEvents()const{ return m_WillProcessEvents; };
    PDICTIONARY( SortedCollection, PCaselessString, SIPSession );

    /// Dialog Parameter Accessors

    PINLINE DialogType GetDialogType()const{ return m_DialogType; };
    PINLINE BOOL IsDialogEstablished()const{ return m_DialogType != NotInDialog; };
    PINLINE const PString & GetLocalTag()const{ return m_LocalTag; };
    PINLINE const PString & GetRemoteTag()const{ return m_RemoteTag; };
    PINLINE const RouteList & GetRemoteRouteSet()const{ return m_RemoteRouteSet; };
    PINLINE const Via & GetLocalVia()const{ return m_LocalVia; };
    PINLINE const DialogId & GetDialogId()const{ return m_DialogId; };
    PINLINE const ContactList & GetLocalContactList()const{ return m_LocalContactList; };
    PINLINE const ContactList & GetRemoteContactList()const{ return m_RemoteContactList; };
    PINLINE const SIPURI & GetLocalURI()const{ return m_LocalURI; };  // from URI to be used for request
    PINLINE const SIPURI & GetRemoteURI()const{ return m_RemoteURI; }; // to URI to be used for requests
    PINLINE const SIPURI & GetTargetURI()const{ return m_TargetURI; };/// the startline URI to use for requests
	  PINLINE const PString & GetInterfaceAddress()const{ return m_InterfaceAddress; };
	  PINLINE void SetInterfaceAddress( const PString & address ){ m_InterfaceAddress = address; } ;
    PINLINE WORD GetInterfacePort()const{ return m_InterfacePort; };
	  PINLINE void SetInterfacePort( WORD port ){ m_InterfacePort = port; } ;
	  PINLINE PINDEX GetLocalSequence()const{ return m_LocalSequence; };
    PINLINE PINDEX GetRemoteSequence()const{ return m_RemoteSequence; };
    PINLINE void SetLocalSequence( PINDEX cseq ){ m_LocalSequence = cseq; };
    PINLINE void SetRemoteSequence( PINDEX cseq ){ m_RemoteSequence = cseq; };
    PINLINE BOOL IsXORHashEnabled()const{ return m_EnableXORHash; };
    PINLINE void EnableXORHash( BOOL enable = TRUE ){ m_EnableXORHash = enable; };
    
    PINLINE const SIPMessage & GetCurrentUACRequest()const{ PWaitAndSignal lock( m_CurrentUACRequestMutex ); return m_CurrentUACRequest; };
    PINLINE void SetCurrentUACRequest( const SIPMessage & request ){ PWaitAndSignal lock( m_CurrentUACRequestMutex ); m_CurrentUACRequest = request; };
    PINLINE const SIPMessage & GetCurrentUACResponse()const{ PWaitAndSignal lock( m_CurrentUACResponseMutex ); return m_CurrentUACResponse; };
    PINLINE void SetCurrentUACResponse( const SIPMessage & response ){ PWaitAndSignal lock( m_CurrentUACResponseMutex ); m_CurrentUACResponse = response; };

    PINLINE const SIPMessage & GetCurrentUASRequest()const{ PWaitAndSignal lock( m_CurrentUASRequestMutex ); return m_CurrentUASRequest; };
    PINLINE void SetCurrentUASRequest( const SIPMessage & request ){ PWaitAndSignal lock( m_CurrentUASRequestMutex ); m_CurrentUASRequest = request; };
    PINLINE const SIPMessage & GetCurrentUASResponse()const{ PWaitAndSignal lock( m_CurrentUASResponseMutex ); return m_CurrentUASResponse; };
    PINLINE void SetCurrentUASResponse( const SIPMessage & response ){ PWaitAndSignal lock( m_CurrentUASResponseMutex ); m_CurrentUASResponse = response; };

    PINLINE const PString & GetLogTag()const{ return  m_LogTag; };

    PINLINE SessionKeepAliveMethod GetSessionKeepAliveMethod()const{ return m_SessionKeepAliveMethod; };
    PINLINE void SetSessionKeepAliveMethod( SessionKeepAliveMethod method ){ m_SessionKeepAliveMethod = method; };

    PINLINE const PIPSocket::Address GetInitialTargetAddress()const{ return m_InitialTargetAddress; };
    PINLINE WORD GetInitialTargetPort()const{ return m_InitialTargetPort; };

    PINLINE void SetSessionRejectResponse( SIPMessage::StatusCodes code ){ m_SessionRejectResponse = code; };
    PINLINE SIPMessage::StatusCodes GetSessionRejectResponse()const{ return m_SessionRejectResponse; };

  protected:

    PMutex m_SessionMutex;
    /// Custom Headers
    PLIST( InternalHeaders, SIPHeader );
    InternalHeaders m_InternalHeaders;
    PMutex m_InternalHeadersMutex;

    PLIST( InternalPObjects, PObject );
    InternalPObjects m_InternalPObjects;
    PMutex m_InternalPObjectsMutex;

    SIPMessage m_Request;
    SIPMessage m_CurrentUACRequest;
    SIPMessage m_CurrentUACResponse;
    PMutex m_CurrentUACRequestMutex;
    PMutex m_CurrentUACResponseMutex;
    SIPMessage m_CurrentUASRequest;
    SIPMessage m_CurrentUASResponse;
    PMutex m_CurrentUASRequestMutex;
    PMutex m_CurrentUASResponseMutex;

    ProfileUA m_SessionProfile;
    SIPSessionManager & m_SessionManager;
    PString m_SessionId;
    PString m_CallId;
    SIPURI m_URI;
    Type m_Type;
    PMutex m_InternalMutex;
    BOOL m_WillProcessEvents;
    SIPMessage::StatusCodes m_SessionRejectResponse;

    /// Dialog parameters
    PMutex m_DialogMutex;
    DialogType m_DialogType;
    RouteList m_RemoteRouteSet;
    PString m_LocalTag;
    PString m_RemoteTag;
    DialogId m_DialogId;
    ContactList m_LocalContactList;
    ContactList m_RemoteContactList;
    SIPURI m_LocalURI;  // from URI to be used for request
    SIPURI m_RemoteURI; // to URI to be used for requests
    SIPURI m_TargetURI; /// the startline URI to use for requests
	  PString m_InterfaceAddress; /// Interface address to be used for sending requests
    WORD m_InterfacePort;
    Via m_LocalVia;
    mutable PINDEX m_LocalSequence;
    mutable PINDEX m_RemoteSequence;

    SIPURI m_NATKeepAliveTarget;
    PTimer m_NATKeepAliveTimer;
    PDECLARE_NOTIFIER(PTimer, SIPSession, OnNATKeepAlive);

    SessionKeepAliveMethod m_SessionKeepAliveMethod;
    PTimer m_SessionKeepAliveTimer;
    PTimer m_SessionKeepAliveTimeout;
    PMutex m_SessionKeepAliveMutex;
    PDECLARE_NOTIFIER(PTimer, SIPSession, OnSessionKeepAlive);
    PDECLARE_NOTIFIER(PTimer, SIPSession, OnSessionKeepAliveTimeout);

    PTimer m_SelfDestructTimer;
    PDECLARE_NOTIFIER( PTimer, SIPSession, OnSelfDestructExpire );
    
    BOOL m_EnableXORHash;
    PString m_LogTag;

    PIPSocket::Address m_InitialTargetAddress;
    WORD m_InitialTargetPort;

    PMutex m_GCMutex;
    BOOL m_IsDestroyed;
    friend class SIPSessionManager;

  public:
    static PAtomicInteger m_InstanceCounter;
    int m_InstanceId;

  };

  class SIPSessionEvent : public PObject
  {
    PCLASSINFO( SIPSessionEvent, PObject );
  public:

    SIPSessionEvent(
      SIPSession & session,
      int event,
      const SIPMessage & msg = SIPMessage()
    ) : m_Session( session ), m_Event( event )
    { 
      m_GCRef = m_Session.GCCreateRef( "SIPSessionEvent" );
      m_Message = msg;  
    }

    ~SIPSessionEvent(){}

    PINLINE SIPSession & GetSession(){ return m_Session; };
    PINLINE int GetEvent()const{ return m_Event; };
    PINLINE const SIPMessage & GetMessage()const{ return m_Message; };

  protected:

    SIPSession & m_Session;
    int m_Event;
    SIPMessage m_Message;
    SIPSession::GCRef m_GCRef;
  
  };


};

#endif
