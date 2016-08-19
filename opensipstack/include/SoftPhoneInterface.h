/*
 *
 * SoftPhoneInterface.h
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
 * $Log: SoftPhoneInterface.h,v $
 * Revision 1.13  2007/01/01 05:12:52  joegenbaclor
 * Introduced termiantion flag to avoid crashes on exit.
 * TO DO: Maybe the best thing to do here is to have a WaitForTermination()
 * function exposed for the SIPStack
 *
 * Revision 1.12  2006/12/19 23:38:31  joegenbaclor
 * Fixed bug in softphone proxy authentication where passwords <= 3 in length may not be properly handled
 *
 * Revision 1.11  2006/12/14 09:49:56  joegenbaclor
 * Tweaked cleanup routines
 *
 * Revision 1.10  2006/12/12 23:53:32  joegenbaclor
 * More work on DTMF encoding
 *
 * Revision 1.9  2006/12/12 05:26:08  joegenbaclor
 * Modified DTMF player to use PDTMFEncoder
 *
 * Revision 1.8  2006/12/08 09:56:13  joegenbaclor
 * Added Hold functions
 *
 * Revision 1.7  2006/12/07 09:12:21  joegenbaclor
 * 1.  Changed registration expire value from milliseconds to seconds
 *
 * Revision 1.6  2006/12/06 10:17:11  joegenbaclor
 * More SF enhancement
 *
 * Revision 1.5  2006/12/06 06:40:54  joegenbaclor
 * STUN related fixes
 *
 * Revision 1.4  2006/12/06 01:59:22  joegenbaclor
 * More Softphone support
 *
 * Revision 1.3  2006/12/04 08:55:32  joegenbaclor
 * Added log hook to transport
 *
 * Revision 1.2  2006/12/04 07:32:49  joegenbaclor
 * Added RFC3680 methods to Opal classes
 *
 * Revision 1.1  2006/12/02 01:12:13  joegenbaclor
 * Initial upload of SoftPhoneInterface classes
 *
 *
 */


#ifndef SOFTPHONEINTERFACE_H
#define SOFTPHONEINTERFACE_H

#include <ptlib.h>
#include "SIPNATMethods.h"

namespace SF
{
  class SoftPhoneManager;

  class SoftPhoneInterface
  {
  public:
    SoftPhoneInterface();

    virtual ~SoftPhoneInterface();

    virtual void CleanUp();

    void Terminate();

    /// pure virtuals
    virtual void  Event_PCSoundSystemInitialized( 
      const PString & recordDevice,
      const PString & soundDevice
    ) = 0;

    virtual void  Event_SIPInitialized(
      const PString & eventInfo = PString::Empty()
    ) = 0;
    
    virtual void  Event_SIPInitializationError( 
      int errorCode, 
      const PString & errorInfo 
    ) = 0;

    virtual void  Event_OutgoingCallRejected( 
      int errorCode, 
      const PString & errorInfo 
    ) = 0;

    virtual void  Event_IncomingCallConnected( 
      const PString & eventInfo  = PString::Empty()
    ) = 0;
    
    virtual void  Event_CallDisconnected(
      const PString & eventInfo  = PString::Empty()
    ) = 0;
    
    virtual void  Event_LoginError( 
      int errorCode, 
      const PString & errorInfo 
    ) = 0;

    virtual void  Event_LoginSuccessful(
      const PString & eventInfo  = PString::Empty()
    ) = 0;
    
    virtual void  Event_LogoutSuccessful(
      const PString & eventInfo  = PString::Empty()
    ) = 0;

    virtual void  Event_OutgoingCallConnected( 
      const PString & eventInfo  = PString::Empty()
    ) = 0;
    
    virtual void  Event_IncomingCall( 
      const PString & eventInfo  = PString::Empty()
    ) = 0;

    virtual void  Event_OutgoingCallRinging( 
      const PString & eventInfo  = PString::Empty()
    ) = 0;

    virtual void  Event_IncomingInstantMessage( 
      const PString & senderInfo,
      const PString & contentType,
      const PString & messageBody
    ) = 0;

    virtual void Event_RegEventStateOnline(
      const PString & user,
      const PString & domain
    ) = 0;

    virtual void Event_RegEventStateOffLine(
      const PString & user,
      const PString & domain
    ) = 0;

    virtual void Event_WritePacketLog(
      const PString & log
    ) = 0;

    virtual void Event_ReadPacketLog(
      const PString & log
    ) = 0;

    /// Initialize SIP and PC Sound Syste,
    BOOL Initialize();

    /// Perform Registration
    BOOL DoLogin();

    /// Perform unregistration
    BOOL DoLogout();


    /// add a contact to the address book
    BOOL AddContact(
	    const PString & displayName, 
	    const PString & sipURI, 
          BOOL queryPresence
    );
      
    /// returns a contact by index
    BOOL GetContact( 
      PINDEX index, 
      PString & accountName, 
      PString & sipURI, 
      BOOL & queryPresence
    )const;

