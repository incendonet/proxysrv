/*
 * sangomalid.h
 *
 * Sangoma PRI Line Interface Device
 *
 * Copyright (C) 2005 by Post Increment
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
 * The Original Code is Open Phone Abstraction Library
 *
 * The Initial Developer of the Original Code is Post Increment
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: sangomalid.h,v $
 * Revision 1.1  2006/08/04 03:33:18  joegenbaclor
 * Moved pwlib and OPAL headers to root include directory
 *
 * Revision 1.1  2006/06/26 03:02:28  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 1.1  2005/12/06 06:34:10  csoutheren
 * Added configure support for Sangoma and empty LID source and header files
 *
 */

#ifndef __OPAL_SANGOMALID_H
#define __OPAL_SANGOMALID_H

#ifdef P_USE_PRAGMA
#pragma interface
#endif

#include <opal/buildopts.h>

#ifdef HAS_SANGOMA

#include <ptlib.h>
#include <lids/lid.h>

//#define OPAL_VPB_TYPE_NAME  "Sangoma"
//#define OPAL_REGISTER_SANGOMA() OPAL_REGISTER_LID(OpalSangomaDevice, OPAL_SANGOMA_TYPE_NAME)

#else // HAS_SANGOMA

//#define OPAL_REGISTER_SANGOMA()

#endif // HAS_SANGOMA

#endif // __OPAL_SANGOMALID_H


/////////////////////////////////////////////////////////////////////////////
