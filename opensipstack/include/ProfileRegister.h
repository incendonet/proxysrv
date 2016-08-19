/*
 *
 * ProfileRegister.h
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
 * $Log: ProfileRegister.h,v $
 * Revision 1.4  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef PROFILEREGISTER_H
#define PROFILEREGISTER_H

#include "ProfileAuth.h"
#include "SIPTransport.h"

namespace Profiles
{
  using namespace SIPTransports;
  class ProfileRegister : public ProfileAuth
  {
    PCLASSINFO( ProfileRegister, ProfileAuth );
  public:

    ProfileRegister();

    ProfileRegister(
      const ProfileRegister & profile
    );

    ProfileRegister(
      const PString & profileId,
      const PString & user,
      const PString & account,
      const PString & password,
      const PString & domain,
      const PStringArray & contacts,
      const PString & serverAddress,
      const PString & realm = PString::Empty(),
      int expireTime = 3600000,
      AuthType authType = Digest,
      const PString & displayName = PString::Empty(),
      SIPTransport::Protocol protocol = SIPTransport::Unknown
    );

    virtual ~ProfileRegister();

    ProfileRegister & operator=(
      const ProfileRegister & profile
    );

    PINLINE const PString & GetDomain()const{ return m_Domain; };
    PINLINE PString GetContact( PINDEX i )const{ return m_Contacts[i]; };
    PINLINE PINDEX GetContactSize()const{ return m_Contacts.GetSize(); };
    PINLINE const PString & GetServerAddress()const{ return m_ServerAddress; };
    PINLINE int GetExpireInterval()const{ return m_ExpireInterval; };
    PINLINE const PString & GetRealm()const{ return m_Realm; };
    PINLINE const PString & GetDisplayName()const{ return m_DisplayName; };
    PINLINE SIPTransport::Protocol GetServerProtocol()const{ return m_Protocol; };
    
    PINLINE void SetDomain( const PString & domain ){ m_Domain = domain; };
    PINLINE void AppendContact( const PString & contact ){ m_Contacts.AppendString( contact ); };
    PINLINE void SetServerAddress( const PString & registrar ){ m_ServerAddress = registrar; };
    PINLINE void SetExpireInterval( int expire ){ m_ExpireInterval = expire; };
    PINLINE void SetRealm( const PString & realm ){ m_Realm = realm; };
    PINLINE void SetDisplayName( const PString & displayName ){ m_DisplayName = displayName; };
    PINLINE void SetServerProtocol( SIPTransport::Protocol protocol ){ m_Protocol = protocol; };
  protected:

    PString m_Domain;
    PString m_DisplayName;
    PStringArray m_Contacts;
    PString m_ServerAddress;
    PString m_Realm;
    int m_ExpireInterval;
    SIPTransport::Protocol m_Protocol;

    
  };
};

#endif