    BOOL GetContactByName(
	    const PString & accountName, 
          PString & contactAddress, 
          BOOL & queryPresence
    )const;

    /// returns the size of the address book
    PINDEX GetContactSize()const;

    /// deletes a contact by name
    BOOL DeleteContact( 
	    const PString & contact
    );

    /// deletes a contact using the index number 
    BOOL DeleteContactByIndex(
	    PINDEX index
    );
      
    /// returns the name of this account.
    /// this is also the displayname sent
    PString GetAccountName()const;

    /// sets the name of this account.
    /// this is also the displayname sent
    void SetAccountName(
	    const PString & accountName
    );

    /// returns the SIP URI for the softphone
    /// this is also the SIP URI the softphone
    /// sends for the TO header
    PString GetAccountAddress()const;

    /// sets the SIP URI for the softphone
    /// this is also the SIP URI the softphone
    /// sends for the TO header
    void SetAccountAddress(
	    const PString & accountAddress
    );

    /// returns the address of the Registrar
    /// this is the address where registrations 
    /// will be sent
    PString GetSIPServerAddress()const;

    /// returns the address of the Registrar
    /// this is the address where registrations 
    /// will be sent
    void SetSIPServerAddress(
      const PString & severAddress
    );


    /// returns the address for the outbound proxy
    PString GetOutboundProxy()const;

    /// returns the address for the outbound proxy
    void SetOutboundProxy(
	    const PString & proxyAddress
    );

    /// returns the user name sent by the softphone
    /// when sending authentication and authorization
    /// responses
    PString GetAuthenticationUser()const;

    /// sets the user name sent by the softphone
    /// when sending authentication and authorization
    /// responses
    void SetAuthenticationUser(
	    const PString & authUser
    );

    /// returns the password sent by the softphone
    /// when sending authentication and authorization
    /// responses
    PString GetAuthenticationPassword()const;

    /// sets the password sent by the softphone
    /// when sending authentication and authorization
    /// responses
    void SetAuthenticationPassword(
	    const PString & authPassword
    );

    /// returns the user name sent by the softphone
    /// when sending authentication and authorization
    /// responses
    PString GetProxyAuthenticationUser()const;

    /// sets the user name sent by the softphone
    /// when sending authentication and authorization
    /// responses
    void SetProxyAuthenticationUser(
	    const PString & authUser
    );

    /// returns the password sent by the softphone
    /// when sending authentication and authorization
    /// responses
    PString GetProxyAuthenticationPassword()const;

    /// sets the password sent by the softphone
    /// when sending authentication and authorization
    /// responses
    void SetProxyAuthenticationPassword(
	    const PString & authPassword
    );


    /// adds a new account to the account database.
    BOOL AddAccount(
	    const PString & accountName,
      const PString & accountAddress,
	    const PString & authenticationUser, 
	    const PString & authenticationPassword, 
      const PString & proxyAuthenticationUser, 
	    const PString & proxyAuthenticationPassword, 
	    const PString & serverAddress,
      const PString & outboundProxy, 
	    BOOL isDefaultAccount, 
	    BOOL isAutoLogin
    );


    /// returns the size of the account database
    PINDEX GetAccountSize()const;

    /// returns an account using its index in the 
    /// account database
    BOOL GetAccountByIndex(
	    PINDEX index,
	    PString & key, 
	    PString & accountName, 
	    PString & accountAddress, 
	    PString & serverAddress, 
	    PString & outboundProxy, 
	    PString & authenticationUser, 
	    PString & authenticationPassword, 
      PString & proxyAuthenticationUser, 
	    PString & proxyAuthenticationPassword, 
	    BOOL & isDefaultAccount, 
	    BOOL & isAutoLogin
    )const;

    /// returns an account using its key in the 
    /// account database which might have been known
    /// by the application from previous call to GetAccountByIndex
    BOOL GetAccountByKey(
	    const PString & key, 
	    PString & accountName, 
	    PString & accountAddress, 
	    PString & serverAddress, 
	    PString & outboundProxy, 
	    PString & authenticationUser, 
	    PString & authenticationPassword, 
      PString & proxyAuthenticationUser, 
	    PString & proxyAuthenticationPassword, 
	    BOOL & isDefaultAccount, 
	    BOOL & isAutoLogin
    )const;

    /// deletes an account using its key in the 
    /// account database which might have been known
    /// by the application from previous call to GetAccountByIndex
    BOOL DeleteAccountByKey(
	    const PString & key
    );

    /// deletes an account using its index in the 
    /// account database
    BOOL DeleteAccountByIndex(
	    PINDEX index
    );


    /// returns the detected record devices in a string array
    /// (microphones)
    PStringList PopAvailableRecordDevices()const;

    /// returns the detected play devices in a string array
    /// (speakers)
    PStringList PopAvailablePlayDevices()const;

    /// returns the detected video input devices in a string array
    /// (video camera)
    PStringList PopAvailableVideoInputDevices()const;

