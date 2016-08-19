/*
 *
 * MimeParam.h
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
 * $Log: MimeParam.h,v $
 * Revision 1.4  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef MIMEPARAM_H
#define MIMEPARAM_H

#include <ptlib.h>
#include "MimeHeader.h"

namespace SIPParser
{
  class MimeParam : public PObject
  {
    PCLASSINFO( MimeParam, PObject );
  public:

    MimeParam();

    MimeParam(
      const MimeParam & param
    );

    MimeParam(
      const PString & param 
    );

    MimeParam(
      const PString & paramName,
      const PString & paramValue
    );

    MimeParam & operator=( 
      const MimeParam & param
    );

    MimeParam & operator=(
      const PString & param
    );

    PLIST( Collection, MimeParam );
    PDICTIONARY( SortedCollection, PCaselessString, MimeParam );

    static void CreateCollection(
      const PString & mime,
      Collection & params,
      const char & delimiter = ';'
    );

    static void CreateCollection(
      const MimeHeader & mime,
      Collection & params,
      const char & delimiter = ';'
    );

    static void CreateCollection(
      const PString & mime,
      SortedCollection & params,
      const char & delimiter = ';'
    );

    static void CreateCollection(
      const MimeHeader & mime,
      SortedCollection & params,
      const char & delimiter = ';'
    );

    virtual void PrintOn( 
      ostream & strm 
    )const;

    virtual PObject * Clone()const;

    PString AsString()const;

    PINLINE void SetParamName( const PString & paramName ){ m_Name = paramName; };

    PINLINE void SetParamValue( const PString & paramValue ){ m_Value = paramValue; };

    PINLINE const PString & GetParamName()const{ return m_Name; };

    PINLINE const PString & GetParamValue()const{ return m_Value; };

    PINLINE void SetStartInNewLine( BOOL startInNewLine = TRUE ){ m_StartsInNewLine = startInNewLine; };

    PINLINE BOOL IsStartInNewLine(){ return m_StartsInNewLine; };

    PINLINE void SetEndWithNewLine( BOOL endsWithNewLine = TRUE ){ m_EndsWithNewLine = endsWithNewLine; };

    PINLINE BOOL IsEndWithNewLine(){ return m_EndsWithNewLine; };
  protected:
    
    PString m_Name;
    PString m_Value;
    BOOL m_StartsInNewLine;
    BOOL m_EndsWithNewLine;
  };
};

#endif

