/*
 *
 * SDPMediaDescription.h
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
 * $Log: SDPMediaDescription.h,v $
 * Revision 1.7  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
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

#ifndef SDPMEDIADESCRIPTION_H
#define SDPMEDIADESCRIPTION_H

#include "SDPHeader.h"
#include "SDPOrigin.h"
#include "SDPProtocolVersion.h"
#include "SDPURI.h"
#include "SDPConnectionData.h"
#include "SDPTime.h"
#include "SDPMediaAnnouncement.h"
#include "SDPBandwidth.h"
#include "SDPAttribute.h"
#include "SDPMediaFormat.h"

namespace SDP
{
  class SDPMediaDesc : public PObject
  {
    PCLASSINFO( SDPMediaDesc, PObject );
  public:

    SDPMediaDesc();

    virtual ~SDPMediaDesc();

    SDPMediaDesc(
      const SDPMediaAnnouncement & mediaAnnouncement
    );

    SDPMediaDesc(
      const PString & mediaType,
      WORD port,
      const PString & transport = "RTP/AVP"
    );


    SDPMediaDesc(
      const SDPMediaDesc & mediaDescription
    );

    SDPMediaDesc(
      PString & tokens
    );

    SDPMediaDesc(
      PStringArray & tokens
    );

    SDPMediaDesc & operator=(
      const PString & tokens
    );

    SDPMediaDesc & operator=(
      const PStringArray & tokens
    );

    SDPMediaDesc & operator=(
      const SDPMediaDesc & mediaDescription
    );

    PObject * Clone()const;

    virtual void PrintOn(
      ostream & strm
    )const;


    void SetMediaAnnouncement(
      const SDPMediaAnnouncement & mediaAnnouncement
    );

    void SetMediaTitle(
      const PString & mediaTitle
    );

    void SetConnectionData(
      const SDPConnectionData & connectionData
    );

    void SetBandwidth(
      const SDPBandwidth & bandwidth
    );

    void SetEncryptionKey(
      const PString & key
    );

    void AppendHeader(
      const PString & header
    );

    void AppendHeader(
      const SDPHeader & header
    );

    void AppendAttribute(
      const SDPAttribute & attribute
    );

    void AppendAttribute(
      const SDPMediaFmt & format
    );

    BOOL GetAttribute(
      SDPAttribute & attribute,
      PINDEX attributeIndex
    );

    BOOL RemoveAttribute(
      PINDEX attributeIndex
    );

    PINDEX GetAttributeSize();


    PINLINE SDPMediaAnnouncement * GetMediaAnnouncement()const{ return m_MediaAnnouncement; };

    PINLINE const SDPHeader * GetMediaTitle()const{ return m_MediaTitle; };

    PINLINE const SDPConnectionData * GetConnectionData()const{ return m_ConnectionData; };

    PINLINE const SDPBandwidth * GetBandwidth()const{ return m_Bandwidth; };

    PINLINE const SDPHeader * GetEncryptionKey()const{ return m_EncryptionKey; };

    PINLINE PINDEX GetAttributeSize()const{ return m_Attributes.GetSize(); };

    const SDPAttribute & GetAttributeAt( PINDEX i )const{ return m_Attributes[i]; };

    PLIST( Collection, SDPMediaDesc );

  protected:

    void Cleanup();
    /*
    Media description
        m=  (media name and transport address)
        i=* (media title)
        c=* (connection information - optional if included at session-level)
        b=* (bandwidth information)
        k=* (encryption key)
        a=* (zero or more media attribute lines)
    */

    SDPMediaAnnouncement * m_MediaAnnouncement;
    SDPHeader * m_MediaTitle;
    SDPConnectionData * m_ConnectionData;
    SDPBandwidth * m_Bandwidth;
    SDPHeader * m_EncryptionKey;
    SDPAttribute::Collection m_Attributes;
    PMutex m_AttributesMutex;
  };
};


#endif


