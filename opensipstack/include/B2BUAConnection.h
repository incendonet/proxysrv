/*
 *
 * B2BUAConnection.h
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
 * $Log: B2BUAConnection.h,v $
 * Revision 1.33  2007/01/10 08:07:15  rcolobong
 * 1. Add new method for OnSendB2BConnect Call Interface.
 * 2. OnSendB2BConnect is supported in B2BUserAgent, B2BUACall and B2BUAConnection
 *
 * Revision 1.32  2006/12/19 10:14:26  rcolobong
 * 1. Add method to insert Warning header in 200 Ok response to inbound
 * 2. Alerting timeout will now do failover
 *
 * Revision 1.31  2006/12/17 13:30:52  joegenbaclor
 * Added call attempt index in call-id string to avoid call-id collision during failovers
 *
 * Revision 1.30  2006/11/30 08:19:35  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.29  2006/11/10 07:55:35  rcolobong
 * 1. Support Fork Invites using B2BUAForkCalls
 * 2. Handle Cancel events during Forking
 *
 * Revision 1.28  2006/10/30 08:35:55  joegenbaclor
 * Removed more references to presence namespace
 *
 * Revision 1.27  2006/10/23 15:44:54  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.26  2006/10/11 04:49:47  rcolobong
 * 1. Add support for Notify During Refer
 * 2. Fixed bug where 3xx invoke CallHandler OnRejected
 *
 * Revision 1.25  2006/09/25 04:43:29  rcolobong
 * 1. Fix bug in 415 Unsupported Media where CallSession::m_ICTState is not properly initialized during construction.
 * 2. Remove the temporary fix for the 415 Unsupported Media where we use Semaphore to wait for second leg SDP in OnRequireSDPAnswer
 *
 * Revision 1.24  2006/09/22 08:52:45  joegenbaclor
 * Temporary fix for OnRequireSDPAnswer to use a semaphore to wait for second leg SDP
 *
 * Revision 1.23  2006/09/20 11:05:25  joegenbaclor
 * Flagged sending of Session Keep Alive.  Defaults to FALSE
 *
 * Revision 1.22  2006/09/18 09:35:15  joegenbaclor
 * Completed initial implementation of RTP media aggregation
 *
 * Revision 1.21  2006/09/18 03:28:54  joegenbaclor
 * 1. More work on media aggregation
 * 2. Check sock->GetPort() for NULL in SIPTransportManager::GetListenerAddress() (thanks to
 *  Vamsi Pottangi for reporting the bug)
 * 3.  Added reference size check to GCObject::ReleaseRef()
 *
 * Revision 1.20  2006/09/15 12:29:44  joegenbaclor
 * Flagged OnDestroySession of B2BConnection so it wont be called twice from different
 *  threads.
 *
 * Revision 1.19  2006/09/08 04:39:16  joegenbaclor
 * Implemented INFO Session Keep-alive
 *
 * Revision 1.18  2006/09/01 13:08:32  rcolobong
 * Determining if we would enable Media Proxy is now set in B2BUAConnection
 *
 * Revision 1.17  2006/08/30 08:42:10  rcolobong
 * 1. Update handling for connection and alerting timeout
 * 2. Fix bug where leg1 is destroyed before calling DestroyConnection
 *
 * Revision 1.16  2006/08/29 10:46:05  rcolobong
 * *** empty log message ***
 *
 * Revision 1.15  2006/08/26 14:31:40  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.14  2006/08/04 05:36:49  rcolobong
 * Add Alerting and Connecting Timer
 *
 * Revision 1.13  2006/07/19 02:19:20  joegenbaclor
 * 1.  Bug fixes concerning B2BUA destruction during during 3xx, 4xx, 5xx, 6xx local response
 * 2.  Fixed bug where Route header is never removed from outbound Invite resulting that may result to a loop when
 *      Inbound Invite has a Route header present
 * 3.  Added RemoveAllRoutes() in SIPMessage
 *
 * Revision 1.12  2006/07/13 06:53:04  joegenbaclor
 * 1.  Introduced Sanity checks for incoming SIP Message
 * 2.  Corrected bug in SIPURI where enum scheme is not getting recognized.
 * 3.  Added ENUM support to routing
 * 4.  Introduced global routing flag to indicate when and not to rewrite outbound To URI's
 *
 * Revision 1.11  2006/06/29 05:09:13  joegenbaclor
 * Changed OnOutgoingCall invite parameter from const to none const to give a
 * chance for applications to modify the outbound invite before being sent to the transport
 *
 * Revision 1.10  2006/06/28 14:14:56  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.9  2006/06/23 04:44:04  joegenbaclor
 * SendB2BRefer() added to B2BUACall
 *
 * Revision 1.8  2006/06/22 08:27:56  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.7  2006/06/21 14:29:11  joegenbaclor
 * This upload marks the first successful call by OpenB2BUA
 *
 * Revision 1.6  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
 *
 * Revision 1.5  2006/06/15 09:47:27  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.4  2006/06/14 08:43:38  joegenbaclor
 * Initial upload of OpenB2BUA applcation and related classes
 *
 * Revision 1.3  2006/04/11 00:18:54  joegenbaclor
 * More work on B2BUA class structures
 *
 * Revision 1.2  2006/04/10 01:09:03  joegenbaclor
 * Implemented Create methods fo B2BUA sessions
 *
 * Revision 1.1  2006/04/08 06:27:13  joegenbaclor
 * Initial upload of B2BUA classes
 *
 *
 */

