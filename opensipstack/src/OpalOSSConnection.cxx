/*
 *
 * OpalOSSConnection.cxx
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
 * $Log: OpalOSSConnection.cxx,v $
 * Revision 1.23  2006/12/08 09:56:13  joegenbaclor
 * Added Hold functions
 *
 * Revision 1.22  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.21  2006/10/12 05:22:42  joegenbaclor
 * Enabled ability to make encrypted outbound calls for OPAL
 *
 * Revision 1.20  2006/10/03 08:41:01  joegenbaclor
 * Added INFO DTMF relay support
 *
 * Revision 1.19  2006/10/03 04:28:45  joegenbaclor
 * Added INFO DTMF relay support
 *
 * Revision 1.18  2006/08/26 14:31:44  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.17  2006/08/15 15:42:09  joegenbaclor
 * 1. Added Instant Messaging functions to OPAL
 *
 * Revision 1.16  2006/06/28 14:15:00  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.15  2006/06/26 03:06:06  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to
 * avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL
 * componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 1.14  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.13  2006/04/20 14:32:28  joegenbaclor
 * Fixed early media bug where soucd channels in OPAL ay not be opened in time for ringback to start streaming.
 *
 * Revision 1.12  2006/04/18 07:18:21  joegenbaclor
 * Fixed unix build environment errors and warnings
 *
 * Revision 1.11  2006/04/08 02:38:15  joegenbaclor
 * 1.  Corrected bug in call session where  OnConnected() is called befoe OnSDPOffe.
 * 2.  Corrected bug where Cancel syncpoint may not be signalled making calls to SendCancel block
 *
 * Revision 1.10  2006/04/07 07:47:20  joegenbaclor
 * More work on OpenPhone/OPAL
 *
 * Revision 1.9  2006/04/06 04:27:51  joegenbaclor
 * More work on OPAL Inbound
 *
 * Revision 1.8  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.7  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.6  2006/03/20 03:51:14  joegenbaclor
 * This upload marks the first successful OPAL call using opensipstack!!!   There is still some pending issues
 * in 183 Progress handling in OPAL classes but a fix should with the next revision.
 *
 * Revision 1.5  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.4  2006/03/15 16:40:46  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */


#include "OpalOSSConnection.h"

#if HAS_OPAL

//#ifdef _MSC_VER
//#ifndef _WIN32_WCE
//#pragma comment(lib, OPAL_LIBRARY)
//#endif // !_WIN32_WCE
//#endif


#include <codec/rfc2833.h>
#include <opal/manager.h>
#include <opal/call.h>
#include <opal/patch.h>



using namespace OPAL;

#define new PNEW

/////////////////////////////////////////////////////////////////////////

OpalOSSConnection::OpalOSSConnection(
  OpalCall & call,          ///<  Owner calll for connection
  OpalOSSEndPoint & endpoint,  ///<  Owner endpoint for connection
  const PString & token,     ///<  Token to identify the connection
  const PString & remoteParty
) : OpalConnection( call, endpoint, token ),
    m_RemoteParty( remoteParty )
{
  m_OpalTransport = NULL;
  m_LocalHold = FALSE;
  m_RemoteHold = FALSE;
  m_HashCall = FALSE;
}

/**Destroy connection.
  */
OpalOSSConnection::~OpalOSSConnection()
{
  CallSession * cs = GetSIPSession();
 
  if( cs != NULL )
  {
    cs->Destroy();
  }

  if( m_OpalTransport != NULL )
    delete m_OpalTransport;
}

/// OPAL CONNECTION CALLBACKS

void OpalOSSConnection::TransferConnection(
  const PString & remoteParty,   ///<  Remote party to transfer the existing call to
  const PString & callIdentity
                                ///<  Call Identity of secondary call if present
)
{
}

/**Put the current connection on hold, suspending all media streams.
  */
void OpalOSSConnection::HoldConnection()
{
  /// get the local SDP
}

/**Retrieve the current connection from hold, activating all media 
  * streams.
  */
void OpalOSSConnection::RetrieveConnection()
{
}

/**Return TRUE if the current connection is on hold.
  */
BOOL OpalOSSConnection::IsConnectionOnHold()
{
  return ( m_LocalHold || m_RemoteHold );
}


/**Start an outgoing connection.
    This function will initiate the connection to the remote entity, for
    example in H.323 it sends a SETUP, in SIP it sends an INVITE etc.

    The default behaviour is pure.
  */
BOOL OpalOSSConnection::SetUpConnection()
{
  OpalOSSEndPoint & endpoint = (OpalOSSEndPoint &)GetEndPoint();
  OpalOSSUserAgent * userAgent = endpoint.GetUserAgent();
  SIPURI remoteParty = m_RemoteParty;
  remoteParty.SetScheme( "sip" );
  m_OSSConnection = userAgent->GetCore()->MakeCall( remoteParty, GetToken(), m_HashCall );
  return m_OSSConnection != NULL;
}

