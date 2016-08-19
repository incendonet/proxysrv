/*
 *
 * Logger.cxx
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
 * $Log: Logger.cxx,v $
 * Revision 1.21  2007/01/17 10:33:59  joegenbaclor
 * Redesigned syslog
 *
 * Revision 1.20  2007/01/17 00:12:19  joegenbaclor
 * 1.  Fixed bug in call session where CANCEL may not be sent if a call ahs been authenticated due to a misplaced SetState( StateDisconnected ) for 4xx events
 * 2.  Fixed register session to ignore authorization headers if allow all reg is flagged
 * 3.  Temporary hacks for syslog
 * 4.  More launcher code -  almost there!
 *
 * Revision 1.19  2006/12/21 08:59:43  joegenbaclor
 * Corrected typos in last commit
 *
 * Revision 1.18  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.17  2006/09/01 06:51:15  joegenbaclor
 * Bulk Commit:
 *
 * 1.  A bug is discovered in SIP Transaction Destructor where destruction of the SIPTimers may cause a deadlock on the timer manager effectivily freezing the entire message Queue System.  This was fixed by introducing a separate function to destroy the timers on receipt of the final event just before the transaction is queued for deletion.
 *
 * 2.  There is bug in Invite Server Transaction where ACK transaction is added to the Ack Transaction Pool after sending og none 2xx final response.  This new transaction is never removed from the pool after Timer_H has fired.  this may result to a bad pointer if ACK is received after the timer expired since the transaction may have been deleted by then.
 *
 * 3.  A few tweaks in GC GetName func to return the Class Name if GC NAme is empty.
 *
 * Revision 1.16  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.15  2006/08/15 07:58:38  rcolobong
 * 1. LogLevel will effect during runtime
 * 2. Overload Log method that can specify contextid
 *
 * Revision 1.14  2006/07/31 07:43:13  joegenbaclor
 * Corrected bug in Logger::SetLogStream where contexts log stream are not updated
 *
 * Revision 1.13  2006/07/26 09:19:43  joegenbaclor
 * Added System Log support
 *
 * Revision 1.12  2006/07/10 06:29:38  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.11  2006/07/07 07:53:26  joegenbaclor
 * 1.  Added setter for Logger stream.
 * 2.  Added static mutex for stream access
 *
 * Revision 1.10  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.9  2006/03/21 03:29:56  joegenbaclor
 * Added Logger functionality to log to PTrace
 *
 * Revision 1.8  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include <ptlib.h>
#include <ptlib/svcproc.h>
#include "Logger.h"
#include "ossbuildopts.h"

#define new PNEW

using namespace Tools;

LoggingContext::LoggingContext( 
  LoggingDestination * dest, 
  const char * label, 
  unsigned logLevel,
  const PString & contextId
)
{
  PAssertNULL( dest );
  m_Destination = dest;
  m_Label = label;
  m_LogLevel = (LoggingLevel)logLevel;
  m_ContextId = contextId;
}

LoggingContext::~LoggingContext()
{
}

void LoggingContext::SetLogStream( 
  LoggingDestination * dest 
)
{
  PWaitAndSignal lock( m_LogStreamMutex );
  m_Destination = dest;
}

void LoggingContext::Log( 
  const char * file, 
  int line, 
  PStringStream & log 
)
{
  Log( file, line, m_ContextId, log );
}

void LoggingContext::Log( 
  const char * file, 
  int line, 
  const PString& contextID, 
  PStringStream & log )
{
  PWaitAndSignal lock( m_LogStreamMutex );

  PString label;
  if( !contextID.IsEmpty() )
  {
    label = m_Label + PString(": [") + contextID + "]"; 
  }
  else
  {
    label = m_Label;
  }

  m_Destination->Log( file, line, (unsigned)m_LogLevel, label, log );
}


///////////////////////////////////////////////////////////////////////////////////

PTimeInterval ApplicationStartTime;

PMutex LoggingDestination::m_ConsoleMutex;

LoggingDestination::LoggingDestination( 
  ostream * strm, 
  unsigned int options, 
  BOOL useConsoleMutex )
{
  m_Level = 1;
  m_Options = options;
  m_Stream = strm;
  m_UseConsoleMutex = useConsoleMutex;
}

LoggingDestination::~LoggingDestination()
{
}

void LoggingDestination::SetOptions( unsigned int options )
{
  m_Options |= options;
}

void LoggingDestination::RemoveOption( unsigned int options )
{
  m_Options &= ~options;
}

void LoggingDestination::Log( 
  const char * fileName, 
  int lineNum, 
  unsigned level, 
  const char * label, 
  PStringStream & log )
{
  PStringStream header;
  if( level > m_Level )
    return;

  if( m_UseConsoleMutex )
    LoggingDestination::m_ConsoleMutex.Wait();
  else
    m_StreamMutex.Wait();

  if ((m_Options&LogSystemLogStream) != 0) {
    unsigned lvl = 4+PSystemLog::Warning;
    if (lvl >= PSystemLog::NumLogLevels)
      lvl = PSystemLog::NumLogLevels-1;
    ((PSystemLog*)m_Stream)->SetLevel((PSystemLog::Level)lvl);
  }
  else {
    if ((m_Options&LogDateAndTime) != 0) {
      PTime now;
      header << now.AsString("yyyy/MM/dd hh:mm:ss.uuu\t", (m_Options&LogGMTTime) ? PTime::GMT : PTime::Local);
    }

    if ((m_Options&LogTimestamp) != 0)
      header << setprecision(3) << setw(10) << (PTimer::Tick()-ApplicationStartTime) << '\t';

    if ((m_Options&LogThread) != 0) {
      PThread * thread = PThread::Current();
      if (thread == NULL)
        header << "ThreadID=0x"
                      << setfill('0') << hex << setw(8)
                      << PThread::GetCurrentThreadId()
                      << setfill(' ') << dec;
      else {
        PString name = thread->GetThreadName();
        if (name.GetLength() <= 23)
          header << setw(23) << name;
        else
          header << name.Left(10) << "..." << name.Right(10);
      }
      header << '\t';
    }

    if ((m_Options&LogThreadAddress) != 0)
      header << hex << setfill('0')
                    << setw(7) << (void *)PThread::Current()
                    << dec << setfill(' ') << '\t';
  }

  if ((m_Options&LogFileAndLine) != 0 && fileName != NULL) {
    const char * file = strrchr(fileName, '/');
    if (file != NULL)
      file++;
    else {
      file = strrchr(fileName, '\\');
      if (file != NULL)
        file++;
      else
        file = fileName;
    }

    header << setw(16) << file << '(' << lineNum << ")\t";
  }

  PStringArray logLines = log.Tokenise( "\n" );

  int label_len = (int)::strlen( label );
  static int longest_label_len = 0;
  if( label_len > longest_label_len )
    longest_label_len = label_len;
  header  << setiosflags( ios::left ) << setfill('.') << setw( longest_label_len > 27 ? longest_label_len + 3 : 30) << label << "| " ;

  PString finalLog;

  if( logLines.GetSize() == 1 )
  {
    finalLog = header + log;  
    OnLog( finalLog, level );
  }
  else
  {
    finalLog = /*PString("\n") +*/ header + " ";
    OnLog( finalLog, level );

    finalLog = /*PString("\n") +*/ header + /*PString("-> ")*/ "\t" + logLines[0].Trim();
    OnLog( finalLog, level );

    for( PINDEX i = 1; i < logLines.GetSize(); i++ )
    {
      if( !logLines[i].IsEmpty() )
      {
        ///*m_Stream << endl << header << "-> " << logLines[i];
        finalLog = /*PString("\n") +*/ header + /*PString("-> ")*/ "\t" + logLines[i].Trim();
        OnLog( finalLog, level );
      }
    }

    finalLog = /*PString("\n") +*/ header + " ";
    OnLog( finalLog, level );
  }

  



    if( m_UseConsoleMutex )
      LoggingDestination::m_ConsoleMutex.Signal();
    else
      m_StreamMutex.Signal();
}

