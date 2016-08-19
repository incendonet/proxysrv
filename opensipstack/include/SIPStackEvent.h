#ifndef JEB_SIPSTACKEVENT_H
#define JEB_SIPSTACKEVENT_H



namespace SIP
{
  using namespace SIPTimers;
  using namespace SIPTransactions;
  using namespace SIPParser;
  using namespace SIPTransports;
  using namespace SIPFSM;

  class SIPStackEvent : public PObject
  {
    PCLASSINFO( SIPStackEvent, PObject );
  public:
    enum Type
    {
      Message,
      Timer,
      TransportWrite,
      TransportRead,
      TransportError,
      Final
    };

    SIPStackEvent(
      Type type,
      const PString & transactionId
    ) : m_Type( type ), m_TransactionId( transactionId ){};

    virtual ~SIPStackEvent(){};

    PINLINE Type GetType(){ return m_Type; };

    PINLINE const PString & GetTransactionId()const{ return m_TransactionId; };

    PINLINE void PrintOn( ostream  & strm )const{ strm << "Event(" << m_Type << ") " << m_TransactionId; };

  protected:
    Type m_Type;
    const PString & m_TransactionId;
  };

  /////////////////////////////////////////////////////

  class SIPStackExit : public SIPStackEvent
  {
    PCLASSINFO( SIPStackExit, SIPStackEvent );
  public:
    SIPStackExit() : SIPStackEvent( SIPStackEvent::Final, "" ){};
    PINLINE void PrintOn( ostream  & strm )const{ strm << "Event(" << "SIPStackExit" << ")"; };
  };

  /////////////////////////////////////////////////////

  class SIPMessageArrival : public SIPStackEvent
  {
    PCLASSINFO( SIPMessageArrival, SIPStackEvent );
  public:
    SIPMessageArrival(
      SIPMessage & message,
      const PString & transactionId
      ) : SIPStackEvent( SIPStackEvent::Message, transactionId ){ m_SIPMessage = message; };

    PINLINE const SIPMessage & GetMessage(){ return m_SIPMessage; };

    PINLINE void PrintOn( ostream  & strm )const{ strm << "Event(" << "SIPMessageArrival" << ") " << m_TransactionId; };
  protected:
    SIPMessage m_SIPMessage;
  };

  ///////////////////////////////////////////////////////

  class SIPTimerExpire : public SIPStackEvent
  {
    PCLASSINFO( SIPTimerExpire, SIPStackEvent );
  public:

    SIPTimerExpire(
      SIPTimerEvent::Type timer,
      const PString & transactionId
    ) : SIPStackEvent( SIPStackEvent::Timer, transactionId ){ m_TimerType = timer; };

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
      SIPMessage & message,
      const PString & transactionId
      ) : SIPStackEvent( SIPStackEvent::TransportWrite, transactionId ){ m_SIPMessage = message; };

    PINLINE SIPMessage & GetMessage(){ return m_SIPMessage; };
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
      SIPMessage & message,
      const PString & transactionId,
      int socketError = 0
      ) : SIPStackEvent( SIPStackEvent::TransportError, transactionId ){ m_SIPMessage = message; m_SocketError = socketError; };

    PINLINE SIPMessage & GetMessage(){ return m_SIPMessage; };
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
      SIPMessage & message,
      const PString & transactionId
      ) : SIPStackEvent( SIPStackEvent::TransportRead, transactionId ){ m_SIPMessage = message; };

    PINLINE SIPMessage & GetMessage(){ return m_SIPMessage; };
    PINLINE void PrintOn( ostream  & strm )const{ strm << "Event(" << "SIPTransportRead" << ") " << m_TransactionId; };
  protected:
    SIPMessage m_SIPMessage;
  };

};

#endif


