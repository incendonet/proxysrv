/*
 *
 * SoftPhoneInterface.cxx
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
 * $Log: SoftPhoneInterface.cxx,v $
 * Revision 1.20  2007/01/14 13:12:57  joegenbaclor
 * Fixed compile warnings
 *
 * Revision 1.19  2007/01/01 05:12:53  joegenbaclor
 * Introduced termiantion flag to avoid crashes on exit.
 * TO DO: Maybe the best thing to do here is to have a WaitForTermination()
 * function exposed for the SIPStack
 *
 * Revision 1.18  2006/12/19 23:38:34  joegenbaclor
 * Fixed bug in softphone proxy authentication where passwords <= 3 in length may not be properly handled
 *
 * Revision 1.17  2006/12/18 10:31:35  joegenbaclor
 * Fixed ACM G.723.1 one way audio bug
 *
 * Revision 1.16  2006/12/17 23:36:04  joegenbaclor
 * Added cleanup method
 *
 * Revision 1.15  2006/12/14 09:49:58  joegenbaclor
 * Tweaked cleanup routines
 *
 * Revision 1.14  2006/12/13 10:01:26  joegenbaclor
 * More work on Softphone
 *
 * Revision 1.13  2006/12/12 23:53:34  joegenbaclor
 * More work on DTMF encoding
 *
 * Revision 1.12  2006/12/12 05:26:10  joegenbaclor
 * Modified DTMF player to use PDTMFEncoder
 *
 * Revision 1.11  2006/12/11 14:35:43  joegenbaclor
 * Allowed setting of empty STUN address
 *
 * Revision 1.10  2006/12/11 10:27:37  joegenbaclor
 * moved config object creation to constructor
 *
 * Revision 1.9  2006/12/08 09:56:13  joegenbaclor
 * Added Hold functions
 *
 * Revision 1.8  2006/12/07 09:12:22  joegenbaclor
 * 1.  Changed registration expire value from milliseconds to seconds
 *
 * Revision 1.7  2006/12/06 10:17:12  joegenbaclor
 * More SF enhancement
 *
 * Revision 1.6  2006/12/06 06:40:56  joegenbaclor
 * STUN related fixes
 *
 * Revision 1.5  2006/12/06 01:59:22  joegenbaclor
 * More Softphone support
 *
 * Revision 1.4  2006/12/04 08:55:32  joegenbaclor
 * Added log hook to transport
 *
 * Revision 1.3  2006/12/04 07:32:50  joegenbaclor
 * Added RFC3680 methods to Opal classes
 *
 * Revision 1.2  2006/12/03 07:32:56  joegenbaclor
 * Softphone is now configured from INI files
 *
 * Revision 1.1  2006/12/02 01:12:13  joegenbaclor
 * Initial upload of SoftPhoneInterface classes
 *
 *
 */


#include "SoftPhoneInterface.h"
#include "SoftPhone.h"
#include "ptclib/dtmf.h"



using namespace SF;
using namespace MS;

#define new PNEW

static SoftPhoneManager * m_SoftPhone = NULL;
static PConfig * m_Config = NULL;
static PMutex m_ConfigMutex;
static PConfig * m_Contacts = NULL;
static PConfig * m_Accounts = NULL;
static PConfig * m_CallHistory = NULL;

static MS::VoiceFile * m_DialToneFile = NULL;
static MS::VoiceFile * m_RingBackToneFile = NULL;
static MS::VoiceFile * m_BusyToneFile = NULL;

SoftPhoneInterface::SoftPhoneInterface()
{
  m_NATType = NAT::SIPSTUNClient::NumNatTypes;
  SIPTransport::AttachWriteNotifier( PCREATE_NOTIFIER( _OnWritePacketLog ) );
  SIPTransport::AttachReadNotifier( PCREATE_NOTIFIER( _OnReadPacketLog ) );

  m_Config = new PConfig( "default.ini", "Account" );
  m_Contacts = new PConfig( "contacts.ini", "Contacts" );
  m_Accounts = new PConfig( "accounts.ini", "Accounts" );
  m_CallHistory = new PConfig( "calls.ini", "Calls" );
  m_SoftPhone = new SoftPhoneManager( this );

  m_PlayBusyTone = FALSE;
  m_PlayDialTone = FALSE;
  m_PlayRingBackTone = FALSE;

  static PDTMFEncoder dt;
  for( PINDEX i = 0; i < 10; i++ )
    dt.GenerateDialTone();

  m_DialToneFile = new MS::VoiceFile( dt, MS::VoiceFile::Wav_8khz_Mono, "TONE_US" );

  static PDTMFEncoder rt;
  rt.GenerateRingBackTone();
  m_RingBackToneFile = new MS::VoiceFile( rt, MS::VoiceFile::Wav_8khz_Mono, "RING_US" );

  static PDTMFEncoder bt;
  bt.GenerateBusyTone();
  m_BusyToneFile = new MS::VoiceFile( bt, MS::VoiceFile::Wav_8khz_Mono, "BUSY_US" );

}

SoftPhoneInterface::~SoftPhoneInterface()
{
  CleanUp();
}

