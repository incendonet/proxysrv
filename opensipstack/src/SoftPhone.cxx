/*
 *
 * SoftPhone.cpp
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
 * $Log: SoftPhone.cxx,v $
 * Revision 1.14  2007/01/14 13:12:57  joegenbaclor
 * Fixed compile warnings
 *
 * Revision 1.13  2007/01/01 05:12:53  joegenbaclor
 * Introduced termiantion flag to avoid crashes on exit.
 * TO DO: Maybe the best thing to do here is to have a WaitForTermination()
 * function exposed for the SIPStack
 *
 * Revision 1.12  2006/12/19 23:38:34  joegenbaclor
 * Fixed bug in softphone proxy authentication where passwords <= 3 in length may not be properly handled
 *
 * Revision 1.11  2006/12/18 10:31:35  joegenbaclor
 * Fixed ACM G.723.1 one way audio bug
 *
 * Revision 1.10  2006/12/17 23:36:04  joegenbaclor
 * Added cleanup method
 *
 * Revision 1.9  2006/12/14 09:49:58  joegenbaclor
 * Tweaked cleanup routines
 *
 * Revision 1.8  2006/12/07 09:12:22  joegenbaclor
 * 1.  Changed registration expire value from milliseconds to seconds
 *
 * Revision 1.7  2006/12/06 06:40:56  joegenbaclor
 * STUN related fixes
 *
 * Revision 1.6  2006/12/06 01:59:22  joegenbaclor
 * More Softphone support
 *
 * Revision 1.5  2006/12/04 07:32:50  joegenbaclor
 * Added RFC3680 methods to Opal classes
 *
 * Revision 1.4  2006/12/02 01:12:13  joegenbaclor
 * Initial upload of SoftPhoneInterface classes
 *
 * Revision 1.3  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.2  2006/11/30 01:31:54  joegenbaclor
 * Corrected linux compile errors
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
 * Revision 1.7  2006/10/30 08:28:50  joegenbaclor
 * Added OSSPhone to ATLSIP source tree
 *
 * Revision 1.6  2006/10/03 09:29:01  joegenbaclor
 * Added INFO DTMF relay support
 *
 * Revision 1.5  2006/09/13 10:56:12  joegenbaclor
 * 1.  Added Release minimal build configuration
 * 2.  Added upx support for DLL compression
 *
 * Revision 1.4  2006/09/12 05:32:41  joegenbaclor
 * 1.  Corrected some bug where Playringing is not called for incoming call.
 * 2.  Implemented OnIncomingCallConnected callback.
 *
 * Revision 1.3  2006/08/30 08:42:54  joegenbaclor
 * Corrected bug on OnDisconnect
 *
 * Revision 1.2  2006/08/24 10:21:48  joegenbaclor
 * 1.  Major bug fix in double creation of pointers for the SIP End Pint in CallManager initialization routines
 * 2.  Fixed flush address book
 *
 * Revision 1.1  2006/08/23 07:47:33  joegenbaclor
 * Initial upload of ATLSIP files
 *
 *
 */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4510)
#pragma warning(disable:4610)
#endif


#include "SoftPhone.h"

#if HAS_OPAL

#include <opal/transcoders.h>
#define H323_STATIC_LIB
#include <codec/allcodecs.h>
#include <lids/alllids.h>
#include "OpalOSSConnection.h"
#include "ECCIAPI.h"

#define new PNEW

using namespace SF;


SoftPhoneManager::SoftPhoneManager( 
  SoftPhoneInterface * iface 
)
{
  m_PCSSEndPoint = NULL;
  m_SIPEndPoint = NULL;
  m_CallState = IdleState;

  m_DefaultAudioPlayDevice = PSoundChannel::GetDefaultDevice( PSoundChannel::Player );
  m_DefaultAudioRecordDevice = PSoundChannel::GetDefaultDevice( PSoundChannel::Recorder );
  m_RegistrarTTL = 3600;
  //m_PlayAlerting = FALSE;
  m_IsInitialized = FALSE;
  m_SoftPhoneInterface = iface;
  m_XORHash = FALSE;
  

  /// This is needed to force static loading of Sound Plugin
  /// before we use the audio devices
  PSound sound( "dummy-file" );
  AddRouteEntry("pc:.*  = sip:<da>");
  AddRouteEntry("sip:.*  = pc:<da>");
  m_PCSSEndPoint = new SoftPhonePCSSEndPoint(*this);
  m_SIPEndPoint = new SoftPhoneSIPEndPoint(*this);
  
  InitMediaInfo(m_PCSSEndPoint->GetPrefixName(), m_PCSSEndPoint->GetMediaFormats());
  
  OpalMediaFormatList mediaFormats;
  mediaFormats += m_PCSSEndPoint->GetMediaFormats();

  InitMediaInfo( "sw", OpalTranscoder::GetPossibleFormats( mediaFormats ) );

  ApplyMediaInfo();
}