    /// returns the available codecs in a string array
    PStringArray PopAvailableAudioCodecs(void);

    /// returns the entries in the call history database
    PStringToString PopCallHistory( PINDEX count );

    /// returns the name of the active microphone
    /// device 
    PString GetDefaultMicrophone()const;

    /// sets the name of the active microphone
    /// device 
    void SetDefaultMicrophone(
	    const PString & deviceName
    );

    /// returns the name of the active speaker device
    PString GetDefaultSpeaker()const;

    void SetDefaultSpeaker(
	    const PString & deviceName
    );

    /// set the default speaker/microphone volume
    void SetVolume( 
      int volume,
      BOOL isPlayerDevice = TRUE
    );

    /// returns the codec based on its slot number.
    /// slot == 1 means highest priority in the codec list 
    PString GetAudioCodec( 
	    PINDEX slot
    )const;

    /// sets the codec based on its slot number.
    /// slot == 1 means highest priority in the codec list. 
    void SetAudioCodec(
	    PINDEX slot,
	    const PString & codecName
    );


    /// plays an audible dtmf tone
    /// take not that this will not send a DTMF tone over
    /// the wire.  Use SendRFC2833Tone instead.
    void PlayDTMF(
	    int tone
    );

    /// plays an audible dtmf tone
    /// take not that this will not send a DTMF tone over
    /// the wire.  Use SendRFC2833Tone instead.
    void PlayDTMF(
	    const PString & tone
    );

    /// sets the STUN server to be used for NAT bindings
    NAT::SIPSTUNClient::NatTypes SetSTUNServer(
      const PString & server,
      WORD udpPortBase = 10000,
      WORD udpPortMax = 20000,
      WORD rtpPortBase = 30000,
      WORD rtpPortMax = 40000
    );

    ///  returns the STUN server address to be used for NAT bindings
    PString GetSTUNServerAddress()const;

    /// makes an outbound call to this address.
    /// address should contain a valid SIP URI
    void MakeCall( 
	    const PString & address
    );

    /// hangup the current call
    void HangupCall();

    /// answers the current call
    void AnswerCall();

    /// Sends a plain text MESSAGE to remoteAddress.
    /// remoteAddress should be a valid SIP URI.
    void SendInstantMessage(
	    const PString & remoteAddress, 
          const PString & message
    );

    /// Returns the call history inside a string array.
    /// count contains the number of calls to be returned
    PStringToString GetCallHistory( 
      PINDEX count 
    )const;


    /// Sends an RFC 2833 DTMF tone over the wire
    void SendRFC2833Tone(
      const PString & tone, 
      int duration
    );

    /// Sends a SIP INFO DTMF tone over the wire
    void SendINFOTone(
      const PString & tone, 
      int duration
    );

    /// sends a DTMF tone.  Default behavior uses RFC2833
    void SendDTMF(
      const PString & tone, 
      int duration = 180
    );

    /// sets RFC 2833 as the default method for sending DTMF
    void SetEnableRFC2833( BOOL enable = TRUE );

    /// check if RFC 2833 is the default method for sending DTMF
    BOOL IsRFC2833Enabled()const;


    /// enable hashing of RTP and SIP over the wire
    /// take note that this is a none standard mechanism.
    /// this will not work for servers other than opensbc
    void SetEnableXORHash( BOOL enable = TRUE );

    /// check if xor hashing for RTP and SIP is enabled
    BOOL IsXOREnabled()const;

    /// Set the maximum registration keep-alive interval
    void SetRegistrarTTL( 
      const PTimeInterval & ttl 
    );

    /// Returns the maximum registration keep alive interval
    PTimeInterval GetRegistrarTTL()const;


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


    /// returns the current signaling state of the softphone
    int GetState()const;

    PConfig * GetMainConfig();
    PConfig * GetContacts();
    PConfig * GetAccounts();
    PConfig * GetCallHistory();
    SoftPhoneManager * GetPhoneManager();

    NAT::SIPSTUNClient::NatTypes m_NATType;
    
    PDECLARE_NOTIFIER( PStringStream, SoftPhoneInterface, _OnWritePacketLog );
    PDECLARE_NOTIFIER( PStringStream, SoftPhoneInterface, _OnReadPacketLog );

    /// plays an audible busy sound
    void PlayBusyTone();
    void StopBusyTone();
    BOOL m_PlayBusyTone;
    PDECLARE_NOTIFIER( PThread, SoftPhoneInterface, PlayBusyTone );

    /// plays an busy dialtone sound
    void PlayDialTone();
    void StopDialTone();
    BOOL m_PlayDialTone;
    PDECLARE_NOTIFIER( PThread, SoftPhoneInterface, PlayDialTone );

    /// plays audible ringback tone
    void PlayRingBackTone();
    void StopRingBackTone();
    BOOL m_PlayRingBackTone;
    PDECLARE_NOTIFIER( PThread, SoftPhoneInterface, PlayRingBackTone );

  };
};

#endif