#ifndef B2BUACONNECTION_H
#define B2BUACONNECTION_H

#include "B2BUACall.h"
#include "B2BUA.h"


namespace B2BUA
{
  class B2BUAForkCalls;

  class B2BUAConnection : public SIPSession
  {
    PCLASSINFO( B2BUAConnection, SIPSession );

  public:
    enum SessionEvents
    {
      SetupOutbound,
      DestroySession,
      ErrorNoRoute,
      ErrorRouteResolution
    };

    enum SessionState
    {
      Idle = 0,
      Trying,
      Proceeding,
      Connected,
      Disconnected,
      Terminated
    };

    B2BUAConnection(
      B2BUAEndPoint & ep,
      const PString & sessionId
    );

    virtual ~B2BUAConnection();

    virtual void OnDestroySession();

    virtual BOOL OnIncomingCall(
      B2BUACall & call,
      const SIPMessage & invite
    );

    virtual void OnRequireRedirectionId(
      B2BUACall & call,
      PString & redirectionId
    );

    virtual void OnRequireCallTransferId(
      B2BUACall & call,
      PString & redirectionId
    );

    virtual BOOL OnIncomingSDPOffer(
      B2BUACall & call,
      const SIPMessage & sdp
    );

    /// your last chance to produce an answer to the offer.  
    /// This will be called either before sending 183, 200 or ACK 
    virtual BOOL OnRequireSDPAnswer(
      B2BUACall & call,
      const SIPMessage & offer,
      SIPMessage & answer
    );

    /// this will be called if an offer is not received in INVITE or when we are initiating the call
    virtual BOOL OnRequireSDPOffer(
      B2BUACall & call,
      SIPMessage & offer
    );

    virtual BOOL OnSendB2BConnect(
      B2BUACall & call,
      const SIPMessage & message
    );

    BOOL AttachCall(
      const PString & id,
      B2BUACall * call 
    );

    SIPSession::GCRef * DetachCall(
      const PString & id
    );

    SIPSession::GCRef * FindCall(
      const PString & id
    );

    virtual void OnSessionEvent(
      int event,
      const SIPMessage & eventMsg
    );

    virtual void OnSetupOutbound(
      const SIPMessage & inboundInvite
    );

    void SetSessionState(
      B2BUACall & call,
      const SIPMessage & msg
    );

    /*
    void SetSessionState(
      B2BUACall & call,
      int event
    );
    */

    void OnSessionKeepAliveTimeout(
      B2BUACall & call 
    );

    void OnHandlePreConnectState( 
      B2BUACall & call,
      const SIPMessage & msg
    );

    void OnHandleStateConnected( 
      B2BUACall & call,
      const SIPMessage & msg
    );

    void OnHandleStateDisconnected( 
      B2BUACall & call,
      const SIPMessage & msg
    );

    void OnHandleStateTerminated( 
      B2BUACall & call,
      const SIPMessage & msg
    );

    void DestroyConnection( 
      const SIPMessage & msg /// SIP Message that caused the session to be destroyed
    );

    void DestroyConnection();

    PINLINE void Post200OkWarningMessage( const Warning& warning ) { m_200OkWarning = warning; }

    PINLINE void EnableForkCalls( BOOL enable = TRUE ) { m_EnableForkCalls = enable; }
    PINLINE BOOL IsForkCalls() const { return m_EnableForkCalls; }
    PINLINE B2BUAForkCalls * GetForkCalls() { return m_ForkCalls; }

