/*
 *
 * SDPSessionDesc.cxx
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
 * $Log: SDPSessionDescription.cxx,v $
 * Revision 1.7  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
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

#include "SDPSessionDescription.h"

using namespace SDP;

#define new PNEW

SDPSessionDesc::SDPSessionDesc()
{
    //v=  (protocol version)
    m_Version = NULL;
    //o=  (owner/creator and session identifier).
    m_Origin = NULL;
    //s=  (session name)
    m_SessionName = NULL;
    //i=* (session information)
    m_SessionInformation = NULL;
    //u=* (URI of description)
    m_URIDescription = NULL;
    //e=* (email address)
    m_EmailAddress = NULL;
    //p=* (phone number)
    m_PhoneNumber = NULL;
    //c=* (connection information - not required if included in all media)
    m_ConnectionData = NULL;
    //b=* (bandwidth information)
    m_Bandwidth = NULL;
    //z=* (time zone adjustments)
    ///m_TimeZoneAdjustments = NULL;
    //k=* (encryption key)
    m_EncryptionKey = NULL;
    //Zero or more media descriptions (see below)
    //t=  (time the session is active)
    m_Time = NULL;
    //r=* (zero or more repeat times)
    m_RepeatTimes = NULL;
}

SDPSessionDesc::SDPSessionDesc(
  const SDPSessionDesc & sdp
)
{
  //v=  (protocol version)
  m_Version = NULL;
  //o=  (owner/creator and session identifier).
  m_Origin = NULL;
  //s=  (session name)
  m_SessionName = NULL;
  //i=* (session information)
  m_SessionInformation = NULL;
  //u=* (URI of description)
  m_URIDescription = NULL;
  //e=* (email address)
  m_EmailAddress = NULL;
  //p=* (phone number)
  m_PhoneNumber = NULL;
  //c=* (connection information - not required if included in all media)
  m_ConnectionData = NULL;
  //b=* (bandwidth information)
  m_Bandwidth = NULL;
  //z=* (time zone adjustments)
  ///m_TimeZoneAdjustments = NULL;
  //k=* (encryption key)
  m_EncryptionKey = NULL;
  //Zero or more media descriptions (see below)
  //t=  (time the session is active)
  m_Time = NULL;
  //r=* (zero or more repeat times)
  m_RepeatTimes = NULL;
  operator=( sdp );
}

SDPSessionDesc::SDPSessionDesc(
  const PString & sdp
)
{
  //v=  (protocol version)
  m_Version = NULL;
  //o=  (owner/creator and session identifier).
  m_Origin = NULL;
  //s=  (session name)
  m_SessionName = NULL;
  //i=* (session information)
  m_SessionInformation = NULL;
  //u=* (URI of description)
  m_URIDescription = NULL;
  //e=* (email address)
  m_EmailAddress = NULL;
  //p=* (phone number)
  m_PhoneNumber = NULL;
  //c=* (connection information - not required if included in all media)
  m_ConnectionData = NULL;
  //b=* (bandwidth information)
  m_Bandwidth = NULL;
  //z=* (time zone adjustments)
  ///m_TimeZoneAdjustments = NULL;
  //k=* (encryption key)
  m_EncryptionKey = NULL;
  //Zero or more media descriptions (see below)
  //t=  (time the session is active)
  m_Time = NULL;
  //r=* (zero or more repeat times)
  m_RepeatTimes = NULL;

  operator=( sdp );
}

SDPSessionDesc::SDPSessionDesc(
  const PStringArray &  sdp
)
{
  //v=  (protocol version)
  m_Version = NULL;
  //o=  (owner/creator and session identifier).
  m_Origin = NULL;
  //s=  (session name)
  m_SessionName = NULL;
  //i=* (session information)
  m_SessionInformation = NULL;
  //u=* (URI of description)
  m_URIDescription = NULL;
  //e=* (email address)
  m_EmailAddress = NULL;
  //p=* (phone number)
  m_PhoneNumber = NULL;
  //c=* (connection information - not required if included in all media)
  m_ConnectionData = NULL;
  //b=* (bandwidth information)
  m_Bandwidth = NULL;
  //z=* (time zone adjustments)
  ///m_TimeZoneAdjustments = NULL;
  //k=* (encryption key)
  m_EncryptionKey = NULL;
  //Zero or more media descriptions (see below)
  //t=  (time the session is active)
  m_Time = NULL;
  //r=* (zero or more repeat times)
  m_RepeatTimes = NULL;

  operator=( sdp );
}

SDPSessionDesc::~SDPSessionDesc()
{
  Cleanup();
}

void SDPSessionDesc::Cleanup()
{
  //v=  (protocol version)
  if( m_Version != NULL )
  {
    delete m_Version;
    m_Version = NULL;
  }

  //o=  (owner/creator and session identifier).
  if( m_Origin != NULL )
  {
    delete m_Origin;
    m_Origin = NULL;
  }

  //s=  (session name)
  if( m_SessionName != NULL )
  {
    delete m_SessionName;
    m_SessionName = NULL;
  }


  //i=* (session information)
  if( m_SessionInformation != NULL )
  {
    delete m_SessionInformation;
    m_SessionInformation = NULL;
  }

  //u=* (URI of description)
  if( m_URIDescription != NULL )
  {
    delete m_URIDescription;
    m_URIDescription = NULL;
  }

  //e=* (email address)
  if( m_EmailAddress != NULL )
  {
    delete m_EmailAddress;
    m_EmailAddress = NULL;
  }

  //p=* (phone number)
  if( m_PhoneNumber != NULL )
  {
    delete m_PhoneNumber;
    m_PhoneNumber = NULL;
  }

  //c=* (connection information - not required if included in all media)
  if( m_ConnectionData != NULL )
  {
    delete m_ConnectionData;
    m_ConnectionData = NULL;
  }

  //b=* (bandwidth information)
  if( m_Bandwidth != NULL )
  {
    delete m_Bandwidth;
    m_Bandwidth = NULL;
  }

  //z=* (time zone adjustments)
  ///if( m_TimeZoneAdjustments != NULL )
  //{
  //  delete m_TimeZoneAdjustments;
  //  m_TimeZoneAdjustments = NULL;
  //}

  //k=* (encryption key)
  if( m_EncryptionKey != NULL )
  {
    delete m_EncryptionKey;
    m_EncryptionKey = NULL;
  }

  //t=  (time the session is active)
  if( m_Time != NULL )
  {
    delete m_Time;
    m_Time = NULL;
  }

  //r=* (zero or more repeat times)
  if( m_RepeatTimes != NULL )
  {
    delete m_RepeatTimes;
    m_RepeatTimes = NULL;
  }

  m_Attributes.MakeUnique();
  m_Attributes.RemoveAll();
}

PObject * SDPSessionDesc::Clone()
{
  SDPSessionDesc * clone = new SDPSessionDesc();

  if( m_Version != NULL )
    clone->m_Version = (SDPProtocolVersion*)m_Version->Clone();

  //o=  (owner/creator and session identifier).
  if( m_Origin != NULL )
    clone->m_Origin = (SDPOrigin*)m_Origin->Clone();

  //s=  (session name)
  if( m_SessionName != NULL )
    clone->m_SessionName = (SDPSessionName*)m_SessionName->Clone();

  //i=* (session information)
  if( m_SessionInformation != NULL )
    clone->m_SessionInformation = (SDPHeader*)m_SessionInformation->Clone();

  //u=* (URI of description)
  if( m_URIDescription != NULL )
    clone->m_URIDescription = (SDPURI*)m_URIDescription->Clone();

  //e=* (email address)
  if( m_EmailAddress != NULL )
    clone->m_EmailAddress = (SDPHeader*)m_EmailAddress->Clone();

  //p=* (phone number)
  if( m_PhoneNumber != NULL )
    clone->m_PhoneNumber = (SDPHeader*)m_PhoneNumber->Clone();

  //c=* (connection information - not required if included in all media)
  if( m_ConnectionData != NULL )
    clone->m_ConnectionData = (SDPConnectionData*)m_ConnectionData->Clone();

  //b=* (bandwidth information)
  if( m_Bandwidth != NULL )
    clone->m_Bandwidth = (SDPBandwidth*)m_Bandwidth->Clone();

  //z=* (time zone adjustments)
  //if( m_TimeZoneAdjustments != NULL )
  //  clone->m_TimeZoneAdjustments = m_TimeZoneAdjustments->Clone();

  //k=* (encryption key)
  if( m_EncryptionKey != NULL )
    clone->m_EncryptionKey = (SDPHeader*)m_EncryptionKey->Clone();

  //t=  (time the session is active)
  if( m_Time != NULL )
    clone->m_Time = (SDPTime*)m_Time->Clone();

  //r=* (zero or more repeat times)
  if( m_RepeatTimes != NULL )
    clone->m_RepeatTimes = (SDPHeader*)m_RepeatTimes->Clone();

  clone->m_Attributes = m_Attributes;
  clone->m_Attributes.MakeUnique();

  return clone;
}

SDPSessionDesc & SDPSessionDesc::operator=(
  const SDPSessionDesc & sdp
)
{
  Cleanup();

  if( sdp.m_Version != NULL )
    m_Version = (SDPProtocolVersion*)sdp.m_Version->Clone();

  //o=  (owner/creator and session identifier).
  if( sdp.m_Origin != NULL )
    m_Origin = (SDPOrigin*)sdp.m_Origin->Clone();

  //s=  (session name)
  if( sdp.m_SessionName != NULL )
    m_SessionName = (SDPSessionName*)sdp.m_SessionName->Clone();

  //i=* (session information)
  if( sdp.m_SessionInformation != NULL )
    m_SessionInformation = (SDPHeader*)sdp.m_SessionInformation->Clone();

  //u=* (URI of description)
  if( sdp.m_URIDescription != NULL )
    m_URIDescription = (SDPURI*)sdp.m_URIDescription->Clone();

  //e=* (email address)
  if( sdp.m_EmailAddress != NULL )
    m_EmailAddress = (SDPHeader*)sdp.m_EmailAddress->Clone();

  //p=* (phone number)
  if( sdp.m_PhoneNumber != NULL )
    m_PhoneNumber = (SDPHeader*)sdp.m_PhoneNumber->Clone();

  //c=* (connection information - not required if included in all media)
  if( sdp.m_ConnectionData != NULL )
    m_ConnectionData = (SDPConnectionData*)sdp.m_ConnectionData->Clone();

  //b=* (bandwidth information)
  if( sdp.m_Bandwidth != NULL )
    m_Bandwidth = (SDPBandwidth*)sdp.m_Bandwidth->Clone();

  //z=* (time zone adjustments)
  //if( sdp.m_TimeZoneAdjustments != NULL )
  //  m_TimeZoneAdjustments = sdp.m_TimeZoneAdjustments->Clone();

  //k=* (encryption key)
  if( sdp.m_EncryptionKey != NULL )
    m_EncryptionKey = (SDPHeader*)sdp.m_EncryptionKey->Clone();

  //t=  (time the session is active)
  if( sdp.m_Time != NULL )
    m_Time = (SDPTime*)sdp.m_Time->Clone();

  //r=* (zero or more repeat times)
  if( sdp.m_RepeatTimes != NULL )
    m_RepeatTimes = (SDPHeader*)sdp.m_RepeatTimes->Clone();

  m_Attributes = sdp.m_Attributes;
  m_Attributes.MakeUnique();


  return *this;
}

SDPSessionDesc & SDPSessionDesc::operator=(
  const PString & sdp
)
{
  PStringArray tokens;
  SDPHeader::Tokenize( sdp, tokens, FALSE );
  return operator=( tokens );
}

SDPSessionDesc & SDPSessionDesc::operator=(
  const PStringArray &  tokens
)
{
  Cleanup();

  for( PINDEX i = 0; i < tokens.GetSize(); i++ )
  {
    PString token = tokens[i];
    SDPHeader header = token;
    PString attrName = header.GetSDPName().ToLower();

    if( attrName == "v" && m_Version == NULL )
      m_Version = new SDPProtocolVersion( header );
    else if( attrName == "o" && m_Origin == NULL)
      m_Origin = new SDPOrigin( header );
    else if( attrName == "s" && m_SessionName == NULL)
      m_SessionName = new SDPSessionName( header );
    else if( attrName == "i" && m_SessionInformation == NULL )
      m_SessionInformation = new SDPHeader( header );
    else if( attrName == "u" && m_URIDescription == NULL )
      m_URIDescription = new SDPURI( header );
    else if( attrName == "e" && m_EmailAddress == NULL )
      m_EmailAddress = new SDPHeader( header );
    else if( attrName == "p" && m_PhoneNumber == NULL )
      m_PhoneNumber = new SDPHeader( header );
    else if( attrName == "c" && m_ConnectionData == NULL )
      m_ConnectionData = new SDPConnectionData( header );
    else if( attrName == "b" && m_Bandwidth == NULL )
      m_Bandwidth = new SDPBandwidth( header );
    else if( attrName == "z" )
      PTRACE( 4, "Unsupported SDP Header - " << token );
    else if( attrName == "k" && m_EncryptionKey == NULL )
      m_EncryptionKey = new SDPHeader( header );
    else if( attrName == "a" )
      m_Attributes.Append( new SDPAttribute( header ) );
    else if( attrName == "t" && m_Time == NULL )
      m_Time = new SDPTime( header );
    else if( attrName == "r" && m_RepeatTimes == NULL )
      m_RepeatTimes = new SDPHeader( header );
    else
      PTRACE( 4, "Unsupported SDP Header - " << token );
  }

  return *this;
}

//v=  (protocol version)
void SDPSessionDesc::SetProtocolVersion(
  const PString & version
)
{
  if( m_Version != NULL )
    delete m_Version;

  SDPHeader h = version;
  h.SetSDPName( "a" );
  m_Version = new SDPProtocolVersion( h );
}

//o=  (owner/creator and session identifier).
void SDPSessionDesc::SetOrigin(
  const SDPOrigin & origin
)
{
  if( m_Origin != NULL )
    delete m_Origin;

  SDPHeader h = origin;
  h.SetSDPName( "o" );
  m_Origin = new SDPOrigin( h );
}

//s=  (session name)
void SDPSessionDesc::SetSessionName(
  const SDPSessionName & sessionName
)
{
  if( m_SessionName != NULL )
    delete m_SessionName;

  m_SessionName = new SDPSessionName( sessionName );
}

void SDPSessionDesc::SetSessionName(
  const PString & sessionName
)
{
  if( m_SessionName != NULL )
    delete m_SessionName;

  SDPHeader h = sessionName;
  h.SetSDPName( "s" );
  m_SessionName = new SDPSessionName( h );
}

//i=* (session information)
void SDPSessionDesc::SetSessionInformation(
  const PString & sessionInformation
)
{
  if( m_SessionInformation != NULL )
    delete m_SessionInformation;

  m_SessionInformation = new SDPHeader( sessionInformation );
  m_SessionInformation->SetSDPName( "i" );
}

//u=* (URI of description)
void SDPSessionDesc::SetURIDescription(
  const PString & uriDesc
)
{
  if( m_URIDescription != NULL )
    delete m_URIDescription;

  SDPHeader h = uriDesc;
  h.SetSDPName( "u" );
  m_URIDescription = new SDPURI( h );
}

//e=* (email address)
void SDPSessionDesc::SetEmailAddress(
  const PString & email
)
{
  if( m_EmailAddress != NULL )
    delete m_EmailAddress;

  SDPHeader h = email;
  h.SetSDPName( "e" );
  m_EmailAddress = new SDPHeader( h );
}

//p=* (phone number)
void SDPSessionDesc::SetPhoneNumber(
  const PString & phone
)
{
  if( m_PhoneNumber != NULL )
    delete m_PhoneNumber;

  m_PhoneNumber = new SDPHeader( phone );
  m_PhoneNumber->SetSDPName( "p" );
}

//b=* (bandwidth information)
void SDPSessionDesc::SetBandwidth(
  const SDPBandwidth & bandwidth
)
{
  if( m_Bandwidth != NULL )
    delete m_Bandwidth;

  m_Bandwidth = new SDPBandwidth( bandwidth );
}

//One or more time descriptions (see below)
//z=* (time zone adjustments)
//void SDPSessionDesc::SetTimeZoneAdjustments(
//  const PString & timeZoneAdjustments
//)
//{
//}

//Zero or more media descriptions (see below)
//t=  (time the session is active)
void SDPSessionDesc::SetTime(
  const SDPTime & time
)
{
  if( m_Time != NULL )
    delete m_Time;

  m_Time = new SDPTime( time );
}

//r=* (zero or more repeat times)
void SDPSessionDesc::SetRepeatTimes(
  const PString & repeatTimes
)
{
  if( m_RepeatTimes != NULL )
    delete m_RepeatTimes;

  m_RepeatTimes = new SDPHeader( repeatTimes );
  m_RepeatTimes->SetSDPName( "r" );
}

void SDPSessionDesc::SetConnectionData(
  const SDPConnectionData & connectionData
)
{
  if( m_ConnectionData != NULL )
    delete m_ConnectionData;

  m_ConnectionData = new SDPConnectionData( connectionData );
}

void SDPSessionDesc::SetEncryptionKey(
  const PString & key
)
{
  if( m_EncryptionKey != NULL )
    delete m_EncryptionKey;

  m_EncryptionKey = new SDPHeader( key );
  m_EncryptionKey->SetSDPName( "k" );
}

void SDPSessionDesc::AppendAttribute(
  const SDPAttribute & attribute
)
{
  m_AttributesMutex.Wait();
  m_Attributes.Append( new SDPAttribute( attribute ) );
  m_AttributesMutex.Signal();
}

BOOL SDPSessionDesc::GetAttribute(
  SDPAttribute & attribute,
  PINDEX i
)
{
  m_AttributesMutex.Wait();
  if( i >= m_Attributes.GetSize() )
  {
    m_AttributesMutex.Signal();
    return FALSE;
  }

  attribute = m_Attributes[i];
  m_AttributesMutex.Signal();
  return TRUE;
}

BOOL SDPSessionDesc::RemoveAttribute(
  PINDEX i
)
{
  m_AttributesMutex.Wait();
  if( i >= m_Attributes.GetSize() )
  {
    m_AttributesMutex.Signal();
    return FALSE;
  }

  m_Attributes.RemoveAt(i);
  m_AttributesMutex.Signal();
  return TRUE;
}

PINDEX SDPSessionDesc::GetAttributeSize()
{
  return m_Attributes.GetSize();
}

void SDPSessionDesc::PrintOn(
  ostream & strm
)const
{
  if( m_Version != NULL )
    strm << *m_Version << "\r\n";

  //o=  (owner/creator and session identifier).
  if( m_Origin != NULL )
    strm << *m_Origin << "\r\n";

  //s=  (session name)
  if( m_SessionName != NULL )
    strm << *m_SessionName << "\r\n";
  else
    strm << "s=-" << "\r\n";
  //i=* (session information)
  if( m_SessionInformation != NULL )
    strm << *m_SessionInformation << "\r\n";

  //u=* (URI of description)
  if( m_URIDescription != NULL )
    strm << *m_URIDescription << "\r\n";

  //e=* (email address)
  if( m_EmailAddress != NULL )
    strm << *m_EmailAddress << "\r\n";

  //p=* (phone number)
  if( m_PhoneNumber != NULL )
    strm << *m_PhoneNumber << "\r\n";

  //c=* (connection information - not required if included in all media)
  if( m_ConnectionData != NULL )
    strm << *m_ConnectionData << "\r\n";

  //b=* (bandwidth information)
  if( m_Bandwidth != NULL )
    strm << *m_Bandwidth << "\r\n";

  //k=* (encryption key)
  if( m_EncryptionKey != NULL )
    strm << *m_EncryptionKey << "\r\n";

  //t=  (time the session is active)
  if( m_Time != NULL )
    strm << *m_Time << "\r\n";

  for( PINDEX i = 0; i < m_Attributes.GetSize(); i++ )
    strm << m_Attributes[i] << "\r\n";

  //r=* (zero or more repeat times)
  if( m_RepeatTimes != NULL )
    strm << *m_RepeatTimes << "\r\n";

}