void LoggingDestination::OnLog( 
  const PString & logLine,
  unsigned int 
)
{
  PAssertNULL( m_Stream );


  *m_Stream << logLine;

      /* Only output if there is something to output, this prevents some blank trace
     entries from appearing under some patholgical conditions. Unfortunately if
     stderr is used the unitbuf flag causes the out_waiting() not to work so we 
     must suffer with blank lines in that case.
   */
  *m_Stream << endl;
#if 0   /// This block doesnt work in VC7
    #if __GNUC__ >= 3
      *m_Stream << endl;
    #else
      ::streambuf * rb = m_Stream->rdbuf();
      if (((m_Stream->flags()&ios::unitbuf) != 0) || (rb->out_waiting() > 0)) {
        if ((m_Options&LogSystemLogStream) != 0)
          m_Stream->flush();
        else
          *m_Stream << endl;
      }
    #endif
#endif
}


/////////////////////////////////////////////////////////////////////////////////

LoggingFileStream::LoggingFileStream( 
  const char * path, 
  unsigned int options )
  : LoggingDestination( NULL, options, FALSE )
{
  if (options&LogAppendToFile) {
    m_FileStream = new PTextFile( path, PFile::ReadWrite );
    m_FileStream->SetPosition(0, PFile::End);
  } else 
    m_FileStream = new PTextFile(path, PFile::WriteOnly);

  if (m_FileStream->IsOpen())
    SetStream(m_FileStream);
  else {
    PError << "Could not open file " << path << "for logging" << endl; 
    delete m_FileStream;
    m_FileStream = NULL;
  }
};

