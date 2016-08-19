/*
 *
 * SIPUserAgent.h
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
 * $Log: SIPUserAgent.h,v $
 * Revision 1.12  2007/01/22 10:01:57  joegenbaclor
 * Fixed bug in handling Notifies
 *
 * Revision 1.11  2006/12/22 07:37:19  joegenbaclor
 * Introduced separate funtion call  for starting of transport thread.  This is necessary
 *  to make sure that the backdoor listener is already initialized prior to calling transport
 *  reads
 *
 * Revision 1.10  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.9  2006/10/30 02:42:26  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.8  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.7  2006/03/25 18:40:13  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.6  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.5  2006/03/14 03:13:22  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef USERAGENT_H
#define USERAGENT_H

#include "SIPStack.h"
#include "ProfileUA.h"
#include "ossbuildopts.h"

namespace SIP
{
  using namespace Profiles;

  class SIPUserAgent : public PObject, public Logger
  {
    PCLASSINFO( SIPUserAgent, PObject );
  public:

    class Worker : public PThread
    {
      PCLASSINFO( Worker, PThread );
    public:

      Worker( SIPUserAgent & userAgent );
      ~Worker();

      virtual void Main();

      SIPUserAgent &m_UserAgent;
    };
/*
    class StaleObjectCollector : public PThread
    {
      PCLASSINFO( StaleObjectCollector, PThread );
    public:
      
      StaleObjectCollector();
      
      ~StaleObjectCollector();

      virtual void Main();

      void Enqueue(
        SIPStackEvent * event
      );
      
    protected:
      PMutex m_QueueMutex;
      PSyncPoint m_SyncPoint;
      BOOL m_ExitFlag;
      SIPStackEvent::Queue m_Queue;
      SIPStackEvent::Queue m_BackLog;
    };
*/
    SIPUserAgent();

    SIPUserAgent(
      ProfileUA & profile
    );

    virtual ~SIPUserAgent();

    virtual BOOL Initialize(
      PINDEX threadCount = FSM_THREAD_COUNT
    );

    virtual BOOL StartTransportThreads();

    virtual void Terminate();
    

    virtual void ProcessEvent(
      SIPStackEvent * event
    ) = 0;

    static void AppendCommonHeaders(
      SIPMessage & msg,
      const ProfileUA & profile
    );


    /// Finds the transaction for the message.  If its not found, a transaction
    /// will be created.
    BOOL SendRequest(
      const SIPMessage & request,
      TransactionId & transactionId
    );

    /// Sends the message directly to the transport queue without looking for 
    /// a transaction.  this would normally be used for retransmitting messages
    /// like IST 200 Ok.
    void SendRequest(
      const SIPMessage & request
    );

    PINLINE ProfileUA & GetDefaultProfile(){ return m_DefaultProfile; };

    PINLINE SIPStack & GetStack(){ return m_SIPStack; };
    
  protected:

    

    PINLINE SIPStackEvent * ReadEvent(){ return m_SIPStack.ReadEvent(); };

    /// transport
    PINLINE BOOL AddTransport(
      SIPTransport::Protocol protocol,
      const PString & iface = PString::Empty(),
      WORD port = 5060
    ){ return m_SIPStack.AddTransport( protocol, iface, port ); };

    SIPStack m_SIPStack;

    ProfileUA m_DefaultProfile;

    /// Worker Threads
    PSemaphore m_EventQueueSync;
    PMutex m_EventQueueAccessMutex;
    SIPTransaction::Queue m_EventQueue;
    PLIST( Workers, Worker );
    Workers m_ThreadPool;
    //StaleObjectCollector m_StaleObjectCollector;

    friend class Worker;
  };
};
#endif


