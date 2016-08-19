/*
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
 * $Log: B2BUACall.h,v $
 * Revision 1.15  2006/11/30 08:19:35  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.14  2006/11/10 07:53:10  rcolobong
 * 1. Change Access modifier for method SetB2BUAConnection
 * 2. Support Fork Calls Messages by giving 1xx-6xx events to B2BUAForkCalls
 *
 * Revision 1.13  2006/10/23 15:44:54  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.12  2006/10/11 04:49:47  rcolobong
 * 1. Add support for Notify During Refer
 * 2. Fixed bug where 3xx invoke CallHandler OnRejected
 *
 * Revision 1.11  2006/09/20 09:51:14  joegenbaclor
 * 1.  Disabled Media Aggregation in B2BMediaInterface
 * 2.  Added logging for RTP statitics for media proxy sessions
 *
 * Revision 1.10  2006/09/18 09:35:15  joegenbaclor
 * Completed initial implementation of RTP media aggregation
 *
 * Revision 1.9  2006/09/08 04:39:16  joegenbaclor
 * Implemented INFO Session Keep-alive
 *
 * Revision 1.8  2006/08/26 14:31:40  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.7  2006/07/19 02:19:20  joegenbaclor
 * 1.  Bug fixes concerning B2BUA destruction during during 3xx, 4xx, 5xx, 6xx local response
 * 2.  Fixed bug where Route header is never removed from outbound Invite resulting that may result to a loop when
 *      Inbound Invite has a Route header present
 * 3.  Added RemoveAllRoutes() in SIPMessage
 *
 * Revision 1.6  2006/06/28 14:14:56  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.5  2006/06/23 04:44:04  joegenbaclor
 * SendB2BRefer() added to B2BUACall
 *
 * Revision 1.4  2006/06/22 08:27:56  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.3  2006/06/21 14:29:11  joegenbaclor
 * This upload marks the first successful call by OpenB2BUA
 *
 * Revision 1.2  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
 *
 * Revision 1.1  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 *
 */
#ifndef B2BUACALL_H
#define B2BUACALL_H

#include "B2BUAEndPoint.h"
#include "B2BMediaInterface.h"
#include "ossbuildopts.h"

#if HAS_OPAL
#include <rtp/rtp.h>
#endif

namespace B2BUA
{
  class B2BUAConnection;

  class B2BUACall : public CallSession
  {
    PCLASSINFO( B2BUACall, CallSession );
  public:

    /// UAC constructor
    B2BUACall(
      B2BUAEndPoint & manager,
      const PString & sessionId,
      const ProfileUA & profile
    );

    /// UAS constructor
    B2BUACall(
      B2BUAEndPoint & sessionManager,
      const SIPMessage & request,
      const PString & sessionId
    );

    ~B2BUACall();

    void OnSessionEvent(
      int event,
      const SIPMessage & eventMsg
    );

    void AnswerB2BCall( 
      B2BUACall & secondLeg,
      const SIPMessage & secondLegResponse
    );

    BOOL SendB2BConnect( 
      const SIPMessage & secondLegResponse 
    );

    BOOL SendB2BProgress( 
      const SIPMessage & secondLegResponse 
    );

    BOOL SendB2BRedirect( 
      const SIPMessage & secondLegResponse 
    );

    BOOL SendB2BRefer(
      B2BUACall & referror,
      const SIPMessage & refer 
    );

    BOOL SendB2BNotify(
      B2BUACall & call,
      const SIPMessage & notify
    );

    BOOL SendB2BReject(
      const SIPMessage & secondLegResponse 
    );

    virtual void OnSetupOutbound(
      const SIPMessage & invite,
      B2BUAConnection & connection
    );


    /// returning FALSE here will reject the call with no acceptable format
    /// depending on call state.  Offers received in ACK will be disconnected
    /// using a BYE.  
    virtual BOOL OnIncomingSDPOffer(
      const SIPMessage & sdp
    );

    /// your last chance to produce an answer to the offer.  
    /// This will be called either before sending 183, 200 or ACK 
    virtual BOOL OnRequireSDPAnswer(
      const SIPMessage & offer,
      SIPMessage & answer
    );