SoftPhoneManager::~SoftPhoneManager()
{
  
}

BOOL SoftPhoneManager::Initialise()
{

  PTrace::Initialise( 3, "opal.log" );


  PTRACE( 1, "SoftPhoneManager::Initialise()" );


  ////////////////////////////////////////
  // Sound fields

  m_PCSSEndPoint->SetSoundChannelPlayDevice( m_DefaultAudioPlayDevice );
  m_PCSSEndPoint->SetSoundChannelRecordDevice( m_DefaultAudioRecordDevice);
  m_PCSSEndPoint->SetSoundChannelBufferDepth( 5 );
  SetAudioJitterDelay(100, 1000);


  ////////////////////////////////////////
  // Codec fields
  //InitMediaInfo(m_PCSSEndPoint->GetPrefixName(), m_PCSSEndPoint->GetMediaFormats());
  
  //OpalMediaFormatList mediaFormats;
  //mediaFormats += m_PCSSEndPoint->GetMediaFormats();

  //InitMediaInfo( "sw", OpalTranscoder::GetPossibleFormats( mediaFormats ) );


  ApplyMediaInfo();

  PString playDevice = m_DefaultAudioPlayDevice;
  PString recordDevice = m_DefaultAudioRecordDevice;
  GetSoftPhoneInterface()->Event_PCSoundSystemInitialized( 
    recordDevice, 
    playDevice );

  return TRUE;
}

void SoftPhoneManager::Terminate()
{
  if( m_SIPEndPoint != NULL )
    m_SIPEndPoint->GetUserAgent()->GetStack().Terminate();
}

void SoftPhoneManager::GetDefaultRecordDevice( 
  PString & dev 
)
{
  dev = PSoundChannel::GetDefaultDevice( PSoundChannel::Recorder );
}

void SoftPhoneManager::GetDefaultPlayDevice( 
  PString & dev 
)
{
  dev = PSoundChannel::GetDefaultDevice( PSoundChannel::Player );
}

void SoftPhoneManager::SetPlayDevice( 
  const PString & dev 
)
{
  m_DefaultAudioPlayDevice = dev;
  m_PCSSEndPoint->SetSoundChannelPlayDevice( m_DefaultAudioPlayDevice );
}

void SoftPhoneManager::SetRecordDevice(
  const PString & dev
)
{
  m_DefaultAudioRecordDevice = dev;
  m_PCSSEndPoint->SetSoundChannelRecordDevice( m_DefaultAudioRecordDevice);
}


BOOL SoftPhoneManager::InitializeSIP()
{
  PIPSocket::Address sipIface = 0;
  WORD sipPort = 5060;

  if( !m_IsInitialized )
  {
    


    Logger::SetDefaultLevel( 5 );
    Logger::SetDefaultLogFile( "sip.log" );


    LOG( LogInfo(), "SIP Stack Initialization Started" );

    
    
    {
      ///do a dummy listen to check the available port
      for( PINDEX i = 5060; i < 10000; i++ )
      {
        PUDPSocket sock( sipPort );
        if( sock.Listen() )
        {
          sock.Close();
          break;
        }
        ++sipPort;
      }
    }
  }
  m_SIPEndPoint->GetProfile().SetUser( m_UserName );
  SetDefaultUserName( m_UserName );

  m_SIPEndPoint->GetProfile().SetDisplayName( m_DisplayName );
  SetDefaultDisplayName( m_DisplayName );

  m_SIPEndPoint->GetProfile().GetRegistrationProfile().SetDomain( m_SIPDomain );
  m_SIPEndPoint->GetProfile().GetRegistrationProfile().SetRealm( m_SIPDomain );
  m_SIPEndPoint->GetProfile().GetRegistrationProfile().SetServerAddress( m_RegistrarAddress );
  m_SIPEndPoint->GetProfile().GetRegistrationProfile().SetUser( m_RegistrarUserName );
  m_SIPEndPoint->GetProfile().GetRegistrationProfile().SetAccount( m_RegistrarUserName );
  m_SIPEndPoint->GetProfile().GetRegistrationProfile().SetPassword( m_RegistrarPassword );
  m_SIPEndPoint->SetRegistrarTimeToLive(PTimeInterval(0, m_RegistrarTTL));

  m_SIPEndPoint->GetProfile().GetProxyProfile().SetDomain( m_SIPDomain );
  m_SIPEndPoint->GetProfile().GetProxyProfile().SetRealm( m_SIPDomain );
  m_SIPEndPoint->GetProfile().GetProxyProfile().SetServerAddress( m_ProxyAddress );

  m_SIPEndPoint->GetProfile().GetProxyProfile().SetUser( m_ProxyUserName );
  m_SIPEndPoint->GetProfile().GetProxyProfile().SetAccount( m_ProxyUserName );
  m_SIPEndPoint->GetProfile().GetProxyProfile().SetPassword( m_ProxyPassword );
  m_SIPEndPoint->SetProxy(m_ProxyAddress, m_ProxyUserName, m_ProxyPassword);
  
  BOOL ok = TRUE;

  if( !m_IsInitialized )
  {
    m_SIPEndPoint->GetProfile().GetTransportProfile().EnableUDP( sipIface, sipPort);

    BOOL ok = m_SIPEndPoint->Initialize();

    LOG( LogInfo(), "SIP Stack Initialized" );

    if( ok )
    {
      PStringArray rpcMethods;
      ok = Initialize( rpcMethods );
    }

    if( ok )
      GetSoftPhoneInterface()->Event_SIPInitialized();
    else
    {
      
      PString error = PString( "Unable to start Listener" );
      GetSoftPhoneInterface()->Event_SIPInitializationError( 0, error );
    }
  }

  m_IsInitialized = TRUE;

  return ok;
}


