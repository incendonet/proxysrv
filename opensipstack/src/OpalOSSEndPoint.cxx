/*
 *
 * OpalOSSEndPoint.cxx
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
 * $Log: OpalOSSEndPoint.cxx,v $
 * Revision 1.37  2006/12/22 07:37:21  joegenbaclor
 * Introduced separate funtion call  for starting of transport thread.  This is necessary
 *  to make sure that the backdoor listener is already initialized prior to calling transport
 *  reads
 *
 * Revision 1.36  2006/12/04 07:32:50  joegenbaclor
 * Added RFC3680 methods to Opal classes
 *
 * Revision 1.35  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.34  2006/11/21 06:55:19  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.33  2006/10/30 02:42:27  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.32  2006/10/23 20:22:28  joegenbaclor
 * Modified Registration Methods to include a reference to the Register request
 *
 * Revision 1.31  2006/10/12 05:22:42  joegenbaclor
 * Enabled ability to make encrypted outbound calls for OPAL
 *
 * Revision 1.30  2006/09/04 02:58:53  joegenbaclor
 * 1.  More bug fixes to garbage collector
 * 2.  Added new Logging macro so call System Log for fatal errors
 *
 * Revision 1.29  2006/08/29 10:47:23  rcolobong
 * *** empty log message ***
 *
 * Revision 1.28  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.27  2006/08/26 14:31:44  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.26  2006/08/24 10:19:14  joegenbaclor
 * 1.  Couple of bug fixes in SIP Intitializarion and Disconnect cause detection
 * 2.  Added ProxyAuthentication call backs to Register Session
 *
 * Revision 1.25  2006/08/23 03:33:54  joegenbaclor
 * Some Opal related bug fixes discovered duriong intial development of ATLSIP
 *
 * Revision 1.24  2006/08/18 07:12:24  joegenbaclor
 * 1.  added Function overload for GetContentType
 * 2.  Changed unix make files to not include opensbc in make process
 *
 * Revision 1.23  2006/08/15 15:42:09  joegenbaclor
 * 1. Added Instant Messaging functions to OPAL
 *
 * Revision 1.22  2006/07/24 07:33:56  joegenbaclor
 * Minor bug fixes allover
 *
 * Revision 1.21  2006/07/10 06:29:38  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.20  2006/06/28 14:15:00  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.19  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.18  2006/06/08 09:02:40  joegenbaclor
 * Added SendUnregister() method to OpalOSSEndPoint
 *
 * Revision 1.17  2006/06/08 04:54:20  joegenbaclor
 * !.  Added OnRegistrationTimeout Call back to Registrar classes
 * 2. Moved ECCIAPI to Tools namespace
 *
 * Revision 1.16  2006/05/19 06:30:37  joegenbaclor
 * 1.  Fixed bug in SIPHeaderB where tag may not be parsed properly if no < > enclosure is present
 * 2.  Various enhancements to OpenSBC and proxy session
 *
 * Revision 1.15  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.14  2006/04/20 14:32:28  joegenbaclor
 * Fixed early media bug where soucd channels in OPAL ay not be opened in time for ringback to start streaming.
 *
 * Revision 1.13  2006/04/18 07:18:21  joegenbaclor
 * Fixed unix build environment errors and warnings
 *
 * Revision 1.12  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.11  2006/04/06 04:27:51  joegenbaclor
 * More work on OPAL Inbound
 *
 * Revision 1.10  2006/04/05 00:26:04  joegenbaclor
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
 * Revision 1.8  2006/03/17 13:51:00  joegenbaclor
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
 * Revision 1.6  2006/03/15 16:40:46  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.5  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */



#include "OpalOSSEndPoint.h"
#include "OpalOSSConnection.h"
#include "SIPUDPTransport.h"

#if HAS_OPAL

using namespace OPAL;

#define new PNEW

/////////////////////////////////////////////////////////////////////////
class DummyOpalTransport : public OpalTransportIP
{
  PCLASSINFO( DummyOpalTransport, OpalTransportIP );
public:
    DummyOpalTransport(
      OpalEndPoint & endpoint,    ///<  Endpoint object
      PUDPSocket & socket         ///<  Preopened socket
    );

    ~DummyOpalTransport();

    virtual BOOL Read(
      void * buffer,
      PINDEX length
    );

