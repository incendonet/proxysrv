/*
 *
 * DialogId.cxx
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
 * $Log: DialogId.cxx,v $
 * Revision 1.5  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "DialogId.h"

#define new PNEW

using namespace UACORE;

DialogId::DialogId()
{
}

DialogId::DialogId(
  const PString & dialogId
)
{
  operator=( dialogId );
}

DialogId::DialogId(
  const DialogId & dialogId
)
{
  operator=( dialogId );
}



DialogId::DialogId( 
  const PString & callId,
  const PString & localTag,
  const PString & remoteTag
) : m_LocalTag( localTag ),
    m_RemoteTag( remoteTag ),
    m_CallId( callId )
{
}

DialogId::~DialogId()
{
}

DialogId & DialogId::operator=(
  const DialogId & dialogId
)
{
  m_LocalTag = dialogId.m_LocalTag;
  m_RemoteTag = dialogId.m_RemoteTag;
  m_CallId = dialogId.m_CallId;
  return *this;
}

DialogId & DialogId::operator=(
  const PString & dialogId
)
{
  Parse( dialogId );
  return *this;
}

BOOL DialogId::Parse( 
  const PString & dialogId
)
{
  PStringArray tokens = dialogId.Tokenise( '|' );
  if( tokens.GetSize() != 3 )
    return FALSE;

  m_CallId = tokens[0];
  m_LocalTag = tokens[1];
  m_RemoteTag = tokens[2];
  
  return TRUE;
}

BOOL DialogId::operator==(
  const PString & dialogId
)
{
  return dialogId == AsString();
}

BOOL DialogId::operator==(
  const DialogId & dialogId
)
{
  return dialogId.AsString() == AsString();
}

PString DialogId::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

void DialogId::PrintOn( 
  ostream & strm 
)const
{
  strm << m_CallId << "|" << m_LocalTag << "|" << m_RemoteTag;
}

