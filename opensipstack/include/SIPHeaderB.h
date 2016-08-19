/*
 *
 * SIPHeaderB.h
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
 * $Log: SIPHeaderB.h,v $
 * Revision 1.7  2007/01/14 13:13:18  joegenbaclor
 * Fixed compile warnings
 *
 * Revision 1.6  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.4  2006/03/21 14:11:50  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPHEADERB_H
#define SIPHEADERB_H

#include "SIPHeader.h"
#include "SIPURI.h"
#include "TelURI.h"

namespace SIPParser
{
  class SIPHeaderB : public SIPHeader
  {
    PCLASSINFO( SIPHeaderB, SIPHeader );
  public:

    SIPHeaderB(
      const PString & headerName 
    );

    SIPHeaderB(
      const PString & headerName,
      const SIPHeaderB & header 
    );

    SIPHeaderB(
      const PString & headerName,
      const PString & header 
    );

    SIPHeaderB(
      const PString & headerName,
      const MimeHeader & header
    );

    SIPHeaderB(
      const PString & headerName,
      const SIPURI & uri,
      const PString & displayName = PString::Empty(),
      const PString & tag = PString::Empty()
    );

    SIPHeaderB & operator=(
      const SIPHeaderB & header 
    );

    SIPHeaderB & operator=(
      const PString & header 
    );

    SIPHeaderB & operator=(
      const MimeHeader & header
    );

    virtual void PrintOn(
      ostream & strm
    )const;

    virtual PObject * Clone()const;

    virtual void PrepareBody();

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
    );

    BOOL GetParameter(
      const PString & name,
      PString & paramValue
    )const;

    MimeParam & GetParameter(
      PINDEX index
    )const;

    PINLINE const MimeParam::SortedCollection & GetParameters()const{ return m_Parameters; };


    PINLINE SIPURI & GetURI(){ return m_URI; };
    PINLINE const PString & GetDisplayName(){ m_HasURIEnclosure = TRUE; return m_DisplayName; };

    PINLINE void SetURI( const SIPURI & uri ){ m_URI = uri; };
    PINLINE void SetDisplayName( const PString & displayName ){ m_DisplayName = displayName; };

  protected:

    MimeParam::SortedCollection m_Parameters;
    SIPURI m_URI;
    PString m_DisplayName;
    BOOL m_HasURIEnclosure;
  };


#define DECLARE_URI_HEADER( className )\
  class className : public SIPHeaderB \
  { \
  PCLASSINFO( className, SIPHeaderB ); \
  public: \
    ##className##(); \
    ##className##(const className & header); \
    ##className##(const PString & header); \
    ##className##(const MimeHeader & header); \
    ##className##(const SIPURI & uri,const PString & displayName = PString::Empty(), \
      const PString & tag = PString::Empty()); \
  };

#define CREATE_URI_HEADER( className, headerName )\
  className##::##className##() : SIPHeaderB( headerName ){}; \
  className##::##className##(const className & header) : SIPHeaderB( headerName, header ){}; \
  className##::##className##(const PString & header) : SIPHeaderB( headerName, header ){}; \
  className##::##className##(const MimeHeader & header) : SIPHeaderB( headerName, header ){}; \
  className##::##className##(const SIPURI & uri,const PString & displayName, const PString & tag ) \
      : SIPHeaderB( headerName, uri, displayName, tag ){}; \

};




#endif


