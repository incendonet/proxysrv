/*
 *
 * ECCIAPI.h
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
 * $Log: ECCIAPI.h,v $
 * Revision 1.11  2007/01/04 09:36:00  joegenbaclor
 * Added XML RPC Configuration Capability
 *
 * Revision 1.10  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.9  2006/07/26 00:44:52  joegenbaclor
 * Added Voice File classes for upcoming IVR support
 *
 * Revision 1.8  2006/06/08 04:54:20  joegenbaclor
 * !.  Added OnRegistrationTimeout Call back to Registrar classes
 * 2. Moved ECCIAPI to Tools namespace
 *
 * Revision 1.7  2006/06/08 01:32:59  joegenbaclor
 * Modified MimeHeaderList::AddHeader() to filter out empty strings
 *
 * Revision 1.6  2006/06/06 14:21:07  joegenbaclor
 * Corrected #endif with trailing comments and changed #ifdef P_EXPAT to #if P_EXPAT
 *
 * Revision 1.5  2006/06/06 13:30:25  joegenbaclor
 * Initial upload of PIDF files - thanks Solegy Systems!
 *
 * Revision 1.4  2006/05/31 07:21:51  joegenbaclor
 * 1.  Added OSSApplication and StringTuple classes.
 * 2.  Corrected some typos
 *
 * Revision 1.3  2006/03/20 03:51:12  joegenbaclor
 * This upload marks the first successful OPAL call using opensipstack!!!   There is still some pending issues
 * in 183 Progress handling in OPAL classes but a fix should with the next revision.
 *
 * Revision 1.2  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */


#ifndef CCIAPI_H
#define CCIAPI_H

#include <ptlib.h>
#include "ossbuildopts.h"


#if ENABLE_ECCI
#if P_EXPAT

#include <ptlib/sockets.h>
#include <ptclib/httpform.h>
#include <ptclib/cypher.h>
#include <ptclib/pxmlrpc.h>
#include <ptclib/pxmlrpcs.h>
#include <ptclib/random.h>

namespace Tools
{
  class  RouteTable : public PObject
  {
    PCLASSINFO( RouteTable, PObject );
	  typedef PIPSocket::Address Address;
	  typedef PIPSocket::InterfaceTable InterfaceTable;

  public:
	  RouteTable() { InitTable(); }
	  ~RouteTable() { ClearTable(); }
	  const Address & GetLocalAddress() const { return defAddr; };
	  Address GetLocalAddress( const Address & ) const;
    BOOL IsLocalAddress( const PString & address );
  	
    PINLINE const Address &GetDefaultAddress()const{ return defAddr; };
    PINLINE BOOL IsNetworkBridge(){ return m_IsNetworkBridge; };

  private:

    void InitTable();
	  void ClearTable();

	  class vxRouteEntry : public PIPSocket::RouteEntry 
    {
	    PCLASSINFO( vxRouteEntry, PObject )
		  public:
      vxRouteEntry(const PString &);
		  vxRouteEntry(const PIPSocket::RouteEntry &, const InterfaceTable &);
		  bool Compare(const Address *) const;
	  };

	  void CreateTable();
	  void CreateTable(const PString &);
    PLIST( ROUTE_ENTRY_LIST, vxRouteEntry );
    ROUTE_ENTRY_LIST m_RouteTable;
	  Address defAddr;
    BOOL m_IsNetworkBridge;
  };

  /////////////////////////////////////////////////////////////////////

  class HTTPServer;

  class HTTPServerThread : public PThread
  {
    PCLASSINFO(HTTPServerThread, PThread)

    public:

      HTTPServerThread(
        PINDEX stackSize,
        HTTPServer & app
      );

      ~HTTPServerThread();

      void Main();
      
      void Close();

    protected:
      
      PINDEX                ECCIStackSize;
      HTTPServer & process;
      PTCPSocket          * socket;
  };

  ////////////////////////////////////////////////////////

  class HTTPServer : public RouteTable
  {
    PCLASSINFO( HTTPServer, RouteTable );
  public:

    public:
      enum {
        MaxSecuredKeys = 10
      };

      HTTPServer();

      virtual ~HTTPServer();

      BOOL OnStart();
      void OnStop();

      virtual BOOL Initialize() = 0;

      BOOL ListenForHTTP(
        WORD port,
        PSocket::Reusability reuse = PSocket::CanReuseAddress,
        PINDEX stackSize = 0x4000
      );

      BOOL ListenForHTTP(
        PSocket * listener,
        PSocket::Reusability reuse = PSocket::CanReuseAddress,
        PINDEX stackSize = 0x4000
      );

