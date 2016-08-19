/*
 *
 * B2BAuthInterface.cxx
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
 * $Log: B2BAuthInterface.cxx,v $
 * Revision 1.6  2007/01/14 13:12:14  joegenbaclor
 * Fixed compile wranings
 *
 * Revision 1.5  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/10/30 02:42:27  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.3  2006/07/10 06:29:38  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.2  2006/07/07 09:54:14  joegenbaclor
 * Added Register Authorization callbacks
 *
 * Revision 1.1  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 *
 */

#include "B2BUA.h"
#include "B2BAuthInterface.h"

#define new PNEW

using namespace B2BUA;

B2BAuthInterface::B2BAuthInterface(  
  B2BUserAgent & b2bua 
) : B2BInterface( b2bua )
{
}

B2BAuthInterface::~B2BAuthInterface()
{
}

B2BAuthInterface::B2BAuthResponse B2BAuthInterface::B2BAuthenticate(
  B2BUAConnection & connection,
  const SIPMessage & invite,
  ProxyAuthenticate & auth
)
{
  /// just accept every call
  return B2BAuthInterface::AuthAccept;
}

BOOL B2BAuthInterface::RequireAuthorizationHash(
  const SIPURI & userURL,
  PString & a1,
  RegisterSession & session 
)
{
  return TRUE;
}

BOOL B2BAuthInterface::IsKnownUser(
  const SIPURI userURI
)const
{
  return FALSE;
}

