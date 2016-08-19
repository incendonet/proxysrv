/*
 *
 * SIPHeaderF.h
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
 * $Log: SIPHeaderF.h,v $
 * Revision 1.6  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPHeaderF_H
#define SIPHeaderF_H

#include "SIPHeaderC.h"
#include "MimeParam.h"
#include "RouteURI.h"


namespace SIPParser
{
  class SIPHeaderF : public SIPHeaderC
  {
    PCLASSINFO( SIPHeaderF, SIPHeaderC );
  public:
      
    SIPHeaderF( 
      const PString & headerName
    );

    SIPHeaderF(
      const PString & headerName,
      const PString & header 
    );

    SIPHeaderF( 
      const PString & headerName,
      const PStringArray & header 
    );

    SIPHeaderF( 
      const PString & headerName,
      const SIPHeaderF & header 
    );
      
    SIPHeaderF(
      const PString & headerName,
      const MimeHeader & header 
    );
      
    

    SIPHeaderF(
      const PString & headerName,
      const RouteURI::Collection & uri
    );

    SIPHeaderF & operator=(
      const RouteURI::Collection & uri
    );

    RouteURI PopTopURI();

    RouteURI PopLastURI();

    BOOL GetURI(
      RouteURI & uri,
      PINDEX uriIndex = 0
    );

    BOOL AddURI(
      const RouteURI & uri
    );

    virtual PObject * Clone()const;

    PLIST( Collection, SIPHeaderF );
   
  };

  //////////////////////////////////////////////////////////

  class SIPHeaderFList : public PObject
  {
    PCLASSINFO( SIPHeaderFList, PObject );
  public:

    SIPHeaderFList();
    
    SIPHeaderFList(
      const SIPHeaderFList & list
    );

    SIPHeaderFList(
      const SIPHeaderF::Collection & list
    );

    BOOL Append( 
      const RouteURI & uri,
      PINDEX idx = 0
    );

    BOOL Append(
      const SIPHeaderF & header
    );

    SIPHeaderF & GetAt(
      PINDEX idx
    )const;

    RouteURI GetAt(
      PINDEX idx1,
      PINDEX idx2
    )const;

    RouteURI PopTopURI();

    RouteURI PopLastURI();

    PINLINE PINDEX GetSize()const{ return m_Collection.GetSize(); };

    PINLINE BOOL MakeUnique(){ return m_Collection.MakeUnique(); };

    virtual void PrintOn(
      ostream & strm
    )const;

    virtual PObject * Clone()const;

    PString AsString()const;

    SIPHeaderFList & operator=(
      const SIPHeaderFList & list
    );

    SIPHeaderFList & operator=(
      const SIPHeaderF::Collection & list
    );

    SIPHeaderFList & operator+=( 
      const SIPHeaderF & header
    );

    SIPHeaderF & operator[](
      PINDEX idx
    );

  protected:

    SIPHeaderF::Collection m_Collection;

  };

  #define DECLARE_ROUTE_HEADER( className ) \
  class className : public SIPHeaderF \
  { \
  PCLASSINFO( className, SIPHeaderF ); \
  public: \
    ##className##(); \
    ##className##(const SIPHeaderF & header); \
    ##className##(const PString & header); \
    ##className##(const MimeHeader & header); \
    ##className##(const PStringArray & header); \
    ##className##(const RouteURI::Collection & uri); \
  }; \
  class className##List : public SIPHeaderFList \
  { \
  PCLASSINFO( className##List, SIPHeaderFList ); \
  public: \
    className##List(); \
    className##List(const SIPHeaderFList & list); \
    className##List(const SIPHeaderF::Collection & list); \
  };

  #define CREATE_ROUTE_HEADER( className, headerName ) \
    className##::##className##() : SIPHeaderF(headerName){}; \
    className##::##className##(const SIPHeaderF & header) : SIPHeaderF(headerName, header){}; \
    className##::##className##(const PString & header) : SIPHeaderF(headerName, header){}; \
    className##::##className##(const MimeHeader & header) : SIPHeaderF(headerName, header){}; \
    className##::##className##(const PStringArray & header) : SIPHeaderF(headerName, header){}; \
    className##::##className##(const RouteURI::Collection & uri) : SIPHeaderF(headerName, uri){}; \
    className##List::className##List(){} \
    className##List::className##List(const SIPHeaderFList & list) : SIPHeaderFList( list ){}; \
    className##List::className##List(const SIPHeaderF::Collection & list) : SIPHeaderFList( list ){};



};


#endif




