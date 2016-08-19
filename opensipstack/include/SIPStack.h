/*
 *
 * SIPStack.h
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
 * $Log: SIPStack.h,v $
 * Revision 1.18  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.17  2006/11/23 09:30:47  joegenbaclor
 * Fixed bug in Route header parsing where the order is reversed by the SIPParser
 *
 * Revision 1.16  2006/11/21 06:55:17  joegenbaclor
 * Added ability to add/append additional listeners.
 *
 * Revision 1.15  2006/11/17 04:22:21  joegenbaclor
 * Added ability to remember core to use via m_UACoreName
 *
 * Revision 1.14  2006/10/30 02:42:26  joegenbaclor
 * 1.  Various changes to support RFC 3680.
 * 2.  Removed Presence files.  (To be promoted as a contrib library for ATLSIP)
 *
 * Revision 1.13  2006/09/02 05:47:22  joegenbaclor
 * Added GetMethod() method to SIPStackEvent
 *
 * Revision 1.12  2006/09/02 05:38:50  joegenbaclor
 * Added GetCallId() method to SIPStackEvent
 *
 * Revision 1.11  2006/08/06 02:32:07  joegenbaclor
 * Corrected bug regarding blank call-ids for UnknownTransaction events
 *
 * Revision 1.10  2006/04/18 04:31:52  hartley2481
 * *** empty log message ***
 *
 * Revision 1.9  2006/04/05 00:26:02  joegenbaclor
 * 1.  Completed support for STUN NAT traversal method and OPAL style address translation
 * 2.  More bug fixes to ProxySession
 * 3.  Fixed bug in messages where zero size list headers may be interpreted by the parser as a valid header
 * 4.  Started work on OPAL outbound
 *
 * Revision 1.8  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.7  2006/03/23 02:17:40  joegenbaclor
 * Corrected Linux compile errors
 *
 * Revision 1.6  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef JEB_SIPSTACK_H
#define JEB_SIPSTACK_H

#include "ptlib.h"
#include "SIPFiniteStateMachine.h"
#include "SIPTransportManager.h"
#include "SDPOffer.h"
#include "StackVersion.h"
#include "MD5Authorization.h"

#ifdef _WIN32
#pragma warning(disable:4312)
#endif

namespace SIP
{
  using namespace SIPTimers;
  using namespace SIPTransactions;
  using namespace SIPParser;
  using namespace SIPTransports;
  using namespace SIPFSM;
  using namespace SDP;

  ////////////////////////////////////////////

  class SIPStackEvent : public PObject
  {
    PCLASSINFO( SIPStackEvent, PObject );
  public:
    enum Type
    {
      Message,
      UnknownTransaction,
      Timer,
      TransportWrite,
      TransportRead,
      TransportError,
      Final,
      GarbageCollect
    };

    SIPStackEvent(
      Type type,
      const TransactionId & transactionId,
      const PString & coreName
    ) : m_Type( type ), m_TransactionId( transactionId ), m_UACoreName( coreName ){};

    virtual ~SIPStackEvent(){};

    PINLINE Type GetType(){ return m_Type; };
    PINLINE const TransactionId & GetTransactionId()const{ return m_TransactionId; };
    PINLINE const PString & GetCallId()const{ return m_TransactionId.GetCallId(); };
    PINLINE const PString & GetMethod()const{ return m_TransactionId.GetMethod(); };
    PINLINE void PrintOn( ostream  & strm )const{ strm << "Event(" << m_Type << ") " << m_TransactionId; };
    PINLINE void SetUACoreName( const PString & name ){ m_UACoreName = name; };
    PINLINE const PString & GetUACoreName()const{ return m_UACoreName; };

    PQUEUE( Queue, SIPStackEvent );
    PLIST( Collection, SIPStackEvent );
  protected:
    Type m_Type;
    TransactionId m_TransactionId;
    PString m_UACoreName;
  };

  /////////////////////////////////////////////////////

  class SIPStackExit : public SIPStackEvent
  {
    PCLASSINFO( SIPStackExit, SIPStackEvent );
  public:
    SIPStackExit() : SIPStackEvent( SIPStackEvent::Final, PString::Empty(), PString::Empty() ){};
    PINLINE void PrintOn( ostream  & strm )const{ strm << "Event(" << "SIPStackExit" << ")"; };
  };

  /////////////////////////////////////////////////////


  class SIPSmartObject : public PObject
  {
    PCLASSINFO( SIPSmartObject, PObject );
  public:

    class SIPSmartPtr : public PObject
    {
      PCLASSINFO( SIPSmartPtr, PObject );
    public:
      SIPSmartPtr() : m_Obj( NULL ){};

      SIPSmartPtr(
        SIPSmartObject * obj
      ) : m_Obj( obj )
      {
        if( m_Obj != NULL )
          m_Obj->AddRef();
      };

      SIPSmartPtr(
        const SIPSmartPtr & autoRef
      )
      {
        if( m_Obj != NULL )
        {
          m_Obj = autoRef.m_Obj;
          m_Obj->AddRef();
        }
      }

      ~SIPSmartPtr()
      {
        if( m_Obj != NULL )
          m_Obj->ReleaseRef();
      };

      SIPSmartPtr & operator=( SIPSmartObject * obj )
      {
        m_Obj = obj;
        if( m_Obj != NULL )
          m_Obj->AddRef();
        return *this;
      };

      SIPSmartPtr & operator=( const SIPSmartPtr & autoRef )
      {
        m_Obj = autoRef.m_Obj;
        if( m_Obj != NULL )
          m_Obj->AddRef();
        return *this;
      };

      PINLINE SIPSmartObject * operator->(){ return m_Obj; };
      PINLINE SIPSmartObject * GetPointer(){ return m_Obj; };
      PINLINE BOOL operator==( SIPSmartObject * obj )const{ return m_Obj == obj; };
      PINLINE BOOL operator!=( SIPSmartObject * obj )const{ return m_Obj != obj; };

    protected:
      SIPSmartObject * m_Obj;
    };

    SIPSmartObject(){};
    virtual ~SIPSmartObject(){}
    PINLINE int AddRef(){ return ++m_RefCount; };
    PINLINE int ReleaseRef(){ return --m_RefCount; };
    PINLINE int GetRefCount()const{ return m_RefCount; };
    PQUEUE( Queue, SIPSmartObject );
  protected:
    virtual void Cleanup()=0;
    friend class SIPGarbageCollect;
    PAtomicInteger m_RefCount;
  };

  //typedef SIPSmartObject::SIPSmartPtr SIPSmartPointer;
  /////////////////////////////////////////////////////

  //class SIPGarbageCollect : public SIPStackEvent
  //{
  //  PCLASSINFO( SIPGarbageCollect, SIPStackEvent );
  //public:
  //  SIPGarbageCollect( SIPSmartObject * staleObject )
  //    : SIPStackEvent( SIPStackEvent::GarbageCollect, PString::Empty() )
  //  {
  //    m_StaleObject = staleObject;
  //  }

  //  ~SIPGarbageCollect()
  //  {
  //    m_StaleObject->Cleanup();
  //    delete m_StaleObject;
  //  }

  //  SIPSmartObject * m_StaleObject;
  //public:
  //  PINLINE SIPSmartObject * GetStaleObject(){ return m_StaleObject; };

  //  friend class SIPStack;
  //};

  /////////////////////////////////////////////////////

  class SIPMessageArrival : public SIPStackEvent
  {
    PCLASSINFO( SIPMessageArrival, SIPStackEvent );
  public:
    SIPMessageArrival(
      const SIPMessage & message,
      const TransactionId & transactionId
    ) : SIPStackEvent( SIPStackEvent::Message, transactionId, message.GetUACoreName() ){ m_SIPMessage = message; };

    PINLINE const SIPMessage & GetMessage()const{ return m_SIPMessage; };
    PINLINE void PrintOn( ostream  & strm )const{ strm << "Event(" << "SIPMessageArrival" << ") " << m_TransactionId; };
  protected:
    SIPMessage m_SIPMessage;
  };

  /////////////////////////////////////////////////////

  class SIPUnknownTransaction : public SIPStackEvent
  {
    PCLASSINFO( SIPUnknownTransaction, SIPStackEvent );
  public:
    SIPUnknownTransaction(
      const SIPMessage & message,
      const TransactionId & transactionId,
      BOOL fromTransport
    ) : SIPStackEvent( SIPStackEvent::UnknownTransaction, transactionId, message.GetUACoreName() ){ m_SIPMessage = message; m_IsFromTransport = fromTransport; };

    PINLINE const SIPMessage & GetMessage()const{ return m_SIPMessage; };
    PINLINE void PrintOn( ostream  & strm )const{ strm << "Event(" << "SIPUnknownTransaction" << ") " ; };
    PINLINE BOOL IsFromTransport()const{ return m_IsFromTransport; };
  protected:
    SIPMessage m_SIPMessage;
    BOOL m_IsFromTransport;
  };

  ///////////////////////////////////////////////////////

  class SIPTimerExpire : public SIPStackEvent
  {
    PCLASSINFO( SIPTimerExpire, SIPStackEvent );
  public:

    SIPTimerExpire(
      SIPTimerEvent::Type timer,
      const TransactionId & transactionId,
      const PString & coreName
    ) : SIPStackEvent( SIPStackEvent::Timer, transactionId, coreName ){ m_TimerType = timer; };

    PINLINE SIPTimerEvent::Type GetTimer(){ return m_TimerType; };
    PINLINE void PrintOn( ostream  & strm )const{ strm << "Event(" << "SIPTimerExpire" << ") " << m_TransactionId; };
  protected:
    SIPTimerEvent::Type m_TimerType;

  };

  //////////////////////////////////////////////////////

  class SIPTransportWrite : public SIPStackEvent
  {
    PCLASSINFO( SIPTransportWrite, SIPStackEvent );
  public:
    SIPTransportWrite(
      const SIPMessage & message,
      const TransactionId & transactionId
      ) : SIPStackEvent( SIPStackEvent::TransportWrite, transactionId, message.GetUACoreName() ){ m_SIPMessage = message; };

    PINLINE const SIPMessage & GetMessage()const{ return m_SIPMessage; };
    PINLINE void PrintOn( ostream  & strm )const{ strm << "Event(" << "SIPTransportWrite" << ") " << m_TransactionId; };
  protected:
    SIPMessage m_SIPMessage;
  };

  ////////////////////////////////////////////////////////

  class SIPTransportWriteError : public SIPStackEvent
  {
    PCLASSINFO( SIPTransportWriteError, SIPStackEvent );
  public:
    SIPTransportWriteError(
      const SIPMessage & message,
      const TransactionId & transactionId,
      int socketError = 0
      ) : SIPStackEvent( SIPStackEvent::TransportError, transactionId, message.GetUACoreName() ){ m_SIPMessage = message; m_SocketError = socketError; };

    PINLINE const SIPMessage & GetMessage()const{ return m_SIPMessage; };
    PINLINE void PrintOn( ostream  & strm )const{ strm << "Event(" << "SIPTransportWriteError" << ") " << m_TransactionId; };
  protected:
    SIPMessage m_SIPMessage;
    int m_SocketError;
  };


  ///////////////////////////////////////////////////////

  class SIPTransportRead : public SIPStackEvent
  {
    PCLASSINFO( SIPTransportRead, SIPStackEvent );
  public:
    SIPTransportRead(
      const SIPMessage & message,
      const PString & transactionId
      ) : SIPStackEvent( SIPStackEvent::TransportRead, transactionId, message.GetUACoreName() ){ m_SIPMessage = message; };

    PINLINE const SIPMessage & GetMessage()const{ return m_SIPMessage; };
    PINLINE void PrintOn( ostream  & strm )const{ strm << "Event(" << "SIPTransportRead" << ") " << m_TransactionId; };
  protected:
    SIPMessage m_SIPMessage;
  };

  ///////////////////////////////////////////////


  class SIPStack : public SIPFiniteStateMachine
  {
    PCLASSINFO( SIPStack, SIPFiniteStateMachine );
  public:

    SIPStack();

    virtual ~SIPStack();

    /// pure virtuals from SIPTranstionManager

    /// This method is called when a new message arrives from the transport
    virtual void OnReceivedMessage(
      const SIPMessage & message,
      SIPTransaction & transaction
    );

    /// This message is called when the FSM asks the TU to send a message to the transport layer
    virtual void OnSendMessageToTransport(
      const SIPMessage & message,
      SIPTransaction & transaction
    );

    virtual void EnqueueTransportWriteEvent(
      const SIPMessage & message,
      SIPTransportEvent::Type eventType = SIPTransportEvent::PacketSend
    );

    /// This callback is called by the FSM when a certain timeout has occured and would
    /// cause the transaction to terminate
    virtual void OnTimerExpire(
      SIPTimerEvent & timer,
      SIPTransaction & transaction
    );

    void OnUnknownTransaction(
      const SIPMessage & event,
      BOOL fromTransport
    );

    /// pure virtual from SIPTransportManager

    /// This callback is called by the transport layer for every event that occurs in the transport
    /// events include read, write and error events
    virtual void OnTransportEvent(
      SIPTransportEvent * event
    );



    SIPStackEvent * ReadEvent();

    void Terminate(
      PINDEX signalCount = 1
    );

    //void DestroyObject(
    //  SIPSmartObject * staleObject
    //);

    /*
    BOOL ConstructRequest(
      SIPMessage & request,
      const PString & method,
      const PString & startLineURI,
      const PString & toURI,
      const PString & fromURI,
      DWORD sequence
    );
    */
    /*
    BOOL ConstructRegister(
      SIPMessage & request,
      const PString & registrar,
      const PString & domain,
      const PString & userName,
      DWORD sequence,
      DWORD expire = 3600,
      const PString & proto = "UDP",
      const PString & displayName = PString::Empty()
    );
    */

    BOOL ConstructInvite(
      SIPMessage & invite,
      const PString & uri,
      const PString & toURI,
      const PString & fromURI,
      const SDPPacket & sdp = SDPPacket()
      );

    /*
    BOOL ConstructAuthorization(
      const SIPMessage & regMsg,
      const SIPMessage & challenge,
      SIPMessage & auth,
      const PString & userName,
      const PString & password
    );
    */

    PINLINE BOOL AppendTransport(
      SIPTransport::Protocol protocol,
      const PString & iface = PString::Empty(),
      WORD port = 5060
    ){ return m_TransportManager->AddTransport( protocol, iface, port, TRUE ); };

    PINLINE BOOL AppendTransport(
      SIPTransport::Protocol protocol,
      const PIPSocket::Address & iface,
      WORD port = 5060
    ){ return m_TransportManager->AddTransport( protocol, iface, port, TRUE ); };

    PINLINE BOOL AddTransport(
      SIPTransport::Protocol protocol,
      const PString & iface = PString::Empty(),
      WORD port = 5060
    ){ return m_TransportManager->AddTransport( protocol, iface, port ); };

    PINLINE BOOL AddTransport(
      SIPTransport::Protocol protocol,
      const PIPSocket::Address & iface,
      WORD port = 5060
      ){ return m_TransportManager->AddTransport( protocol, iface, port ); };


    PINLINE SIPTransportManager * GetTransportManager(){ return m_TransportManager; };

    PINLINE void SetPrimaryDomain( const PString & domain ){ m_PrimaryDomain = domain; };

    PINLINE const PString & GetPrimaryDomain()const{ return m_PrimaryDomain; };

  protected:
    PDECLARE_NOTIFIER( SIPTransportManager, SIPStack, _OnTransportEvent );
    SIPTransportManager * m_TransportManager;

    /// Read Queue
    PQUEUE( ReadQueue, SIPStackEvent );
    ReadQueue m_ReadQueue;
    PMutex m_ReadQueueMutex;
    PSemaphore m_ReadQueueSync;

    PString m_PrimaryDomain;
    PStringArray m_SecondaryDomains;
  };
};


#endif

