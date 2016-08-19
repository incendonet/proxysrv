/*
 *
 * OpenSBC.cxx
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
 * $Log: OpenSBC.cxx,v $
 * Revision 1.31  2007/01/22 10:00:58  joegenbaclor
 * Fixed ProxyRouteRequest
 *
 * Revision 1.30  2007/01/18 08:39:27  joegenbaclor
 * Added VC7.10 project files for launcher
 *
 * Revision 1.29  2007/01/18 06:06:41  joegenbaclor
 * Incremented Build Number for latest development release
 *
 * Revision 1.28  2007/01/17 10:33:43  joegenbaclor
 * Redesigned syslog
 *
 * Revision 1.27  2007/01/17 00:04:31  joegenbaclor
 * Added SysLog
 *
 * Revision 1.26  2007/01/15 10:06:57  joegenbaclor
 * More on XML-RPC Registration Queries
 *
 * Revision 1.25  2007/01/15 09:24:47  joegenbaclor
 * Added capability to find Registraton via XML-RPC
 *
 * Revision 1.24  2007/01/14 13:15:09  joegenbaclor
 * Fixed bug were mode is not initialized properly for configuration file
 *
 * Revision 1.23  2007/01/12 10:50:04  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.22  2007/01/12 01:02:44  joegenbaclor
 * launcher related code
 *
 * Revision 1.21  2007/01/11 10:00:26  joegenbaclor
 * more launcher code
 *
 * Revision 1.20  2007/01/11 02:34:58  rcolobong
 * Add new http config for enable/disable proxying media
 *
 * Revision 1.19  2007/01/10 23:58:23  joegenbaclor
 * launcher specific code
 *
 * Revision 1.18  2007/01/10 05:00:39  joegenbaclor
 * Added B2BUpperReg mode.
 *
 * Revision 1.17  2007/01/09 02:29:44  joegenbaclor
 * Fixed Unregister Post code
 *
 * Revision 1.16  2007/01/08 11:15:18  joegenbaclor
 * Added registration status page
 *
 * Revision 1.15  2007/01/08 07:13:23  joegenbaclor
 * Added ability to run SBC in pure proxy or pure b2b modes
 *
 * Revision 1.14  2007/01/02 01:54:48  joegenbaclor
 * Corrected parser bug for Route Records
 *
 * Revision 1.13  2006/12/22 12:05:42  joegenbaclor
 * We need to include all params in arg.parse in daemon constructor or unix implementation would break.  It seems Parse of PargList can only be called one.  Second call the Parse wont update the params.
 *
 * Revision 1.12  2006/12/22 07:40:27  joegenbaclor
 * More on command line argument parsing.
 *
 * Revision 1.11  2006/12/21 08:56:59  joegenbaclor
 * Added pid in b2bua log file name
 * Fixed bug in proxy relay where startline URI is not rewritten if a static route is
 *  not found
 *
 * Revision 1.10  2006/11/25 02:59:13  joegenbaclor
 * Added project files to opensbc.  Old scheme of producing the VC 8.0 workspace using configure is still available
 *
 * Revision 1.9  2006/11/24 10:59:55  rcolobong
 * Remove support for RTBE Classes
 *
 * Revision 1.8  2006/11/22 11:37:09  rcolobong
 * 1. Added new support for Force B2BUA in HTTP configuration
 * 2. Added log level in Application level and SIP/RTP Level
 * 3. Update OnAcceptRegistration signature
 *
 * Revision 1.7  2006/10/11 04:59:50  rcolobong
 * Add http configuration "Accept all registration". If this true then it will accept all registration even if it is not included on the "Local domain accounts"
 *
 * Revision 1.6  2006/09/04 14:39:07  joegenbaclor
 * Corrected bug where logger prefix defaults to b2bua--
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
 * Revision 1.10  2006/08/04 07:30:52  joegenbaclor
 * #ifdefed RTBE headers
 *
 * Revision 1.9  2006/08/04 05:44:18  rcolobong
 * Instantiate B2BRTBEUA instead of openB2BUA if HAS_RTBECLIENT is defined
 *
 * Revision 1.8  2006/07/26 09:19:41  joegenbaclor
 * Added System Log support
 *
 * Revision 1.7  2006/07/17 11:36:44  joegenbaclor
 * 1.  More routing enhancements to B2BUA
 *
 * Revision 1.6  2006/07/11 13:59:34  joegenbaclor
 * Completed Proxy functionalities for B2BUA
 *
 * Revision 1.5  2006/07/10 06:29:37  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.4  2006/07/03 15:29:47  joegenbaclor
 * Started merging OpenB2BUA into OpenSBC
 *
 * Revision 1.3  2006/06/29 00:50:25  joegenbaclor
 * Corrected some typos that makes unix compilation barf
 *
 * Revision 1.2  2006/06/20 09:58:11  joegenbaclor
 * *** empty log message ***
 *
 *
 */


