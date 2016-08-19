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
 * $Log: OSSAppConfig.cxx,v $
 * Revision 1.11  2007/01/22 14:29:25  joegenbaclor
 * some more stray debug PTRACE
 *
 * Revision 1.10  2007/01/17 10:33:59  joegenbaclor
 * Redesigned syslog
 *
 * Revision 1.9  2007/01/17 00:12:19  joegenbaclor
 * 1.  Fixed bug in call session where CANCEL may not be sent if a call ahs been authenticated due to a misplaced SetState( StateDisconnected ) for 4xx events
 * 2.  Fixed register session to ignore authorization headers if allow all reg is flagged
 * 3.  Temporary hacks for syslog
 * 4.  More launcher code -  almost there!
 *
 * Revision 1.8  2007/01/14 13:12:54  joegenbaclor
 * Fixed compile warnings
 *
 * Revision 1.7  2007/01/13 08:04:20  joegenbaclor
 * Removed unsused references
 *
 * Revision 1.6  2007/01/12 10:49:10  joegenbaclor
 * minor proxy bug fixes
 * more launcher code
 *
 * Revision 1.5  2007/01/12 00:31:02  joegenbaclor
 * launcher related code
 *
 * Revision 1.4  2007/01/11 10:01:24  joegenbaclor
 * more launcher code
 *
 * Revision 1.3  2007/01/08 11:15:51  joegenbaclor
 * Added capability to add status pages
 *
 * Revision 1.2  2007/01/08 04:09:27  joegenbaclor
 * Added selection field support
 *
 * Revision 1.1  2007/01/05 11:49:31  joegenbaclor
 * Initial upload of OSSAppConfig Files
 *
 *
 */


#include "OSSApplication.h"
#include "ossbuildopts.h"
#if P_EXPAT


#define new PNEW

using namespace Tools;

OSSAppConfig::OSSAppConfig( 
  Type type,
  WORD localPort,
  const PString & serverAddress,
  const PString & clientKey
)
{
  m_Type = type;
  m_ServerURL = serverAddress;
  m_ClientKey = clientKey;
  m_HTTPPort = localPort;
  m_FirstRun = TRUE;

  if( m_Type != HTTPDefaultProvider )
  {
    InitXMLRPC();
    
    if( m_Type == XMLRPCClientProvider )
      RPCSubcribe();
  }
}

OSSAppConfig *OSSAppConfig::GetInstance()
{
  static OSSAppConfig * instance = NULL;
  
  if( instance == NULL )
    instance = OSSApplication::GetInstance()->OnCreateAppConfig();

  return instance;
}


///////////////////////////////////////////////////////////

OSSAppConfig::StatusPage::StatusPage(
  PHTTPAuthority & auth,
  PXMLElement * config
) : PServiceHTTPString( "Status", "", "text/html; charset=UTF-8", auth )
{
  m_Config = config;
  Prepare();
}
      
BOOL OSSAppConfig::StatusPage::Post(
  PHTTPRequest & request,
  const PStringToString &,
  PHTML & msg
)
{
  return FALSE;
}

void OSSAppConfig::StatusPage::Prepare()
{
  if( m_Config == NULL )
    return;

  PString title = m_Config->GetAttribute( "Title" );
  PStringArray items;
  for( PINDEX i = 0; i < P_MAX_INDEX; i++ )
  {
    PXMLElement * item = m_Config->GetElement( "Item", i );
    if( item == NULL )
      break;
    items.AppendString( item->GetData() );
  }

  PHTML html;

  html << PHTML::Title(title)
       << "<meta http-equiv=\"Refresh\" content=\"30\">\n"
       << PHTML::Body()
       << OSSApplication::GetInstance()->GetPageGraphic()
       << PHTML::Paragraph() << "<center>"

       << PHTML::Form("POST")

       << PHTML::TableStart("border=1")
       << PHTML::TableRow()
       << PHTML::TableHeader()
       << "&nbsp;End&nbsp;Point&nbsp;Identifier&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;Call&nbsp;Signal&nbsp;Addresses&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;Aliases&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;Application&nbsp;"
       << PHTML::TableHeader()
       << "&nbsp;Active&nbsp;OSS Application Status&nbsp;"

       << "<!--#macrostart " << title << "-->"
         << PHTML::TableRow();

       for( PINDEX j = 0; j < items.GetSize() ; j++ )
       {
         html << PHTML::TableData();
         html << "<!--#status " << items[j] << "-->";
       }

       html << "<!--#macroend "<< title << "-->"
        << PHTML::TableEnd()
        << PHTML::Form()
        << PHTML::HRule()
        << OSSApplication::GetInstance()->GetCopyrightText()
        << PHTML::Body();

  string = html;
}

