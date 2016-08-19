/*
 *
 * SBCSysLog.cxx
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
 * $Log: SBCSysLog.cxx,v $
 * Revision 1.2  2007/01/17 10:33:45  joegenbaclor
 * Redesigned syslog
 *
 * Revision 1.1  2007/01/17 00:09:14  joegenbaclor
 * Added SysLog
 *
 *
 */

#include "SBCSysLog.h"
#include "ossbuildopts.h"
#define new PNEW



SBCSysLog::SBCSysLog(
    const PIPSocket::Address & bindAddress,
    WORD port,
    const PDirectory & directory,
    const PString & filePrefix,
    const PString & fileSuffix
) : PThread( 1024 * 2, NoAutoDeleteThread, NormalPriority, "SBCSysLog" )  
{
  m_Directory = directory;
  m_FilePrefix = filePrefix;
  m_FileSuffix = fileSuffix;

  m_Socket = new PUDPSocket( port );
  m_Socket->SetOption(SO_RCVBUF, SIP_UDP_DEFAULT_BUFFER_SIZE);
  m_Socket->Listen( bindAddress );

  PStringStream logFile;
  logFile << directory << GenerateCurrentFile( filePrefix, fileSuffix );

  if( !PDirectory::Exists( directory ) )
    PDirectory::Create( directory );

  m_LogFile.Open( logFile );
  m_LogFile.SetPosition( PFile::End );
  m_LogFile.WriteLine( "*** BEGIN ***" );
  Resume();

  PTRACE( 1, "SBCSysLog [" << logFile <<"] Listener: " << bindAddress << ":" << port );
}

PTextFile & SBCSysLog::GetLogFile()
{
  PTime now;
  if( m_Today.GetDay() != now.GetDay() )
  {
    m_Today = now;
    PStringStream logFile;
    logFile << m_Directory << GenerateCurrentFile( m_FilePrefix, m_FileSuffix );

    if( !PDirectory::Exists( m_Directory ) )
      PDirectory::Create( m_Directory );

    m_LogFile.Open( logFile );
    m_LogFile.SetPosition( PFile::End );
  }

  return m_LogFile;
}

PString SBCSysLog::GenerateCurrentFile(
  const PString & prefix,
  const PString &  suffix
)
{
  PStringStream traceFile;
 
  if( ! prefix.IsEmpty() )
	  traceFile << prefix << "-";
  else
    traceFile << "syslog-";

  traceFile << m_Today.GetYear()
            << "-"
            << m_Today.GetMonth()
            << "-"
            << m_Today.GetDay();

  if( !suffix.IsEmpty() )
    traceFile << "-" << suffix;
  else
    traceFile << "-" << PTimer::Tick().GetMilliSeconds();

  traceFile << ".log";

  return traceFile;
}

SBCSysLog::~SBCSysLog()
{
  m_Socket->Close();
  WaitForTermination();
  delete m_Socket;
}

BOOL SBCSysLog::ReadLogs()
{
  PString line;
  m_Socket->Read( line.GetPointer( 1024 ), 1024 );
  line.SetSize( m_Socket->GetLastReadCount() );
  line[m_Socket->GetLastReadCount()] = '\0';
  if( !line.IsEmpty() )
    GetLogFile().WriteLine( line );
  return m_Socket->IsOpen();
}

void SBCSysLog::Main()
{
  for(;;)
    if( !ReadLogs() )
      break;

  PTRACE( 1, "SBCSysLog TERMINATED" );
}