#include "OpenSBC.h"
#include "SBCRoutingHandler.h"
#include "SBCCallHandler.h"
#include "SBCAuthHandler.h"
#include "Version.h"

#define new PNEW

static const char configKeySection[] = "ProxySrv General Parameters";
static const char configKeyInterfaceAddress[] = "Interface Address";
static const char configKeyTransactionThreadCount[] = "Transaction Thread Count";
static const char configKeySessionThreadCount[] = "Session Thread Count";
static const char configKeyAppLogLevel[] = "Application Log Level";
static const char configKeyCoreLogLevel[] = "SIP and RTP Log Level";
static const char configKeySysLogServer[] = "Syslog Server";
static const char configKeyConnectionTimeout[] = "Connection Timeout";
static const char configKeyAlertingTimeout[] = "Alerting Timeout";
static const char configKeyAppLogFilePrefix[] = "Application Log File";
static const char configKeySIPSessionTimer[] = "SIP Session Timer";
static const char configKeyActiveSessions[] = "Active Sessions";
static const char configKeyHashKey1[] = "Hash Key 1";
static const char configKeyHashKey2[] = "Hash Key 2";
static const char configKeySBCMode[] = "SBC Mode";
static const char configKeyMediaProxy[] = "Always Proxy Media";

static const char configKeyAccountSection[] = "Local Domain Accounts";
static const char configKeyAcceptAllRegistration[] = "Accept All Registration";

static const char configKeyB2BUASection[] = "Force B2BUA Routes";
static const char configKeyB2BUARoutes[] = "Route";

OpenSBCDaemon::OpenSBCDaemon()
{
  m_IsInitialized = FALSE;
  m_HasExtendedArgs = FALSE;
  m_SBC = NULL;
  m_EndPoint = NULL;
  m_Registrar = NULL;
  m_Proxy = NULL;

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

  PStringStream prodName;
  prodName << "OpenSBC";

  if( args.HasOption( 'P' ) )
    prodName << " [Port:" << args.GetOptionString( 'P' ) << "]";

  SetProductName( prodName );
  SetMajorVersion( SBC_MAJOR_VERSION );
  SetMinorVersion( SBC_MINOR_VERSION );
  SetBuildNumber( SBC_BUILD_NUMBER );

}

OpenSBCDaemon::~OpenSBCDaemon()
{
  if( m_SBC != NULL )
    delete m_SBC;

  if( m_EndPoint != NULL )
    delete m_EndPoint;

  if( m_Registrar != NULL )
    delete m_Registrar;

  if( m_Proxy != NULL )
    delete m_Proxy;
}

