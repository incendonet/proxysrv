/*
 *
 * SDPMediaDesc.cxx
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
 * $Log: SDPMediaDescription.cxx,v $
 * Revision 1.8  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.7  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.6  2006/03/15 16:40:46  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 * 3.  Fixed SDP parser bugs
 * 5.  Renamed SDP Classes to avoid collision with OPAL classes with the same name
 * 4.  Worked on OpalOSSConnection RTP
 *
 * Revision 1.5  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SDPMediaDescription.h"

using namespace SDP;

#define new PNEW



SDPMediaDesc::SDPMediaDesc()
{
  m_MediaAnnouncement = NULL;
  m_MediaTitle = NULL;
  m_ConnectionData = NULL;
  m_Bandwidth = NULL;
  m_EncryptionKey = NULL;
}

SDPMediaDesc::SDPMediaDesc(
  const SDPMediaAnnouncement & mediaAnnouncement
)
{
  m_MediaAnnouncement = NULL;
  m_MediaTitle = NULL;
  m_ConnectionData = NULL;
  m_Bandwidth = NULL;
  m_EncryptionKey = NULL;

  SetMediaAnnouncement( mediaAnnouncement );
}

SDPMediaDesc::SDPMediaDesc(
  const PString & mediaType,
  WORD port,
  const PString & transport
)
{
  m_MediaAnnouncement = NULL;
  m_MediaTitle = NULL;
  m_ConnectionData = NULL;
  m_Bandwidth = NULL;
  m_EncryptionKey = NULL;

  SDPMediaAnnouncement audioMa;
  audioMa.SetPort( port );
  audioMa.SetMedia( mediaType );
  audioMa.SetTransport( transport );
  SetMediaAnnouncement( audioMa );
}

SDPMediaDesc::~SDPMediaDesc()
{
  Cleanup();
}

SDPMediaDesc::SDPMediaDesc(
  const SDPMediaDesc & mediaDescription
)
{
  m_MediaAnnouncement = NULL;
  m_MediaTitle = NULL;
  m_ConnectionData = NULL;
  m_Bandwidth = NULL;
  m_EncryptionKey = NULL;
  operator=( mediaDescription );
}

SDPMediaDesc::SDPMediaDesc(
  PString & tokens
)
{
  m_MediaAnnouncement = NULL;
  m_MediaTitle = NULL;
  m_ConnectionData = NULL;
  m_Bandwidth = NULL;
  m_EncryptionKey = NULL;
  operator=( tokens );
}

SDPMediaDesc::SDPMediaDesc(
  PStringArray & tokens
)
{
  m_MediaAnnouncement = NULL;
  m_MediaTitle = NULL;
  m_ConnectionData = NULL;
  m_Bandwidth = NULL;
  m_EncryptionKey = NULL;
  operator=(tokens);
}

void SDPMediaDesc::Cleanup()
{
  if( m_MediaAnnouncement != NULL )
  {
    delete m_MediaAnnouncement;
    m_MediaAnnouncement = NULL;
  }

  if( m_MediaTitle != NULL )
  {
    delete m_MediaTitle;
    m_MediaTitle = NULL;
  }

  if( m_ConnectionData != NULL )
  {
    delete m_ConnectionData;
    m_ConnectionData = NULL;
  }

  if( m_Bandwidth != NULL )
  {
    delete m_Bandwidth;
    m_Bandwidth = NULL;
  }

  if( m_EncryptionKey != NULL )
  {
    delete m_EncryptionKey;
    m_EncryptionKey = NULL;
  }

  m_Attributes.MakeUnique();
  m_Attributes.RemoveAll();
}


SDPMediaDesc & SDPMediaDesc::operator=(
  const PString & _tokens
)
{
  PStringArray tokens;
  SDPHeader::Tokenize( _tokens, tokens );
  return operator=( tokens );
}

SDPMediaDesc & SDPMediaDesc::operator=(
  const PStringArray & tokens
)
{
    //SDPMediaAnnouncement m_MediaAnnouncement;
    //SDPHeader * m_MediaTitle;
    //SDPConnectionData * m_ConnectionData;
    //SDPBandwidth * m_Bandwidth;
    //SDPHeader * m_EncryptionKey;
    //SDPAttribute::Collection m_Attributes;

      /*
    Media description
        m=  (media name and transport address)
        i=* (media title)
        c=* (connection information - optional if included at session-level)
        b=* (bandwidth information)
        k=* (encryption key)
        a=* (zero or more media attribute lines)
    */
  SDPHeader h = tokens[0];
  if( h.GetSDPName() *= "m" )
  {
    m_MediaAnnouncement = new SDPMediaAnnouncement(h);
    for( PINDEX i = 1; i < tokens.GetSize(); i++ )
    {
      h = tokens[i];
      if( h.GetSDPName() *= "i" )
      {
        if( m_MediaTitle != NULL )
          delete m_MediaTitle;

        m_MediaTitle = new SDPHeader( h );
      }else if( h.GetSDPName() *= "c" )
      {
        if( m_ConnectionData != NULL )
          delete m_ConnectionData;

        m_ConnectionData = new SDPConnectionData( h );
      }else if( h.GetSDPName() *= "b" )
      {
        if( m_Bandwidth != NULL )
          delete m_Bandwidth;

        m_Bandwidth = new SDPBandwidth( h );
      }else if( h.GetSDPName() *= "k" )
      {
        if( m_EncryptionKey != NULL )
          delete m_EncryptionKey;

        m_EncryptionKey = new SDPHeader( h );
      }else if( h.GetSDPName() *= "a" )
      {
        m_Attributes.Append( new SDPAttribute( h ) );
      }else
      {
        PTRACE( 1, "Unknown SDP header - " << h );
      }
    }
  }
  return *this;
}



