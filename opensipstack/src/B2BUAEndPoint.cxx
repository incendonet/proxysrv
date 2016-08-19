/*
 *
 * B2BUAEndPoint.cxx
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
 * $Log: B2BUAEndPoint.cxx,v $
 * Revision 1.12  2007/01/14 13:12:54  joegenbaclor
 * Fixed compile warnings
 *
 * Revision 1.11  2007/01/11 02:11:41  rcolobong
 * Add setter for enable/disable proxying media
 *
 * Revision 1.10  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.9  2006/11/23 11:00:12  joegenbaclor
 * Added Instant Message B2B support
 *
 * Revision 1.8  2006/11/10 07:58:05  rcolobong
 * Add Method to disable/enable Session Timers in B2BUACall
 *
 * Revision 1.7  2006/08/30 03:42:05  joegenbaclor
 * Corrected bug in garbage collector loop.
 *
 * Revision 1.6  2006/07/19 02:19:21  joegenbaclor
 * 1.  Bug fixes concerning B2BUA destruction during during 3xx, 4xx, 5xx, 6xx local response
 * 2.  Fixed bug where Route header is never removed from outbound Invite resulting that may result to a loop when
 *      Inbound Invite has a Route header present
 * 3.  Added RemoveAllRoutes() in SIPMessage
 *
 * Revision 1.5  2006/07/10 06:29:38  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.4  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
 *
 * Revision 1.3  2006/06/14 08:43:38  joegenbaclor
 * Initial upload of OpenB2BUA applcation and related classes
 *
 * Revision 1.2  2006/04/10 01:09:03  joegenbaclor
 * Implemented Create methods fo B2BUA sessions
 *
 * Revision 1.1  2006/04/08 06:27:13  joegenbaclor
 * Initial upload of B2BUA classes
 *
 *
 */

#include "B2BUAEndPoint.h"
#include "B2BUAConnection.h"

#define new PNEW

using namespace B2BUA;

B2BUAEndPoint::B2BUAEndPoint(
  SIPUserAgent & ua,
  PINDEX sessionThreadCount,
  PINDEX stackSize
) : CallSessionManager( ua, sessionThreadCount, stackSize )
{
  m_EnableSessionTimer = FALSE;
  m_MediaProxyIfPrivate = FALSE;
}

B2BUAEndPoint::~B2BUAEndPoint()
{
}

B2BUAConnection * B2BUAEndPoint::CreateB2BUA(
  const SIPMessage & request,
  B2BUACall * call
)
{
  PWaitAndSignal lock( m_SessionsMutex );

  PString sessionId = call->GetSessionId();
  PString b2buaId = sessionId + PString( "-connection" );

  B2BUAConnection * b2bua = OnCreateB2BUA( 
    request, 
    b2buaId, 
    call 
  );

  if( b2bua == NULL )
    return NULL;

  b2bua->AttachCall( sessionId, call );
   
  m_SessionsBySessionId.SetAt( b2buaId, b2bua );
  m_SessionsByCallId.SetAt( b2buaId, b2bua );

  return b2bua;
}

B2BUAConnection * B2BUAEndPoint::OnCreateB2BUA(
  const SIPMessage & /*request*/,
  const PString & sessionId,
  B2BUACall * /*call*/
)
{
  B2BUAConnection * connection = new B2BUAConnection( *this, sessionId );
  connection->SetMediaProxyIfPrivate( m_MediaProxyIfPrivate );
  return connection;
}


CallSession * B2BUAEndPoint::OnCreateServerCallSession(
  const SIPMessage & request,
  const PString & sessionId
)
{
  if( !request.IsInvite() )
    return NULL;

  /// incoming INVITE... create a session
  B2BUACall * call = new B2BUACall( *this, request, sessionId );
  call->SetLegIndex( 0 );
  B2BUAConnection * b2bua = CreateB2BUA( request, call );

  if( b2bua == NULL )
  {
    delete call;
    return NULL;
  }

  b2bua->OnIncomingCall( *call, request );

  return call;
}


CallSession * B2BUAEndPoint::OnCreateClientCallSession(
  const ProfileUA & profile,
  const PString & sessionId
)
{
  B2BUACall * call = new B2BUACall( *this, sessionId, profile );
  call->SetLegIndex( 1 );
  call->EnableSessionTimer( m_EnableSessionTimer );

  return call;
}

void B2BUAEndPoint::OnReceivedInstantMessage(
  const SIPMessage & message
)
{
  B2BUserAgent& ua = dynamic_cast<B2BUserAgent&>( GetUserAgent() );
  ua.OnReceivedInstantMessage( message );
}

