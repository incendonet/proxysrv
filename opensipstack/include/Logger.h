/*
 *
 * Logger.h
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
 * $Log: Logger.h,v $
 * Revision 1.23  2007/01/17 10:33:59  joegenbaclor
 * Redesigned syslog
 *
 * Revision 1.22  2007/01/17 00:12:19  joegenbaclor
 * 1.  Fixed bug in call session where CANCEL may not be sent if a call ahs been authenticated due to a misplaced SetState( StateDisconnected ) for 4xx events
 * 2.  Fixed register session to ignore authorization headers if allow all reg is flagged
 * 3.  Temporary hacks for syslog
 * 4.  More launcher code -  almost there!
 *
 * Revision 1.21  2007/01/08 10:32:55  joegenbaclor
 * Removed BUILD_MINIMAL macro
 *
 * Revision 1.20  2006/12/21 08:59:43  joegenbaclor
 * Corrected typos in last commit
 *
 * Revision 1.19  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.18  2006/09/13 10:36:25  joegenbaclor
 * Added Release-Minimal build for smaller DLL footprint for ATLSIP
 *
 * Revision 1.17  2006/09/04 08:15:50  joegenbaclor
 * Reviced LOG_FATAL macro.  For some reason, PProcess::IsSeviceProcess()
 * won't resolve in linux
 *
 * Revision 1.16  2006/09/04 02:58:52  joegenbaclor
 * 1.  More bug fixes to garbage collector
 * 2.  Added new Logging macro so call System Log for fatal errors
 *
 * Revision 1.15  2006/08/28 07:18:34  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.14  2006/08/15 07:59:32  rcolobong
 * 1. LogLevel will effect during runtime
 * 2. Overload Log method that can specify contextid
 *
 * Revision 1.13  2006/07/31 07:43:13  joegenbaclor
 * Corrected bug in Logger::SetLogStream where contexts log stream are not updated
 *
 * Revision 1.12  2006/07/26 09:19:43  joegenbaclor
 * Added System Log support
 *
 * Revision 1.11  2006/07/07 07:53:22  joegenbaclor
 * 1.  Added setter for Logger stream.
 * 2.  Added static mutex for stream access
 *
 * Revision 1.10  2006/06/26 03:06:05  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to
 * avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL
 * componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 1.9  2006/03/21 03:29:56  joegenbaclor
 * Added Logger functionality to log to PTrace
 *
 * Revision 1.8  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <ptlib.h>
#include <ptlib/sockets.h>

namespace Tools
{
  #define LOG( context, log ) \
  { \
      PStringStream logData; \
      logData << log; \
      if( context != NULL ) \
        context->Log( __FILE__, __LINE__, logData ); \
    \
  }


  #if _DEBUG
  #define LOG_IF_DEBUG LOG
  #else
  #define LOG_IF_DEBUG( context, log ){ /*do nothing here*/ }
  #endif

  #define LOG_CONTEXT( context, id, log ) \
  { \
      PStringStream logData; \
      logData << log; \
      if( context != NULL ) \
        context->Log( __FILE__, __LINE__, id, logData ); \
    \
  } \

  #define TRACELOG( context, log ) LOG( context, log ); TRACE( log )

  class  LoggingDestination;

  enum LoggingLevel
  {
    LogLevelNone = 0,
    LogLevelInfo = 1,
    LogLevelDetail = 2,
    LogLevelDebugLow = 3,
    LogLevelDebugHigh = 4,
    LogLevelHigh = 5
  };
    
  class  LoggingContext : public PObject
  {
    PCLASSINFO( LoggingContext, PObject );
  public:
    LoggingContext( 
      LoggingDestination * dest, 
      const char * label, 
      unsigned logLevel = 1,
      const PString & contextId = PString::Empty()
    );

    virtual ~LoggingContext();
    
    void Log( const char * file, int line, PStringStream & log );
    void Log( const char * file, int line, const PString& contextID, PStringStream & log );
    
    void SetLogStream( 
      LoggingDestination * dest 
    );

    PINLINE void SetContextId( const PString & id ){ m_ContextId = id; };

  protected:
    LoggingDestination * m_Destination;
    LoggingLevel m_LogLevel;
    const char * m_Label;
    PString m_ContextId;
    PMutex m_LogStreamMutex;
  };

  /// Options for trace output.
  enum LoggingOptions {
    /**Include PTrace::Block constructs in output
      If this is bit is clear, all PTrace::Block output is inhibited
      regardless of the trace level. If set, the PTrace::Block may occur
      provided the trace level is greater than zero.
    */
    LogBlocks = 1,
    /// Include date and time in all output
    LogDateAndTime = 2,
    /// Include (millisecond) timestamp in all output
    LogTimestamp = 4,
    /// Include identifier for thread trace is made from in all output
    LogThread = 8,
    /// Include trace level in all output
    LogTraceLevel = 16,
    /// Include the file and line for the trace call in all output
    LogFileAndLine = 32,
    /// Include thread object pointer address in all trace output
    LogThreadAddress = 64,
    /// Append to log file rather than resetting every time
    LogAppendToFile = 128,
    /** Output timestamps in GMT time rather than local time
      */
    LogGMTTime = 256,
    /** SystemLog flag for tracing within a PServiceProcess application. Must
        be set in conjection with SetStream(new PSystemLog).
      */
    LogSystemLogStream = 32768
  };

  class LoggingDestination : public PObject
  {
    PCLASSINFO( LoggingDestination, PObject );

  public:
    LoggingDestination( 
      ostream * strm = &cerr, 
      unsigned int options = LogDateAndTime | LogThreadAddress | LogAppendToFile,
      BOOL useConsoleMutex = TRUE 
    );

    virtual ~LoggingDestination();
  
    void SetOptions( unsigned int options );
    PINLINE void ClearOptions(){ m_Options = 0; };
    void RemoveOption( unsigned int options );
    void SetStream( ostream * strm ){ m_Stream = strm; };
    PINLINE void SetLevel( unsigned level ){ m_Level = level; };
    PINLINE unsigned GetLevel()const{ return m_Level; };

    virtual void OnLog( 
      const PString & logLine,
      unsigned int level = 0 /// included for PTRACE support
    );

  protected:
    virtual void Log( const char * file, int line, unsigned level, const char * label, PStringStream & log );

  protected:
    unsigned m_Level;
    unsigned int m_Options;
    BOOL m_UseConsoleMutex;
    PMutex m_StreamMutex;
    static PMutex m_ConsoleMutex;
    ostream * m_Stream;

    friend class LoggingContext;
    friend class LoggingConsoleStream;
    friend class LoggingFileStream;
  };

  class  LoggingFileStream : public LoggingDestination
  {
    PCLASSINFO( LoggingFileStream, LoggingDestination );

  public:
    
    LoggingFileStream( 
      const char * path, 
      unsigned int options = LogDateAndTime | LogThreadAddress | LogAppendToFile 
    );

    virtual ~LoggingFileStream();

  private:
    
    PTextFile * m_FileStream;

  };


  class  LoggingIncrementingFileStream : public LoggingDestination
  {
    PCLASSINFO( LoggingIncrementingFileStream, LoggingDestination );

  public:
    
    LoggingIncrementingFileStream( 
      const char * prefix, 
      const char * suffix = NULL,
      unsigned int options = LogDateAndTime | LogThreadAddress | LogAppendToFile
      
    );

    virtual ~LoggingIncrementingFileStream();
    
    virtual void Log( 
      const char * file, 
      int line, 
      unsigned level, 
      const char * label,
      PStringStream & log 
    );

    virtual void OnLog( 
      const PString & logLine,
      unsigned int level = 0 /// included for PTRACE support
    );

   
  protected:
    void InternalOpen();

  private:
    PTextFile * m_FileStream;
    PTime m_Today;
    PString m_Prefix;
    PString m_Suffix;
  }; 
