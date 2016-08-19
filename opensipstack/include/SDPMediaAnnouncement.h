/*
 *
 * SDPMediaAnnouncement.h
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
 * $Log: SDPMediaAnnouncement.h,v $
 * Revision 1.6  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
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

#ifndef SDPMEDIAANNOUNCEMENT_H
#define SDPMEDIAANNOUNCEMENT_H

#include "SDPHeader.h"
#include "SDPAttribute.h"
#include "SDPMediaFormat.h"

namespace SDP
{
  class SDPMediaAnnouncement : public SDPHeader
  {
    PCLASSINFO( SDPMediaAnnouncement, SDPHeader );
  public:

    SDPMediaAnnouncement();

    SDPMediaAnnouncement( 
      const SDPHeader & header
    );

    SDPMediaAnnouncement(
      const PString & header
    );

    SDPMediaAnnouncement(
      const PString & name,
      const PString & value
    );

    SDPMediaAnnouncement(
      const SDPMediaAnnouncement & header
    );

    SDPMediaAnnouncement(
      const PString & media,
      WORD port,
      const PString & transport,
      const PBYTEArray & formatList
    );

    SDPMediaAnnouncement & operator=( const PString & header );

    SDPMediaAnnouncement & operator=( const SDPMediaAnnouncement & header );

    void SetMedia(
      const PString & media
    );

    void SetPort(
      WORD port
    );

    void SetTransport(
      const PString & transport
    );

    void AppendFormat(
      BYTE format
    );

    void AppendFormat(
      const SDPMediaFmt & format
    );

    PObject * Clone()const;
    
    PINLINE const PString & GetMedia()const{ return m_Media; };

    PINLINE WORD GetPort(){ return m_Port; };

    PINLINE const PString & GetTransport()const{ return m_Transport; };

    PINLINE const PBYTEArray & GetFormatList()const{ return m_FormatList; };

    PINLINE BYTE GetFormatAt( PINDEX i )const{ return m_FormatList[i]; };

    PINLINE PINDEX GetFormatListSize()const{ return m_FormatList.GetSize(); };
  
  protected:
    PString m_Media;
    WORD m_Port;
    PString m_Transport;
    PBYTEArray m_FormatList;

  };
};

#endif