void SoftPhoneManager::MakeCall(const PString & address)
{
  if( m_CallState != IdleState )
    return;

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

        if( !m_SIPDomain.IsEmpty() )
        {
          SIPURI destURI = m_SIPDomain;
          destURI.SetUser( address );
          dest = destURI.AsString();
        }else if( !m_RegistrarAddress.IsEmpty() )
        {
          /// get the domain part of the registrar

          SIPURI destURI = m_RegistrarAddress;
          destURI.SetUser( address );
          dest = destURI.AsString();
        }else if( !m_ProxyAddress.IsEmpty() )
        {
          SIPURI destURI = m_ProxyAddress;
          destURI.SetUser( address );
          dest = destURI.AsString();
        }else
        {
          PString error = PString( "Error - Unable to determine destination address" );
          StopRinging();
          GetSoftPhoneInterface()->Event_OutgoingCallRejected( 0, error ); 
          return;
        }
      }
    }
  }

  if( dest.ToLower().Left( 4 ) != "sip:" )
    dest = "sip:" + address;

  SetState(CallingState);

  PTime now;
  To to;
  to.SetURI( dest );
  SetUpCall("pc:*", dest, m_CurrentCallToken);
}


void SoftPhoneManager::SendInstantMessage(
  const PString & address,
  const PString & msg
)
{
  PString dest = address;

  if (address.IsEmpty())
    return;

  //check if this it has a host part
  //if not append a domain
  if( address.Find( "@" ) == P_MAX_INDEX )
  {
    PIPSocket::Address ip( address );
    
    PRegularExpression regex("[0-9]*[.][0-9]*[.][0-9]*[.][0-9]*");
    PINDEX pos,len;
    if( !address.FindRegEx(regex, pos, len) || !ip.IsValid() ) 
    {
      /// not an ip address
      /// must be a telephone number
      /// or an alias
      

      if( !m_SIPDomain.IsEmpty() )
      {
        SIPURI destURI = m_SIPDomain;
        destURI.SetUser( address );
        dest = destURI.AsString();
      }else if( !m_RegistrarAddress.IsEmpty() )
      {
        /// get the domain part of the registrar

        SIPURI destURI = m_RegistrarAddress;
        destURI.SetUser( address );
        dest = destURI.AsString();
      }else if( !m_ProxyAddress.IsEmpty() )
      {
        SIPURI destURI = m_ProxyAddress;
        destURI.SetUser( address );
        dest = destURI.AsString();
      }else
      {
        return;
      }
    }
  }
  
  m_SIPEndPoint->GetUserAgent()->GetCore()->SendInstantMessage( dest, msg );
}



void SoftPhoneManager::AnswerCall()
{
  GetSoftPhoneInterface()->StopRingBackTone();

  if (m_CallState != RingingState)
    return;

  SetState(AnsweringState);
  m_PCSSEndPoint->AcceptIncomingConnection(m_CurrentConnectionToken);

  PString callToken = m_CurrentConnectionToken;
  StopRinging();
  GetSoftPhoneInterface()->Event_IncomingCallConnected( callToken );
}


void SoftPhoneManager::RejectCall()
{
  GetSoftPhoneInterface()->StopRingBackTone();
  ClearCall(m_CurrentCallToken);
  SetState(IdleState);
}


void SoftPhoneManager::HangUpCall()
{
  GetSoftPhoneInterface()->StopRingBackTone();
  
  if (m_CallState == IdleState)
    return;

  ClearCall(m_CurrentCallToken);
  SetState(IdleState);
}


void SoftPhoneManager::OnRinging(const OpalPCSSConnection & connection)
{
  if( m_CallState != IdleState )
    return;

  m_CurrentConnectionToken = connection.GetToken();
  m_CurrentCallToken = connection.GetCall().GetToken();
  m_CallState = RingingState;
  PStringStream remoteName; 
  remoteName << connection.GetRemotePartyAddress();
}

