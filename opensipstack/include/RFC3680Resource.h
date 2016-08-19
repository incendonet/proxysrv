/*
 *
 * RFC3680Resource.h
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
 * $Log: RFC3680Resource.h,v $
 * Revision 1.2  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/10/30 02:15:52  joegenbaclor
 * Initial upload of RFC3680 files
 *
 *
 */

#ifndef RFC3680RESOURCE_H
#define RFC3680RESOURCE_H

#include "RFC3265Resource.h"

using namespace RFC3265;

namespace RFC3680
{
  class RFC3680Resource : public RFC3265Resource
  {
    PCLASSINFO( RFC3680Resource, RFC3265Resource );
  
  public:

    RFC3680Resource(
      const SIPURI & resourceURI,
      RFC3265EventPackage & eventPackage
    );

    virtual ~RFC3680Resource();

    virtual void OnUserOnline();

    virtual void OnUserOffline();

    virtual BOOL OnAuthorizeSubscription(
      RFC3265Subscription * subscription
    );

    virtual BOOL OnAuthenticateSubscription(
      RFC3265Subscription * subscription
    );

    virtual BOOL OnNewSubscription(
      RFC3265Subscription * subscription
    );

    /// pure virtuals
    virtual BOOL PollEvents();
    virtual void Cleanup();

  };
 

};

#endif






