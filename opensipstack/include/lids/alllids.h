/*
 * alllids.h
 *
 * Open Phone Abstraction Library (OPAL)
 * Formally known as the Open H323 project.
 *
 * Copyright (c) 2001 Equivalence Pty. Ltd.
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is Open Phone Abstraction Library.
 *
 * The Initial Developer of the Original Code is Equivalence Pty. Ltd.
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: alllids.h,v $
 * Revision 1.1  2006/08/04 03:33:18  joegenbaclor
 * Moved pwlib and OPAL headers to root include directory
 *
 * Revision 1.1  2006/06/26 03:02:27  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 2.2  2004/10/06 13:03:40  rjongbloed
 * Added "configure" support for known LIDs
 * Changed LID GetName() function to be normalised against the GetAllNames()
 *   return values and fixed the pre-factory registration system.
 * Added a GetDescription() function to do what the previous GetName() did.
 *
 * Revision 2.1  2003/03/24 07:18:29  robertj
 * Added registration system for LIDs so can work with various LID types by
 *   name instead of class instance.
 *
 */

#ifndef __LIDS_ALLLIDS_H
#define __LIDS_ALLLIDS_H

#include <opal/buildopts.h>

#include <lids/ixjlid.h>
OPAL_REGISTER_IXJ();

#include <lids/vpblid.h>
OPAL_REGISTER_VPB();

#include <lids/vblasterlid.h>
OPAL_REGISTER_VBLASTER();


#endif // __LIDS_ALLLIDS_H


/////////////////////////////////////////////////////////////////////////////
