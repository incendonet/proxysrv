/*
 *
 * ProfileMedia.h
 * 
 * Open SIP Stack ( OSS )
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
 * $Log: ProfileMedia.h,v $
 * Revision 1.8  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.7  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 * Revision 1.6  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.5  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef PROFILEMEDIA_H
#define PROFILEMEDIA_H

#include "Profile.h"
#include "SIPTransport.h"
#include "SDPOffer.h"

namespace Profiles
{
  using namespace SIPTransports;
  using namespace SDP;

  class ProfileMedia : public Profile
  {
    PCLASSINFO( ProfileMedia, Profile );
  public:

    ProfileMedia();

    ProfileMedia( 
      const ProfileMedia & profile
    );

    ProfileMedia & operator=(
      const ProfileMedia & profile
    );

    virtual ~ProfileMedia();

    void AddMediaFormat( 
      const SDPMediaFmt & mediaFormat 
    );

    void RemoveMediaFormat( 
      const SDPMediaFmt & mediaFormat 
    );

    BOOL ContructOffer( PString & offer )const;

    PINLINE DWORD GetSDPSessionId()const{ return m_SDPSessionId; };
    PINLINE DWORD GetSDPVersionId()const{ return m_SDPVersionId; };
    PINLINE const PIPSocket::Address & GetSDPMediaAddress()const{ return m_MediaAddress; };
    PINLINE WORD GetSDPAudioPort()const{ return m_AudioPort; };
    PINLINE WORD GetSDPVideoPort()const{ return m_VideoPort; };
    PINLINE const SDPMediaFmt::Collection & GetMediaFormats()const{ return m_MediaFormats;}
    PINLINE void ClearFormats(){ m_MediaFormats.RemoveAll(); };

    PINLINE void SetSDPSessionId( DWORD sessionId ){ m_SDPSessionId = sessionId; };
    PINLINE void SetSDPVersionId( DWORD versionId ){ m_SDPVersionId = versionId; };
    PINLINE void SetSDPMediaAddress( const PIPSocket::Address & mediaAddress ){ m_MediaAddress = mediaAddress; };
    PINLINE void SetSDPAudioPort( WORD port ){ m_AudioPort = port; };
    PINLINE void SetSDPVideoPort( WORD port ){ m_VideoPort = port; };

  protected:

    DWORD m_SDPSessionId;
    DWORD m_SDPVersionId;
    PIPSocket::Address m_MediaAddress;
    WORD m_AudioPort;
    WORD m_VideoPort;
    SDPMediaFmt::Collection m_MediaFormats;
  };
};

#endif