SDPMediaDesc & SDPMediaDesc::operator=(
  const SDPMediaDesc & h
)
{


    //SDPMediaAnnouncement m_MediaAnnouncement;
    //SDPHeader * m_MediaTitle;
    //SDPConnectionData * m_ConnectionData;
    //SDPBandwidth * m_Bandwidth;
    //SDPHeader * m_EncryptionKey;
    //SDPAttribute::Collection m_Attributes;
  if( h.m_MediaAnnouncement != NULL )
    m_MediaAnnouncement = new SDPMediaAnnouncement( *h.m_MediaAnnouncement );

  if( h.m_MediaTitle != NULL )
    m_MediaTitle = new SDPHeader( *h.m_MediaTitle );

  if( h.m_ConnectionData != NULL )
    m_ConnectionData = new SDPConnectionData( *h.m_ConnectionData );

  if( h.m_Bandwidth != NULL )
    m_Bandwidth = new SDPBandwidth( *h.m_Bandwidth );

  if( h.m_EncryptionKey != NULL )
    m_EncryptionKey = new SDPHeader( *h.m_EncryptionKey );

  m_Attributes = h.m_Attributes;
  m_Attributes.MakeUnique();

  return *this;
}

void SDPMediaDesc::PrintOn(
  ostream & strm
)const
{
  if( m_MediaAnnouncement != NULL )
    strm << *m_MediaAnnouncement << "\r\n";

  if( m_MediaTitle != NULL )
    strm << *m_MediaTitle << "\r\n";

  if( m_ConnectionData != NULL )
    strm << *m_ConnectionData << "\r\n";

  if( m_Bandwidth != NULL )
    strm << *m_Bandwidth << "\r\n";

  if( m_EncryptionKey != NULL )
    strm << *m_EncryptionKey << "\r\n";

  for( PINDEX i = 0; i < m_Attributes.GetSize(); i++ )
  {
    strm << m_Attributes[i];

    if( i < m_Attributes.GetSize() )
      strm << "\r\n";
  }
}


PObject * SDPMediaDesc::Clone()const
{
  SDPMediaDesc * md = new SDPMediaDesc();

  ///SDPMediaAnnouncement * m_MediaAnnouncement;
  if( m_MediaAnnouncement != NULL )
    md->m_MediaAnnouncement = (SDPMediaAnnouncement*)m_MediaAnnouncement->Clone();

  ///SDPHeader * m_MediaTitle;
  if( m_MediaTitle != NULL )
    md->m_MediaTitle = (SDPHeader*)m_MediaTitle->Clone();

  ///SDPConnectionData * m_ConnectionData;
  if( m_ConnectionData != NULL )
    md->m_ConnectionData = (SDPConnectionData*)m_ConnectionData->Clone();

  ///SDPBandwidth * m_Bandwidth;
  if( m_Bandwidth != NULL )
    md->m_Bandwidth = (SDPBandwidth*)m_Bandwidth->Clone();

  ///SDPHeader * m_EncryptionKey;
  if( m_EncryptionKey != NULL )
    md->m_EncryptionKey = (SDPHeader*)m_EncryptionKey->Clone();

  md->m_Attributes = m_Attributes;
  md->m_Attributes.MakeUnique();

  return md;
}

void SDPMediaDesc::SetMediaAnnouncement(
  const SDPMediaAnnouncement & mediaAnnouncement
)
{
  if( m_MediaAnnouncement != NULL )
    delete m_MediaAnnouncement;

  m_MediaAnnouncement = new SDPMediaAnnouncement( mediaAnnouncement );
}

void SDPMediaDesc::SetMediaTitle(
  const PString & mediaTitle
)
{
  if( m_MediaTitle != NULL )
    delete m_MediaTitle;

  m_MediaTitle = new SDPHeader( mediaTitle );
  m_MediaTitle->SetSDPName( "i" );
}

