/*
 *
 * SIPSrvRecord.h
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
 * $Log: SIPSrvRecord.h,v $
 * Revision 1.4  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPSrvRecord_H
#define SIPSrvRecord_H


#include <ptlib.h>
#include <ptlib/sockets.h>
#include <ptclib/pdns.h>
#include "SIPMessage.h"


namespace SIPTransports
{
  using namespace SIPParser;

  class SIPSrvRecord : public PObject
  {
    PCLASSINFO( SIPSrvRecord, PObject );
  public:

    class HostRecord : public PObject
    {
      PCLASSINFO( HostRecord, PObject );
    public:

      enum Type
      {
        Host,
        Srv,
        Addr
      };

      HostRecord()
      {
        m_Port = 5060;
        m_Proto = "UDP";
      }

      HostRecord(
         const HostRecord & rec
      )
      {
        operator=(rec);
      }

      HostRecord & operator=(
        const HostRecord & rec 
      )
      {
        m_Address = rec.m_Address;
        m_Port = rec.m_Port;
        m_Proto = rec.m_Proto;
        return *this;
      }

      void PrintOn(
        ostream & strm
      )const
      {
        if( m_Type == Addr )
          strm << "IP$";
        else if( m_Type == Host )
          strm << "HOST$";
        else if( m_Type == Srv )
          strm << "SRV$";
        strm << m_Address << ":" << m_Port << ":" << m_Proto;
      }

      PIPSocket::Address m_Address;
      WORD m_Port;
      PString m_Proto;
      Type m_Type;

      PLIST( Collection, HostRecord );
    };

    SIPSrvRecord();

    SIPSrvRecord(
      const PString & domain
    );

    SIPSrvRecord(
      const SIPMessage & message
    );

    SIPSrvRecord(
      const SIPURI & uri
    );
    
    SIPSrvRecord & operator=( 
      const PString & domain 
    );

    SIPSrvRecord & operator=(
      const SIPMessage & message
    );

    SIPSrvRecord & operator=(
      const SIPURI & uri
    );

    PINDEX GetSize( 
      const PString & proto = "UDP" 
    )const;

    const HostRecord * GetRecordAt( 
      PINDEX i,
      const PString & proto = "UDP"
    )const;

    BOOL Resolve( 
      const PString & domain,
      const PString & proto = "*",
      WORD port = 5060
    );

    void PrintOn(
      ostream & strm
    )const;

    HostRecord::Collection m_UdpHostRecords;
    HostRecord::Collection m_TcpHostRecords;
  };

};

#endif