BOOL OSSAppConfig::Initialize(
  const PFilePath & path 
)
{
  PWaitAndSignal lock( m_InitializeMutex );

  if( m_Type == XMLRPCClientProvider ) /// No need to initialize anything
  {
    m_FirstRun = FALSE;
    return TRUE;
  }

  PXML xml;
  BOOL hasConfig = xml.LoadFile( path );

  PStringStream configTitle;
  
  
  if( !hasConfig )
  {
#if 0
    configTitle << OSSApplication::GetInstance()->GetName() << " HTTP Admin";
#endif
    PSYSTEMLOG(Fatal, "Unable to find " << path << ".  " << "Please install this file in the same directory as your applcation" );
    return FALSE;
  }else
  {
    PXMLElement * appName = xml.GetElement( "Application" );
    PXMLElement * httpPort = xml.GetElement( "HTTPPort" );
    if( appName == NULL )
      configTitle << OSSApplication::GetInstance()->GetName() << " HTTP Admin";
    else
      configTitle << appName->GetData() << " HTTP Admin";

    if( httpPort != NULL && !httpPort->GetData().IsEmpty() )
    {
      m_HTTPPort = (WORD)httpPort->GetData().AsUnsigned();
    }
  }
  
  
  m_Config = PConfig( configTitle );

  // Get the HTTP basic authentication info
  PString adminUserName = m_Config.GetString( "HTTP User" );
  PString adminPassword = PHTTPPasswordField::Decrypt( m_Config.GetString( "HTTP Password" ) );
  PHTTPSimpleAuth authority( OSSApplication::GetInstance()->GetName(), adminUserName, adminPassword );

  // Create the parameters URL page, and start adding fields to it
  PConfigPage * mainConfig = new PConfigPage( 
    *(OSSApplication::GetInstance()), 
    configTitle, 
    configTitle, 
    authority );
  
  ConfigPages extraConfig;
  extraConfig.DisallowDeleteObjects();


  if( hasConfig )
  {
    IntializeConfigPages( xml, extraConfig, authority );
  }


#if P_SSL
  // SSL certificate file.
  PString certificateFile = "cert.pem";
  if (!OSSApplication::GetInstance()->SetServerCertificate(certificateFile, TRUE)) {
    PSYSTEMLOG(Fatal, "BMAC\tCould not load certificate \"" << certificateFile << '"');
    return FALSE;
  }
#endif

  adminUserName = m_Config.GetString( "HTTP User" );
  adminPassword = PHTTPPasswordField::Decrypt(m_Config.GetString( "HTTP Password" ));

  // HTTP authentication username/password
  mainConfig->Add(new PHTTPStringField( "HTTP User", 25, adminUserName));
  mainConfig->Add(new PHTTPPasswordField( "HTTP Password", 25, adminPassword));

  CreateConfig( xml, extraConfig );

  /// Build the links
  PServiceHTML htmllink( configTitle );
  htmllink << PHTML::Paragraph()
      << PHTML::HotLink("Home Page") << "Reload page" << PHTML::HotLink()
      << "&nbsp;&nbsp;&nbsp;&nbsp;"
      << PHTML::HotLink("/") << "Home page" << PHTML::HotLink();

  mainConfig->BuildHTML( htmllink );

  PINDEX i = 0;

  for( i = 0; i < extraConfig.GetSize(); i++ )
  {
    PConfigPage * extra = (PConfigPage *)extraConfig.GetAt( i );
    PServiceHTML link( extra->GetConfigSection() );
    link << PHTML::Paragraph()
      << PHTML::HotLink(extra->GetConfigSection()) << "Reload page" << PHTML::HotLink()
      << "&nbsp;&nbsp;&nbsp;&nbsp;"
      << PHTML::HotLink("/") << "Home page" << PHTML::HotLink();

    extra->BuildHTML( link );

   
  }

  OSSApplication::GetInstance()->GetHTTPNameSpace().AddResource( mainConfig, PHTTPSpace::Overwrite );
 

  for( i = 0; i < extraConfig.GetSize(); i++ )
  {
    PConfigPage * extra = (PConfigPage *)extraConfig.GetAt( i );
    OSSApplication::GetInstance()->GetHTTPNameSpace().AddResource( extra, PHTTPSpace::Overwrite );
  }

  OSSApplication::GetInstance()->OnAdditionalHTTPResource( authority );

    

  PHTML html;
  html << PHTML::Title(configTitle)
        << PHTML::Body()
        << OSSApplication::GetInstance()->GetPageGraphic();

  html  << PHTML::Paragraph() << "<center>"
        << PHTML::HotLink(configTitle) << configTitle << PHTML::HotLink()
        << PHTML::Paragraph();

 
  for( i = 0; i < extraConfig.GetSize(); i++ )
  {
    PConfigPage * extra = (PConfigPage *)extraConfig.GetAt( i );
    PString linkTitle = extra->GetConfigSection();

    html  << PHTML::Paragraph() << "<center>"
          << PHTML::HotLink(linkTitle) << linkTitle << PHTML::HotLink()
          << PHTML::Paragraph();
  }

  OSSApplication::GetInstance()->OnAdditionalHTTPLinks( html );

  html  << PHTML::HRule()
        << OSSApplication::GetInstance()->GetCopyrightText()
        << PHTML::Body();

  OSSApplication::GetInstance()->GetHTTPNameSpace().AddResource(new PServiceHTTPString("welcome.html", html), PHTTPSpace::Overwrite);


  m_FirstRun = FALSE;

  return TRUE;
}

BOOL OSSAppConfig::IntializeConfigPages(
  PXML & schema,
  ConfigPages & configPages,
  PHTTPSimpleAuth & auth
)
{
  for( PINDEX  i = 0; i < P_MAX_INDEX; i++ )
  {
    PXMLElement * page = schema.GetElement( "ConfigurationPage", i );
    
    if( page == NULL )
      return configPages.GetSize() > 0;

    PString title = page->GetAttribute( "Title" );
    if( title.IsEmpty() )
      continue;

    if( title *= "System" )
      continue;

    configPages.Append( new PConfigPage( 
      *(OSSApplication::GetInstance()), 
      title, 
      title, 
      auth ) );
  }

  return FALSE;
}