void SDPMediaDesc::SetConnectionData(
  const SDPConnectionData & connectionData
)
{
  if( m_ConnectionData != NULL )
    delete m_ConnectionData;

  m_ConnectionData = new SDPConnectionData( connectionData );

}

void SDPMediaDesc::SetBandwidth(
  const SDPBandwidth & bandwidth
)
{
  if( m_Bandwidth != NULL )
    delete m_Bandwidth;

  m_Bandwidth = new SDPBandwidth( bandwidth );
}

void SDPMediaDesc::SetEncryptionKey(
  const PString & key
)
{
          /*
    Media description
        m=  (media name and transport address)
        i=* (media title)
        c=* (connection information - optional if included at session-level)
        b=* (bandwidth information)
        k=* (encryption key)
        a=* (zero or more media attribute lines)
    */

    //SDPMediaAnnouncement * m_MediaAnnouncement;
    //SDPHeader * m_MediaTitle;
    //SDPConnectionData * m_ConnectionData;
    //SDPBandwidth * m_Bandwidth;
    //SDPHeader * m_EncryptionKey;
    //SDPAttribute::Collection m_Attributes;

  if( m_EncryptionKey != NULL )
    delete m_EncryptionKey;

  m_EncryptionKey = new SDPHeader( key );
  m_EncryptionKey->SetSDPName( "k" );
}

void SDPMediaDesc::AppendHeader(
  const PString & header
)
{
  SDPHeader h = header;
  AppendHeader( h );
}

void SDPMediaDesc::AppendHeader(
  const SDPHeader & h
)
{
  if( h.GetSDPName() *= "i" )
  {
    if( m_MediaTitle != NULL )
      delete m_MediaTitle;

    m_MediaTitle = new SDPHeader( h );
  }else if( h.GetSDPName() *= "c" )
  {
    if( m_ConnectionData != NULL )
      delete m_ConnectionData;

    m_ConnectionData = new SDPConnectionData( h );
  }else if( h.GetSDPName() *= "b" )
  {
    if( m_Bandwidth != NULL )
      delete m_Bandwidth;

    m_Bandwidth = new SDPBandwidth( h );
  }else if( h.GetSDPName() *= "k" )
  {
    if( m_EncryptionKey != NULL )
      delete m_EncryptionKey;

    m_EncryptionKey = new SDPHeader( h );
  }else if( h.GetSDPName() *= "a" )
  {
    m_Attributes.Append( new SDPAttribute( h ) );
  }else
  {
    PTRACE( 1, "Unknown SDP header - " << h );
  }
}

void SDPMediaDesc::AppendAttribute(
  const SDPAttribute & attribute
)
{
  m_AttributesMutex.Wait();
  m_Attributes.Append( new SDPAttribute( attribute ) );
  m_AttributesMutex.Signal();
}

void SDPMediaDesc::AppendAttribute(
  const SDPMediaFmt & _format
)
{
  SDPMediaFmt format = _format;

  PString payloadName = format.GetEncodingName();
  if( payloadName.Find( "G729" ) != P_MAX_INDEX )
  {
    payloadName = "G729";
  }

  PString pt( format.GetPayloadType() );
  PStringStream ptName;
  ptName << payloadName << "/" << format.GetClockRate();
  SDPAttribute attribute( "rtpmap", pt, ptName );
  AppendAttribute( attribute );

#if 0
  if( format.GetSamplingRate() != 0 )
  {
    SDPAttribute ptime( "ptime", PString( format.GetSamplingRate() ), "" );
    AppendAttribute( ptime );
  }
#endif

  PString encodingName = format.GetEncodingName();

  if( encodingName == "G729ab" || encodingName == "G729b" )
  {
    format.SetEncodingName( "G729" );
    SDPAttribute fmtp( "a", "fmtp:18 annexb=yes" );
    AppendAttribute( fmtp );
  }else if( encodingName == "G729" || encodingName == "G729a" )
  {
    format.SetEncodingName( "G729" );
    if( encodingName == "G729a" )
    {
      SDPAttribute fmtp( "a", "fmtp:18 annexb=no" );
      AppendAttribute( fmtp );
    }
  }

  const PStringArray & customAttributes = format.GetCustomAttributes();

  for( PINDEX i = 0; i < customAttributes.GetSize(); i++ )
  {
    SDPAttribute custom( customAttributes[i] );
    AppendAttribute( attribute );
  }
}


BOOL SDPMediaDesc::GetAttribute(
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

BOOL SDPMediaDesc::RemoveAttribute(
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

PINDEX SDPMediaDesc::GetAttributeSize()
{
  return m_Attributes.GetSize();
}






