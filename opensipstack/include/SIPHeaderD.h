/*
 *
 * SIPHeaderD.h
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
 * $Log: SIPHeaderD.h,v $
 * Revision 1.4  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPHEADERD_H
#define SIPHEADERD_H

#include "SIPHeader.h"
#include "MimeParam.h"


/// Implements a comma delimited header with name=value parameters

namespace SIPParser
{
  class SIPHeaderD : public SIPHeader
  {
    PCLASSINFO( SIPHeaderD, SIPHeader );
  public:

    SIPHeaderD(
      const PString & headerName 
    );

    SIPHeaderD(
      const PString & headerName,
      const SIPHeaderD & header 
    );

    SIPHeaderD(
      const PString & headerName,
      const PString & header 
    );

    SIPHeaderD(
      const PString & headerName,
      const MimeHeader & header
    );

    SIPHeaderD & operator=(
      const SIPHeaderD & header 
    );

    SIPHeaderD & operator=(
      const PString & header 
    );

    SIPHeaderD & operator=(
      const MimeHeader & header
    );

    virtual void PrintOn(
      ostream & strm
    )const;

    virtual PObject * Clone()const;

    virtual void PrepareBody();

    void AddParameter(
      const PString & name,
      const PString & value
    );

    void SetParameter(
      const PString & name,
      const PString & value,
      PINDEX offSet = 0
    );

    PINDEX FindParameter(
      const PString & paramName,
      PINDEX offSet = 0
    );

    BOOL GetParameter(
      const PString & name,
      PString & paramValue,
      PINDEX offSet = 0
    );

    MimeParam & GetParameter(
      PINDEX index
    );

    PINLINE MimeParam::Collection & GetParameters(){ return m_Parameters; };

    PINLINE const PString & GetLeadString()const{ return m_LeadString; };

    PINLINE void SetLeadString( const PString & leadString ){ m_LeadString = leadString; };

  protected:

    MimeParam::Collection m_Parameters;
    PString m_LeadString;

  };


#define DECLARE_COMMA_DELIMITED_PARAM_HEADER( className )\
  class className : public SIPHeaderD \
  { \
  PCLASSINFO( className, SIPHeaderD ); \
  public: \
    ##className##(); \
    ##className##(const className & header); \
    ##className##(const PString & header); \
    ##className##(const MimeHeader & header); \
  };

#define CREATE_COMMA_DELIMITED_PARAM_HEADER( className, headerName )\
    className##::##className##() : SIPHeaderD( headerName ){}; \
    className##::##className##(const className & header) : SIPHeaderD( headerName, header ){}; \
    className##::##className##(const PString & header) : SIPHeaderD( headerName, header ){}; \
    className##::##className##(const MimeHeader & header) : SIPHeaderD( headerName, header ){}; \


  

};

#endif