BOOL OSSAppConfig::IntializeStatusPages(
  PXML & schema,
  StatusPages & statusPages,
  PHTTPSimpleAuth & auth
)
{
  return FALSE;
}

BOOL OSSAppConfig::CreateConfig(
  PXML & schema,
  ConfigPages & configPages
)
{
  for( PINDEX i = 0; i < configPages.GetSize(); i++ )
  {
    
    PXMLElement * pageSchema = NULL;
    PConfigPage * currentPage = (PConfigPage *)configPages.GetAt( i );

    for( PINDEX  j = 0; j < P_MAX_INDEX; j++ )
    {
      pageSchema = schema.GetElement( "ConfigurationPage", j );
      if( pageSchema == NULL )
        break;

      PString title = pageSchema->GetAttribute( "Title" );
      if( title.IsEmpty() )
      continue;

      if( title *= "System" )
        continue;

      if( title *= currentPage->GetConfigSection() )
        break;
    }

    CreatePage( *pageSchema, *currentPage );
  }

  

  return FALSE;
}

BOOL OSSAppConfig::CreatePage(
  PXMLElement & pageSchema,
  PConfigPage & page
)
{

  PXMLElement * params = pageSchema.GetElement( "Parameters" );
  if( params == NULL )
    return FALSE;

  for( PINDEX  i = 0; i < P_MAX_INDEX; i++ )
  {
    PXMLElement * item = params->GetElement( "Item", i );
    if( item == NULL )
      break;


    PString section = pageSchema.GetAttribute( "Title" );

    int dfltLen = 30;
    PString key = item->GetData();
    PString type = item->GetAttribute( "Type" );
    PString dflt = item->GetAttribute( "Default" );
    PString min = item->GetAttribute( "Minimum" ); 
    PString max = item->GetAttribute( "Maximum" );
    PString length = item->GetAttribute( "Length" );
    PString comment = item->GetAttribute( "Comment" );
    PString listItemString = item->GetAttribute( "Items" );
    PStringArray listItems;
    if( !listItemString.IsEmpty() )
      listItems = listItemString.Tokenise( ',' );

    if( !length.IsEmpty() )
        dfltLen = length.AsInteger();

    if( type *= "string" )
    {
      
      PString str = m_Config.GetString( section, key, dflt ); 
      page.Add(new PHTTPStringField( key, dfltLen, str, comment ) );
    }else if( type *= "list" )
    {
      PHTTPFieldArray * fieldArray = 
      new PHTTPFieldArray( new PHTTPStringField( key, dfltLen, 0, comment ), TRUE );
      page.Add(fieldArray);
    }else if( type *= "integer" )
    {
      int intDflt = 0;
      int intMin = 0;
      int intMax = P_MAX_INDEX;
      
      if( !dflt.IsEmpty() )
        intDflt = dflt.AsInteger();

      if( !min.IsEmpty() )
        intMin = min.AsInteger();

      if( !max.IsEmpty() )
      {
        intMax = max.AsInteger();
        if( intMax <= intMin )
          intMax = P_MAX_INDEX;
      }

      long val = m_Config.GetInteger( section, key, intDflt ); 
      page.Add( new PHTTPIntegerField( key, key, intMin, intMax, val, comment ) );

    }else if( type *= "boolean" )
    {
      BOOL boolDflt = FALSE;
      
      if( !dflt.IsEmpty() )
        boolDflt = dflt.ToLiteral().Find( "t" ) != P_MAX_INDEX;

      BOOL val = m_Config.GetBoolean( section, key, boolDflt );

      page.Add( new PHTTPBooleanField( key, val, comment ) );

    }else if( type *= "password" )
    {
      PString str = m_Config.GetString( section, key, dflt ); 
      page.Add(new PHTTPPasswordField(key, dfltLen, str));
    }if( type *= "selection" )
    {
      PString str = m_Config.GetString( section, key, dflt ); 
      PINDEX index = 0;
      if( !str.IsEmpty() )
      {
        for( PINDEX i = 0; i < listItems.GetSize(); i++ )
        {
          PString item = listItems[i].Trim();
          if( str *= item )
          {
            index = i;
            break;
          }
        }
      }

      page.Add( new PHTTPSelectField( key, listItems, index, comment ) );
    }else
    {
      continue;
    }
  }
  return TRUE;
}

PString OSSAppConfig::GetListItem(
  const PString & section,
  const PString & itemName,
  PINDEX index
)
{ 
  PWaitAndSignal lock( m_InitializeMutex );
  
  if( m_Type != XMLRPCClientProvider )
  {    
    PString dflt = m_Config.GetDefaultSection();
    m_Config.SetDefaultSection( section );
    PHTTPFieldArray fieldArray( new PHTTPStringField( itemName, 1024 ), TRUE );
    PStringArray list = fieldArray.GetStrings( m_Config );
    m_Config.SetDefaultSection( dflt );

    if( index >= list.GetSize() )
      return PString::Empty();

    return list[index];
  }

  PArray<PStringToString>  cmd;
  PArray<PStringToString>  response;
  PStringToString * cmdParams = new PStringToString();
  
  cmdParams->SetAt( "ClientKey", m_ClientKey );
  cmdParams->SetAt( "Section", section );
  cmdParams->SetAt( "ItemName", itemName );
  cmdParams->SetAt( "Index", PString( index ) );
  cmd.SetAt( 0, cmdParams );

  SendRequest( "Config.GetListItem", cmd, response, m_ServerURL );
  if( response.GetSize() == 0 )
    return PString::Empty();

  PStringToString * responseParams = static_cast<PStringToString*>(response.GetAt( 0 ));
  PString * value = responseParams->GetAt( "Value" );
  
  return *value;
}

