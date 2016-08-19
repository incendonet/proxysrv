/*
 *
 * CallSession.h
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
 * $Log: CallSession.h,v $
 * Revision 1.50  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.49  2006/11/07 07:50:32  joegenbaclor
 * Corrected bug in Session timer processing.
 *
 * Revision 1.48  2006/10/23 15:44:54  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.47  2006/10/11 04:49:47  rcolobong
 * 1. Add support for Notify During Refer
 * 2. Fixed bug where 3xx invoke CallHandler OnRejected
 *
 * Revision 1.46  2006/10/03 04:28:43  joegenbaclor
 * Added INFO DTMF relay support
 *
 * Revision 1.45  2006/09/13 10:36:25  joegenbaclor
 * Added Release-Minimal build for smaller DLL footprint for ATLSIP
 *
 * Revision 1.44  2006/09/07 04:25:25  joegenbaclor
 * Added callback for 200 Ok retransmission timeout
 *
 * Revision 1.43  2006/08/30 08:43:31  rcolobong
 * Create Set and Get method for Current UAS and UAC invite
 *
 * Revision 1.42  2006/08/30 07:48:54  joegenbaclor
 * Implemented OntimerExpire
 *
 * Revision 1.41  2006/08/26 14:31:40  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.40  2006/08/14 12:22:11  joegenbaclor
 * Added m_CanSendCancel flag to determine if provisional response has been received prior to sending CANCEL
 *
 * Revision 1.39  2006/08/09 12:17:46  joegenbaclor
 * Added RFC 3428 Support
 *
 * Revision 1.38  2006/08/06 02:32:07  joegenbaclor
 * Corrected bug regarding blank call-ids for UnknownTransaction events
 *
 * Revision 1.37  2006/06/28 14:14:56  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.36  2006/06/21 14:29:11  joegenbaclor
 * This upload marks the first successful call by OpenB2BUA
 *
 * Revision 1.35  2006/06/15 09:47:27  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.34  2006/04/29 03:22:54  joegenbaclor
 * 1.  More Refer support code added to call session
 * 2.  Added StoreRegistrationInfo callback to Registrar classes
 *
 * Revision 1.33  2006/04/26 01:00:10  joegenbaclor
 * Added REFER NIST callbacks
 *
 * Revision 1.32  2006/04/25 03:41:10  joegenbaclor
 * 1.  Added REFER UAC subscription callbacks.
 * 2.  Renamed Event class in StatefulEventQueue to avoid collision with Event SIP Header
 *
 * Revision 1.31  2006/04/25 02:14:05  joegenbaclor
 * 1.  Added SendRefer method.
 * 2.  Added StateCallTransfer to SessionState enum
 *
 * Revision 1.30  2006/04/20 14:32:28  joegenbaclor
 * Fixed early media bug where soucd channels in OPAL ay not be opened in time for ringback to start streaming.
 *
 * Revision 1.29  2006/04/07 07:47:19  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.28  2006/04/05 00:26:01  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.27  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.26  2006/03/17 13:50:54  joegenbaclor
 * 1.  Implemented UAC Call Redirection
 * 2.  Implemented SendCancel() method in CallSession
 * 3.  Introduced ClearInternalHeaders() function in SIP Message to erase internal headers inheritted from previous transaction.
 *
 * Revision 1.25  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.24  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.23  2006/03/14 12:55:17  joegenbaclor
 * Removed Separate SIPClassese for simplicity.
 * Instead we will implement dialog support directly in SIPSession.
 *
 * Revision 1.22  2006/03/14 12:34:50  joegenbaclor
 * Implemented UAC OnAlerting(), OnConnect() and OnDisconnect() methods
 *
 * Revision 1.21  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef CALLSESSION_H
#define CALLSESSION_H

#include "SIPSession.h"
#include "SIPStack.h"
#include "ProfileUA.h"

using namespace SIP;
using namespace Profiles;


namespace UACORE
{
  class CallSessionManager;

  class CallSession : public SIPSession 
  {
    PCLASSINFO( CallSession, SIPSession );
  public:

   
    enum SessionEvents
    {
      /// Invite Client Transaction Events
      ICT_Recv1xx,
      ICT_Recv2xx,
      ICT_Recv3xx,
      ICT_Recv4xx,
      ICT_Recv5xx,
      ICT_Recv6xx,
      ICT_RecvUnknown,
      ICT_InviteSent,
      ICT_AckSent,
      ICT_UnknownSent,
      /// None Invite Client Transaction Events
      NICT_Recv1xx,
      NICT_Recv2xx,
      NICT_Recv3xx,
      NICT_Recv4xx,
      NICT_Recv5xx,
      NICT_Recv6xx,
      NICT_RecvUnknown,
      NICT_ByeSent,
      NICT_OptionsSent,
      NICT_InfoSent,
      NICT_CancelSent,
      NICT_NotifySent,
      NICT_SubscribeSent,
      NICT_UnknownSent,
      /// Invite Server Transaction Events
      IST_RecvInvite,
      IST_RecvMoreInvite,
      IST_RecvAck,
      IST_RecvUnknown,
      IST_1xxSent,
      IST_2xxSent,
      IST_3xxSent,
      IST_4xxSent,
      IST_5xxSent,
      IST_6xxSent,
      IST_UnknownSent,
      /// None Invite Server Transaction Events
      NIST_RecvBye,
      NIST_RecvOptions,
      NIST_RecvInfo,
      NIST_RecvCancel,
      NIST_RecvNotify,
      NIST_RecvSubscribe,
      NIST_RecvRefer,
      NIST_RecvMessage,
      NIST_RecvUpdate,
      NIST_RecvUnknown,
      NIST_1xxSent,
      NIST_2xxSent,
      NIST_3xxSent,
      NIST_4xxSent,
      NIST_5xxSent,
      NIST_6xxSent,
      NIST_UnknownSent,
      /// answer call events
      IST_AnswerCallNow,                // Sends 200 OK back with SDP after receipt of INVITE
      IST_AnswerCallDeferred,           // Answers with 180 ringing
      IST_AnswerCallDeferredWithMedia,  // Like AnswerCallDefered Only media is sent in 183 progress
      IST_AnswerCallRedirect,           // Redirect the call to another address
      IST_AnswerCallRedirectToProxy,    // Redirect the call to a proxy
      IST_AnswerCallQueued,             // Tell remote that the call is queued
      IST_AnswerCallDenied,             // Reject the call
      IST_200OkRetransmitTimeout        // Retransmision of 200 Ok  has expired
    };

    enum SessionState
    {
      StateIdle,
      StateNewCall,
      StateProceeding,
      StateAlerting,
      StateQueued,
      StateMaxAnswerCall,
      StateRedirecting,
      StateCancelling,
      StateConnected,
      StateCallTransfer,
      StateDisconnected,
      StateFinal
    };

    enum AnswerCallResponse
    {
      AnswerCallNow,                // Sends 200 OK back with SDP after receipt of INVITE
      AnswerCallDeferred,           // Answers with 180 ringing
      AnswerCallDeferredWithMedia,  // Like AnswerCallDefered Only media is sent in 183 progress
      AnswerCallRedirect,           // Redirect the call to another address
      AnswerCallRedirectToProxy,    // Redirect the call to a proxy
      AnswerCallQueued,             // Tell remote that the call is queued
      AnswerCallDenied,             // Reject the call
      NumAnswerCall
    };

    enum AnswerCallDeniedResponse
    {
      DontDeny = 0,
      /** Client Error Response */
      DisconnectWithBadRequest               = 400,
      DisconnectWithUnauthorized             = 401,
      DisconnectWithPaymentRequired          = 402,
      DisconnectWithForbidden                = 403,
      DisconnectWithNotFound                 = 404,
      DisconnectWithMethodNotAllowed         = 405,
      DisconnectWithNotAcceptable            = 406,
      DisconnectWithAuthenticationRequired   = 407,
      DisconnectWithRequestTimeout           = 408,
      DisconnectWithConflict                 = 409,
      DisconnectWithGone                     = 410,
      DisconnectWithLengthRequired           = 411,
      DisconnectWithTooLargeRequestEntity    = 413,
      DisconnectWithURITooLong               = 414,
      DisconnectWithUnsupportedMediaType     = 415,
      DisconnectWithBadExtension             = 420,
      DisconnectWithExtensionRequired        = 421,
      DisconnectWithIntervalTooBrief         = 423,
      DisconnectWithTemporarilyUnavailable   = 480,
      DisconnectWithTransactionDoesNotExist  = 481,
      DisconnectWithLoopDetected             = 482,
      DisconnectWithTooManyHops              = 483,
      DisconnectWithAddressIncomplete        = 484,
      DisconnectWithURIAmbiguos              = 485,
      DisconnectWithBusyHere                 = 486,
      DisconnectWithRequestCancelled         = 487,
      DisconnectWithNotAcceptableHere        = 488,
      /** Server Error Response */
      DisconnectWithServerInternalError      = 500,
      DisconnectWithNotImplemented           = 501,
      DisconnectWithBadGateway               = 502,
      DisconnectWithServiceNotAvailable      = 503,
      DisconnectWithGatewayTimeout           = 504,
      DisconnectWithVersionNotSupported      = 505,
      /** Global Error Response */
      DisconnectWithBusyEverywhere           = 600,
      DisconnectWithDeclineWithUnknownReason = 603,
      DisconnectWithDoesNotExistAnywhere     = 604,
      DisconnectWithGloballyNotAcceptable    = 606
    };

    enum ICT_State
    {
      ICT_StateIdle,
      ICT_StateTrying,
      ICT_StateAlerting,
      ICT_StateConfirmed
    };

    CallSession(
      CallSessionManager & sessionManager,
      const SIPMessage & request,
      const PString & sessionId
    );

    CallSession(
      CallSessionManager & manager,
      const PString & sessionId,
      const ProfileUA & profile
    );

    virtual ~CallSession();

    BOOL MakeCall(
      const SIPURI & uri
    );

    BOOL MakeCallTransfer(
      CallSession & callToTransfer,
      const SIPMessage & refer
    );