BOOL OpalOSSConnection::OnIncomingCall(
    CallSession & session
)
{
  m_OSSConnection = session.GCCreateRef();
  // indicate the other is to start ringing
  if (!OnIncomingConnection()) {
    //PTRACE(2, "SIP\tOnIncomingConnection failed for INVITE from " << request.GetURI() << " for " << *this);
    Release();
    return FALSE;
  }
  if ((phase == ReleasingPhase) || (phase == ReleasedPhase)) {
    //PTRACE(1, "H225\tApplication called ClearCall during OnIncomingCall");
    return FALSE;
  }


  //PTRACE(2, "SIP\tOnIncomingConnection succeeded for INVITE from " << request.GetURI() << " for " << *this);
  phase = SetUpPhase;
  ownerCall.OnSetUp(*this);

  AnsweringCall(OnAnswerCall(m_RemoteParty));

  return TRUE;
}


/**Indicate to remote endpoint an alert is in progress.
    If this is an incoming connection and the AnswerCallResponse is in a
    AnswerCallDeferred or AnswerCallPending state, then this function is
    used to indicate to that endpoint that an alert is in progress. This is
    usually due to another connection which is in the call (the B party)
    has received an OnAlerting() indicating that its remoteendpoint is
    "ringing".

    The default behaviour is pure.
  */
BOOL OpalOSSConnection::SetAlerting(
  const PString & calleeName,   ///<  Name of endpoint being alerted.
  BOOL withMedia                ///<  Open media with alerting
)
{
  if (IsOriginating()) {
    PTRACE(2, "SIP\tSetAlerting ignored on call we originated.");
    return TRUE;
  }

  PTRACE(2, "SIP\tSetAlerting");

  if (phase != SetUpPhase) 
    return FALSE;

  phase = AlertingPhase;

  return TRUE;
}

/**Indicate the result of answering an incoming call.
    This should only be called if the OnAnswerCall() callback function has
    returned a AnswerCallPending or AnswerCallDeferred response.

    Note sending further AnswerCallPending responses via this function will
    have the result of notification PDUs being sent to the remote endpoint (if possible).
    In this way multiple notification PDUs may be sent.

    Sending a AnswerCallDeferred response would have no effect.
  */
void OpalOSSConnection::AnsweringCall(
  AnswerCallResponse response ///<  Answer response to incoming call
)
{
    switch (phase) {
    case SetUpPhase:
    case AlertingPhase:
      switch (response) {
        case AnswerCallNow:
          SetConnected();
          GetSIPSession()->AnswerCall( CallSession::AnswerCallNow );
          OnEstablished();
          break;

        case AnswerCallDenied:
          PTRACE(1, "SIP\tApplication has declined to answer incoming call");
          Release(EndedByAnswerDenied);
          break;

        case AnswerCallPending:
          SetAlerting(localPartyName, FALSE);
          break;

        case AnswerCallAlertWithMedia:
          SetAlerting(localPartyName, TRUE);
          break;

        case AnswerCallDeferred:
        case AnswerCallDeferredWithMedia:
	      case NumAnswerCallResponses:
          break;
      }
      break;

    // 
    // cannot answer call in any of the following phases
    //
    case ConnectedPhase:
    case UninitialisedPhase:
    case EstablishedPhase:
    case ReleasingPhase:
    case ReleasedPhase:
    default:
      break;
  }
}

/**A call back function whenever a connection is "connected".
    This indicates that a connection to an endpoint was connected. That
    is the endpoint received acknowledgement via whatever protocol it uses
    that the connection may now start media streams.

    In the context of H.323 this means that the CONNECT pdu has been
    received.

    The default behaviour calls the OpalEndPoint function of the same name.
  */
void OpalOSSConnection::OnConnected()
{
  // switch phase 
  connectedTime = PTime ();

  phase = ConnectedPhase;
  OpalConnection::OnConnected();

  // init DTMF handler
  InitRFC2833Handler();
}

void OpalOSSConnection::OnEstablished()
{
  phase = EstablishedPhase;
  OpalConnection::OnEstablished();
}

/**Indicate to remote endpoint we are connected.

    The default behaviour is pure.
  */
BOOL OpalOSSConnection::SetConnected()
{
  PTRACE(2, "SIP\tSetConnected");

  if (IsOriginating()) {
    PTRACE(2, "SIP\tSetConnected ignored on call we originated.");
    return TRUE;
  }

  OnConnected();

  GetSIPSession()->AnswerCall( CallSession::AnswerCallNow );
  OnEstablished();

  return TRUE;
}

