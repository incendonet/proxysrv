/*
 *
 * SIPNATMethods.h
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
 * $Log: SIPNATMethods.h,v $
 * Revision 1.3  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.2  2006/04/07 13:50:17  joegenbaclor
 * updated autoconf files in linux
 *
 * Revision 1.1  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 *
 */

#ifndef _SIPSTUN_H
#define _SIPSTUN_H


#include <ptlib.h>
#include <ptclib/pnat.h>
#include <ptlib/sockets.h>

/// The STUN Client in this code set is directly based from PSTUNClient found in PWLIB.
/// Due to its use of internal classes it is impossible to just derive from it and modify its behavior
/// See: http://www.openh323.org/pipermail/openh323/Week-of-Mon-20060327/subject.html#76790

namespace NAT
{

  /**UDP socket that has been created by the STUN client.
    */
  class SIPSTUNUDPSocket : public PUDPSocket
  {
    PCLASSINFO(SIPSTUNUDPSocket, PUDPSocket);
    public:
      SIPSTUNUDPSocket();

      virtual BOOL GetLocalAddress(
        Address & addr    ///< Variable to receive hosts IP address
      );
      virtual BOOL GetLocalAddress(
        Address & addr,    ///< Variable to receive peer hosts IP address
        WORD & port        ///< Variable to receive peer hosts port number
      );

    protected:
      PIPSocket::Address externalIP;

    friend class SIPSTUNClient;
  };


  /**STUN client.
    */
  class SIPSTUNClient : public PNatMethod
  {
    PCLASSINFO(SIPSTUNClient, PNatMethod);
    public:
      enum {
        DefaultPort = 3478
      };

      SIPSTUNClient(
        const PString & server,
        WORD portBase = 0,
        WORD portMax = 0,
        WORD portPairBase = 0,
        WORD portPairMax = 0
      );
      SIPSTUNClient(
        const PIPSocket::Address & serverAddress,
        WORD serverPort = DefaultPort,
        WORD portBase = 0,
        WORD portMax = 0,
        WORD portPairBase = 0,
        WORD portPairMax = 0
      );


      /**Get the current STUN server address and port being used.
        */
      PString GetServer() const;

      /**Set the STUN server to use.
        The server string may be of the form host:port. If :port is absent
        then the default port 3478 is used. The substring port can also be
        a service name as found in /etc/services. The host substring may be
        a DNS name or explicit IP address.
        */
      BOOL SetServer(
        const PString & server
      );

      /**Set the STUN server to use by IP address and port.
        If serverPort is zero then the default port of 3478 is used.
        */
      BOOL SetServer(
        const PIPSocket::Address & serverAddress,
        WORD serverPort = 0
      );

      enum NatTypes {
        UnknownNat,
        OpenNat,
        ConeNat,
        RestrictedNat,
        PortRestrictedNat,
        SymmetricNat,
        SymmetricFirewall,
        BlockedNat,
        PartialBlockedNat,
        NumNatTypes
      };

      /**Determine via the STUN protocol the NAT type for the router.
        This will cache the last determine NAT type. Use the force variable to
        guarantee an up to date value.
        */
      NatTypes GetNatType(
        BOOL force = FALSE    ///< Force a new check
      );

      /**Determine via the STUN protocol the NAT type for the router.
        As for GetNatType() but returns an English string for the type.
        */
      PString GetNatTypeName(
        BOOL force = FALSE    ///< Force a new check
      ) { return GetNatTypeString(GetNatType(force)); }

      /**Get NatTypes enumeration as an English string for the type.
        */
      static PString GetNatTypeString(
        NatTypes type   ///< NAT Type to get name of
      );

      enum RTPSupportTypes {
        RTPOK,
        RTPUnknown,
        RTPUnsupported,
        RTPIfSendMedia
      };

      /**Return an indication if the current STUN type supports RTP
        Use the force variable to guarantee an up to date test
        */
      RTPSupportTypes IsSupportingRTP(
        BOOL force = FALSE    ///< Force a new check
      );

