/*
 *
 * Contact.h
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
 * $Log: Contact.h,v $
 * Revision 1.4  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef CONTACT_H
#define CONTACT_H

#include "SIPHeaderC.h"
#include "MimeParam.h"
#include "ContactURI.h"

//Contact: "Mr. Watson" <sip:watson@worcester.bell-telephone.com>
//         ;q=0.7; expires=3600,
//         "Mr. Watson" <mailto:watson@bell-telephone.com> ;q=0.1

namespace SIPParser
{
  class Contact : public SIPHeaderC
  {
    PCLASSINFO( Contact, SIPHeaderC );
  public:
      
    Contact();

    Contact( 
      const Contact & header 
    );
      
    Contact( 
      const PString & header 
    );
      
    Contact( 
      const MimeHeader & header 
    );
      
    Contact( 
      const PStringArray & header 
    );

    Contact(
      const ContactURI::Collection & uri
    );

    Contact & operator=(
      const ContactURI::Collection & uri
    );

    virtual PObject * Clone()const;

    BOOL GetURI(
      ContactURI & uri,
      PINDEX uriIndex = 0
    );

    BOOL AddURI(
      const ContactURI & uri
    );
   
  };

  class ContactList : public SIPHeaderCList
  {
    PCLASSINFO( ContactList, SIPHeaderCList );
  public:

    ContactList(){};

    ContactList(
      const SIPHeaderCList & list
    ) : SIPHeaderCList( list ){};

    ContactList(
      const SIPHeaderC::Collection & list
    ) : SIPHeaderCList( list ){};
  };
};

#endif