// pure virtual from OSSApplication
void OpenSBCDaemon::OnStart( 
  OSSAppConfig & config 
)
{
  if( HasExtendedArgs() )  
    m_InstanceId = m_ExtendedArgs.GetParameter( "iid" );

  int logLevel = config.GetInteger( configKeySection, configKeyAppLogLevel, 3 );
  PTRACE( 1, "Setting Log Level to " << logLevel );
  
  PString logPrefix = GetLogDirectory() + "/" + config.GetString( 
    configKeySection, configKeyAppLogFilePrefix, "b2bua" ) ;

  if( !m_InstanceId.IsEmpty() )
    logPrefix = GetLogDirectory() + "/" + m_InstanceId;
  
  if( m_SBC == NULL )
  {
    PString mode = m_ExtendedArgs.GetParameter( "ua-mode" );
  
    if( mode.IsEmpty() )
      mode = config.GetString( configKeySection, configKeySBCMode, "Full Mode" );

    B2BUserAgent::UAMode uaMode = B2BUserAgent::FullMode;

    if( mode == "Full Mode" || mode == "FullMode" )
      uaMode = B2BUserAgent::FullMode;
    else if( mode == "Proxy Only Mode" || mode == "ProxyOnlyMode" )
      uaMode = B2BUserAgent::ProxyOnlyMode;
    else if( mode == "B2B Only Mode" || mode == "B2BOnlyMode" )
      uaMode = B2BUserAgent::B2BOnlyMode;
    else if( mode == "B2BUpperReg Mode" || mode == "B2BUpperRegMode" )
      uaMode = B2BUserAgent::B2BUpperRegMode;

    if( ( uaMode == B2BUserAgent::ProxyOnlyMode || 
          uaMode == B2BUserAgent::B2BOnlyMode ) &&
          config.GetType() == OSSAppConfig::XMLRPCClientProvider )
    {
      PString addr = m_ExtendedArgs.GetParameter( "syslog" );
      if( addr.IsEmpty() )
        addr = "127.0.0.1:514";
      
      SIPURI sysLogAddress( addr );
      Logger::SetDefaultLogStream( 
        new LoggingSystemLogStream( 
          m_InstanceId, 
          PIPSocket::Address( "127.0.0.1"  ),
          sysLogAddress.GetAddress(),
          (WORD)sysLogAddress.GetPort().AsInteger(),
          TRUE ) );

    }else
    {
      Logger::SetDefaultLogStream( new LoggingIncrementingFileStream( logPrefix, PString( GetProcessID() ) ) );
    }

    m_SBC = new OpenSBC( this, uaMode );

    if( uaMode == B2BUserAgent::FullMode )
    {
      PTRACE( 1, "Running in Full Mode" );
    }else if( uaMode == B2BUserAgent::ProxyOnlyMode )
    {
      PTRACE( 1, "Running in Proxy Mode" );
    }else if( uaMode == B2BUserAgent::B2BOnlyMode )
    {
      PTRACE( 1, "Running in Back To Back Mode" );
    }else if( uaMode == B2BUserAgent::B2BUpperRegMode )
    {
      PTRACE( 1, "Running in Back To Back with Upper Registration Mode" );
    }

    if( m_EndPoint == NULL )
      m_EndPoint = new B2BUAEndPoint( *m_SBC, config.GetInteger( configKeySection, configKeySessionThreadCount, 10 ), 1024 * 2 );
      
    m_SBC->SetEndPoint( m_EndPoint );

    if( m_Registrar == NULL )
      m_Registrar = new B2BUserAgent::Registrar( *m_SBC, config.GetInteger( configKeySection, configKeySessionThreadCount, 10 ), 1024 * 2 );

    m_SBC->SetRegistrar( m_Registrar );

    if( m_Proxy == NULL )
      m_Proxy = new B2BUserAgent::Proxy( *m_SBC, config.GetInteger( configKeySection, configKeySessionThreadCount, 10 ), 1024 * 2 );

    m_SBC->SetProxy( m_Proxy );

    m_SBC->OnStart( config );

  }


  PTRACE( 1, "OpenSBC STARTED " << m_InstanceId ); 
}

void OpenSBCDaemon::OnStop()
{
  m_SBC->OnStop();
  
  if( m_SBC != NULL )
  {
    m_SBC = NULL;
    delete m_SBC;
  }

  if( m_EndPoint != NULL )
  {
    m_EndPoint->GetUserAgent().Terminate();
    delete m_EndPoint;
    m_EndPoint = NULL;
  }

  if( m_Registrar != NULL )
  {
    delete m_Registrar;
    m_Registrar = NULL;
  }

  if( m_Proxy != NULL )
  {
    delete m_Proxy;
    m_Proxy = NULL;
  }

  OSSApplication::OnStop();
}

