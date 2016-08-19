/*
 *
 * ProcessMonitor.cxx
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
 * $Log: ProcessMonitor.cxx,v $
 * Revision 1.5  2007/01/17 10:33:45  joegenbaclor
 * Redesigned syslog
 *
 * Revision 1.4  2007/01/17 00:09:14  joegenbaclor
 * Added SysLog
 *
 * Revision 1.3  2007/01/15 06:32:23  joegenbaclor
 * no message
 *
 * Revision 1.2  2007/01/13 08:04:40  joegenbaclor
 * more launcher code
 *
 * Revision 1.1  2007/01/04 09:38:50  joegenbaclor
 * Initial upload of Launch Pad classes
 *
 *
 */

#include "ProcessMonitor.h"

#define new PNEW

ProcessMonitor::ProcessMonitor( 
  const PTimeInterval & pollInterval
) 
  : PThread( 1024 * 2, NoAutoDeleteThread, NormalPriority, "Process Monitor" )
{
  m_ProcessList.DisallowDeleteObjects();
  m_Limbo.DisallowDeleteObjects();
  m_PollInterval = pollInterval;
  Resume();
}

ProcessMonitor::~ProcessMonitor()
{
  m_ExitSync.Signal();
  WaitForTermination();
  m_ProcessList.AllowDeleteObjects();
  m_Limbo.AllowDeleteObjects();
}

void ProcessMonitor::Main()
{
  for(;;)
  {
    if( m_ExitSync.Wait( m_PollInterval ) )
      break;
    MonitorProcess();
  }
}

void ProcessMonitor::MonitorProcess()
{
  PWaitAndSignal lock( m_ProcessMutex );
  for( PINDEX i = 0; i < m_ProcessList.GetSize(); i++ )
  {
    ProcessObject & proc = m_ProcessList[i];
    if( FetchPID( proc ) )
    {
      if( !ProcessExist( proc.m_PID ) )
      {
        OnProcessDown( proc );
      }
    }
  }
}

static void SpliceMacro(PString & text, const PString & token, const PString & value)
{
  PRegularExpression RegEx("<?!--#status[ \t\r\n]+" + token + "[ \t\r\n]*-->?",
                           PRegularExpression::Extended|PRegularExpression::IgnoreCase);
  PINDEX pos, len;
  while (text.FindRegEx(RegEx, pos, len))
    text.Splice(value, pos, len);
}

PString ProcessMonitor::OnLoadApplicationStatus( 
  const PString & htmlBlock 
)
{
  PString substitution;
  PWaitAndSignal lock( m_ProcessMutex );
  PINDEX i = 0;
  for( i = 0; i < m_ProcessList.GetSize(); i++ )

  {
    PString insert = htmlBlock;
    ProcessObject & proc = m_ProcessList[i];
    
    SpliceMacro(insert, "ID", proc.GetProcessKey().ToUpper() );
    SpliceMacro(insert, "PATH", proc.GetApplicationPath() );
    SpliceMacro(insert, "PID", PString( proc.GetProcessId() ) );
    SpliceMacro(insert, "TIME STAMP", proc.GetTimeStamp().AsString() );
    

    if( FetchPID( proc ) )
    {
      if( !ProcessExist( proc.m_PID ) )
        SpliceMacro(insert, "Status", "Not running!" );
      else
        SpliceMacro(insert, "Status", "OK" );
    }
    substitution+=insert;
  }

  for( i = 0; i < m_Limbo.GetSize(); i++ )

  {
    PString insert = htmlBlock;
    ProcessObject & proc = m_Limbo[i];
    SpliceMacro(insert, "ID", proc.GetProcessKey().ToUpper() );
    SpliceMacro(insert, "PATH", proc.GetApplicationPath() );
    SpliceMacro(insert, "PID", PString( proc.GetProcessId() ) );
    SpliceMacro(insert, "TIME STAMP", proc.GetTimeStamp().AsString() );
    SpliceMacro(insert, "Status", "Stopped" );
    substitution+=insert;
  }


  return substitution;
}

BOOL ProcessMonitor::FetchPID( 
  ProcessObject & proc 
)
{
  PTextFile pidFile;
  if( !pidFile.Open( proc.GetPIDFilePath(), PFile::ReadOnly ) )
    return FALSE;

  PString line;
  if( !pidFile.ReadLine( line ) )
    return FALSE;

  proc.m_PID = line.AsInteger();
  
  return TRUE;
}