      BOOL GetListenerAddress(
        PIPSocket::Address & address,
        WORD & port
      );
      
      PINLINE const PTEACypher::Key & GetProductKey() const { return m_ProductKey; }
      PINLINE void SetProductKey( const PTEACypher::Key & key ){ m_ProductKey = key; };

      PINLINE const PStringArray & GetSecuredKeys() const { return m_SecuredKeys; }
      PINLINE void SetSecuredKeys( const PStringArray & keys ){ m_SecuredKeys = keys; };
      
      PINLINE const PTEACypher::Key & GetSignatureKey() const { return m_SignatureKey; }
      PINLINE void SetSignatureKey( const PTEACypher::Key & key ){ m_SignatureKey = key; };

      PINLINE BOOL ShouldIgnoreSignatures() const { return m_IgnoreSignatures; }
      PINLINE void SetIgnoreSignatures(BOOL ig) { m_IgnoreSignatures = ig; }

      virtual PHTTPServer * CreateHTTPServer(
        PTCPSocket & socket
      );

      virtual PHTTPServer * OnCreateHTTPServer(
        const PHTTPSpace & urlSpace
      );
      
      PTCPSocket * AcceptHTTP();
      
      BOOL ProcessHTTP(PTCPSocket & socket);

    protected:
      PSocket  * m_HTTPListeningSocket;
      PHTTPSpace httpNameSpace;
      PString    macroKeyword;

      PTEACypher::Key m_ProductKey;
      PStringArray    m_SecuredKeys;
      PTEACypher::Key m_SignatureKey;
      BOOL            m_IgnoreSignatures;

      void ShutdownListener();

      PThread *  restartThread;

      PLIST(ThreadList, HTTPServerThread);
      ThreadList m_HTTPThreads;
      PMutex     httpThreadsMutex;

      friend class HTTPServerThread;
  };

  ///////////////////////////////////////////////////////////////////////////

  class XMLRPCRequest : public PXML
  {
    PCLASSINFO(XMLRPCRequest, PXML);
    
  public:
      XMLRPCRequest();
      
      XMLRPCRequest(const PXMLRPCBlock & xml);

      XMLRPCRequest & operator=( const PXMLRPCBlock & xml ); 

      PXMLElement * GetParams();
      PXMLElement * GetParam(PINDEX idx) const;
      PINDEX GetParamCount() const;

      // used when used as a response
      PINDEX  GetFaultCode() const                     { return faultCode; }
      PString GetFaultText() const                     { return faultText; }
      void SetFault(PINDEX code, const PString & text) { faultCode = code; faultText = text; }
      BOOL ValidateResponse();

      // helper functions for getting parameters
      BOOL GetParams(PXMLRPCStructBase & data);
      BOOL GetParam(PINDEX idx, PString & type, PString & result);
      BOOL GetExpectedParam(PINDEX idx, const PString & expectedType, PString & value);

      BOOL GetParam(PINDEX idx, PString & result);
      BOOL GetParam(PINDEX idx, int & result);
      BOOL GetParam(PINDEX idx, double & result);
      BOOL GetParam(PINDEX idx, PTime & result, int tz = PTime::GMT);
      BOOL GetParam(PINDEX idx, PStringToString & result);
      BOOL GetParam(PINDEX idx, PXMLRPCStructBase & result);
      BOOL GetParam(PINDEX idx, PStringArray & result);
      BOOL GetParam(PINDEX idx, PArray<PStringToString> & result);

      // static functions for parsing values
      BOOL ParseScalar(PXMLElement * element, PString & type, PString & value);
      BOOL ParseStruct(PXMLElement * element, PStringToString & structDict);
      BOOL ParseStruct(PXMLElement * element, PXMLRPCStructBase & structData);
      BOOL ParseArray(PXMLElement * element, PStringArray & array);
      BOOL ParseArray(PXMLElement * element, PArray<PStringToString> & array);
      BOOL ParseArray(PXMLElement * element, PXMLRPCVariableBase & array);

      // static functions for creating values
      static PXMLElement * CreateValueElement(PXMLElement * element);
      static PXMLElement * CreateScalar(const PString & type, const PString & scalar);
      static PXMLElement * CreateMember(const PString & name, PXMLElement * value);

      static PXMLElement * CreateScalar(const PString & str);
      static PXMLElement * CreateScalar(int value);
      static PXMLElement * CreateScalar(double value);
      static PXMLElement * CreateDateAndTime(const PTime & time);
      static PXMLElement * CreateBinary(const PBYTEArray & data);

