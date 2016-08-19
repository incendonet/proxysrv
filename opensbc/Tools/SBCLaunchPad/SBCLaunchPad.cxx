/*
 *
 * SBCLaunchPad.cxx
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
 * $Log: SBCLaunchPad.cxx,v $
 * Revision 1.14  2007/01/22 10:01:03  joegenbaclor
 * Fixed ProxyRouteRequest
 *
 * Revision 1.13  2007/01/18 10:54:02  joegenbaclor
 * Added unix configure script
 *
 * Revision 1.12  2007/01/18 06:56:25  joegenbaclor
 * Defined PTRACING for windows release build
 *
 * Revision 1.11  2007/01/18 06:07:05  joegenbaclor
 * Incremented Build Number for latest development release
 *
 * Revision 1.10  2007/01/17 10:33:45  joegenbaclor
 * Redesigned syslog
 *
 * Revision 1.9  2007/01/17 00:09:14  joegenbaclor
 * Added SysLog
 *
 * Revision 1.8  2007/01/15 06:32:23  joegenbaclor
 * no message
 *
 * Revision 1.7  2007/01/13 08:04:41  joegenbaclor
 * more launcher code
 *
 * Revision 1.6  2007/01/12 10:50:05  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.5  2007/01/12 01:02:55  joegenbaclor
 * launcher related code
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


#include "SBCLaunchPad.h"
#include "SIPTransportManager.h"
#include "../../Version.h"

#define new PNEW

using namespace SIPTransports;

PCREATE_PROCESS( SBCLaunchPad );

static void VMPrintFunc(HSQUIRRELVM v,const SQChar * s,...) 
{
  static SQChar temp[2048];
  va_list vl;
  va_start(vl,s);
  scvsprintf( temp,s,vl);
  //SCPUTS(temp);
  PTRACE( 1, "SCRIPT\t" << temp );
  va_end(vl);
} // printFunc

static SBCLaunchPad * m_LaunchPad = NULL;
class ScriptingHost
{
public:
  ScriptingHost()
  {
  }

  static void Init()
  {
    /// Initialize Scripting VM
    SquirrelVM::Init();
    sq_setprintfunc(SquirrelVM::GetVMPtr(),VMPrintFunc);

    SQClassDef<ScriptingHost>(_T("ScriptingHost")).
      func(&ScriptingHost::AddProcess,_T("AddProcess")).
      func(&ScriptingHost::RemoveProcess,_T("RemoveProcess")).
      func(&ScriptingHost::AddRelayRoute,_T("AddRelayRoute")).
      func(&ScriptingHost::StartSysLog,_T("StartSysLog")).
      func(&ScriptingHost::IntegerToString,_T("IntegerToString")).
      func(&ScriptingHost::SetProxyXMLRPCAddress,_T("SetProxyXMLRPCAddress")).
      func(&ScriptingHost::AppendB2BPort,_T("AppendB2BPort")).
      func(&ScriptingHost::SetB2BInterfaceAddress,_T("SetB2BInterfaceAddress"));
  }

  const SQChar * IntegerToString( int i )
  {
    return static_cast<const char *>( PString( i ) );
  }

  void AddProcess(
    const SQChar * processKey,
    const SQChar * dir,
    const SQChar * app,
    const SQChar * args,
    const SQChar * pidFile,
    const SQChar * extraInfo,
    BOOL autoExec
  )
  {
    PString _dir( dir );
    PDirectory path  = PProcess::Current().GetFile().GetDirectory();
    PString finalPath;
    PString finalPidFile;
    PString appName( processKey );
    PString arguments( args );
    if( _dir != "$default" )
      path = _dir;
    
    finalPath = path + app;
    finalPidFile = path + pidFile;

    if( appName.ToLower().Find( "oss." ) != P_MAX_INDEX )
    {
      arguments = "-p " + finalPidFile + " " + arguments;
    }

    m_LaunchPad->GetProcessMonitor()->AddProcess( processKey, finalPath, arguments, finalPidFile, extraInfo, autoExec );
  }

  void RemoveProcess(
    const SQChar * processKey,
    BOOL autoShutdown
  )
  {
    m_LaunchPad->GetProcessMonitor()->RemoveProcess( processKey, autoShutdown );
  }

  void AddRelayRoute(
    const SQChar * route,
    const SQChar * xmlRPCServer
  )
  {
    m_LaunchPad->AddRelayRoute( route, xmlRPCServer );
  }

  void StartSysLog(
    const SQChar * bindAddress,
    const SQChar * port,
    const SQChar * directory,
    const SQChar * prefix,
    const SQChar * suffix
  )
  {
    PString _port( port );
    m_LaunchPad->StartSysLog( 
      PIPSocket::Address( bindAddress ), 
      _port.AsInteger(), 
      directory,
      prefix,
      suffix );
  }

  void SetProxyXMLRPCAddress(
    const SQChar * addr
  )
  {
    ScriptingHost::m_ProxyXMLRPCAddress = addr;
  }

  void SetB2BInterfaceAddress( 
    const SQChar * addr
  )
  {
    m_B2BInterfaceAddress = addr;

  }

  void AppendB2BPort( const SQChar * port )
  {
    PString _port( port );
    m_B2BPorts[m_B2BPorts.GetSize()] = _port.AsInteger();
  }

  static PString m_ProxyXMLRPCAddress;
  static PIntArray m_B2BPorts;
  static PString m_B2BInterfaceAddress;
};

PString ScriptingHost::m_ProxyXMLRPCAddress;
PIntArray ScriptingHost::m_B2BPorts;
PString ScriptingHost::m_B2BInterfaceAddress;

SBCLaunchPad::SBCLaunchPad()
{
  m_ProcessMonitor = NULL;
  m_LaunchPad = this;
  m_SysLog = NULL;

  SetProductName( "OpenSBC Launch Pad" );
  SetMajorVersion( SBC_MAJOR_VERSION );
  SetMinorVersion( SBC_MINOR_VERSION );
  SetBuildNumber( SBC_BUILD_NUMBER );
}

SBCLaunchPad::~SBCLaunchPad()
{
  delete m_ProcessMonitor;
  delete m_SysLog;
}

// pure virtuals from OSSApplication
void SBCLaunchPad::OnStart( 
  OSSAppConfig & config 
)
{
  if( m_ProcessMonitor == NULL )
    m_ProcessMonitor = new ProcessMonitor( 1000 );

  ScriptingHost::Init();

  PFilePath lauchScript( "scripts/launch.script" );
  SquirrelObject sq = SquirrelVM::CompileScript( lauchScript );

  try {
    SquirrelVM::RunScript(sq);
  } catch (SquirrelError & e) {
    scprintf(_T("Error: %s, %s\n"),e.desc,_T("launch.script"));
  } // catch


}

BOOL SBCLaunchPad::AddRelayRoute(
  const PString & route,
  const PString & url
)
{
  PArray<PStringToString> request;
  PStringToString * requestParam = new PStringToString();
  requestParam->SetAt( "Route", route );
  request.SetAt( 0, requestParam );
  PArray<PStringToString> response;
  return SendCustomXMLRPCRequest( "Routing.AddRelayRoute", request, response, url );
}

PString SBCLaunchPad::UnregisterSession(
  const PString & callId,
  const PString & xmlRPCServer
)
{
  PArray<PStringToString> request;
  PStringToString * requestParam = new PStringToString();
  requestParam->SetAt( "Call-ID", callId );
  request.SetAt( 0, requestParam );
  PArray<PStringToString> response;
  SendCustomXMLRPCRequest( "Registrar.Unregister", request, response, xmlRPCServer );

  PStringToString * responseParams = dynamic_cast<PStringToString*>(response.GetAt(1));
  if( responseParams == NULL )
    return "Server Error [No Parameter in response]";
  
  PString * msg = responseParams->GetAt( "Message" );
  if( msg == NULL )
    return "Server Error - [Response Message not set]";

  return *msg;
  
}

void SBCLaunchPad::OnStop()
{
  PFilePath lauchScript( "scripts/stop.script" );
  SquirrelObject sq = SquirrelVM::CompileScript( lauchScript );

  try {
    SquirrelVM::RunScript(sq);
  } catch (SquirrelError & e) {
    scprintf(_T("Error: %s, %s\n"),e.desc,_T("stop.script"));
  } 
}

BOOL SBCLaunchPad::StartSysLog(
  const PIPSocket::Address & bindAddress,
  WORD port,
  const PString & _directory,
  const PString & filePrefix,
  const PString & fileSuffix
)
{
  if( !bindAddress.IsValid() )
    return FALSE;

  PString directory = _directory;
  if( directory.IsEmpty() )
    directory = GetFile().GetDirectory();

  PStringStream key;
  key << bindAddress << ":" << port;
  if( m_SysLogs.Contains( key ) )
    return FALSE;
  

  m_SysLogs.SetAt( key, new SBCSysLog( bindAddress, port, directory, filePrefix, fileSuffix ) );
  
  return TRUE;
}


void SBCLaunchPad::OnConfigChanged( 
  OSSAppConfig & config 
)
{
  config.RPCConfigChanged();
}

void SBCLaunchPad::OnHandleCommandArgs(
    PINDEX i,
    const PString & arg
)
{
  if( i == 0 )  /// 'X'
  {
    m_ExtendedArgs = arg;
  }
}


OSSAppConfig * SBCLaunchPad::OnCreateAppConfig()
{
  PString xmlRPCPort =  m_ExtendedArgs.GetParameter( "xml-rpc-port" );
  WORD port = static_cast<WORD>(xmlRPCPort.IsEmpty() ? 10010 : xmlRPCPort.AsInteger());
  return new OSSAppConfig( OSSAppConfig::XMLRPCAndHTTPServerProvider, port );
}



void SBCLaunchPad::OnAdditionalHTTPResource( 
  PHTTPSimpleAuth & authority
)
{
  httpNameSpace.AddResource(new SBCRegistrationStatusPage(*this, authority), PHTTPSpace::Overwrite);
  httpNameSpace.AddResource(new ProcessStatusPage(*this, authority), PHTTPSpace::Overwrite);
}

void SBCLaunchPad::OnAdditionalHTTPLinks( 
  PHTML & html 
)
{
  html  << PHTML::Paragraph() << "<center>"
        << PHTML::HotLink("OpenSBC Registration Status") << "OpenSBC Registration Status" << PHTML::HotLink()
        << PHTML::Paragraph();

  html  << PHTML::Paragraph() << "<center>"
        << PHTML::HotLink("OpenSBC Application Status") << "OpenSBC Application Status" << PHTML::HotLink()
        << PHTML::Paragraph();

}



void SBCLaunchPad::OnAdditionalXMLRPCMethods(
  PStringArray & methods
)
{
  methods.AppendString( "Registrar.GetRegStatus" );
  methods.AppendString( "Registrar.FindReg" );
  methods.AppendString( "Registrar.Unregister" );
  methods.AppendString( "Proxy.Startup" );
  methods.AppendString( "B2BUA.Startup" );
}

void SBCLaunchPad::OnCustomXMLRPCRequest(
  const PString & method,
  PArray<PStringToString> & request,
  PArray<PStringToString> & response
)
{
  if( method == "Proxy.Startup" )
  {
    PString iface = ScriptingHost::m_B2BInterfaceAddress;
    if( iface == "*" )
    {
      PIPSocket::InterfaceTable ifTable;
      if( PIPSocket::GetInterfaceTable( ifTable ) )
      {
        for( PINDEX i = 0; i < ifTable.GetSize(); i++ )
        {
          PIPSocket::Address addr = ifTable[i].GetAddress();
          PStringStream wildCard;
          wildCard << "[sip:*" << addr.AsString() << "*]";

          PString iface =  addr.AsString();
          
          for( PINDEX i = 0; i < ScriptingHost::m_B2BPorts.GetSize(); i++ )
          {
            wildCard << " sip:" << iface << ":" << ScriptingHost::m_B2BPorts[i] << ";method=\"INVITE\"";
            if( i < ScriptingHost::m_B2BPorts.GetSize() - 1 )
              wildCard << " | ";
          }

          ScriptingHost sh;
          sh.AddRelayRoute( 
            static_cast<const char *>(wildCard),
            static_cast<const char *>(ScriptingHost::m_ProxyXMLRPCAddress) );

          wildCard = PString::Empty();

          for( PINDEX i = 0; i < ScriptingHost::m_B2BPorts.GetSize(); i++ )
          {
            wildCard << " sip:" << iface << ":" << ScriptingHost::m_B2BPorts[i] << ";method=\"OPTIONS\"";
            if( i < ScriptingHost::m_B2BPorts.GetSize() - 1 )
              wildCard << " | ";
          }

          sh.AddRelayRoute( 
            static_cast<const char *>(wildCard),
            static_cast<const char *>(ScriptingHost::m_ProxyXMLRPCAddress) );
        } 
      }
    }else
    {
      PStringStream wildCard;
        wildCard << "[sip:*" << iface << "*]";
      
      for( PINDEX i = 0; i < ScriptingHost::m_B2BPorts.GetSize(); i++ )
      {
        wildCard << " sip:" << iface << ":" << ScriptingHost::m_B2BPorts[i];
        if( i < ScriptingHost::m_B2BPorts.GetSize() - 1 )
          wildCard << " | ";
      }
      ScriptingHost sh;
      sh.AddRelayRoute( 
        static_cast<const char *>(wildCard),
        static_cast<const char *>(ScriptingHost::m_ProxyXMLRPCAddress) );
    }
    

    PFilePath lauchScript( "scripts/domains.script" );
    SquirrelObject sq = SquirrelVM::CompileScript( lauchScript );

    try {
      SquirrelVM::RunScript(sq);
    } catch (SquirrelError & e) {
      scprintf(_T("Error: %s, %s\n"),e.desc,_T("proxy_startup.script"));
    } // catch

    PStringToString * immediateResponse = new PStringToString();
    immediateResponse->SetAt( "ResponseCode", "0" );
    immediateResponse->SetAt( "ResponseMessage", "Ok" );
    response.SetAt( 0, immediateResponse );

    
    PStringToString * cmdParams = dynamic_cast<PStringToString *>(request.GetAt(0));
    if( cmdParams != NULL )
    {
      PString * iid = cmdParams->GetAt( "iid" );
      if( iid != NULL )
        PTRACE( 1, *iid << " has gone up" );
    }

  }else if( method == "B2BUA.Startup" )
  {
    PStringToString * cmdParams = dynamic_cast<PStringToString *>(request.GetAt(0));
    if( cmdParams != NULL )
    {
      PString * iid = cmdParams->GetAt( "iid" );
      if( iid != NULL )
        PTRACE( 1, *iid << " has gone up" );
    }

    ///do some stuffs here
    PStringToString * immediateResponse = new PStringToString();
    immediateResponse->SetAt( "ResponseCode", "0" );
    immediateResponse->SetAt( "ResponseMessage", "Ok" );
    response.SetAt( 0, immediateResponse );
  }
}



///////////////////////////////////////////////////////////////////////////////

static void SpliceMacro(PString & text, const PString & token, const PString & value)
{
  PRegularExpression RegEx("<?!--#status[ \t\r\n]+" + token + "[ \t\r\n]*-->?",
                           PRegularExpression::Extended|PRegularExpression::IgnoreCase);
  PINDEX pos, len;
  while (text.FindRegEx(RegEx, pos, len))
    text.Splice(value, pos, len);
}


PCREATE_SERVICE_MACRO_BLOCK(SBCRegInfo,P_EMPTY,P_EMPTY,htmlBlock)
{
  SBCLaunchPad * app = dynamic_cast<SBCLaunchPad *>( OSSApplication::GetInstance() );
  return app->OnLoadRegistrationStatus( htmlBlock );
}

///////////////////////////////////////////////////////////////////////////////

SBCRegistrationStatusPage::SBCRegistrationStatusPage(SBCLaunchPad & _app, PHTTPAuthority & auth)
  : PServiceHTTPString("OpenSBC Registration Status", "", "text/html; charset=UTF-8", auth),
    m_SBCLaunchPad(_app)
{
  
  PHTML html;

  html << PHTML::Title("OpenSBC Registration Status")
       << "<meta http-equiv=\"Refresh\" content=\"30\">\n"
       << PHTML::Body()
       << m_SBCLaunchPad.GetPageGraphic()

       << PHTML::Paragraph()
       << PHTML::HotLink("OpenSBC Registration Status") << "Reload page" << PHTML::HotLink()
       << "&nbsp;&nbsp;&nbsp;&nbsp;"
       << PHTML::HotLink("/") << "Home page" << PHTML::HotLink()

       << PHTML::Paragraph() /*<< "<center>"*/
       << PHTML::Form("POST")
       << PHTML::TableStart("border=1")
       << PHTML::TableRow()
       << PHTML::TableHeader()
       << "&nbsp;URI&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;AOR&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;Expires&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;Call-ID&nbsp;"
       << "<!--#macrostart SBCRegInfo-->"
         << PHTML::TableRow()
         << PHTML::TableData()
         << "<!--#status URI-->"
         << PHTML::TableData()
         << "<!--#status AOR-->"
         << PHTML::TableData()
         << "<!--#status Expires-->"
         << PHTML::TableData()
         << "<!--#status Call-ID-->"
         << PHTML::TableData()
         << PHTML::SubmitButton("Unregister", "!--#status Call-ID--")
       << "<!--#macroend SBCRegInfo-->"
       << PHTML::TableEnd()

       
       << PHTML::Form()
       << PHTML::HRule()

       << m_SBCLaunchPad.GetCopyrightText()
       << PHTML::Body();

  string = html;

}