    virtual BOOL Close();

    virtual BOOL IsReliable() const;

    virtual BOOL IsCompatibleTransport(
      const OpalTransportAddress & address
    ) const;

    virtual BOOL Connect();

    virtual void EndConnect(
      const OpalTransportAddress & localAddress  ///<  Resultant local address
    );

    virtual BOOL SetLocalAddress(
      const OpalTransportAddress & address
    );

    virtual BOOL SetRemoteAddress(
      const OpalTransportAddress & address
    );

    virtual void SetPromiscuous(
      PromisciousModes promiscuous
    );

    virtual OpalTransportAddress GetLastReceivedAddress() const;

    virtual BOOL ReadPDU(
      PBYTEArray & packet   ///<  Packet read from transport
    );

    virtual BOOL WritePDU(
      const PBYTEArray & pdu     ///<  Packet to write
    );

    virtual BOOL WriteConnect(
      WriteConnectCallback function,  ///<  Function for writing data
      void * userData                 ///<  User data to pass to write function
    );

  protected:
    virtual const char * GetProtoPrefix() const;

    PromisciousModes     promiscuousReads;
    OpalTransportAddress lastReceivedAddress;
    BOOL                 socketOwnedByListener;
    PBYTEArray           preReadPacket;
    PSocketList          connectSockets;
    BOOL 		 reuseAddressFlag;
};

static const char IpPrefix[] = "ip$"; // For backward compatibility with OpenH323
static const char TcpPrefix[] = "tcp$";
static const char UdpPrefix[] = "udp$";

DummyOpalTransport::DummyOpalTransport(OpalEndPoint & ep, PUDPSocket & udp)
  : OpalTransportIP(ep, PIPSocket::GetDefaultIpAny(), 0)
{
  promiscuousReads = AcceptFromAnyAutoSet;
  socketOwnedByListener = TRUE;
  reuseAddressFlag = FALSE;

  udp.GetLocalAddress(localAddress, localPort);

  Open(udp);


  PTRACE(3, "OpalUDP\tPre-bound to interface: " << localAddress << ':' << localPort);
}

DummyOpalTransport::~DummyOpalTransport()
{
  CloseWait();
  PTRACE(4,"Opal\tDeleted transport " << *this);
}


BOOL DummyOpalTransport::Close()
{
  PTRACE(4, "OpalUDP\tClose");
  PReadWaitAndSignal mutex(channelPointerMutex);

  if (socketOwnedByListener) {
    channelPointerMutex.StartWrite();
    readChannel = writeChannel = NULL;
    // Thread also owned by listener as well, don't wait on or delete it!
    thread = NULL; 
    channelPointerMutex.EndWrite();
    return TRUE;
  }

  if (connectSockets.IsEmpty())
    return OpalTransport::Close();

  channelPointerMutex.StartWrite();
  readChannel = writeChannel = NULL;

  // Still in connection on multiple interface phase. Close all of the
  // sockets we have open.
  for (PINDEX i = 0; i < connectSockets.GetSize(); i++)
    connectSockets[i].Close();

  channelPointerMutex.EndWrite();

  return TRUE;
}


BOOL DummyOpalTransport::IsReliable() const
{
  return FALSE;
}


BOOL DummyOpalTransport::IsCompatibleTransport(const OpalTransportAddress & address) const
{
  return address.Left(strlen(UdpPrefix)).ToLower () == UdpPrefix ||
        address.Left(sizeof(IpPrefix)-1).ToLower () == IpPrefix;
}


BOOL DummyOpalTransport::Connect()
{	
  return FALSE;
}


void DummyOpalTransport::EndConnect(const OpalTransportAddress & theLocalAddress)
{
}


BOOL DummyOpalTransport::SetLocalAddress(const OpalTransportAddress & newLocalAddress)
{
  return FALSE;
}


BOOL DummyOpalTransport::SetRemoteAddress(const OpalTransportAddress & address)
{
  if (!OpalTransportIP::SetRemoteAddress(address))
    return FALSE;

  PUDPSocket * socket = (PUDPSocket *)GetReadChannel();
  if (socket != NULL)
    socket->SetSendAddress(remoteAddress, remotePort);
  return TRUE;
}


void DummyOpalTransport::SetPromiscuous(PromisciousModes promiscuous)
{
  promiscuousReads = promiscuous;
}


