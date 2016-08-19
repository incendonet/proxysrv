/*
 *
 * Router.cxx
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
 * Contributor(s): ________________________________________________.
 *
 * $Log: Router.cxx,v $
 * Revision 1.6  2007/01/22 10:00:58  joegenbaclor
 * Fixed ProxyRouteRequest
 *
 * Revision 1.5  2007/01/17 00:08:44  joegenbaclor
 * Added SysLog
 *
 * Revision 1.4  2007/01/08 07:13:23  joegenbaclor
 * Added ability to run SBC in pure proxy or pure b2b modes
 *
 * Revision 1.3  2006/11/22 11:33:25  rcolobong
 * 1. Change method FindRoute to HasScheme
 * 2. Fix problem where it "Rounds Robin" in HasScheme method
 *
 * Revision 1.2  2006/10/11 05:04:50  rcolobong
 * Add new method FindRoute where it will search in "OpenSBC Route" based on scheme
 *
 * Revision 1.1  2006/08/14 10:04:59  rcolobong
 * Convert B2BUA to SBC
 * Support MP logging
 *
 * Revision 1.5  2006/08/04 05:14:03  rcolobong
 * 1.  Add getter for Routes
 * 2.  Add support for RTBE Scheme in FindRoute
 *
 * Revision 1.4  2006/07/17 11:36:44  joegenbaclor
 * 1.  More routing enhancements to B2BUA
 *
 * Revision 1.3  2006/07/13 06:53:03  joegenbaclor
 * 1.  Introduced Sanity checks for incoming SIP Message
 * 2.  Corrected bug in SIPURI where enum scheme is not getting recognized.
 * 3.  Added ENUM support to routing
 * 4.  Introduced global routing flag to indicate when and not to rewrite outbound To URI's
 *
 * Revision 1.2  2006/06/23 10:30:54  joegenbaclor
 * This revision marks the first successful call transfer via REFER by OpenB2BUA
 *
 * Revision 1.1  2006/06/20 09:58:11  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.2  2006/05/19 06:30:37  joegenbaclor
 * 1.  Fixed bug in SIPHeaderB where tag may not be parsed properly if no < > enclosure is present
 * 2.  Various enhancements to OpenSBC and proxy session
 *
 * Revision 1.1  2006/05/17 04:04:48  joegenbaclor
 * Initial upload of OpenSBC files
 *
 *
 */

#include "Router.h"

#define new PNEW

#define CALL_TRANSFER_PREFIX "xfer-oss-"

Router::Router()
{
}

Router::Router( 
  const PStringArray & routes
)
{
  Parse( routes );
}

Router & Router::operator=( 
  const PStringArray & routes
)
{
  Parse( routes );
  return *this;
}

BOOL Router::Parse( 
  const PStringArray & routes
)
{
  PWaitAndSignal lock( m_RoutesMutex );

  if( routes.GetSize() == 0 )
    return FALSE;

  m_Routes.RemoveAll();

  for( PINDEX i = 0; i < routes.GetSize(); i++ )
    AppendRoute( routes[i] );

  return m_Routes.GetSize() != 0;
}

BOOL Router::AppendRoute(
  const PString & route
)
{
  PWaitAndSignal lock( m_RoutesMutex );

  RouteRecord * rec = new RouteRecord();
  if( !rec->Parse( route ) )
    delete rec;
  else
  {
    PTRACE( 1, "Appending route: " << *rec );
    m_Routes.Append( rec );
    return TRUE;
  }

  return FALSE;
}