//// HIGH LEVEL UAS CALLBACKS

    /// returning FALSE here will automatically reject the incoming call
    virtual BOOL OnIncomingConnection(
      const SIPMessage & invite
    );

    virtual BOOL OnProcessSessionTimers(
      const SIPMessage & invite,
      SIPMessage & ok
    );

    virtual BOOL OnSetupIncomingCall( 
      const SIPMessage & invite 
    );

    virtual BOOL OnReinvite(
      const SIPMessage & invite
    );

    virtual BOOL OnProxyAuthentication(
      const SIPMessage & auth
    );

    virtual BOOL OnWWWAuthentication(
      const SIPMessage & auth
    );

    virtual BOOL OnCallRedirect(
      const SIPMessage & redirect
    );

    virtual BOOL OnHandleCallRedirect();

    /// will be called when a trasfered call
    /// receives provisional and final responses
    /// from the transfer target.  The default behavior of this
    /// method is to send NOTIFY to the transfer requestor
    virtual void OnCallTransferResponse(
      CallSession & callTransfer,
      const SIPMessage & response
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


    /// this is where implementors may modify or add headers before underlying implementation
    /// sends the actual response to INVITE to the FSM
    virtual BOOL OnSendingResponseToInvite(
      SIPMessage & response
    );

    /// this is where implementors may modify or add headers before underlying implementation
    /// sends the actual response messages to the FSM for none-invite transactions
    virtual BOOL OnSendingResponseToNoneInvite(
      SIPMessage & response
    );


    /// answers the call.  based on the mode.  May be called several times
    BOOL AnswerCall( 
      AnswerCallResponse mode
    );

    BOOL SendBye();

    BOOL SendCancel();

    BOOL SendRefer(
      const ReferTo & referTo
    );

    BOOL SendNotify(
      const SubscriptionState & state,
      const ContentType & contentType,
      const PString & body
    );

    BOOL SendReinvite();

    BOOL SendINFODTMF(
      const PString & digit,
      int duration
    );

    void OnReceivedINFODTMF(
      const SIPMessage & info
    );

    virtual void OnReceivedINFODTMF(
      const PString & digit,
      int duration
    );


  protected:
    BOOL InternalAnswerCall( 
      AnswerCallResponse mode,
      const SIPMessage & invite
    );

    /// sends and retransmits 200 Ok for an INVITE with SDP
    /// the actual response will be generated within the function
    BOOL SendConnect( const SIPMessage & request );

    /// Sends 182 Call Queued
    BOOL SendCallQueued( const SIPMessage & request );

    /// Sends 180 Ringing
    BOOL SendAlerting( const SIPMessage & request );

    /// sends 183 Progress with SDP
    BOOL SendProgress( const SIPMessage & request );

    /// Sends a redirect to proxy
    BOOL SendProxyRedirect( const SIPMessage & request );

    /// Sends a redirect to another UAS
    BOOL SendUARedirect( const SIPMessage & request );

    /// Sends a forbidden by default.
    /// or GetAnswerCallresponse() if explicitly set
    BOOL SendReject(
      const SIPMessage & request,
      PINDEX statusCode = SIPMessage::Code403_Forbidden,
      const PString & reasonPhrase = PString::Empty(),
      const PString & warning = PString::Empty()
    );

    BOOL SendAck(
      const SIPMessage & ack,
      const PString & sdp = PString::Empty()
    );


  public:
    /// will be called if a rediration URI is needed by OnAnswerCall
    virtual BOOL OnRequireRedirectionURI(
      ContactList & contacts,
      BOOL proxyRedirect = FALSE
    );

    /// will be called when the call is established
    virtual void OnConnected();

    /// will be called when the REFER is responded to by a 202 or 200
    virtual void OnReferAccepted();

    /// will be called if the refer is rejected by the UAS using 3xx-6xx response
    virtual void OnReferRejected(
      const SIPMessage & reject
    );

    virtual void OnReferSubscriptionState(
      const Event & event,
      const SubscriptionState & state,
      const PString sipFrag
    );
    
    /// will be called when the call is disconnected
    virtual void OnDisconnected();

    void OnTimerExpire(
      SIPTimerExpire & /*timerEvent*/
    );

    virtual void OnAckFor200OkExpire();

//// LOW LEVEL CALLBACKS

    virtual void OnIncomingSIPMessage(
      SIPMessageArrival & messageEvent
    );

    virtual void OnSIPMessageSent(
      SIPTransportWrite & messageEvent
    );

    virtual void OnSessionEvent(
      int event,
      const SIPMessage & eventMsg
    );

    virtual void OnSessionTimerExpire();
    

///////////////////////

  protected:
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

    virtual void IST_On1xxSent(
      const SIPMessage & msg
    );

    virtual void IST_On2xxSent(
      const SIPMessage & msg
    );

    virtual void IST_On3xxSent(
      const SIPMessage & msg
    );

    virtual void IST_On4xxSent(
      const SIPMessage & msg
    );

    virtual void IST_On5xxSent(
      const SIPMessage & msg
    );

    virtual void IST_On6xxSent(
      const SIPMessage & msg
    );

    virtual void IST_OnUnknownSent(
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

    virtual void NIST_OnReceivedMessage(
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedUpdate(
      const SIPMessage & msg
    );

    virtual void NIST_OnReceivedUnknown(
      const SIPMessage & msg
    );


    virtual void NIST_On1xxSent(
      const SIPMessage & msg
    );

    virtual void NIST_On2xxSent(
      const SIPMessage & msg
    );

    virtual void NIST_On3xxSent(
      const SIPMessage & msg
    );

    virtual void NIST_On4xxSent(
      const SIPMessage & msg
    );

    virtual void NIST_On5xxSent(
      const SIPMessage & msg
    );

    virtual void NIST_On6xxSent(
      const SIPMessage & msg
    );

    virtual void NIST_OnUnknownSent(
      const SIPMessage & msg
    );


  /** NICT CALLBACKS */
    virtual void NICT_OnRegisterSent(
      const SIPMessage & msg
    );

    virtual void NICT_OnByeSent(
      const SIPMessage & msg
    );

    virtual void NICT_OnOptionsSent(
      const SIPMessage & msg
    );

    virtual void NICT_OnInfoSent(
      const SIPMessage & msg
    );

    virtual void NICT_OnCancelSent(
      const SIPMessage & msg
    );

    virtual void NICT_OnNotifySent(
      const SIPMessage & msg
    );

    virtual void NICT_OnSubscribeSent(
      const SIPMessage & msg
    );

    virtual void NICT_OnUnknownSent(
      const SIPMessage & msg
    );

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
    virtual void ICT_OnInviteSent(
      const SIPMessage & msg
    );

    virtual void ICT_OnAckSent(
      const SIPMessage & msg
    );

    virtual void ICT_OnUnknownSent(
      const SIPMessage & msg
    );

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

    void EnqueueMessageEvent(
      int event,
      const SIPMessage & msg
    );

public:
    /// explicitly set the CallAnwerResponse.  If set, and OnIncomingConnection() returns FALSE,
    /// the underlying logic will send the respond code.  If not, the default will be sent
    PINLINE void SetCallAnswerResponse( AnswerCallDeniedResponse answer ){ m_CallAnswerResponse = answer; };
    PINLINE AnswerCallDeniedResponse GetCallAnswerResponse()const{ return m_CallAnswerResponse; };
    PINLINE void SetState( SessionState state ){ m_State = state; };
    PINLINE SessionState GetState()const{ return m_State; };
    PINLINE int GetCallEndReason()const{ return m_CallEndReason; };
    PINLINE void SetCallEndReason( int code ){ m_CallEndReason = code; };
    PINLINE int GetCallEndStatusCode()const{ return m_CallEndStatusCode; };
    PINLINE void SetCallEndStatusCode( int code ){ m_CallEndStatusCode = code; };
    PINLINE BOOL IsReferredCall()const{ return m_IsReferredCall; };
    PINLINE void SetReferredCall( BOOL isReferred = TRUE ){ m_IsReferredCall = isReferred; };
    PINLINE BOOL IsReferrerCall()const{ return m_IsReferrerCall; };
    PINLINE void SetReferrerCall( BOOL isReferrer = TRUE ){ m_IsReferrerCall = isReferrer; };
    PINLINE const SIPMessage& GetCurrentUASInvite() const { return m_CurrentUASInvite; }
    PINLINE const SIPMessage& GetCurrentUACInvite() const { return m_CurrentUACInvite; }
    void ReleaseTransferedCall();
    void SetTransferedCall( CallSession * call );
    PINLINE ICT_State GetICTState()const{ return m_ICTState; };
    PINLINE void SetXORHashOutgoingCalls( BOOL hash = TRUE ){ m_XORHashOutgoingCalls = hash; };
    PINLINE const PTimeInterval & GetMinSessionExpire()const{ return m_MinSessionExpire; };
    PINLINE void SetMinSessionExpire( const PTimeInterval & expire ){ m_MinSessionExpire = expire; };
    PINLINE const PTimeInterval & GetMaxSessionExpire()const{ return m_MaxSessionExpire; };
    PINLINE void SetMaxSessionExpire( const PTimeInterval & expire ){ m_MaxSessionExpire = expire; };
    PINLINE void EnableSessionTimer( BOOL enable = TRUE ){ m_EnableSessionTimer = enable; };
    PINLINE void DisableSessionTimer(){ m_EnableSessionTimer = FALSE; };
protected:

    CallSessionManager & m_CallSessionManager;
    PMutex m_EventMutex;
    PMutex m_AnswerCallMutex;
    PSyncPoint m_UACCancelSync;
    SessionState m_State;
    int m_CurrentInviteCSeq;
    int m_CallEndReason;
    int m_CallEndStatusCode;
    ICT_State m_ICTState;

    AnswerCallDeniedResponse m_CallAnswerResponse;
    friend class CallSessionManager;

    SIPMessage m_CurrentUACInvite;
    SIPMessage m_CurrentUASInvite;

    PMutex m_ProxyAuthMutex;
    BOOL m_HasSentProxyAuth;
    BOOL m_IsRedirectingCall;
    BOOL m_CanSendCancel;

    PQUEUE( UACFailoverQueue, SIPURI );
    PQUEUE( UACRedirectFailoverQueue, SIPURI );
    PMutex m_FailOverQueueMutex;
    UACFailoverQueue m_UACFailoverQueue;
    UACRedirectFailoverQueue m_UACRedirectFailoverQueue;
    int m_RedirectAttempts;

    BOOL m_IsReferredCall;
    BOOL m_IsReferrerCall;
    BOOL m_XORHashOutgoingCalls;
    SIPSession::GCRef * m_TransferedCall;

    PTimeInterval m_MinSessionExpire;
    PTimeInterval m_MaxSessionExpire;
    PTimer m_SessionTimer;
    PMutex m_SessionTimerMutex;
    BOOL m_EnableSessionTimer;
    PDECLARE_NOTIFIER( PTimer, UACORE::CallSession, _OnSessionTimerExpire );
  };
};

#endif


