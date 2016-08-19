/*
 *
 * GCObject.h
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
 * $Log: GCObject.h,v $
 * Revision 1.21  2007/01/17 10:33:59  joegenbaclor
 * Redesigned syslog
 *
 * Revision 1.20  2006/12/19 10:19:47  rcolobong
 * Logging stale objects will only be log in debug mode
 *
 * Revision 1.19  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.18  2006/09/22 04:54:52  joegenbaclor
 * Deprecate PrintAllReferences
 *
 * Revision 1.17  2006/09/18 09:35:15  joegenbaclor
 * Completed initial implementation of RTP media aggregation
 *
 * Revision 1.16  2006/09/18 03:28:54  joegenbaclor
 * 1. More work on media aggregation
 * 2. Check sock->GetPort() for NULL in SIPTransportManager::GetListenerAddress() (thanks to
 *  Vamsi Pottangi for reporting the bug)
 * 3.  Added reference size check to GCObject::ReleaseRef()
 *
 * Revision 1.15  2006/09/15 15:31:39  joegenbaclor
 * Changed Log levels to decrease log size.
 * Changed default session keep alaive interval from 10 seconds to two minutes
 *
 * Revision 1.14  2006/09/06 07:50:08  joegenbaclor
 * Corrected compile error in linux for GCCREATECOLLECTOR macro
 *
 * Revision 1.13  2006/09/06 07:36:38  joegenbaclor
 * Revised garbage collector so that each Garbage collectible objects will have its own
 *  instance of the GC.  This eliminates the need to cast to PObject
 *
 * Revision 1.12  2006/09/04 16:36:23  joegenbaclor
 * changed dynamic_cast to static_cast when casting PObject to GCObject
 *
 * Revision 1.11  2006/09/04 02:58:52  joegenbaclor
 * 1.  More bug fixes to garbage collector
 * 2.  Added new Logging macro so call System Log for fatal errors
 *
 * Revision 1.10  2006/09/02 17:58:18  joegenbaclor
 * Corrected compile errors in GCC regarding euqation of NULL to GCRef.
 *
 * Revision 1.9  2006/09/02 14:30:25  joegenbaclor
 * using POrdinalKey as the GC Collector database key may result to pointer truncation
 *  in unix where void * is bigger than int.  The solution applied is to create a new
 *  GCOrdinalKey that wraps an INT
 *
 * Revision 1.8  2006/09/01 03:00:32  joegenbaclor
 * Bulk Commit:
 *
 * 1.  A bug is discovered in SIP Transaction Destructor where destruction of the SIPTimers may cause a deadlock on the timer manager effectivily freezing the entire message Queue System.  This was fixed by introducing a separate function to destroy the timers on receipt of the final event just before the transaction is queued for deletion.
 *
 * 2.  There is bug in Invite Server Transaction where ACK transaction is added to the Ack Transaction Pool after sending og none 2xx final response.  This new transaction is never removed from the pool after Timer_H has fired.  this may result to a bad pointer if ACK is received after the timer expired since the transaction may have been deleted by then.
 *
 * 3.  A few tweaks in GC GetName func to return the Class Name if GC NAme is empty.
 *
 * Revision 1.7  2006/08/30 08:45:08  rcolobong
 * Overload GCCreateRef method for GCREFINFO macro wherein you can pass GCObject as parameter and return the casted object you prefer
 *
 * Revision 1.6  2006/08/30 05:40:14  rcolobong
 * Fixed pragma warning by define guards
 *
 * Revision 1.5  2006/08/30 03:42:04  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.4  2006/08/29 10:46:05  rcolobong
 * *** empty log message ***
 *
 * Revision 1.3  2006/08/26 14:31:40  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.2  2006/08/25 18:17:57  joegenbaclor
 * Added GC singleton and macro definitions
 *
 * Revision 1.1  2006/08/25 16:24:41  joegenbaclor
 * Initial upload of Gartbage Collector Object
 *
 */

#ifndef GCOBJECT_H
#define GCOBJECT_H


#include <ptlib.h>
#include "Logger.h"

#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4311 )
#pragma warning(disable:4312)
#endif // _MSC_VER

using namespace Tools;

namespace GC
{
  


