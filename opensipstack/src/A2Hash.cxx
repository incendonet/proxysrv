/*
 *
 * A2Hash.cxx
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
 * $Log: A2Hash.cxx,v $
 * Revision 1.4  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "A2Hash.h"
#include "ParserTools.h"

#define new PNEW

using namespace MD5;
using namespace SIPParser;

A2Hash::A2Hash()
{
}

A2Hash::A2Hash( 
  const A2Hash & hash 
)
{
  operator=( hash );
}

A2Hash::A2Hash(
  const PString & method,
  const PString & uri
  ) : m_Method( method.ToUpper() ),
    m_URI( uri )
{
  PMessageDigest5 digestor_a1;
  digestor_a1.Start();
  digestor_a1.Process( ParserTools::UnQuote( m_Method ) );
  digestor_a1.Process(":");
  digestor_a1.Process( ParserTools::UnQuote( m_URI ) );
  digestor_a1.Complete( m_Digest );
  m_IsConstructed = TRUE;
}

BOOL A2Hash::Construct()
{
  if( m_IsConstructed )
    return TRUE;

  if( m_Method.IsEmpty() || m_URI.IsEmpty() )
  {
    PTRACE( 2, "A2Hash Error - Empty Method or URI" );
    return FALSE;
  }

  PMessageDigest5 digestor_a1;
  digestor_a1.Start();
  digestor_a1.Process( ParserTools::UnQuote( m_Method ) );
  digestor_a1.Process(":");
  digestor_a1.Process( ParserTools::UnQuote( m_URI ) );
  digestor_a1.Complete( m_Digest );

  m_IsConstructed = TRUE;

  return m_IsConstructed;
}


BOOL A2Hash::operator==( const A2Hash & digest )
{
  return MD5Hash::operator==( digest.GetDigest() );
}



A2Hash & A2Hash::operator=( const A2Hash & digest )
{
  m_Digest = digest.m_Digest;
  m_Method = digest.m_Method;
  m_URI = digest.m_URI;
  m_IsConstructed = digest.m_IsConstructed;
  return *this;
}