/////////////////////////////////////////////////////////////////////////

  class  LoggingSystemLogStream : public LoggingDestination
  {
    PCLASSINFO( LoggingSystemLogStream, LoggingDestination );

  public:
    
    LoggingSystemLogStream( 
      const PString & applicationId,
      const PIPSocket::Address & locaAddress,
      const PIPSocket::Address & serverAddress,
      WORD port,
      BOOL excludeSysLogHeaders = FALSE,
      unsigned int options = LogDateAndTime | LogThreadAddress | LogAppendToFile
      
    );

    virtual void OnLog( 
      const PString & logLine,
      unsigned int level = 0 /// included for PTRACE support
    );

  protected:
    PUDPSocket m_Socket;
    PString m_ApplicationId;
    BOOL m_ExcludeSysLogHeaders;
  }; 
  
  /////////////////////////////////////////////////////////////////////////
  class  LoggingConsoleStream : public LoggingDestination
  {
    PCLASSINFO( LoggingConsoleStream, LoggingDestination );
  public:
    LoggingConsoleStream( 
      unsigned int options = 
      LogDateAndTime | LogAppendToFile ) 
      : LoggingDestination( &cerr, options ){}
  };

  /////////////////////////////////////////////////////////////////////////

  class  LoggingPTRACEStream : public LoggingDestination
  {
    PCLASSINFO( LoggingPTRACEStream, LoggingDestination );
  public:
    LoggingPTRACEStream();

    virtual void OnLog( 
      const PString & logLine,
      unsigned int level = 0 /// included for PTRACE support
    );

  };

  /////////////////////////////////////////////////////////////////////////

  class  Logger
  {
  public:

    Logger();

    Logger( 
      LoggingDestination * stream 
    );

    /// copy constructor
    Logger(
      const Logger & logger 
    );

    Logger & operator=(
      const Logger & logger
    );
    

    virtual ~Logger();

    BOOL Open( 
      const PFilePath & logFile
    );

    BOOL Open( 
      LoggingDestination * logFile,
      BOOL autoDeleteFile = FALSE
    );


    Logger & operator=( const PFilePath & logFile );

    Logger & operator=( LoggingDestination * logFile );

    void SetLogStream( 
      LoggingDestination * fileStream,
      BOOL autoDeleteStream 
    );

    void SetLevel( int level );

  public:
    PINLINE LoggingDestination * GetLogStream()const{ return m_FileStream; };
    PINLINE const PString & GetLogContextId()const{ return m_ContextId; };
    PINLINE LoggingContext * LogInfo(){ return m_Info; };
    PINLINE LoggingContext * LogDetail(){ return m_Detail; };
    PINLINE LoggingContext * LogDebug(){ return m_Debug; };
    PINLINE LoggingContext * LogDebugHigh(){ return m_DebugHigh; };
    PINLINE LoggingContext * LogDebugVeryHigh(){ return m_DebugVeryHigh; };
    PINLINE LoggingContext * LogInsane(){ return m_Insane; };
    PINLINE LoggingContext * LogError(){ return m_Error; };
    PINLINE LoggingContext * LogWarning(){ return m_Warning; };
    
    void SetLogContextId( const PString & id );

  protected:
    BOOL CreateContexts();
    void DestroyContexts();

    PString m_ContextId;
    BOOL m_CanDeleteStream;
    BOOL m_LogToConsole;
    LoggingDestination * m_FileStream;
    LoggingContext * m_Info;
    LoggingContext * m_Detail;
    LoggingContext * m_Debug;
    LoggingContext * m_DebugHigh;
    LoggingContext * m_DebugVeryHigh;
    LoggingContext * m_Insane;
    LoggingContext * m_Error;
    LoggingContext * m_Warning;

  public:
    static int m_DefaultLevel;
    static void SetDefaultLevel( 
      int level 
    );
    static LoggingDestination * m_DefaultStream;
    static void SetDefaultLogFile( 
      const PFilePath & path 
    );
    static void SetDefaultLogStream( 
      LoggingDestination * strm 
    );
    static Logger m_DefaultConsoleStream;
    static PMutex m_StreamMutex;
  };
};



#endif //LOGGER_H