OpalTransportAddress DummyOpalTransport::GetLastReceivedAddress() const
{
  if (!lastReceivedAddress)
    return lastReceivedAddress;

  return OpalTransport::GetLastReceivedAddress();
}


BOOL DummyOpalTransport::Read(void * buffer, PINDEX length)
{
  return FALSE;
}


BOOL DummyOpalTransport::ReadPDU(PBYTEArray & packet)
{
  return FALSE;
}


BOOL DummyOpalTransport::WritePDU(const PBYTEArray & packet)
{
  return FALSE;
}


BOOL DummyOpalTransport::WriteConnect(WriteConnectCallback function, void * userData)
{
  return FALSE;
}


const char * DummyOpalTransport::GetProtoPrefix() const
{
  return UdpPrefix;
}

  ///////////////////////////////////////////////////////////////////

OpalOSSEndPoint::OpalOSSEndPoint(
  OpalManager & manager
) : OpalEndPoint( manager, OPAL_OSS_PREFIX, CanTerminateCall )
{
  m_RegisterExpires = PTimeInterval( 0, 3600 );
  m_IsRegistered  = FALSE;
  m_OSSUserAgent = new OpalOSSUserAgent( *this );
}
    
OpalOSSEndPoint::~OpalOSSEndPoint()
{
  m_OSSUserAgent->Terminate();
  delete m_OSSUserAgent;
  m_OSSUserAgent = NULL;
}

ProfileUA & OpalOSSEndPoint::GetProfile()
{
  return m_OSSUserAgent->GetDefaultProfile();
}

OpalMediaFormatList OpalOSSEndPoint::GetMediaFormats() const
{
  return OpalMediaFormat::GetAllRegisteredMediaFormats();
}

BOOL OpalOSSEndPoint::Initialize()
{
  PStringArray formatOrder = GetManager().GetMediaFormatOrder();
  for( PINDEX i = 0; i < formatOrder.GetSize(); i++ )
  {
    SDPMediaFmt format = SDPMediaFmt::CreateOpalCompatibleFormat( formatOrder[i] );
    if( format.GetPayloadType() != SDPMediaFmt::IllegalPayloadType )
      GetProfile().GetMediaProfile().AddMediaFormat( format );
  }

  SetDefaultLocalPartyName( GetManager().GetDefaultUserName() );

  GetProfile().SetDisplayName( GetManager().GetDefaultDisplayName() );
  GetProfile().SetUser( GetDefaultLocalPartyName() );

  return m_OSSUserAgent->Initialize( OPAL_UA_THREAD_COUNT );

}

BOOL OpalOSSEndPoint::SendRegister(
  const PString & userName,
  const PString & password,
  const PString & uri,
  const PString & _domain,
  BOOL synchronous
)
{
  ProfileRegister & profile = GetProfile().GetRegistrationProfile();
  profile.SetUser( userName );
  profile.SetAccount( userName );
  profile.SetPassword( password );
  profile.SetServerAddress( uri );

  PString domain = _domain;
  SIPURI serverURI( uri );
  if( domain.IsEmpty() )
    domain = serverURI.GetHost();
  
  if( profile.GetContactSize() == 0 )
  {

    PIPSocket::Address contactAddress;
    WORD contactPort;
    PIPSocket::Address targetAddress;
    WORD targetPort;

    if( !SIPTransport::Resolve( serverURI, targetAddress, targetPort ) )
      return FALSE;

    SIPTransportManager * transport = m_OSSUserAgent->GetStack().GetTransportManager();
    
    if( !transport->GetListenerAddress( SIPTransport::UDP, targetAddress, contactAddress, contactPort ) )
      return FALSE;

    PStringStream contact;
    contact << contactAddress << ":" << contactPort;

    profile.AppendContact( contact );
  }

  PStringStream sessionId;
  sessionId << "REGISTER-" << userName << "@" << domain;

  /// destroy the old session if it exists
  SIPSession::GCRef autoRef( "OpalOSSEndPoint::SendRegister" );
  RegisterSession * oldSession = 
    m_OSSUserAgent->GetRegistrar()->FindGCRefBySessionId<RegisterSession>( sessionId, autoRef );
  
  if(  oldSession != NULL )
    oldSession->Destroy();

  GetProfile().GetRegistrationProfile().SetExpireInterval( m_RegisterExpires.GetSeconds() );
  
  RegisterSession * regSession = (RegisterSession*)m_OSSUserAgent->GetRegistrar()->CreateClientSession( GetProfile(), sessionId );

  if( regSession != NULL )
  {
    regSession->SendRegister();
    if( synchronous )
      m_RegMutex.Wait( OPAL_REGISTRAR_TIMEOUT );  
    else
      return TRUE;
  }

  return m_IsRegistered;

}