void OSSAppConfig::RPCGetListItem(
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  PStringToString * params = dynamic_cast<PStringToString*>(request.GetAt(0));
  if( params == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1000" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Parameters" );
    return;
  }

  PString * section = params->GetAt( "Section" );
  if( section == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1001" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Section Parameter" );
    return;
  }

  PString * itemName = params->GetAt( "ItemName" );
  if( itemName == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1002" );
    immediateResponse.SetAt( "ResponseMessage", "Missing ItemName Parameter" );
    return;
  }

  PString * index = params->GetAt( "Index" );
  if( index == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1003" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Index Parameter" );
    return;
  }

  immediateResponse.SetAt( "ResponseCode", "0" );
  immediateResponse.SetAt( "ResponseMessage", "Ok" );
  immediateResponse.SetAt( "Value", GetListItem( *section, *itemName, index->AsInteger() ) );
}

BOOL OSSAppConfig::SetListItem(
  const PString & section,
  const PString & itemName,
  const PString & value,
  const PINDEX index
)
{
  PWaitAndSignal lock( m_InitializeMutex );

  if( m_Type != XMLRPCClientProvider )
  {
    PString dflt = m_Config.GetDefaultSection();
    m_Config.SetDefaultSection( section );
    PHTTPFieldArray fieldArray( new PHTTPStringField( itemName, 1024 ), TRUE );
    PStringArray routes = fieldArray.GetStrings( m_Config );

    if( index >= routes.GetSize() )
      return FALSE;

    routes[index] = value;

    for( PINDEX i = 0; i < routes.GetSize(); ++i )
      m_Config.SetString( section, itemName, routes[i] );

    return TRUE;
  }

  PArray<PStringToString>  cmd;
  PArray<PStringToString>  response;
  PStringToString * cmdParams = new PStringToString();

  cmdParams->SetAt( "ClientKey", m_ClientKey );
  cmdParams->SetAt( "Section", section );
  cmdParams->SetAt( "ItemName", itemName );
  cmdParams->SetAt( "Value", value );
  cmdParams->SetAt( "Index", PString( index ) );
  cmd.SetAt( 0, cmdParams );

  return SendRequest( "Config.SetListItem", cmd, response, m_ServerURL ) == XMLRPCTransactionServer::Success;
}

void OSSAppConfig::RPCSetListItem(
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  PStringToString * params = dynamic_cast<PStringToString*>(request.GetAt(0));
  if( params == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1000" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Parameters" );
    return;
  }

  PString * section = params->GetAt( "Section" );
  if( section == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1001" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Section Parameter" );
    return;
  }

  PString * itemName = params->GetAt( "ItemName" );
  if( itemName == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1002" );
    immediateResponse.SetAt( "ResponseMessage", "Missing ItemName Parameter" );
    return;
  }

  PString * index = params->GetAt( "Index" );
  if( index == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1003" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Index Parameter" );
    return;
  }

  PString * value = params->GetAt( "Value" );
  
  if( itemName == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1004" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Value Parameter" );
    return;
  }

  immediateResponse.SetAt( "ResponseCode", "0" );
  immediateResponse.SetAt( "ResponseMessage", "Ok" );

  SetListItem( *section, *itemName, *value, index->AsInteger() );
}

void OSSAppConfig::AppendListItem(
  const PString & section,
  const PString & itemName,
  const PString & value
)
{
  PWaitAndSignal lock( m_InitializeMutex );

  if( m_Type != XMLRPCClientProvider )
  {
    PString dflt = m_Config.GetDefaultSection();
    m_Config.SetDefaultSection( section );
    PHTTPFieldArray fieldArray( new PHTTPStringField( itemName, 1024 ), TRUE );
    PStringArray routes = fieldArray.GetStrings( m_Config );

    routes.AppendString( value );

    for( PINDEX i = 0; i < routes.GetSize(); ++i )
      m_Config.SetString( section, itemName, routes[i] );
  }

  PArray<PStringToString>  cmd;
  PArray<PStringToString>  response;
  PStringToString * cmdParams = new PStringToString();

  cmdParams->SetAt( "ClientKey", m_ClientKey );
  cmdParams->SetAt( "Section", section );
  cmdParams->SetAt( "ItemName", itemName );
  cmdParams->SetAt( "Value", value );
  cmd.SetAt( 0, cmdParams );

  SendRequest( "Config.AppendListItem", cmd, response, m_ServerURL );
}

void OSSAppConfig::RPCAppendListItem(
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  PStringToString * params = dynamic_cast<PStringToString*>(request.GetAt(0));
  if( params == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1000" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Parameters" );
    return;
  }

  PString * section = params->GetAt( "Section" );
  if( section == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1001" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Section Parameter" );
    return;
  }

  PString * itemName = params->GetAt( "ItemName" );
  if( itemName == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1002" );
    immediateResponse.SetAt( "ResponseMessage", "Missing ItemName Parameter" );
    return;
  }


  PString * value = params->GetAt( "Value" );
  
  if( itemName == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1004" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Value Parameter" );
    return;
  }

  immediateResponse.SetAt( "ResponseCode", "0" );
  immediateResponse.SetAt( "ResponseMessage", "Ok" );

  AppendListItem( *section, *itemName, *value );
}