void SoftPhoneInterface::Terminate()
{
  if( m_SoftPhone != NULL )
    m_SoftPhone->Terminate();
}
void SoftPhoneInterface::CleanUp()
{
  if( m_DialToneFile != NULL )
  {
    delete m_DialToneFile;
    m_DialToneFile = NULL;
  }

  if( m_RingBackToneFile != NULL )
  {
    delete m_RingBackToneFile;
    m_RingBackToneFile = NULL;
  }


  if( m_BusyToneFile != NULL )
  {
    delete m_BusyToneFile;
    m_BusyToneFile = NULL;
  }

  if( m_SoftPhone != NULL )
  {
    delete m_SoftPhone;
    m_SoftPhone = NULL;
  }
  
  if( m_Config != NULL )
  {
    delete m_Config;
    m_Config = NULL;
  }

  if( m_Contacts != NULL )
  {
    delete m_Contacts;
    m_Contacts = NULL;
  }

  if( m_Accounts != NULL )
  {
    delete m_Accounts;
    m_Accounts = NULL;
  }

  if( m_CallHistory != NULL )
  {
    delete m_CallHistory;
    m_CallHistory = NULL;
  }
  
}

PConfig * SoftPhoneInterface::GetMainConfig(){ return m_Config; };
PConfig * SoftPhoneInterface::GetContacts(){ return m_Contacts; };
PConfig * SoftPhoneInterface::GetAccounts(){ return m_Accounts; };
PConfig * SoftPhoneInterface::GetCallHistory(){ return m_CallHistory; };
SoftPhoneManager * SoftPhoneInterface::GetPhoneManager(){ return m_SoftPhone; };

/// Initialize SIP and PC Sound Syste,
BOOL SoftPhoneInterface::Initialize()
{
  m_SoftPhone->SetDisplayName( m_Config->GetString( "Account", "AccountName", "" ) );
  
  SIPURI accountAddress( m_Config->GetString( "Account", "AccountAddress", "" ) );
  PString accountHost = accountAddress.GetHost();
  PString userName = accountAddress.GetUser();

  m_SoftPhone->SetSIPDomain( accountHost );
  m_SoftPhone->SetUserName( userName );
  
  SIPURI registrarAddress( m_Config->GetString( "Account", "ServerAddress", "" ) );
  if( accountHost.IsEmpty() && !registrarAddress.GetHost().IsEmpty() )
    m_SoftPhone->SetSIPDomain( registrarAddress.GetHost() );

  m_SoftPhone->SetRegistrarAddress( registrarAddress.AsString() );
  m_SoftPhone->SetProxyAddress( m_Config->GetString( "Account", "OutboundProxy", "" ) );
  m_SoftPhone->SetRegistrarUserName( m_Config->GetString( "Account", "AuthenticationUser", "" ) );
  m_SoftPhone->SetRegistrarPassword( m_SoftPhone->GetRegistrarPassword() );
  m_SoftPhone->SetProxyUserName( m_Config->GetString( "Account", "ProxyAuthenticationUser", "" ) );
  m_SoftPhone->SetProxyPassword( m_SoftPhone->GetProxyPassword() );
  m_SoftPhone->SetRecordDevice( m_Config->GetString( "Audio", "Microphone", PSoundChannel::GetDefaultDevice( PSoundChannel::Recorder ) ) );
  m_SoftPhone->SetPlayDevice( m_Config->GetString( "Audio", "Speaker", PSoundChannel::GetDefaultDevice( PSoundChannel::Player ) ) );
  m_SoftPhone->SetXORHash( m_Config->GetBoolean( "Account", "XORHash", FALSE ) );
  m_SoftPhone->SetRegistrarTTL(  m_Config->GetInteger( "Account", "RegistrarTTL", 3600 ) );

  PStringArray formatMask;

  for( PINDEX i = 0; i < 20; i++ )
  {
    PStringStream codecSlot;
    codecSlot << "Codec" << i;
    PString codec = m_Config->GetString( "Audio", codecSlot, "" );
    if( !codec.IsEmpty() )
      formatMask.AppendString( codec );

  }

  if( formatMask.GetSize() == 0 )
  {

    m_Config->SetString( "Audio", "Codec0", "iLBC-13k3" );
    formatMask.AppendString( "iLBC-13k3" );

    m_Config->SetString( "Audio", "Codec1", "GSM-06.10" );
    formatMask.AppendString( "GSM-06.10" );
  }
    
  /// Initialize OPAL PC Sound System
  m_SoftPhone->Initialise();


  /// Initialize the SIP Stack
  m_SoftPhone->InitializeSIP();

  return TRUE;
}

/// Perform Registration
BOOL SoftPhoneInterface::DoLogin()
{
  return m_SoftPhone->SendRegister();
}

/// Perform unregistration
BOOL SoftPhoneInterface::DoLogout()
{
  return m_SoftPhone->SendUnregister();
}


