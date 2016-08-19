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
 * $Log: OpalOSSConnection.h,v $
 * Revision 1.17  2006/12/08 09:56:13  joegenbaclor
 * Added Hold functions
 *
 * Revision 1.16  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.15  2006/10/12 05:22:42  joegenbaclor
 * Enabled ability to make encrypted outbound calls for OPAL
 *
 * Revision 1.14  2006/10/03 04:28:43  joegenbaclor
 * Added INFO DTMF relay support
 *
 * Revision 1.13  2006/08/26 14:31:40  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.12  2006/08/03 12:48:05  joegenbaclor
 * Minor tweaks for integrating pwlib into OSS source tree
 *
 * Revision 1.11  2006/06/28 14:14:56  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.10  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.9  2006/04/20 14:32:28  joegenbaclor
 * Fixed early media bug where soucd channels in OPAL ay not be opened in time for ringback to start streaming.
 *
 * Revision 1.8  2006/04/18 07:18:21  joegenbaclor
 * Fixed unix build environment errors and warnings
 *
 * Revision 1.7  2006/04/18 04:31:52  hartley2481
 * *** empty log message ***
 *
 * Revision 1.6  2006/04/06 04:27:50  joegenbaclor
 * More work on OPAL Inbound
 *
 * Revision 1.5  2006/04/05 00:26:01  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.4  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.3  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.2  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef OPALOSSCONNECTION_H
#define OPALOSSCONNECTION_H



#include "ossbuildopts.h"

#if HAS_OPAL

#include <ptlib.h>
#include <opal/connection.h>
#include <opal/sdp.h>

#include "OpalOSSEndPoint.h"

class OpalCall;

namespace OPAL
{
  
  class OpalOSSConnection : public OpalConnection
  {
    PCLASSINFO( OpalOSSConnection, OpalConnection );
  public:

    /*
    OpalOSSConnection(
      OpalCall & call,          ///<  Owner calll for connection
      OpalOSSEndPoint & endpoint,  ///<  Owner endpoint for connection
      const PString & token     ///<  Token to identify the connection
    );*/


    OpalOSSConnection( 
      OpalCall & call, 
      OpalOSSEndPoint & endpoint, 
      const PString & token,
      const PString & remoteParty
    );

    /**Destroy connection.
     */
    ~OpalOSSConnection();

    /// OPAL CONNECTION CALLBACKS

    virtual void TransferConnection(
      const PString & remoteParty,   ///<  Remote party to transfer the existing call to
      const PString & callIdentity = PString::Empty()
                                    ///<  Call Identity of secondary call if present
    );

    /**Put the current connection on hold, suspending all media streams.
     */
    virtual void HoldConnection();

    /**Retrieve the current connection from hold, activating all media 
     * streams.
     */
    virtual void RetrieveConnection();

    /**Return TRUE if the current connection is on hold.
     */
    virtual BOOL IsConnectionOnHold();


    /**Start an outgoing connection.
       This function will initiate the connection to the remote entity, for
       example in H.323 it sends a SETUP, in SIP it sends an INVITE etc.

       The default behaviour is pure.
      */
    virtual BOOL SetUpConnection();

    virtual BOOL OnIncomingCall(
       CallSession & session
    );

    /**Indicate to remote endpoint an alert is in progress.
       If this is an incoming connection and the AnswerCallResponse is in a
       AnswerCallDeferred or AnswerCallPending state, then this function is
       used to indicate to that endpoint that an alert is in progress. This is
       usually due to another connection which is in the call (the B party)
       has received an OnAlerting() indicating that its remoteendpoint is
       "ringing".

       The default behaviour is pure.
      */
    virtual BOOL SetAlerting(
      const PString & calleeName,   ///<  Name of endpoint being alerted.
      BOOL withMedia                ///<  Open media with alerting
    );

    /**Indicate the result of answering an incoming call.
       This should only be called if the OnAnswerCall() callback function has
       returned a AnswerCallPending or AnswerCallDeferred response.

       Note sending further AnswerCallPending responses via this function will
       have the result of notification PDUs being sent to the remote endpoint (if possible).
       In this way multiple notification PDUs may be sent.

       Sending a AnswerCallDeferred response would have no effect.
      */
    virtual void AnsweringCall(
      AnswerCallResponse response ///<  Answer response to incoming call
    );

    /**A call back function whenever a connection is "connected".
       This indicates that a connection to an endpoint was connected. That
       is the endpoint received acknowledgement via whatever protocol it uses
       that the connection may now start media streams.

       In the context of H.323 this means that the CONNECT pdu has been
       received.

       The default behaviour calls the OpalEndPoint function of the same name.
      */
    virtual void OnConnected();

    virtual void OnEstablished();

    /**Indicate to remote endpoint we are connected.

       The default behaviour is pure.
      */
    virtual BOOL SetConnected();

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
    virtual void OnReleased();

    /**Forward incoming call to specified address.
       This would typically be called from within the OnIncomingCall()
       function when an application wishes to redirct an unwanted incoming
       call.

       The return value is TRUE if the call is to be forwarded, FALSE
       otherwise. Note that if the call is forwarded the current connection is
       cleared with teh ended call code of EndedByCallForwarded.
      */
    virtual BOOL ForwardCall(
      const PString & forwardParty   ///<  Party to forward call to.
    );

