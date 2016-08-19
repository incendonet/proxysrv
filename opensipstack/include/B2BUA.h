/*
 *
 * B2BUserAgent.h
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
 * $Log: B2BUA.h,v $
 * Revision 1.21  2007/01/10 08:07:15  rcolobong
 * 1. Add new method for OnSendB2BConnect Call Interface.
 * 2. OnSendB2BConnect is supported in B2BUserAgent, B2BUACall and B2BUAConnection
 *
 * Revision 1.20  2007/01/10 04:58:49  joegenbaclor
 * Added B2BUpperReg mode.
 *
 * Revision 1.19  2007/01/08 07:12:56  joegenbaclor
 * Added ability to run SBC in pure proxy or pure b2b modes
 *
 * Revision 1.18  2006/12/20 16:24:44  joegenbaclor
 * Implemented keep-alives for upper reg.
 *
 * Revision 1.17  2006/11/30 08:19:35  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.16  2006/11/23 11:00:02  joegenbaclor
 * Added Instant Message B2B support
 *
 * Revision 1.15  2006/11/23 01:28:18  joegenbaclor
 * Corrected compile errors from ryan's last commit
 *
 * Revision 1.14  2006/11/21 06:55:17  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.13  2006/10/30 02:42:26  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.12  2006/10/23 20:22:28  joegenbaclor
 * Modified Registration Methods to include a reference to the Register request
 *
 * Revision 1.11  2006/07/17 11:36:45  joegenbaclor
 * 1.  More routing enhancements to B2BUA
 *
 * Revision 1.10  2006/07/11 13:59:35  joegenbaclor
 * Completed Proxy functionalities for B2BUA
 *
 * Revision 1.9  2006/07/10 06:29:37  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.8  2006/06/29 05:09:13  joegenbaclor
 * Changed OnOutgoingCall invite parameter from const to none const to give a
 * chance for applications to modify the outbound invite before being sent to the transport
 *
 * Revision 1.7  2006/06/28 14:14:55  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.6  2006/06/21 14:29:11  joegenbaclor
 * This upload marks the first successful call by OpenB2BUA
 *
 * Revision 1.5  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
 *
 * Revision 1.4  2006/06/15 09:47:27  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.3  2006/06/14 08:43:38  joegenbaclor
 * Initial upload of OpenB2BUA applcation and related classes
 *
 * Revision 1.2  2006/04/11 00:18:54  joegenbaclor
 * More work on B2BUA class structures
 *
 * Revision 1.1  2006/04/10 01:07:36  joegenbaclor
 * Initial upload
 *
 *
 *
 */

#ifndef B2BUA_H
#define B2BUA_H

#include "SIPUserAgent.h"
#include "B2BUAEndPoint.h"
#include "Logger.h"
#include "B2BAuthInterface.h"
#include "B2BCallInterface.h"
#include "B2BMediaInterface.h"
#include "B2BRoutingInterface.h"
#include "ProxySessionManager.h"
#include "RegisterSessionManager.h"


using namespace Tools;
using namespace SIP;

namespace B2BUA
{

  class B2BCallInterface;
  class B2BAuthInterface;
  class B2BRoutingInterface;
  class B2BMediaInterface;
  class B2BUAConnection;
  class B2BUACall;


  class B2BUserAgent : public SIPUserAgent
  {
    PCLASSINFO( B2BUserAgent, SIPUserAgent );
  public:
    //////////////////////////////////////////////////////////
    class Registrar : public RegisterSessionManager
    {
      PCLASSINFO( Registrar, RegisterSessionManager );

    public:
      Registrar(
        B2BUserAgent & ua,
        PINDEX sessionThreadCount,
        PINDEX stackSize
      );

      virtual ~Registrar();

      /// OSS Callbacks

      virtual RegisterSession::AcceptRegistrationResponse OnAcceptRegistration( 
        RegisterSession & session,
        const SIPMessage & request
      );

      /// pure virtuals from SIPRegistrar
      virtual BOOL OnRequestA1Hash(
        const SIPURI & userURL,
        PString & a1,
        RegisterSession & session 
      );

      virtual BOOL OnStoreRegistrationInfo(
          RegisterSession& session,
          const SIPURI& accountInfo
      );

      virtual BOOL OnStoreUpperRegistrationInfo(
          ProxySession & session,
          const SIPMessage & message
      );

      virtual void OnRegistrationAccepted(
        RegisterSession & session,
        const SIPMessage & request
      );

      virtual void OnRegistrationRejected(
        RegisterSession & session,
        const SIPMessage & request
      );

      virtual void OnUnregistration(
        RegisterSession & session,
        const SIPMessage & request
      );


      virtual BOOL IsKnownUser(
        const SIPURI & usrURI
      )const;

      virtual void ProcessUpperRegKeepAlive();

    protected:
      PThread * m_UpperRegKeepAliveThread;
      PSyncPoint m_UpperRegSync;
    };
    ////////////////////////////////////////////////////////////
    class Proxy : public ProxySessionManager
    {
      PCLASSINFO( Proxy, ProxySessionManager );
    public:
      Proxy(
        B2BUserAgent & ua,
        PINDEX sessionThreadCount,
        PINDEX stackSize
      );

      virtual ProxySession::RoutePolicy OnCheckRoutePolicy(
        ProxySession & session,
        SIPMessage & request
      );

      virtual void OnFinalResponse(
        ProxySession & session,
        const SIPMessage & request
      );

      virtual void OnOrphanedMessage(
        const SIPMessage & message
      );
    };

