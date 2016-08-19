/*
 *
 * OpalOSSEndPoint.h
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
 * $Log: OpalOSSEndPoint.h,v $
 * Revision 1.29  2006/12/22 07:39:06  joegenbaclor
 * More on command line argument parsing.
 *
 * Revision 1.28  2006/12/04 07:32:49  joegenbaclor
 * Added RFC3680 methods to Opal classes
 *
 * Revision 1.27  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.26  2006/10/30 02:42:26  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.25  2006/10/23 20:22:28  joegenbaclor
 * Modified Registration Methods to include a reference to the Register request
 *
 * Revision 1.24  2006/10/12 05:22:42  joegenbaclor
 * Enabled ability to make encrypted outbound calls for OPAL
 *
 * Revision 1.23  2006/08/28 07:18:34  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.22  2006/08/26 14:31:40  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.21  2006/08/15 15:42:08  joegenbaclor
 * 1. Added Instant Messaging functions to OPAL
 *
 * Revision 1.20  2006/07/10 06:29:37  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.19  2006/06/28 14:14:57  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.18  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.17  2006/06/08 09:02:40  joegenbaclor
 * Added SendUnregister() method to OpalOSSEndPoint
 *
 * Revision 1.16  2006/06/08 04:54:20  joegenbaclor
 * !.  Added OnRegistrationTimeout Call back to Registrar classes
 * 2. Moved ECCIAPI to Tools namespace
 *
 * Revision 1.15  2006/05/19 06:30:37  joegenbaclor
 * 1.  Fixed bug in SIPHeaderB where tag may not be parsed properly if no < > enclosure is present
 * 2.  Various enhancements to OpenSBC and proxy session
 *
 * Revision 1.14  2006/04/20 14:32:28  joegenbaclor
 * Fixed early media bug where soucd channels in OPAL ay not be opened in time for ringback to start streaming.
 *
 * Revision 1.13  2006/04/18 07:18:21  joegenbaclor
 * Fixed unix build environment errors and warnings
 *
 * Revision 1.12  2006/04/07 13:50:17  joegenbaclor
 * updated autoconf files in linux
 *
 * Revision 1.11  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.10  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.9  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.8  2006/03/17 13:50:54  joegenbaclor
 * 1.  Implemented UAC Call Redirection
 * 2.  Implemented SendCancel() method in CallSession
 * 3.  Introduced ClearInternalHeaders() function in SIP Message to erase internal headers inheritted from previous transaction.
 *
 * Revision 1.7  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.6  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.5  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */


#ifndef OPALOSSENDPOINT_H
#define OPALOSSENDPOINT_H

#include "ossbuildopts.h"

#if HAS_OPAL

#include <ptlib.h>
#include <opal/endpoint.h>

#include "SIPStack.h"
#include "SIPUserAgent.h"
#include "CallSessionManager.h"
#include "RegisterSessionManager.h"
#include "RFC3680Client.h"

using namespace SIP;
using namespace UACORE;

namespace OPAL
{

  class OpalOSSUserAgent;
  class OpalOSSRegistrar;
  class OpalOSSCore;
  class OpalOSSConnection;

  class OpalOSSEndPoint : public OpalEndPoint
  {
    PCLASSINFO( OpalOSSEndPoint, OpalEndPoint );
  public:

    OpalOSSEndPoint(
      OpalManager & manager
    );
    
    virtual ~OpalOSSEndPoint();

    virtual BOOL NewIncomingConnection(
      OpalTransport * transport  /// Transport connection came in on
    );
    

    virtual BOOL MakeConnection(
	    OpalCall & call,        /// Owner of connection
	    const PString & party,  /// Remote party to call
	    void * userData = NULL  /// Arbitrary data to pass to connection
	  );

    virtual OpalOSSConnection * CreateConnection(
      OpalCall & call,
      const PString &remoteParty,
      const PString & token,
      void * userData
    );

    BOOL Initialize();