/// add a contact to the address book
BOOL SoftPhoneInterface::AddContact(
  const PString & displayName, 
  const PString & sipURI, 
  BOOL queryPresence
)
{
  PWaitAndSignal lock( m_ConfigMutex );

  PStringStream record;
  record << "\"" << displayName << "\" "; 
  record << "<" <<  sipURI << ">";
  record << ";query-presence=" << queryPresence;

  PMessageDigest5 digestor;
  PMessageDigest5::Code digest; 
  digestor.Start();
  digestor.Process( displayName );
  digestor.Complete( digest );
  MD5::MD5Hash hashKey( digest );
  
  m_Contacts->SetString( hashKey.AsString(), record );

  return TRUE;
}
  
/// returns a contact by index
BOOL SoftPhoneInterface::GetContact( 
  PINDEX index, 
  PString & displayName, 
  PString & sipURI, 
  BOOL & queryPresence
)const
{
  PWaitAndSignal lock( m_ConfigMutex );

  PStringToString contacts = m_Contacts->GetAllKeyValues();
  PString contact = m_Contacts->GetString( contacts.GetKeyAt( index ), "" );

  if( !contact.IsEmpty() )
  {
    PStringStream record;
    record << "From: " << contact;
    From from = record;

    displayName = from.GetDisplayName();
    SIPParser::ParserTools::UnQuote( displayName );

    sipURI = from.GetURI().AsString( FALSE );

    PString _queryPresence;
    if( from.GetParameter( "query-presence", _queryPresence ) )
      queryPresence = (BOOL)_queryPresence.AsInteger();
  }

  return TRUE;
}

BOOL SoftPhoneInterface::GetContactByName(
  const PString & accountName, 
  PString & contactAddress, 
  BOOL & queryPresence
)const
{
  PWaitAndSignal lock( m_ConfigMutex );

  PMessageDigest5 digestor;
  PMessageDigest5::Code digest; 
  digestor.Start();
  digestor.Process( accountName );
  digestor.Complete( digest );
  MD5::MD5Hash hashKey( digest );

  PStringStream record;
  record << "From: " << m_Contacts->GetString( hashKey.AsString(), "" );
  From from = record;

  contactAddress = from.GetURI().AsString( FALSE );

  PString query;
  if( from.GetParameter( "query-presence", query ) )
    queryPresence = (BOOL)query.AsInteger();

  return TRUE;
}

/// returns the size of the address book
PINDEX SoftPhoneInterface::GetContactSize()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  PStringToString contacts = m_Contacts->GetAllKeyValues();
  return contacts.GetSize();
}

/// deletes a contact by name
BOOL SoftPhoneInterface::DeleteContact( 
  const PString & contact
)
{
  PWaitAndSignal lock( m_ConfigMutex );

  PMessageDigest5 digestor;
  PMessageDigest5::Code digest; 
  digestor.Start();
  digestor.Process( contact );
  digestor.Complete( digest );
  MD5::MD5Hash hashKey( digest );

  m_Contacts->DeleteKey( hashKey.AsString() );

  return TRUE;
}

/// deletes a contact using the index number 
BOOL SoftPhoneInterface::DeleteContactByIndex(
  PINDEX index
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  PStringToString contacts = m_Contacts->GetAllKeyValues();
  m_Contacts->DeleteKey( contacts.GetKeyAt( index ) );
  return TRUE;
}
  
/// returns the name of this account.
/// this is also the displayname sent
PString SoftPhoneInterface::GetAccountName()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  return m_Config->GetString( "Account", "AccountName", "" ); 
}

/// sets the name of this account.
/// this is also the displayname sent
void SoftPhoneInterface::SetAccountName(
  const PString & accountName
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  m_Config->SetString( "Account", "AccountName", accountName );
  m_SoftPhone->SetDisplayName( accountName );
}

/// returns the SIP URI for the softphone
/// this is also the SIP URI the softphone
/// sends for the TO header
PString SoftPhoneInterface::GetAccountAddress()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  return m_Config->GetString( "Account", "AccountAddress", "" ); 
}

/// sets the SIP URI for the softphone
/// this is also the SIP URI the softphone
/// sends for the TO header
void SoftPhoneInterface::SetAccountAddress(
  const PString & accountAddress
)
{
  PWaitAndSignal lock( m_ConfigMutex );

  m_Config->SetString( "Account", "AccountAddress", accountAddress );
  SIPURI uri( accountAddress );
  m_SoftPhone->SetUserName( uri.GetUser() );
  m_SoftPhone->SetSIPDomain( uri.GetHost() );
}

/// returns the address of the Registrar
/// this is the address where registrations 
/// will be sent
PString SoftPhoneInterface::GetSIPServerAddress()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  return m_Config->GetString( "Account", "ServerAddress", "" );
}

/// returns the address of the Registrar
/// this is the address where registrations 
/// will be sent
void SoftPhoneInterface::SetSIPServerAddress( 
  const PString & serverAddress 
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  m_Config->SetString( "Account", "ServerAddress", serverAddress );
  m_SoftPhone->SetRegistrarAddress( serverAddress );
}


/// returns the address for the outbound proxy
PString SoftPhoneInterface::GetOutboundProxy()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  return m_Config->GetString( "Account", "OutboundProxy", "" ); 
}

