/*
 *
 * MD5Authorization.h
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
 * $Log: MD5Authorization.h,v $
 * Revision 1.6  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.4  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef MD5AUTHORIZATION_H
#define MD5AUTHORIZATION_H

#include "A1Hash.h"
#include "A2Hash.h"
#include "Nonce.h"
#include "Logger.h"

using namespace Tools;


namespace MD5
{
  class MD5Authorization : public MD5Hash
  {
    PCLASSINFO( MD5Authorization, MD5Hash );
  public:
    MD5Authorization();

    MD5Authorization( 
      const MD5Authorization & hash 
    );

    MD5Authorization(
      const A1Hash & a1,
      const Nonce & nonce,
      const A2Hash & a2
    );

    MD5Authorization(
      const PString & a1,
      const Nonce & nonce,
      const A2Hash & a2
    );

    MD5Authorization(
      const PString & a1,
      const PString & nonce,
      const A2Hash & a2
    );

    BOOL Construct();

    static PString Construct(
      const PString & a1,
      const PString & nonce,
      const PString & a2
    );

    BOOL operator==( const MD5Authorization & digest );

    MD5Authorization & operator=( const MD5Authorization & digest );

    PINLINE const A1Hash & GetA1Hash()const{ return m_A1; };
    PINLINE void SetA1Hash( const A1Hash & hash ){ m_A1 = hash; };
    PINLINE const A2Hash & GetA2Hash()const{ return m_A2; };
    PINLINE void SetA2Hash( const A2Hash & hash ){ m_A2 = hash; };
    PINLINE const Nonce & GetNonce()const{ return m_Nonce; };
    PINLINE void SetNonce( const Nonce & hash ){ m_Nonce = hash; };
  
  protected:
    A1Hash m_A1;
    A2Hash m_A2;
    Nonce m_Nonce;
  };
};



#endif