  /*Sample Usage:
    Tools::GC::Collector collector;
    PString * myString = new PString( "Hello world!" );
    Tools::GC::GCReference<PString> gcRef = collector.CreateReference<PString>( myString, "", "My Reference Name" );
    Tools::GC::GCReference<PString> gcRef2 = collector.CreateReference<PString>( myString, "", "My Reference Name" );
    gcRef.PrintReferences( cout );
    collector.DestroyObject<PString>( myString );
  */

  template <class T>
  class GCReference;
  
  /**This class is used when an ordinal index value is the key for #PSet#
   and #PDictionary# classes.
 */

  template <class T>
  class GCOrdinalKey : public PObject
  {
    PCLASSINFO(GCOrdinalKey, PObject);

    public:

      GCOrdinalKey(
        T * newKey = NULL   ///< Ordinal index value to use as a key.
      )
      { 
        theKey = PString( (INT)newKey );
        thePointer = newKey;
      }

      GCOrdinalKey & operator=(T * newKey)
      { 
        theKey = PString( (INT)newKey );
        thePointer = newKey;
        return *this;
      };

      PObject * Clone() const{ 
        return new GCOrdinalKey( thePointer ); 
      };

      Comparison Compare(const PObject & obj) const
      {
        PAssert(PIsDescendant(&obj, GCOrdinalKey), PInvalidCast);
        const GCOrdinalKey & other = (const GCOrdinalKey &)obj;
        
        if ( thePointer < other.thePointer)
          return LessThan;

        if ( thePointer > other.thePointer)
          return GreaterThan;

        return EqualTo;
      }

      PINDEX HashFunction() const
      {
        return theKey.HashFunction();
      }

      void PrintOn(ostream & strm) const
      {
        strm << std::hex << thePointer;
      }

      operator T *() const
      {
        return thePointer;
      }


    protected:
      PString theKey;
      T * thePointer;
  };

  template <class T>
  class GCObject : public PObject
  {
    PCLASSINFO( GCObject<T>, PObject );

    protected:
    T * m_Object;
    
    GCObject( 
      const PString & name = PString::Empty() 
    )
    {
      m_Name = name;
      m_Object = NULL;
      m_TotalReferences = 0;
      m_RefCount = 0; 
    };

    GCObject( 
      T * object,
      const PString & name = PString::Empty() 
    )
    {
      m_Name = name;
      m_RefCount = 0;
      m_Object = object;
      m_TotalReferences = 0;
    };

    ~GCObject()
    {
      if( m_Object != NULL )
        delete m_Object; 
    };

    T * GetObject(){ return m_Object; };
    GCReference<T> CreateReference(){ return GCReference<T>( this ); };
    PString GetName()const{ return m_Name.IsEmpty() ? PString( Class() ) : m_Name; };
    void AddRef( PString & refName )
    {
      PWaitAndSignal lock( m_RefMutex );
      
      if( m_Object == NULL )
        return;

      ++m_RefCount;
      ++m_TotalReferences;
      refName = refName 
        + PString( " (" ) 
        + PString( m_TotalReferences )
        + PString( ")" );

      m_References+=refName;
    }
    void ReleaseRef( const PString & refName )
    {
      PWaitAndSignal lock( m_RefMutex );

      if( m_Object == NULL )
        return;

      if( --m_RefCount > 0 )
        m_References-=refName;
    }

    void PrintReferences( ostream & strm )const
    {
      PString name = m_Name;

      if( name.IsEmpty() )
        name = "?";

      for( PINDEX i = 0; i < m_References.GetSize(); i++ )
        strm << "GCObject: " <<  name <<  " --> " << *m_References.GetAt( i ) << endl;
    }

  private:
    PMutex m_RefMutex;
    PAtomicInteger m_RefCount;
    int m_TotalReferences;
    PString m_Name;
    PStringSet m_References;
    
    template<class> friend class GCReference;
    template<class> friend class Collector;
  };

  template <class T>
  class GCReference : public PObject
  {
    PCLASSINFO( GCReference<T>, PObject );
  protected:
    GCObject<T> * m_GCObject;
  public:
    GCReference( 
      const PString & refName = PString::Empty() 
    )
    {
      m_Name = refName;
      m_GCObject = NULL;
    };