    /// this will be called if an offer is not received in INVITE or when we are initiating the call
    virtual BOOL OnRequireSDPOffer(
      SIPMessage & offer
    );

    virtual void OnSessionKeepAliveTimeout();

    /** IST CallBacks */
    virtual void IST_OnReceivedInvite(
      const SIPMessage & msg
    );

    virtual void IST_OnReceivedMoreInvite(
      const SIPMessage & msg
    );

    virtual void IST_OnReceivedAck(
      const SIPMessage & msg
    );

    virtual void IST_OnReceivedUnknown(
      const SIPMessage & msg
    );

    /** NIST Callbacks */
    virtual void NIST_OnReceivedRegister(
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedBye(
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedOptions(
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedInfo(
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedMessage(
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedCancel(
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedNotify(
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedSubscribe(
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedRefer(
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedUnknown(
      const SIPMessage & msg
    );


    
  /** NICT CALLBACKS */
    
    virtual void NICT_OnReceived1xx(
      const SIPMessage & msg
    );

    virtual void NICT_OnReceived2xx(
      const SIPMessage & msg
    );

    virtual void NICT_OnReceived3xx(
      const SIPMessage & msg
    );

    virtual void NICT_OnReceived4xx(
      const SIPMessage & msg
    );

    virtual void NICT_OnReceived5xx(
      const SIPMessage & msg
    );

    virtual void NICT_OnReceived6xx(
      const SIPMessage & msg
    );

    virtual void NICT_OnReceivedUnknown(
      const SIPMessage & msg
    );

  /** ICT CALLBACKS */
    
    virtual void ICT_OnReceived1xx(
      const SIPMessage & msg
    );

    virtual void ICT_OnReceived2xx(
      const SIPMessage & msg
    );

    virtual void ICT_OnReceived3xx(
      const SIPMessage & msg
    );

    virtual void ICT_OnReceived4xx(
      const SIPMessage & msg
    );

    virtual void ICT_OnReceived5xx(
      const SIPMessage & msg
    );

    virtual void ICT_OnReceived6xx(
      const SIPMessage & msg
    );

    virtual void ICT_OnReceivedUnknown(
      const SIPMessage & msg
    );

    virtual void OnSessionTimerExpire();

    virtual void OnDestroySession();

    void SetB2BUAConnection( B2BUAConnection * conn );
    PINLINE B2BUAConnection & GetB2BUAConnection(){ return *m_Connection; };

    PINLINE const ProxyAuthenticate & GetProxyAuthenticate()const{ return m_ProxyAuthenticate; };
    PINLINE void SetProxyAuthenticate( const ProxyAuthenticate & auth ){ m_ProxyAuthenticate = auth; };
    PINLINE PINDEX GetLegIndex()const{ return m_LegIndex; };
    PINLINE void SetLegIndex( PINDEX legIndex ){ m_LegIndex = legIndex; };
#if HAS_OPAL
    PINLINE RTP_SessionManager & GetRTPSessionManager(){ return m_RTPSessionManager; };
    PINLINE void AttachAudioRTPSessionThread( PThread * t ){ m_AudioRTPSessionThread = t; };
    PINLINE void AttachVideoRTPSessionThread( PThread * t ){ m_VideoRTPSessionThread = t; };
    PINLINE void AttachAudioRTPAggregator( B2BMediaInterface::AggregatedMedia * t ){ m_AudioAggregator = t; };
    PINLINE void AttachVideoRTPAggregator( B2BMediaInterface::AggregatedMedia * t ){ m_VideoAggregator = t; };
#endif

  protected:

    B2BUAConnection * m_Connection;
    SIPSession::GCRef * m_ConnectionRef;
    ProxyAuthenticate m_ProxyAuthenticate;
    PINDEX m_LegIndex;

    friend class B2BUAConnection;
#if HAS_OPAL
    RTP_SessionManager m_RTPSessionManager;
    PThread * m_AudioRTPSessionThread;
    PThread * m_VideoRTPSessionThread;

    B2BMediaInterface::AggregatedMedia * m_AudioAggregator;
    B2BMediaInterface::AggregatedMedia * m_VideoAggregator;

    void CloseMediaStreams();
    PMutex m_MediaStreamsMutex;
    BOOL m_HasClosedMediaStreams;
#endif
  };
}

#endif