void SoftPhoneManager::OnReceivedInstantMessage(
  const SIPMessage & message
)
{
  PString contentType = message.GetContentType();
  PString body; 
  if( contentType *= "text/html" )
  {
    body = ParserTools::StripHTMLTags( message.GetBody() );
  }else if( contentType *= "text/plain" )
  {
    body = message.GetBody();
  }else
  {
    /// Ignore the rest for now
    return;
  }

  PString senderInfo = message.GetFromURI().AsString();
  PString msg = body;

  GetSoftPhoneInterface()->Event_IncomingInstantMessage( 
    senderInfo, 
    contentType,
    msg );
}

void SoftPhoneManager::OnRegEventStateOnline(
  const SIPURI & resourceURI
)
{
  GetSoftPhoneInterface()->Event_RegEventStateOnline( resourceURI.GetUser(), resourceURI.GetHost() );
}

void SoftPhoneManager::OnRegEventStateOffLine(
  const SIPURI & resourceURI
)
{
  GetSoftPhoneInterface()->Event_RegEventStateOffLine( resourceURI.GetUser(), resourceURI.GetHost() );
}

void SoftPhoneManager::OnEstablishedCall(OpalCall & call)
{
  m_CurrentCallToken = call.GetToken();
  SetState(InCallState);
}


void SoftPhoneManager::OnClearedCall(OpalCall & call)
{
  PString remoteName = '"' + call.GetPartyB() + '"';
  switch (call.GetCallEndReason()) {
    case OpalConnection::EndedByRemoteUser :
      std::cout << remoteName << " has cleared the call";
      break;
    case OpalConnection::EndedByCallerAbort :
      std::cout << remoteName << " has stopped calling";
      break;
    case OpalConnection::EndedByRefusal :
      std::cout << remoteName << " did not accept your call";
      break;
    case OpalConnection::EndedByNoAnswer :
      std::cout << remoteName << " did not answer your call";
      break;
    case OpalConnection::EndedByTransportFail :
      std::cout << "Call with " << remoteName << " ended abnormally";
      break;
    case OpalConnection::EndedByCapabilityExchange :
      std::cout << "Could not find common codec with " << remoteName;
      break;
    case OpalConnection::EndedByNoAccept :
      std::cout << "Did not accept incoming call from " << remoteName;
      break;
    case OpalConnection::EndedByAnswerDenied :
      std::cout << "Refused incoming call from " << remoteName;
      break;
    case OpalConnection::EndedByNoUser :
      std::cout << "Gatekeeper could find user " << remoteName;
      break;
    case OpalConnection::EndedByNoBandwidth :
      std::cout << "Call to " << remoteName << " aborted, insufficient bandwidth.";
      break;
    case OpalConnection::EndedByUnreachable :
      std::cout << remoteName << " could not be reached.";
      break;
    case OpalConnection::EndedByNoEndPoint :
      std::cout << "No phone running for " << remoteName;
      break;
    case OpalConnection::EndedByHostOffline :
      std::cout << remoteName << " is not online.";
      break;
    case OpalConnection::EndedByConnectFail :
      std::cout << "Transport error calling " << remoteName;
      break;
    default :
      break;
      std::cout << "Call with " << remoteName << " completed";
  }
  PTime now;
  std::cout << ", on " << now.AsString("w h:mma") << ". Duration "
            << setprecision(0) << setw(5) << (now - call.GetStartTime())
            << "s." << endl;

  if( call.GetCallEndReason() == OpalConnection::EndedByLocalUser )
  {
    PString str = PString( "Nomal Disconnect" );
    StopRinging();
    GetSoftPhoneInterface()->Event_CallDisconnected( str );
  }

  SetState(IdleState);
}


BOOL SoftPhoneManager::OnOpenMediaStream(OpalConnection & connection, OpalMediaStream & stream)
{
  if (!OpalManager::OnOpenMediaStream(connection, stream))
    return FALSE;

  PString prefix = connection.GetEndPoint().GetPrefixName();
  if (prefix == m_PCSSEndPoint->GetPrefixName())
    return TRUE;

  PStringStream info;

  info << "Started ";

  if (stream.IsSource())
    info << "receiving ";
  else
    info << "sending ";

  std::cout << stream.GetMediaFormat();

  if (stream.IsSource())
    info << " from ";
  else
    std::cout << " to ";

  info << connection.GetEndPoint().GetPrefixName() << " endpoint" << endl;

  PTRACE( 1, info );

  return TRUE;
}


void SoftPhoneManager::SendUserInput(
  char tone,
  int duration,
  BOOL asRFC2833
)
{
  PSafePtr<OpalCall> call = GetCall();
  if (call != NULL) {
    PSafePtr<OpalConnection> connection = call->GetConnection(1);
    if (connection != NULL) {
      if (!PIsDescendant(&(*connection), OpalPCSSConnection))
        connection = call->GetConnection(0);

      connection->SetSendUserInputMode( 
        asRFC2833 ? OpalConnection::SendUserInputAsInlineRFC2833 :
        OpalConnection::SendUserInputAsString );

      connection->OnUserInputTone(tone, duration);
    }
  }
}


