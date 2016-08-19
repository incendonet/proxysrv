/*
 *
 * ProfileTransport.cxx
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
 * $Log: ProfileTransport.cxx,v $
 * Revision 1.6  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/04/05 00:26:04  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.4  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */


#include "ProfileTransport.h"

#define new PNEW

using namespace Profiles;

ProfileTransport::ProfileTransport()
{
  m_UDPListenerPort = 5060;
  m_TLSListenerPort = 0;
  m_TCPListenerPort = 0;

  EnableUDP();
  DisableTCP();
  DisableTLS();
}
    
ProfileTransport::~ProfileTransport()
{
}

ProfileTransport::ProfileTransport(
  const ProfileTransport & profile
)
{
  m_UDPListenerPort = 0;
  m_TLSListenerPort = 0;
  m_TCPListenerPort = 0;

  operator=( profile );
}

ProfileTransport & ProfileTransport::operator=( 
  const ProfileTransport & profile
)
{
  Profile::operator =( profile );

  m_UDPListenerAddress = profile.m_UDPListenerAddress;
  m_TCPListenerAddress = profile.m_TCPListenerAddress;
  m_TLSListenerAddress = profile.m_TLSListenerAddress;

  m_UDPListenerPort = profile.m_UDPListenerPort;
  m_TCPListenerPort = profile.m_TCPListenerPort;
  m_TLSListenerPort = profile.m_TLSListenerPort;

  m_EnableUDP = profile.m_EnableUDP;
  m_EnableTCP = profile.m_EnableTCP;
  m_EnableTLS = profile.m_EnableTLS;

  m_Domains = profile.m_Domains;
  m_Domains.MakeUnique();

  m_STUNServerAddress = profile.m_STUNServerAddress;

  return *this;
}

BOOL ProfileTransport::EnableUDP(
  const PIPSocket::Address & address,
  WORD port
)
{
  m_UDPListenerAddress = address;
  m_UDPListenerPort = port;
  m_EnableUDP = TRUE;

  return TRUE;
}

void ProfileTransport::DisableUDP()
{
  m_EnableUDP = FALSE;
}

BOOL ProfileTransport::EnableTCP(
  const PIPSocket::Address & address,
  WORD port
)
{
  m_TCPListenerAddress = address;
  m_TCPListenerPort = port;
  m_EnableTCP = TRUE;

  return TRUE;
}

void ProfileTransport::DisableTCP()
{
  m_EnableTCP = FALSE;
}

BOOL ProfileTransport::EnableTLS(
  const PIPSocket::Address & address,
  WORD port
)
{
  m_TLSListenerAddress = address;
  m_TLSListenerPort = port;
  m_EnableTLS = TRUE;

  return TRUE;
}

void ProfileTransport::DisableTLS()
{
  m_EnableTLS = FALSE;
}


