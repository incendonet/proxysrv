/*
 *
 * StartLine.cxx
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
 * $Log: StartLine.cxx,v $
 * Revision 1.4  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:53:54  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "StartLine.h"

#define new PNEW

using namespace SIPParser;

StartLine::StartLine()
{
  m_IsRequest = -1;
  m_RequestLine = NULL;
  m_StatusLine = NULL;
}

StartLine::StartLine(
  const StartLine & startLine
)
{
  m_IsRequest = -1;
  m_RequestLine = NULL;
  m_StatusLine = NULL;
  operator=(startLine);
}

StartLine::StartLine(
  const PString & startLine
)
{
  m_IsRequest = -1;
  m_RequestLine = NULL;
  m_StatusLine = NULL;
  operator=(startLine);
}

StartLine::StartLine(
  const StatusLine & statusLine
)
{
  m_IsRequest = -1;
  m_RequestLine = NULL;
  m_StatusLine = NULL;
  operator=(statusLine);
}

StartLine::StartLine(
  const RequestLine & requestLine
)
{
  m_IsRequest = -1;
  m_RequestLine = NULL;
  m_StatusLine = NULL;
  operator=(requestLine);
}

StartLine::~StartLine()
{
  CleanUp();
}

StartLine & StartLine::operator=(
  const StartLine & startLine
)
{
  CleanUp();
  m_IsRequest = startLine.m_IsRequest;
  if( startLine.m_RequestLine != NULL )
    m_RequestLine = (RequestLine*)startLine.m_RequestLine->Clone();

  if( startLine.m_StatusLine != NULL )
    m_StatusLine = (StatusLine*)startLine.m_StatusLine->Clone();
  return *this;
}

StartLine & StartLine::operator=(
  const PString & startLine
)
{
  CleanUp();

  PINDEX sp = startLine.Find( ' ' );
  PString hint = startLine.Left( sp ).Trim();
  
  if( hint *= "SIP/2.0" )
  {
    m_IsRequest = FALSE;
    m_StatusLine = new StatusLine( startLine );
  }else
  {
    m_IsRequest = TRUE;
    m_RequestLine = new RequestLine( startLine );
  }

  return *this;
  
}

StartLine & StartLine::operator=(
  const StatusLine & statusLine
)
{
  CleanUp();
  m_StatusLine = new StatusLine(statusLine);
  m_IsRequest = FALSE;

  return *this;
}

StartLine & StartLine::operator=(
  const RequestLine & requestLine
)
{
  CleanUp();
  m_RequestLine = new RequestLine(requestLine);
  m_IsRequest = TRUE;
  return *this;
}



StartLine::operator PString()
{
  return AsString();
}

PString StartLine::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

void StartLine::PrintOn(
  ostream & strm
) const
{
  if( m_RequestLine != NULL )
  {
    m_RequestLine->PrintOn( strm );
    return;
  }

  if( m_StatusLine != NULL )
  {
    m_StatusLine->PrintOn( strm );
    return;
  }
}

void StartLine::CleanUp()
{
  m_IsRequest = -1;

  if( m_RequestLine != NULL )
  {
    delete m_RequestLine;
    m_RequestLine = NULL;
  }

  if( m_StatusLine != NULL )
  {
    delete m_StatusLine;
    m_StatusLine = NULL;
  }
}

PObject * StartLine::Clone()const
{
  return new StartLine( *this );
}

