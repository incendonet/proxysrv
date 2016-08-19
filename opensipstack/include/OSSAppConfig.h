/*
 *
 * OSSAppConfig.h
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
 * $Log: OSSAppConfig.h,v $
 * Revision 1.7  2007/01/15 09:25:52  joegenbaclor
 * Added RPC related accessors
 *
 * Revision 1.6  2007/01/14 13:13:18  joegenbaclor
 * Fixed compile warnings
 *
 * Revision 1.5  2007/01/12 10:49:07  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.4  2007/01/12 00:31:01  joegenbaclor
 * launcher related code
 *
 * Revision 1.3  2007/01/11 10:01:24  joegenbaclor
 * more launcher code
 *
 * Revision 1.2  2007/01/10 23:57:52  joegenbaclor
 * launcher specific code
 *
 * Revision 1.1  2007/01/05 11:49:31  joegenbaclor
 * Initial upload of OSSAppConfig Files
 *
 *
 */


#ifndef OSSAPPCONFIG_H
#define OSSAPPCONFIG_H

#include <ptlib.h>

#if P_EXPAT

#include <ptclib/pxml.h>

#if P_SSL
#include <ptclib/shttpsvc.h>
#else
#include <ptclib/httpsvc.h>
#endif

#include "Logger.h"
#include "ECCIAPI.h"

namespace Tools
{
  class OSSAppConfig : public XMLRPCTransactionServer
  {
    PCLASSINFO( OSSAppConfig, XMLRPCTransactionServer );
  public:
    enum Type
    { 
      HTTPDefaultProvider,
      XMLRPCClientProvider,
      XMLRPCAndHTTPServerProvider,
      XMLRPCServerProviderOnly
    };

    OSSAppConfig( 
      Type type = HTTPDefaultProvider,
      WORD localPort = 9999,
      const PString & serverURL = PString::Empty(),  /// server address 
      const PString & clientKey = PString::Empty()   /// key/id this client would use to identifiy itself to the server
    );

  protected:
    PLIST( ConfigPages, PConfigPage );
    
    void InitXMLRPC();

    //override from XMLRPCTransactionServer

    void OnIncomingRequest(
      const PString & method,
      PArray<PStringToString>  & request,
      PArray<PStringToString>  & response
    );

    BOOL OnIncomingXMLRPCTransaction(
      const PString & method,
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );



  public:
   
    PString GetListItem(
      const PString & section,
      const PString & itemName,
      PINDEX index
    );

    BOOL SetListItem(
      const PString & section,
      const PString & itemName,
      const PString & value,
      const PINDEX index
    );

    void AppendListItem(
      const PString & section,
      const PString & itemName,
      const PString & value
    );

    PINDEX GetListSize(
      const PString & section,
      const PString & itemName
    );

    PString GetString(
      const PString & section,
      const PString & itemName,
      const PString & dflt
    );

    void SetString(
      const PString & section,
      const PString & itemName,
      const PString & value
    );

    int GetInteger(
      const PString & section,
      const PString & itemName,
      int dflt
    );

    void SetInteger(
      const PString & section,
      const PString & itemName,
      const int value
    );

    BOOL GetBoolean(
      const PString & section,
      const PString & itemName,
      BOOL dflt
    );

    void SetBoolean(
      const PString & section,
      const PString & itemName,
      const BOOL value
    );

    /// XML-RPC Setters and Getters
    virtual void RPCGetListItem(
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );

    virtual void RPCSetListItem(
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );

    virtual void RPCAppendListItem(
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );

    virtual void RPCGetListSize(
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );

    virtual void RPCGetString(
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );

    virtual void RPCSetString(
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );

    virtual void RPCGetInteger(
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );

    virtual void RPCSetInteger(
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );

    virtual void RPCGetBoolean(
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );

    virtual void RPCSetBoolean(
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );

    BOOL RPCSubcribe();

    virtual void OnRPCSubscription(
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );

    BOOL RPCConfigChanged();

    BOOL RPCConfigChanged( 
      const PString & subscriberAddress
    );

    PString GetSubscriberAddress(
      const PString & key 
    )const;

    virtual void OnRPCConfigChanged();

    static OSSAppConfig * GetInstance();

    PINLINE WORD GetHTTPPort()const{ return m_HTTPPort; };
    PINLINE void SetHTTPPort( WORD port ){  m_HTTPPort = port; };
    PINLINE BOOL IsFirstRun()const{ return m_FirstRun; };
    PINLINE PConfig & GetConfig(){ return m_Config; };
    PINLINE Type GetType()const{ return m_Type; };
    PINLINE const PString & GetXMLRPCServerURL()const{ return m_ServerURL; }; 
    PINLINE const PString & GetXMLRPCClientKey()const{ return m_ClientKey; };

  protected:

    class StatusPage : public PServiceHTTPString
    {
      PCLASSINFO(StatusPage, PServiceHTTPString);

      public:
        StatusPage(
          PHTTPAuthority & auth,
          PXMLElement * config
        );
        
        virtual BOOL Post(
          PHTTPRequest & request,
          const PStringToString &,
          PHTML & msg
        );
      
      private:
        void Prepare();
        PXMLElement * m_Config;
    };

    PLIST( StatusPages, StatusPage );

    BOOL Initialize( 
      const PFilePath & xml 
    );

    BOOL IntializeConfigPages(
      PXML & schema,
      ConfigPages & configPages,
      PHTTPSimpleAuth & auth
    );

    BOOL IntializeStatusPages(
      PXML & schema,
      StatusPages & statusPages,
      PHTTPSimpleAuth & auth
    );


    BOOL CreateConfig(
      PXML & schema,
      ConfigPages & configPages
    );

    BOOL CreatePage(
      PXMLElement & pageSchema,
      PConfigPage & page
    );
    
    WORD m_HTTPPort;
    BOOL m_FirstRun;
    PMutex m_InitializeMutex;
    PConfig m_Config;
    Type m_Type;
    PString m_ServerURL;
    PString m_ClientKey;
    PDICTIONARY( Subscriptions, PCaselessString, PString );
    Subscriptions m_Subscriptions;
    PMutex m_SubscriptionsMutex;

    friend class OSSApplication;
  };
};

#endif // P_EXPAT

#endif



