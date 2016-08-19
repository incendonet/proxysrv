/*
 *
 * MimeHeader.h
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
 * $Log: MimeHeader.h,v $
 * Revision 1.5  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/06/07 09:18:33  joegenbaclor
 * Added the ability to SIPMessage to read from a PChannel
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef MimeHeader_H
#define MimeHeader_H

#include <ptlib.h>

namespace SIPParser
{
  class MimeHeader : public PObject
  {
    PCLASSINFO( MimeHeader, PObject );
  public:
    
    MimeHeader();

    MimeHeader( 
      const MimeHeader & header
    );

    MimeHeader(
      const PString & MimeHeader
    );

    MimeHeader(
      const PString & name,
      const PString & value
    );

    virtual MimeHeader & operator=( 
      const MimeHeader & header 
    );

    virtual MimeHeader & operator=( 
      const PString & header 
    );

    operator PString();

    virtual void PrintOn( 
      ostream & strm 
    )const;

    virtual PObject * Clone()const;

    PString AsString()const;

    PINLINE void SetMimeName( const PString & name ){ m_Name = name; };

    PINLINE void SetMimeValue( const PString & value ){ m_Value = value; };

    PINLINE const PString & GetMimeName()const{ return m_Name; };

    PINLINE const PString & GetMimeValue()const{ return m_Value; };

    PLIST( Collection, MimeHeader );
  protected:

    PString m_Name;
    PString m_Value;
  };

  class MimeHeaderList : public PObject
  {
    PCLASSINFO( MimeHeaderList, PObject );

  public:

    MimeHeaderList();
    
    MimeHeaderList( 
      const MimeHeaderList & list 
    );
    
    MimeHeaderList( 
      istream &strm
    );

    virtual void ReadFrom(
      istream &strm   ///< Stream to read the objects contents from.
    );

    virtual void PrintOn(
      ostream &strm
    )const;

    MimeHeaderList & operator=( 
      const MimeHeaderList & list 
    );

    void AddHeader(
      const PString & header
    );

    void AddHeader(
      const PString & key,
      const PString & value
    );

    void AddHeader(
      const MimeHeader & header
    );

    PString GetString(
      const PString & key,
      const PString & dflt = PString::Empty(),
      PINDEX i = 0
    );

    int GetInteger(
      const PString &key,
      int dflt = 0,
      PINDEX i = 0
    );

   
    const MimeHeader * GetHeaderAt(
      const PString &key,
      PINDEX i = 0
    )const;

    const MimeHeader * GetHeaderAt(
      PINDEX i
    )const;

    PINLINE const MimeHeader::Collection & GetList()const{ return m_MimeHeaders; };
    PINLINE BOOL Contains( const PString & key )const{ return GetHeaderAt( key ) != NULL; };

  protected:

    MimeHeader::Collection m_MimeHeaders;

  };
};

#endif

