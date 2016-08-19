/*
 *
 * SDPHeader.cxx
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
 * $Log: SDPHeader.cxx,v $
 * Revision 1.4  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SDPHeader.h"

using namespace SDP;

#define new PNEW

SDPHeader::SDPHeader()
{
}

SDPHeader::~SDPHeader()
{
}

SDPHeader::SDPHeader( 
  const SDPHeader & header
)
{
  operator=( header );
}

SDPHeader::SDPHeader(
  const PString & header
)
{
  operator=( header );
}

SDPHeader::SDPHeader(
  const PString & name,
  const PString & value
)
{
  m_Name = name.Trim();
  m_Value = value.Trim();
}

SDPHeader & SDPHeader::operator=( 
  const SDPHeader & header 
)
{
  m_Name = header.m_Name;
  m_Value = header.m_Value;
  return *this;
}

SDPHeader & SDPHeader::operator=( 
  const PString & header 
)
{
  PINDEX equal = header.Find( "=" );

  if( equal != P_MAX_INDEX )
  {
    m_Name = header.Left( equal ).Trim();
    m_Value = header.Mid( equal + 1 ).Trim();
  }else
  {
    m_Value = header.Trim();
  }

  return *this;
}

void SDPHeader::PrintOn( 
  ostream & strm 
)const
{
  strm << m_Name << "=" << m_Value;
}

PString SDPHeader::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

PObject * SDPHeader::Clone()const
{
  return new SDPHeader( *this );
}

SDPHeader::operator PString()
{
  return AsString();
}

void SDPHeader::Tokenize(
  const PString & sdp,
  PStringArray & tokens,
  BOOL includeMedia
)
{

  PINDEX lastOffSet  = 0;
  PINDEX offSet = 0;

  ///got start of message
  BOOL gotSOM = FALSE;

  BOOL crLFCompliant = sdp.Find( "\r\n" ) != P_MAX_INDEX;

  for(;;)
  {
    lastOffSet = offSet;

    if( !crLFCompliant )
      offSet = sdp.FindOneOf( "\r\n", offSet );
    else
      offSet = sdp.Find( "\r\n", offSet );
    
    if( offSet != P_MAX_INDEX )
    {
      //
      //  lastOffSet++;
      PString token = sdp.Mid( lastOffSet, ++offSet - lastOffSet );

      token = token.Trim();

      if( !includeMedia )
      {
        SDPHeader h = token;
        if( h.GetSDPName() *= "m" )
          break;
      }

      if( !gotSOM && token.IsEmpty() )
        continue;
      else
      {
        tokens.AppendString( token );
        gotSOM = TRUE;
      }

    }else
    {
      break;
    }


  }
}
