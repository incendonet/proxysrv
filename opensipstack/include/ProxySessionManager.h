/*
 *
 * ProxySessionManager.h
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
 * $Log: ProxySessionManager.h,v $
 * Revision 1.13  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.12  2006/08/30 03:42:04  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.11  2006/05/19 06:30:37  joegenbaclor
 * 1.  Fixed bug in SIPHeaderB where tag may not be parsed properly if no < > enclosure is present
 * 2.  Various enhancements to OpenSBC and proxy session
 *
 * Revision 1.10  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.9  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.8  2006/03/24 15:42:13  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.7  2006/03/22 08:45:52  joegenbaclor
 * More work on simple-proxy and affected classes
 *
 * Revision 1.6  2006/03/14 17:00:00  joegenbaclor
 * Implemented Dialog methods in SIPSession object
 *
 * Revision 1.5  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef PROXYSESSIONMANAGER_H
#define PROXYSESSIONMANAGER_H

#include "SIPSessionManager.h"
#include "ProxySession.h"
#include "CacheManager.h"
#include "Contact.h"


using namespace SIP;
using namespace Profiles;
using namespace Cache;
using namespace SIPParser;

namespace UACORE
{
  ///////////////////////////////////////////////////////////////

  class ProxySessionTupleManager : public CacheManager
  {
    PCLASSINFO( ProxySessionTupleManager, CacheManager );

  public:

    ProxySessionTupleManager();

    BOOL RemoveTuple(
      const SIPMessage & routedMsg
    );

    BOOL CreateTuple(
      const SIPMessage & senderMsg,
      const SIPMessage & routedMsg,
      int expire = 30000
    );

    BOOL FindTuple(
      const PString & callId,
      SIPMessage & originalRequest,
      SIPMessage & routedRequest
    );

  protected:

    class Tuple : public PObject
    {
      PCLASSINFO( Tuple, PObject );

    public:
      Tuple();

      Tuple( 
        const Tuple & tuple 
      );

      Tuple(
        const SIPMessage & originalRequest,
        const SIPMessage & routedRequest
      );

      virtual ~Tuple();

      Tuple & operator=( const Tuple & tuple );

      PINLINE const SIPMessage & GetOriginalRequest()const{ return m_OriginalRequest; };
      PINLINE const SIPMessage & GetRoutedRequest()const{ return m_RoutedRequest; };
     
    protected:
      
      SIPMessage m_OriginalRequest;
      SIPMessage m_RoutedRequest;

    };
  };

  ///////////////////////////////////////////////////////////////

  class ProxySessionManager : public SIPSessionManager, public ProxySessionTupleManager
  {
    PCLASSINFO( ProxySessionManager, SIPSessionManager );
  public:


    ProxySessionManager(
      SIPUserAgent & ua,
      PINDEX sessionThreadCount,
      PINDEX stackSize
    );

    virtual ~ProxySessionManager();

    virtual SIPSession::GCRef OnCreateServerSession(
      const SIPMessage & request
    );

    ProxySession::AuthStatus OnCheckAuthorization(
      ProxySession & session,
      const SIPMessage & request
    );

    virtual ProxySession::RoutePolicy OnCheckRoutePolicy(
      ProxySession & session,
      SIPMessage & request
    );

    virtual void OnProvisionalResponse(
      ProxySession & session,
      const SIPMessage & request
    );

    virtual void OnFinalResponse(
      ProxySession & session,
      const SIPMessage & request
    );

    virtual void OnProcessRequestAsLocal(
      ProxySession & session,
      const SIPMessage & request
    );

    void OnUnknownTransaction(
      SIPUnknownTransaction & event
    );

    virtual void OnOrphanedMessage(
      const SIPMessage & message
    );
  };
  
};

#endif