PString SBCLaunchPad::OnLoadRegistrationStatus( 
  const PString & htmlBlock 
)
{
  PString substitution;

  PArray<PStringToString> request;
  request.SetAt(0, new PStringToString());
  PArray<PStringToString> response;
  
  PURL url = ScriptingHost::m_ProxyXMLRPCAddress;
  SendCustomXMLRPCRequest( "Registrar.GetRegStatus", request, response, url );
  
  for( PINDEX i = 1; i < response.GetSize(); i++ )
  {
    PStringToString * cacheData = static_cast<PStringToString*>(response.GetAt(i));
    if( cacheData != NULL )
    {
      PString insert = htmlBlock;

      PString * userURI = cacheData->GetAt( "URI" );
      PString * aor = cacheData->GetAt( "AOR" );
      PString * expires = cacheData->GetAt( "Expires" );
      PString * callId = cacheData->GetAt( "Call-ID" );
      
      if( userURI == NULL || aor == NULL || expires == NULL || callId == NULL )
        continue;

      SpliceMacro(insert, "URI", *userURI );
      SpliceMacro(insert, "AOR", *aor );
      SpliceMacro(insert, "Expires", *expires );
      SpliceMacro(insert, "Call-ID", *callId );
      substitution+=insert;
    }
  }

  return substitution;
}

