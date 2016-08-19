/*
 *
 * ProfileTransport.h
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
 * $Log: ProfileTransport.h,v $
 * Revision 1.6  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.4  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef PROFILETRANSPORT_H
#define PROFILETRANSPORT_H

#include "Profile.h"
#include <ptlib/sockets.h>

namespace Profiles
{
  class ProfileTransport : public Profile
  {
    PCLASSINFO( ProfileTransport, Profile );
  public:

    ProfileTransport();

    ProfileTransport(
      const ProfileTransport & profile
    );
    
    virtual ~ProfileTransport();

    ProfileTransport & operator=( 
      const ProfileTransport & profile
    );

    BOOL EnableUDP(
      const PIPSocket::Address & address = 0,
      WORD port = 5060
    );

    void DisableUDP();

    BOOL EnableTCP(
      const PIPSocket::Address & address = 0,
      WORD port = 5060
    );

    void DisableTCP();

    BOOL EnableTLS(
      const PIPSocket::Address & address = 0,
      WORD port = 5061
    );

    void DisableTLS();

    PINLINE BOOL IsUDPEnabled()const{ return m_EnableUDP; };
    PINLINE BOOL IsTCPEnabled()const{ return m_EnableTCP; };
    PINLINE BOOL IsTLSEnabled()const{ return m_EnableTLS; };

    PINLINE const PIPSocket::Address & GetUDPListenerAddress()const{ return m_UDPListenerAddress; };
    PINLINE const PIPSocket::Address & GetTCPListenerAddress()const{ return m_TCPListenerAddress; };
    PINLINE const PIPSocket::Address & GetTLSListenerAddress()const{ return m_TLSListenerAddress; };

    PINLINE void SetUDPListenerAddress( PIPSocket::Address & addr ){ m_UDPListenerAddress = addr; };
    PINLINE void SetTCPListenerAddress( PIPSocket::Address & addr ){ m_TCPListenerAddress = addr; };
    PINLINE void SetTLSListenerAddress( PIPSocket::Address & addr ){ m_TLSListenerAddress = addr; };

    WORD GetUDPListenerPort()const{ return m_UDPListenerPort; };
    WORD GetTCPListenerPort()const{ return m_TCPListenerPort; };
    WORD GetTLSListenerPort()const{ return m_TLSListenerPort; };

    PINLINE void AppendDomain( const PString & domain ){ m_Domains.AppendString( domain ); };
    PINLINE void RemoveDomain( PINDEX i ){ m_Domains.RemoveAt( i ); };
    PINLINE PINDEX GetDomainSize()const{ return m_Domains.GetSize(); };
    PString GetDomain( PINDEX i = 0 )const{ return m_Domains[i]; };
    PINLINE const PString & GetSTUNServerAddress()const{ return m_STUNServerAddress; };
    PINLINE void SetSTUNServerAddress( const PString & addr ){ m_STUNServerAddress = addr; };


  protected:
    PIPSocket::Address m_UDPListenerAddress;
    PIPSocket::Address m_TCPListenerAddress;
    PIPSocket::Address m_TLSListenerAddress;

    WORD m_UDPListenerPort;
    WORD m_TCPListenerPort;
    WORD m_TLSListenerPort;

    BOOL m_EnableUDP;
    BOOL m_EnableTCP;
    BOOL m_EnableTLS;

    PString m_STUNServerAddress;

    PStringArray m_Domains;
  };
};

#endif