void OpalOSSConnection::InitRFC2833Handler()
{
  if (rfc2833Handler != NULL) {
    for (int i = 0; i < mediaStreams.GetSize(); i++) {
      OpalMediaStream & mediaStream = mediaStreams[i];
      if (mediaStream.GetSessionID() == OpalMediaFormat::DefaultAudioSessionID) {
	OpalMediaPatch * patch = mediaStream.GetPatch();
	if (patch != NULL) {
	  if (mediaStream.IsSource()) {
	    patch->AddFilter(rfc2833Handler->GetReceiveHandler(), mediaStream.GetMediaFormat());
	  }
	  else {
	    patch->AddFilter(rfc2833Handler->GetTransmitHandler());
	  }
	}
      }
	}
  }
}
BOOL OpalOSSConnection::SendUserInputTone(
  char tone,        ///<  DTMF tone code
  unsigned duration  ///<  Duration of tone in milliseconds
)
{
  if (duration == 0)
    duration = 180;

  if( sendUserInputMode == OpalConnection::SendUserInputAsInlineRFC2833 )
    return rfc2833Handler->SendTone(tone, duration);

  return GetSIPSession()->SendINFODTMF( tone, duration );
}

/**Clean up the termination of the connection.
    This function can do any internal cleaning up and waiting on background
    threads that may be using the connection object.

    Note that there is not a one to one relationship with the
    OnEstablishedConnection() function. This function may be called without
    that function being called. For example if SetUpConnection() was used
    but the call never completed.

    Classes that override this function should make sure they call the
    ancestor version for correct operation.

    An application will not typically call this function as it is used by
    the OpalManager during a release of the connection.

    The default behaviour calls the OpalEndPoint function of the same name.
  */
void OpalOSSConnection::OnReleased()
{
  // Close media
  {
    PWaitAndSignal lock( m_StreamsMutex );
    CloseMediaStreams();
  }


  CallSession * cs = GetSIPSession();
  if( cs != NULL )
    cs->SendBye();

  SetPhase(ReleasedPhase);

  OpalConnection::OnReleased();
}

/**Forward incoming call to specified address.
    This would typically be called from within the OnIncomingCall()
    function when an application wishes to redirct an unwanted incoming
    call.

    The return value is TRUE if the call is to be forwarded, FALSE
    otherwise. Note that if the call is forwarded the current connection is
    cleared with teh ended call code of EndedByCallForwarded.
  */
BOOL OpalOSSConnection::ForwardCall(
  const PString & forwardParty   ///<  Party to forward call to.
)
{
  return FALSE;
}

/**Get the data formats this connection is capable of operating.
    This provides a list of media data format names that a
    OpalMediaStream may be created in within this connection.

    The default behaviour is pure.
  */
OpalMediaFormatList OpalOSSConnection::GetMediaFormats() const
{
  return m_RemoteFormatList;
}

/**Open source transmitter media stream for session.
  */
BOOL OpalOSSConnection::OpenSourceMediaStream(
  const OpalMediaFormatList & mediaFormats, ///<  Optional media format to open
  unsigned sessionID                   ///<  Session to start stream on
)
{
  // The remote user is in recvonly mode or in inactive mode for that session
  switch (m_RemoteSDP.GetDirection(sessionID)) {
    case SDPMediaDescription::Inactive :
    case SDPMediaDescription::RecvOnly :
      return FALSE;

    default :
      return OpalConnection::OpenSourceMediaStream(mediaFormats, sessionID);
  }
}

/**Open source transmitter media stream for session.
  */
OpalMediaStream * OpalOSSConnection::OpenSinkMediaStream(
  OpalMediaStream & source    ///<  Source media sink format to open to
)
{
  // The remote user is in sendonly mode or in inactive mode for that session
  switch (m_RemoteSDP.GetDirection(source.GetSessionID())) {
    case SDPMediaDescription::Inactive :
    case SDPMediaDescription::SendOnly :
      return NULL;

    default :
      return OpalConnection::OpenSinkMediaStream(source);
  }
}

/**Create a new media stream.
    This will create a media stream of an appropriate subclass as required
    by the underlying connection protocol. For instance H.323 would create
    an OpalRTPStream.

    The sessionID parameter may not be needed by a particular media stream
    and may be ignored. In the case of an OpalRTPStream it us used.

    Note that media streams may be created internally to the underlying
    protocol. This function is not the only way a stream can come into
    existance.

    The default behaviour is pure.
  */