/// returns the address for the outbound proxy
void SoftPhoneInterface::SetOutboundProxy(
  const PString & proxyAddress
)
{
  m_Config->SetString( "Account", "OutboundProxy", proxyAddress );
  m_SoftPhone->SetProxyAddress( proxyAddress );
}

/// returns the user name sent by the softphone
/// when sending authentication and authorization
/// responses
PString SoftPhoneInterface::GetAuthenticationUser()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  return m_Config->GetString( "Account", "AuthenticationUser", "" ); 
}

/// sets the user name sent by the softphone
/// when sending authentication and authorization
/// responses
void SoftPhoneInterface::SetAuthenticationUser(
  const PString & authUser
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  m_Config->SetString( "Account", "AuthenticationUser", authUser );
  m_SoftPhone->SetRegistrarUserName( authUser );
}

/// returns the password sent by the softphone
/// when sending authentication and authorization
/// responses
PString SoftPhoneInterface::GetAuthenticationPassword()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  PString pwd = PBase64::Decode( m_Config->GetString( "Account", "AuthenticationPassword", "-----" ) ); 
  return pwd.Left( pwd.GetLength() - 5 );
}

/// sets the password sent by the softphone
/// when sending authentication and authorization
/// responses
void SoftPhoneInterface::SetAuthenticationPassword(
  const PString & authPassword
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  m_SoftPhone->SetRegistrarPassword( authPassword );
  PString pwd = PBase64::Encode( authPassword + "-----" );
  m_Config->SetString( "Account", "AuthenticationPassword", pwd );
}

/// returns the user name sent by the softphone
/// when sending authentication and authorization
/// responses
PString SoftPhoneInterface::GetProxyAuthenticationUser()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  return m_Config->GetString( "Account", "ProxyAuthenticationUser", "" ); 
}

/// sets the user name sent by the softphone
/// when sending authentication and authorization
/// responses
void SoftPhoneInterface::SetProxyAuthenticationUser(
  const PString & authUser
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  m_Config->SetString( "Account", "ProxyAuthenticationUser", authUser );
  m_SoftPhone->SetProxyUserName( authUser );
}

/// returns the password sent by the softphone
/// when sending authentication and authorization
/// responses
PString SoftPhoneInterface::GetProxyAuthenticationPassword()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  PString pwd = PBase64::Decode( m_Config->GetString( "Account", "ProxyAuthenticationPassword", "-----" ) ); 
  return pwd.Left( pwd.GetLength() - 5 );
}

/// sets the password sent by the softphone
/// when sending authentication and authorization
/// responses
void SoftPhoneInterface::SetProxyAuthenticationPassword(
  const PString & authPassword
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  m_SoftPhone->SetProxyPassword( authPassword );
  PString pwd = PBase64::Encode( authPassword + "-----" );
  m_Config->SetString( "Account", "ProxyAuthenticationPassword", pwd );
}

/// sets the STUN server to be used for NAT bindings
NAT::SIPSTUNClient::NatTypes SoftPhoneInterface::SetSTUNServer(
  const PString & server,
  WORD udpPortBase,
  WORD udpPortMax,
  WORD rtpPortBase,
  WORD rtpPortMax
)
{
  PWaitAndSignal lock( m_ConfigMutex );

  if( server.IsEmpty() )
  {
    m_Config->SetString( "Account", "STUNServer", "" );
    return m_NATType;
  }
  
  if( m_NATType != NAT::SIPSTUNClient::NumNatTypes && server == GetSTUNServerAddress() )
    return m_NATType;

  PStringStream log;
  log << "----------------" << PTimer::Tick() << "----------------\r\n";
  log << "Querying STUN server at " << server << ".\r\nThis may take a while ... \r\n\r\n";
  Event_WritePacketLog( log ); 
  m_Config->SetString( "Account", "STUNServer", server );
  m_NATType = m_SoftPhone->SetSTUNServer( server, udpPortBase, udpPortMax, rtpPortBase, rtpPortMax );

  return m_NATType;
}

    ///  returns the STUN server address to be used for NAT bindings
PString SoftPhoneInterface::GetSTUNServerAddress()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  return m_Config->GetString( "Account", "STUNServer", "" );
}

/// adds a new account to the account database.
BOOL SoftPhoneInterface::AddAccount(
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
)
{
  PWaitAndSignal lock( m_ConfigMutex );
 
  PStringStream record;
  record << "\"" << accountName << "\" "; 
  record << "<" <<  accountAddress << ">";
  record << ";auth-user=" << authenticationUser;
  record << ";auth-password=" << authenticationPassword;
  record << ";proxy-auth-user=" << proxyAuthenticationUser;
  record << ";proxy-auth-password=" << proxyAuthenticationPassword;
  record << ";sip-server=" << serverAddress;
  record << ";outbound-proxy=" << outboundProxy;
  record << ";is-default-account=" << isDefaultAccount;
  record << ";is-auto-login=" << isAutoLogin;

  PMessageDigest5 digestor;
  PMessageDigest5::Code digest; 
  digestor.Start();
  digestor.Process( accountName + accountAddress );
  digestor.Complete( digest );
  MD5::MD5Hash hashKey( digest );
  PString key = hashKey.AsString();

  if( isDefaultAccount )
  {
    PStringToString accounts = m_Accounts->GetAllKeyValues();
    for( PINDEX i = 0; i < accounts.GetSize(); i++ )
    {
      PString key = accounts.GetKeyAt( i );
      PStringStream account;
      account << "From: ";
      account << accounts.GetDataAt( i );
      From from = account;
      from.SetParameter( "is-default-account" , "0" );
      m_Accounts->SetString( key, from.GetHeaderBody() );
    }
  }

  m_Accounts->SetString( key, record );

  return TRUE;
}