BOOL SBCRegistrationStatusPage::Post(PHTTPRequest & request,
                          const PStringToString & data,
                          PHTML & msg)
{
  
  PTRACE(2, "Main\tClear call POST received " << data);

  msg << PHTML::Title() << "Accepted Control Command" << PHTML::Body()
      << PHTML::Heading(1) << "Accepted Control Command" << PHTML::Heading(1);

  m_SBCLaunchPad.OnPostControl(data, msg);


  msg << PHTML::Paragraph()
      << PHTML::HotLink(request.url.AsString()) << "Reload page" << PHTML::HotLink()
      << "&nbsp;&nbsp;&nbsp;&nbsp;"
      << PHTML::HotLink("/") << "Home page" << PHTML::HotLink();

  PServiceHTML::ProcessMacros(request, msg, "html/status.html",
                              PServiceHTML::LoadFromFile|PServiceHTML::NoSignatureForFile);
                              
  return TRUE;
}

////////////////////////////////////////////////////////////////////////////

PCREATE_SERVICE_MACRO_BLOCK(SBCAppInfo,P_EMPTY,P_EMPTY,htmlBlock)
{
  SBCLaunchPad * app = dynamic_cast<SBCLaunchPad *>( OSSApplication::GetInstance() );
  return app->OnLoadApplicationStatus( htmlBlock );
}