BOOL OpalOSSEndPoint::SendUnregister()
{
  SIPSession::GCRef autoRef( "OpalOSSEndPoint::SendUnregister" );
  RegisterSession * session = 
    GetUserAgent()->GetRegistrar()->FindGCRefBySessionId<RegisterSession>( 
    m_CurrentRegSession, autoRef );

  if( session != NULL )
  {
    session->SendUnregister();
    return TRUE;
  }

  return FALSE;
}

/// registrar callbacks
RegisterSession::AcceptRegistrationResponse OpalOSSEndPoint::OnAcceptRegistration(
  RegisterSession & session,
  const SIPMessage & request
)
{
  return RegisterSession::AcceptByRejection;
}

void OpalOSSEndPoint::OnRegistrationAccepted(
  RegisterSession & session,
  const SIPMessage & request
)
{
  m_IsRegistered = TRUE;
  m_CurrentRegSession = session.GetSessionId();
  m_RegMutex.Signal();
}

void OpalOSSEndPoint::OnRegistrationRejected(
  RegisterSession & session,
  const SIPMessage & request
)
{
  m_IsRegistered = FALSE;
  m_RegMutex.Signal();
}

void OpalOSSEndPoint::OnRegistrationTimeout(
  RegisterSession & session
)
{
  m_IsRegistered = FALSE;
  m_RegMutex.Signal();
}

void OpalOSSEndPoint::OnUnregistration(
  RegisterSession & session,
  const SIPMessage & request
)
{
  m_IsRegistered = FALSE;
}

BOOL OpalOSSEndPoint::OnRequestA1Hash(
  const SIPURI & userURL,
  PString & a1,
  RegisterSession & session
)
{
  return FALSE;
}

BOOL OpalOSSEndPoint::SetDomain(
  const PString & uri /// uri of the domain
)
{
  ProfileRegister & proxy = GetProfile().GetProxyProfile();
  proxy.SetDomain( uri );

  ProfileRegister & registrar = GetProfile().GetProxyProfile();
  registrar.SetDomain( uri );
  return TRUE;
}

BOOL OpalOSSEndPoint::SetProxy(
  const PString & uri, 
  const PString & userName, 
  const PString & password 
)
{
  ProfileRegister & profile = GetProfile().GetProxyProfile();
  profile.SetAccount( userName );
  profile.SetPassword( password );
  profile.SetServerAddress( uri );
  return TRUE;
}

SIPURI OpalOSSEndPoint::GetProxy()
{
  ProfileRegister & profile = GetProfile().GetProxyProfile();
  SIPURI uri( profile.GetServerAddress() );
  uri.SetPassword( profile.GetPassword() );
  uri.SetUser( profile.GetAccount() );
  return uri;
}

BOOL OpalOSSEndPoint::NewIncomingConnection(
  OpalTransport * /*transport*/  /// Transport connection came in on
)
{
  return TRUE;
}


BOOL OpalOSSEndPoint::MakeConnection(
	OpalCall & call,        /// Owner of connection
	const PString & party,  /// Remote party to call
	void * userData  /// Arbitrary data to pass to connection
)
{
  
  PString ossPrefix = PString( OPAL_OSS_PREFIX ) + PString( ":" );
  if( party.Find( ossPrefix ) == P_MAX_INDEX )
    return FALSE;  /// not an opensipstack call
  
  PString callId = ParserTools::GenGUID();

  OpalOSSConnection * connection = CreateConnection( call, party, callId, userData );
  
  if (connection == NULL)
    return FALSE;

  connectionsActive.SetAt(connection->GetToken(), connection);

  // If we are the A-party then need to initiate a call now in this thread. If
  // we are the B-Party then SetUpConnection() gets called in the context of
  // the A-party thread.
  if (call.GetConnection(0) == (OpalConnection*)connection)
    connection->SetUpConnection();

  return TRUE;
}