PINDEX OSSAppConfig::GetListSize(
  const PString & section,
  const PString & itemName
)
{
  PWaitAndSignal lock( m_InitializeMutex );
  if( m_Type != XMLRPCClientProvider )
  {
    PString dflt = m_Config.GetDefaultSection();
    m_Config.SetDefaultSection( section );
    PHTTPFieldArray fieldArray( new PHTTPStringField( itemName, 1024 ), TRUE );
    PStringArray routes = fieldArray.GetStrings( m_Config );
    PINDEX size = routes.GetSize();
    m_Config.SetDefaultSection( dflt );
    return size;
  }


  PArray<PStringToString>  cmd;
  PArray<PStringToString>  response;
  PStringToString * cmdParams = new PStringToString();

  cmdParams->SetAt( "ClientKey", m_ClientKey );
  cmdParams->SetAt( "Section", section );
  cmdParams->SetAt( "ItemName", itemName );
  cmd.SetAt( 0, cmdParams );
  SendRequest( "Config.GetListSize", cmd, response, m_ServerURL );

  if( response.GetSize() == 0 )
    return 0;

  PStringToString * responseParams = static_cast<PStringToString*>(response.GetAt(0));

  if( responseParams == NULL )
    return 0;

  PString * size = responseParams->GetAt( "ListSize" );

  if( size == NULL )
    return 0 ;

  return size->AsInteger();
}

void OSSAppConfig::RPCGetListSize(
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  PStringToString * params = dynamic_cast<PStringToString*>(request.GetAt(0));
  if( params == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1000" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Parameters" );
    return;
  }

  PString * section = params->GetAt( "Section" );
  if( section == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1001" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Section Parameter" );
    return;
  }

  
  PString * itemName = params->GetAt( "ItemName" );
  if( itemName == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1002" );
    immediateResponse.SetAt( "ResponseMessage", "Missing ItemName Parameter" );
    return;
  }


  immediateResponse.SetAt( "ResponseCode", "0" );
  immediateResponse.SetAt( "ResponseMessage", "Ok" );
  immediateResponse.SetAt( "ListSize", PString( GetListSize( *section, *itemName )  ) );

}

PString OSSAppConfig::GetString(
  const PString & section,
  const PString & itemName,
  const PString & dflt
)
{
  PWaitAndSignal lock( m_InitializeMutex );
  if( m_Type != XMLRPCClientProvider )
  {
    return m_Config.GetString( section, itemName, dflt );
  }

  PArray<PStringToString>  cmd;
  PArray<PStringToString>  response;
  PStringToString * cmdParams = new PStringToString();

  cmdParams->SetAt( "ClientKey", m_ClientKey );
  cmdParams->SetAt( "Section", section );
  cmdParams->SetAt( "ItemName", itemName );
  cmdParams->SetAt( "Default", dflt );
  cmd.SetAt( 0, cmdParams );
  SendRequest( "Config.GetString", cmd, response, m_ServerURL );

  if( response.GetSize() == 0 )
    return dflt;

  PStringToString * responseParams = static_cast<PStringToString*>(response.GetAt(0));

  if( responseParams == NULL )
    return dflt;

  PString * value = responseParams->GetAt( "Value" );

  if( value == NULL )
    return dflt;

  return *value;
}

void OSSAppConfig::RPCGetString(
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  PStringToString * params = dynamic_cast<PStringToString*>(request.GetAt(0));
  if( params == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1000" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Parameters" );
    return;
  }

  PString * section = params->GetAt( "Section" );
  if( section == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1001" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Section Parameter" );
    return;
  }

  PString * itemName = params->GetAt( "ItemName" );
  if( itemName == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1002" );
    immediateResponse.SetAt( "ResponseMessage", "Missing ItemName Parameter" );
    return;
  }

  PString * dflt = params->GetAt( "Default" );
  if( dflt == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1003" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Default Parameter" );
    return;
  }

  immediateResponse.SetAt( "ResponseCode", "0" );
  immediateResponse.SetAt( "ResponseMessage", "Ok" );
  immediateResponse.SetAt( "Value", GetString( *section, *itemName, *dflt ) );
}

void OSSAppConfig::SetString(
  const PString & section,
  const PString & itemName,
  const PString & value
)
{
  PWaitAndSignal lock( m_InitializeMutex );
  if( m_Type != XMLRPCClientProvider )
  {
    return m_Config.SetString( section, itemName, value );
  }

  PArray<PStringToString>  cmd;
  PArray<PStringToString>  response;
  PStringToString * cmdParams = new PStringToString();

  cmdParams->SetAt( "ClientKey", m_ClientKey );
  cmdParams->SetAt( "Section", section );
  cmdParams->SetAt( "ItemName", itemName );
  cmdParams->SetAt( "Value", value );
  cmd.SetAt( 0, cmdParams );
  SendRequest( "Config.SetString", cmd, response, m_ServerURL );
}

