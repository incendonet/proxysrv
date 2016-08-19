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
 * $Log: B2BUAForkCalls.cxx,v $
 * Revision 1.2  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.1  2006/11/10 08:28:04  rcolobong
 * Added Class to handle Forking an Invite
 *
 *
 *
 */

#include "B2BUAForkCalls.h"
#include "B2BUAConnection.h"

using namespace B2BUA;

/////////////////////////////////////////////////////////////
// B2BUAForkCalls Public Method

B2BUAForkCalls::B2BUAForkCalls( 
  B2BUAConnection & connection 
) : m_Connection( connection )
  , m_B2BUA( (B2BUserAgent &)m_Connection.GetSessionManager().GetUserAgent() )
{
  m_100TryingReceived = FALSE;
  m_EarlyMediaReceived = FALSE;
  m_2xxReceived = FALSE;

  m_ConnectionRef = m_Connection.GCCreateRef();

  m_OutboundList.DisallowDeleteObjects();
}

B2BUAForkCalls::~B2BUAForkCalls()
{
  m_OutboundList.AllowDeleteObjects();
}

BOOL B2BUAForkCalls::CreateForkCalls(
  SIPMessage & invite
)
{
  B2BRoutingInterface::B2BRouteList & routes = m_Connection.GetRoutes();

  if( routes.IsEmpty() )
    return FALSE;

  // store max size of routes in order
  // to remove data without altering the size
  // of routes
  PINDEX maxRouteSize = routes.GetSize();
  
  for( PINDEX index = 0; index < maxRouteSize; ++index )
  {
    B2BUACall * call = CreateOutboundClientSession( 
      GenerateCallId( invite ) );
    
    m_B2BUA.OnOutgoingCall( m_Connection, *call, invite );
    call->OnSetupOutbound( invite, m_Connection );

    routes.RemoveAt( 0 );
  }

  return TRUE;
}

void B2BUAForkCalls::CancelForkCalls()
{
  PWaitAndSignal scopedLock( m_Mutex );

  for( PINDEX index = 0; index < m_OutboundList.GetSize(); ++index )
  {
    B2BUACall * call = (B2BUACall*)m_OutboundList.GetDataAt( index ).GetObject();
    if( call )
    {
      call->SendCancel();
    }
  }

  m_OutboundList.AllowDeleteObjects();
  m_OutboundList.RemoveAll();
}

BOOL B2BUAForkCalls::OnReceived1xxResponse(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  if( !msg.Is1xx() )
    PAssertAlways( PLogicError );

  PWaitAndSignal scopedLock( m_Mutex );

  switch( msg.GetStatusCode() )
  {
  case SIPMessage::Code100_Trying:
    {
      if( m_100TryingReceived )
        return FALSE;

      m_100TryingReceived = TRUE;
      break;
    }
  case SIPMessage::Code180_Ringing:
  case SIPMessage::Code183_SessionProgress:
    {
      if( m_EarlyMediaReceived )
        return FALSE;

      m_EarlyMediaReceived = TRUE;
      break;
    }
  default:
    /// hmmm we don't know how to handle this code
    /// so dont propagate it...
    return FALSE;
  };

  return TRUE;
}

BOOL B2BUAForkCalls::OnReceived2xxResponse(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  if( !msg.Is2xx() )
    PAssertAlways( PLogicError );

  PWaitAndSignal scopedLock( m_Mutex );
  
  if( m_2xxReceived )
    return FALSE;

  if( m_Connection.AttachCall( call.GetCallId(), &call ) )
    m_2xxReceived = TRUE;
  else
    return FALSE;

  for( PINDEX index = 0; index < m_OutboundList.GetSize(); ++index )
  {
    B2BUACall * forkCall = (B2BUACall*)m_OutboundList.GetDataAt( index ).GetObject();
    if( forkCall )
    {
      if( !(call.GetCallId() *= forkCall->GetCallId()) )
      {
        // Send cancel
        forkCall->SendCancel();

        // then destroy session 
        // and release all reference
        forkCall->Destroy();
      }
    }
  }

  m_OutboundList.AllowDeleteObjects();
  m_OutboundList.RemoveAll();

  return TRUE;
}

BOOL B2BUAForkCalls::OnReceivedErrorResponses(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  if( msg.IsRequest() )
    PAssertAlways( PLogicError );

  PWaitAndSignal scopedLock( m_Mutex );

  if( m_2xxReceived )
    return FALSE;

  if( !m_OutboundList.IsEmpty() )
  {
    m_OutboundList.RemoveAt( call.GetCallId() );
  } 

  if( m_OutboundList.IsEmpty() )
  {
    B2BUACall * leg1Call = m_Connection.GetLeg1Call();
    leg1Call->SetCallAnswerResponse( CallSession::DisconnectWithNotFound );

    m_Connection.DestroyConnection( leg1Call->GetRequest() );
  }

  return TRUE;
}

BOOL B2BUAForkCalls::OnReceivedRequest(
  B2BUACall & call,
  const SIPMessage & msg
)
{
  return FALSE;
}


/////////////////////////////////////////////////////////////
// B2BUAForkCalls Protected Method

B2BUACall * B2BUAForkCalls::CreateOutboundClientSession(
  const PString callId
)
{
  PWaitAndSignal scopedLock( m_Mutex );

  ProfileUA defaultProfile = m_B2BUA.GetDefaultProfile();
  B2BUACall * call = (B2BUACall*)m_B2BUA.GetB2BUAEndPoint()->CreateClientSession( 
    defaultProfile, callId );

  call->SetB2BUAConnection( &m_Connection );

  m_OutboundList.SetAt( callId, new SIPSession::GCRef( call->GCCreateRef() ) );

  return call;
}

PString B2BUAForkCalls::GenerateCallId(
  const SIPMessage& msg
)
{
  PStringStream stream;
  stream << msg.GetCallId() << "-outbound-" << m_OutboundList.GetSize();
  return stream;
}

