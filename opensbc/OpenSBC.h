/*
 *
 * OpenSBC.h
 * 
 * Open SIP Stack ( OSS )
 *
 * Copyright (c) opensipstack.org.
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either expressed or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is OpenSIPStack.
 *
 * The Initial Developer of the Original Code is opensipstack.org.
 *
 * The author of this code is Joegen E. Baclor
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: OpenSBC.h,v $
 * Revision 1.15  2007/01/17 10:33:43  joegenbaclor
 * Redesigned syslog
 *
 * Revision 1.14  2007/01/17 00:05:59  joegenbaclor
 * Added SysLog
 *
 * Revision 1.13  2007/01/12 10:50:05  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.12  2007/01/12 01:02:50  joegenbaclor
 * launcher related code
 *
 * Revision 1.11  2007/01/10 23:58:30  joegenbaclor
 * launcher specific code
 *
 * Revision 1.10  2007/01/08 11:15:18  joegenbaclor
 * Added registration status page
 *
 * Revision 1.9  2007/01/08 07:13:23  joegenbaclor
 * Added ability to run SBC in pure proxy or pure b2b modes
 *
 * Revision 1.8  2006/12/22 07:40:27  joegenbaclor
 * More on command line argument parsing.
 *
 * Revision 1.7  2006/11/22 11:37:09  rcolobong
 * 1. Added new support for Force B2BUA in HTTP configuration
 * 2. Added log level in Application level and SIP/RTP Level
 * 3. Update OnAcceptRegistration signature
 *
 * Revision 1.6  2006/10/11 04:59:50  rcolobong
 * Add http configuration "Accept all registration". If this true then it will accept all registration even if it is not included on the "Local domain accounts"
 *
 * Revision 1.5  2006/09/01 12:51:03  rcolobong
 * Use ActiveSessionCounter to update session count through OSSApplication
 *
 * Revision 1.4  2006/08/30 09:07:31  rcolobong
 * Update setting connection timer and alerting timer
 *
 * Revision 1.3  2006/08/29 07:43:58  rcolobong
 * 1. Rename file to a better filename
 * 2. Change SBCRTBESesionManager class to RTBESessionManager
 *
 * Revision 1.2  2006/08/16 10:34:32  rcolobong
 * 1. Syslog server is now configurable
 * 2. Alerting timer and Connection Timer is now configurable
 *
 * Revision 1.1  2006/08/14 10:04:59  rcolobong
 * Convert B2BUA to SBC
 * Support MP logging
 *
 * Revision 1.5  2006/07/11 13:59:34  joegenbaclor
 * Completed Proxy functionalities for B2BUA
 *
 * Revision 1.4  2006/07/10 06:29:37  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.3  2006/07/03 15:29:47  joegenbaclor
 * Started merging OpenB2BUA into OpenSBC
 *
 * Revision 1.2  2006/06/20 09:58:11  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.1  2006/06/14 08:43:38  joegenbaclor
 * Initial upload of OpenB2BUA applcation and related classes
 *
 *
 */

#ifndef OPENSBC_H
#define OPENSBC_H

#include <ptlib.h>
#include "OSSApplication.h"
#include "B2BUA.h"

using namespace B2BUA;

class OpenSBCDaemon;
class SBCRoutingHandler;
class SBCCallHandler;
class SBCAuthHandler;

class OpenSBC : public B2BUserAgent
{
  PCLASSINFO( OpenSBC, B2BUserAgent );
public:
  class ActiveSessionCounter : public PThread
  {
    PCLASSINFO( ActiveSessionCounter, PThread );
  public:
    ActiveSessionCounter();

    void Stop();

    virtual void Main();

  private:
    PSyncPoint m_SyncPoint;
  }; // class ActiveSessionCounter

  OpenSBC( 
    OpenSBCDaemon * application,
    UAMode mode
  );

  virtual void OnStart( 
    OSSAppConfig & config 
  );

  virtual void OnStop();

  virtual void OnConfigChanged( 
    OSSAppConfig & config 
  );

  virtual B2BCallInterface * OnCreateCallInterface();

