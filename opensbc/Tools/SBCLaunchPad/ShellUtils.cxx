/*
 *
 * ShellUtils.cxx
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
 * $Log: ShellUtils.cxx,v $
 * Revision 1.2  2007/01/15 06:32:23  joegenbaclor
 * no message
 *
 * Revision 1.1  2007/01/04 09:38:50  joegenbaclor
 * Initial upload of Launch Pad classes
 *
 *
 */

#include "ShellUtils.h"

#ifdef WIN32
#include <shellapi.h>
int kill(pid_t pid, int sig)
{
  HANDLE hProc;
  hProc=OpenProcess( PROCESS_TERMINATE|PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE,pid );
  if(hProc)
	{
		if(TerminateProcess(hProc,0))
		{
			// process terminated
			CloseHandle(hProc);
			return 0;
		}
    CloseHandle(hProc);
  }
  return -1;
}
#endif

BOOL ProcessExist( int pid )
{
#ifdef WIN32
  HANDLE hProc;
  hProc=OpenProcess( PROCESS_QUERY_INFORMATION|PROCESS_VM_READ, FALSE,pid );
  if(hProc)
  {
    CloseHandle(hProc);
	  return TRUE;
  }

  return FALSE;
#else
  return kill( pid, 0 ) == 0; 
#endif
}