    GCReference( 
      GCObject<T> * gcObject, 
      const PString & refName
    )
    { 
      m_Name = refName;
      m_GCObject = gcObject; 
      if( m_GCObject != NULL )
        m_GCObject->AddRef( m_Name );  
    };

    GCReference( 
      const GCReference<T> & gcRef
    )
    {
      m_GCObject = NULL;
      operator=( gcRef );
    };
    
    ~GCReference()
    {
      if( m_GCObject != NULL )
        m_GCObject->ReleaseRef( m_Name ); 
    };
    
    T * operator->()
    { 
      return m_GCObject->GetObject(); 
    };
    
    T * GetObject()
    { 
      return m_GCObject == NULL ? NULL : m_GCObject->GetObject();  
    };
    
    GCReference & operator=( const GCReference<T> & ref )
    {
      if( m_GCObject != NULL )
        m_GCObject->ReleaseRef( m_Name );

      m_GCObject = ref.m_GCObject; 
      m_Name = ref.m_Name;

      if( m_GCObject != NULL )
        m_GCObject->AddRef( m_Name );
      
      return *this;
    };

    GCReference & operator=(PObject*obj)
    {
      if( m_GCObject != NULL )
        m_GCObject->ReleaseRef( m_Name );
      
      m_GCObject = PNEW GCObject<T>( obj );
      if( m_GCObject != NULL )
        m_GCObject->AddRef( m_Name );

      return *this;
    }

    BOOL operator==(PObject*obj)
    {
      return obj == GetObject();
    }

    BOOL operator!=(PObject*obj)
    {
      return obj != GetObject();
    }

    void PrintReferences( ostream & strm )const
    { 
      if( m_GCObject == NULL )
        return;

      m_GCObject->PrintReferences( strm );
    }

    PString GetName()const{ return m_Name.IsEmpty() ? PString( Class() ) : m_Name; };
  protected:
    PString m_Name;
    template<class> friend class Collector;
    template<class> friend class GCObject;
  };


  template <class T>
  class Collector : public PThread, public Logger
  {
    PCLASSINFO( Collector<T>, PThread );
  public:
    Collector( const PTimeInterval & frequency = 5000, PINDEX heapSize = 1024 * 10 ) 
      : PThread( heapSize, PThread::NoAutoDeleteThread, PThread::NormalPriority, "Garbage Collector" )
    {
      m_CurrentObjects.DisallowDeleteObjects();
      m_StaleObjects.DisallowDeleteObjects();
      m_ExitFlag = FALSE;
      m_Frequency = frequency;
      Resume();
    };

    ~Collector()
    {
      m_ExitFlag = TRUE;
      m_GCSyncPoint.Signal();
      WaitForTermination();
    }

  public:


    GCReference<T> CreateReference( 
      T * obj,
      const PString & referenceName = PString::Empty() ,
      const PString & objName = PString::Empty()
    )
    { 
      PWaitAndSignal lock( m_GCMutex );
      /// check current objects if obj is already in the database
      T * objKey = dynamic_cast<T*>( obj );
      if( objKey == NULL )
      {
        LOG( LogError(), "GC: Attempt to CreateReference() a NULL Pointer or none descendant of PObject!!!" );
        return GCReference<T>( NULL, referenceName );
      }

      GCOrdinalKey<T> key = objKey; 
      GCObject<T> * gcObj = NULL;

      if( m_CurrentObjects.Contains( key ) )
      {
        gcObj = dynamic_cast<GCObject<T>*>(m_CurrentObjects.GetAt( key ));
        
        if( gcObj == NULL )
          PAssertAlways( PLogicError );
        
        return GCReference<T>( gcObj, referenceName );  
      }

      gcObj = PNEW GCObject<T>( obj, objName );
      m_CurrentObjects.SetAt( key, dynamic_cast<gcObject*>(gcObj) );
      return GCReference<T>( gcObj, referenceName ); 
    };