void OSSAppConfig::RPCSetString(
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  PStringToString * params = dynamic_cast<PStringToString*>(request.GetAt(0));
  if( params == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1000" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Parameters" );
    return;
  }

  PString * section = params->GetAt( "Section" );
  if( section == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1001" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Section Parameter" );
    return;
  }

  PString * itemName = params->GetAt( "ItemName" );
  if( itemName == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1002" );
    immediateResponse.SetAt( "ResponseMessage", "Missing ItemName Parameter" );
    return;
  }

  PString * value = params->GetAt( "Value" );
  if( value == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1003" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Value Parameter" );
    return;
  }

  immediateResponse.SetAt( "ResponseCode", "0" );
  immediateResponse.SetAt( "ResponseMessage", "Ok" );

  SetString( *section, *itemName, *value );
}

int OSSAppConfig::GetInteger(
  const PString & section,
  const PString & itemName,
  int dflt
)
{
  PWaitAndSignal lock( m_InitializeMutex );
  if( m_Type != XMLRPCClientProvider )
  {
    return m_Config.GetInteger( section, itemName, dflt );
  }

  PArray<PStringToString>  cmd;
  PArray<PStringToString>  response;
  PStringToString * cmdParams = new PStringToString();

  cmdParams->SetAt( "ClientKey", m_ClientKey );
  cmdParams->SetAt( "Section", section );
  cmdParams->SetAt( "ItemName", itemName );
  cmdParams->SetAt( "Default", dflt );
  cmd.SetAt( 0, cmdParams );
  SendRequest( "Config.GetInteger", cmd, response, m_ServerURL );

  if( response.GetSize() == 0 )
    return dflt;

  PStringToString * responseParams = static_cast<PStringToString*>(response.GetAt(0));

  if( responseParams == NULL )
    return dflt;

  PString * value = responseParams->GetAt( "Value" );

  if( value == NULL )
    return dflt;

  return value->AsInteger();
}

void OSSAppConfig::RPCGetInteger(
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  PStringToString * params = dynamic_cast<PStringToString*>(request.GetAt(0));
  if( params == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1000" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Parameters" );
    return;
  }

  PString * section = params->GetAt( "Section" );
  if( section == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1001" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Section Parameter" );
    return;
  }

  PString * itemName = params->GetAt( "ItemName" );
  if( itemName == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1002" );
    immediateResponse.SetAt( "ResponseMessage", "Missing ItemName Parameter" );
    return;
  }

  PString * dflt = params->GetAt( "Default" );
  if( dflt == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1003" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Default Parameter" );
    return;
  }

  immediateResponse.SetAt( "ResponseCode", "0" );
  immediateResponse.SetAt( "ResponseMessage", "Ok" );
  immediateResponse.SetAt( "Value", PString( GetInteger( *section, *itemName, dflt->AsInteger() ) ) );
}

void OSSAppConfig::SetInteger(
  const PString & section,
  const PString & itemName,
  const int value
)
{
  PWaitAndSignal lock( m_InitializeMutex );
  if( m_Type != XMLRPCClientProvider )
  {
    return m_Config.SetInteger( section, itemName, value );
  }

  PArray<PStringToString>  cmd;
  PArray<PStringToString>  response;
  PStringToString * cmdParams = new PStringToString();

  cmdParams->SetAt( "ClientKey", m_ClientKey );
  cmdParams->SetAt( "Section", section );
  cmdParams->SetAt( "ItemName", itemName );
  cmdParams->SetAt( "Value", PString( value ) );
  cmd.SetAt( 0, cmdParams );
  SendRequest( "Config.SetInteger", cmd, response, m_ServerURL );
}

void OSSAppConfig::RPCSetInteger(
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  PStringToString * params = dynamic_cast<PStringToString*>(request.GetAt(0));
  if( params == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1000" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Parameters" );
    return;
  }

  PString * section = params->GetAt( "Section" );
  if( section == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1001" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Section Parameter" );
    return;
  }

  PString * itemName = params->GetAt( "ItemName" );
  if( itemName == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1002" );
    immediateResponse.SetAt( "ResponseMessage", "Missing ItemName Parameter" );
    return;
  }

  PString * value = params->GetAt( "Value" );
  if( value == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1003" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Value Parameter" );
    return;
  }

  immediateResponse.SetAt( "ResponseCode", "0" );
  immediateResponse.SetAt( "ResponseMessage", "Ok" );

  SetInteger( *section, *itemName, value->AsInteger() );
}

void OSSAppConfig::SetBoolean(
  const PString & section,
  const PString & itemName,
  const BOOL value
)
{ 
  PWaitAndSignal lock( m_InitializeMutex );
  if( m_Type != XMLRPCClientProvider )
  {
    m_Config.SetBoolean( section, itemName, value );
  }

  PArray<PStringToString>  cmd;
  PArray<PStringToString>  response;
  PStringToString * cmdParams = new PStringToString();

  cmdParams->SetAt( "ClientKey", m_ClientKey );
  cmdParams->SetAt( "Section", section );
  cmdParams->SetAt( "ItemName", itemName );
  cmdParams->SetAt( "Value", PString( value ) );
  cmd.SetAt( 0, cmdParams );
  SendRequest( "Config.SetBoolean", cmd, response, m_ServerURL );
}