BOOL OpalOSSEndPoint::OnIncomingConnection(
  const SIPMessage & invite,
  CallSession & session
)
{
  PString callId = invite.GetCallId();
  From from;
  invite.GetFrom( from );
  SIPURI caller = from.GetURI();

  SIPURI contact = invite.GetContactTopURI();
  contact.SetUser( caller.GetUser() );

  To to;
  invite.GetTo( to );
  PString target = to.GetURI().AsString();
  
  OpalOSSConnection *connection = 
    CreateConnection(
      *GetManager().CreateCall(), 
      contact.AsString(),
      callId, 
      NULL
  );


  if (connection == NULL) {
    PTRACE(2, "SIP\tFailed to create SIPConnection for INVITE from " << caller << " for " << target);
    return FALSE;
  }

  PString remotePartyName;
  if( !from.GetDisplayName().IsEmpty() )
    remotePartyName = from.GetDisplayName();
  else
    remotePartyName = from.GetURI().GetUser();

  connection->SetRemotePartyAddress( from.GetURI().AsString() );
  connection->SetRemotePartyName( remotePartyName );
  
  session.AddInternalHeader( "OpalConnectionId", connection->GetToken() );

  // add the connection to the endpoint list
  connectionsActive.SetAt(connection->GetToken(), connection);

  return TRUE;
}

BOOL OpalOSSEndPoint::OnSetupIncomingCall(
  const SIPMessage & invite,
  CallSession & session
)
{
    PString token = session.GetInternalHeader( "OpalConnectionId" );
  
  PSafePtr<OpalConnection> opalConnection = 
    GetConnectionWithLock(token, PSafeReference);
  
  if (opalConnection == NULL) 
    return FALSE;
 
  OpalOSSConnection * ossCon = (OpalOSSConnection *)(OpalConnection*)opalConnection;

  return ossCon->OnIncomingCall( session );
}

OpalOSSConnection * OpalOSSEndPoint::CreateConnection(
  OpalCall & call,
  const PString &remoteParty,
  const PString & token,
  void * userData
)
{
  return new OpalOSSConnection( call, *this, token, remoteParty );
}

/// will be called when the call is established
void OpalOSSEndPoint::OnEstablished(
  CallSession & session
)
{
  PString token = session.GetInternalHeader( "OpalConnectionId" );
  
  PSafePtr<OpalConnection> opalConnection = 
    GetConnectionWithLock(token, PSafeReference);
  
  if (opalConnection == NULL) 
    return;
 
  OpalOSSConnection * ossCon = (OpalOSSConnection *)(OpalConnection*)opalConnection;

  ossCon->OnEstablished();
}

void OpalOSSEndPoint::OnConnected(
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

  ossCon->OnConnected();
}

/// will be called when the call is disconnected
void OpalOSSEndPoint::OnDisconnected(
  CallSession & session,
  const SIPMessage & message
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
      ossCon->Release( OpalConnection::EndedByLocalUser );
      break;
    case CallSession::NIST_RecvBye:
    case CallSession::NIST_RecvCancel:
      ossCon->Release( OpalConnection::EndedByRemoteUser );
      break;
    default:
      ossCon->Release( OpalConnection::EndedByTemporaryFailure );
      break;
  }
}


/// Progress callbacks
void OpalOSSEndPoint::OnAlerting(
  CallSession & session,
  const SIPMessage & message
)
{
  PString token = session.GetInternalHeader( "OpalConnectionId" );
  
  PSafePtr<OpalConnection> opalConnection = 
    GetConnectionWithLock(token, PSafeReference);
  
  if (opalConnection == NULL) 
    return;
 
  OpalOSSConnection * ossCon = (OpalOSSConnection *)(OpalConnection*)opalConnection;

  ossCon->OnAlerting();

}

void OpalOSSEndPoint::OnProgress(
  CallSession & session,
  const SIPMessage & message
)
{
  PString token = session.GetInternalHeader( "OpalConnectionId" );
  
  PSafePtr<OpalConnection> opalConnection = 
    GetConnectionWithLock(token, PSafeReference);
  
  if (opalConnection == NULL) 
    return;
 
  OpalOSSConnection * ossCon = (OpalOSSConnection *)(OpalConnection*)opalConnection;
  ossCon->OnAlerting();
}

  /// returning TRUE here will give permission to the session
  /// to resend the invite to the new contact header in the response.
  /// returning FALSE will end the connection
