/*
 *
 * ProfileUA.h
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
 * $Log: ProfileUA.h,v $
 * Revision 1.7  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/10/30 02:42:26  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.5  2006/08/11 08:38:42  joegenbaclor
 * Initial upload of Presence package
 *
 * Revision 1.4.4.2  2006/07/05 06:11:11  hartley2481
 * Minor changes
 *
 * Revision 1.4.4.1  2006/07/04 06:12:45  hartley2481
 * *** empty log message ***
 *
 * Revision 1.4  2006/03/15 00:40:33  joegenbaclor
 * 1.  More Dialog Support.
 * 2.  Improved SIPUserAgent::AppendCommonHeaders().
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef PROFILEUA_H
#define PROFILEUA_H

#include "ProfileTransport.h"
#include "ProfileRegister.h"
#include "ProfileMedia.h"


#include <ptlib/sockets.h>

namespace Profiles
{
  class ProfileUA : public Profile
  {
    PCLASSINFO( ProfileUA, Profile );
  public:

    ProfileUA();

    ProfileUA(
      const ProfileUA & profile
    );

    virtual ~ProfileUA();

    ProfileUA & operator=(
      const ProfileUA & profile
    );

    /// constant accessors
    PINLINE const ProfileRegister & RegistrationProfile()const{ return m_RegistrationProfile; };
    PINLINE const ProfileRegister & ProxyProfile()const{ return m_ProxyProfile; };
    PINLINE const ProfileMedia & MediaProfile()const{ return m_MediaProfile; };
    PINLINE const ProfileTransport & TransportProfile()const{ return m_TransportProfile; };
   
    /// none-constant accessors;
    PINLINE ProfileRegister & GetRegistrationProfile(){ return m_RegistrationProfile; };
    PINLINE ProfileRegister & GetProxyProfile(){ return m_ProxyProfile; };
    PINLINE ProfileMedia & GetMediaProfile(){ return m_MediaProfile; };
    PINLINE ProfileTransport & GetTransportProfile(){ return m_TransportProfile; };
  
    // User-Agent name to be sent for each request
    PINLINE const PString & GetUserAgentName()const{ return m_UserAgentName; };
    PINLINE void SetUserAgentName( const PString & uaName ){ m_UserAgentName = uaName; };

    // Display-Name to be used for from header for client calls
    PINLINE const PString & GetDisplayName()const{ return m_DisplayName; };
    PINLINE void SetDisplayName( const PString & displayName ){ m_DisplayName = displayName; };

    /// URI to be used for to header for client calls
    PINLINE const PString & GetRemoteURI()const{ return m_RemoteURI; };
    PINLINE void SetRemoteURI( const PString & uri ){ m_RemoteURI = uri; };

    /// Protocol where remote peer is listening.  Defaults to SIPTrnasport::Unknown
    PINLINE SIPTransport::Protocol GetRemoteProtocol()const{ return m_RemoteProtocol; };
    PINLINE void SetRemoteProtocol( SIPTransport::Protocol proto ){ m_RemoteProtocol = proto; };

    /// methods allowed and acceptable to this user agent
    PINLINE void AddAllowedMethod( const PString & method ){ m_AllowedMethods.AppendString( method ); };
    PINLINE PINDEX GetAllowedMethodSize()const{ return m_AllowedMethods.GetSize(); };
    PINLINE PString GetAllowedMethod( PINDEX i )const{ return m_AllowedMethods[i]; };
    PINLINE const PStringArray & GetAllowedMethods()const{ return m_AllowedMethods; };


    PINLINE WORD GetMaxForwards()const{ return m_MaxForwards; };
    PINLINE void SetMaxForwards( WORD max ){ m_MaxForwards = max; };
  protected:

    /// Profiles
    ProfileTransport m_TransportProfile;
    ProfileRegister m_RegistrationProfile;
    ProfileRegister m_ProxyProfile;
    ProfileMedia m_MediaProfile;

    PString m_UserAgentName;
    PString m_DisplayName;
    PString m_RemoteURI;
    SIPTransport::Protocol m_RemoteProtocol;
    WORD m_MaxForwards;

    PStringArray m_AllowedMethods;
  };
};

#endif

