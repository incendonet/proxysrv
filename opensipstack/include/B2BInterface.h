/*
 *
 * B2BInterface.h
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
 * $Log: B2BInterface.h,v $
 * Revision 1.2  2006/11/30 08:19:35  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 *
 */

#ifndef B2BINTERFACE_H
#define B2BINTERFACE_H

#include "ptlib.h"
#include "Logger.h"

using namespace Tools;

namespace B2BUA
{

  class B2BUserAgent;

  class B2BInterface : public PObject, public Logger
  {
    PCLASSINFO( B2BInterface, PObject );

  public:
    
    B2BInterface( 
      B2BUserAgent & b2bua
    ) : m_B2BUA( b2bua ){}

    virtual ~B2BInterface(){};

    PINLINE B2BUserAgent &GetB2BUA(){ return m_B2BUA; };
  protected:

    B2BUserAgent & m_B2BUA;

  };

}

#endif