    /**Get the data formats this connection is capable of operating.
       This provides a list of media data format names that a
       OpalMediaStream may be created in within this connection.

       The default behaviour is pure.
      */
    virtual OpalMediaFormatList GetMediaFormats() const;

    /**Open source transmitter media stream for session.
      */
    virtual BOOL OpenSourceMediaStream(
      const OpalMediaFormatList & mediaFormats, ///<  Optional media format to open
      unsigned sessionID                   ///<  Session to start stream on
    );

    /**Open source transmitter media stream for session.
      */
    virtual OpalMediaStream * OpenSinkMediaStream(
      OpalMediaStream & source    ///<  Source media sink format to open to
    );

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
    virtual OpalMediaStream * CreateMediaStream(
      const OpalMediaFormat & mediaFormat, ///<  Media format for stream
      unsigned sessionID,                  ///<  Session number for stream
      BOOL isSource                        ///<  Is a source stream
    );

    /**See if the media can bypass the local host.

       The default behaviour returns FALSE indicating that media bypass is not
       possible.
     */
    virtual BOOL IsMediaBypassPossible(
      unsigned sessionID                  ///<  Session ID for media channel
    ) const;

    virtual BOOL OnIncomingSDPOffer(
      const SIPMessage & sdp
    );

    virtual BOOL OnRequireSDPAnswer(
      const SIPMessage & offer,
      SIPMessage & answer
    );

    virtual BOOL OnRequireSDPOffer(
      SIPMessage & offer
    );

    virtual BOOL BuildSDP(
      SDPSessionDescription *&,			     
      RTP_SessionManager & rtpSessions,
      unsigned rtpSessionId
    );

    virtual BOOL OnSendSDPMediaDescription(
      const SDPSessionDescription & sdpIn,
      SDPMediaDescription::MediaType rtpMediaType,
      unsigned rtpSessionId,
      SDPSessionDescription & sdpOut
    );

    virtual BOOL OnReceivedSDPMediaDescription(
      SDPSessionDescription & sdp,
      SDPMediaDescription::MediaType mediaType,
      unsigned sessionId
    );

    virtual RTP_UDP *OnUseRTPSession(
      const unsigned rtpSessionId,
      const OpalTransportAddress & mediaAddress,
      OpalTransportAddress & localAddress
    );

    virtual BOOL OnOpenSourceMediaStreams(
      const OpalMediaFormatList & remoteFormatList,
      unsigned sessionId,
      SDPMediaDescription *localMedia
    );

    SDPMediaDescription::Direction GetDirection(
      unsigned sessionId
    );

    OpalTransportAddress GetLocalAddress(WORD port = 0) const;

    virtual void OnRTPStatistics(
      const RTP_Session & session   ///<  Session with statistics
    ) const;

    void InitRFC2833Handler();

    CallSession * GetSIPSession();

    virtual BOOL SendUserInputTone(
      char tone,        ///<  DTMF tone code
      unsigned duration = 0  ///<  Duration of tone in milliseconds
    );

    PINLINE const PString & GetRemoteParty()const{ return m_RemoteParty; };
    
    OpalTransport & GetTransport() const;
    OpalTransport & GetDummyTransport();
   
    PINLINE void SetHashCall( BOOL hash = TRUE ){ m_HashCall = hash; };
    PINLINE BOOL WillHashCall()const{ return m_HashCall; };
    protected:
      SIPSession::GCRef m_OSSConnection;
      BOOL m_LocalHold;
      BOOL m_RemoteHold;
      SDPSessionDescription m_RemoteSDP;
      SDPSessionDescription m_LocalSDP;
      OpalMediaFormatList m_RemoteFormatList;
      PMutex m_StreamsMutex;
      OpalTransport * m_OpalTransport;
      SIPURI m_RemotePartyURI;

    private:
      PString m_RemoteParty;
      BOOL m_HashCall;
      
  };

  //////////////////////////////////////////

  class OpalOSSRTPCallBack : public RTP_UserData
{
  PCLASSINFO(OpalOSSRTPCallBack, RTP_UserData);

  /**@name Construction */
  //@{
    /**Create a new channel.
     */
    OpalOSSRTPCallBack(
      const OpalOSSConnection & connection  ///<  Owner of the RTP session
    );
  //@}

  /**@name Overrides from RTP_UserData */
  //@{
    /**Callback from the RTP session for transmit statistics monitoring.
       This is called every RTP_Session::senderReportInterval packets on the
       transmitter indicating that the statistics have been updated.

       The default behaviour calls H323Connection::OnRTPStatistics().
      */
    virtual void OnTxStatistics(
      const RTP_Session & session   ///<  Session with statistics
    ) const;

    /**Callback from the RTP session for receive statistics monitoring.
       This is called every RTP_Session::receiverReportInterval packets on the
       receiver indicating that the statistics have been updated.

       The default behaviour calls H323Connection::OnRTPStatistics().
      */
    virtual void OnRxStatistics(
      const RTP_Session & session   ///<  Session with statistics
    ) const;
  //@}

  protected:
    const OpalOSSConnection & m_OSSConnection; /// Owner of the RTP session
    
};
};

#endif // HAS_OPAL
#endif // OPALOSSCONNECTION_H