/// returns the size of the account database
PINDEX SoftPhoneInterface::GetAccountSize()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  PStringToString accounts = m_Accounts->GetAllKeyValues();
  return accounts.GetSize();
}

/// returns an account using its index in the 
/// account database
BOOL SoftPhoneInterface::GetAccountByIndex(
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
)const
{
  PWaitAndSignal lock( m_ConfigMutex );
  PStringToString accounts = m_Accounts->GetAllKeyValues();
  key = accounts.GetKeyAt( index );
  return GetAccountByKey( 
    key, 
    accountName, 
    accountAddress, 
    serverAddress, 
    outboundProxy, 
    authenticationUser, 
    authenticationPassword, 
    proxyAuthenticationUser, 
	  proxyAuthenticationPassword,
    isDefaultAccount, 
    isAutoLogin );
}

/// returns an account using its key in the 
/// account database which might have been known
/// by the application from previous call to GetAccountByIndex
BOOL SoftPhoneInterface::GetAccountByKey(
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
)const
{
  PWaitAndSignal lock( m_ConfigMutex );
  PString account = m_Accounts->GetString( key, "" );

  if( !account.IsEmpty() )
  {
    PStringStream record;
    record << "From: " << account;
    From from = record;

    accountName = from.GetDisplayName();
    accountName = ParserTools::UnQuote( accountName );
    accountAddress = from.GetURI().AsString();
    from.GetParameter( "auth-user", authenticationUser );
    from.GetParameter( "auth-password", authenticationPassword );
    from.GetParameter( "proxy-auth-user", proxyAuthenticationUser );
    from.GetParameter( "proxy-auth-password", proxyAuthenticationPassword );
    from.GetParameter( "sip-server", serverAddress );
    from.GetParameter( "outbound-proxy", outboundProxy );

    PString defAccount;
    from.GetParameter( "is-default-account", defAccount );
    isDefaultAccount = (BOOL)defAccount.AsInteger();

    PString isAuto;
    from.GetParameter( "is-auto-login", isAuto );
    isAutoLogin = (BOOL)isAuto.AsInteger();
    
    return TRUE;
  }

  return FALSE;
  
}

/// deletes an account using its key in the 
/// account database which might have been known
/// by the application from previous call to GetAccountByIndex
BOOL SoftPhoneInterface::DeleteAccountByKey(
  const PString & key
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  m_Accounts->DeleteKey( key );
  return TRUE;
}

/// deletes an account using its index in the 
/// account database
BOOL SoftPhoneInterface::DeleteAccountByIndex(
  PINDEX index
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  PStringToString accounts = m_Accounts->GetAllKeyValues();
  PString key = accounts.GetKeyAt( index );
  return DeleteAccountByKey( key );
}


/// returns the detected record devices in a string array
/// (microphones)
PStringList SoftPhoneInterface::PopAvailableRecordDevices()const
{
  return PSoundChannel::GetDeviceNames( PSoundChannel::Recorder );
}

/// returns the detected play devices in a string array
/// (speakers)
PStringList SoftPhoneInterface::PopAvailablePlayDevices()const
{
  return PSoundChannel::GetDeviceNames( PSoundChannel::Player );
}

/// returns the detected video input devices in a string array
/// (video camera)
PStringList SoftPhoneInterface::PopAvailableVideoInputDevices()const
{
  return PVideoInputDevice::GetDriversDeviceNames("*");
}

/// returns the available codecs in a string array
PStringArray SoftPhoneInterface::PopAvailableAudioCodecs()
{
  PStringArray formats = m_SoftPhone->GetMediaFormats();;
  PStringArray final;

  for( PINDEX i = 0; i < formats.GetSize(); i++ )
  {
    if( formats[i].Find( "H.26" ) == P_MAX_INDEX )
      final.AppendString( formats[i] );
  }

  return final;
}

PStringToString SoftPhoneInterface::PopCallHistory(PINDEX count)
{
  PWaitAndSignal lock( m_ConfigMutex );

  PStringToString history;

  if( m_CallHistory == NULL )
    return history;

  PStringToString keys = m_CallHistory->GetAllKeyValues();
  
  count = count > keys.GetSize() ? keys.GetSize() : count;
  PINDEX j = keys.GetSize() - 1;
  for( PINDEX i = count; i > 0; i-- )
  {
    PString time = keys.GetKeyAt( j );
    PString uri = keys.GetDataAt( j-- );
    history.SetAt( time, uri );
  }

  return history;
}


/// returns the name of the active microphone
/// device 
PString SoftPhoneInterface::GetDefaultMicrophone()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  return m_Config->GetString( "Audio", "Microphone", PSoundChannel::GetDefaultDevice( PSoundChannel::Recorder ) ); 
}

