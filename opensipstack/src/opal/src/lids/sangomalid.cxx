/*
 * sangomalid.cxx
 *
 * Sangoma Line Interface Device
 *
 * Open Phone Abstraction Library
 *
 * Copyright (C) 2005 Post Increment
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
 * $Log: sangomalid.cxx,v $
 * Revision 1.1  2006/06/26 03:03:21  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 1.1  2005/12/06 06:34:10  csoutheren
 * Added configure support for Sangoma and empty LID source and header files
 *
 */

#define __LINUX__
#define _LINUX_IF_H

#include "/usr/local/include/libsangoma.h"

#include <lids/sangomalid.h>



