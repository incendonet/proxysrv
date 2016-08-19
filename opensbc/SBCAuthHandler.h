/*
 *
 * SBCAuthHandler.h
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
 * Contributor(s): ______________________________________.
 *
 * $Log: SBCAuthHandler.h,v $
 * Revision 1.1  2006/08/14 10:04:59  rcolobong
 * Convert B2BUA to SBC
 * Support MP logging
 *
 * Revision 1.2  2006/07/26 09:19:41  joegenbaclor
 * Added System Log support
 *
 * Revision 1.1  2006/07/11 14:34:38  joegenbaclor
 * Initial upload
 *
 *
 */

#ifndef SBCAUTHHANDLER_H
#define SBCAUTHHANDLER_H

#include "OpenSBC.h"
#include "B2BAuthInterface.h"


class SBCAuthHandler : public B2BAuthInterface
{
  PCLASSINFO( SBCAuthHandler, B2BAuthInterface );
public:

  SBCAuthHandler(  
    OpenSBC & b2bua 
  );

  virtual ~SBCAuthHandler();

  virtual B2BAuthResponse B2BAuthenticate(
    B2BUAConnection & connection,
    const SIPMessage & invite,
    ProxyAuthenticate & auth
  );

  virtual BOOL RequireAuthorizationHash(
    const SIPURI & userURL,
    PString & a1,
    RegisterSession & session 
  );
};


#endif

