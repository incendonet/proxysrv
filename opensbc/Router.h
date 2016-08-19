/*
 *
 * Router.h
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
 * $Log: Router.h,v $
 * Revision 1.5  2007/01/22 10:00:58  joegenbaclor
 * Fixed ProxyRouteRequest
 *
 * Revision 1.4  2007/01/17 00:08:50  joegenbaclor
 * Added SysLog
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
 * Revision 1.4  2006/08/04 05:14:03  rcolobong
 * 1.  Add getter for Routes
 * 2.  Add support for RTBE Scheme in FindRoute
 *
 * Revision 1.3  2006/07/17 11:36:44  joegenbaclor
 * 1.  More routing enhancements to B2BUA
 *
 * Revision 1.2  2006/07/13 06:53:03  joegenbaclor
 * 1.  Introduced Sanity checks for incoming SIP Message
 * 2.  Corrected bug in SIPURI where enum scheme is not getting recognized.
 * 3.  Added ENUM support to routing
 * 4.  Introduced global routing flag to indicate when and not to rewrite outbound To URI's
 *
 * Revision 1.1  2006/06/20 09:58:11  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.1  2006/05/17 04:04:48  joegenbaclor
 * Initial upload of OpenSBC files
 *
 *
 */

#ifndef ROUTER_H
#define ROUTER_H

#include "RouteRecord.h"
#include "SIPMessage.h"
#include "B2BUAConnection.h"


using namespace SIPParser;
using namespace B2BUA;

class Router : public PObject, public Logger
{
  PCLASSINFO( Router, PObject );

  PLIST( Routes, RouteRecord );
public:

  Router();

  Router( 
    const PStringArray & routes
  );

  Router & operator=( 
    const PStringArray & routes
  );

  BOOL Parse( 
    const PStringArray & routes
  );

  BOOL FindRoute(
    const SIPMessage & request,
    B2BUAConnection & connection
  );

  BOOL FindRoute(
    const SIPMessage & request,
    SIPURI & target,
    BOOL incrementCurrentRoute = TRUE,
    BOOL compareMethods = FALSE
  );

  BOOL HasScheme(
    const SIPMessage & request,
    const PString & scheme
  );

  BOOL AppendRoute(
    const PString & route
  );

  PINLINE Routes GetRoutes()
  { 
    PWaitAndSignal lock( m_RoutesMutex );
    return m_Routes; 
  };

protected:
  PMutex m_RoutesMutex;
  Routes m_Routes;
};
    

#endif

