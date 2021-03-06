/*
 *
 * SBCSysLog.h
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
 * $Log: SBCSysLog.h,v $
 * Revision 1.2  2007/01/17 10:33:45  joegenbaclor
 * Redesigned syslog
 *
 * Revision 1.1  2007/01/17 00:09:14  joegenbaclor
 * Added SysLog
 *
 *
 */

#ifndef SBCSYSLOG_H
#define SBCSYSLOG_H

#include "Logger.h"
#include <ptlib/sockets.h>

using namespace Tools;

class SBCSysLog : public Logger, public PThread
{
  PCLASSINFO( SBCSysLog, PThread );
public:

  SBCSysLog(
    const PIPSocket::Address & bindAddress = PIPSocket::Address( "127.0.0.1" ),
    WORD port = 514,
    const PDirectory & directory = "logs",
    const PString & filePrefix = "syslog",
    const PString & fileSuffix = PString::Empty()
  );

  ~SBCSysLog();

protected:
  BOOL ReadLogs();

  PString GenerateCurrentFile(
    const PString & prefix,
    const PString & suffic
  );

  PTextFile & GetLogFile();

  void Main();

  PUDPSocket * m_Socket;
  PTextFile m_LogFile;
  PTime m_Today;


  PDirectory m_Directory;
  PString m_FilePrefix;
  PString m_FileSuffix;
};


#endif



