/*
 *
 * SIPHeaderC.h
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
 * $Log: SIPHeaderC.h,v $
 * Revision 1.6  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPHEADERC_H
#define SIPHEADERC_H

#include "SIPHeader.h"
#include "MimeHeader.h"
#include "StringParam.h"

//implements a comma delimited header

namespace SIPParser
{
  class SIPHeaderC : public SIPHeader
  {
    PCLASSINFO( SIPHeaderC, SIPHeader );
  public:

    SIPHeaderC( 
      const PString & headerName
    );

    SIPHeaderC(
      const PString & headerName,
      const SIPHeaderC & header 
    );

    SIPHeaderC(
      const PString & headerName,
      const PString & header 
    );

    SIPHeaderC(
      const PString & headerName,
      const MimeHeader & header
    );

    SIPHeaderC(
      const PString & headerName,
      const PStringArray & header
    );

    SIPHeaderC & operator=( 
      const SIPHeaderC & header 
    );

    SIPHeaderC & operator=(
      const PString & header 
    );

    SIPHeaderC & operator=(
      const MimeHeader & header
    );

    SIPHeaderC & operator=(
      const PStringArray & header
    );

    virtual void PrintOn( 
      ostream & strm 
    )const;

    virtual PObject * Clone()const;

    void PrepareBody();
  
    PString GetEntry(
      PINDEX index
    );

    void AddEntry(
      const PString & entry
    );

    void PrependEntry(
      const PString & entry
    );

    StringParam PopTopEntry();

    StringParam PopLastEntry();

    PString operator[]( 
      PINDEX index 
    );

    void operator+=(
      const PString & entry
    );

    void RemoveAll();

    PINLINE PINDEX GetSize(){ return m_CollectionEntries.GetSize(); };
    
    PLIST( Collection, SIPHeaderC );

  protected:

    StringParam::Collection m_CollectionEntries;


  };

  class SIPHeaderCList : public PObject
  {
    PCLASSINFO( SIPHeaderCList, PObject );
  public:

    SIPHeaderCList();

    SIPHeaderCList(
      const SIPHeaderCList & list
    );

    SIPHeaderCList(
      const SIPHeaderC::Collection & list
    );
    
    /// appends a new token at the header specified by idx
    BOOL Append( 
      const PString & token,
      PINDEX idx
    );

    /// appends a new header at the end of the list
    BOOL Append(
      const SIPHeaderC & header
    );


    /// returns the header from the address specified by idx
    const SIPHeaderC & GetAt(
      PINDEX idx
    )const;

    /// returns the token at position idx2 from header positions at idx1
    PString GetTokenAt(
      PINDEX idx1,
      PINDEX idx2
    );

    PString operator()(
      PINDEX idx1,
      PINDEX idx2
    );

    virtual  void PrintOn(
      ostream & strm
    )const;

    virtual PObject * Clone()const;


    PString AsString()const;

    SIPHeaderCList & operator=(
      const SIPHeaderCList & list
    );

    SIPHeaderCList & operator=(
      const SIPHeaderC::Collection & list
    );

    SIPHeaderCList & operator+=( 
      const SIPHeaderC & header
    );

    SIPHeaderC & operator[](
      PINDEX idx
    );

    PINLINE SIPHeaderC::Collection & GetCollection(){ return m_Collection; };

    PINLINE BOOL MakeUnique(){ return m_Collection.MakeUnique(); };

    PINLINE PINDEX GetSize()const{ return m_Collection.GetSize(); };

  protected:

    SIPHeaderC::Collection m_Collection;


  };
    

/////////////////////////////////////////////////
  #define DECLARE_COMMA_DELIMITED_HEADER( className )\
    class className : public SIPHeaderC \
    { \
      PCLASSINFO( className, SIPHeaderC ); \
    public:\
      ##className##(); \
      ##className##( const className & header ); \
      ##className##( const PString & header ); \
      ##className##( const MimeHeader & header ); \
      ##className##( const PStringArray & header ); \
    }; \
    class className##List : public SIPHeaderCList \
    { \
    public: \
      ##className##List(); \
      ##className##List( const SIPHeaderCList & list ); \
      ##className##List( const SIPHeaderC::Collection & list ); \
    }

  #define CREATE_COMMA_DELIMITED_HEADER( className, headerName )\
      className##::##className##() : SIPHeaderC( headerName ){} \
      className##::##className##( const className & header ) : SIPHeaderC( headerName, (SIPHeaderC&)header ){} \
      className##::##className##( const PString & header ) : SIPHeaderC( headerName, header ){} \
      className##::##className##( const MimeHeader & header ) : SIPHeaderC( headerName, header ){} \
      className##::##className##( const PStringArray & header ) : SIPHeaderC( headerName, header ){} \
      className##List::##className##List(){} \
      className##List::##className##List(const SIPHeaderCList & list) : SIPHeaderCList( list ){} \
      className##List::##className##List(const SIPHeaderC::Collection & list) : SIPHeaderCList(list){}

  

};

#endif