  virtual B2BAuthInterface * OnCreateAuthInterface();

  virtual B2BMediaInterface * OnCreateMediaInterface();

  virtual B2BRoutingInterface * OnCreateRoutingInterface();

  virtual RegisterSession::AcceptRegistrationResponse OnAcceptRegistration( 
    RegisterSession & session,
	  const SIPMessage & request
  );

  virtual void ProcessEvent(
    SIPStackEvent * event
  );

  PINLINE BOOL WillAcceptAllRegistration()const{ return m_AcceptAllRegistration; };
  PINLINE OpenSBCDaemon * GetApplication(){ return m_Application; };
  PINLINE SBCAuthHandler * GetSBCAuthHandler(){ return m_SBCAuthHandler; };
  PINLINE SBCCallHandler * GetSBCCallHandler(){ return m_SBCCallHandler; };
  PINLINE SBCRoutingHandler * GetSBCRoutingHandler(){ return m_SBCRoutingHandler; };

protected:
  OpenSBCDaemon * m_Application;
  
  
  PLIST( ForceRoutes, SIPURI );
  ForceRoutes m_ForceRoutes;

  ActiveSessionCounter m_ActiveSessionCounter;
  BOOL m_AcceptAllRegistration;

  SBCAuthHandler * m_SBCAuthHandler;
  SBCCallHandler * m_SBCCallHandler;
  SBCRoutingHandler * m_SBCRoutingHandler;
};

//////////////////////////////////////////////////////////////////

class OpenSBCDaemon : public OSSApplication
{
  PCLASSINFO( OpenSBCDaemon, OSSApplication );
public:

  OpenSBCDaemon();

  ~OpenSBCDaemon();

  
  // pure virtual from OSSApplication
  virtual void OnStart( 
    OSSAppConfig & config 
  );

  virtual void OnStop();

  virtual void OnConfigChanged( 
    OSSAppConfig & config 
  );

  PDECLARE_NOTIFIER( PThread, OpenSBCDaemon, OnConfigChanged );

  virtual void OnHandleCommandArgs(
      PINDEX i,
      const PString & arg
  );

  virtual OSSAppConfig * OnCreateAppConfig();

  virtual void OnAdditionalHTTPResource( 
    PHTTPSimpleAuth & authority
  );

  virtual void OnAdditionalHTTPLinks( 
    PHTML & html 
  );

  PString OnLoadRegistrationStatus( 
    const PString & htmlBlock 
  );

  BOOL OnPostControl(
    const PStringToString & data, 
    PHTML & msg
  );

  virtual void OnAdditionalXMLRPCMethods(
    PStringArray & moreMethods
  );

  virtual void OnCustomXMLRPCRequest(
    const PString & method,
    PArray<PStringToString> & request,
    PArray<PStringToString> & response
  );

  void SetPTraceLevel( unsigned level );

  PINLINE OpenSBC * GetSBC(){ return m_SBC; };
  PINLINE const PString & GetInstanceId()const{ return m_InstanceId; };
  PINLINE const SIPURI & GetExtendedArgs()const{ return m_ExtendedArgs; };
  PINLINE BOOL HasExtendedArgs()const{ return m_HasExtendedArgs; };
protected:
  OpenSBC * m_SBC;
  B2BUAEndPoint * m_EndPoint;
  B2BUserAgent::Registrar * m_Registrar;
  B2BUserAgent::Proxy * m_Proxy;
  PString m_InstanceId;
  SIPURI m_ExtendedArgs;
  BOOL m_HasExtendedArgs;
  BOOL m_IsInitialized;
};

class SBCRegistrationStatusPage : public PServiceHTTPString
{
  PCLASSINFO( SBCRegistrationStatusPage, PServiceHTTPString );

  public:
    SBCRegistrationStatusPage(
      OpenSBCDaemon & app, PHTTPAuthority & auth);
    
    virtual BOOL Post(
      PHTTPRequest & request,
      const PStringToString &,
      PHTML & msg
    );
  
  private:
    OpenSBCDaemon & app;
};

#endif




