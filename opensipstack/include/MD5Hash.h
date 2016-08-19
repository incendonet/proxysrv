/*
 *
 * MD5Hash.h
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
 * $Log: MD5Hash.h,v $
 * Revision 1.6  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/08/23 03:33:53  joegenbaclor
 * Some Opal related bug fixes discovered duriong intial development of ATLSIP
 *
 * Revision 1.4  2006/07/07 10:04:24  joegenbaclor
 * Changed AsString() to const
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef MD5HASH_H
#define MD5HASH_H

#include <ptlib.h>
#include <ptclib/cypher.h>

namespace MD5
{
  class MD5Hash : public PObject
  {
    PCLASSINFO( MD5Hash, PObject );
  public:
    MD5Hash();

    MD5Hash( 
      const MD5Hash & hash 
    );

    MD5Hash(
      const PMessageDigest5::Code & digest 
    );

    virtual BOOL Construct();

    PString AsString()const;

    PString AsQuotedString();

    BOOL operator==( const PMessageDigest5::Code & digest );
    BOOL operator==( const MD5Hash & digest );
    BOOL operator==( const PString & digest );

    MD5Hash & operator=( const PMessageDigest5::Code & digest );
    MD5Hash & operator=( const MD5Hash & digest );

    PINLINE const PMessageDigest5::Code & GetDigest()const{ return m_Digest; };

  protected:
    PMessageDigest5::Code m_Digest;
    BOOL m_IsConstructed;
  };
};

#endif

