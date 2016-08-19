/*
 *
 * CacheManager.cxx
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
 * $Log: CacheManager.cxx,v $
 * Revision 1.8  2006/12/20 16:24:46  joegenbaclor
 * Implemented keep-alives for upper reg.
 *
 * Revision 1.7  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/03/27 15:25:10  joegenbaclor
 * Last minute improvements before creating a new tarball for download including
 * 1.  Major bug fix in simple-proxy where events may leak for REGISTER transactions.
 * 2.  Used incremental logging in HTTP admin.
 * 3.  Removed unused parameters in HTTPAdmin.
 * 4.  Local domain calls are now record-routed.
 * 5.  Fixed bug where ACK routeset is not popped during relay.
 *
 * Revision 1.5  2006/03/27 01:38:11  joegenbaclor
 * Removed some test related codes
 *
 * Revision 1.4  2006/03/25 18:40:21  joegenbaclor
 * 1.  Worked on some race conditions on Finite State Machines during heavy load.
 * 2.  Intoduced Global Mutex to SIPMessage.  This is inevitable beacuse PDictionary and PList members are not thread-safe
 * 3,  Some minor improvements to simple-proxy application
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */


#include "CacheManager.h"

#define new PNEW

using namespace Cache;

CacheManager::CacheManager(
  PINDEX cacheMaxSize,
  PINDEX stackSize
) : PThread( stackSize, NoAutoDeleteThread, NormalPriority, "Cache Manager" ),
    m_CacheMaxSize( cacheMaxSize )
{
  m_Cache.DisallowDeleteObjects();
  Resume();
}

CacheManager::~CacheManager()
{
  m_Cache.AllowDeleteObjects();
  m_ExitFlag.Signal();
  WaitForTermination();
}

BOOL CacheManager::Append(
  CacheObject * cacheObject
)
{
  //PAssertNULL( cacheObject );
  PWaitAndSignal lock( m_CacheMutex );

  if( m_CacheMaxSize <= m_Cache.GetSize() )
    return FALSE;

  m_Cache.SetAt( cacheObject->GetObjectIdentifier(), cacheObject );
  return TRUE;
}

CacheObject * CacheManager::GetAt(
  const PCaselessString & objectIdentifier
)
{
  PWaitAndSignal lock( m_CacheMutex );
  return m_Cache.GetAt( objectIdentifier );
}

CacheObject * CacheManager::GetAt(
  PINDEX index
)
{
  PWaitAndSignal lock( m_CacheMutex );

  if( index >= m_Cache.GetSize() )
    return NULL;

  return GetAt( m_Cache.GetKeyAt( index ) );
}

CacheObject * CacheManager::RemoveAt(
  const PCaselessString & objectIdentifier,
  BOOL autoDelete
)
{
  PWaitAndSignal lock( m_CacheMutex );
  CacheObject * obj = m_Cache.RemoveAt( objectIdentifier );

  if( autoDelete && obj != NULL )
  {
    delete obj;
    return NULL;
  }

  return obj;
}

void CacheManager::Main()
{
  while( !m_ExitFlag.Wait( 1000 ) )
  {
    PWaitAndSignal lock( m_CacheMutex );
    CacheObject::Collection expiredObjects;
    for( PINDEX i = 0; i < m_Cache.GetSize(); i++ )
    {
      CacheObject & obj = m_Cache.GetDataAt( i );
      if( obj.DecreaseLifeSpan() == 0 )
      {
        CacheObject * expiredObject = m_Cache.RemoveAt( obj.GetObjectIdentifier() );
        expiredObjects.Append( expiredObject );
      }
    }
  }
  
  m_Cache.AllowDeleteObjects();
}