BOOL SoftPhoneManager::CreateVideoOutputDevice(const OpalConnection & connection,
                                        const OpalMediaFormat & mediaFormat,
                                        BOOL preview,
                                        PVideoOutputDevice * & device,
                                        BOOL & autoDelete)
{
  if (preview && !m_VideoGrabPreview)
    return FALSE;

#ifdef WIN32
  // We ALWAYS use a Window
  device = PVideoOutputDevice::CreateDevice("Window");
  if (device != NULL) {
    autoDelete = TRUE;
    if (device->Open(psprintf("MSWIN STYLE=0x%08X TITLE=\"%s\"",
                              WS_POPUP|WS_BORDER|WS_SYSMENU|WS_CAPTION,
                              preview ? "Local" : (const char *)connection.GetRemotePartyName())))
      return TRUE;

    delete device;
  }
#endif

  return OpalManager::CreateVideoOutputDevice(connection, mediaFormat, preview, device, autoDelete);
}

    
void SoftPhoneManager::OnUserInputString(OpalConnection & connection, const PString & value)
{
  std::cout << "User input received: \"" << value << '"' << endl;
  OpalManager::OnUserInputString(connection, value);
}


void SoftPhoneManager::SetState(CallState newState)
{
  m_CallState = newState;
}

NAT::SIPSTUNClient::NatTypes SoftPhoneManager::SetSTUNServer(
  const PString & server,
  WORD udpPortBase,
  WORD udpPortMax,
  WORD rtpPortBase,
  WORD rtpPortMax
)
{
  SIPTransportManager * transport = m_SIPEndPoint->GetUserAgent()->GetStack().GetTransportManager();
  PAssertNULL( transport );
  this->m_SIPEndPoint->GetManager().SetSTUNServer( server );
  return transport->SetSTUNServer( server, udpPortBase, udpPortMax, rtpPortBase, rtpPortMax );
}

BOOL SoftPhoneManager::SendUnregister()
{
  if( m_SIPEndPoint == NULL )
    return FALSE;

  return m_SIPEndPoint->SendUnregister();
}

BOOL SoftPhoneManager::SendRegister()
{
  if( m_SIPEndPoint->IsRegistered() )
    return TRUE;
  
  m_SIPEndPoint->GetUserAgent()->GetRegistrar()->SetEnableXORHash( IsXORHashSet() );

  if (!m_SIPEndPoint->SendRegister(
    m_RegistrarUserName, 
    m_RegistrarPassword, 
    m_RegistrarAddress, 
    m_SIPDomain, 
    FALSE ))
  {
    PString error = PString( "Local Transport Error" );
      GetSoftPhoneInterface()->Event_LoginError( 
        SIPMessage::Code500_InternalServerError, 
        error );

    return FALSE;
  }
  
  
  return TRUE;
}


void SoftPhoneManager::InitMediaInfo(const char * source, const OpalMediaFormatList & mediaFormats)
{
  for (PINDEX i = 0; i < mediaFormats.GetSize(); i++) {
    const OpalMediaFormat & mediaFormat = mediaFormats[i];
    if (mediaFormat.GetPayloadType() != RTP_DataFrame::MaxPayloadType)
    {
      m_MediaInfo.push_back(SoftPhoneMedia(source, mediaFormat));
      m_MediaFormats.AppendString( mediaFormat );
    }
  }
}


void SoftPhoneManager::ApplyMediaInfo()
{
  PStringList mediaFormatOrder, mediaFormatMask;

  m_MediaInfo.sort();

  for (SoftPhoneMediaList::iterator mm = m_MediaInfo.begin(); mm != m_MediaInfo.end(); ++mm) {
    if (mm->preferenceOrder < 0)
    {
      mediaFormatMask.AppendString(mm->mediaFormat);
    }else
    {
      mediaFormatOrder.AppendString(mm->mediaFormat);
      
    }
  }

  if (!mediaFormatOrder.IsEmpty()) {
    SetMediaFormatOrder(mediaFormatOrder);
    SetMediaFormatMask(mediaFormatMask);
  }
}

double SoftPhoneManager::RPCGetAccountBalance(
  const PString & server,
  const PString & account
)
{
  double balance = 0;
  PString method = "prepaid.info";
  XMLRPCTransactionServer * rpc = static_cast<XMLRPCTransactionServer*>(this);
  PArray<PStringToString> request;
  PStringToString * acct = new PStringToString();
  acct->SetAt( "Username", account ); 
  request.Append( acct );
  PArray<PStringToString> response;
  rpc->SendRequest(
    method,
    request,
    response,
    server
  );

  if( response.GetSize() >= 2 )
  {
    PStringToString * data = static_cast<PStringToString*>(response.GetAt( 1 ));

#if 0
 	  PString * SerialNo = static_cast<PStringToString*>(data->GetAt( "SerialNo" ));
    PString * CardValue = static_cast<PStringToString*>(data->GetAt( "CardValue" ));
    PString * UsedValue = static_cast<PStringToString*>(data->GetAt( "UsedValue" ));
    PString * LastUsedDate = static_cast<PStringToString*>(data->GetAt( "LastUsedDate" ));
#endif
    PString * RemainingBalance = data->GetAt( "RemainingBalance" );
    if( RemainingBalance != NULL )
    {
      balance = RemainingBalance->AsReal();
    }
  }

  return balance;
}

