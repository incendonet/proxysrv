/*
 *
 * Profile.h
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
 * $Log: Profile.h,v $
 * Revision 1.5  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef PROFILE_H
#define PROFILE_H

#include <ptlib.h>

namespace Profiles
{
  class Profile : public PObject
  {
    PCLASSINFO( Profile, PObject );
  public:

    Profile();

    Profile(
      const PString & profileId, /// a unique id that identifies this profile within a collection
      const PString & user,
      const PString & details = PString::Empty()
    );
    
    Profile( 
      const Profile & profile 
    );

    virtual ~Profile();

    Profile & operator=( 
      const Profile & profile 
    );

    virtual void PrintOn(
      ostream & strm
    )const;

    PINLINE const PString & GetUser()const{ return m_User; };
    PINLINE const PString & GetDetails()const{ return m_Details; };
    PINLINE const PString & GetProfileId()const{ return m_ProfileId; };
    PINLINE void SetUser( const PString & user ){ m_User = user; };
    PINLINE void SetDetails( const PString & details ){ m_Details = details; };
    PINLINE void SetProfileId( const PString & profileId ){ m_ProfileId = profileId; };
    PINLINE PString GetCustomInfo( PINDEX i )const{ return m_CustomInfo[i]; };
    PINLINE PINDEX GetCustomInfoSize()const{ return m_CustomInfo.GetSize(); }; 
    PINLINE void AppendCustomInfo( const PString & info ){ m_CustomInfo.AppendString( info ); };

    PLIST ( Collection, Profile );
    PDICTIONARY ( SortedCollection, PCaselessString, Profile );

  protected:
    PString m_ProfileId;
    PString m_User;
    PString m_Details;
    PStringArray m_CustomInfo;
  };
};

#endif

