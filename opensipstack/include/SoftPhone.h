/*
 *
 * SoftPhone.h
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
 * $Log: SoftPhone.h,v $
 * Revision 1.10  2007/01/01 05:12:52  joegenbaclor
 * Introduced termiantion flag to avoid crashes on exit.
 * TO DO: Maybe the best thing to do here is to have a WaitForTermination()
 * function exposed for the SIPStack
 *
 * Revision 1.9  2006/12/18 10:31:35  joegenbaclor
 * Fixed ACM G.723.1 one way audio bug
 *
 * Revision 1.8  2006/12/12 05:26:08  joegenbaclor
 * Modified DTMF player to use PDTMFEncoder
 *
 * Revision 1.7  2006/12/07 09:12:21  joegenbaclor
 * 1.  Changed registration expire value from milliseconds to seconds
 *
 * Revision 1.6  2006/12/06 06:40:54  joegenbaclor
 * STUN related fixes
 *
 * Revision 1.5  2006/12/06 01:59:22  joegenbaclor
 * More Softphone support
 *
 * Revision 1.4  2006/12/04 07:32:49  joegenbaclor
 * Added RFC3680 methods to Opal classes
 *
 * Revision 1.3  2006/12/02 01:12:13  joegenbaclor
 * Initial upload of SoftPhoneInterface classes
 *
 * Revision 1.2  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/11/28 15:43:01  joegenbaclor
 * Migrated SoftPhone.* to lirabry level code
 *
 * Revision 1.2  2006/11/27 08:59:40  joegenbaclor
 * more work on libsoftphone
 *
 * Revision 1.1  2006/11/26 23:06:24  joegenbaclor
 * Added libsoftphone files
 *
 * Revision 1.6  2006/10/30 08:29:28  joegenbaclor
 * Added OSSPhone to ATLSIP source tree
 *
 * Revision 1.5  2006/10/03 09:29:59  joegenbaclor
 * Added INFO DTMF relay support
 *
 * Revision 1.4  2006/09/13 10:56:12  joegenbaclor
 * 1.  Added Release minimal build configuration
 * 2.  Added upx support for DLL compression
 *
 * Revision 1.3  2006/08/30 08:42:54  joegenbaclor
 * Corrected bug on OnDisconnect
 *
 * Revision 1.2  2006/08/24 10:21:49  joegenbaclor
 * 1.  Major bug fix in double creation of pointers for the SIP End Pint in CallManager initialization routines
 * 2.  Fixed flush address book
 *
 * Revision 1.1  2006/08/23 07:47:33  joegenbaclor
 * Initial upload of ATLSIP files
 *
 *
 */

#ifndef OPAL_SOFTPHONE_H
#define OPAL_SOFTPHONE_H

#include <ptlib.h>
#include "ossbuildopts.h"

#if HAS_OPAL


#include <OpalOSSEndPoint.h>
#include <OpalOSSConnection.h>
#include <opal/manager.h>
#include <opal/pcss.h>
#include <ptclib/pstun.h>
#include "ECCIAPI.h"
#include "SoftPhoneInterface.h"
#include "VoiceFileChannel.h"
#include <list>

using namespace EQ;
using namespace Tools;
using namespace OPAL;

namespace SF
{
  class SoftPhoneManager;
  class SoftPhoneInterface;

  class SoftPhonePCSSEndPoint : public OpalPCSSEndPoint
  {
    PCLASSINFO(SoftPhonePCSSEndPoint, OpalPCSSEndPoint);

    public:
      SoftPhonePCSSEndPoint(
        SoftPhoneManager & manager
      );

      PINLINE SoftPhoneManager & GetSoftPhoneManager(){ return m_Manager; };

    private:

      virtual PString OnGetDestination(
        const OpalPCSSConnection & connection
      );

      virtual void OnShowIncoming(
        const OpalPCSSConnection & connection
      );

      virtual BOOL OnShowOutgoing(
        const OpalPCSSConnection & connection
      );


      SoftPhoneManager & m_Manager;
  };

  class SoftPhoneSIPEndPoint : public OpalOSSEndPoint
  {
    public:
      SoftPhoneSIPEndPoint(
        SoftPhoneManager & manager
      );

      PINLINE SoftPhoneManager & GetSoftPhoneManager(){ return m_Manager; };