void ProcessMonitor::OnProcessDown(
  ProcessObject & proc
)
{
  PTRACE( 1, proc.GetProcessKey() << " Monitored Down! Bringing back up ..." );
  proc.Execute();
}


BOOL ProcessMonitor::AddProcess(
  const PString & processKey,
  const PFilePath & app,
  const PString & args,
  const PFilePath & pidFile,
  const PString & extraInfo,
  BOOL autoExec
)
{
  PWaitAndSignal lock( m_ProcessMutex );

  if( FindProcess( processKey ) != NULL)
    return FALSE;

  /// get rid of any previous instances
  if( PFile::Exists( pidFile ) )
  {
    PTextFile pid;
    if( !pid.Open( pidFile, PFile::ReadOnly ) )
      return FALSE;

    PString line;
    if( !pid.ReadLine( line ) )
      return FALSE;

    int procId = line.AsInteger();
    StopShellApplication( procId );
  }

  PFile::Remove( pidFile, TRUE );

  if( autoExec )
    ProcessMonitor::ExecShellCmd( app, args );

  ProcessObject * proc =  new ProcessObject( processKey, app, args, pidFile, extraInfo ) ;
  m_ProcessList.Append( proc );

  FetchPID( *proc );

  return TRUE;
}

BOOL ProcessMonitor::AddProcessFromLimbo(
  const PString & processKey
)
{
  PWaitAndSignal lock( m_ProcessMutex );
  for( PINDEX i = 0; i < m_Limbo.GetSize(); i++ )
  {
    if( m_Limbo[i].GetProcessKey() *= processKey )
    {
      ProcessObject * proc = dynamic_cast<ProcessObject*>(m_Limbo.RemoveAt( i ));
      m_ProcessList.Append( proc );
      proc->Execute();
      PThread::Sleep( 1000 );
      FetchPID( *proc );
    }
  }

  return FALSE;
}

ProcessMonitor::ProcessObject * ProcessMonitor::FindProcess(
  const PString & processKey
)
{
  PWaitAndSignal lock( m_ProcessMutex );
  for( PINDEX i = 0; i < m_ProcessList.GetSize(); i++ )
  {
    ProcessObject & proc = m_ProcessList[i];
    if( proc.GetProcessKey() *= processKey )
      return &m_ProcessList[i];
  }

  return NULL;
}


BOOL ProcessMonitor::RemoveProcess(
  const PString & processKey,
  BOOL autoShutdown,
  BOOL placeInLimbo
)
{
  PWaitAndSignal lock( m_ProcessMutex );
  for( PINDEX i = 0; i < m_ProcessList.GetSize(); i++ )
  {
    if( m_ProcessList[i].GetProcessKey() *= processKey )
    {
      ProcessObject * proc = dynamic_cast<ProcessObject*>(m_ProcessList.RemoveAt( i ));
      if( autoShutdown || placeInLimbo )
      {
        FetchPID( *proc );
        StopShellApplication( proc->GetProcessId() );
      }
      if( !placeInLimbo )
        delete proc;
      else
        m_Limbo.Append( proc );
    }
  }
  return TRUE;
}

#ifdef WIN32
int ProcessMonitor::ExecShellCmd(
  const PFilePath & app,
  const PString & args,
  BOOL hidden
)
#else
int ProcessMonitor::ExecShellCmd(
  const PFilePath & app,
  const PString & args
)
#endif
{
#ifdef WIN32
  int mode = SW_NORMAL;
  if( hidden )
    mode = SW_HIDE;
  return reinterpret_cast<int>(ShellExecute( 
    NULL, NULL, 
    static_cast<const char *>(app), 
    static_cast<const char *>(args), 
    static_cast<const char *>(app.GetDirectory()),
    mode ));
#else
  PStringStream cmd;
  cmd << app << " " << args;
  PTRACE( 1, app << " " << args );
  return system( static_cast<const char *>(cmd) );
#endif
}


BOOL ProcessMonitor::StopShellApplication(
  int pid
)
{
  if( !ProcessExist( pid ) )
    return TRUE;

  for (PINDEX retry = 1; retry <= 10; retry++) 
  {
    if (kill(pid, SIGTERM) == 0) 
    {
      return TRUE;
    }
    PThread::Sleep( 1000 );
  }
  return  FALSE;
}