OSSAppConfig * OpenSBCDaemon::OnCreateAppConfig()
{
  PString configMode = m_ExtendedArgs.GetParameter( "config-mode" );
  PString xmlRPCPort =  m_ExtendedArgs.GetParameter( "xml-rpc-port" );
  PString xmlRPCServer = m_ExtendedArgs.GetParameter( "xml-rpc-server" );
  PString instanceId = m_ExtendedArgs.GetParameter( "iid" );

  WORD port = static_cast<WORD>(xmlRPCPort.IsEmpty() ? 10000 : xmlRPCPort.AsInteger());
  if( configMode.IsEmpty() )
  {
    return new OSSAppConfig( OSSAppConfig::HTTPDefaultProvider, port );
  }else
  {
    if( (configMode *= "XMLRPCClientProvider") && !xmlRPCServer.IsEmpty() )
    {
      return new OSSAppConfig( OSSAppConfig::XMLRPCClientProvider, port, xmlRPCServer, instanceId );
    }else if( configMode *= "XMLRPCAndHTTPServerProvider" )
    {
      return new OSSAppConfig( OSSAppConfig::XMLRPCAndHTTPServerProvider, port );
    }else if( configMode *= "XMLRPCServerProviderOnly" )
    {
      return new OSSAppConfig( OSSAppConfig::XMLRPCServerProviderOnly, port );
    }
  }

  return new OSSAppConfig( OSSAppConfig::HTTPDefaultProvider, port );
}

void OpenSBCDaemon::OnConfigChanged( 
  OSSAppConfig & config 
)
{
  PThread::Create( 
      PCREATE_NOTIFIER( 
      OnConfigChanged ), 
      (INT)&config, 
      PThread::AutoDeleteThread,
      PThread::NormalPriority,
      "OnConfigChanged"  );

}

void OpenSBCDaemon::OnConfigChanged( PThread & thrd, INT data )
{
  OSSAppConfig * _config = reinterpret_cast<OSSAppConfig*>(data);
  if( _config != NULL )
  {
    OSSAppConfig & config = *_config;
    PTRACE( 1, "Configuration change detected" ); 

    int logLevel = config.GetInteger( configKeySection, configKeyAppLogLevel, 3 );
    PString syslogAddress = config.GetString( configKeySection, 
      configKeySysLogServer, "127.0.0.1" );
    
    PString logPrefix = config.GetString( 
      configKeySection, configKeyAppLogFilePrefix, "b2bua" );

    if( !m_InstanceId.IsEmpty() )
      logPrefix  += "-" + m_InstanceId;

    Logger::SetDefaultLevel( logLevel );
    PTRACE( 1, "Setting Log Level to " << logLevel );
    

    SetPTraceLevel( config.GetInteger( configKeySection, configKeyCoreLogLevel, 3 ) );

    BOOL mediaProxy = config.GetBoolean( configKeySection, configKeyMediaProxy, FALSE );
    if( m_EndPoint )
      m_EndPoint->SetMediaProxyIfPrivate( !mediaProxy );

    if( m_SBC != NULL )
      m_SBC->OnConfigChanged( config );

     m_IsInitialized = TRUE;

    static BOOL firstRun = TRUE;
    if( firstRun && m_SBC->GetUAMode() == B2BUserAgent::ProxyOnlyMode && 
          config.GetType() == OSSAppConfig::XMLRPCClientProvider )
    {

      PString serverURL = GetExtendedArgs().GetParameter( "xml-rpc-server" );
      if( !serverURL.IsEmpty() )
      {
        PArray<PStringToString>  cmd;
        PArray<PStringToString>  response;
        PStringToString * cmdParams = new PStringToString();
        cmdParams->SetAt( "iid", GetExtendedArgs().GetParameter( "iid" ) );
        cmd.SetAt( 0, cmdParams );
        config.SendRequest( "Proxy.Startup", cmd, response, serverURL );
      }
    }else if( firstRun && m_SBC->GetUAMode() == B2BUserAgent::B2BOnlyMode && 
          config.GetType() == OSSAppConfig::XMLRPCClientProvider )
    {

      PString serverURL = GetExtendedArgs().GetParameter( "xml-rpc-server" );
      if( !serverURL.IsEmpty() )
      {
        PArray<PStringToString>  cmd;
        PArray<PStringToString>  response;
        PStringToString * cmdParams = new PStringToString();
        cmdParams->SetAt( "iid", GetExtendedArgs().GetParameter( "iid" ) );
        cmd.SetAt( 0, cmdParams );
        config.SendRequest( "B2BUA.Startup", cmd, response, serverURL );
      }
    }
    firstRun = FALSE;
  } 

 
}