/// sets the name of the active microphone
/// device 
void SoftPhoneInterface::SetDefaultMicrophone(
  const PString & deviceName
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  m_Config->SetString( "Audio", "Microphone", deviceName );
  m_SoftPhone->SetRecordDevice( deviceName );
  /// Reinitialize the PC Sound System
  m_SoftPhone->Initialise();
}

/// returns the name of the active speaker device
PString SoftPhoneInterface::GetDefaultSpeaker()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  return m_Config->GetString( "Audio", "Speaker", PSoundChannel::GetDefaultDevice( PSoundChannel::Player ) ); 
}

void SoftPhoneInterface::SetDefaultSpeaker(
  const PString & deviceName
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  m_Config->SetString( "Audio", "Speaker", deviceName );
  m_SoftPhone->SetPlayDevice( deviceName );
  /// Reinitialize the PC Sound System
  m_SoftPhone->Initialise();
}

void SoftPhoneInterface::SetVolume( 
      int volume,
      BOOL isPlayerDevice
)
{
  

  if( volume < 0 || volume > 100 )
    volume = 60;
  
  if( isPlayerDevice )
  {
    PString device = GetDefaultSpeaker();
    PSoundChannel soundChannel( device, PSoundChannel::Player );
    soundChannel.SetVolume( volume );
  }else
  {
    PString device = GetDefaultMicrophone();
    PSoundChannel soundChannel( device, PSoundChannel::Recorder );
    soundChannel.SetVolume( volume );
  }
}


/// returns the codec based on its slot number.
/// slot == 1 means highest priority in the codec list 
PString SoftPhoneInterface::GetAudioCodec( 
  PINDEX slot
)const
{
  PWaitAndSignal lock( m_ConfigMutex );
  PStringStream  codecSlot;
  codecSlot << "Codec" << slot;
  return m_Config->GetString( "Audio", codecSlot, "" ); 
}

/// sets the codec based on its slot number.
/// slot == 1 means highest priority in the codec list. 
void SoftPhoneInterface::SetAudioCodec(
  PINDEX slot,
  const PString & codecName
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  PStringStream  codecSlot;
  codecSlot << "Codec" << slot;
  m_Config->SetString( "Audio", codecSlot, codecName );
}


/// plays an audible dtmf tone
/// take not that this will not send a DTMF tone over
/// the wire.  Use SendRFC2833Tone instead.
void SoftPhoneInterface::PlayDTMF(
  int tone
)
{
  switch( tone )
  {
    case 0:
      MS::DTMFTone_0.PlaySound();
      break;
    case 1:
      MS::DTMFTone_1.PlaySound();
      break;
    case 2:
      MS::DTMFTone_2.PlaySound();
      break;
    case 3:
      MS::DTMFTone_3.PlaySound();
      break;
    case 4:
      MS::DTMFTone_4.PlaySound();
      break;
    case 5:
      MS::DTMFTone_5.PlaySound();
      break;
    case 6:
      MS::DTMFTone_6.PlaySound();
      break;
    case 7:
      MS::DTMFTone_7.PlaySound();
      break;
    case 8:
      MS::DTMFTone_8.PlaySound();
      break;
    case 9:
      MS::DTMFTone_9.PlaySound();
      break;
    case 10:
      MS::DTMFTone_10.PlaySound();
      break;
    case 11:
      MS::DTMFTone_11.PlaySound();
      break;
  }
}

/// plays an audible dtmf tone
/// take not that this will not send a DTMF tone over
/// the wire.  Use SendRFC2833Tone instead.
void SoftPhoneInterface::PlayDTMF(
  const PString & tone
)
{
  for( PINDEX i = 0; i < tone.GetLength(); i++ )
  {
    char c = tone[i];
    switch( c )
    {

      case '0':
        MS::DTMFTone_0.PlaySound();
        break;
      case '1':
        MS::DTMFTone_1.PlaySound();
        break;
      case '2':
        MS::DTMFTone_2.PlaySound();
        break;
      case '3':
        MS::DTMFTone_3.PlaySound();
        break;
      case '4':
        MS::DTMFTone_4.PlaySound();
        break;
      case '5':
        MS::DTMFTone_5.PlaySound();
        break;
      case '6':
        MS::DTMFTone_6.PlaySound();
        break;
      case '7':
        MS::DTMFTone_7.PlaySound();
        break;
      case '8':
        MS::DTMFTone_8.PlaySound();
        break;
      case '9':
        MS::DTMFTone_9.PlaySound();
        break;
      case '*':
        MS::DTMFTone_10.PlaySound();
        break;
      case '#':
        MS::DTMFTone_11.PlaySound();
        break;
    }
  }
}