///////////////////////////////////////////////////////////////////////////////

SoftPhonePCSSEndPoint::SoftPhonePCSSEndPoint(SoftPhoneManager & manager)
  : OpalPCSSEndPoint(manager),
    m_Manager(manager)
{
}


PString SoftPhonePCSSEndPoint::OnGetDestination(const OpalPCSSConnection & /*connection*/)
{
  // Dialog to prompt for address
  return PString();
}


void SoftPhonePCSSEndPoint::OnShowIncoming(const OpalPCSSConnection & connection)
{
  m_Manager.OnRinging(connection);
}


BOOL SoftPhonePCSSEndPoint::OnShowOutgoing(const OpalPCSSConnection & connection)
{
  PTime now;
  std::cout << connection.GetRemotePartyName() << " is ringing on "
            << now.AsString("w h:mma") << " ..." << endl;
  return TRUE;
}


///////////////////////////////////////////////////////////////////////////////


SoftPhoneSIPEndPoint::SoftPhoneSIPEndPoint(SoftPhoneManager & manager)
  : OpalOSSEndPoint(manager),
    m_Manager(manager)
{
}


 void SoftPhoneSIPEndPoint::OnRegistrationAccepted(
  RegisterSession & session,
  const SIPMessage & request
)
{
  OpalOSSEndPoint::OnRegistrationAccepted( session, request );
  m_Manager.GetSoftPhoneInterface()->Event_LoginSuccessful();
}

void SoftPhoneSIPEndPoint::OnRegistrationRejected(
  RegisterSession & session,
  const SIPMessage & request
)
{
  OpalOSSEndPoint::OnRegistrationRejected( session, request );

  PString reasonPhrase;
  int errorCode = 0;
  
  if( session.GetCurrentUACResponse().IsValid() )
  {
    reasonPhrase = session.GetCurrentUACResponse().GetStartLine();
    errorCode = session.GetCurrentUACResponse().GetStatusCode();
  }

  m_Manager.GetSoftPhoneInterface()->Event_LoginError( errorCode, reasonPhrase );
}

void SoftPhoneSIPEndPoint::OnRegistrationTimeout(
  RegisterSession & session
)
{
 
  OpalOSSEndPoint::OnRegistrationTimeout( session );
  PString reason = PString( "Request Timeout" );
  m_Manager.GetSoftPhoneInterface()->Event_LoginError( 
    SIPMessage::Code408_RequestTimeout,  reason );
}

void SoftPhoneSIPEndPoint::OnUnregistration(
  RegisterSession & session,
  const SIPMessage & request
)
{
  OpalOSSEndPoint::OnUnregistration( session, request ); 
  m_IsRegistered = FALSE;
  m_Manager.GetSoftPhoneInterface()->Event_LogoutSuccessful();
}

void SoftPhoneSIPEndPoint::OnConnected(
  CallSession & session,
  const SIPMessage & connect
)
{
  OpalOSSEndPoint::OnConnected( session, connect );
  PString remote = session.GetTargetURI().AsString();
  m_Manager.StopRinging();
  m_Manager.GetSoftPhoneInterface()->Event_OutgoingCallConnected( remote );
}