void OpenSBCDaemon::OnHandleCommandArgs(
    PINDEX i,
    const PString & arg
)
{
  if( i == 0 )  /// 'X'
  {
    m_HasExtendedArgs = TRUE;
    m_ExtendedArgs = arg;
    PTRACE( 1, m_ExtendedArgs.AsString() );
  }
}

void OpenSBCDaemon::SetPTraceLevel( unsigned level )
{
#if PTRACING
  PTrace::SetLevel( level );
  PTrace::ClearOptions( PTrace::Timestamp );
  PTrace::SetOptions( PTrace::DateAndTime );
#endif
}

//////////////////////////////////////////////////////////////////////////////



OpenSBC::OpenSBC( 
  OpenSBCDaemon * application,
  UAMode mode
  ) : B2BUserAgent( mode )
{
  m_Application = application;
  
  m_SBCAuthHandler = NULL;
  m_SBCCallHandler = NULL;
  m_SBCRoutingHandler = NULL;
}

void OpenSBC::OnStart( 
  OSSAppConfig & 
)
{
}

void OpenSBC::OnStop()
{
  m_ActiveSessionCounter.Stop();
  m_ActiveSessionCounter.WaitForTermination( 5000 );
}

void OpenSBC::OnConfigChanged( 
  OSSAppConfig & config 
)
{
  int workerThreadCount = config.GetInteger( configKeySection, configKeyTransactionThreadCount, 10 );

  SIPURI bindAddress;
  if( !m_Application->HasExtendedArgs() )
    bindAddress = config.GetString( configKeySection, configKeyInterfaceAddress, "sip:*:5060" );
  else
    bindAddress = m_Application->GetExtendedArgs().AsString( FALSE );

  B2BUAConnection::SetConnectionTimeout( config.GetInteger( configKeySection, configKeyConnectionTimeout, 0 ) );
  B2BUAConnection::SetAlertingTimeout( config.GetInteger( configKeySection, configKeyAlertingTimeout, 0 ) );
  
  // Indicate if we will accept all registrations
  m_AcceptAllRegistration = config.GetBoolean( configKeyAccountSection, configKeyAcceptAllRegistration, 0 );
  GetRegistrar()->SetAcceptAllRegistrations( m_AcceptAllRegistration );

  // Enable Feature SIP Session Timers
  GetB2BUAEndPoint()->EnableSessionTimer( config.GetBoolean( configKeySection, configKeySIPSessionTimer, 0 ) );

  // Set encryption value
  ParserTools::SetDefaultXORHashKey1( static_cast< WORD >(
    config.GetInteger( configKeySection, configKeyHashKey1, GS_HASH_KEY_1 ) ) );
  
  ParserTools::SetDefaultXORHashKey2( static_cast< BYTE >(
    config.GetInteger( configKeySection, configKeyHashKey2, GS_HASH_KEY_2 ) ) ); 
  
  PIPSocket::Address iface = PIPSocket::Address( bindAddress.GetHost() );

  m_ForceRoutes.RemoveAll();
  for( PINDEX index = 0; index < config.GetListSize( configKeyB2BUASection, configKeyB2BUARoutes ); ++index )
    m_ForceRoutes.Append( new SIPURI( config.GetListItem( configKeyB2BUASection, configKeyB2BUARoutes, index ) ) );

  if( !iface.IsValid() )
    iface = 0;

  WORD port = 5060;
  if( bindAddress.GetPort() != 0 )
    port = (WORD)bindAddress.GetPort().AsUnsigned();

  GetDefaultProfile().GetTransportProfile().EnableUDP( iface, port  );

  Initialize( workerThreadCount );

  ((SBCRoutingHandler*)m_RoutingHandler)->RefreshStaticRoutes();

}

B2BCallInterface * OpenSBC::OnCreateCallInterface()
{
  if( m_SBCCallHandler == NULL )
    m_SBCCallHandler = new SBCCallHandler( *this );
  return m_SBCCallHandler;
}