ProcessStatusPage::ProcessStatusPage(
  SBCLaunchPad & app, PHTTPAuthority & auth
  ) : PServiceHTTPString("OpenSBC Application Status", "", "text/html; charset=UTF-8", auth),
      m_SBCLaunchPad( app )
{
  PHTML html;

  html << HTMLString( app );
#if 0
  html << PHTML::Title("OpenSBC Application Status")
       << "<meta http-equiv=\"Refresh\" content=\"30\">\n"
       << PHTML::Body()
       << m_SBCLaunchPad.GetPageGraphic()

       << PHTML::Paragraph()
       << PHTML::HotLink("OpenSBC Application Status") << "Reload page" << PHTML::HotLink()
       << "&nbsp;&nbsp;&nbsp;&nbsp;"
       << PHTML::HotLink("/") << "Home page" << PHTML::HotLink()

       << PHTML::Paragraph() /*<< "<center>"*/
       << PHTML::Form("POST")
       << PHTML::TableStart("border=1")
       << PHTML::TableRow()
       << PHTML::TableHeader()
       << "&nbsp;ID&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;PATH&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;PID&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;Time Stamp&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;STATUS&nbsp;"
       << "<!--#macrostart SBCAppInfo-->"
         << PHTML::TableRow()
         << PHTML::TableData()
         << "<!--#status ID-->"
         << PHTML::TableData()
         << "<!--#status PATH-->"
         << PHTML::TableData()
         << "<!--#status PID-->"
         << PHTML::TableData()
         << "<!--#status TIME STAMP-->"
         << PHTML::TableData()
         << "<!--#status STATUS-->"
         << PHTML::TableData()
         << PHTML::SubmitButton("Start", "!--#status ID--")
         << PHTML::SubmitButton("Stop", "!--#status ID--")
       << "<!--#macroend SBCAppInfo-->"
       << PHTML::TableEnd()

       
       << PHTML::Form()
       << PHTML::HRule()

       << m_SBCLaunchPad.GetCopyrightText()
       << PHTML::Body();
#endif

  string = html;
}

