/*
 *
 * SDPSessionDescription.h
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
 * $Log: SDPSessionDescription.h,v $
 * Revision 1.6  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/03/15 16:40:37  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.4  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SDPSESSIONDESCRIPTION_H
#define SDPSESSIONDESCRIPTION_H

#include "SDPAttribute.h"
#include "SDPProtocolVersion.h"
#include "SDPConnectionData.h"
#include "SDPURI.h"
#include "SDPBandwidth.h"
#include "SDPTime.h"
#include "SDPOrigin.h"
#include "SDPSessionName.h"

namespace SDP
{
  class SDPSessionDesc : public PObject
  {
    PCLASSINFO( SDPSessionDesc, PObject );
  public:

    SDPSessionDesc();

    SDPSessionDesc(
      const SDPSessionDesc & sdp
    );

    SDPSessionDesc(
      const PString & sdp
    );

    SDPSessionDesc(
      const PStringArray &  sdp
    );

    virtual ~SDPSessionDesc();

    SDPSessionDesc & operator=(
      const SDPSessionDesc & sdp
    );

    SDPSessionDesc & operator=(
      const PString & sdp
    );

    SDPSessionDesc & operator=(
      const PStringArray &  sdp
    );

    //v=  (protocol version)
    void SetProtocolVersion(
      const PString & version
    );

    //o=  (owner/creator and session identifier).
    void SetOrigin(
      const SDPOrigin & origin
    );

    //s=  (session name)
    void SetSessionName(
      const SDPSessionName & sessionName
    );

    void SetSessionName(
      const PString & sessionName
    );

    //i=* (session information)
    void SetSessionInformation(
      const PString & sessionInformation
    );

    //u=* (URI of description)
    void SetURIDescription(
      const PString & uriDesc
    );

    //e=* (email address)
    void SetEmailAddress(
      const PString & email
    );

    //p=* (phone number)
    void SetPhoneNumber(
      const PString & phone
    );

    //b=* (bandwidth information)
    void SetBandwidth(
      const SDPBandwidth & bandwidth
    );

    /// ****not supported****
    //One or more time descriptions (see below)
    //z=* (time zone adjustments)
    ///void SetTimeZoneAdjustments(
    ///  const PString & timeZoneAdjustments
    ///);

    //Zero or more media descriptions (see below)
    //t=  (time the session is active)
    void SetTime(
      const SDPTime & time
    );

    //r=* (zero or more repeat times)
    void SetRepeatTimes(
      const PString & repeatTimes
    );

    void SetConnectionData(
      const SDPConnectionData & connectionData
    );

    void SetEncryptionKey(
      const PString & key
    );

    void AppendAttribute(
      const SDPAttribute & attribute
    );

    BOOL GetAttribute(
      SDPAttribute & attribute,
      PINDEX attributeIndex
    );

    BOOL RemoveAttribute(
      PINDEX attributeIndex
    );

    PINDEX GetAttributeSize();

    PObject * Clone();

    virtual void PrintOn(
      ostream & strm
    )const;

    //v=  (protocol version)
    PINLINE const SDPProtocolVersion * GetVersion()const{ return m_Version; };
    //o=  (owner/creator and session identifier).
    PINLINE const SDPOrigin * GetOrigin()const{ return m_Origin; };
    //s=  (session name)
    PINLINE const SDPSessionName * GetSessionName()const{ return m_SessionName; };
    //i=* (session information)
    PINLINE const SDPHeader * GetSessionInformation()const{ return m_SessionInformation; };
    //u=* (URI of description)
    PINLINE const SDPURI * GetURIDescription()const{ return m_URIDescription; };
    //e=* (email address)
    PINLINE const SDPHeader * GetEmail()const{ return m_EmailAddress; };
    //p=* (phone number)
    PINLINE const SDPHeader * GetPhoneNumber()const{ return m_PhoneNumber; };
    //c=* (connection information - not required if included in all media)
    PINLINE const SDPConnectionData * GetConnectionData()const{ return m_ConnectionData; };
    //b=* (bandwidth information)
    PINLINE const SDPBandwidth * GetBandwidth()const{ return m_Bandwidth; };
    //One or more time descriptions (see below)
    //z=* (time zone adjustments)
    ///SDPHeader * m_TimeZoneAdjustments;
    //k=* (encryption key)
    PINLINE const SDPHeader * GetEncryptionKey()const{ return m_EncryptionKey; };
    //t=  (time the session is active)
    PINLINE const SDPTime * GetTime()const{ return m_Time; };
    //r=* (zero or more repeat times)
    PINLINE const SDPHeader * GetRepeatTimes()const{ return m_RepeatTimes; };

  protected:

    void Cleanup();

    //v=  (protocol version)
    SDPProtocolVersion * m_Version;
    //o=  (owner/creator and session identifier).
    SDPOrigin * m_Origin;
    //s=  (session name)
    SDPSessionName * m_SessionName;
    //i=* (session information)
    SDPHeader * m_SessionInformation;
    //u=* (URI of description)
    SDPURI * m_URIDescription;
    //e=* (email address)
    SDPHeader * m_EmailAddress;
    //p=* (phone number)
    SDPHeader * m_PhoneNumber;
    //c=* (connection information - not required if included in all media)
    SDPConnectionData * m_ConnectionData;
    //b=* (bandwidth information)
    SDPBandwidth * m_Bandwidth;
    //One or more time descriptions (see below)
    //z=* (time zone adjustments)
    ///SDPHeader * m_TimeZoneAdjustments;
    //k=* (encryption key)
    SDPHeader * m_EncryptionKey;
    //a=* (zero or more session attribute lines)
    SDPAttribute::Collection m_Attributes;
    PMutex m_AttributesMutex;
    //Zero or more media descriptions (see below)
    //t=  (time the session is active)
    SDPTime * m_Time;
    //r=* (zero or more repeat times)
    SDPHeader * m_RepeatTimes;
  };
};
#endif


