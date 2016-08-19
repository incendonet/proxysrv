/*
 *
 * CSeq.cxx
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
 * $Log: CSeq.cxx,v $
 * Revision 1.5  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/10/30 02:42:27  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "CSeq.h"

#define new PNEW

using namespace SIPParser;

CSeq::CSeq() 
  : SIPHeader( "CSeq" )
{
}

CSeq::CSeq( 
  const PString & cseq 
)  : SIPHeader( "CSeq", cseq )
{
}

CSeq::CSeq(
  const PString & method,
  DWORD sequence
)  : SIPHeader( "CSeq" )
{
  //CSeq: 4711 INVITE
  m_HeaderBody = PString( sequence ) + " " + method;
}

CSeq & CSeq::operator=( 
  DWORD sequence 
)
{
  if( m_HeaderBody.IsEmpty() )
  {
    m_HeaderBody = "0 Unknown";
  }

  PStringArray tokens = m_HeaderBody.Trim().Tokenise( ' ' );
  if( tokens.GetSize() == 2 )
  {
    m_HeaderBody = PString( sequence ) + " " + tokens[1];
  }

  return *this;
}

CSeq & CSeq::operator++()
{
  if( m_HeaderBody.IsEmpty() )
  {
    m_HeaderBody = "0 Unknown";
  }

  PStringArray tokens = m_HeaderBody.Trim().Tokenise( ' ' );
  if( tokens.GetSize() == 2 )
  {
    DWORD seq = (DWORD)tokens[0].AsUnsigned();
    seq++;
    m_HeaderBody = PString( seq ) + " " + tokens[1];
  }

  return *this;
}

PString CSeq::GetMethod()
{
  PStringArray tokens = m_HeaderBody.Trim().Tokenise( ' ' );
  if( tokens.GetSize() == 2 )
    return tokens[1].ToUpper().Trim();

  return PString::Empty();
}

DWORD CSeq::GetSequence()
{
  PStringArray tokens = m_HeaderBody.Trim().Tokenise( ' ' );
  if( tokens.GetSize() == 2 )
    return (DWORD)tokens[0].AsUnsigned();

  return 0;
}

void CSeq::SetSequence( 
  DWORD sequence 
)
{
  if( m_HeaderBody.IsEmpty() )
  {
    m_HeaderBody = "0 Unknown";
  }

  PStringArray tokens = m_HeaderBody.Trim().Tokenise( ' ' );
  if( tokens.GetSize() == 2 )
  {
    m_HeaderBody = PString( sequence ) + " " + tokens[1];
  }
}

void CSeq::SetMethod(
  const PString & method
)
{
  if( m_HeaderBody.IsEmpty() )
  {
    m_HeaderBody = "0 Unknown";
  }

  PStringArray tokens = m_HeaderBody.Trim().Tokenise( ' ' );
  if( tokens.GetSize() == 2 )
  {
    m_HeaderBody = tokens[0] + " " + method;
  }
}

PObject * CSeq::Clone()const
{
  return SIPHeader::Clone();
}

