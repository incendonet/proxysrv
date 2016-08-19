/*
 *
 * SDPPacket.h
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
 * $Log: SDPPacket.h,v $
 * Revision 1.6  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/07/24 07:33:56  joegenbaclor
 * Minor bug fixes allover
 *
 * Revision 1.4  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SDPPACKET_H
#define SDPPACKET_H

#include "SDPSessionDescription.h"
#include "SDPMediaDescription.h"

namespace SDP
{
  class SDPPacket : public PObject
  {
    PCLASSINFO( SDPPacket, PObject );
  public:

    SDPPacket();

    SDPPacket( 
      const PString & sdp
    );

    SDPPacket(
      const SDPPacket & sdp
    );

    virtual ~SDPPacket();

    SDPPacket & operator=( 
      const PString & sdp 
    );

    SDPPacket & operator=(
      const SDPPacket & sdp
    );

    virtual void PrintOn(
      ostream & strm
    )const;

    virtual PObject * Clone();

    PString AsString()const;

    static BOOL Construct( 
      SDPPacket & packet,
      DWORD sessionId,
      DWORD versionId,
      const PString & mediaAddress,
      const PString & addressType = "IP4",
      const PString & user = "phone",
      const PString & sessionName = PString::Empty(),
      const PString & description = PString::Empty(),
      const PString & email = PString::Empty(),
      const PString & phone = PString::Empty() 
    );

    
    PINLINE SDPSessionDesc & GetSession(){ return m_Session; };
    PINLINE SDPMediaDesc::Collection & GetMediaSessions(){ return m_MediaSessions; };

  protected:

    void ParseMediaHeaders(
      const PStringArray & headers
    );

    SDPSessionDesc m_Session;
    SDPMediaDesc::Collection m_MediaSessions;
  };
};

#endif