void OSSAppConfig::RPCGetBoolean(
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  PStringToString * params = dynamic_cast<PStringToString*>(request.GetAt(0));
  if( params == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1000" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Parameters" );
    return;
  }

  PString * section = params->GetAt( "Section" );
  if( section == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1001" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Section Parameter" );
    return;
  }

  PString * itemName = params->GetAt( "ItemName" );
  if( itemName == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1002" );
    immediateResponse.SetAt( "ResponseMessage", "Missing ItemName Parameter" );
    return;
  }

  PString * dflt = params->GetAt( "Default" );
  if( dflt == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1003" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Default Parameter" );
    return;
  }

  immediateResponse.SetAt( "ResponseCode", "0" );
  immediateResponse.SetAt( "ResponseMessage", "Ok" );
  immediateResponse.SetAt( "Value", PString( GetBoolean( *section, *itemName, dflt->AsInteger() ) ) );
}

BOOL OSSAppConfig::GetBoolean(
  const PString & section,
  const PString & itemName,
  BOOL dflt
)
{
  PWaitAndSignal lock( m_InitializeMutex );
  if( m_Type != XMLRPCClientProvider )
  {
    return m_Config.GetBoolean( section, itemName, dflt );
  }

  PArray<PStringToString>  cmd;
  PArray<PStringToString>  response;
  PStringToString * cmdParams = new PStringToString();

  cmdParams->SetAt( "ClientKey", m_ClientKey );
  cmdParams->SetAt( "Section", section );
  cmdParams->SetAt( "ItemName", itemName );
  cmdParams->SetAt( "Default", PString( dflt ) );
  cmd.SetAt( 0, cmdParams );
  SendRequest( "Config.GetBoolean", cmd, response, m_ServerURL );

  if( response.GetSize() == 0 )
    return dflt;

  PStringToString * responseParams = static_cast<PStringToString*>(response.GetAt(0));

  if( responseParams == NULL )
    return dflt;

  PString * value = responseParams->GetAt( "Value" );

  if( value == NULL )
    return dflt;

  return value->AsInteger();
}


void OSSAppConfig::RPCSetBoolean(
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  PStringToString * params = dynamic_cast<PStringToString*>(request.GetAt(0));
  if( params == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1000" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Parameters" );
    return;
  }

  PString * section = params->GetAt( "Section" );
  if( section == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1001" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Section Parameter" );
    return;
  }

  PString * itemName = params->GetAt( "ItemName" );
  if( itemName == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1002" );
    immediateResponse.SetAt( "ResponseMessage", "Missing ItemName Parameter" );
    return;
  }

  PString * value = params->GetAt( "Value" );
  if( value == NULL )
  {
    immediateResponse.SetAt( "ResponseCode", "1003" );
    immediateResponse.SetAt( "ResponseMessage", "Missing Value Parameter" );
    return;
  }

  immediateResponse.SetAt( "ResponseCode", "0" );
  immediateResponse.SetAt( "ResponseMessage", "Ok" );

  SetBoolean( *section, *itemName, value->AsInteger() );
}

BOOL OSSAppConfig::RPCSubcribe()
{
  PWaitAndSignal lock( m_InitializeMutex );
  if( m_Type != XMLRPCClientProvider )
    return FALSE;

  PArray<PStringToString>  cmd;
  PArray<PStringToString>  response;
  PStringToString * cmdParams = new PStringToString();

  PIPSocket::Address addr;
  WORD port;
  m_XMLRPCServer->GetListenerAddress( addr, port );

  PStringStream httpAddress;
  httpAddress << "http://" << addr << ":" << port << "/RPC2";
  m_XMLRPCServer->GetListenerAddress( addr, port );
  cmdParams->SetAt( "ClientKey", m_ClientKey );
  cmdParams->SetAt( "ClientContact", httpAddress );

  cmd.SetAt( 0, cmdParams );
  SendRequest( "Config.Subscribe", cmd, response, m_ServerURL );

  if( response.GetSize() == 0 )
    return FALSE;

  PStringToString * responseParams = static_cast<PStringToString*>(response.GetAt(0));

  if( responseParams == NULL )
    return FALSE;

  PString * value = responseParams->GetAt( "ResponseCode" );

  if( value == NULL )
    return FALSE;

  return value->AsInteger() == 0;
}

PString OSSAppConfig::GetSubscriberAddress(
  const PString & key 
)const
{
  PWaitAndSignal lock( m_SubscriptionsMutex );
  PString * str = m_Subscriptions.GetAt( key );

  if( str == NULL )
    return PString::Empty();

  return *str;
}

void OSSAppConfig::OnRPCSubscription(
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  PWaitAndSignal lock( m_InitializeMutex );
  if( m_Type != XMLRPCClientProvider )
  {
    PStringToString * params = dynamic_cast<PStringToString*>(request.GetAt(0));
    if( params == NULL )
    {
      immediateResponse.SetAt( "ResponseCode", "1000" );
      immediateResponse.SetAt( "ResponseMessage", "Missing Parameters" );
      return;
    }

    PString * clientKey = params->GetAt( "ClientKey" );
    if( clientKey == NULL )
    {
      immediateResponse.SetAt( "ResponseCode", "1001" );
      immediateResponse.SetAt( "ResponseMessage", "Missing ClientKey Parameter" );
      return;
    }

    PString * clientContact = params->GetAt( "ClientContact" );
    if( clientContact == NULL )
    {
      immediateResponse.SetAt( "ResponseCode", "1003" );
      immediateResponse.SetAt( "ResponseMessage", "Missing ClientContact Parameter" );
      return;
    }

    PWaitAndSignal lock( m_SubscriptionsMutex );
    if( !m_Subscriptions.Contains( *clientKey ) )
      m_Subscriptions.SetAt( *clientKey, new PString( *clientContact ) );
  }

  immediateResponse.SetAt( "ResponseCode", "0" );
  immediateResponse.SetAt( "ResponseMessage", "Ok" );
}

