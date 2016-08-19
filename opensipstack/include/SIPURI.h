/*
 *
 * SIPURI.h
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
 * $Log: SIPURI.h,v $
 * Revision 1.10  2006/12/19 23:38:31  joegenbaclor
 * Fixed bug in softphone proxy authentication where passwords <= 3 in length may not be properly handled
 *
 * Revision 1.9  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.8  2006/11/30 00:51:14  joegenbaclor
 * 1.  Corrected bug in authorization construction where the URI can have a double port concatenation.
 * 2.  Modified AsString() method of SIPURI to allow not returning the URI parameters.
 * 3.  Added GetBasicURI method
 *
 * Revision 1.7  2006/07/07 10:05:13  joegenbaclor
 * Added functionality to autogenerate A1 Hash from SIPURI's
 *
 * Revision 1.6  2006/07/07 09:53:25  joegenbaclor
 * Added loose comparisong operator to SIPURI
 *
 * Revision 1.5  2006/06/28 14:14:57  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.4  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.3  2006/03/14 03:13:22  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPURI_H
#define SIPURI_H


#include "MimeParam.h"
#include "ptlib/sockets.h"
#include "A1Hash.h"

namespace SIPParser
{
  class SIPURI : public PObject
  {
    PCLASSINFO( SIPURI, PObject );
  public:

    SIPURI();

    SIPURI( 
      const SIPURI & uri
    );

    SIPURI(
      const PString & uri 
    );

    SIPURI(
      const MimeHeader & header 
    );

    ////sip:user:password@host:port;uri-parameters?headers

    SIPURI & operator=( 
      const SIPURI & uri
    );

    SIPURI & operator=(
      const PString & uri 
    );

    SIPURI & operator=(
      const MimeHeader & header 
    );

    BOOL operator*=(
      const SIPURI & uri
    );

    void PrintOn( 
      ostream & strm 
    )const;

    PString AsString( BOOL includeURIParams = TRUE, BOOL includeUser = TRUE, BOOL includePort = TRUE )const;

    SIPURI GetBasicURI()const;

    PString AsA1Hash()const;

    void AddParameter(
      const PString & name,
      const PString & value,
      BOOL replaceIfExist = FALSE
    );

    void SetParameter(
      const PString & name,
      const PString & value
    );

    PINDEX FindParameter(
      const PString & paramName
    )const;

    BOOL GetParameter(
      const PString & name,
      PString & paramValue
    )const;

    PString GetParameter(
      const PString & name
    )const;

    MimeParam & GetParameter(
      PINDEX index
    )const;

    //--

    void AddHeader(
      const PString & name,
      const PString & value,
      BOOL replaceIfExist = FALSE
    );

    void SetHeader(
      const PString & name,
      const PString & value
    );

    PINDEX FindHeader(
      const PString & paramName
    )const;

    BOOL GetHeader(
      const PString & name,
      PString & value
    )const;

    MimeParam & GetHeader(
      PINDEX index
    )const;

    BOOL IsPhoneURI()const;

    BOOL IsPrivateNetwork()const;

    PIPSocket::Address GetAddress()const;

    PINLINE const PString & GetScheme()const{ return  m_Scheme; };
    PINLINE const PString & GetUser()const{ return m_User; };
    PINLINE const PString & GetHost()const{ return m_Host; };
    PINLINE const PString & GetPassword()const{ return m_Password; };
    PINLINE const PString & GetPort()const{ return m_Port; };
    PINLINE const MimeParam::SortedCollection & GetParameters()const{ return m_Parameters; };
    PINLINE const MimeParam::Collection & GetHeaders()const{ return m_Headers; };

    PINLINE void SetScheme( const PString & scheme ){  m_Scheme = scheme; };
    PINLINE void SetUser( const PString & user ){ m_User = user; };
    PINLINE void SetHost( const PString & host ){ m_Host = host; };
    PINLINE void SetPassword( const PString & password ){ m_Password = password; };
    PINLINE void SetPort( const PString & port ){ m_Port = port; };
    PINLINE void SetParameters( const MimeParam::SortedCollection & params ){ m_Parameters = params; };
    PINLINE void SetHeaders( const MimeParam::Collection & headers ){ m_Headers = headers; };

  protected:

    PString m_Scheme;
    PString m_User;
    PString m_Password;
    PString m_Host;
    PString m_Port;
    MimeParam::SortedCollection m_Parameters;
    MimeParam::Collection m_Headers;

  };
};

#endif




