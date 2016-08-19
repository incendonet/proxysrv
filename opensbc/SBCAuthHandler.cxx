/*
 *
 * SBCAuthHandler.cxx
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
 * $Log: SBCAuthHandler.cxx,v $
 * Revision 1.3  2007/01/17 00:08:55  joegenbaclor
 * Added SysLog
 *
 * Revision 1.2  2006/08/29 07:45:42  rcolobong
 * Remove warning messages in Windows
 *
 * Revision 1.1  2006/08/14 10:04:59  rcolobong
 * Convert B2BUA to SBC
 * Support MP logging
 *
 * Revision 1.1  2006/07/11 14:34:38  joegenbaclor
 * Initial upload
 *
 *
 */

#include "B2BUA.h"
#include "SBCAuthHandler.h"

#define new PNEW

using namespace B2BUA;

static const char configKeySection[] = "Local Domain Accounts";
static const char configKeyAccount[] = "Account";

SBCAuthHandler::SBCAuthHandler(  
  OpenSBC & b2bua 
) : B2BAuthInterface( b2bua )
{
}

SBCAuthHandler::~SBCAuthHandler()
{
}

SBCAuthHandler::B2BAuthResponse SBCAuthHandler::B2BAuthenticate(
  B2BUAConnection &,
  const SIPMessage &,
  ProxyAuthenticate &
)
{
  /// just accept every call
  return SBCAuthHandler::AuthAccept;
}

BOOL SBCAuthHandler::RequireAuthorizationHash(
  const SIPURI & userURL,
  PString & a1,
  RegisterSession & 
)
{
  /// dont process the auth if sbc wants to accept all regs
  if( dynamic_cast<OpenSBC&>(GetB2BUA()).WillAcceptAllRegistration() )
    return TRUE;

  OSSAppConfig * config = OSSAppConfig::GetInstance();
  PStringArray accounts;
  for( PINDEX i = 0; i <  config->GetListSize( configKeySection, configKeyAccount ); i++ )
    accounts.AppendString( config->GetListItem( configKeySection, configKeyAccount, i ) );

  for( PINDEX j = 0; j < accounts.GetSize(); j++ )
  {
    SIPURI account( accounts[j] );

    if( account *= userURL )
    {
      a1 = account.AsA1Hash();
      return TRUE;
    }
  }

  return FALSE;
}

