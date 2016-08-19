/*
 *
 * OpenSIPStack.h
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
 * $Log: OpenSIPStack.h,v $
 * Revision 1.6  2006/12/12 05:26:08  joegenbaclor
 * Modified DTMF player to use PDTMFEncoder
 *
 * Revision 1.5  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/10/30 08:35:55  joegenbaclor
 * Removed more references to presence namespace
 *
 * Revision 1.3  2006/08/29 03:56:43  joegenbaclor
 * 1.  Added GC Reference in SIP Event
 * 2.  Corrected buf in ProcessStackEvent where session is checked against NULL not allowing requests to create new sessions
 *
 * Revision 1.2  2006/08/23 03:33:53  joegenbaclor
 * Some Opal related bug fixes discovered duriong intial development of ATLSIP
 *
 * Revision 1.1  2006/06/08 09:05:08  joegenbaclor
 * Initial upload
 *
 *
 */

#ifndef OPENSIPSTACK_H
#define OPENSIPSTACK_H

#include <ptlib.h>
#include "ossbuildopts.h"
#include "CallSessionManager.h"
#include "ProxySessionManager.h"
#include "RedirectSessionManager.h"
#include "RegisterSessionManager.h"
#include "SIPSessionManager.h"
#include "OpalOSSEndPoint.h"
#include "RFC3265StateAgent.h"

#endif