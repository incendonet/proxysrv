/*
 *
 * GCObject.cxx
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
 * $Log: GCObject.cxx,v $
 * Revision 1.4  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/09/06 07:36:38  joegenbaclor
 * Revised garbage collector so that each Garbage collectible objects will have its own
 *  instance of the GC.  This eliminates the need to cast to PObject
 *
 * Revision 1.2  2006/09/06 04:14:35  joegenbaclor
 * Added check on packet size for SIP UDP Transport Reads
 *
 * Revision 1.1  2006/08/25 18:17:59  joegenbaclor
 * Added GC singleton and macro definitions
 *
 *
 */

#include "GCObject.h"

using namespace GC;

#define new PNEW

class TestGC : public PObject
{
  public:
  typedef GC::GCReference<TestGC> GCRef; 
  typedef GC::Collector<TestGC> GarbageCollector; 
  GCRef GCCreateRef( const PString & rfn = PString::Empty(), const PString & cln = "TestGC" )
  {
    return GCGetIntance()->CreateReference(this, rfn, cln);
  };

  template< class T1 > T1* GCCreateRef( GCRef & autoRef, const PString & rfn = PString::Empty(), const PString & cln = "TestGC" )
  {
    autoRef = GCCreateRef( rfn, cln );
    return dynamic_cast< T1* >( autoRef.GetObject() );
  }
  void GCCollect()
  {
    GCGetIntance()->DestroyObject(this);
  };
  static TestGC::GarbageCollector * GCGetIntance();
public:
  TestGC(){};
  virtual ~TestGC(){}
};

GCCREATECOLLECTOR( TestGC );