LoggingFileStream::~LoggingFileStream()
{
  if( m_FileStream != NULL )
  {
    m_FileStream->Close();
    delete m_FileStream;
  }
}

LoggingIncrementingFileStream::LoggingIncrementingFileStream( 
  const char * prefix, 
  const char * suffix,
  unsigned int options
)
  : LoggingDestination( NULL, options, FALSE )
{
  m_FileStream = NULL;
  m_Prefix = prefix;

  if( suffix != NULL )
    m_Suffix = suffix;
  
  InternalOpen();
}

LoggingIncrementingFileStream::~LoggingIncrementingFileStream()
{
  if( m_FileStream != NULL )
  {
    m_FileStream->Close();
    delete m_FileStream;
  }

}

void LoggingIncrementingFileStream::InternalOpen()
{
#if 0
  if( m_FileStream != NULL )
  {
    m_FileStream->Close();
    PFilePath path = m_FileStream->GetFilePath();
    delete m_FileStream;

    /// compress the file;
    PFilePath dest(path + ".gz");
    ::api_compress_file( path, dest );
  }else /// check if the file the day prior exist and compress it
  {
    PStringStream yesterdaysFile;
 
	  yesterdaysFile << m_Prefix << "-"
              << m_Today.GetYear()
              << "-"
              << m_Today.GetMonth()
              << "-"
              << m_Today.GetDay() - 1
              << ".log";

    if( PFile::Exists( yesterdaysFile ) )
    {
      PFilePath path( yesterdaysFile );
      PFilePath dest(path + ".gz");
      ::api_compress_file( path, dest );
    }
  }
#endif

  PStringStream traceFile;
 
	traceFile << m_Prefix << "-"
            << m_Today.GetYear()
            << "-"
            << m_Today.GetMonth()
            << "-";
  traceFile << m_Today.GetDay();

  if( m_Suffix != NULL )
    traceFile << "-"
              << m_Suffix
              << ".log";

  if (m_Options&LogAppendToFile) 
  {
    m_FileStream = new PTextFile( traceFile, PFile::ReadWrite );
    m_FileStream->SetPosition(0, PFile::End);
  } else 
    m_FileStream = new PTextFile( traceFile, PFile::WriteOnly );

  if (m_FileStream->IsOpen())
    SetStream(m_FileStream);
  else 
  {
    PError << "Could not open file " << traceFile << "for logging" << endl; 
    delete m_FileStream;
    m_FileStream = NULL;
  }
}