OpalMediaStream * OpalOSSConnection::CreateMediaStream(
  const OpalMediaFormat & mediaFormat, ///<  Media format for stream
  unsigned sessionID,                  ///<  Session number for stream
  BOOL isSource                        ///<  Is a source stream
)
{
  if (ownerCall.IsMediaBypassPossible(*this, sessionID))
    return new OpalNullMediaStream(mediaFormat, sessionID, isSource);

  if (rtpSessions.GetSession(sessionID) == NULL)
    return NULL;


  return new OpalRTPMediaStream(mediaFormat, isSource, *rtpSessions.GetSession(sessionID),
				endpoint.GetManager().GetMinAudioJitterDelay(),
				endpoint.GetManager().GetMaxAudioJitterDelay());
}

/**See if the media can bypass the local host.

    The default behaviour returns FALSE indicating that media bypass is not
    possible.
  */
BOOL OpalOSSConnection::IsMediaBypassPossible(
  unsigned sessionID ///<  Session ID for media channel
) const
{
  return FALSE;
}


BOOL OpalOSSConnection::OnIncomingSDPOffer(
  const SIPMessage & _sdp
)
{
  PString sdp = _sdp.GetBody();
  m_RemoteSDP.Decode( sdp );
  OnReceivedSDPMediaDescription(m_RemoteSDP, SDPMediaDescription::Audio, OpalMediaFormat::DefaultAudioSessionID);
  m_RemoteFormatList += OpalRFC2833;
  OnReceivedSDPMediaDescription(m_RemoteSDP, SDPMediaDescription::Video, OpalMediaFormat::DefaultVideoSessionID);
  return TRUE;
}

BOOL OpalOSSConnection::OnRequireSDPAnswer(
  const SIPMessage & offer,
  SIPMessage & answer
)
{
  SDPSessionDescription sdpOut(GetLocalAddress());
  BOOL failure = OnSendSDPMediaDescription(m_RemoteSDP, SDPMediaDescription::Audio, OpalMediaFormat::DefaultAudioSessionID, sdpOut);
  failure = OnSendSDPMediaDescription(m_RemoteSDP, SDPMediaDescription::Video, OpalMediaFormat::DefaultVideoSessionID, sdpOut) && failure;
  
  if(failure)
    Release(EndedByCapabilityExchange);

  PString sdp = sdpOut.Encode();
  answer.SetContentType( "application/sdp" );
  answer.SetBody( sdp );

  m_LocalSDP = sdpOut;
 
  return !failure;
}

BOOL OpalOSSConnection::OnRequireSDPOffer(
  SIPMessage & offer
)
{
  SDPSessionDescription * sdp = NULL;
  BuildSDP(sdp, rtpSessions, OpalMediaFormat::DefaultAudioSessionID);
  if ( GetEndPoint().GetManager().CanAutoStartTransmitVideo()
       || GetEndPoint().GetManager().CanAutoStartReceiveVideo())
    BuildSDP(sdp, rtpSessions, OpalMediaFormat::DefaultVideoSessionID);

  if( sdp == NULL )
    return FALSE;
  
  offer.SetSDP( sdp->Encode() );

  m_LocalSDP = *sdp;

  delete sdp;

  return TRUE;
}

