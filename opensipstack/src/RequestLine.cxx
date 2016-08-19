/*
 *
 * RequestLine.cxx
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
 * $Log: RequestLine.cxx,v $
 * Revision 1.4  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "RequestLine.h"

#define new PNEW

using namespace SIPParser;

RequestLine::RequestLine()
{
  m_SIPVersion = "SIP/2.0";
}

RequestLine::RequestLine(
  const PString & method,
  const SIPURI & requestURI,
  const PString & version
)
{
  m_Method = method;
  m_RequestURI = requestURI;
  m_SIPVersion = version;
}
    
RequestLine::RequestLine(
  const RequestLine & requestLine
)
{
  operator=( requestLine );
}

RequestLine::RequestLine(
  const PString & requestLine
)
{
  operator=( requestLine );
}

RequestLine & RequestLine::operator=( 
  const RequestLine & r
)
{
  m_Method = r.m_Method;
  m_RequestURI = r.m_RequestURI;
  m_SIPVersion = r.m_SIPVersion;
  return *this;
}

RequestLine & RequestLine::operator=( 
  const PString & requestLine
)
{
  ///Request-Line  =  Method SP Request-URI SP SIP-Version CRLF

  PStringArray tokens = requestLine.Tokenise( ' ', FALSE );

  if( tokens.GetSize() == 3 )
  {
    m_Method = tokens[0];
    m_RequestURI = tokens[1];
    m_SIPVersion = tokens[2];
  }

  return *this;
}

RequestLine::operator PString()
{
  return AsString();
}

void RequestLine::PrintOn(
  ostream & strm
)const
{
  strm << m_Method << " " << m_RequestURI << " " << m_SIPVersion;
}

PString RequestLine::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

PObject * RequestLine::Clone()const
{
  return new RequestLine( *this );
}