BOOL OSSAppConfig::RPCConfigChanged()
{
  PWaitAndSignal lock( m_SubscriptionsMutex );
  for( PINDEX i = 0; i < m_Subscriptions.GetSize(); i++ )
    RPCConfigChanged( m_Subscriptions.GetDataAt( i ) );

  return TRUE;
}

BOOL OSSAppConfig::RPCConfigChanged(
  const PString & subscriberAddress
)
{
  PWaitAndSignal lock( m_InitializeMutex );
  if( m_Type != XMLRPCAndHTTPServerProvider )
    return FALSE;

  PArray<PStringToString>  cmd;
  PArray<PStringToString>  response;
  PStringToString * cmdParams = new PStringToString();
  cmdParams->SetAt( "ClientKey", m_ClientKey );


  cmd.SetAt( 0, cmdParams );
  SendRequest( "Config.Changed", cmd, response, subscriberAddress );

  return TRUE;
}

void OSSAppConfig::OnRPCConfigChanged()
{
  OSSApplication::GetInstance()->OnConfigChanged( *this );
}

void OSSAppConfig::InitXMLRPC()
{
  if( m_XMLRPCServer == NULL && m_Type != HTTPDefaultProvider )
  {
    m_XMLRPCServer = new XMLRPCServer();
    m_XMLRPCServer->SetXMLRPCPort( m_HTTPPort );
    m_XMLRPCServer->ListenForXMLRPC( 1024 * 2 );
    m_Exchange = new XMLRPCEndpoint( *m_XMLRPCServer, *this );
    RegisterMethod( "Config.GetListItem" );
    RegisterMethod( "Config.SetListItem" );
    RegisterMethod( "Config.AppendListItem" );
    RegisterMethod( "Config.GetListSize" );
    RegisterMethod( "Config.GetString" );
    RegisterMethod( "Config.SetString" );
    RegisterMethod( "Config.GetInteger" );
    RegisterMethod( "Config.SetInteger" );
    RegisterMethod( "Config.GetBoolean" );
    RegisterMethod( "Config.SetBoolean" );
    RegisterMethod( "Config.Subscribe" );
    RegisterMethod( "Config.Changed" );

    /// allow the application  to register more methods
    PStringArray moreMethods;
    OSSApplication::GetInstance()->OnAdditionalXMLRPCMethods( moreMethods );
    for( PINDEX i = 0; i < moreMethods.GetSize(); i++ )
      RegisterMethod( moreMethods[i] );
  }
}


void OSSAppConfig::OnIncomingRequest(
  const PString & method,
  PArray<PStringToString>  & request,
  PArray<PStringToString>  & response
)
{
  PStringToString * responseParam = new PStringToString();
  if( request.GetSize() == 0 )
  {
    responseParam->SetAt( "ResponseCode", "999" );
    responseParam->SetAt( "ResponseMessage", "Missing Request Parameter" );
    response.SetAt(0, responseParam );
    return;
  }

  PString * tag = request[0].GetAt( "Tag" );
  
  if( !OnIncomingXMLRPCTransaction(
    method, 
    request,
    *responseParam
  ) )
  {
    OSSApplication::GetInstance()->OnCustomXMLRPCRequest( method, request, response );
    return;
  }

  response.SetAt(0, responseParam );
}

BOOL OSSAppConfig::OnIncomingXMLRPCTransaction(
  const PString & method,
  PArray<PStringToString> & request,
  PStringToString & immediateResponse
)
{
  if( method *= "Config.GetListItem" )
  {
    RPCGetListItem( request, immediateResponse );
  }else if( method *= "Config.SetListItem" )
  {
    RPCSetListItem( request, immediateResponse );
  }else if( method *= "Config.AppendListItem" )
  {
    RPCAppendListItem( request, immediateResponse );
  }else if( method *= "Config.GetListSize" )
  {
    RPCGetListSize( request, immediateResponse );
  }else if( method *= "Config.GetString" )
  {
    RPCGetString( request, immediateResponse );
  }else if( method *= "Config.SetString" )
  {
    RPCSetString( request, immediateResponse );
  }else if( method *= "Config.GetInteger" )
  {
    RPCGetInteger( request, immediateResponse );
  }else if( method *= "Config.SetInteger" )
  {
    RPCSetInteger( request, immediateResponse );
  }else if( method *= "Config.GetBoolean" )
  {
    RPCGetBoolean( request, immediateResponse );
  }else if( method *= "Config.SetBoolean" )
  {
    RPCSetBoolean( request, immediateResponse );
  }else if( method *= "Config.Subscribe" )
  {
    OnRPCSubscription( request, immediateResponse );
  }else if( method *= "Config.Changed" )
  {
    OnRPCConfigChanged();
    immediateResponse.SetAt( "ResponseCode", "0" );
    immediateResponse.SetAt( "ResponseMessage", "Ok" );
  }else 
  {
    // pass it to the application
    return FALSE;
  }

  return TRUE;
}



#endif // P_EXPAT





