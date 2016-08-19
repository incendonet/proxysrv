/*
 *
 * StatusLine.cxx
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
 * $Log: StatusLine.cxx,v $
 * Revision 1.4  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:53:54  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "StatusLine.h"
#include "ParserTools.h"

#define new PNEW

using namespace SIPParser;

StatusLine::StatusLine()
{
}

StatusLine::StatusLine(
  WORD statusCode,
  const PString & reasonPhrase,
  const PString & version
)
{
  m_Version = version;
  m_StatusCode = statusCode;
  m_ReasonPhrase = reasonPhrase;
}

StatusLine::StatusLine(
  const StatusLine & statusLine
)
{
  operator=( statusLine );  
}

StatusLine::StatusLine(
  const PString & statusLine
)
{
  operator=( statusLine );
}

StatusLine::StatusLine(
  WORD statusCode
)
{
  operator=( statusCode );
}

StatusLine & StatusLine::operator=(
  const StatusLine & statusLine
)
{
  m_Version = statusLine.m_Version;
  m_StatusCode = statusLine.m_StatusCode;
  m_ReasonPhrase = statusLine.m_ReasonPhrase;
  return *this;
}

StatusLine & StatusLine::operator=(
  const PString & statusLine
)
{
  //SIP/2.0 200 OK
  PINDEX sp1, sp2;
  sp1 = statusLine.Find( ' ' );
  sp2 = statusLine.Find( ' ', sp1 + 1 );

  if( sp1 != P_MAX_INDEX && sp2 != P_MAX_INDEX )
  {
    PStringArray tokens = statusLine.Tokenise( ' ' );
    m_Version = tokens[0];
    m_StatusCode = (WORD)tokens[1].AsUnsigned();
    m_ReasonPhrase = statusLine.Mid( sp2 + 1 ).Trim();
  }

  return *this;
}

StatusLine & StatusLine::operator=(
  WORD statusCode
)
{
  m_Version = "SIP/2.0";
  m_StatusCode = statusCode;
  m_ReasonPhrase = ParserTools::GetReasonPhrase( statusCode );
  return *this;
}

StatusLine::operator PString()
{
  return AsString();
}

StatusLine::operator WORD()
{
  return m_StatusCode;
}

void StatusLine::PrintOn(
  ostream & strm
)const
{
  strm << m_Version << " " << m_StatusCode << " " << m_ReasonPhrase;
}

PString StatusLine::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

PObject * StatusLine::Clone()const
{
  return new StatusLine( *this );
}

