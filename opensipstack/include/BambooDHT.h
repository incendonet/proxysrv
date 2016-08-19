/*
 *
 * BambooDHT.h
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
 * $Log: BambooDHT.h,v $
 * Revision 1.2  2006/11/30 08:19:35  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/08/01 03:55:41  joegenbaclor
 * 1.  Completed Initial classes for IVR Prompt channel
 * 2.  Added support for Bamboo Dynamic Hash Table for Network Overlay support
 *
 *
 */

#ifndef BAMBOODHT_H
#define BAMBOODHT_H

#include <ptlib.h>
#include <ptclib/pxmlrpc.h>
#include "ossbuildopts.h"

#if ENABLE_BAMBOO
namespace DHT
{
  class BambooDHT : public PObject 
  {
    PCLASSINFO( BambooDHT, PObject );
  public:

    enum PutResult
    {
      Success = 0,
      OverCapacity = 1,
      TryAgain = 2 
    };

    BambooDHT( 
      const PString & appName,
      const PString & key,
      const PBYTEArray & value,
      const PTimeInterval & ttl,
      const PString & secret,
      const PURL & bambooServer = "http://opendht.nyuld.net:5851"
    );

    ~BambooDHT();


    BambooDHT::PutResult Put();
    
    BambooDHT::PutResult PutRemovable();
    
    BOOL Get(
      PBYTEArray & value,
      PBYTEArray & newIter,
      const PBYTEArray & iter = PBYTEArray()
    );

    static PString GetNearestServer();

    BOOL Remove();

    PINLINE const PString & GetKey()const{ return m_Key; };
    PINLINE void SetKey( const PString & key ){ m_Key = key; };
    PINLINE const PBYTEArray & GetValue()const{ return m_Value; };
    PINLINE void SetValue( const PBYTEArray & value ){ m_Value = value; };
    PINLINE const PString & GetApplication()const{ return m_Application; };
    PINLINE void SetApplication( const PString & app ){ m_Application = app; };
    PINLINE const PString & GetSecret()const{ return m_Secret; };
    PINLINE void SetSecret( const PString & secret ){ m_Secret = secret; };
    PINLINE const PTimeInterval & GetTTL()const{ return m_TTL; };
    PINLINE void SetTTL( const PTimeInterval & ttl ){ m_TTL = ttl; };
    PINLINE const PURL & GetServer()const{ return m_BambooServer; };
    PINLINE void SetServer( const PURL & url ){ m_BambooServer = url; };

  protected:
    PString m_Key;
    PBYTEArray m_Value;
    PString m_Application;
    PString m_Secret;
    PTimeInterval m_TTL;
    PURL m_BambooServer;
    PMutex m_Lock;
  };
};

#endif  /// ENABLE_BAMBOO
#endif  /// BAMBOODHT_H