BOOL OpalOSSConnection::OnReceivedSDPMediaDescription(SDPSessionDescription & sdp,
                                                  SDPMediaDescription::MediaType mediaType,
                                                  unsigned rtpSessionId)
{
  /*
  RTP_UDP *rtpSession = NULL;
  SDPMediaDescription * mediaDescription = sdp.GetMediaDescription(mediaType);
  
  if (mediaDescription == NULL) {
    PTRACE(1, "SIP\tCould not find SDP media description for " << mediaType);
    return FALSE;
  }

  // Create the RTPSession
  OpalTransportAddress localAddress;
  OpalTransportAddress address = mediaDescription->GetTransportAddress();
  rtpSession = OnUseRTPSession(rtpSessionId, address, localAddress);
  
  if (rtpSession == NULL) {
    Release(EndedByTransportFail);
    return FALSE;
  }

  // Adjust the remote formats list
  m_RemoteFormatList += mediaDescription->GetMediaFormats(rtpSessionId);
  AdjustMediaFormats(m_RemoteFormatList);
  
  // Open the streams and the reverse streams
  OnOpenSourceMediaStreams(m_RemoteFormatList, rtpSessionId, NULL);

  // set the remote address after the stream is opened
  PIPSocket::Address ip;
  WORD port;
  address.GetIpAndPort(ip, port);
  if (!rtpSession->SetRemoteSocketInfo(ip, port, TRUE)) {
    PTRACE(1, "SIP\tCannot set remote ports on RTP session");
    ReleaseSession(rtpSessionId);
    return FALSE;
  }

  // init DTMF handler
  //InitRFC2833Handler();

  
  // switch phase 
  //connectedTime = PTime ();

  //phase = ConnectedPhase;
  //OpalConnection::OnConnected();

  return TRUE;
  */

    RTP_UDP *rtpSession = NULL;
  SDPMediaDescription * mediaDescription = sdp.GetMediaDescription(mediaType);
  
  if (mediaDescription == NULL) {
    PTRACE(1, "SIP\tCould not find SDP media description for " << mediaType);
    return FALSE;
  }

  // Create the RTPSession
  OpalTransportAddress localAddress;
  OpalTransportAddress address = mediaDescription->GetTransportAddress();
  rtpSession = OnUseRTPSession(rtpSessionId, address, localAddress);
  if (rtpSession == NULL && !ownerCall.IsMediaBypassPossible(*this, rtpSessionId)) {
    Release(EndedByTransportFail);
    return FALSE;
  }

  // When receiving an answer SDP, keep the remote SDP media formats order
  // but remove the media formats we do not support.
  m_RemoteFormatList += mediaDescription->GetMediaFormats(rtpSessionId);
  m_RemoteFormatList.Remove(endpoint.GetManager().GetMediaFormatMask());

  // create map for RTP payloads
  mediaDescription->CreateRTPMap(rtpSessionId, rtpPayloadMap);
  
  // Open the streams and the reverse streams
  OnOpenSourceMediaStreams(m_RemoteFormatList, rtpSessionId, NULL);

  // set the remote address after the stream is opened
  PIPSocket::Address ip;
  WORD port;
  address.GetIpAndPort(ip, port);
  if (rtpSession && !rtpSession->SetRemoteSocketInfo(ip, port, TRUE)) {
    PTRACE(1, "SIP\tCannot set remote ports on RTP session");
    Release(EndedByTransportFail);
    return FALSE;
  }

  return TRUE;
}

BOOL OpalOSSConnection::BuildSDP(
  SDPSessionDescription *& sdp, 
  RTP_SessionManager & rtpSessions,
  unsigned rtpSessionId
)
{
  OpalTransportAddress localAddress;
  RTP_DataFrame::PayloadTypes ntePayloadCode = RTP_DataFrame::IllegalPayloadType;

  if (rtpSessionId == OpalMediaFormat::DefaultVideoSessionID && !endpoint.GetManager().CanAutoStartReceiveVideo() && !endpoint.GetManager().CanAutoStartTransmitVideo())
    return FALSE;

  if (ownerCall.IsMediaBypassPossible(*this, rtpSessionId)) {
    OpalConnection * otherParty = GetCall().GetOtherPartyConnection(*this);
    if (otherParty != NULL) {
      MediaInformation info;
      if (otherParty->GetMediaInformation(rtpSessionId, info)) {
        localAddress = info.data;
        ntePayloadCode = info.rfc2833;
      }
    }
  }

  if (localAddress.IsEmpty()) {
    /* We are not doing media bypass, so must have an RTP session.
       Due to the possibility of several INVITEs going out, all with different
       transport requirements, we actually need to use an rtpSession dictionary
       for each INVITE and not the one for the connection. Once an INVITE is
       accepted the rtpSessions for that INVITE is put into the connection. */
    RTP_Session * rtpSession = rtpSessions.UseSession(rtpSessionId);
    if (rtpSession == NULL) {

      // Not already there, so create one
      rtpSession = CreateSession(GetTransport(), rtpSessionId, NULL);
      if (rtpSession == NULL) {
	      Release(OpalConnection::EndedByTransportFail);
	      return FALSE;
      }

      rtpSession->SetUserData(new OpalOSSRTPCallBack(*this));

      // add the RTP session to the RTP session manager in INVITE
      rtpSessions.AddSession(rtpSession);
    }

    localAddress = GetLocalAddress(((RTP_UDP *)rtpSession)->GetLocalDataPort());
  }

  if (sdp == NULL)
    sdp = new SDPSessionDescription(localAddress);

  SDPMediaDescription * localMedia = new SDPMediaDescription(localAddress, (rtpSessionId == OpalMediaFormat::DefaultAudioSessionID)?SDPMediaDescription::Audio:SDPMediaDescription::Video);

  // Set format if we have an RTP payload type for RFC2833
  if (rtpSessionId == OpalMediaFormat::DefaultAudioSessionID) {

    if (ntePayloadCode != RTP_DataFrame::IllegalPayloadType) {
      PTRACE(3, "SIP\tUsing bypass RTP payload " << ntePayloadCode << " for NTE");
      localMedia->AddSDPMediaFormat(new SDPMediaFormat("0-15", ntePayloadCode));
    }
    else {
      ntePayloadCode = rfc2833Handler->GetPayloadType();
      if (ntePayloadCode == RTP_DataFrame::IllegalPayloadType) {
	      ntePayloadCode = OpalRFC2833.GetPayloadType();
      }

      if (ntePayloadCode != RTP_DataFrame::IllegalPayloadType) {
	      PTRACE(3, "SIP\tUsing RTP payload " << ntePayloadCode << " for NTE");

	      // create and add the NTE media format
	      localMedia->AddSDPMediaFormat(new SDPMediaFormat("0-15", ntePayloadCode));
      }
      else {
      	PTRACE(2, "SIP\tCould not allocate dynamic RTP payload for NTE");
      }
    }

    rfc2833Handler->SetPayloadType(ntePayloadCode);
  }
  
  // add the formats
  OpalMediaFormatList formats = ownerCall.GetMediaFormats(*this, FALSE);
  AdjustMediaFormats(formats);

  localMedia->AddMediaFormats(formats, rtpSessionId, rtpPayloadMap);

  localMedia->SetDirection(GetDirection(rtpSessionId));
  sdp->AddMediaDescription(localMedia);
  return TRUE;
}

