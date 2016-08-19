/*
 *
 * CacheManager.h
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
 * $Log: CacheManager.h,v $
 * Revision 1.5  2006/12/20 16:24:44  joegenbaclor
 * Implemented keep-alives for upper reg.
 *
 * Revision 1.4  2006/11/30 08:19:35  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include "CacheObject.h"

namespace Cache
{
  class CacheManager : public PThread
  {
    PCLASSINFO( CacheManager, PThread );
  public:

    CacheManager(
      PINDEX cacheMaxSize = P_MAX_INDEX,
      PINDEX stackSize = 1024 * 2
    );

    virtual ~CacheManager();

    BOOL Append(
      CacheObject * cacheObject
    );

    CacheObject * GetAt(
      const PCaselessString & objectIdentifier
    );

    CacheObject * GetAt(
      PINDEX index
    );

    CacheObject * RemoveAt(
      const PCaselessString & objectIdentifier,
      BOOL autoDelete = TRUE
    );

    void Main();

    PINLINE PINDEX GetSize()const{ return m_Cache.GetSize(); };
    PINLINE CacheObject::SortedCollection & GetCache(){ return m_Cache; };
    PINLINE PINDEX GetMaximumSize()const{ return m_CacheMaxSize; };

  protected:

    PMutex m_CacheMutex;
    PSyncPoint m_ExitFlag;
    CacheObject::SortedCollection m_Cache;
    PINDEX m_CacheMaxSize;

  };
};

#endif




