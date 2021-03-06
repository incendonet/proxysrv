/*
 *
 * RedirectSession.cxx
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
 * $Log: RedirectSession.cxx,v $
 * Revision 1.7  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.6  2006/08/26 14:31:44  joegenbaclor
 * Bulk Commit:  All SIP Sessions are now using new Garbage Collector and smart Pointers
 *
 * Revision 1.5  2006/08/14 01:29:08  joegenbaclor
 * Introduced separate provisions for UAS/UAC Current Request/Reponse.  Previousely the current request/response can be accessed via a single provision SIPSession::Get/Set/CurrentRequest()
 *
 * Revision 1.4  2006/03/30 05:34:16  joegenbaclor
 * 1.  Added configure support for windows build.  Note: Unix configure might be broken at this point.
 * 2.  Changed SIPSessionEvent to include the actual SIPMessage in the event.
 * 3.  Changed SDP to include a blank session name if it is not present.
 * 4.  Worked on OPAL classes in preparation for openphone use
 *
 * Revision 1.3  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "RedirectSession.h"
#include "RedirectSessionManager.h"

#define new PNEW

using namespace UACORE;


/// Server redirection, does NOT use profiles
RedirectSession::RedirectSession(
  RedirectSessionManager & sessionManager,
  const SIPMessage & request,
  const PString & sessionId
  ) : SIPSession( (SIPSessionManager &)sessionManager, request, sessionId )
{
}

RedirectSession::~RedirectSession()
{
}

void RedirectSession::OnIncomingSIPMessage(
  SIPMessageArrival & messageEvent
)
{
  GCREF( "Auto" );

  SIPSession::OnIncomingSIPMessage( messageEvent );

  m_Request = messageEvent.GetMessage();

  EnqueueSessionEvent( new SIPSessionEvent( *this, IncomingRequest, m_Request ) );
}

void RedirectSession::OnSessionEvent(
  int event,
  const SIPMessage & eventMsg
)
{
  GCREF( "Auto" );

  if( event == IncomingRequest )
  {
    if( ((RedirectSessionManager&)m_SessionManager).OnRequestRedirection(
        m_Request,
        m_ContactList ) )
    {
      SendAcceptByRedirection( m_ContactList );
    }else
    {
      SendAcceptByRejection( SIPMessage::Code404_NotFound );
    }

    /// commit suicide
    Destroy();
  }
}



