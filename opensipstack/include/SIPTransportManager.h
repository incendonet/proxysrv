/*
 *
 * SIPTransportManager.h
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
 * $Log: SIPTransportManager.h,v $
 * Revision 1.12  2007/01/06 07:30:29  joegenbaclor
 * Added mask to filter GetDefaultInterfaceAddress
 *
 * Revision 1.11  2006/12/22 07:37:19  joegenbaclor
 * Introduced separate funtion call  for starting of transport thread.  This is necessary
 *  to make sure that the backdoor listener is already initialized prior to calling transport
 *  reads
 *
 * Revision 1.10  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.9  2006/11/21 06:55:17  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.8  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.7  2006/03/21 09:03:57  joegenbaclor
 * Introduced Terminate() method in SIPTransportManager to allow
 * SIPStack to terminate Transport threads prior to the deletion of the manager
 *
 * Revision 1.6  2006/03/14 03:13:22  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPTRANSPORTMANAGER_H
#define SIPTRANSPORTMANAGER_H
#include <ptlib.h>
#include "SIPNATMethods.h"
#include "SIPTransport.h"
#include "Logger.h"

using namespace Tools;

namespace SIPTransports
{
  class SIPTransportManager : public PObject, public Logger
  {
    PCLASSINFO( SIPTransportManager, PObject );
  public:

    SIPTransportManager( 
      PNotifier eventNotifier 
    );

    virtual ~SIPTransportManager();

    BOOL AddTransport(
      SIPTransport::Protocol protocol,
      const PString & iface = PString::Empty(),
      WORD port = 5060,
      BOOL append = FALSE
    );

    BOOL AddTransport(
      SIPTransport::Protocol protocol,
      const PIPSocket::Address & iface,
      WORD port = 5060,
      BOOL append = FALSE
    );

    BOOL AppendTransport(
      SIPTransport::Protocol protocol,
      const PString & iface = PString::Empty(),
      WORD port = 5060
    );

    BOOL AppendTransport(
      SIPTransport::Protocol protocol,
      const PIPSocket::Address & iface,
      WORD port = 5060
    );

    virtual SIPTransport * OnCreateTransport(
      SIPTransport::Protocol protocol
    );

    BOOL EnqueueEvent(
      SIPTransportEvent * event
    );

    void OnTransportEvent(
      SIPTransportEvent * event
    );

    SIPTransport::Protocol DetermineServerProtocol(
      SIPTransportEvent * event
    );

    BOOL HasListener()const;

    BOOL Terminate();

    NAT::SIPSTUNClient * GetSTUN(
      const PIPSocket::Address & address = 0
    ) const;

    NAT::SIPSTUNClient::NatTypes SetSTUNServer(
      const PString & server,
      WORD udpPortBase = 0,
      WORD udpPortMax = 0,
      WORD rtpPortBase = 0,
      WORD rtpPortMax = 0
    );

    BOOL TranslateIPAddress(
      PIPSocket::Address & localAddress,
      const PIPSocket::Address & remoteAddress
    );

    BOOL GetListenerAddress(
      SIPTransport::Protocol proto,
      const PIPSocket::Address & target,
      PIPSocket::Address & listenerAddress,
      WORD & listenerPort
    );

    BOOL IsLocalAddressAndPort(
      const SIPURI & uri
    )const;

    BOOL IsLocalAddressAndPort(
      const PIPSocket::Address & addr,
      WORD port
    )const;

    static PIPSocket::Address GetDefaultInterfaceAddress( 
      BOOL includeLoopBack = FALSE, 
      PStringArray * mask = NULL /// string array containing ip address of bound interfaces
    );

    static PIPSocket::Address GetDefaultInterfaceAddress(
      const PIPSocket::Address & destination,
      BOOL includeLoopBack = FALSE, 
      PStringArray * mask = NULL /// string array containing ip address of bound interfaces
    );

    BOOL StartThreads();

    PINLINE SIPTransport * GetUDPTransport(){ return m_UDPTransport; };
    PINLINE SIPTransport * GetTCPTransport(){ return m_TCPTransport; };
    PINLINE SIPTransport * GetTLSTransport(){ return m_TLSTransport; };

  protected:
    NAT::SIPSTUNClient * m_STUNClient;
    PIPSocket::Address m_TranslationAddress;

    BOOL m_IsTerminated;
    SIPTransport * m_UDPTransport;
    SIPTransport * m_TCPTransport;
    SIPTransport * m_TLSTransport;

    PNotifier m_EventNotifier;
  };
}



#endif





