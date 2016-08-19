/*
 *
 * RouteRecord.h
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
 * $Log: RouteRecord.h,v $
 * Revision 1.3  2007/01/22 10:00:58  joegenbaclor
 * Fixed ProxyRouteRequest
 *
 * Revision 1.2  2006/11/22 11:33:25  rcolobong
 * 1. Change method FindRoute to HasScheme
 * 2. Fix problem where it "Rounds Robin" in HasScheme method
 *
 * Revision 1.1  2006/08/14 10:04:59  rcolobong
 * Convert B2BUA to SBC
 * Support MP logging
 *
 * Revision 1.2  2006/07/11 13:59:34  joegenbaclor
 * Completed Proxy functionalities for B2BUA
 *
 * Revision 1.1  2006/06/20 09:58:11  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.1  2006/05/17 04:04:48  joegenbaclor
 * Initial upload of OpenSBC files
 *
 *
 */

#ifndef ROUTERECORD_H
#define ROUTERECORD_H

#include "SIPURI.h"

using namespace SIPParser;

class RouteRecord : public PObject
{
  PCLASSINFO( RouteRecord, PObject );
public:

  RouteRecord();

  RouteRecord( 
    const PString & routeRec
  );

  RouteRecord( 
    const RouteRecord & routeRec
  );

  RouteRecord & operator=(
    const PString & routeRec
  );

  RouteRecord & operator=(
    const RouteRecord & routeRec
  );

  BOOL SetRouteURI(
    const PString & uri
  );
  
  const PString & GetRouteURI()const;

  BOOL GetTargetURI( 
    SIPURI & target
  );

  BOOL GetTargetURI( 
    SIPURI & target,
    PINDEX index
  );

  BOOL AppendTargetURI(
    const SIPURI & target
  );

  BOOL RemoveTargetURI(
    PINDEX index
  );

  BOOL HasScheme(
    const PString& scheme
  );

  BOOL operator*=(
    const SIPURI & uri
  );

  BOOL operator*=(
    const PString & uri
  );

  PINDEX GetTargetURICount()const;

  PStringArray GetTargetURIList()const;

  BOOL Parse( 
    const PString & routeRec
  );

  virtual void PrintOn( ostream & strm )const;


protected:

  PString m_RouteURI;
  PStringArray m_Targets;
  PIntArray m_TargetsSubIndex;
  PAtomicInteger m_CurrentTargetIndex;
};

#endif




