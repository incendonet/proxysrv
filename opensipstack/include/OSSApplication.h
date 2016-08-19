/*
 *
 * OSSApplication.h
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
 * $Log: OSSApplication.h,v $
 * Revision 1.14  2007/01/17 00:12:19  joegenbaclor
 * 1.  Fixed bug in call session where CANCEL may not be sent if a call ahs been authenticated due to a misplaced SetState( StateDisconnected ) for 4xx events
 * 2.  Fixed register session to ignore authorization headers if allow all reg is flagged
 * 3.  Temporary hacks for syslog
 * 4.  More launcher code -  almost there!
 *
 * Revision 1.13  2007/01/12 10:49:07  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.12  2007/01/10 23:57:52  joegenbaclor
 * launcher specific code
 *
 * Revision 1.11  2007/01/08 11:15:50  joegenbaclor
 * Added capability to add status pages
 *
 * Revision 1.10  2007/01/04 09:36:01  joegenbaclor
 * Added XML RPC Configuration Capability
 *
 * Revision 1.9  2006/12/22 07:39:06  joegenbaclor
 * More on command line argument parsing.
 *
 * Revision 1.8  2006/12/21 18:10:09  joegenbaclor
 * Allowed setting of HTTP service port via command line.  More changes to follow ...
 *
 * Revision 1.7  2006/12/21 08:59:43  joegenbaclor
 * Corrected typos in last commit
 *
 * Revision 1.6  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/08/29 10:46:05  rcolobong
 * *** empty log message ***
 *
 * Revision 1.4  2006/06/14 08:43:38  joegenbaclor
 * Initial upload of OpenB2BUA applcation and related classes
 *
 * Revision 1.3  2006/06/09 07:37:30  joegenbaclor
 * 1.  Added Publish, SIPETag & SIPIfMatch  support in SIPMessage
 * 2.  Started work on StatusPage Resource for OSSAppConfig
 *
 * Revision 1.2  2006/06/06 13:30:25  joegenbaclor
 * Initial upload of PIDF files - thanks Solegy Systems!
 *
 * Revision 1.1  2006/05/31 07:21:51  joegenbaclor
 * 1.  Added OSSApplication and StringTuple classes.
 * 2.  Corrected some typos
 *
 *
 */


#ifndef OSSAPPLICATION_H
#define OSSAPPLICATION_H

#include <ptlib.h>
#include "OSSAppConfig.h"

#if P_EXPAT

namespace Tools
{
 #if P_SSL
  #include <ptclib/shttpsvc.h>
  typedef PSecureHTTPServiceProcess OSSApplicationAncestor;
  #else
  #include <ptclib/httpsvc.h>
  typedef PHTTPServiceProcess OSSApplicationAncestor;
  #endif

  //extern PHTTPServiceProcess::Info OSSAppInfo;

  class OSSApplication : public OSSApplicationAncestor
  {
    PCLASSINFO( OSSApplication, OSSApplicationAncestor );

  public:
    OSSApplication();

    void Main();

    BOOL OnStart();

    void OnStop();

    void OnControl();

    BOOL Initialise(
      const char * initMsg
    );

    virtual PString GetConfigurationFilePath()const;

    virtual void OnStart( 
      OSSAppConfig & config 
    ) = 0;

    virtual void OnConfigChanged();

    virtual void OnConfigChanged( 
      OSSAppConfig & config 
    ) = 0;

    virtual OSSAppConfig * OnCreateAppConfig();

    virtual void OnAdditionalHTTPResource(
      PHTTPSimpleAuth & authority
    );

    virtual void OnAdditionalHTTPLinks( 
      PHTML & html 
    );

    virtual void OnAdditionalXMLRPCMethods(
      PStringArray & moreMethods
    );

    virtual void OnCustomXMLRPCRequest(
      const PString & method,
      PArray<PStringToString> & request,
      PArray<PStringToString> & response
    );

    BOOL SendCustomXMLRPCRequest(
      const PString & method,
      PArray<PStringToString> & request,
      PArray<PStringToString> & immediateResponse,
      const PURL & url
    );

    virtual void OnHandleCommandArgs(
      PINDEX i,
      const PString & arg
    ) = 0;

    BOOL StartHTTPService();

    static OSSApplication * GetInstance();

    PINLINE PHTTPSpace & GetHTTPNameSpace(){ return httpNameSpace; };
    PINLINE BOOL IsFirstRun()const{ return m_FirstRun; };
    PINLINE const PString & GetLogDirectory()const{ return m_LogDirectory; };
    PINLINE void SetLogDirectory( const PString & dir ){ m_LogDirectory = dir; };
    
  protected:
    static OSSApplication * m_Instance;
    BOOL m_FirstRun;
    PString m_LogDirectory;
  };

};

#endif // P_EXPAT
#endif // OSSAPPLICATION_H