BOOL OpalOSSConnection::OnSendSDPMediaDescription(
  const SDPSessionDescription & sdpIn,
  SDPMediaDescription::MediaType rtpMediaType,
  unsigned rtpSessionId,
  SDPSessionDescription & sdpOut
)
{
  /*
  RTP_UDP * rtpSession = NULL;

  // if no matching media type, return FALSE
  SDPMediaDescription * incomingMedia = sdpIn.GetMediaDescription(rtpMediaType);
  if (incomingMedia == NULL) {
    PTRACE(2, "SIP\tCould not find matching media type for session " << rtpSessionId);
    return FALSE;
  }
  
  // Create the list of Opal format names for the remote end.
  // We will answer with the media format that will be opened.
  // When sending an answer SDP, remove media formats that we do not support.
  m_RemoteFormatList += incomingMedia->GetMediaFormats(rtpSessionId);
  m_RemoteFormatList.Remove(endpoint.GetManager().GetMediaFormatMask());
  if (m_RemoteFormatList.GetSize() == 0) {
    Release(EndedByTransportFail);
    return FALSE;
  }

  // find the payload type used for telephone-event, if present
  const SDPMediaFormatList & sdpMediaList = incomingMedia->GetSDPMediaFormats();
  BOOL hasTelephoneEvent = FALSE;
  PINDEX i;
  for (i = 0; !hasTelephoneEvent && (i < sdpMediaList.GetSize()); i++) {
    if (sdpMediaList[i].GetEncodingName() == "telephone-event") {
      rfc2833Handler->SetPayloadType(sdpMediaList[i].GetPayloadType());
      hasTelephoneEvent = TRUE;
      m_RemoteFormatList += OpalRFC2833;
    }
  }

  // Create the RTPSession if required
  OpalTransportAddress localAddress;
  OpalTransportAddress mediaAddress = incomingMedia->GetTransportAddress();
  rtpSession = OnUseRTPSession(rtpSessionId, mediaAddress, localAddress);
  if (rtpSession == NULL && !ownerCall.IsMediaBypassPossible(*this, rtpSessionId)) {
    Release(EndedByTransportFail);
    return FALSE;
  }

  // construct a new media session list 
  SDPMediaDescription * localMedia = new SDPMediaDescription(localAddress, rtpMediaType);

  // Open the streams and the reverse media streams
  BOOL reverseStreamsFailed = TRUE;
  reverseStreamsFailed = OnOpenSourceMediaStreams(m_RemoteFormatList, rtpSessionId, localMedia);
  
  // Add in the RFC2833 handler, if used
  if (hasTelephoneEvent) {
    localMedia->AddSDPMediaFormat(new SDPMediaFormat("0-15", rfc2833Handler->GetPayloadType()));
  }
  
  // set the remote address after the stream is opened
  PIPSocket::Address ip;
  WORD port;
  mediaAddress.GetIpAndPort(ip, port);
  if (rtpSession && !rtpSession->SetRemoteSocketInfo(ip, port, TRUE)) {
    PTRACE(1, "SIP\tCannot set remote ports on RTP session");
    Release(EndedByTransportFail);
    delete localMedia;
    return FALSE;
  }
  
  // No stream opened for this session, use the default SDP
  if (reverseStreamsFailed) {
    SDPSessionDescription *sdp = (SDPSessionDescription *) &sdpOut;
    if (!BuildSDP(sdp, rtpSessions, rtpSessionId)) {
      delete localMedia;
      return FALSE;
    }
    return TRUE;
  }

  localMedia->SetDirection(GetDirection(rtpSessionId));
  sdpOut.AddMediaDescription(localMedia);

  */

  RTP_UDP * rtpSession = NULL;

  // if no matching media type, return FALSE
  SDPMediaDescription * incomingMedia = sdpIn.GetMediaDescription(rtpMediaType);
  if (incomingMedia == NULL) {
    PTRACE(2, "SIP\tCould not find matching media type for session " << rtpSessionId);
    return FALSE;
  }
  
  // Create the list of Opal format names for the remote end.
  // We will answer with the media format that will be opened.
  // When sending an answer SDP, remove media formats that we do not support.
  m_RemoteFormatList += incomingMedia->GetMediaFormats(rtpSessionId);
  m_RemoteFormatList.Remove(endpoint.GetManager().GetMediaFormatMask());
  if (m_RemoteFormatList.GetSize() == 0) {
    Release(EndedByTransportFail);
    return FALSE;
  }

  // find the payload type used for telephone-event, if present
  const SDPMediaFormatList & sdpMediaList = incomingMedia->GetSDPMediaFormats();
  BOOL hasTelephoneEvent = FALSE;
  PINDEX i;
  for (i = 0; !hasTelephoneEvent && (i < sdpMediaList.GetSize()); i++) {
    if (sdpMediaList[i].GetEncodingName() == "telephone-event") {
      rfc2833Handler->SetPayloadType(sdpMediaList[i].GetPayloadType());
      hasTelephoneEvent = TRUE;
      m_RemoteFormatList += OpalRFC2833;
    }
  }

  // Create the RTPSession if required
  OpalTransportAddress localAddress;
  OpalTransportAddress mediaAddress = incomingMedia->GetTransportAddress();
  rtpSession = OnUseRTPSession(rtpSessionId, mediaAddress, localAddress);
  if (rtpSession == NULL && !ownerCall.IsMediaBypassPossible(*this, rtpSessionId)) {
    Release(EndedByTransportFail);
    return FALSE;
  }

  // construct a new media session list 
  SDPMediaDescription * localMedia = new SDPMediaDescription(localAddress, rtpMediaType);
  
  // create map for RTP payloads
  incomingMedia->CreateRTPMap(rtpSessionId, rtpPayloadMap);

  // Open the streams and the reverse media streams
  BOOL reverseStreamsFailed = TRUE;
  reverseStreamsFailed = OnOpenSourceMediaStreams(m_RemoteFormatList, rtpSessionId, localMedia);
  
  // Add in the RFC2833 handler, if used
  if (hasTelephoneEvent) {
    localMedia->AddSDPMediaFormat(new SDPMediaFormat("0-15", rfc2833Handler->GetPayloadType()));
  }
  
  // set the remote address after the stream is opened
  PIPSocket::Address ip;
  WORD port;
  mediaAddress.GetIpAndPort(ip, port);
  if (rtpSession && !rtpSession->SetRemoteSocketInfo(ip, port, TRUE)) {
    PTRACE(1, "SIP\tCannot set remote ports on RTP session");
    Release(EndedByTransportFail);
    delete localMedia;
    return FALSE;
  }
  
  // No stream opened for this session, use the default SDP
  if (reverseStreamsFailed) {
    SDPSessionDescription *sdp = (SDPSessionDescription *) &sdpOut;
    if (!BuildSDP(sdp, rtpSessions, rtpSessionId)) {
      delete localMedia;
      return FALSE;
    }
    return TRUE;
  }

  localMedia->SetDirection(GetDirection(rtpSessionId));
  sdpOut.AddMediaDescription(localMedia);

  return TRUE;
}