void LoggingIncrementingFileStream::Log( 
  const char * file, 
  int line, 
  unsigned level,
  const char * label,
  PStringStream & log )
{
  PTime now;
  if( now.GetDay() != m_Today.GetDay() )
  {
    m_Today = now;
    InternalOpen(); /// open a new file
  }

  LoggingDestination::Log( file, line, level, label, log );
}

void LoggingIncrementingFileStream::OnLog( 
  const PString & _logLine,
  unsigned int
)
{
  PString logLine = _logLine;

  LoggingDestination::OnLog( logLine );
}

//////////////////////////////////////////////////////////////
Logger Logger::m_DefaultConsoleStream;
PMutex Logger::m_StreamMutex;
int Logger::m_DefaultLevel = 3;
LoggingDestination * Logger::m_DefaultStream = NULL;

void Logger::SetDefaultLevel( int level )
{ 
  PWaitAndSignal lock( Logger::m_StreamMutex );
  Logger::m_DefaultLevel = level;
  if( Logger::m_DefaultStream != NULL )
    Logger::m_DefaultStream->SetLevel( level );
}

void Logger::SetDefaultLogFile( const PFilePath & path )
{
  PWaitAndSignal lock( Logger::m_StreamMutex );

  static unsigned int options = 
      LogDateAndTime | 
      LogThread | 
      LogAppendToFile;

  if( Logger::m_DefaultStream != NULL )
    delete Logger::m_DefaultStream;
  
  Logger::m_DefaultStream = new LoggingFileStream( path, options );
  Logger::m_DefaultStream->SetLevel( Logger::m_DefaultLevel );

}

void Logger::SetDefaultLogStream( LoggingDestination * strm )
{
  PWaitAndSignal lock( Logger::m_StreamMutex );

  if( Logger::m_DefaultStream != NULL )
  {
    delete Logger::m_DefaultStream;
    Logger::m_DefaultStream = NULL;
  }

  if( strm != NULL )
  {
    Logger::m_DefaultStream = strm;
    Logger::m_DefaultStream->SetLevel( Logger::m_DefaultLevel );
  }
}

void Logger::SetLevel( int level )
{
  PWaitAndSignal lock( Logger::m_StreamMutex );
  if( m_FileStream != NULL )
  {
    m_FileStream->SetLevel( level );
  }
}

void Logger::SetLogStream( 
  LoggingDestination * strm,
  BOOL autoDeleteStream
)
{
  PWaitAndSignal lock( Logger::m_StreamMutex );
  

  if( m_FileStream != NULL && m_CanDeleteStream )
    delete m_FileStream;

  m_CanDeleteStream = autoDeleteStream;

  if( strm != NULL )
  {
    m_FileStream = strm;
    m_FileStream->SetLevel( Logger::m_DefaultLevel );
  }

  if( m_Info != NULL )
    m_Info->SetLogStream( m_FileStream );
  if( m_Detail != NULL )
    m_Detail->SetLogStream( m_FileStream );
  if( m_Debug != NULL )
    m_Debug->SetLogStream( m_FileStream );
  if( m_DebugHigh != NULL )
    m_DebugHigh->SetLogStream( m_FileStream );
  if( m_DebugVeryHigh != NULL )
    m_DebugVeryHigh->SetLogStream( m_FileStream );
  if( m_Insane != NULL )
    m_Insane->SetLogStream( m_FileStream );
  if( m_Error != NULL )
    m_Error->SetLogStream( m_FileStream );
  if( m_Warning != NULL )
    m_Warning->SetLogStream( m_FileStream );
}

