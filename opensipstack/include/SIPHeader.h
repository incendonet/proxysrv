/*
 *
 * SIPHeader.h
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
 * $Log: SIPHeader.h,v $
 * Revision 1.5  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/08/14 01:29:06  joegenbaclor
 * Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPHeader_H
#define SIPHeader_H

#include <ptlib.h>

#include "MimeHeader.h"
#include "ParserTools.h"

namespace SIPParser
{
  class SIPHeader : public PObject
  {
    PCLASSINFO( SIPHeader, PObject );
  public:

    SIPHeader(
      const PString & headerName,
      const PString & headerBody = PString::Empty()
    );

    SIPHeader(
      const MimeHeader & value 
    );

    SIPHeader(
      const SIPHeader & header
    );

    SIPHeader & operator=( const PString & value );
    
    SIPHeader & operator=( const MimeHeader & value );

    SIPHeader & operator=( const SIPHeader & header );

    operator PString();

    PString AsString()const;

    virtual void PrintOn(
      ostream & strm 
    )const;

    virtual PObject * Clone()const;

    virtual void PrepareBody();

    const PString & GetHeaderBody()const;

    PINLINE const PString & GetHeaderName()const{ return m_HeaderName; };

    PINLINE void SetHeaderBody( const PString & body ){ m_HeaderBody = body; };

  protected:

    PString m_HeaderName;
    PString m_HeaderBody;

  };

#define DECLARE_STANDARD_SIP_HEADER( className )\
  class className : public SIPHeader \
  { \
    PCLASSINFO( className, SIPHeader ); \
  public:\
    ##className##( const PString & body ); \
    ##className##( const MimeHeader & value ); \
    ##className##( const SIPHeader & header ); \
  };

#define CREATE_STANDARD_SIP_HEADER( className, headerName, headerBody )\
  className##::##className##( const PString & body ) : SIPHeader( headerName ) { m_HeaderBody = body; } \
  className##::##className##( const MimeHeader & value ) : SIPHeader( value ){} \
  className##::##className##( const SIPHeader & header ) : SIPHeader( header ){} \



};

#endif