PString ProcessStatusPage::HTMLString( SBCLaunchPad & app )
{
  PHTML html;

  html << PHTML::Title("OpenSBC Application Status")
       << "<meta http-equiv=\"Refresh\" content=\"30\">\n"
       << PHTML::Body()
       << app.GetPageGraphic()

       << PHTML::Paragraph()
       << PHTML::HotLink("OpenSBC Application Status") << "Reload page" << PHTML::HotLink()
       << "&nbsp;&nbsp;&nbsp;&nbsp;"
       << PHTML::HotLink("/") << "Home page" << PHTML::HotLink()

       << PHTML::Paragraph() /*<< "<center>"*/
       << PHTML::Form("POST")
       << PHTML::TableStart("border=1")
       << PHTML::TableRow()
       << PHTML::TableHeader()
       << "&nbsp;ID&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;PATH&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;PID&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;Time Stamp&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;STATUS&nbsp;"
       << "<!--#macrostart SBCAppInfo-->"
         << PHTML::TableRow()
         << PHTML::TableData()
         << "<!--#status ID-->"
         << PHTML::TableData()
         << "<!--#status PATH-->"
         << PHTML::TableData()
         << "<!--#status PID-->"
         << PHTML::TableData()
         << "<!--#status TIME STAMP-->"
         << PHTML::TableData()
         << "<!--#status STATUS-->"
         << PHTML::TableData()
         << PHTML::SubmitButton("Start", "!--#status ID--")
         << PHTML::SubmitButton("Stop", "!--#status ID--")
       << "<!--#macroend SBCAppInfo-->"
       << PHTML::TableEnd()

       
       << PHTML::Form()
       << PHTML::HRule()

       << app.GetCopyrightText()
       << PHTML::Body();

  return html;
}