/// will be called when the call is disconnected
void SoftPhoneSIPEndPoint::OnDisconnected(
  CallSession & session,
  const SIPMessage & msg
)
{
  PString token = session.GetInternalHeader( "OpalConnectionId" );
  
  PSafePtr<OpalConnection> opalConnection = 
    GetConnectionWithLock(token, PSafeReference);
  
  if (opalConnection == NULL) 
    return;
 
  OpalOSSConnection * ossCon = (OpalOSSConnection *)(OpalConnection*)opalConnection;

  int callEndReason = session.GetCallEndReason();
  int callEndStatusCode = session.GetCallEndStatusCode();
  

  BOOL isFromRemote = TRUE;

  switch( callEndReason )
  {
    case CallSession::ICT_Recv3xx:
      switch( callEndStatusCode )
      {
        case SIPMessage::Code300_MultipleChoices:
        case SIPMessage::Code301_MovedPermanently:
        case SIPMessage::Code302_MovedTemporarily:
        case SIPMessage::Code305_UseProxy:
        case SIPMessage::Code380_AlternativeService:
        default:
          ossCon->Release( OpalConnection::EndedByCallForwarded );
          break;
      }
      break;
    case CallSession::ICT_Recv4xx:
      switch( callEndStatusCode )
      {
        case SIPMessage::Code400_BadRequest:
          ossCon->Release( OpalConnection::EndedByNoAccept );
          break;
        case SIPMessage::Code401_Unauthorized:
        case SIPMessage::Code402_PaymentRequired:
        case SIPMessage::Code403_Forbidden:
          ossCon->Release( OpalConnection::EndedBySecurityDenial );
          break;
        case SIPMessage::Code404_NotFound:
          ossCon->Release( OpalConnection::EndedByNoUser );
            break;
        case SIPMessage::Code405_MethodNotAllowed:
        case SIPMessage::Code406_NotAcceptable:
          ossCon->Release( OpalConnection::EndedByNoAccept );
          break;
        case SIPMessage::Code407_ProxyAuthenticationRequired:
          ossCon->Release( OpalConnection::EndedBySecurityDenial );
          break;
        case SIPMessage::Code408_RequestTimeout:
          ossCon->Release( OpalConnection::EndedByUnreachable );
          break;
        case SIPMessage::Code409_Conflict:
        case SIPMessage::Code410_Gone:
        case SIPMessage::Code411_LengthRequired:
        case SIPMessage::Code413_RequestEntityTooLarge:
        case SIPMessage::Code414_RequestURITooLarge:
          ossCon->Release( OpalConnection::EndedByNoAccept );
          break;
        case SIPMessage::Code415_UnsupportedMedia:
        case SIPMessage::Code416_UnsupportedURIScheme:
          ossCon->Release( OpalConnection::EndedByCapabilityExchange );
          break;

        case SIPMessage::Code420_BadExtension:
        case SIPMessage::Code423_IntervalTooShort:
        case SIPMessage::Code480_TemporarilyNotAvailable:
        case SIPMessage::Code481_TransactionDoesNotExist:
        case SIPMessage::Code482_LoopDetected:
        case SIPMessage::Code483_TooManyHops:
        case SIPMessage::Code484_AddressIncomplete:
        case SIPMessage::Code485_Ambiguous:
          ossCon->Release( OpalConnection::EndedByNoAccept );
          break;
        case SIPMessage::Code486_BusyHere:
          ossCon->Release( OpalConnection::EndedByRemoteBusy );
          break;
        case SIPMessage::Code487_RequestCancelled:
          ossCon->Release( OpalConnection::EndedByCallerAbort );
          break;
        case SIPMessage::Code488_NotAcceptableHere:
        case SIPMessage::Code489_BadRequest:
        default:
          ossCon->Release( OpalConnection::EndedByNoAccept );
          break;
      }
      break;
    case CallSession::ICT_Recv5xx:
      switch( callEndStatusCode )
      {
        case SIPMessage::Code500_InternalServerError:
        case SIPMessage::Code501_NotImplemented:
        case SIPMessage::Code502_BadGateway:
        case SIPMessage::Code503_ServiceUnavailable:
        case SIPMessage::Code504_GatewayTimeout:
        case SIPMessage::Code505_VersionNotSupported:
        default:
          ossCon->Release( OpalConnection::EndedByTemporaryFailure );
          break;
      }
      break;
    case CallSession::ICT_Recv6xx:
      switch( callEndStatusCode )
      {
        case SIPMessage::Code600_BusyEverywhere:
        case SIPMessage::Code603_Decline:
        case SIPMessage::Code604_DoesNotExistAnywhere:
        case SIPMessage::Code606_NotAcceptable:
        default:
          ossCon->Release( OpalConnection::EndedByTemporaryFailure );
          break;
      }
      break;
    case CallSession::IST_3xxSent:
    case CallSession::IST_4xxSent:
    case CallSession::IST_5xxSent:
    case CallSession::IST_6xxSent:
    case CallSession::NICT_ByeSent:
    case CallSession::NICT_CancelSent:
      isFromRemote = FALSE;
      ossCon->Release( OpalConnection::EndedByLocalUser );
      break;
    case CallSession::NIST_RecvBye:
    case CallSession::NIST_RecvCancel:
      ossCon->Release( OpalConnection::EndedByRemoteUser );
      break;
    default:
      isFromRemote = FALSE;
      ossCon->Release( OpalConnection::EndedByTemporaryFailure );
      break;
  }

  PString  reasonPhrase = PString( "Normal Disconnect" );
  if( isFromRemote )
  {
    if( msg.IsValid() 
      && !msg.Is2xx() 
      && msg.GetCSeqMethod().ToUpper() == "INVITE" )
    {
      StatusLine statusLine;
      msg.GetStatusLine( statusLine );
      reasonPhrase = PString( statusLine.GetStatusCode() ) 
        + " " + statusLine.GetReasonPhrase();
      
    }
  }

  m_Manager.SetState( SoftPhoneManager::IdleState );
  m_Manager.StopRinging();
  m_Manager.GetSoftPhoneInterface()->Event_CallDisconnected( reasonPhrase );
}