    ////////////////////////////////////////////////////////////

    enum UAMode
    {
      FullMode = 0,
      ProxyOnlyMode,
      B2BOnlyMode,
      B2BUpperRegMode
    };

    B2BUserAgent( 
      UAMode uaMode = FullMode 
    );
    
    virtual ~B2BUserAgent();

    /// SIPUserAgent virtual func override
    virtual BOOL Initialize(
      PINDEX threadCount
    );

    virtual void ProcessEvent(
        SIPStackEvent * event
    );

    /// Signaling Callbacks

    virtual BOOL OnIncomingCall(
      B2BUAConnection & connection,
      B2BUACall & call,
      const SIPMessage & invite
    );

    BOOL OnIncomingSDPOffer(
      B2BUAConnection & connection,
      B2BUACall & call,
      const SIPMessage & sdp
    );

    virtual BOOL OnRequireSDPOffer(
      B2BUAConnection & connection,
      B2BUACall & call,
      SIPMessage & offer
    );

    virtual BOOL OnRequireSDPAnswer(
      B2BUAConnection & connection,
      B2BUACall & call,
      const SIPMessage & offer,
      SIPMessage & answer
    );

    virtual BOOL OnOutgoingCall(
      B2BUAConnection & connection,
      B2BUACall & call,
      SIPMessage & invite
    );

    virtual BOOL OnCallReinvite(
      B2BUAConnection & connection,
      B2BUACall & call,
      const SIPMessage & invite
    );

    virtual BOOL OnProceeding(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & proceeding
    );

    virtual BOOL OnCallAbandoned(
      B2BUAConnection & connection,
      B2BUACall & inboundCall,
      const SIPMessage & cancel
    );

    virtual BOOL OnAlerting(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & alerting
    );

    virtual BOOL OnProgress(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & progress
    );

    virtual BOOL OnRejected(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & reject
    );

    virtual BOOL OnConnected(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & connect
    );

    virtual BOOL OnSendB2BConnect(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & connect
    );

    virtual BOOL OnDisconnected(
      B2BUAConnection & connection,
      B2BUACall & outboundCall,
      const SIPMessage & bye
    );


    virtual BOOL OnInSessionMessage(
      B2BUAConnection & connection,
      B2BUACall & remoteCall,
      const SIPMessage & request
    );

    virtual void OnSetupOutbound(
      const SIPMessage & request,
      B2BUAConnection & connection
    );

    virtual void OnRequireRedirectionId(
      B2BUAConnection & connection,
      B2BUACall & call,
      PString & redirectionId
    );

    virtual void OnRequireCallTransferId(
      B2BUAConnection & connection,
      B2BUACall & call,
      PString & redirectionId
    );

    virtual void OnReceivedInstantMessage(
      const SIPMessage & message
    );

    /// Registrar Callbacks
    virtual RegisterSession::AcceptRegistrationResponse OnAcceptRegistration( 
      RegisterSession & session,
      const SIPMessage & request
    );

    /// pure virtuals from SIPRegistrar
    virtual BOOL OnRequireAuthorizationHash(
      const SIPURI & userURL,
      PString & a1,
      RegisterSession & session 
    );

    virtual BOOL OnStoreRegistrationInfo(
        RegisterSession& session,
        const SIPURI& accountInfo
    );

    virtual BOOL OnStoreUpperRegistrationInfo(
        ProxySession & session,
        const SIPMessage & message
    );


    virtual void OnRegistrationAccepted(
      RegisterSession & session,
      const SIPMessage & request
    );

    virtual void OnRegistrationRejected(
      RegisterSession & session,
      const SIPMessage & request
    );

    virtual void OnUnregistration(
      RegisterSession & session,
      const SIPMessage & request
    );

    ProxySession::RoutePolicy RouteProxyRequest(
      ProxySession & session,
      SIPMessage & request
    );

    BOOL FindAndRemovePendingInviteByCallId(
      const PString & i_sCallid
    );

    BOOL IsKnownUser(
      const SIPURI & userURI
    )const;

    virtual void OnOrphanedMessage(
      SIPMessage & message
    );

    virtual B2BCallInterface * OnCreateCallInterface();

    virtual B2BAuthInterface * OnCreateAuthInterface();

    virtual B2BMediaInterface * OnCreateMediaInterface();

    virtual B2BRoutingInterface * OnCreateRoutingInterface();

    PINLINE void SetEndPoint( B2BUAEndPoint * ep ){ m_EndPoint = ep; };
    PINLINE B2BUAEndPoint * GetB2BUAEndPoint(){ return m_EndPoint; };
    PINLINE void SetRegistrar( Registrar * registrar ){ m_Registrar = registrar; };
    PINLINE Registrar * GetRegistrar(){ return m_Registrar; };
    PINLINE void SetProxy( Proxy * proxy ){ m_Proxy = proxy; };
    PINLINE Proxy * GetProxy(){ return m_Proxy; };
    PINLINE WORD GetBackDoorPort()const{ return m_BackDoorPort; };
    PINLINE UAMode GetUAMode()const{ return m_UAMode; };
  protected:

    B2BCallInterface * m_CallHandler;
    B2BAuthInterface * m_AuthHandler;
    B2BRoutingInterface * m_RoutingHandler;
    B2BMediaInterface * m_MediaHandler;

    B2BUAEndPoint * m_EndPoint;
    Registrar * m_Registrar;
    Proxy * m_Proxy;

    WORD m_BackDoorPort;
    UAMode m_UAMode;

  protected:
    

  };

}

#endif