    void DestroyObject( T * obj )
    {
      PWaitAndSignal lock( m_GCMutex );
      /// check current objects if obj is already in the database
      
      T * objKey = dynamic_cast<T*>( obj );
      if( objKey == NULL )
      {
        LOG( LogError(), "GC: Attempt to DestroyObject() a NULL Pointer or none descendant of PObject!!!" );
        return;
      }
      GCOrdinalKey<T> key = objKey;
      GCObject<T> * gcObj = NULL;

      if( m_CurrentObjects.Contains( key ) )
      {
        gcObj = dynamic_cast<GCObject<T>*>(m_CurrentObjects.RemoveAt( key ));
        
        if( gcObj == NULL )
          PAssertAlways( PLogicError );

        if( gcObj->m_RefCount == 0 )
        {
          LOG_IF_DEBUG( LogDebug(), "GC: First DELETE  ---> " <<  gcObj->GetName() );
          delete gcObj;
        }else
        {
          LOG_IF_DEBUG( LogDebug(), "GC:  First Stale Object " << gcObj->GetName() );
          m_StaleObjects.Enqueue( dynamic_cast<gcObject*>(gcObj) );
          m_GCSyncPoint.Signal();
        }
      }else /// object may not have been referenced yet
      {
        m_StaleObjects.Enqueue( PNEW GCObject<T>( obj, "" ) );
        m_GCSyncPoint.Signal();
      }

    }

    void Main()
    {
      for(;;)
      {
        m_GCSyncPoint.Wait( m_Frequency );
        if( m_ExitFlag )
          return;

        GCList stale;
        stale.DisallowDeleteObjects();

        PWaitAndSignal lock( m_GCMutex );

        int staleCount = 0;
        for(;;)
        {
          GCObject<T> * gcObj = dynamic_cast<GCObject<T>*>(m_StaleObjects.Dequeue());
          if( gcObj == NULL )
            break;

          if( gcObj->m_RefCount <= 0 )
          {
            LOG( LogInsane(), "GC: DELETE (Stale) ---> " <<  gcObj->GetName() );
            delete gcObj;
          }else
          {
            stale.Append( gcObj );
            staleCount++;
          }
        }

        if( staleCount == 0 )
        {
          LOG(  LogInsane(), "GC: Garbage Collector empty - Object Count: " << m_CurrentObjects.GetSize() ); 
         
        }

        for( PINDEX i = 0; i < staleCount; i++ )
        {
          GCObject<T> * obj = dynamic_cast<GCObject<T>*>(stale.RemoveAt( 0 ));
          
          if( obj == NULL )
            PAssertAlways( PLogicError );

          m_StaleObjects.Enqueue( dynamic_cast<gcObject*>(obj) );
          LOG( LogInsane(), "GC:  Late Still Stale Object " << obj->GetName() );
        }
      }
    }

    typedef GC::GCOrdinalKey<T> gcOrdinalKey;
    typedef GC::GCObject<T> gcObject;
    PLIST( GCList, PObject );
    PDICTIONARY( GCCollection, gcOrdinalKey, gcObject );
    PQUEUE( StaleObjects, gcObject );

  private:
    PMutex m_GCMutex;
    GCCollection m_CurrentObjects;
    StaleObjects m_StaleObjects;
    PSyncPoint m_GCSyncPoint;
    PTimeInterval m_Frequency;
    BOOL m_ExitFlag;
  };

  #define GCREFINFO(T, objName) \
  public:\
  typedef GC::GCReference<T> GCRef; \
  typedef GC::Collector<T> GarbageCollector; \
  GCRef GCCreateRef( const PString & rfn = PString::Empty(), const PString & cln = objName ) \
  { \
    return GCGetIntance()->CreateReference(this, rfn, cln); \
  }; \
  template< class T1 > T1* GCCreateRef( GCRef & autoRef, const PString & rfn = PString::Empty(), const PString & cln = objName ) \
  { \
    autoRef = GCCreateRef( rfn, cln ); \
    return dynamic_cast< T1* >( autoRef.GetObject() ); \
  } \
  void GCCollect() \
  { \
    GCGetIntance()->DestroyObject(this); \
  };\
  static T::GarbageCollector * GCGetIntance();

  #define GCREF(name)GCRef autoRef = GCCreateRef( name )
  #define GCCREATECOLLECTOR(cls)\
  cls::GarbageCollector * cls::GCGetIntance()\
  {\
    static cls::GarbageCollector * instance = NULL;\
    if( instance == NULL )\
      instance=new cls::GarbageCollector();\
    return instance;\
  }


};

#if defined(_MSC_VER)
#pragma warning( pop ) // restore disabled warnings
#endif // ! _MSC_VER

#endif