BOOL OpalOSSEndPoint::OnCallRedirect(
  CallSession & session,
  const SIPMessage & redirect
)
{
  return TRUE;
}

BOOL OpalOSSEndPoint::OnIncomingSDPOffer(
  const SIPMessage & sdp,
  CallSession & session
)
{
  PString token = session.GetInternalHeader( "OpalConnectionId" );
  
  PSafePtr<OpalConnection> opalConnection = 
    GetConnectionWithLock(token, PSafeReference);
  
  if (opalConnection == NULL) 
    return FALSE;
 
  OpalOSSConnection * ossCon = (OpalOSSConnection *)(OpalConnection*)opalConnection;

  return ossCon->OnIncomingSDPOffer( sdp );
}

BOOL OpalOSSEndPoint::OnRequireSDPAnswer(
  const SIPMessage & offer,
  SIPMessage & answer,
  CallSession & session
)
{
  PString token = session.GetInternalHeader( "OpalConnectionId" );
  
  PSafePtr<OpalConnection> opalConnection = 
    GetConnectionWithLock(token, PSafeReference);
  
  if (opalConnection == NULL) 
    return FALSE;
 
  OpalOSSConnection * ossCon = (OpalOSSConnection *)(OpalConnection*)opalConnection;

  return ossCon->OnRequireSDPAnswer( offer, answer );
}

BOOL OpalOSSEndPoint::OnRequireSDPOffer(
  SIPMessage & offer,
  CallSession & session
)
{
  PString token = session.GetInternalHeader( "OpalConnectionId" );
  
  PSafePtr<OpalConnection> opalConnection = 
    GetConnectionWithLock(token, PSafeReference);
  
  if (opalConnection == NULL) 
    return FALSE;
 
  OpalOSSConnection * ossCon = (OpalOSSConnection *)(OpalConnection*)opalConnection;

  return ossCon->OnRequireSDPOffer( offer );
}

SIPTransportManager & OpalOSSEndPoint::GetTransportManager()
{
  SIPTransportManager * mgr = m_OSSUserAgent->GetStack().GetTransportManager();
  return *mgr;
}

OpalTransport * OpalOSSEndPoint::GetOpalTransport(
  const PIPSocket::Address & target
)
{
  SIPTransportManager * mgr = m_OSSUserAgent->GetStack().GetTransportManager();
  SIPUDPTransport * transport = (SIPUDPTransport *)mgr->GetUDPTransport();
  if( transport == NULL )
    return NULL;

  PUDPSocket * sock = transport->GetSocket( target, transport->GetListenerPort() );
  if( sock == NULL )
    return NULL;

  return new DummyOpalTransport( *this, *sock );
}

void OpalOSSEndPoint::OnReceivedInstantMessage(
  const SIPMessage & message
)
{
}

void OpalOSSEndPoint::OnRegEventStateOnline(
  const SIPURI & resourceURI
)
{
}

void OpalOSSEndPoint::OnRegEventStateOffLine(
  const SIPURI & resourceURI
)
{
}

////////////////////////////////////////////////////////////////////////


OpalRFC3680Client::OpalRFC3680Client(
  OpalOSSUserAgent & ua
  ) : RFC3680Client( ua ), 
      m_OpalOSSUserAgent( ua )
{
}

void OpalRFC3680Client::OnStateOnline(
  const SIPURI & resourceURI
)
{
  m_OpalOSSUserAgent.OnRegEventStateOnline( resourceURI );
}

void OpalRFC3680Client::OnStateOffLine(
  const SIPURI & resourceURI
)
{
  m_OpalOSSUserAgent.OnRegEventStateOffLine( resourceURI );
}


////////////////////////////////////////////////////////////////////////


OpalOSSUserAgent::OpalOSSUserAgent( 
  OpalOSSEndPoint & ep
) : m_OpalEndPoint( ep )
{
  m_Core = new OpalOSSCore( *this, ep );
  m_Registrar = new OpalOSSRegistrar( *this, ep );
  m_RFC3680Client = new RFC3680Client( *this );
}