B2BAuthInterface * OpenSBC::OnCreateAuthInterface()
{
  if( m_SBCAuthHandler == NULL  )
    m_SBCAuthHandler = new SBCAuthHandler( *this );
  return m_SBCAuthHandler;
}

B2BRoutingInterface * OpenSBC::OnCreateRoutingInterface()
{
  if( m_SBCRoutingHandler == NULL )
    m_SBCRoutingHandler = new SBCRoutingHandler( *this );
  return m_SBCRoutingHandler;
}

B2BMediaInterface * OpenSBC::OnCreateMediaInterface()
{
  return new B2BMediaInterface( *this );
}



RegisterSession::AcceptRegistrationResponse OpenSBC::OnAcceptRegistration( 
  RegisterSession &,
  const SIPMessage &
)
{
  if( m_AcceptAllRegistration )
    return RegisterSession::AcceptByOk;

  return RegisterSession::AcceptByChallenge;
}

void OpenSBC::ProcessEvent(
  SIPStackEvent * event
)
{
  if( event->GetType() == SIPStackEvent::Message )
  {
    SIPMessageArrival & msgEvent = (SIPMessageArrival&)*event;
    SIPMessage msg = msgEvent.GetMessage();

    if( msg.GetCSeqMethod() *= "INVITE" )
    {
      Via via;
      if( msg.GetViaAt( 0, via ) )
      {
        PString address = via.GetAddress();
        for( PINDEX index = 0; index < m_ForceRoutes.GetSize(); ++index )
        {
          if( m_ForceRoutes[index].GetHost() == address )
          {
            event->SetUACoreName( m_EndPoint->GetUACoreName() );
            break;
          }
        }
      }
    }
  }

  B2BUserAgent::ProcessEvent( event );
}

//////////////////////////////////////////////////////////////////////////////

OpenSBC::ActiveSessionCounter::ActiveSessionCounter()
  : PThread( 1024 * 2, PThread::NoAutoDeleteThread )
{
  Resume();
}

void OpenSBC::ActiveSessionCounter::Stop()
{
  m_SyncPoint.Signal();
}

void OpenSBC::ActiveSessionCounter::Main()
{
  while( !m_SyncPoint.Wait( 10000 ) )
  {
    OSSAppConfig::GetInstance()->SetInteger( configKeySection, configKeyActiveSessions, 
      B2BUAConnection::GetSessionCount() );
  }
}


////////////////////////////////////////////////////////////////////////////////

void OpenSBCDaemon::OnAdditionalHTTPResource(
  PHTTPSimpleAuth & authority
)
{
  httpNameSpace.AddResource(new SBCRegistrationStatusPage(*this, authority), PHTTPSpace::Overwrite);
}

void OpenSBCDaemon::OnAdditionalHTTPLinks( 
  PHTML & html 
)
{
  html << PHTML::HotLink("OpenSBC Registration Status") << "OpenSBC Registration Status" << PHTML::HotLink();
}

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
  OpenSBCDaemon * app = dynamic_cast<OpenSBCDaemon *>( OSSApplication::GetInstance() );
  return app->OnLoadRegistrationStatus( htmlBlock );
}

