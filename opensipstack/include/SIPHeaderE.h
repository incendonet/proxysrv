/*
 *
 * SIPHeaderE.h
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
 * $Log: SIPHeaderE.h,v $
 * Revision 1.6  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/10/23 15:44:54  joegenbaclor
 * 1.  Numerous bug fixes for REFER
 * 2.  Added new SIP Headers for SIP timers and Attended call transfer
 * 3.  Added loop sanity check to SIP UDP Transport
 * 4.  Corrected parser bug for ProxyRequire header
 *
 * Revision 1.4  2006/10/12 08:41:22  joegenbaclor
 * Added support for RFC 4028 headers
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPHEADERE_H
#define SIPHEADERE_H

#include "SIPHeader.h"
#include "MimeParam.h"

namespace SIPParser
{
  class SIPHeaderE : public SIPHeader
  {
    PCLASSINFO( SIPHeaderE, SIPHeader );
  public:
    SIPHeaderE(
      const PString & name
    );

    SIPHeaderE(
      const PString & name,
      PInt64 value,
      PInt64 minValue,
      PInt64 maxValue,
      PInt64 defaultValue
    );

    SIPHeaderE(
      const PString & name,
      const PString & header,
      PInt64 minValue,
      PInt64 maxValue,
      PInt64 defaultValue
    );

    SIPHeaderE(
      const MimeHeader & header,
      PInt64 minValue,
      PInt64 maxValue,
      PInt64 defaultValue
    );

    virtual PObject * Clone()const;

    SIPHeaderE & operator=( 
      PInt64 & val 
    );

    SIPHeaderE & operator--();

    SIPHeaderE & operator++();

    BOOL operator<( PInt64 value );

    BOOL operator>( PInt64 value );

    PInt64 AsInt64()const;

    void SetValue( PInt64 value );

    PString GetParameter( 
      const PString & name
    );

    void SetParameter(
      const PString & name,
      const PString & value
    );


  protected:

    PInt64 m_MaxValue;
    PInt64 m_MinValue;
    PInt64 m_DefaultValue;


  };

  #define DECLARE_INTEGER_HEADER( className ) \
  class className : public SIPHeaderE { \
    PCLASSINFO( className, SIPHeaderE ); \
  public: \
    ##className##( PInt64 value ); \
    ##className##( const PString & header ); \
    ##className##( const MimeHeader & header ); \
  }

  #define CREATE_INTEGER_HEADER( className, headerName, minValue, maxValue, defaultValue ) \
    className##::##className##( PInt64 value ) : SIPHeaderE( headerName, value, minValue, maxValue, defaultValue ) {}; \
    className##::##className##( const PString & header ) : SIPHeaderE( headerName, header, minValue, maxValue, defaultValue ){}; \
    className##::##className##( const MimeHeader & header ) : SIPHeaderE( header, minValue, maxValue, defaultValue ){}; \


};


#endif