      static PXMLElement * CreateStruct();
      static PXMLElement * CreateStruct(const PStringToString & dict);
      static PXMLElement * CreateStruct(const PStringToString & dict, const PString & typeStr);
      static PXMLElement * CreateStruct(const PXMLRPCStructBase & structData);

      static PXMLElement * CreateArray(const PStringArray & array);
      static PXMLElement * CreateArray(const PStringArray & array, const PString & typeStr);
      static PXMLElement * CreateArray(const PStringArray & array, const PStringArray & types);
      static PXMLElement * CreateArray(const PArray<PStringToString> & array);
      static PXMLElement * CreateArray(const PXMLRPCVariableBase & array);

      // helper functions for adding parameters
      void AddParam(PXMLElement * parm);
      void AddParam(const PString & str);
      void AddParam(int value);
      void AddParam(double value);
      void AddParam(const PTime & time);
      void AddParam(const PXMLRPCStructBase & structData);
      void AddBinary(const PBYTEArray & data);
      void AddStruct(const PStringToString & dict);
      void AddStruct(const PStringToString & dict, const PString & typeStr);
      void AddArray(const PStringArray & array);
      void AddArray(const PStringArray & array, const PString & typeStr);
      void AddArray(const PStringArray & array, const PStringArray & types);
      void AddArray(const PArray<PStringToString> & array);

    protected:
      PXMLElement * params;
      PString faultText;
      PINDEX  faultCode;
  };

  //////////////////////////////////////////////////////////////////////

  class XMLRPCServer : public HTTPServer
  {
    PCLASSINFO( XMLRPCServer, HTTPServer );
  public:

    
    XMLRPCServer();

    virtual ~XMLRPCServer();

    BOOL ListenForXMLRPC(
      PINDEX stackSize = 1024 * 16
    );

    BOOL RegisterMethod(
      const PString & method,
      PNotifier notifier
    );

    BOOL SendRequest(
      PXMLRPCBlock & request,
      const PURL & url,
      XMLRPCRequest & response
    );

    PINLINE void SetXMLRPCPort( WORD port ){ m_XMLRPCPort = port; };

    PINLINE WORD GetXMLRPCPort()const{ return m_XMLRPCPort; };

  protected:

    BOOL Initialize();

  protected:
    PXMLRPCServerResource m_XMLRPCServer;
    WORD m_XMLRPCPort;
    PINDEX m_StackSize;
  };

  //////////////////////////////////////////////////////////////////

  class XMLRPCExchange : public PObject
  {
    PCLASSINFO( XMLRPCExchange, PObject );
  public:

    XMLRPCExchange( XMLRPCServer & server );

    ~XMLRPCExchange();

    BOOL SendRequest(
      const PString & method,
      PArray<PStringToString>  & request,
      PArray<PStringToString>  & provisionalResponse,
      const PURL & url
    );

    void RegisterMethod(
      const PString & method 
    );

    PDECLARE_NOTIFIER(PXMLRPCServerParms, XMLRPCExchange, OnXMLRPCRequest);

    virtual void OnIncomingRequest( 
      PXMLRPCServerParms & params 
    );

    virtual void OnIncomingRequest( 
      const PString & method,
      PArray<PStringToString> & requestParams,
      PArray<PStringToString> & responseParams
    );

  protected:

    XMLRPCServer & m_XMLRPCServer;
  };

  ////////////////////////////////////////////////////////////////////////////

  class XMLRPCTransactionServer;

  class XMLRPCTransaction : public PObject
  {
    PCLASSINFO( XMLRPCTransaction, PObject );
  public:

    enum Direction
    {
      Client,
      Server
    };

    XMLRPCTransaction(
      XMLRPCTransactionServer * server,
      Direction dir,
      const PString & method,
      const PString & transactionId,
      const PString & sessionId
    );

    virtual ~XMLRPCTransaction();

    void SetFinalResponse(
      const PArray<PStringToString> & response
    );

    PINLINE const PString & GetTransactionId()const{ return m_TransactionId; };
    PINLINE const PString & GetSessionId()const{ return m_SessionId; };
    PINLINE const PString & GetMethod()const{ return m_Method; };
    PINLINE const PArray<PStringToString> & GetFinalResponse()const{ return m_FinalResponse; };
    PINLINE BOOL WaitForFinalReponseEvent( const PTimeInterval & time ){ return m_FinalResponseEvent.Wait( time ); };
    PINLINE Direction GetDirection()const{ return m_Direction; };
    PINLINE const PArray<PStringToString> & GetRequest()const{ return m_Request; };
    PINLINE void SetRequest( const PArray<PStringToString> & request ){ m_Request = request; };
    PINLINE void SetResponseURL( const PURL & url ){ m_ResponseURL = url; };
    PINLINE const PURL & GetResponseURL()const{ return m_ResponseURL; };
    PDICTIONARY( SortedCollection, PCaselessString, XMLRPCTransaction );
    PLIST( Collection, XMLRPCTransaction );
  protected:

