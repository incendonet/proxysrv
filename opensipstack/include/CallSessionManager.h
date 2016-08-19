/*
 *
 * CallSessionManager.h
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
 * $Log: CallSessionManager.h,v $
 * Revision 1.31  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.30  2006/09/01 13:09:59  rcolobong
 * Provide Setter for Expiry in IST200OkRetransmission
 *
 * Revision 1.29  2006/08/30 03:42:04  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.28  2006/08/28 07:18:34  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.27  2006/08/26 14:31:40  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.26  2006/08/09 12:17:46  joegenbaclor
 * Added RFC 3428 Support
 *
 * Revision 1.25  2006/06/28 14:14:56  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.24  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.23  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.22  2006/04/29 03:22:54  joegenbaclor
 * 1.  More Refer support code added to call session
 * 2.  Added StoreRegistrationInfo callback to Registrar classes
 *
 * Revision 1.21  2006/04/26 01:00:10  joegenbaclor
 * Added REFER NIST callbacks
 *
 * Revision 1.20  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.19  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.18  2006/03/14 12:55:17  joegenbaclor
 * Removed Separate SIPClassese for simplicity.
 * Instead we will implement dialog support directly in SIPSession.
 *
 * Revision 1.17  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef CALLSESSIONMANAGER_H
#define CALLSESSIONMANAGER_H

#include "SIPSessionManager.h"
#include "CallSession.h"


using namespace SIP;
using namespace Profiles;

namespace UACORE
{
  class CallSessionManager : public SIPSessionManager
  {
    PCLASSINFO( CallSessionManager, SIPSessionManager );
  public:

      class IST200OkRetransmission : public PObject
      {
        PCLASSINFO( IST200OkRetransmission, PObject );
      public:
        IST200OkRetransmission( 
          const SIPMessage & msg,
          const PString & sessionId,
          CallSessionManager & manager 
        ); 

        ~IST200OkRetransmission();

        void Retransmit();
        
        void PostExpireEvent();
        
        BOOL IsExpired();

        PINLINE const PString & GetRetransmissionId()const{return m_RetransmissionId;};
        PINLINE const PString & GetSessionId()const{ return m_SessionId; };
        PINLINE void SetExpired() { PWaitAndSignal lock( m_LocalMutex ); m_IsExpired = TRUE; }

      private:
        PString m_SessionId;
        PString m_RetransmissionId;
        BOOL m_IsExpired;

        CallSessionManager & m_CallSessionManager;
        PMutex m_LocalMutex;

        PTime m_StartTime;
        DWORD m_TimerValue;
        int m_RetransmitCount;
        SIPMessage m_200Ok;
      };

      //////////////////////////////////////////////////////////////////////////
      class RetransmitThread : public PThread
      {
        PCLASSINFO( RetransmitThread, PThread );
      public:
        
        RetransmitThread( CallSessionManager & manager );
        
        ~RetransmitThread();

        void Expire200Ok( 
          const PString & id 
        );

        void Expire200Ok(
          const SIPMessage & ack
        );
        
        void AddMessage(
          const PString & sessionId,
          const SIPMessage & message 
        );

        void Handle200OkRetransmission();
        
        void Main();

        BOOL Has200OkMessage( 
          const PString & id 
        )const;

      private:
        PStringSet m_200OkToBeCleaned;
        PDICTIONARY( _RETRAN200OK, PString, IST200OkRetransmission );
        _RETRAN200OK m_200OkRetransmissionQueue;
        PMutex m_200OkRetransmissionMutex;
        PSyncPoint m_SyncPoint;
        BOOL m_ExitFlag;
        CallSessionManager & m_CallSessionManager;
      };

    CallSessionManager(
      SIPUserAgent & ua,
      PINDEX sessionThreadCount,
      PINDEX stackSize
    );

    virtual ~CallSessionManager();


    virtual SIPSession::GCRef MakeCall(
      SIPURI & remoteURI,
      ProfileUA * profile = NULL
    );

    /// will be called by a CallSession after receipt of a REFER.
    /// this method should NOT be called from outside the call session object
    /// returns a smart pointer to the new call
    virtual SIPSession::GCRef TransferCallAsUAS(
      CallSession & callToTransfer,
      const SIPMessage & refer
    );

    /// will be called by the call session before calling TransferCallAsUAS.
    /// returning FALSE here will result to the rejection of the transfer request
    virtual BOOL OnCallBeingTransfered(
      CallSession & callToTransfer,
      const SIPMessage & refer
    );
    
    /// will send a REFER to UAS and ask it to transfer the 
    /// call to a reote URI. Returns FALSE if sending fails
    virtual BOOL TransferCallAsUAC(
      CallSession & callToTransfer,
      SIPURI & remoteURI
    );



  protected:
    virtual SIPSession::GCRef OnCreateServerSession(
      const SIPMessage & request
    );

    virtual SIPSession * OnCreateClientSession(
      const ProfileUA & profile,
      const PString & sessionId
    );

  public:
    virtual CallSession * OnCreateServerCallSession(
      const SIPMessage & request,
      const PString & sessionId
    );

    virtual CallSession * OnCreateClientCallSession(
      const ProfileUA & profile,
      const PString & sessionId
    );

    virtual void OnUnknownTransaction(
      SIPUnknownTransaction & event
    );

    /// returning FALSE here will automatically reject the incoming call
    virtual BOOL OnIncomingConnection(
      const SIPMessage & invite,
      CallSession & session
    );

    virtual BOOL OnSetupIncomingCall(
      const SIPMessage & invite,
      CallSession & session
    );

    virtual BOOL OnReinvite(
      const SIPMessage & invite,
      CallSession & session
    );

    /// returning FALSE here will reject the call with no acceptable format
    /// depending on call state.  Offers received in ACK will be disconnected
    /// using a BYE.  
    virtual BOOL OnIncomingSDPOffer(
      const SIPMessage & sdp,
      CallSession & session
    );

    /// your last chance to produce an answer to the offer.  
    /// This will be called either before sending 183, 200 or ACK.
    /// AnsweCall is therefore a prerequisite before this callback
    /// is called
    virtual BOOL OnRequireSDPAnswer(
      const SIPMessage & offer,
      SIPMessage & answer,
      CallSession & session
    );

    virtual BOOL OnRequireSDPOffer(
      SIPMessage & offer,
      CallSession & session
    );


    /// this is where implementors may modify or add headers before underlying implementation
    /// sends the actual response to INVITE to the FSM
    virtual BOOL OnSendingResponseToInvite(
      SIPMessage & response,
      CallSession & session
    );

    /// this is where implementors may modify or add headers before underlying implementation
    /// sends the actual response messages to the FSM for none-invite transactions
    virtual BOOL OnSendingResponseToNoneInvite(
      SIPMessage & response,
      CallSession & session
    );


    /// answers the call.  based on the mode.  May be called several times
    BOOL AnswerCall( 
      const PString & sessionId,
      CallSession::AnswerCallResponse mode
    );

    /// returns the type of response the underlying implmentation would
    /// send in response to an invite.  the default behavior is to return
    /// AnswerCallNow
    virtual CallSession::AnswerCallResponse OnAnswerCall(
      const SIPMessage & invite,
      CallSession & session
    );

    /// will be called if a rediration URI is needed by OnAnswerCall
    virtual BOOL OnRequireRedirectionURI(
      ContactList & contacts,
      CallSession & session,
      BOOL isProxyRedirect
    );

    /// will be called when the call is established
    virtual void OnConnected(
      CallSession & session,
      const SIPMessage & msg
    );
    
    /// will be called when the call is disconnected
    virtual void OnDisconnected(
      CallSession & session,
      const SIPMessage & message
    );

    
    /// Progress callbacks
    virtual void OnAlerting(
      CallSession & session,
      const SIPMessage & message
    );

    virtual void OnProgress(
      CallSession & session,
      const SIPMessage & message
    );

    /// Call being forwarded
    virtual BOOL OnCallRedirect(
      CallSession & session,
      const SIPMessage & redirect
    );


    // will be called when the REFER is responded to by a 202 or 200
    virtual void OnReferAccepted(
      CallSession & session
    );

    /// will be called if the refer is rejected by the UAS using 3xx-6xx response
    virtual void OnReferRejected(
      CallSession & session,
      const SIPMessage & reject
    );

    virtual void OnReferSubscriptionState(
      CallSession & session,
      const Event & event,
      const SubscriptionState & state,
      const PString sipFrag
    );


    virtual void OnOrphanedMessage(
      const SIPMessage & message
    );

    virtual void OnReceivedOptions(
      const SIPMessage & options
    );

    private: void InternalOnReceivedInstantMessage(
      const SIPMessage & message
    );

    public: virtual void OnReceivedInstantMessage(
      const SIPMessage & message
    );

    void SendInstantMessage(
      const SIPURI & uri,
      const PString & body
    );

    virtual void OnSessionManagerEvent(
      PObject * event
    );

    PINLINE PThread * GetRetransmitThread(){ return m_200OkRetransmitThread; };

  protected:
    PThread * m_200OkRetransmitThread;

  };


};

#endif