/// makes an outbound call to this address.
/// address should contain a valid SIP URI
void SoftPhoneInterface::MakeCall( 
  const PString & address
)
{
  PString dest = address;

  if (address.IsEmpty())
    return;

  //check if this it has a host part
  //if not append a domain
  if( address.Find( "@" ) == P_MAX_INDEX )
  {
    
    PRegularExpression regex("[0-9]*[.][0-9]*[.][0-9]*[.][0-9]*");
    PINDEX pos,len;
    if( !address.FindRegEx(regex, pos, len)  ) 
    {
      /// not in ip address notation
      SIPURI uriAddress( address );
      PIPSocket::Address ip;
      WORD port = 0;
      if( ParserTools::IsNumeric( address ) || !SIPTransport::Resolve( uriAddress, ip, port ) )
      {
        /// not an ip address
        /// must be a telephone number
        /// or an alias

        
        if( !m_SoftPhone->GetSIPDomain().IsEmpty() )
        {
          SIPURI destURI = m_SoftPhone->GetSIPDomain();
          destURI.SetUser( address );
          dest = destURI.AsString();
        }else if( !m_SoftPhone->GetRegistrarAddress().IsEmpty() )
        {
          /// get the domain part of the registrar

          SIPURI destURI = m_SoftPhone->GetRegistrarAddress();
          destURI.SetUser( address );
          dest = destURI.AsString();
        }else if( !m_SoftPhone->GetProxyAddress().IsEmpty() )
        {
          SIPURI destURI = m_SoftPhone->GetProxyAddress();
          destURI.SetUser( address );
          dest = destURI.AsString();
        }
      }
    }
  }

  if( dest.ToLower().Left( 4 ) != "sip:" )
    dest = "sip:" + address;

  PTime now;
  To to;
  to.SetURI( dest );
  m_CallHistory->SetString( now.AsString(), to.AsString() );

  PStringArray mediaFormats = m_SoftPhone->GetMediaFormats();
  PStringArray mediaFormatOrder;

  for( PINDEX i = 0; i < 20; i++ )
    if( !GetAudioCodec(i).IsEmpty() )
      mediaFormatOrder.AppendString( GetAudioCodec(i) );

  m_SoftPhone->SetMediaFormatOrder( mediaFormatOrder );

  PStringArray mediaFormatMask;

  for( PINDEX i = 0; i < mediaFormats.GetSize(); i++ )
  {
    BOOL listed = FALSE;
    for( PINDEX j = 0; j < 20; j++ )
      if( mediaFormats[i] == GetAudioCodec(j) )
        listed = TRUE;

    if( !listed )
      mediaFormatMask.AppendString( mediaFormats[i] );
  }

  m_SoftPhone->SetMediaFormatMask( mediaFormatMask );

  m_SoftPhone->InitializeSIP();
  m_SoftPhone->MakeCall( address );
}

/// hangup the current call
void SoftPhoneInterface::HangupCall()
{
  m_SoftPhone->HangUpCall();
}

/// answers the current call
void SoftPhoneInterface::AnswerCall()
{
  m_SoftPhone->AnswerCall();
}

/// Sends a plain text MESSAGE to remoteAddress.
/// remoteAddress should be a valid SIP URI.
void SoftPhoneInterface::SendInstantMessage(
  const PString & remoteAddress, 
  const PString & message
)
{
  m_SoftPhone->InitializeSIP();
  m_SoftPhone->SendInstantMessage( remoteAddress, message );
}

/// Returns the call history inside a string array.
/// count contains the number of calls to be returned
PStringToString SoftPhoneInterface::GetCallHistory( 
  PINDEX count 
)const
{
  PWaitAndSignal lock( m_ConfigMutex );
  PStringToString history;
  PStringToString keys = m_CallHistory->GetAllKeyValues();

  count = count > keys.GetSize() ? keys.GetSize() : count;
  PINDEX j = keys.GetSize() - 1;

  for( PINDEX i = count; i > 0; i-- )
  {
    PStringStream strm;
    history.SetAt( keys.GetKeyAt( j ),  keys.GetDataAt( j ) );
    j--;
  }

  return history;
}


/// Sends an RFC 2833 DTMF tone over the wire
void SoftPhoneInterface::SendRFC2833Tone(
  const PString & userInput, 
  int duration
)
{

  if( duration < 100 )
    duration = 100;

  for( PINDEX i = 0; i < userInput.GetLength(); i++ )
  {
    if( userInput[i] == ',' )
    {
      PThread::Sleep( 100 );
      continue;
    }

    m_SoftPhone->SendUserInput( userInput[i], duration, TRUE );
    PThread::Sleep( duration );
  }
}

/// Sends a SIP INFO DTMF tone over the wire
void SoftPhoneInterface::SendINFOTone(
  const PString & userInput, 
  int duration
)
{
  if( duration < 100 )
    duration = 100;

  for( PINDEX i = 0; i < userInput.GetLength(); i++ )
  {
    if( userInput[i] == ',' )
    {
      PThread::Sleep( 100 );
      continue;
    }

    m_SoftPhone->SendUserInput( userInput[i], duration, FALSE );
    PThread::Sleep( duration );
  }
}

/// sends a DTMF tone.  Default behavior uses RFC2833
void SoftPhoneInterface::SendDTMF(
  const PString & tone, 
  int duration
)
{
  if( IsRFC2833Enabled() )
    SendRFC2833Tone( tone, duration );
  else
    SendINFOTone( tone, duration );
}



