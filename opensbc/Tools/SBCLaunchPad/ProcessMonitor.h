/*
 *
 * ProcessMonitor.h
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
 * $Log: ProcessMonitor.h,v $
 * Revision 1.5  2007/01/17 10:33:45  joegenbaclor
 * Redesigned syslog
 *
 * Revision 1.4  2007/01/17 00:09:14  joegenbaclor
 * Added SysLog
 *
 * Revision 1.3  2007/01/15 06:32:23  joegenbaclor
 * no message
 *
 * Revision 1.2  2007/01/13 08:04:41  joegenbaclor
 * more launcher code
 *
 * Revision 1.1  2007/01/04 09:38:50  joegenbaclor
 * Initial upload of Launch Pad classes
 *
 *
 */

#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <ptlib.h>
#include "ShellUtils.h"


class ProcessMonitor : PThread
{
  PCLASSINFO( ProcessMonitor, PThread );
public:
  class ProcessObject : public PObject
  {
    PCLASSINFO( ProcessObject, PObject );
  public:
    ProcessObject(
      const PString & processKey,
      const PFilePath & app,
      const PString & args,
      const PFilePath & pidFile,
      const PString & extraInfo = PString::Empty()
    )
    {
      m_ProcessKey = processKey;
      m_ApplicationPath = app;
      m_Arguments = args;
      m_PIDFilePath = pidFile;
      m_PID = -1;
      m_ExtraInfo = extraInfo;
    }

#ifdef WIN32
    PINLINE int Execute( BOOL hidden = TRUE )
#else
    PINLINE int Execute()
#endif
    {
      m_TimeStamp = PTime();
#ifdef WIN32
    int mode = SW_NORMAL;
    if( hidden )
      mode = SW_HIDE;
    return reinterpret_cast<int>(ShellExecute( 
    NULL, NULL, 
    static_cast<const char *>(m_ApplicationPath), 
    static_cast<const char *>(m_Arguments), 
    static_cast<const char *>(m_ApplicationPath.GetDirectory()),
    mode ));
#else
      PStringStream cmd;
      cmd << m_ApplicationPath << " " << m_Arguments;
      return system( static_cast<const char *>(cmd) );
#endif
      
    }


    PINLINE const PString & GetProcessKey()const{ return m_ProcessKey; };
    PINLINE const PFilePath & GetApplicationPath()const{ return m_ApplicationPath; };
    PINLINE const PString & GetArguments()const{ return m_Arguments; };
    PINLINE const PFilePath & GetPIDFilePath()const{ return m_PIDFilePath; };
    PINLINE int GetProcessId()const{ return m_PID; };
    PINLINE const PTime & GetTimeStamp()const{ return m_TimeStamp; };
    PINLINE const PString & GetExtraInfo()const{ return m_ExtraInfo; };
    PINLINE void SetExtraInfo( const PString & info ){ m_ExtraInfo = info; };

  protected:
    PString m_ProcessKey;
    int m_PID;
    PFilePath m_ApplicationPath;
    PString m_Arguments;
    PFilePath m_PIDFilePath;
    PTime m_TimeStamp;
    PString m_ExtraInfo;

    friend class ProcessMonitor;

  };

  ProcessMonitor( 
    const PTimeInterval & pollInterval = 1000 /// 5 seconds
  );

  ~ProcessMonitor();


protected:
  virtual void Main();

  void MonitorProcess();

  BOOL FetchPID( 
    ProcessObject & proc 
  );

public:

  BOOL AddProcess(
    const PString & processKey,
    const PFilePath & app,
    const PString & args,
    const PFilePath & pidFile,
    const PString & extraInfo,
    BOOL autoExec = TRUE
  );

  BOOL AddProcessFromLimbo(
    const PString & processKey
  );

  BOOL RemoveProcess(
    const PString & processKey,
    BOOL autoShutdown = TRUE,
    BOOL placeInLimbo = FALSE
  );

  ProcessObject * FindProcess(
    const PString & processKey
  );

  /// Shell functions
  /// Executes a command line app
#ifdef WIN32
  int ExecShellCmd(
    const PFilePath & app,
    const PString & args,
    BOOL hidden = TRUE
  );
#else
  int ExecShellCmd(
    const PFilePath & app,
    const PString & args
  );
#endif
  BOOL StopShellApplication(
    int pid
  );

  virtual void OnProcessDown(
    ProcessObject & proc
  );

  PString OnLoadApplicationStatus( 
    const PString & htmlBlock 
  );

protected:
  PSyncPoint m_ExitSync;
  PTimeInterval m_PollInterval;
  PMutex m_ProcessMutex;
  PLIST( ProcessList, ProcessObject );
  ProcessList m_ProcessList;
  ProcessList m_Limbo;
  friend class ProcessStatusPage;
};





#endif