BOOL Router::FindRoute(
  const SIPMessage & request,
  SIPURI & target,
  BOOL incrementCurrentRoute,
  BOOL compareMethods
)
{
  PWaitAndSignal lock( m_RoutesMutex );

  if( !request.IsRequest() )
    return FALSE;

  /// get the To URI
  SIPURI to = request.GetToURI();

  PString userName = to.GetUser();

  PString  transferPrefix = CALL_TRANSFER_PREFIX;

  if( userName.Left( transferPrefix.GetLength() ) == transferPrefix )
    to.SetUser( userName.Mid( transferPrefix.GetLength() ) );
  
  BOOL ok = FALSE;
  for( PINDEX i = 0; i < m_Routes.GetSize(); i++ )
  {
    ok = FALSE;
    if(  m_Routes[i] *= to )
    {
      if( incrementCurrentRoute )
      {
        ok = m_Routes[i].GetTargetURI( target );
      }else
      {
        ok = m_Routes[i].GetTargetURI( target, 0 );
      }

      if( ok )
      {
        if( compareMethods )
        {
          /// check if the target uri has a method specified
          PString method;
          if( target.GetParameter( "method", method ) )
          {
            method = ParserTools::UnQuote( method );
            LOG( LogDebug(), "method=" << method << "request.method=" << request.GetMethod() );
            if( request.GetMethod() *= method )
              break;
            else
              continue;
          }else
          {
            LOG( LogDebug(), "method=?" << "request.method=" << request.GetMethod() );
            if( request.IsInvite() )
              break;
            else
              continue;  //searching
          }
        }

        break;
      }
    }
  }

  
  if( ok )
  {
    if( target.GetUser().IsEmpty() )
      target.SetUser( to.GetUser() );

    PTRACE( 1, "-->> From: " << request.GetFromURI() 
    << " Target: " << request.GetMethod() << " " << target );
  }else
  {
    PTRACE( 1, "*** NO STATIC ROUTE DEFINED *** From: " << request.GetFromURI()  
      <<  " Target: " <<  to );
  }

  return ok;
}


BOOL Router::FindRoute(
  const SIPMessage & invite,
  B2BUAConnection & connection
)
{
  PWaitAndSignal lock( m_RoutesMutex );

  /// get the To URI
  SIPURI to = invite.GetToURI();
  PString userName = to.GetUser();
  PString  transferPrefix = CALL_TRANSFER_PREFIX;
  if( userName.Left( transferPrefix.GetLength() ) == transferPrefix )
  {
    to.SetUser( userName.Mid( transferPrefix.GetLength()  ) );
    userName = userName.Mid( transferPrefix.GetLength()  );
  }

  BOOL ok = FALSE;
  for( PINDEX i = 0; i < m_Routes.GetSize(); i++ )
  {
    if(  m_Routes[i] *= to )
    {
      const PStringArray & routes = m_Routes[i].GetTargetURIList();
      ok = routes.GetSize() != 0;

      if( ok )
      {
        for( PINDEX j = 0; j < routes.GetSize(); j++ )
        {
          SIPURI routeURI( routes[j] );
          
          if( routeURI.GetScheme() *= "enum" )
          {
            PStringArray enumServer;
            enumServer.AppendString( routeURI.GetHost() );
            SIPTransport::ENUMLookup(
              enumServer,
              userName,
              routeURI ); 
            

          }

          if( ( routeURI.GetScheme() *= "sip"  ) 
            || ( routeURI.GetScheme() *= "rtbe"  ) ) 
          {
            if( routeURI.GetUser().IsEmpty() )
              routeURI.SetUser( userName );
            connection.AddRoute( routeURI );
          }
        }
      }
      break;
    }
  }

  return ok;

}

BOOL Router::HasScheme(
  const SIPMessage & request,
  const PString & scheme
)
{
  PWaitAndSignal lock( m_RoutesMutex );

  if( !request.IsRequest() )
    return FALSE;

  /// get the To URI
  SIPURI to = request.GetToURI();

  PString userName = to.GetUser();
  PString  transferPrefix = CALL_TRANSFER_PREFIX;

  if( userName.Left( transferPrefix.GetLength() ) == transferPrefix )
    to.SetUser( userName.Mid( transferPrefix.GetLength()  ) );
  
  for( PINDEX index = 0; index < m_Routes.GetSize(); ++index )
  {
    if( m_Routes[index] *= to )
      return m_Routes[index].HasScheme( scheme );
  }

  return FALSE;
}