/// sets RFC 2833 as the default method for sending DTMF
void SoftPhoneInterface::SetEnableRFC2833( BOOL enable )
{
  PWaitAndSignal lock( m_ConfigMutex );
  m_Config->SetBoolean( "Audio", "DTMFAsRFC2833", enable );
}

/// check if RFC 2833 is the default method for sending DTMF
BOOL SoftPhoneInterface::IsRFC2833Enabled()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  return m_Config->GetBoolean( "Audio", "DTMFAsRFC2833", TRUE );
}


/// enable hashing of RTP and SIP over the wire
/// take note that this is a none standard mechanism.
/// this will not work for servers other than opensbc
void SoftPhoneInterface::SetEnableXORHash( BOOL enable )
{
  PWaitAndSignal lock( m_ConfigMutex );
  m_Config->SetBoolean( "Account", "XORHash", enable );
  m_SoftPhone->SetXORHash( enable );
}

/// check if xor hashing for RTP and SIP is enabled
BOOL SoftPhoneInterface::IsXOREnabled()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  return m_Config->GetBoolean( "Account", "XORHash", FALSE );
}

/// Set the maximum registration keep-alive interval
void SoftPhoneInterface::SetRegistrarTTL( 
  const PTimeInterval & ttl 
)
{
  PWaitAndSignal lock( m_ConfigMutex );
  m_Config->SetInteger( "Account", "RegistrarTTL", ttl.GetSeconds() );
  m_SoftPhone->SetRegistrarTTL( ttl.GetSeconds() );
}

/// Returns the maximum registration keep alive interval
PTimeInterval SoftPhoneInterface::GetRegistrarTTL()const
{
  PWaitAndSignal lock( m_ConfigMutex );
  int seconds = m_Config->GetInteger( "Account", "RegistrarTTL", 3600 );
  PTimeInterval ttl( 0, seconds );
  return  ttl;
}



void SoftPhoneInterface::WatchRegistrationState(
  const PString & stateAgent,
  const PString & resourceURI,
  const PTimeInterval & expires,
  const PString & authUser,
  const PString & authPassword
  
)
{
  m_SoftPhone->WatchRegistrationState( stateAgent, resourceURI, expires, authUser, authPassword );
}

void SoftPhoneInterface::StopWatchingRegistrationState(
  const PString & resourceURI
)
{
  m_SoftPhone->StopWatchingRegistrationState( resourceURI );
}

void SoftPhoneInterface::_OnWritePacketLog( PStringStream & _log, INT )
{
  if( _log.IsEmpty() )
    return;

  PStringStream log;
  log << "----------------" << PTimer::Tick() << "----------------\r\n" << _log;
  Event_WritePacketLog( log );
}

void SoftPhoneInterface::_OnReadPacketLog( PStringStream & _log, INT )
{
  if( _log.IsEmpty() )
    return;

  PStringStream log;
  log << "----------------" << PTimer::Tick() << "----------------\r\n" << _log;
  Event_ReadPacketLog( log );
}

int SoftPhoneInterface::GetState()const
{ 
  return const_cast<SoftPhoneInterface&>(*this).GetPhoneManager()->GetState(); 
}

/// plays an audible busy sound
void SoftPhoneInterface::PlayBusyTone( PThread &, INT )
{
  while( m_PlayBusyTone )
    m_BusyToneFile->PlaySound();
}

void SoftPhoneInterface::PlayBusyTone()
{
  if( !m_PlayBusyTone )
  {
    m_PlayBusyTone = TRUE;
    PThread::Create( PCREATE_NOTIFIER( PlayBusyTone ) );
  }
}

void SoftPhoneInterface::StopBusyTone()
{
  m_PlayBusyTone = FALSE;

  if( m_BusyToneFile != NULL )
    m_BusyToneFile->StopPlaying();
}

/// plays an busy dialtone sound
void SoftPhoneInterface::PlayDialTone( PThread &, INT )
{
  m_DialToneFile->StopPlaying();
  while( m_PlayDialTone )
    m_DialToneFile->PlaySound();
}

void SoftPhoneInterface::PlayDialTone()
{
  if( !m_PlayDialTone )
  {
    m_PlayDialTone = TRUE;
    PThread::Create( PCREATE_NOTIFIER( PlayDialTone ) );
  }
}

void SoftPhoneInterface::StopDialTone()
{
  m_PlayDialTone = FALSE;
  if( m_DialToneFile != NULL )
    m_DialToneFile->StopPlaying();
}

/// plays audible ringback tone
void SoftPhoneInterface::PlayRingBackTone( PThread &, INT )
{
  while( m_PlayRingBackTone )
    m_RingBackToneFile->PlaySound();

}

void SoftPhoneInterface::PlayRingBackTone()
{
  if( !m_PlayRingBackTone )
  {
    m_PlayRingBackTone = TRUE;
    PThread::Create( PCREATE_NOTIFIER( PlayRingBackTone ) );
  }
}

void SoftPhoneInterface::StopRingBackTone()
{
  m_PlayRingBackTone = FALSE;
  if( m_RingBackToneFile != NULL )
    m_RingBackToneFile->StopPlaying();
}