    PINLINE void AddRoute( const SIPURI & route ){ m_Routes.Append( new SIPURI( route ) ); };
    PINLINE B2BRoutingInterface::B2BRouteList & GetRoutes(){ return m_Routes; };
    PINLINE const PString & GetLeg1SDP()const{ return m_Leg1SDP; };
    PINLINE void SetLeg1SDP( const PString & sdp ){ m_Leg1SDP = sdp; };
    PINLINE const PString & GetLeg2SDP()const{ return m_Leg2SDP; };
    PINLINE void SetLeg2SDP( const PString & sdp ){ m_Leg2SDP = sdp; };
    PINLINE B2BUACall * GetLeg1Call(){ return m_Leg1Call; };
    PINLINE B2BUACall * GetLeg2Call(){ return m_Leg2Call; };
    PINLINE void SetMediaProxyIfPrivate( BOOL media = TRUE ) { m_MediaProxyIfPrivate = media; }
    PINLINE BOOL GetMediaProxyIfPrivate() const { return m_MediaProxyIfPrivate; }
    static void SetConnectionTimeout( DWORD timeout ){ m_ConnectionTimeout = timeout; }
    static void SetAlertingTimeout( DWORD timeout ){ m_AlertingTimeout = timeout; }
    static long GetSessionCount(){ return m_ConnectionCounter; }

  protected:
    PMutex m_CallSessionsMutex;
    PDICTIONARY( CallSessionList, PCaselessString, SIPSession::GCRef );
    CallSessionList m_CallSessions;
    B2BUACall * m_Leg1Call;
    B2BUACall * m_Leg2Call;
    B2BUAForkCalls * m_ForkCalls;

    static DWORD m_ConnectionTimeout;
    static DWORD m_AlertingTimeout;
    BOOL m_MediaProxyIfPrivate;
    BOOL m_EnableForkCalls;

    B2BRoutingInterface::B2BRouteList m_Routes;

    PString m_Leg1SDP;
    PString m_Leg2SDP;

    PMutex m_SessionStateMutex;
    SessionState m_SessionState;

    Warning m_200OkWarning;

    static PAtomicInteger m_ConnectionCounter;

    PTimer m_ConnectionTimer;
    PDECLARE_NOTIFIER( PTimer, B2BUAConnection, OnConnectionTimeout );

    PTimer m_AlertingTimer;
    PDECLARE_NOTIFIER( PTimer, B2BUAConnection, OnAlertingTimeout );

  protected:
    void StartConnectionTimer();
    void StopConnectionTimer();
    void StartAlertingTimer();
    void StopAlertingTimer();


    /** IST Callbacks */
    void IST_OnReceivedMoreInvite(
      B2BUACall & call,
      const SIPMessage & msg
    );

    /** NIST Callbacks */
    virtual void NIST_OnReceivedRegister(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedRefer(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedBye(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedOptions(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedInfo(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedMessage(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedCancel(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedNotify(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedSubscribe(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedUnknown(
      B2BUACall & call,
      const SIPMessage & msg
    );



  /** NICT CALLBACKS */
    virtual void NICT_OnReceived1xx(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NICT_OnReceived2xx(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NICT_OnReceived3xx(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NICT_OnReceived4xx(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NICT_OnReceived5xx(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NICT_OnReceived6xx(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void NICT_OnReceivedUnknown(
      B2BUACall & call,
      const SIPMessage & msg
    );

  /** ICT CALLBACKS */
    virtual void ICT_OnReceived1xx(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void ICT_OnReceived2xx(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void ICT_OnReceived3xx(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void ICT_OnReceived4xx(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void ICT_OnReceived5xx(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void ICT_OnReceived6xx(
      B2BUACall & call,
      const SIPMessage & msg
    );

    virtual void ICT_OnReceivedUnknown(
      B2BUACall & call,
      const SIPMessage & msg
    );

    friend class B2BUACall;

#if HAS_OPAL
  public:
    PINLINE BOOL HasStartedMediaStream()const{ return m_HasStartedMediaStream; };
    PINLINE void StartMediaStream(){ m_HasStartedMediaStream = TRUE; }
    BOOL m_HasStartedMediaStream;
#endif

    BOOL m_HasReceivedSDP;
    int m_OutboundAttempts;

    public:
      PINLINE void SetRewriteToURI( BOOL rewrite = TRUE ){ m_RewriteToURI = rewrite; };
      PINLINE BOOL WillRewriteToURI()const{ return m_RewriteToURI; };
      PINLINE void SetSendKeepAlive( BOOL send = TRUE ) { m_SendSessionKeepAlive = send; };
      PINLINE BOOL WillSendKeepAlive()const{ return m_SendSessionKeepAlive; };
    protected:
      BOOL m_RewriteToURI;
      BOOL m_ConnectionDestroyed;
      BOOL m_SendSessionKeepAlive;
    
  };
}
    
#endif