PString OpenSBCDaemon::OnLoadRegistrationStatus( 
  const PString & htmlBlock 
)
{
  RegistrationDatabase & regDB = GetSBC()->GetRegistrar()->GetRegistrationDB();
  PWaitAndSignal lock( regDB.GetMutex() );
  PString substitution;

  for( PINDEX i = 0; i < regDB.GetSize(); i++ )
  {
    CacheObject * cacheData = regDB.GetAt( i );
    if( cacheData != NULL )
    {
      SIPMessage * reg = dynamic_cast<SIPMessage *>(cacheData->GetObject());
      if( reg != NULL )
      {
        PString insert = htmlBlock;
        SpliceMacro(insert, "URI", cacheData->GetObjectIdentifier() );
        SpliceMacro(insert, "AOR", reg->GetContactTopURI().AsString( FALSE ) );
        SpliceMacro(insert, "Expires", PString( cacheData->GetLifeSpan() ) );
        SpliceMacro(insert, "Call-ID", reg->GetCallId());
        substitution+=insert;
      }
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

  app.OnPostControl(data, msg);


  msg << PHTML::Paragraph()
      << PHTML::HotLink(request.url.AsString()) << "Reload page" << PHTML::HotLink()
      << "&nbsp;&nbsp;&nbsp;&nbsp;"
      << PHTML::HotLink("/") << "Home page" << PHTML::HotLink();

  PServiceHTML::ProcessMacros(request, msg, "html/status.html",
                              PServiceHTML::LoadFromFile|PServiceHTML::NoSignatureForFile);
                              
  return TRUE;
}

BOOL OpenSBCDaemon::OnPostControl(const PStringToString & data, PHTML & msg)
{
  BOOL ok = FALSE;
  for( PINDEX i = 0; i < data.GetSize(); i++ )
  {
    PString key = data.GetKeyAt(i);
    PString command = data.GetDataAt(i);

    if (command == "Unregister") 
    {
      SIPSession::GCRef session = GetSBC()->GetRegistrar()->FindGCRefByCallId( key );
      if( session != NULL )
      {
        ok = TRUE;
        session->Destroy();

        msg << PHTML::Heading(2) << "AOR removed for Call-ID " << key << PHTML::Heading(2);

      }else
      {
        msg << PHTML::Heading(2) << "No Registration Found for Call-ID " << key  << PHTML::Heading(2);
      }
    }
  }
  return ok;
}

void OpenSBCDaemon::OnAdditionalXMLRPCMethods(
  PStringArray & methods
)
{
  methods.AppendString( "Registrar.GetRegStatus" );
  methods.AppendString( "Registrar.FindRegistration" );
  methods.AppendString( "Registrar.Unregister" );
  methods.AppendString( "Routing.AddRelayRoute" );
  methods.AppendString( "Proxy.Startup" );
  methods.AppendString( "B2BUA.Startup" );
}

void OpenSBCDaemon::OnCustomXMLRPCRequest(
  const PString & method,
  PArray<PStringToString> & request,
  PArray<PStringToString> & response
)
{
  if( !m_IsInitialized )
  {
    PStringToString * immediateResponse = new PStringToString();
    immediateResponse->SetAt( "ResponseCode", "999" );
    immediateResponse->SetAt( "ResponseMessage", "Server Not Ready" );
    response.SetAt( 0, immediateResponse );
    return;
  }

  PStringToString * requestParams = dynamic_cast<PStringToString*>(request.GetAt(0));

  if( method == "Registrar.GetRegStatus" )
  {
    RegistrationDatabase & regDB = GetSBC()->GetRegistrar()->GetRegistrationDB();
    PWaitAndSignal lock( regDB.GetMutex() );
    PString substitution;

    PStringToString * immediateResponse = new PStringToString();
    immediateResponse->SetAt( "ResponseCode", "0" );
    immediateResponse->SetAt( "ResponseMessage", "Ok" );
    response.SetAt( 0, immediateResponse );
    
    for( PINDEX i = 0; i < regDB.GetSize(); i++ )
    {
      CacheObject * cacheData = regDB.GetAt( i );
      if( cacheData != NULL )
      {
        SIPMessage * reg = dynamic_cast<SIPMessage *>(cacheData->GetObject());
        if( reg != NULL )
        {
          PStringToString * record = new PStringToString();
          record->SetAt( "URI", cacheData->GetObjectIdentifier() );
          record->SetAt( "AOR", reg->GetContactTopURI().AsString( FALSE ) );
          record->SetAt( "Expires", PString( cacheData->GetLifeSpan() ) );
          record->SetAt( "Call-ID", reg->GetCallId());
          response.SetAt( i + 1, record );
        }
      }
    }
  }else if( method == "Registrar.FindRegistration" )
  {
    PTRACE( 1, "Registrar.FindRegistration" );
    RegistrationDatabase & regDB = GetSBC()->GetRegistrar()->GetRegistrationDB();
    PWaitAndSignal lock( regDB.GetMutex() );

    if( requestParams == NULL )
    {
      PStringToString * immediateResponse = new PStringToString();
      immediateResponse->SetAt( "ResponseCode", "999" );
      immediateResponse->SetAt( "ResponseMessage", "Missing URI parameter" );
      response.SetAt( 0, immediateResponse );
      return;
    }else
    {
      PString * uri = requestParams->GetAt( "URI" );
      if( uri == NULL || uri->IsEmpty() )
      {
        PStringToString * immediateResponse = new PStringToString();
        immediateResponse->SetAt( "ResponseCode", "999" );
        immediateResponse->SetAt( "ResponseMessage", "Missing URI parameter" );
        response.SetAt( 0, immediateResponse );
        return;
      }

      SIPURI userURI( *uri );
      SIPMessage registration;
      PStringToString * immediateResponse = new PStringToString();
      if( regDB.FindRegistration( userURI, registration ) )
      {
        SIPURI targetURI;
        GetSBC()->GetSBCRoutingHandler()->ExtractTargetURI( targetURI, registration );
        immediateResponse->SetAt( "Registration", targetURI.AsString() );
        immediateResponse->SetAt( "ResponseCode", "0" );
        immediateResponse->SetAt( "ResponseMessage", "Ok" );
        response.SetAt( 0, immediateResponse );
        PTRACE( 1, "Found registration " << targetURI );
        return;
      }else
      {
        immediateResponse->SetAt( "ResponseCode", "0" );
        immediateResponse->SetAt( "ResponseMessage", "No Registation" );
        response.SetAt( 0, immediateResponse );
        return;
      }
    }
  }else if( method == "Routing.AddRelayRoute" )
  {
    PStringToString * immediateResponse = new PStringToString();
    if( requestParams == NULL )
    { 
      immediateResponse->SetAt( "ResponseCode", "999" );
      immediateResponse->SetAt( "ResponseMessage", "Missing Route parameter" );
      response.SetAt( 0, immediateResponse );
      return;
    }else
    {
      PString * route = requestParams->GetAt( "Route" );
      if( route == NULL || route->IsEmpty() )
      {
        immediateResponse->SetAt( "ResponseCode", "999" );
        immediateResponse->SetAt( "ResponseMessage", "Missing Route parameter" );
        response.SetAt( 0, immediateResponse );
        return;
      }

      GetSBC()->GetSBCRoutingHandler()->AppendAppRelayRoutes( *route );
      immediateResponse->SetAt( "ResponseCode", "0" );
      immediateResponse->SetAt( "ResponseMessage", "Ok" );
      response.SetAt( 0, immediateResponse );
      return;
    }
  }else if( method == "Registrar.Unregister" )
  {
    PString * key = requestParams->GetAt( "Call-ID" );
    PStringToString * immediateResponse = new PStringToString();
    if( key == NULL || key->IsEmpty() )
    {
      immediateResponse->SetAt( "ResponseCode", "999" );
      immediateResponse->SetAt( "ResponseMessage", "Missing Call-ID parameter" );
      response.SetAt( 0, immediateResponse );
      return;
    }

    SIPSession::GCRef session = GetSBC()->GetRegistrar()->FindGCRefByCallId( *key );
    PStringStream msg;
    if( session != NULL )
    {
      session->Destroy();
      msg << "AOR removed for Call-ID " << *key ;
    }else
    {
      msg << "No Registration Found for Call-ID " << *key ;
    }

    immediateResponse->SetAt( "ResponseCode", "0" );
    immediateResponse->SetAt( "ResponseMessage", "Ok" );
    response.SetAt( 0, immediateResponse );

    PStringToString * message = new PStringToString();
    message->SetAt( "Message", msg );
    response.SetAt( 1, message );
    return;
  }
}

SBCRegistrationStatusPage::SBCRegistrationStatusPage(OpenSBCDaemon & _app, PHTTPAuthority & auth)
  : PServiceHTTPString("OpenSBC Registration Status", "", "text/html; charset=UTF-8", auth),
    app(_app)
{
  
  PHTML html;

  html << PHTML::Title("OpenSBC Registration Status")
       << "<meta http-equiv=\"Refresh\" content=\"30\">\n"
       << PHTML::Body()
       << app.GetPageGraphic()

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

       << app.GetCopyrightText()
       << PHTML::Body();

  string = html;

}






