/*
 *
 * RequestLine.h
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
 * $Log: RequestLine.h,v $
 * Revision 1.5  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef REQUESTLINE_H
#define REQUESTLINE_H

#include "SIPURI.h"

namespace SIPParser
{
  class RequestLine : public PObject
  {
    PCLASSINFO( RequestLine, PObject );
  public:
    RequestLine();

    RequestLine(
      const PString & method,
      const SIPURI & requestURI,
      const PString & version = "SIP/2.0"
    );
    
    RequestLine(
      const RequestLine & requestLine
    );

    RequestLine(
      const PString & requestLine
    );

    RequestLine & operator=( 
      const RequestLine & requestLine
    );

    RequestLine & operator=( 
      const PString & requestLine
    );

    operator PString();

    virtual void PrintOn(
      ostream & strm
    )const;

    virtual PObject * Clone()const;

    PString AsString()const;

    PINLINE void SetMethod( const PString & method ){ m_Method = method; };
    PINLINE const PString & GetMethod()const{ return m_Method; };
    PINLINE void SetRequestURI( const SIPURI & uri ){ m_RequestURI = uri; };
    PINLINE void SetRequestURI( const PString & uri ){ m_RequestURI = SIPURI( uri ); };
    PINLINE SIPURI & GetRequestURI(){ return m_RequestURI; };
    PINLINE void SetVersion( const PString & version ){ m_SIPVersion = version; };
    PINLINE const PString & GetVersion()const{ return m_SIPVersion; };

  protected:
    /// Request-Line  =  Method SP Request-URI SP SIP-Version CRLF

    PString m_Method;
    SIPURI m_RequestURI;
    PString m_SIPVersion;

  };
};

#endif