    Direction m_Direction;
    XMLRPCTransactionServer * m_TransactionServer;
    PString m_TransactionId;
    PString m_SessionId;
    PString m_Method;
    PURL m_ResponseURL;

    PSyncPoint m_FinalResponseEvent;
    PArray<PStringToString> m_FinalResponse;
    PArray<PStringToString> m_Request;
  };

  ///////////////////////////////////////////////////////////////////////////////

  class XMLRPCTransactionServer : public PObject
  {
    PCLASSINFO( XMLRPCTransactionServer, PObject );
  public:

    enum ResponseCode
    {
      FailureOnSend,
      FailureOnProvision,
      FailureOnFinal,
      Success
    };

    XMLRPCTransactionServer();

    XMLRPCTransactionServer(
      const PStringArray & methods,
      WORD port = 8090,
      PINDEX stackSize = 0x400 * 10
    );

    BOOL Initialize(
      const PStringArray & methods,
      WORD port = 8090,
      PINDEX stackSize = 0x400 * 10
    );

    virtual ~XMLRPCTransactionServer();

    ResponseCode SendRequest(
      const PString & method,
      PArray<PStringToString>  & request,
      PArray<PStringToString>  & provisionalResponse,
      const PURL & url
    );

    ResponseCode SendRequest(
      const PString & method,
      PArray<PStringToString>  & request,
      PArray<PStringToString>  & response,
      const PTimeInterval & timeOut, 
      const PURL & url,
      const PString & sessionId = PString::Empty()
    );

    virtual void OnIncomingRequest(
      const PString & method,
      PArray<PStringToString>  & request,
      PArray<PStringToString>  & response
    );

    virtual void OnIncomingTransaction(
      PURL responseURL,
      const PString & method,
      const PString & sessionId,
      const PString & tag,
      PArray<PStringToString> & request,
      PStringToString & immediateResponse
    );



    BOOL FindTransactionAndSetResponse(
      const PString & method,
      PArray<PStringToString> & request 
    );

    XMLRPCTransaction * DequeueTransaction();


    PString GenTagParameter();

    PINLINE XMLRPCServer * GetXMLRPCServer(){ return m_XMLRPCServer; };

    BOOL RegisterMethod( 
      const PString & method 
    );

  protected:

    XMLRPCExchange * m_Exchange;
    XMLRPCServer * m_XMLRPCServer;
    PStringArray m_Methods;
    
    XMLRPCTransaction::SortedCollection m_Transactions;
    PMutex m_TransactionsMutex;

    /// Read Queue
    PQUEUE( TransactionQueue, XMLRPCTransaction );
    TransactionQueue m_TransactionQueue;
    PMutex m_TransactionQueueMutex;
    PSemaphore m_TransactionQueueSync;

  };

  class XMLRPCEndpoint : public XMLRPCExchange
  {
    PCLASSINFO( XMLRPCEndpoint, XMLRPCExchange );
  public:

    XMLRPCEndpoint( 
      XMLRPCServer & server, 
      XMLRPCTransactionServer & transactionServer 
    );

    virtual void OnIncomingRequest( 
      const PString & method,
      PArray<PStringToString> & params,
      PArray<PStringToString> & responseParams
    );

  protected:
    XMLRPCTransactionServer & m_TransactionServer;
  };

  //////////////////////////////////////////////////////////////////////

  class CallControlClient : public XMLRPCTransactionServer
  {
    PCLASSINFO( CallControlClient, XMLRPCTransactionServer );
    
  public:

    class XMLRPCProcessor : public PThread
    {
      PCLASSINFO( XMLRPCProcessor, PThread );
    public:
      XMLRPCProcessor( 
        CallControlClient & callControl
      );

      void Main();

    protected:
      CallControlClient & m_CallControl;
    };

    CallControlClient();

    virtual ~CallControlClient();

    BOOL CCIInitialize(
      WORD listenerPort,
      const PString & serverURI
    );

    virtual BOOL ProcessCCIEvents();

    BOOL MakeCall(
      const PString & uri
    );

  protected:
    PThread * m_XMLRPCProcessor;
    PURL m_ServerURL;
  };

};

#endif /// P_EXPAT
#endif //ENABLE_ECCI
#endif