    private:
      virtual BOOL OnIncomingConnection(
        const SIPMessage & invite,
        CallSession & session
      );

      virtual OpalOSSConnection * CreateConnection(
        OpalCall & call,
        const PString &remoteParty,
        const PString & token,
        void * userData
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

      void OnUnregistration(
        RegisterSession & session,
        const SIPMessage & request
      );

      virtual void OnConnected(
        CallSession & session,
        const SIPMessage & connect
      );

     
      /// will be called when the call is disconnected
      virtual void OnDisconnected(
        CallSession & session,
        const SIPMessage & msg
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

      virtual void OnReceivedInstantMessage(
        const SIPMessage & message
      );

      virtual void OnRegEventStateOnline(
        const SIPURI & resourceURI
      );

      virtual void OnRegEventStateOffLine(
        const SIPURI & resourceURI
      );

  protected:
      SoftPhoneManager & m_Manager;
  };

  class SoftPhoneMedia
  {
  public:
    SoftPhoneMedia(
      const char * source,
      const PString & format
    ) : sourceProtocol(source),
        mediaFormat(format),
        preferenceOrder(-1), // -1 indicates disabled
        dirty(false)
    { }

    BOOL operator<(const SoftPhoneMedia & other) { return preferenceOrder < other.preferenceOrder; }

    const char    * sourceProtocol;
    OpalMediaFormat mediaFormat;
    int             preferenceOrder;
    BOOL            dirty;
  };

  

  typedef std::list<SoftPhoneMedia> SoftPhoneMediaList;

  class SoftPhoneManager : public OpalManager, public Logger, public XMLRPCTransactionServer
  {
    PCLASSINFO( SoftPhoneManager, OpalManager );
  public:

    enum CallState {
      IdleState = 0,
      CallingState,
      RingingState,
      AnsweringState,
      InCallState,
      NumCallState
    };

    SoftPhoneManager( 
      SoftPhoneInterface * iface
    );

    ~SoftPhoneManager();

    BOOL Initialise();

    BOOL InitializeSIP();

    void Terminate();

    void MakeCall(
      const PString & address
    );

    void SendInstantMessage(
      const PString & address,
      const PString & msg
    );

    PString IMParseURI(
      const PString & msg
    );

    PString IMParseMessage(
      const PString & msg
    );

    BOOL SendRegister();

    BOOL SendUnregister();
    
    void AnswerCall();
    
    void RejectCall();
    
    void HangUpCall();
    
    void SendUserInput(
      char tone,
      int duration = 180,
      BOOL asRFC2833 = TRUE
    );
    
    void OnRinging(
      const OpalPCSSConnection & connection
    );

    void SetPlayDevice( 
      const PString & dev 
    );

    void GetDefaultRecordDevice( 
      PString & dev 
    );

    void GetDefaultPlayDevice( 
      PString & dev 
    );

    void SetRecordDevice(
      const PString & dev
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

    virtual void OnReceivedInstantMessage(
      const SIPMessage & message
    );

    virtual void OnRegEventStateOnline(
      const SIPURI & resourceURI
    );

    virtual void OnRegEventStateOffLine(
      const SIPURI & resourceURI
    );


    PSafePtr<OpalCall> GetCall() { return FindCallWithLock(m_CurrentCallToken); }

  private:

    virtual void OnEstablishedCall(
      OpalCall & call   /// Call that was completed
    );

    virtual void OnClearedCall(
      OpalCall & call   /// Connection that was established
    );

    virtual BOOL OnOpenMediaStream(
      OpalConnection & connection,  /// Connection that owns the media stream
      OpalMediaStream & stream    /// New media stream being opened
    );

    virtual BOOL CreateVideoOutputDevice(
      const OpalConnection & connection,    /// Connection needing created video device
      const OpalMediaFormat & mediaFormat,  /// Media format for stream
      BOOL preview,                         /// Flag indicating is a preview output
      PVideoOutputDevice * & device,        /// Created device
      BOOL & autoDelete                     /// Flag for auto delete device
    );

    virtual void OnUserInputString(
      OpalConnection & connection,  /// Connection input has come from
      const PString & value         /// String value of indication
    );

    
    
    void InitMediaInfo(
      const char * source, 
      const OpalMediaFormatList & formats
    );
    
    void ApplyMediaInfo();

  public:

    double RPCGetAccountBalance(
      const PString & server,
      const PString & account
    );

    //void PlayRinging();
    
    void SetState(
      CallState newState
    );

    NAT::SIPSTUNClient::NatTypes SetSTUNServer(
      const PString & server,
      WORD udpPortBase = 10000,
      WORD udpPortMax = 20000,
      WORD rtpPortBase = 30000,
      WORD rtpPortMax = 40000
    );

    /// plays an audible ringing sound
    void PlayRinging();

    void StopRinging();

    //BOOL m_PlayAlerting;
    //PDECLARE_NOTIFIER( PThread, SoftPhoneManager, PlayRinging );
    
    PINLINE CallState GetState()const{ return m_CallState; };
    PINLINE const PString & GetUserName()const{ return m_UserName; };
    PINLINE void SetUserName( const PString & name ){ m_UserName = name; };
    PINLINE const PString & GetDisplayName()const{ return m_DisplayName; };
    PINLINE void SetDisplayName( const PString & display ){ m_DisplayName = display; };

    PINLINE const PString & GetRegistrarAddress()const{ return m_RegistrarAddress; };
    PINLINE void SetRegistrarAddress( const PString & address ){ m_RegistrarAddress = address; };
    PINLINE const PString & GetRegistrarUserName()const{ return m_RegistrarUserName; };
    PINLINE void SetRegistrarUserName( const PString & user ){ m_RegistrarUserName = user; };
    PINLINE const PString & GetRegistrarPassword()const{ return m_RegistrarPassword; };
    PINLINE void SetRegistrarPassword( const PString & password ){ m_RegistrarPassword = password; };
    PINLINE int GetRegistrarTTL()const{ return m_RegistrarTTL; };
    PINLINE void SetRegistrarTTL( int ttl ){ m_RegistrarTTL = ttl; m_SIPEndPoint->SetRegistrarTimeToLive(PTimeInterval(0, m_RegistrarTTL)); };
    
    PINLINE const PString & GetProxyAddress()const{ return m_ProxyAddress; };
    PINLINE void SetProxyAddress( const PString & address ){ m_ProxyAddress = address; };
    PINLINE const PString & GetProxyUserName()const{ return m_ProxyUserName; };
    PINLINE void SetProxyUserName( const PString & user ){ m_ProxyUserName = user; };
    PINLINE const PString & GetProxyPassword()const{ return m_ProxyPassword; };
    PINLINE void SetProxyPassword( const PString & password ){ m_ProxyPassword = password; };

    PINLINE const PString & GetSIPDomain()const{ return m_SIPDomain; };
    PINLINE void SetSIPDomain( const PString & domain ){ m_SIPDomain = domain; };

    PINLINE const PStringArray & GetMediaFormats()const{ return m_MediaFormats; };

    PINLINE const PString & GetPlayDevice()const{ return m_DefaultAudioPlayDevice; };
    PINLINE const PString & GetRecordDevice()const{ return m_DefaultAudioRecordDevice; };
    PINLINE void SetXORHash( BOOL hash = TRUE ){ m_XORHash = hash; };
    PINLINE BOOL IsXORHashSet()const{ return m_XORHash; };
    PINLINE SoftPhoneInterface * GetSoftPhoneInterface(){ return m_SoftPhoneInterface; };

  protected:

    CallState m_CallState;
    SoftPhonePCSSEndPoint   * m_PCSSEndPoint;
    SoftPhoneSIPEndPoint * m_SIPEndPoint;
    PString m_UserName;
    PString m_DisplayName;
    PString m_RegistrarAddress;
    PString m_RegistrarUserName;
    PString m_RegistrarPassword;
    PString m_ProxyAddress;
    PString m_ProxyUserName;
    PString m_ProxyPassword;
    PString m_SIPDomain;
    SoftPhoneMediaList m_MediaInfo;
    PString m_CurrentConnectionToken;
    PString m_CurrentCallToken;
    BOOL m_IsAutoAnswer;
    BOOL m_VideoGrabPreview;
    PStringArray m_MediaFormats;
    int m_RegistrarTTL; 
    PString m_DefaultAudioPlayDevice;
    PString m_DefaultAudioRecordDevice;
    BOOL m_IsInitialized;
    BOOL m_XORHash;

    SoftPhoneInterface * m_SoftPhoneInterface;
  };

};

#endif
#endif