OpalOSSUserAgent::~OpalOSSUserAgent()
{
  delete m_Core;
  m_Core = NULL;

  delete m_Registrar;
  m_Registrar = NULL;

  delete m_RFC3680Client;
  m_RFC3680Client = NULL;
}

BOOL OpalOSSUserAgent::Initialize(
  PINDEX threadCount
)
{
  SIPUserAgent::Initialize( threadCount );
  return StartTransportThreads();
}

/// SIPUserAgent virtual func override
void OpalOSSUserAgent::ProcessEvent(
    SIPStackEvent * event
)
{
  const TransactionId & transactionId = event->GetTransactionId();
  PString method = transactionId.GetMethod().ToUpper();
  PString callId = transactionId.GetCallId();

  if( method == "REGISTER" )
  {
    m_Registrar->ProcessStackEvent( event );
  }else
  {
    SIPSession::GCRef autoRef =  m_RFC3680Client->FindGCRefByCallId( callId );
    if( autoRef != NULL )
       m_RFC3680Client->ProcessStackEvent( autoRef, event );
    else
      m_Core->ProcessStackEvent( event );
  }
}

void OpalOSSUserAgent::OnRegEventStateOnline(
  const SIPURI & resourceURI
)
{
  m_Core->OnRegEventStateOnline( resourceURI );
}

void OpalOSSUserAgent::OnRegEventStateOffLine(
  const SIPURI & resourceURI
)
{
  m_Core->OnRegEventStateOffLine( resourceURI );
}

/// RFC 3680 support
void OpalOSSUserAgent::WatchRegistrationState(
  const PString & stateAgent,
  const PString & resourceURI,
  const PTimeInterval & expires,
  const PString & authUser,
  const PString & authPassword
  
)
{
  m_RFC3680Client->WatchRegistrationState( stateAgent, resourceURI, expires, authUser, authPassword );
}

void OpalOSSUserAgent::StopWatchingRegistrationState(
  const PString & resourceURI
)
{
  m_RFC3680Client->StopWatchingRegistrationState( resourceURI );
}

////////////////////////////////////////////////////////////////////////

OpalOSSRegistrar::OpalOSSRegistrar( 
  OpalOSSUserAgent & ua,
  OpalOSSEndPoint & ep
) : RegisterSessionManager( ua, OPAL_REGISTRAR_THREAD_COUNT, OPAL_REGISTRAR_STACK_SIZE ),
    m_OpalEndPoint( ep )
{
}

OpalOSSRegistrar::~OpalOSSRegistrar()
{
}

RegisterSession::AcceptRegistrationResponse OpalOSSRegistrar::OnAcceptRegistration(
  RegisterSession & session,
  const SIPMessage & request
)
{
  return m_OpalEndPoint.OnAcceptRegistration( session, request );
}

void OpalOSSRegistrar::OnRegistrationAccepted(
  RegisterSession & session,
  const SIPMessage & request
)
{
  m_OpalEndPoint.OnRegistrationAccepted( session, request );
}

void OpalOSSRegistrar::OnRegistrationRejected(
  RegisterSession & session,
  const SIPMessage & request
)
{
  m_OpalEndPoint.OnRegistrationRejected( session, request );
}

void OpalOSSRegistrar::OnRegistrationTimeout(
  RegisterSession & session
)
{
  SIPMessage dummy;
  m_OpalEndPoint.OnRegistrationRejected( session, dummy );
}

void OpalOSSRegistrar::OnUnregistration(
  RegisterSession & session,
  const SIPMessage & request
)
{
  m_OpalEndPoint.OnUnregistration( session, request );
}

BOOL OpalOSSRegistrar::OnRequestA1Hash(
  const SIPURI & userURL,
  PString & a1,
  RegisterSession & session
)
{
  return m_OpalEndPoint.OnRequestA1Hash( userURL, a1, session );
}



////////////////////////////////////////////////////////////////////////

OpalOSSCore::OpalOSSCore( 
  OpalOSSUserAgent & ua,
  OpalOSSEndPoint & ep
) : CallSessionManager( ua, OPAL_CORE_THREAD_COUNT, OPAL_STACK_SIZE ),
    m_OpalEndPoint( ep )
{
}

OpalOSSCore::~OpalOSSCore()
{
}

