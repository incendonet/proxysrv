/*
 *
 * SIPTransport.h
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
 * $Log: SIPTransport.h,v $
 * Revision 1.17  2007/01/06 07:30:29  joegenbaclor
 * Added mask to filter GetDefaultInterfaceAddress
 *
 * Revision 1.16  2006/12/22 07:37:19  joegenbaclor
 * Introduced separate funtion call  for starting of transport thread.  This is necessary
 *  to make sure that the backdoor listener is already initialized prior to calling transport
 *  reads
 *
 * Revision 1.15  2006/12/04 08:55:32  joegenbaclor
 * Added log hook to transport
 *
 * Revision 1.14  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.13  2006/11/21 06:55:17  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.12  2006/11/07 09:30:28  joegenbaclor
 * Added new internal header "IFACE-ADDR" to allow transctions to remember the interface address for sending future responses within the transaction lifespan
 *
 * Revision 1.11  2006/09/01 13:13:19  rcolobong
 * Create static method SetMinBufferSize to specify Maximum Buffer Size for PSocket
 *
 * Revision 1.10  2006/07/17 11:36:45  joegenbaclor
 * 1.  More routing enhancements to B2BUA
 *
 * Revision 1.9  2006/06/28 14:14:57  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.8  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.7  2006/03/24 15:42:13  joegenbaclor
 * Some bug fixes including possible deadlocks between Session Destruction and ProcessStackEvent
 *
 * Revision 1.6  2006/03/21 14:11:50  joegenbaclor
 * 1.  Added rport support for NAT Traversal
 * 2.  Added NAT Keep alive mechanism in SIPSession
 * 3.  Some Improvements in various classes affecting NAT like Via, SIPTransport etc.
 *
 * Revision 1.5  2006/03/14 03:13:22  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SIPTRANSPORT_H
#define SIPTRANSPORT_H

#include <ptlib.h>
#include <ptlib/sockets.h>
#include <ptclib/enum.h>
#include "SIPMessage.h"
#include "SIPSrvRecord.h"
#include "Logger.h"

using namespace Tools;

namespace SIPTransports
{
  class SIPTransportManager;

  using namespace SIPParser;

  #ifdef MAX_SIP_MESSAGE_LENGTH
  #undef MAX_SIP_MESSAGE_LENGTH
  #endif

  #ifdef MIN_SIP_MESSAGE_LENGTH
  #undef MIN_SIP_MESSAGE_LENGTH
  #endif

  #define MAX_SIP_MESSAGE_LENGTH 2048
  #define MIN_SIP_MESSAGE_LENGTH 20

  class SIPTransportEvent : public PObject
  {
    PCLASSINFO( SIPTransportEvent, PObject );
  public:

    enum Type
    {
      PacketSend,
      UDPReadError,
      UDPWriteError,
      UDPPacketSend,
      UDPPacketSent,
      UDPPacketArrival,
      TCPReadError,
      TCPWriteError,
      TCPPacketSent,
      TCPPacketSend,
      TCPPacketArrival,
      TLSReadError,
      TLSWriteError,
      TLSPacketSent,
      TLSPacketSend,
      TLSPacketArrival,
      ErrorUnknown,
      FinalEvent
    };

    SIPTransportEvent( 
      PObject * packet,
      Type type,
      int socketError = 0
    ) : m_Type( type ),
        m_SocketError( socketError ),
        m_Event( packet )
    {
    }

    virtual ~SIPTransportEvent(){ delete m_Event; };

    PINLINE PObject * GetEvent(){ return m_Event; };

    Type m_Type;
    int m_SocketError;
    PObject * m_Event;

  };

  class SIPTransport : public PObject, public Logger
  {
    PCLASSINFO( SIPTransport, PObject );
  public:

    enum Protocol
    {
      UDP,
      TCP,
      TLS,
      Unknown
    };

    enum Version
    {
      IPV4,
      IPV6
    };

    SIPTransport( 
      SIPTransportManager * manager,
      Protocol proto
    );

    ~SIPTransport();

    /// enqueues a new packet for sending
    /// none-blocking
    void Enqueue( 
      const SIPMessage & packet 
    );

    /// Dequeues the new received packet
    /// Will block until a packet is received
    //BOOL Dequeue( SIPMessage & packet );

    virtual BOOL Listen(
      const PString & iface = PString::Empty(),
      WORD port = 5060,
      BOOL append = FALSE
    ) = 0;


    virtual BOOL GetListenerAddress(
      const PIPSocket::Address & target,
      PIPSocket::Address & listenerAddress,
      WORD & listenerPort
    ) = 0;

    virtual void CleanUp();

    BOOL StartThreads();

    static void SetMinBufferSize(PUDPSocket & sock, 
      int buftype, 
      int bufferSize = SIP_UDP_DEFAULT_BUFFER_SIZE
    );

  protected:

    /// Enqueue read packets.  Should never be used outside of the transport
    /// read thread
    //void InternalEnqueueRead(
    //  SIPMessage * packet
    //);

    /// Dequeue packets to be written.  Should never be used outside of the transport
    /// write thread
    SIPMessage * InternalDequeueWrite();

    BOOL ProcessRead();

    BOOL ProcessWrite();
      
    virtual BOOL ReadPacket( 
      SIPMessage *& packet,
      PIPSocket::Address & addr,
      WORD & port,
      PIPSocket::Address & ifaceAddress,
      WORD & ifacePort,
      int & length
    ) = 0;

    virtual BOOL WritePacket( 
      SIPMessage & packet,
      const PIPSocket::Address & address = 0,
      WORD port = 0,
      const PString & iface = PString::Empty()
    ) = 0;

    
    
  public:
    /// INLINES
    PINLINE WORD GetListenerPort(){ return m_ListenerPort; };

    PINLINE Protocol GetProtocol()const{ return m_Protocol; };

  protected:

    class ReadThread : public PThread
    {
      PCLASSINFO( ReadThread, PThread );
    public:

      ReadThread(
        SIPTransport * transport
      );

      ~ReadThread();
      
      void Main();
    protected:
      SIPTransport * m_Transport;
      BOOL m_ExitThread;
    };

    class WriteThread : public PThread
    {
      PCLASSINFO( WriteThread, PThread );
    public:

      WriteThread(
        SIPTransport * transport
      );

      ~WriteThread();
      
      void Main();
    protected:
      SIPTransport * m_Transport;
      BOOL m_ExitThread;
    };

  public:

    static BOOL Initialize();
    
    static void Deinitialize();

    static BOOL IsLocalAddress( 
      const PIPSocket::Address & 
    );

    static BOOL IsLocalAddress(
      const SIPURI & uri
    );

    static BOOL IsPrivateNetwork(
      const PIPSocket::Address & address
    );

    static BOOL IsPrivateNetwork(
      const SIPURI & address
    );

    /// only matches IP address and port
    static BOOL IsTheSameAddress(
      const SIPURI & uri_a,
      const SIPURI & uri_b,
      BOOL treatNoPortAs5060
    );

    static BOOL Resolve(
      const SIPURI & uri,
      PIPSocket::Address & address,
      WORD & port
    );

    static BOOL Resolve(
      const SIPMessage & msg,
      PIPSocket::Address & address,
      WORD & port
    );

    static BOOL ENUMLookup(
      const PStringArray & enumServer,
      const PString & e164,
      SIPURI & result
    );

    PDICTIONARY( Collection, PCaselessString, SIPTransport );

    PINLINE BOOL IsListening()const{ return m_IsListening; };

  public:
    static void AttachReadNotifier( const PNotifier & notifier );
    static void AttachWriteNotifier( const PNotifier & notifier );
    static BOOL m_HasReadNotifier;
    static BOOL m_HasWriteNotifier;
    static PNotifier m_ReadNotifier;
    static PNotifier m_WriteNotifier;

  protected:
    static BOOL m_IsInitialized;

    BOOL m_IsListening;

    SIPTransportManager * m_Manager;
    Protocol m_Protocol;
    PString m_InterfaceAddress;
    WORD m_ListenerPort;
    
    /// Read Queue
    //PQUEUE( ReadQueue, SIPMessage );
    //ReadQueue m_ReadQueue;
    //PMutex m_ReadQueueMutex;
    //PSemaphore m_ReadQueueSync;

    /// Write Queue
    PQUEUE( WriteQueue, SIPMessage );
    WriteQueue m_WriteQueue;
    PMutex m_WriteQueueMutex;
    PSemaphore m_WriteQueueSync;

    ReadThread * m_ReadThread;
    WriteThread * m_WriteThread;



    friend class ReadThread;
    friend class WriteThread;
  };

  
}
#endif