PString SBCLaunchPad::OnLoadApplicationStatus( 
  const PString & htmlBlock 
)
{
  return m_ProcessMonitor->OnLoadApplicationStatus( htmlBlock );
}

BOOL ProcessStatusPage::Post(
  PHTTPRequest & request,
  const PStringToString & data,
  PHTML & msg
)
{
  PTRACE(2, "Main\tClear call POST received " << data);


  //msg << PHTML::Title() << "Accepted Control Command" << PHTML::Body()
  //    << PHTML::Heading(1) << "Accepted Control Command" << PHTML::Heading(1);

  m_SBCLaunchPad.OnPostControl(data, msg);


  //msg << PHTML::Paragraph()
 //     << PHTML::HotLink(request.url.AsString()) << "Reload page" << PHTML::HotLink()
  //    << "&nbsp;&nbsp;&nbsp;&nbsp;"
  //    << PHTML::HotLink("/") << "Home page" << PHTML::HotLink();

  PServiceHTML::ProcessMacros(request, msg, "html/status.html",
                              PServiceHTML::LoadFromFile|PServiceHTML::NoSignatureForFile);
                              
  return TRUE;
}

BOOL SBCLaunchPad::OnPostControl(
  const PStringToString & data, 
  PHTML & msg
)
{
  BOOL ok = FALSE;

  PString key = data.GetKeyAt(0);
  PString command = data.GetDataAt(0);

  if (command == "Unregister") 
  {
    msg << PHTML::Heading(2) << UnregisterSession( key, ScriptingHost::m_ProxyXMLRPCAddress ) << PHTML::Heading(2);
  }else if( command == "Start" || command == "Stop" )
  {
    if( command == "Start" )
    {
      m_ProcessMonitor->AddProcessFromLimbo( key );
    }else if( command == "Stop" )
    {
      m_ProcessMonitor->RemoveProcess( key, TRUE, TRUE );
    }

    msg << ProcessStatusPage::HTMLString( *this );
  }


  return ok;
}