      /**Determine the external router address.
        This will send UDP packets out using the STUN protocol to determine
        the intervening routers external IP address.

        A cached address is returned provided it is no older than the time
        specified.
        */
      virtual BOOL GetExternalAddress(
        PIPSocket::Address & externalAddress, ///< External address of router
        const PTimeInterval & maxAge = 1000   ///< Maximum age for caching
      );

      /**Create a single socket.
        The STUN protocol is used to create a socket for which the external IP
        address and port numbers are known. A PUDPSocket descendant is returned
        which will, in response to GetLocalAddress() return the externally
        visible IP and port rather than the local machines IP and socket.

        The will create a new socket pointer. It is up to the caller to make
        sure the socket is deleted to avoid memory leaks.

        The socket pointer is set to NULL if the function fails and returns
        FALSE.
        */
      BOOL CreateSocket(
        PUDPSocket * & socket
      );

      BOOL CreateSocket(
        PUDPSocket * & socket,
        const PIPSocket::Address & iface,
        WORD port
      );

      /**Create a socket pair.
        The STUN protocol is used to create a pair of sockets with adjacent
        port numbers for which the external IP address and port numbers are
        known. PUDPSocket descendants are returned which will, in response
        to GetLocalAddress() return the externally visible IP and port rather
        than the local machines IP and socket.

        The will create new socket pointers. It is up to the caller to make
        sure the sockets are deleted to avoid memory leaks.

        The socket pointers are set to NULL if the function fails and returns
        FALSE.
        */
      virtual BOOL CreateSocketPair(
        PUDPSocket * & socket1,
        PUDPSocket * & socket2
      );

      /**Get the timeout for responses from STUN server.
        */
      const PTimeInterval GetTimeout() const { return replyTimeout; }

      /**Set the timeout for responses from STUN server.
        */
      void SetTimeout(
        const PTimeInterval & timeout   ///< New timeout in milliseconds
      ) { replyTimeout = timeout; }

      /**Get the number of retries for responses from STUN server.
        */
      PINDEX GetRetries() const { return pollRetries; }

      /**Set the number of retries for responses from STUN server.
        */
      void SetRetries(
        PINDEX retries    ///< Number of retries
      ) { pollRetries = retries; }

      /**Get the number of sockets to create in attempt to get a port pair.
        RTP requires a pair of consecutive ports. To get this several sockets
        must be opened and fired through the NAT firewall to get a pair. The
        busier the firewall the more sockets will be required.
        */
      PINDEX GetSocketsForPairing() const { return numSocketsForPairing; }

      /**Set the number of sockets to create in attempt to get a port pair.
        RTP requires a pair of consecutive ports. To get this several sockets
        must be opened and fired through the NAT firewall to get a pair. The
        busier the firewall the more sockets will be required.
        */
      void SetSocketsForPairing(
        PINDEX numSockets   ///< Number opf sockets to create
      ) { numSocketsForPairing = numSockets; }

      /**Returns whether the Nat Method is ready and available in
        assisting in NAT Traversal. The principal is this function is
        to allow the EP to detect various methods and if a method
        is detected then this method is available for NAT traversal.
        The availablity of the STUN Method is dependant on the Type
        of NAT being used.
      */
      virtual BOOL IsAvailable();

    protected:
      PIPSocket::Address serverAddress;
      WORD               serverPort;
      PTimeInterval      replyTimeout;
      PINDEX             pollRetries;
      PINDEX             numSocketsForPairing;

      bool OpenSocket(PUDPSocket & socket, PortInfo & portInfo) const;

      BOOL OpenSocket(
        PUDPSocket & socket,
        const PIPSocket::Address & iface,
        WORD port
      );

      NatTypes           natType;
      PIPSocket::Address cachedExternalAddress;
      PTime              timeAddressObtained;
  };


  inline ostream & operator<<(ostream & strm, SIPSTUNClient::NatTypes type) { return strm << SIPSTUNClient::GetNatTypeString(type); }

};

#endif // _SIPSTUN_H


// End of file ////////////////////////////////////////////////////////////////