BOOL OpalOSSConnection::OnOpenSourceMediaStreams(const OpalMediaFormatList & m_RemoteFormatList, unsigned sessionId, SDPMediaDescription *localMedia)
{
  BOOL reverseStreamsFailed = TRUE;
  
  PWaitAndSignal m(m_StreamsMutex);
  ownerCall.OpenSourceMediaStreams(*this, m_RemoteFormatList, sessionId);

  for (PINDEX i = 0; i < mediaStreams.GetSize(); i++) {
    OpalMediaStream & mediaStream = mediaStreams[i];
    if (mediaStream.GetSessionID() == sessionId) {
      OpalMediaFormat mediaFormat = mediaStream.GetMediaFormat();
      if (OpenSourceMediaStream(mediaFormat, sessionId) && localMedia) {
	localMedia->AddMediaFormat(mediaStream.GetMediaFormat(), rtpPayloadMap);
	reverseStreamsFailed = FALSE;
      }
    }
  }

  return reverseStreamsFailed;
}


RTP_UDP * OpalOSSConnection::OnUseRTPSession(const unsigned rtpSessionId, const OpalTransportAddress & mediaAddress, OpalTransportAddress & localAddress)
{
    RTP_UDP *rtpSession = NULL;
  RTP_DataFrame::PayloadTypes ntePayloadCode = RTP_DataFrame::IllegalPayloadType;

  // if doing media bypass, we need to set the local address
  // otherwise create an RTP session
  if (ownerCall.IsMediaBypassPossible(*this, rtpSessionId)) {
    OpalConnection * otherParty = GetCall().GetOtherPartyConnection(*this);
    if (otherParty != NULL) {
      MediaInformation info;
      if (otherParty->GetMediaInformation(rtpSessionId, info)) {
        localAddress = info.data;
        ntePayloadCode = info.rfc2833;
      }
    }
    mediaTransportAddresses.SetAt(rtpSessionId, new OpalTransportAddress(mediaAddress));
  }
  else {
    // create an RTP session
    rtpSession = (RTP_UDP *)UseSession(GetTransport(), rtpSessionId, NULL);
    if (rtpSession == NULL) {
      return NULL;
    }
    
    // Set user data
    if (rtpSession->GetUserData() == NULL)
      rtpSession->SetUserData(new OpalOSSRTPCallBack(*this));

    // Local Address of the session
    localAddress = GetLocalAddress(rtpSession->GetLocalDataPort());
  }

  return rtpSession;
}

