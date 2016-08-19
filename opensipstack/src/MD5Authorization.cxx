/*
 *
 * MD5Authorization.cxx
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
 * $Log: MD5Authorization.cxx,v $
 * Revision 1.5  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "MD5Authorization.h"
#include "ParserTools.h"

#define new PNEW

using namespace MD5;
using namespace SIPParser;

MD5Authorization::MD5Authorization()
{
}

MD5Authorization::MD5Authorization( 
  const MD5Authorization & hash 
)
{
  operator=( hash );
}

MD5Authorization::MD5Authorization(
  const A1Hash & a1,
  const Nonce & nonce,
  const A2Hash & a2
)
{
  m_A1 = a1;
  m_A2 = a2;
  m_Nonce = nonce;

  
}

MD5Authorization::MD5Authorization(
  const PString & a1,
  const Nonce & nonce,
  const A2Hash & a2
)
{
  m_A2 = a2;
  m_Nonce = nonce;

  if( !m_A2.Construct() )
  {
    PTRACE( 2, "MD5Authorization Error - Unable to construct A2 hash" );
  }

  if( !m_Nonce.Construct() )
  {
    PTRACE( 2, "MD5Authorization Error - Unable to construct NONCE hash" );
  }

  PMessageDigest5 digestor;
  digestor.Start();
  digestor.Process( a1 );
  digestor.Process( ":" );
  digestor.Process( m_Nonce.AsString() );
  digestor.Process( ":" );
  digestor.Process( m_A2.AsString() );
  digestor.Complete( m_Digest );

  m_IsConstructed = TRUE;
}

MD5Authorization::MD5Authorization(
  const PString & a1,
  const PString & nonce,
  const A2Hash & a2
)
{
  m_A2 = a2;

  if( !m_A2.Construct() )
  {
    PTRACE( 2, "MD5Authorization Error - Unable to construct A2 hash" );
  }

  PMessageDigest5 digestor;
  digestor.Start();
  digestor.Process( a1 );
  digestor.Process( ":" );
  digestor.Process( nonce );
  digestor.Process( ":" );
  digestor.Process( m_A2.AsString() );
  digestor.Complete( m_Digest );

  m_IsConstructed = TRUE;
}

BOOL MD5Authorization::Construct()
{
  if( m_IsConstructed )
    return TRUE;

  if( !m_A1.Construct() )
  {
    PTRACE( 2, "MD5Authorization Error - Unable to construct A1 hash" );
    return FALSE;
  }

  if( !m_A2.Construct() )
  {
    PTRACE( 2, "MD5Authorization Error - Unable to construct A2 hash" );
    return FALSE;
  }

  if( !m_Nonce.Construct() )
  {
    PTRACE( 2, "MD5Authorization Error - Unable to construct NONCE hash" );
    return FALSE;
  }

  PMessageDigest5 digestor;
  digestor.Start();
  digestor.Process( m_A1.AsString() );
  digestor.Process( ":" );
  digestor.Process( m_Nonce.AsString() );
  digestor.Process( ":" );
  digestor.Process( m_A2.AsString() );
  digestor.Complete( m_Digest );

  m_IsConstructed = TRUE;

  return m_IsConstructed;
}

PString MD5Authorization::Construct(
  const PString & a1,
  const PString & nonce,
  const PString & a2
)
{
  PMessageDigest5 digestor;
  PMessageDigest5::Code digest;
  digestor.Start();
  digestor.Process( a1);
  digestor.Process( ":" );
  digestor.Process( nonce );
  digestor.Process( ":" );
  digestor.Process( a2 );
  digestor.Complete( digest );
  return ParserTools::AsHex( digest );
}

BOOL MD5Authorization::operator==( const MD5Authorization & digest )
{
  return MD5Hash::operator==( digest.GetDigest() );
}

MD5Authorization & MD5Authorization::operator=( const MD5Authorization & digest )
{
  m_A1 = digest.GetA1Hash();
  m_A2 = digest.GetA2Hash();
  m_Nonce = digest.GetNonce();
  m_Digest = digest.m_Digest;
  m_IsConstructed = digest.m_IsConstructed;
  return *this;
}


