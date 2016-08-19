/*
 *
 * SDPOrigin.h
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
 * $Log: SDPOrigin.h,v $
 * Revision 1.5  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SDPORIGIN_H
#define SDPORIGIN_H

#include "SDPHeader.h"

namespace SDP
{
  class SDPOrigin : public SDPHeader
  {
    PCLASSINFO( SDPOrigin, SDPHeader );
  public:

    SDPOrigin();

    SDPOrigin( 
      const SDPHeader & header
    );

    SDPOrigin(
      const PString & header
    );

    SDPOrigin(
      const PString & name,
      const PString & value
    );

    SDPOrigin(
      const SDPOrigin & header
    );

    SDPOrigin(
      const PString & userName,
      const PString & sessionId,
      const PString & version,
      const PString & networkType,
      const PString & addressType,
      const PString & address
    );

    SDPOrigin & operator=( const PString & header );

    SDPOrigin & operator=( const SDPOrigin & header );

    virtual void PrintOn( 
      ostream & strm 
    )const;

    void SetUserName(
      const PString & value
    );

    void SetSessionId(
      const PString & value
    );

    void SetVersion(
      const PString & value
    );

    void SetNetworkType(
      const PString & value
    );

    void SetAddressType(
      const PString & value
    );

    void SetAddress(
      const PString & value
    );

    virtual PObject * Clone()const;
    
    PINLINE const PString & GetUserName()const{ return m_UserName; };
    PINLINE const PString & GetSessionId()const{ return m_SessionId; };
    PINLINE const PString & GetVersion()const{ return m_Version; };
    PINLINE const PString & GetNetworkType()const{ return m_NetworkType; };
    PINLINE const PString & GetAddressType()const{ return m_AddressType; };
    PINLINE const PString & GetAddress()const{ return m_Address; };

  protected:

    PString m_UserName;
    PString m_SessionId;
    PString m_Version;
    PString m_NetworkType;
    PString m_AddressType;
    PString m_Address;

  };
};

#endif