SIPSession::GCRef OpalOSSCore::MakeCall(
  SIPURI & remoteURI,
  const PString & callToken,
  BOOL hashCall
)
{
  PString callId = ParserTools::GenGUID();
  ProfileUA defaultProfile = m_OpalEndPoint.GetProfile();

  CallSession * callSession = (CallSession*)CreateClientSession( defaultProfile, callId );
  callSession->SetXORHashOutgoingCalls( hashCall );

  if( callSession == NULL )
    return SIPSession::GCRef( NULL, "SIPSession" );

  SIPSession::GCRef call = callSession->GCCreateRef();
  callSession->AddInternalHeader( "OpalConnectionId", callToken );

  if( !callSession->MakeCall( remoteURI ) )
    return SIPSession::GCRef( NULL, "SIPSession" );

  return call;
}

CallSession::AnswerCallResponse OpalOSSCore::OnAnswerCall(
  const SIPMessage & invite,
  CallSession & session
)
{
  return CallSession::AnswerCallDeferred;
}

void OpalOSSCore::OnConnected(
  CallSession & session,
  const SIPMessage & msg
)
{
  m_OpalEndPoint.OnConnected( session, msg );
  m_OpalEndPoint.OnEstablished( session );
}

void OpalOSSCore::OnEstablished(
  CallSession & session
)
{
  m_OpalEndPoint.OnEstablished( session );
}

/// will be called when the call is disconnected
void OpalOSSCore::OnDisconnected(
  CallSession & session,
  const SIPMessage & message
)
{
  m_OpalEndPoint.OnDisconnected( session, message );
}


/// Progress callbacks
void OpalOSSCore::OnAlerting(
  CallSession & session,
  const SIPMessage & message
)
{
  m_OpalEndPoint.OnAlerting( session, message );
}

void OpalOSSCore::OnProgress(
  CallSession & session,
  const SIPMessage & message
)
{
  m_OpalEndPoint.OnProgress( session, message );

  /// start media channels
  m_OpalEndPoint.OnConnected( session, message );
}

/// returning TRUE here will give permission to the session
/// to resend the invite to the new contact header in the response.
/// returning FALSE will end the connection
BOOL OpalOSSCore::OnCallRedirect(
  CallSession & session,
  const SIPMessage & redirect
)
{
  return m_OpalEndPoint.OnCallRedirect( session, redirect );
}

/// returning FALSE here will reject the call with no acceptable format
/// depending on call state.  Offers received in ACK will be disconnected
/// using a BYE.  
BOOL OpalOSSCore::OnIncomingSDPOffer(
  const SIPMessage & sdp,
  CallSession & session
)
{
  return m_OpalEndPoint.OnIncomingSDPOffer( sdp, session );
}

/// your last chance to produce an answer to the offer.  
/// This will be called either before sending 183, 200 or ACK.
/// AnsweCall is therefore a prerequisite before this callback
/// is called
BOOL OpalOSSCore::OnRequireSDPAnswer(
  const SIPMessage & offer,
  SIPMessage & answer,
  CallSession & session
)
{
  return m_OpalEndPoint.OnRequireSDPAnswer( offer, answer, session );
}

BOOL OpalOSSCore::OnRequireSDPOffer(
  SIPMessage & offer,
  CallSession & session
)
{
  return m_OpalEndPoint.OnRequireSDPOffer( offer, session );
}


BOOL OpalOSSCore::OnIncomingConnection(
  const SIPMessage & invite,
  CallSession & session
)
{
  return m_OpalEndPoint.OnIncomingConnection( invite, session );
}

BOOL OpalOSSCore::OnSetupIncomingCall(
  const SIPMessage & invite,
  CallSession & session
)
{
  return m_OpalEndPoint.OnSetupIncomingCall( invite, session );
}

void OpalOSSCore::OnReceivedInstantMessage(
  const SIPMessage & message
)
{
  LOG_IF_DEBUG( LogDebugVeryHigh(), message );
  m_OpalEndPoint.OnReceivedInstantMessage( message );
}

void OpalOSSCore::OnRegEventStateOnline(
  const SIPURI & resourceURI
)
{
  m_OpalEndPoint.OnRegEventStateOnline( resourceURI );
}

void OpalOSSCore::OnRegEventStateOffLine(
  const SIPURI & resourceURI
)
{
  m_OpalEndPoint.OnRegEventStateOffLine( resourceURI );
}

#endif  // HAS_OPAL