void OpalOSSConnection::OnRTPStatistics(
  const RTP_Session & session   ///<  Session with statistics
) const
{
}

CallSession * OpalOSSConnection::GetSIPSession()
{
  return dynamic_cast<CallSession*>(m_OSSConnection.GetObject());
}

SDPMediaDescription::Direction OpalOSSConnection::GetDirection(unsigned sessionId)
{
  if (m_RemoteHold)
    return SDPMediaDescription::RecvOnly;
  else if (m_LocalHold)
    return SDPMediaDescription::SendOnly;
  else if (sessionId == OpalMediaFormat::DefaultVideoSessionID) {

    if (endpoint.GetManager().CanAutoStartTransmitVideo() && !endpoint.GetManager().CanAutoStartReceiveVideo())
      return SDPMediaDescription::SendOnly;
    else if (!endpoint.GetManager().CanAutoStartTransmitVideo() && endpoint.GetManager().CanAutoStartReceiveVideo())
      return SDPMediaDescription::RecvOnly;
  }
  
  return SDPMediaDescription::Undefined;
}

OpalTransport & OpalOSSConnection::GetTransport() const
{
  return (const_cast< OpalOSSConnection * >( this ))->GetDummyTransport();
}

OpalTransport & OpalOSSConnection::GetDummyTransport()
{
  if( m_OpalTransport != NULL )
    return *m_OpalTransport;

  SIPURI remoteParty = m_RemoteParty;
  remoteParty.SetScheme( "sip" );

  /// resolve the remote
  PIPSocket::Address localAddress, targetAddress;
  WORD localPort, targetPort;
  SIPTransport::Resolve( remoteParty, targetAddress, targetPort );

  OpalTransportAddress remoteAddress( targetAddress, targetPort, "udp" );

  /// get the local listener
  OpalOSSEndPoint & ep = (OpalOSSEndPoint &)GetEndPoint();
  ep.GetTransportManager().GetListenerAddress( SIPTransport::UDP, targetAddress, localAddress, localPort );
  
  m_OpalTransport = ep.GetOpalTransport( targetAddress );

  m_OpalTransport->SetRemoteAddress( remoteAddress );

  return *m_OpalTransport;
}

OpalTransportAddress OpalOSSConnection::GetLocalAddress(WORD port) const
{
  GetTransport();  /// make sure that the transport is already created

  OpalTransport * transport = m_OpalTransport;

  PIPSocket::Address localIP;
  if (!transport->GetLocalAddress().GetIpAddress(localIP)) {
    PTRACE(1, "SIP\tNot using IP transport");
    return OpalTransportAddress();
  }

  PIPSocket::Address remoteIP;
  if (!transport->GetRemoteAddress().GetIpAddress(remoteIP)) {
    PTRACE(1, "SIP\tNot using IP transport");
    return OpalTransportAddress();
  }

  endpoint.GetManager().TranslateIPAddress(localIP, remoteIP);
  return OpalTransportAddress(localIP, port, "udp");
}


/////////////////////////////////////////////////////////////////////////////

OpalOSSRTPCallBack::OpalOSSRTPCallBack(const OpalOSSConnection & conn)
  : m_OSSConnection(conn)
{
}

void OpalOSSRTPCallBack::OnTxStatistics(const RTP_Session & session) const
{
  m_OSSConnection.OnRTPStatistics(session);
}


void OpalOSSRTPCallBack::OnRxStatistics(const RTP_Session & session) const
{
  m_OSSConnection.OnRTPStatistics(session);
}


#endif ///HAS_OPAL



