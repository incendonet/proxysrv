/*
 *
 * RFC3265StateAgent.h
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
 * $Log: RFC3265StateAgent.h,v $
 * Revision 1.3  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.2  2006/10/30 02:42:26  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.1  2006/05/03 03:28:54  joegenbaclor
 * Initial upload of RFC3265 and MWI files
 *
 *
 */


#ifndef RFC3265STATEAGENT_H
#define RFC3265STATEAGENT_H

#include "RFC3265EventPackage.h"
#include "SIPUserAgent.h"

using namespace SIP;

namespace RFC3265
{
  ///////////////////////////////////////////////////////////////////////

  class RFC3265StateAgent : public PObject
  {
    /*
    State Agent: A state agent is a notifier which publishes state
    information on behalf of a resource; in order to do so, it may
    need to gather such state information from multiple sources.
    State agents always have complete state information for the
    resource for which they are creating notifications.
    */
    PCLASSINFO( RFC3265StateAgent, PObject );
  public:
    RFC3265StateAgent(
      SIPUserAgent & ua
    );

    virtual ~RFC3265StateAgent();

    BOOL AddPackage(
      const PCaselessString & name,
      RFC3265EventPackage * package
    );

    RFC3265EventPackage * RemovePackage(
      const PCaselessString & name,
      BOOL autoDelete = FALSE
    );

    RFC3265EventPackage * FindPackage(
      const PCaselessString & name
    );

    RFC3265Subscription::GCRef Subscribe(
      const PNotifier & notifier,
      const PString & event,
      const PString & callId,
      const SIPURI & subscriber,
      const SIPURI & resource
    );

    BOOL Unsubscribe(
      RFC3265Subscription * subscription,
      BOOL autoDelete = TRUE
    );

    PINLINE SIPUserAgent & GetUserAgent(){ return m_UserAgent; };

  protected:

    PDICTIONARY( RFC3265EventPackageList, PCaselessString, RFC3265EventPackage );
    PMutex m_PackagesMutex;
    RFC3265EventPackageList m_Packages;
    SIPUserAgent & m_UserAgent;
  };

};

#endif





