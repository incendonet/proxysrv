/*
 *
 * Via.h
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
 * $Log: Via.h,v $
 * Revision 1.9  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.8  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.7  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.6  2006/03/22 08:45:52  joegenbaclor
 * More work on simple-proxy and affected classes
 *
 * Revision 1.5  2006/03/21 14:11:50  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.4  2006/03/14 03:13:22  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef VIA_H
#define VIA_H

#include <ptlib.h>
#include <ptlib/sockets.h>
#include "SIPHeaderB.h"

namespace SIPParser
{
  class Via : public SIPHeaderB
  {
    PCLASSINFO( Via, SIPHeaderB );
  public:

    Via();

    Via( 
      const Via & via 
    );

    Via(
      const PString & mime
    );

    Via(
      const MimeHeader & mime
    );

    Via(
      const PString & protocol,
      const PString & address,
      const MimeParam::SortedCollection & params
    );

    Via & operator=( 
      const Via & via
    );

    Via & operator=( 
      const PString & via
    );

    Via & operator=(
      const MimeHeader & mime
    );

    void SetProtocol(
      const PString & transport
    );

    void SetAddress(
      const PString & address
    );

    void SetPort(
      WORD port
    );
 
    const PString & GetProtocol()const;

    const PString & GetAddress()const;

    WORD GetPort()const;

    PString GetBranch()const;

    void SetBranch( 
      const PString & branch = PString::Empty() 
    );

    virtual void PrintOn(
      ostream & strm 
    ) const;

    virtual PObject * Clone()const;

    void PrepareBody();

    SIPURI GetURI()const;

    WORD GetRPort()const;

    void SetRPort(
      WORD rport
    );

    PIPSocket::Address GetReceiveAddress()const;

    void SetReceiveAddress(
      const PIPSocket::Address & raddr 
    );

    BOOL IsBehindNAT()const;

    BOOL IsLoopback()const;

    PLIST( Collection, Via );

    /*
    static void CreateCollection(
      const PStringArray & viaList,
      Collection & collection
    );
    */

    /*
    static Via & GetVia(
      PINDEX index,
      const Collection & collection
    );*/

    /*
    static Via PopTopVia(
      Collection & collection
    );
    */

  private:

    /// hide all these accessors as they are not used in this class
    PINLINE const PString & GetDisplayName(){ return m_DisplayName; };
    PINLINE void SetURI( const SIPURI & uri ){ m_URI = uri; };
    PINLINE void SetDisplayName( const PString & displayName ){ m_DisplayName = displayName; };
  protected:
    
    PString m_Protocol;
    PString m_Address;
    WORD m_Port;
  };

  typedef Via::Collection ViaList;
};

#endif

