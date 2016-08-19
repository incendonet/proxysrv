/*
 *
 * SIPSessionManager.h
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
 * $Log: SIPSessionManager.h,v $
 * Revision 1.18  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.17  2006/11/22 00:05:50  joegenbaclor
 * Bug fixes for multi-listener support
 *
 * Revision 1.16  2006/11/17 04:22:21  joegenbaclor
 * Added ability to remember core to use via m_UACoreName
 *
 * Revision 1.15  2006/09/02 04:38:44  joegenbaclor
 * Added new Process Stack Event override to allow a GCRef as an argument
 *
 * Revision 1.14  2006/08/30 03:42:04  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.13  2006/08/26 14:31:40  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.12  2006/08/09 12:17:46  joegenbaclor
 * Added RFC 3428 Support
 *
 * Revision 1.11  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.10  2006/04/08 02:38:15  joegenbaclor
 * 1.  Corrected bug in call session where  OnConnected() is called befoe OnSDPOffe.
 * 2.  Corrected bug where Cancel syncpoint may not be signalled making calls to SendCancel block
 *
 * Revision 1.9  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.8  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.7  2006/03/24 15:42:13  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.6  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPSESSIONMGR_H
#define SIPSESSIONMGR_H

#include "SIPStack.h"
#include "SIPSession.h"
#include "EventQueue.h"
#include "SIPUserAgent.h"
#include "Logger.h"

using namespace EQ;
using namespace Tools;

namespace UACORE
{
  class SIPSessionManager : public Logger, public PObject
  {
    PCLASSINFO( SIPSessionManager, PObject );
  public:

    SIPSessionManager(
      SIPUserAgent & ua,
      PINDEX sessionThreadCount = 1,
      PINDEX stackSize = 1024 * 2,
      const PString & mgrName = "Session Manager"
    );

    virtual ~SIPSessionManager();

    SIPSession * CreateMultiDirectionalSession(
      const ProfileUA & profile,
      const PString & sessionId
    );

    SIPSession * CreateClientSession(
      const ProfileUA & profile,
      const PString & sessionId
    );

    SIPSession::GCRef CreateServerSession(
      const SIPMessage & request
    );

    virtual SIPSession * OnCreateMultiDirectionalSession(
      const ProfileUA & profile,
      const PString & sessionId
    );

    virtual SIPSession * OnCreateClientSession(
      const ProfileUA & profile,
      const PString & sessionId
    );

    virtual SIPSession::GCRef OnCreateServerSession(
      const SIPMessage & request
    );

    BOOL DestroySession(
      const PString & sessionId
    );

    SIPSession::GCRef FindGCRefByCallId(
      const PString & callId
    );

    SIPSession::GCRef FindGCRefBySessionId(
      const PString & sessionId
    );

    template< class T >
    T * FindGCRefByCallId(
      const PString & callId,
      SIPSession::GCRef & autoRef
    )
    {
      autoRef = FindGCRefByCallId( callId );
      return dynamic_cast<T*>( autoRef.GetObject() );
    }


    template< class T >
    T * FindGCRefBySessionId(
      const PString & sessionId,
      SIPSession::GCRef & autoRef
    )
    {
      autoRef = FindGCRefBySessionId( sessionId );
      return dynamic_cast<T*>( autoRef.GetObject() );
    }

    PDECLARE_NOTIFIER( EventQueue, SIPSessionManager, OnSessionEvent );
    PDECLARE_NOTIFIER( EventQueue, SIPSessionManager, OnSessionManagerEvent );

    void EnqueueSessionEvent(
      SIPSession & session,
      SIPSessionEvent * event
    );

    void EnqueueSessionManagerEvent(
      PObject * event
    );

    virtual void OnSessionManagerEvent(
      PObject * event
    );

    virtual void ProcessStackEvent(
      SIPStackEvent * event
    );

    virtual void ProcessStackEvent(
      SIPSession::GCRef & gcRef,
      SIPStackEvent * event 
    );

    virtual void OnIncomingSIPMessage(
      SIPMessageArrival & messageEvent,
      SIPSession * session
    );

    virtual void OnSIPMessageSent(
      SIPTransportWrite & messageEvent,
      SIPSession * session
    );

    virtual void OnUnknownTransaction(
      SIPUnknownTransaction & event
    );

    virtual void OnOrphanedMessage(
      const SIPMessage & message
    );

    virtual void OnTimerExpire(
      SIPTimerExpire & timerEvent,
      SIPSession * session
    );

    virtual void OnTransportFailure(
      int failureType,
      SIPSession * session
    );

    BOOL Authenticate(
      const SIPMessage & request
    );

    BOOL Authorize(
      const SIPMessage & request
    );

    virtual BOOL OnRequestA1Hash(
      const PString & userName,
      const PString & realm,
      const PString & method,
      PString & hash
    );

    BOOL ConstructVia(
      const PIPSocket::Address & target,
      Via & via,
      SIPTransport::Protocol proto,
      WORD port = 0
    );

    BOOL SendRequest(
      const SIPMessage & request
    );

    SIPTransportManager * GetTransportManager();

    PINLINE SIPUserAgent & GetUserAgent(){ return m_UserAgent; };
    PINLINE const PString & GetUACoreName()const{ return m_UACoreName; };
    PINLINE void SetUACoreName( const PString & name ){ m_UACoreName = name; };
  protected:
    PMutex m_SessionsMutex;
    SIPSession::SortedCollection m_SessionsBySessionId;
    SIPSession::SortedCollection m_SessionsByCallId;
    EventQueue * m_EventQueue;
    EventQueue * m_ManagerEventQueue;
    PMutex m_ManagerEventQueueMutex;
    SIPUserAgent & m_UserAgent;
    PString m_UACoreName;
  };
};


#endif


