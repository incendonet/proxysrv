/*
 *
 * RFC3680Package.h
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
 * $Log: RFC3680Package.h,v $
 * Revision 1.2  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/10/30 02:15:52  joegenbaclor
 * Initial upload of RFC3680 files
 *
 *
 */


#ifndef RFC3680PACKAGE_H
#define RFC3680PACKAGE_H

#include "RFC3265EventPackage.h"
#include "RFC3265StateAgent.h"
#include "RFC3265Notification.h"
#include "RFC3680Resource.h"
#include "RFC3265Subscription.h"

using namespace RFC3265;

namespace RFC3680
{
  class RFC3680Package : public RFC3265EventPackage
  {
    PCLASSINFO( RFC3680Package, RFC3265EventPackage );
  public:

    RFC3680Package(
      RFC3265StateAgent & stateAgent
    );

    ~RFC3680Package();

    PString GetPackageName()const{ return "reg"; };
    
    PINDEX GetRFCNumber() const{ return 3680; };

    void PrintOn( ostream & strm )const{ strm << GetPackageName() << "-" << GetRFCNumber(); };

    virtual void OnUserOnline(
      RFC3680Resource & resource
    );

    virtual void OnUserOffline(
      RFC3680Resource & resource
    );

    /// virtuals from RFC3265Event Package

    virtual RFC3265Resource * OnCreateResource(
      const SIPURI & resourceURI
    );

    virtual BOOL OnAuthorizeSubscription(
      RFC3265Resource & resource,
      RFC3265Subscription * subscription
    );

    virtual BOOL OnAuthenticateSubscription(
      RFC3265Resource & resource,
      RFC3265Subscription * subscription
    );

    virtual BOOL OnNewSubscription(
      RFC3265Resource & resource,
      RFC3265Subscription * subscription
    );
  };
};

#endif