BOOL SoftPhoneSIPEndPoint::OnIncomingConnection(
  const SIPMessage & invite,
  CallSession & session
)
{
  if( m_Manager.GetState() != SoftPhoneManager::IdleState )
    return FALSE;


  PTime now;
  From from;
  from.SetURI( invite.GetFromURI() );

  //m_Manager.PlayRinging();
  PString callInfo = invite.GetFromURI().AsString();
  m_Manager.PlayRinging();
  m_Manager.GetSoftPhoneInterface()->Event_IncomingCall( callInfo );
 

  return OpalOSSEndPoint::OnIncomingConnection(
    invite, session );
}

/// Progress callbacks
void SoftPhoneSIPEndPoint::OnAlerting(
  CallSession & session,
  const SIPMessage & alerting
)
{
  OpalOSSEndPoint::OnAlerting( session, alerting );

  //if( !alerting.HasSDP() && alerting.Is1xx() )
  //  m_Manager.PlayRinging();

  PString info = session.GetTargetURI().AsString();
  if( session.GetType() == CallSession::Client )
  {
    m_Manager.PlayRinging();
    m_Manager.GetSoftPhoneInterface()->Event_OutgoingCallRinging( info );
  }
}

void SoftPhoneSIPEndPoint::OnProgress(
  CallSession & session,
  const SIPMessage & alerting
)
{
  OpalOSSEndPoint::OnProgress( session, alerting );
  PString info = session.GetTargetURI().AsString();
  if( session.GetType() == CallSession::Client )
    m_Manager.GetSoftPhoneInterface()->Event_OutgoingCallRinging( info );
}

void SoftPhoneSIPEndPoint::OnReceivedInstantMessage(
  const SIPMessage & message
)
{
  m_Manager.OnReceivedInstantMessage( message );
}

void SoftPhoneSIPEndPoint::OnRegEventStateOnline(
  const SIPURI & resourceURI
)
{
  m_Manager.OnRegEventStateOnline( resourceURI );
}

void SoftPhoneSIPEndPoint::OnRegEventStateOffLine(
  const SIPURI & resourceURI
)
{
  m_Manager.OnRegEventStateOffLine( resourceURI );
}

OpalOSSConnection * SoftPhoneSIPEndPoint::CreateConnection(
  OpalCall & call,
  const PString &remoteParty,
  const PString & token,
  void * /*userData*/
)
{
  OpalOSSConnection * conn =  new OpalOSSConnection( call, *this, token, remoteParty );
  conn->SetHashCall( m_Manager.IsXORHashSet() );
  return conn;
}



PString SoftPhoneManager::IMParseURI(
  const PString & msg
)
{
  MimeHeader message( msg );
  if( message.GetMimeName() != "IM" )
    return PString::Empty();

  PString body = message.GetMimeValue();
  if( body.IsEmpty() )
    return PString::Empty();

  PINDEX idx = body.Find( ": >>" );
  if(  idx == P_MAX_INDEX )
    return PString::Empty();

  PString _uri = body.Left( idx ).Trim();
  SIPURI uri( _uri );
  
  if( !uri.GetUser().IsEmpty() && !uri.GetHost().IsEmpty() )
    return uri.GetUser() + "@" + uri.GetHost();
  else if( uri.GetUser().IsEmpty() && !uri.GetHost().IsEmpty() )
    return uri.GetHost();
  else if( !uri.GetUser().IsEmpty() && uri.GetHost().IsEmpty() )
    return uri.GetUser();

  return PString::Empty();

}

PString SoftPhoneManager::IMParseMessage(
  const PString & msg
)
{
  MimeHeader message( msg );
  if( message.GetMimeName() != "IM" )
    return PString::Empty();

  PString body = message.GetMimeValue();
  if( body.IsEmpty() )
    return PString::Empty();

  PINDEX idx = body.Find( ": >>" );
  if(  idx == P_MAX_INDEX )
    return PString::Empty();

  return body.Mid( idx + 4 );
}

void SoftPhoneManager::WatchRegistrationState(
  const PString & stateAgent,
  const PString & resourceURI,
  const PTimeInterval & expires,
  const PString & authUser,
  const PString & authPassword
  
)
{
  m_SIPEndPoint->GetUserAgent()->WatchRegistrationState( stateAgent, resourceURI, expires, authUser, authPassword );
}

void SoftPhoneManager::StopWatchingRegistrationState(
  const PString & resourceURI
)
{
  m_SIPEndPoint->GetUserAgent()->StopWatchingRegistrationState( resourceURI );
}


/// plays an audible ringing sound
void SoftPhoneManager::PlayRinging()
{
  m_SoftPhoneInterface->PlayRingBackTone();
  //if( !m_PlayAlerting )
  //{
  //  m_PlayAlerting = TRUE;
  //  PThread::Create( PCREATE_NOTIFIER( PlayRinging ) );
  //}
}


void SoftPhoneManager::StopRinging()
{
  //m_PlayAlerting = FALSE;
  //MS::RingTone_0.StopPlaying();
  m_SoftPhoneInterface->StopRingBackTone();
}


#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif


