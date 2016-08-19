/*
 *
 * SQLiteDb.cxx
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
 * $Log: OSSApplication.cxx,v $
 * Revision 1.22  2007/01/17 00:12:19  joegenbaclor
 * 1.  Fixed bug in call session where CANCEL may not be sent if a call ahs been authenticated due to a misplaced SetState( StateDisconnected ) for 4xx events
 * 2.  Fixed register session to ignore authorization headers if allow all reg is flagged
 * 3.  Temporary hacks for syslog
 * 4.  More launcher code -  almost there!
 *
 * Revision 1.21  2007/01/12 10:49:10  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.20  2007/01/12 00:31:02  joegenbaclor
 * launcher related code
 *
 * Revision 1.19  2007/01/10 23:57:54  joegenbaclor
 * launcher specific code
 *
 * Revision 1.18  2007/01/08 11:15:51  joegenbaclor
 * Added capability to add status pages
 *
 * Revision 1.17  2007/01/04 09:36:01  joegenbaclor
 * Added XML RPC Configuration Capability
 *
 * Revision 1.16  2007/01/03 05:27:10  joegenbaclor
 * Added PID file generation for windows daemons
 *
 * Revision 1.15  2006/12/23 12:33:22  joegenbaclor
 * Made sure logs directory is created in the same directory as the executable
 *
 * Revision 1.14  2006/12/22 07:37:21  joegenbaclor
 * Introduced separate funtion call  for starting of transport thread.  This is necessary
 *  to make sure that the backdoor listener is already initialized prior to calling transport
 *  reads
 *
 * Revision 1.13  2006/12/21 18:10:09  joegenbaclor
 * Allowed setting of HTTP service port via command line.  More changes to follow ...
 *
 * Revision 1.12  2006/12/21 10:49:31  joegenbaclor
 * Moved creation of logs directory in OSSApplication constructor
 *
 * Revision 1.11  2006/12/21 08:59:43  joegenbaclor
 * Corrected typos in last commit
 *
 * Revision 1.10  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.9  2006/11/22 11:22:36  rcolobong
 * 1. Update "Configuration Page" name to "HTTP Admin"
 * 2. Remove Log Level configuration
 *
 * Revision 1.8  2006/11/17 04:22:22  joegenbaclor
 * Added ability to remember core to use via m_UACoreName
 *
 * Revision 1.7  2006/08/29 10:47:23  rcolobong
 * *** empty log message ***
 *
 * Revision 1.6  2006/07/11 13:59:36  joegenbaclor
 * Completed Proxy functionalities for B2BUA
 *
 * Revision 1.5  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
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
 * Revision 1.1  2006/05/31 07:21:52  joegenbaclor
 * 1.  Added OSSApplication and StringTuple classes.
 * 2.  Corrected some typos
 *
 */

#include "OSSApplication.h"
#include "../version.h"

#if P_EXPAT

#define new PNEW

using namespace Tools;
/////////////////////////////////////////////////////////////////////////
#define	PRODUCT_NAME_TEXT	"OpenSIPStack Application"
#define	EXE_NAME_TEXT	    "OpenSIPStack Application"
#define	MANUFACTURER_TEXT	"opensipstack.org"
#define	COPYRIGHT_HOLDER	MANUFACTURER_TEXT

#ifndef GIF_NAME
#define GIF_NAME  		    "banner.jpg"
#define	GIF_WIDTH  380
#define GIF_HEIGHT 101
#endif

#define	EMAIL "info@opensipstack.org"
#define	HOME_PAGE "http://www.opensipstack.org"
#define	PRODUCT_NAME_HTML PRODUCT_NAME_TEXT
#define DEFAULT_LOG_DIR "logs"

PHTTPServiceProcess::Info OSSAppInfo = {
    PRODUCT_NAME_TEXT,
    MANUFACTURER_TEXT,
    MAJOR_VERSION, MINOR_VERSION, PProcess::BUILD_TYPE, BUILD_NUMBER, __TIME__ __DATE__,
    
    {{ 0 }}, { NULL }, 0, {{ 0 }},  // Only relevent for commercial apps

    HOME_PAGE,
    EMAIL,
    PRODUCT_NAME_HTML,
    NULL,  // GIF HTML, use calculated from below
    GIF_NAME,
    GIF_WIDTH,
    GIF_HEIGHT,

    MANUFACTURER_TEXT,
    HOME_PAGE,
    EMAIL

};

/////////////////////////////////////////////////////////////////////////
OSSApplication * OSSApplication::m_Instance = NULL;
OSSApplication * OSSApplication::GetInstance()
{
  return OSSApplication::m_Instance;
}

OSSApplication::OSSApplication()
  : OSSApplicationAncestor( OSSAppInfo )
{
  m_FirstRun = TRUE;
 
  m_LogDirectory = GetFile().GetDirectory() + DEFAULT_LOG_DIR;
  
  PDirectory logDir( m_LogDirectory );

  if( !logDir.Exists() )
    logDir.Create();
}

void OSSApplication::Main()
{
  Suspend();
}

PString OSSApplication::GetConfigurationFilePath()const
{
  PStringStream f;
  f << "oss-application" << ".conf" << ".xml";
  return f;
}

BOOL OSSApplication::OnStart()
{
  OSSApplication::m_Instance = this;

  PTRACE( 1, GetName() << " Process is starting" );
  // change to the default directory to the one containing the executable
  PDirectory exeDir = GetFile().GetDirectory();

#if defined(_WIN32) && defined(_DEBUG)
  // Special check to aid in using DevStudio for debugging.
  if (exeDir.Find("\\Debug\\") != P_MAX_INDEX)
    exeDir = exeDir.GetParent();
#endif
  exeDir.Change();

  httpNameSpace.AddResource(new PHTTPDirectory("data", "data"));
  httpNameSpace.AddResource(new PServiceHTTPDirectory("html", "html"));

  // parse arguments so we can grab what we want
  PArgList & args = GetArguments();
  
  args.Parse("v-version."
             "d-daemon."
             "c-console."
             "h-help."
             "x-execute."
             "p-pid-file:"
             "H-handlemax:"
             "i-ini-file:"
             "k-kill."
             "t-terminate."
             "s-status."
             "l-log-file:"
             "u-uid:"
             "g-gid:"
             "C-core-size:"
             "D-debug."
             "P-http-port:"
             "X-misc-param-1:"
             "Y-misc-param-2:"
             "Z-misc-param-3:");

  if( args.HasOption( 'X' ) )
  {
    OnHandleCommandArgs( 0, args.GetOptionString( 'X' ) );
  }

  if( args.HasOption( 'Y' ) )
  {
    OnHandleCommandArgs( 0, args.GetOptionString( 'Y' ) );
  }

  if( args.HasOption( 'Z' ) )
  {
    OnHandleCommandArgs( 0, args.GetOptionString( 'Z' ) );
  }

  if(  !OSSAppConfig::GetInstance()->Initialize( GetConfigurationFilePath() ) )
  {
    PTRACE( 1, "Unable to Initialize HTTP Server" );
    return FALSE;
  }

  OnStart( *(OSSAppConfig::GetInstance()) );

  return PHTTPServiceProcess::OnStart();
}

void OSSApplication::OnStop()
{
  return PHTTPServiceProcess::OnStop();
}

void OSSApplication::OnControl()
{
  PStringStream url;
  url << "http://";

  PString host = PIPSocket::GetHostName();
  PIPSocket::Address addr;
  if (PIPSocket::GetHostAddress(host, addr))
    url << host;
  else
    url << "localhost";

  url << ':' << OSSAppConfig::GetInstance()->GetHTTPPort();

  PURL::OpenBrowser(url);
}

void OSSApplication::OnConfigChanged()
{
  OnConfigChanged( *(OSSAppConfig::GetInstance()) );
}

BOOL OSSApplication::Initialise(
  const char * initMsg
)
{
  if(  !m_FirstRun && !OSSAppConfig::GetInstance()->Initialize( GetConfigurationFilePath() ) )
  {
    PTRACE( 1, "Unable to Initialize HTTP Server" );
    return FALSE;
  }

  // parse arguments so we can grab what we want
  PArgList & args = GetArguments();
  
  args.Parse("v-version."
             "d-daemon."
             "c-console."
             "h-help."
             "x-execute."
             "p-pid-file:"
             "H-handlemax:"
             "i-ini-file:"
             "k-kill."
             "t-terminate."
             "s-status."
             "l-log-file:"
             "u-uid:"
             "g-gid:"
             "C-core-size:"
             "D-debug."
             "P-http-port:"
             "X-misc-param-1:"
             "Y-misc-param-2:"
             "Z-misc-param-3:");

  if( args.HasOption( 'p' ) )
  {
    /// we have a pidfile
    PTextFile pid( args.GetOptionString( 'p', "pidfile" ), PFile::ReadWrite );
    pid.WriteLine( PString( GetProcessID() ) );
  }

  if( args.HasOption( 'P' ) )
  {
    PString httpPort = args.GetOptionString( 'P', PString( OSSAppConfig::GetInstance()->GetHTTPPort() ) );
    if( !httpPort.IsEmpty() )
      OSSAppConfig::GetInstance()->SetHTTPPort( static_cast<WORD>(httpPort.AsUnsigned()) );
  }

  if( OSSAppConfig::GetInstance()->GetType() != OSSAppConfig::XMLRPCClientProvider  &&  m_FirstRun  )
  {
    if( !StartHTTPService() )
      return FALSE;
    else
      PTRACE(1, "Opened master socket for HTTP: " << OSSAppConfig::GetInstance()->GetHTTPPort() );
  }
  
  m_FirstRun= FALSE;

  return TRUE;
}

BOOL OSSApplication::StartHTTPService()
{
  return ListenForHTTP( OSSAppConfig::GetInstance()->GetHTTPPort() );
}

OSSAppConfig * OSSApplication::OnCreateAppConfig()
{
  return new OSSAppConfig( OSSAppConfig::HTTPDefaultProvider, 9999 );
}

void OSSApplication::OnAdditionalHTTPResource(
  PHTTPSimpleAuth & authority
)
{
}

void OSSApplication::OnAdditionalHTTPLinks( 
  PHTML & html 
)
{
}

void OSSApplication::OnAdditionalXMLRPCMethods(
  PStringArray & moreMethods
)
{
}

void OSSApplication::OnCustomXMLRPCRequest(
  const PString & method,
  PArray<PStringToString> & request,
  PArray<PStringToString> & response
)
{
  PStringToString * immediateResponse = static_cast<PStringToString*>(response.GetAt( 0 ));
  PAssertNULL( immediateResponse );
  immediateResponse->SetAt( "ResponseCode", "9999" );
  immediateResponse->SetAt( "ResponseMessage", "Unsupported Command" );
}

BOOL OSSApplication::SendCustomXMLRPCRequest(
  const PString & method,
  PArray<PStringToString> & request,
  PArray<PStringToString> & immediateResponse,
   const PURL & url
)
{
  OSSAppConfig::GetInstance()->SendRequest( method, request, immediateResponse, url );
  return TRUE;
}

#endif //P_EXPAT