    BOOL SendRegister(
      const PString & userName,  /// user account
      const PString & password,  /// user password
      const PString & uri, /// address of the registrar
      const PString & domain = PString::Empty(), /// domain/realm
      BOOL synchronous = FALSE
    );

    BOOL SendUnregister();

    BOOL SetProxy(
      const PString & uri, /// address of the proxy
      const PString & userName = PString::Empty(), /// username to be sent when we get challenged
      const PString & password = PString::Empty()  /// password to be sent when we get challenged
    );

    SIPURI GetProxy();

    BOOL SetDomain(
      const PString & uri /// uri of the domain
    );

    OpalMediaFormatList GetMediaFormats() const;

    ProfileUA & GetProfile();

    /// registrar callbacks
    virtual RegisterSession::AcceptRegistrationResponse OnAcceptRegistration(
      RegisterSession & session,
      const SIPMessage & request
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

    virtual BOOL OnRequestA1Hash(
      const SIPURI & userURL,
      PString & a1,
      RegisterSession & session
    );

    /// Call related callbacks

    
    virtual void OnConnected(
      CallSession & session,
      const SIPMessage & msg
    );

    /// will be called when the call is established
    virtual void OnEstablished(
      CallSession & session
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

    /// returning TRUE here will give permission to the session
    /// to resend the invite to the new contact header in the response.
    /// returning FALSE will end the connection
    virtual BOOL OnCallRedirect(
      CallSession & session,
      const SIPMessage & redirect
    );

    virtual BOOL OnIncomingSDPOffer(
      const SIPMessage & sdp,
      CallSession & session
    );

    virtual BOOL OnRequireSDPAnswer(
      const SIPMessage & offer,
      SIPMessage & answer,
      CallSession & session
    );

    virtual BOOL OnRequireSDPOffer(
      SIPMessage & offer,
      CallSession & session
    );

    virtual BOOL OnIncomingConnection(
      const SIPMessage & invite,
      CallSession & session
    );

    virtual BOOL OnSetupIncomingCall(
      const SIPMessage & invite,
      CallSession & session
    );

    virtual void OnReceivedInstantMessage(
      const SIPMessage & message
    );

    virtual void OnRegEventStateOnline(
      const SIPURI & resourceURI
    );

    virtual void OnRegEventStateOffLine(
      const SIPURI & resourceURI
    );
    
    SIPTransportManager & GetTransportManager();

    OpalTransport * GetOpalTransport(
      const PIPSocket::Address & target
    );

    PINLINE BOOL IsRegistered() const{ return m_IsRegistered; };
    PINLINE OpalOSSUserAgent * GetUserAgent(){ return m_OSSUserAgent; };
    PINLINE void SetRegistrarTimeToLive( const PTimeInterval & ttl ){ m_RegisterExpires = ttl; };
    PINLINE const PTimeInterval & GetRegistrarTimeToLive()const{ return m_RegisterExpires; };
  protected:
    OpalOSSUserAgent * m_OSSUserAgent;
    ProfileUA m_Profile;
    PSyncPoint m_RegMutex;
    BOOL m_IsRegistered;
    PTimeInterval m_RegisterExpires;
    PString m_CurrentRegSession;
  };

  ////////////////////////////////////////////////////////////////////////
  class OpalRFC3680Client : public RFC3680Client
  {
    PCLASSINFO( OpalRFC3680Client, RFC3680Client );
  public:
    OpalRFC3680Client(
      OpalOSSUserAgent & ua
    );

    virtual void OnStateOnline(
      const SIPURI & resourceURI
    );

    virtual void OnStateOffLine(
      const SIPURI & resourceURI
    );

  protected:
    OpalOSSUserAgent & m_OpalOSSUserAgent;
    
  };

  class OpalOSSUserAgent : public SIPUserAgent
  {
    PCLASSINFO( OpalOSSUserAgent, SIPUserAgent );
  public:

    OpalOSSUserAgent( 
      OpalOSSEndPoint & ep
    );
    virtual ~OpalOSSUserAgent();

    /// SIPUserAgent virtual func override
    virtual void ProcessEvent(
        SIPStackEvent * event
    );

    virtual void OnRegEventStateOnline(
      const SIPURI & resourceURI
    );

    virtual void OnRegEventStateOffLine(
      const SIPURI & resourceURI
    );


    /// RFC 3680 support
    void WatchRegistrationState(
      const PString & stateAgent,
      const PString & resourceURI,
      const PTimeInterval & expires = PTimeInterval( 0, 90 ),
      const PString & authUser = PString::Empty(),
      const PString & authPassword = PString::Empty()
      
    );

    void StopWatchingRegistrationState(
      const PString & resourceURI
    );

    virtual BOOL Initialize(
      PINDEX threadCount = 2
    );

    PINLINE OpalOSSEndPoint & GetOpalEndPoint(){ return m_OpalEndPoint; };
    PINLINE OpalOSSCore * GetCore(){return m_Core; };
    PINLINE OpalOSSRegistrar * GetRegistrar(){ return m_Registrar; };
  private:

    OpalOSSEndPoint & m_OpalEndPoint;
    OpalOSSCore * m_Core;
    OpalOSSRegistrar * m_Registrar;
    RFC3680Client * m_RFC3680Client;
  };

  ////////////////////////////////////////////////////////////////////////

  class OpalOSSRegistrar : public RegisterSessionManager
  {
    PCLASSINFO( OpalOSSRegistrar, RegisterSessionManager );
  public:

    OpalOSSRegistrar( 
      OpalOSSUserAgent & ua,
      OpalOSSEndPoint & ep
    );

    ~OpalOSSRegistrar();

    virtual RegisterSession::AcceptRegistrationResponse OnAcceptRegistration(
      RegisterSession & session,
      const SIPMessage & request
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

    virtual BOOL OnRequestA1Hash(
      const SIPURI & userURL,
      PString & a1,
      RegisterSession & session
    );


    PINLINE OpalOSSEndPoint & GetOpalEndPoint(){ return m_OpalEndPoint; };

  private:

    OpalOSSEndPoint & m_OpalEndPoint;

  };

  ////////////////////////////////////////////////////////////////////////

  class OpalOSSCore : public CallSessionManager
  {
    PCLASSINFO( OpalOSSCore, CallSessionManager );
  public:

    OpalOSSCore( 
      OpalOSSUserAgent & ua,
      OpalOSSEndPoint & ep
    );


    virtual ~OpalOSSCore();

    SIPSession::GCRef MakeCall(
      SIPURI & remoteURI,
      const PString & callToken,
      BOOL hashCall = FALSE
    );

    CallSession::AnswerCallResponse OnAnswerCall(
      const SIPMessage & invite,
      CallSession & session
    );

    virtual void OnConnected(
      CallSession & session,
      const SIPMessage & msg
    );

    virtual void OnEstablished(
      CallSession & session
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

    /// returning TRUE here will give permission to the session
    /// to resend the invite to the new contact header in the response.
    /// returning FALSE will end the connection
    virtual BOOL OnCallRedirect(
      CallSession & session,
      const SIPMessage & redirect
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

    virtual BOOL OnIncomingConnection(
      const SIPMessage & invite,
      CallSession & session
    );

    virtual BOOL OnSetupIncomingCall(
      const SIPMessage & invite,
      CallSession & session
    );

    virtual void OnReceivedInstantMessage(
      const SIPMessage & message
    );

    virtual void OnRegEventStateOnline(
      const SIPURI & resourceURI
    );

    virtual void OnRegEventStateOffLine(
      const SIPURI & resourceURI
    );

    PINLINE OpalOSSEndPoint & GetOpalEndPoint(){ return m_OpalEndPoint; };

  private:

    OpalOSSEndPoint & m_OpalEndPoint;
  };

};

#endif // HAS_OPAL

#endif // OPALOSSENDPOINT_H


