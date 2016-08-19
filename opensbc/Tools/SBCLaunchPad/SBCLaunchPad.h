/*
 *
 * SBCLaunchPad.h
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
 * $Log: SBCLaunchPad.h,v $
 * Revision 1.9  2007/01/17 10:33:45  joegenbaclor
 * Redesigned syslog
 *
 * Revision 1.8  2007/01/17 00:09:14  joegenbaclor
 * Added SysLog
 *
 * Revision 1.7  2007/01/15 06:32:23  joegenbaclor
 * no message
 *
 * Revision 1.6  2007/01/13 08:04:41  joegenbaclor
 * more launcher code
 *
 * Revision 1.5  2007/01/12 10:50:05  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.4  2007/01/11 10:00:43  joegenbaclor
 * more launcher code
 *
 * Revision 1.3  2007/01/10 23:58:38  joegenbaclor
 * launcher specific code
 *
 * Revision 1.2  2007/01/09 23:47:49  joegenbaclor
 * Added Scripting Squirrel Support
 *
 * Revision 1.1  2007/01/04 09:38:50  joegenbaclor
 * Initial upload of Launch Pad classes
 *
 *
 */

#ifndef SBCLAUNCHPAD_H
#define SBCLAUNCHPAD_H

#include <ptlib.h>
#include "OSSApplication.h"
#include "ProcessMonitor.h"
#include "sqplus.h"
#include "SIPMessage.h"
#include "SBCSysLog.h"


using namespace Tools;
using namespace SqPlus;
using namespace SIPParser;

class ProcessMonitor;

class SBCLaunchPad : public OSSApplication
{
  PCLASSINFO( SBCLaunchPad, OSSApplication );
public:
 SBCLaunchPad();

 virtual ~SBCLaunchPad();

  // pure virtuals from OSSApplication
  virtual void OnStart( 
    OSSAppConfig & config 
  );

  virtual void OnStop();

  virtual void OnConfigChanged( 
    OSSAppConfig & config 
  );

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

  PString OnLoadApplicationStatus( 
    const PString & htmlBlock 
  );

  BOOL OnPostControl(
    const PStringToString & data, 
    PHTML & msg
  );

  virtual void OnAdditionalXMLRPCMethods(
    PStringArray & methods
  );

  virtual void OnCustomXMLRPCRequest(
    const PString & method,
    PArray<PStringToString> & request,
    PArray<PStringToString> & response
  );

  BOOL AddRelayRoute(
    const PString & route,
    const PString & xmlRPCServer
  );

  PString UnregisterSession(
    const PString & callIs,
    const PString & xmlRPCServer
  );

  BOOL StartSysLog(
    const PIPSocket::Address & bindAddress = PIPSocket::Address( "127.0.0.1" ),
    WORD port = 514,
    const PString & directory = "logs",
    const PString & filePrefix = "syslog",
    const PString & fileSuffix = PString::Empty()
  );

  PINLINE ProcessMonitor * GetProcessMonitor(){ return m_ProcessMonitor; };


protected:
  ProcessMonitor * m_ProcessMonitor;
  SIPURI m_ExtendedArgs;
  SBCSysLog * m_SysLog;
  PDICTIONARY( SysLogs, PCaselessString, SBCSysLog );
  SysLogs m_SysLogs;
};



class SBCRegistrationStatusPage : public PServiceHTTPString
{
  PCLASSINFO( SBCRegistrationStatusPage, PServiceHTTPString );

  public:
    SBCRegistrationStatusPage(
      SBCLaunchPad & app, PHTTPAuthority & auth);
    
    virtual BOOL Post(
      PHTTPRequest & request,
      const PStringToString &,
      PHTML & msg
    );

  
  private:
    SBCLaunchPad & m_SBCLaunchPad;
};

class ProcessStatusPage : public PServiceHTTPString
{
  PCLASSINFO( ProcessStatusPage, PServiceHTTPString );

  public:
    ProcessStatusPage(
      SBCLaunchPad & app, PHTTPAuthority & auth
    );
    
    virtual BOOL Post(
      PHTTPRequest & request,
      const PStringToString &,
      PHTML & msg
    );
  
    static PString HTMLString( SBCLaunchPad & app );

  private:
    SBCLaunchPad & m_SBCLaunchPad;
};

#endif