Logger::Logger( LoggingDestination * stream )
{
  m_Info = NULL;
  m_Detail = NULL;
  m_Debug = NULL;
  m_DebugHigh = NULL;
  m_DebugVeryHigh = NULL;
  m_Insane = NULL;
  m_Error = NULL;
  m_Warning = NULL;

  Open( stream, FALSE );
}

//////////////////////////////////////////////////////////////////////
LoggingPTRACEStream::LoggingPTRACEStream() : LoggingDestination( NULL, 0, FALSE )
{
}

void LoggingPTRACEStream::OnLog( 
  const PString & logLine,
  unsigned int level
)
{
  PTRACE( level, logLine );
}

//////////////////////////////////////////////////////////////////////


Logger::Logger()
{
  m_Info = NULL;
  m_Detail = NULL;
  m_Debug = NULL;
  m_DebugHigh = NULL;
  m_DebugVeryHigh = NULL;
  m_Insane = NULL;
  m_Error = NULL;
  m_Warning = NULL;
  m_CanDeleteStream = FALSE;

  if( Logger::m_DefaultStream != NULL )
  {
    m_FileStream = Logger::m_DefaultStream;
  }
  else
  {
    m_CanDeleteStream = TRUE;
    m_FileStream = new LoggingConsoleStream();
    m_FileStream->SetLevel( Logger::m_DefaultLevel );
  }

  CreateContexts();
}

Logger::~Logger()
{
  if( m_CanDeleteStream && m_FileStream != NULL )
    delete m_FileStream;

  DestroyContexts();
}

/// copy constructor
Logger::Logger(
  const Logger & logger 
)
{
  operator=( logger );
}

Logger & Logger::operator=(
  const Logger & logger
)
{
  Open( logger.GetLogStream(), FALSE );
  SetLogContextId( logger.GetLogContextId() );
  return *this;
}

BOOL Logger::Open( 
  const PFilePath & logFile
)
{
  PWaitAndSignal lock( Logger::m_StreamMutex );

  if( m_CanDeleteStream && m_FileStream != NULL )
    delete m_FileStream;

  m_CanDeleteStream = TRUE;

  unsigned int options = 
      LogDateAndTime | 
      LogThread | 
      LogAppendToFile;
  
  LoggingFileStream * strm = new LoggingFileStream( logFile, options);
  strm->SetLevel( Logger::m_DefaultLevel );

  return Open( strm, TRUE );
}

BOOL Logger::Open( 
  LoggingDestination * logFile,
  BOOL autoDeleteFile
)
{
  PWaitAndSignal lock( Logger::m_StreamMutex );
  m_CanDeleteStream = autoDeleteFile;
  m_FileStream = logFile;
  return CreateContexts();
}

Logger & Logger::operator=( const PFilePath & logFile )
{
  Open( logFile );
  return *this;
}

Logger & Logger::operator=( LoggingDestination * logFile )
{
  Open( logFile );
  return *this;
}

void Logger::DestroyContexts()
{
  if( m_Info != NULL )
  {
    delete m_Info;
    m_Info = NULL;
  }
  
  if( m_Detail != NULL )
  {
    delete m_Detail;
    m_Detail = NULL;
  }

  if( m_Debug != NULL )
  {
    delete m_Debug;
    m_Debug = NULL;
  }
  
  if( m_DebugHigh != NULL )
  {
    delete m_DebugHigh;
    m_DebugHigh = NULL;
  }
  
  if( m_DebugVeryHigh != NULL )
  {
    delete m_DebugVeryHigh;
    m_DebugVeryHigh = NULL;
  }
  
  if( m_Insane != NULL )
  {
    delete m_Insane;
    m_Insane = NULL;
  }
  
  if( m_Error != NULL )
  {
    delete m_Error;
    m_Error = NULL;
  }
  
  if( m_Warning != NULL )
  {
    delete m_Warning;
    m_Warning = NULL;
  }
}

