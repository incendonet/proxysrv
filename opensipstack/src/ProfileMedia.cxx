/*
 *
 * ProfileMedia.cxx
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
 * $Log: ProfileMedia.cxx,v $
 * Revision 1.8  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.7  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.6  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.5  2006/03/15 16:40:46  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.4  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "ProfileMedia.h"

#define new PNEW

using namespace Profiles;

ProfileMedia::ProfileMedia()
{
  m_SDPSessionId = 0;
  m_SDPVersionId = 0;
  m_MediaAddress = 0;
  m_AudioPort = 0;
  m_VideoPort = 0;
}

ProfileMedia::~ProfileMedia()
{
}

ProfileMedia::ProfileMedia( 
  const ProfileMedia & profile
)
{
  operator=( profile );
}

ProfileMedia & ProfileMedia::operator=(
  const ProfileMedia & profile
)
{
  Profile::operator=( profile );
  m_SDPSessionId = profile.m_SDPSessionId;
  m_SDPVersionId = profile.m_SDPVersionId;
  m_MediaAddress = profile.m_MediaAddress;
  m_AudioPort = profile.m_AudioPort;
  m_VideoPort = profile.m_VideoPort;
  m_MediaFormats = profile.m_MediaFormats;
  m_MediaFormats.MakeUnique();

  return *this;
}


void ProfileMedia::AddMediaFormat( 
  const SDPMediaFmt & mediaFormat 
)
{
  m_MediaFormats.Append( new SDPMediaFmt( mediaFormat ) );
}

void ProfileMedia::RemoveMediaFormat( 
  const SDPMediaFmt & mediaFormat 
)
{
  for( PINDEX i = 0; i < m_MediaFormats.GetSize(); i++ )
  {
    if( m_MediaFormats[i] == mediaFormat )
    {
      m_MediaFormats.RemoveAt( i );
      break;
    }
  }
}

BOOL ProfileMedia::ContructOffer( PString & offer )const
{
  
  if( m_SDPSessionId == 0 ||
    m_SDPVersionId == 0 ||
    m_MediaAddress == 0 ||
    m_AudioPort == 0 
  )
  {
    return FALSE;
  }

  PString mediaAddress = m_MediaAddress.AsString();
  SIPTransport::Version ipVersion = SIPTransport::IPV4;

  if( m_MediaAddress.GetVersion() == 6 )
    ipVersion = SIPTransport::IPV6;

   SDPOffer sdpOffer(
    m_SDPSessionId,
    m_SDPVersionId,
    mediaAddress,
    m_AudioPort,
    m_VideoPort,
    ipVersion
  );

  for( PINDEX i = 0; i < m_MediaFormats.GetSize(); i++ )
  {
    sdpOffer.AddFormat( m_MediaFormats[i] );
  }
  
  offer = sdpOffer.AsString();

  return TRUE;;
}

