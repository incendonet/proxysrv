/*
 *
 * ProfileUA.cxx
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
 * $Log: ProfileUA.cxx,v $
 * Revision 1.8  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.7  2006/10/30 02:42:27  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.6  2006/08/11 08:38:42  joegenbaclor
 * Initial upload of Presence package
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

#include "ProfileUA.h"
#include "StackVersion.h"

#define new PNEW

using namespace Profiles;

ProfileUA::ProfileUA()
{
  PStringStream uaName;
  uaName << STACK_NAME << " " 
         << STACK_MAJOR_VERSION << "."
         << STACK_MINOR_VERSION << "."
         << STACK_BUILD_NUMBER;

  m_UserAgentName = uaName;

  m_MaxForwards = 70;
}

ProfileUA::ProfileUA(
  const ProfileUA & profile
)
{
  operator=( profile );
}

ProfileUA::~ProfileUA()
{
}

ProfileUA & ProfileUA::operator=(
  const ProfileUA & profile
)
{
  m_TransportProfile = profile.m_TransportProfile;
  m_RegistrationProfile = profile.m_RegistrationProfile;
  m_ProxyProfile = profile.m_ProxyProfile;
  m_MediaProfile = profile.m_MediaProfile;

  m_UserAgentName = profile.m_UserAgentName;
  m_DisplayName = profile.m_DisplayName;
  m_RemoteURI = profile.m_RemoteURI;
  m_RemoteProtocol = profile.m_RemoteProtocol;

  m_AllowedMethods = profile.m_AllowedMethods;
  m_AllowedMethods.MakeUnique();

  m_MaxForwards = profile.m_MaxForwards;

  Profile::operator =( profile );

  return *this;
}