BOOL Logger::CreateContexts()
{
  if( m_FileStream == NULL )
    return FALSE;

  /// destroy the old context and create new ones
  DestroyContexts();
   /// logging contexts
  
  m_Info = new LoggingContext( 
    m_FileStream,
    "OSS INF",
    1,
    m_ContextId );

  m_Detail = new LoggingContext( 
    m_FileStream,
    "OSS DTL",
    2,
    m_ContextId );

  m_Debug = new LoggingContext( 
    m_FileStream,
    "OSS DBG",
    3,
    m_ContextId );

  m_DebugHigh = new LoggingContext( 
    m_FileStream,
    "OSS DBG",
    4,
    m_ContextId );

  m_DebugVeryHigh = new LoggingContext( 
    m_FileStream,
    "OSS DBG",
    5,
    m_ContextId );

  m_Insane = new LoggingContext( 
    m_FileStream,
    "OSS DBG",
    10,
    m_ContextId );

  m_Warning = new LoggingContext(
    m_FileStream,
    "OSS WRN",
    1,
    m_ContextId );

  m_Error = new LoggingContext(
    m_FileStream,
    "OSS ERR",
    1,
    m_ContextId );

  return TRUE;
}

void Logger::SetLogContextId( const PString & id )
{
  m_ContextId = id;

  if( m_Info != NULL )
    m_Info->SetContextId( id );

  if( m_Detail != NULL )
    m_Detail->SetContextId( id );

  if( m_Debug != NULL )
    m_Debug->SetContextId( id );
  
  if( m_DebugHigh != NULL )
    m_DebugHigh->SetContextId( id );
  
  if( m_DebugVeryHigh != NULL )
    m_DebugVeryHigh->SetContextId( id );

  if( m_Insane != NULL )
    m_Insane->SetContextId( id );

  if( m_Error != NULL )
    m_Error->SetContextId( id );

  if( m_Warning != NULL )
    m_Warning->SetContextId( id );
}

LoggingSystemLogStream::LoggingSystemLogStream( 
  const PString & applicationId,
  const PIPSocket::Address & locaAddress,
  const PIPSocket::Address & serverAddress,
  WORD port,
  BOOL excludeSysLogHeaders,
  unsigned int options
  
) : LoggingDestination( NULL, options, FALSE )
{
  m_Socket.SetOption(SO_SNDBUF, SIP_UDP_DEFAULT_BUFFER_SIZE);
  m_Socket.Listen( locaAddress );
  m_Socket.SetSendAddress( serverAddress, port );
  m_ApplicationId = applicationId;
  m_ExcludeSysLogHeaders = excludeSysLogHeaders;
}

 
void LoggingSystemLogStream::OnLog( 
  const PString & logLine,
  unsigned int level
)
{
  //// format the log
  /*
  0       Emergency: system is unusable
  1       Alert: action must be taken immediately
  2       Critical: critical conditions
  3       Error: error conditions
  4       Warning: warning conditions
  5       Notice: normal but significant condition
  6       Informational: informational messages
  7       Debug: debug-level messages
  */
  switch( level )
  {
    case 1:
      level = 5;
      break;
    case 2:
      level = 6;
      break;
   default:
     level = 7;
  }

  PStringStream packet;

  /// set the HEADEr
  if( !m_ExcludeSysLogHeaders )
  {
    PTime now;
    packet << "<1" << level << "> ";
    packet << now.AsString( "MMM dd hh:mm:ss", PTime::GMT ) << " " 
           << PIPSocket::GetHostName() << " ";
  }

  /// set the MESSAGE body
  PString id = "[" + m_ApplicationId.ToUpper() + "]";
  packet      << setiosflags( ios::left ) << setfill(' ') <<  setw(10) << id 
              << setfill(' ') << logLine;

  m_Socket.WriteString( packet );
}




