/*
 *
 * RecordRouteBranch.cxx
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
 * $Log: RecordRouteBranch.cxx,v $
 * Revision 1.4  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "RecordRouteBranch.h"

#define new PNEW

using namespace SIPParser;


RecordRouteBranch::RecordRouteBranch()
{
}

RecordRouteBranch::RecordRouteBranch(
  const RecordRouteBranch & tag
)
{
  operator=( tag );
}

RecordRouteBranch::RecordRouteBranch(
  const PString & tag
)
{
  operator=( tag );
}

RecordRouteBranch::RecordRouteBranch(
  const SIPMessage & msg
)
{
  operator=( msg );
}

RecordRouteBranch & RecordRouteBranch::operator=(
  const RecordRouteBranch & tag
)
{
  m_Tag = tag.m_Tag;
  m_Hash = tag.m_Hash;
  return *this;
}

RecordRouteBranch & RecordRouteBranch::operator=(
  const PString & tag
)
{
  PStringArray tokens = tag.Tokenise( '-' );
  if( tokens.GetSize() == 2 )
  {
    m_Tag = tokens[0];
    m_Hash = tokens[1];
  }

  return *this;
}

RecordRouteBranch & RecordRouteBranch::operator=(
  const SIPMessage & msg
)
{
  
  /*
  Loop detection is performed by verifying that, when a request
  returns to a proxy, those fields having an impact on the
  processing of the request have not changed.  The value placed
  in this part of the branch parameter SHOULD reflect all of
  those fields (including any Route, Proxy-Require and Proxy-
  Authorization header fields).  This is to ensure that if the
  request is routed back to the proxy and one of those fields
  changes, it is treated as a spiral and not a loop (see Section
  16.3).  A common way to create this value is to compute a
  cryptographic hash of the To tag, From tag, Call-ID header
  field, the Request-URI of the request received (before
  translation), the topmost Via header, and the sequence number
  from the CSeq header field, in addition to any Proxy-Require
  and Proxy-Authorization header fields that may be present.
  */
  
  if( !msg.IsRequest() )
  {
    m_Tag = PString::Empty();
    m_Hash = PString::Empty();
    return *this;
  }
  
  PString hashKey;

  PString toTag;
  To to;
  msg.GetTo( to );
  to.GetParameter( "tag", toTag );
  hashKey += toTag;
  
  PString fromTag;
  From from;
  msg.GetFrom( from );
  from.GetParameter( "tag", fromTag );
  hashKey += fromTag;

  PString callId;
  CallId cid;
  msg.GetCallId( cid );
  callId = cid.GetHeaderBody();
  hashKey += callId;

  PString requestURI;
  RequestLine rline;
  msg.GetRequestLine( rline );
  requestURI = rline.GetRequestURI().AsString();
  hashKey += requestURI;

  PString viaString;
  Via via;
  if( msg.GetViaSize() > 0 )
    msg.GetViaAt( 0, via );
  viaString = via.GetHeaderBody();
  hashKey += viaString;

  DWORD cseqSequence;
  CSeq cseq;
  msg.GetCSeq( cseq );
  cseqSequence = cseq.GetSequence();
  hashKey += PString( cseqSequence );

  PMessageDigest5::Code digest;
  PMessageDigest5 digestor;
  digestor.Start();
  digestor.Process( hashKey );
  digestor.Complete( digest );

  m_Hash = ParserTools::AsHex( digest );
  m_Tag = ParserTools::GenBranchParameter();

  return *this;
}

BOOL RecordRouteBranch::operator==(
  const RecordRouteBranch & tag
)
{
  return m_Hash == tag.m_Hash && m_Tag == tag.m_Tag;
}

BOOL RecordRouteBranch::operator==(
  const PString & tag
)
{
 
  PString localTag = AsString();
  
  if( localTag.IsEmpty() )
    return FALSE;

  return tag *= localTag;
}

BOOL RecordRouteBranch::operator*=(
  const RecordRouteBranch & tag
)
{
  if( tag.m_Hash.IsEmpty() )
    return FALSE;

  return m_Hash == tag.m_Hash;
}

BOOL RecordRouteBranch::operator*=(
  const PString & tag
)
{
  RecordRouteBranch branch( tag );
  return operator*=( branch );
}


PString RecordRouteBranch::AsString()
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

void RecordRouteBranch::PrintOn( 
  ostream & strm
)const
{
  if( !m_Tag.IsEmpty() && !m_Hash.IsEmpty() )
    strm << m_Tag << "-" << m_Hash;
}

