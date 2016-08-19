/*
 *
 * SIPEvent.cxx
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
 * $Log: SIPEvent.cxx,v $
 * Revision 1.6  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.5  2006/08/29 03:56:44  joegenbaclor
 * 1.  Added GC Reference in SIP Event
 * 2.  Corrected buf in ProcessStackEvent where session is checked against NULL not allowing requests to create new sessions
 *
 * Revision 1.4  2006/03/14 03:53:53  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "SIPTransaction.h"
#include "SIPEvent.h"
#include "SIPMessage.h"


#define new PNEW

using namespace SIPTransactions;
using namespace SIPParser;


      
SIPEvent::SIPEvent(
  PObject * eventObject
) 
{
  m_EventObject = eventObject;
  m_Logger = NULL;
  m_GCRef = NULL;
}

SIPEvent::~SIPEvent()
{
  if( m_EventObject != NULL )
    delete m_EventObject;

  if( m_GCRef != NULL )
    delete m_GCRef;
}


/// copy cosntructor is private and therefore is hidden
SIPEvent::SIPEvent( 
  const SIPEvent & 
)
{
  m_Logger = NULL;
}

SIPEvent & SIPEvent::operator=(
  const SIPEvent & 
)
{
  return *this;
}

void SIPEvent::PrintOn(
  ostream & strm
)const
{
  strm << "Event( Unknown )";
}

///////////////////////////////////////////////////////

SIPTimerEvent::SIPTimerEvent()
{
  m_Type = None;
}

SIPTimerEvent::SIPTimerEvent(
  const SIPTimerEvent & event
)
{
  m_Type = event.m_Type;
  m_Interval = event.m_Interval;
}

SIPTimerEvent::SIPTimerEvent(
  SIPTimerEvent::Type type,
  PInt64 duration
)
{
  m_Type = type;
  m_Interval = duration;
}

SIPTimerEvent::~SIPTimerEvent()
{
}

SIPTimerEvent & SIPTimerEvent::operator=(
  const SIPTimerEvent & event
)
{
  m_Type = event.m_Type;
  m_Interval = event.m_Interval;
  return *this;
}

SIPTimerEvent & SIPTimerEvent::operator=(
  SIPTimerEvent::Type type
)
{
  m_Type = type;
  m_Interval = 0;
  return *this;
}

BOOL SIPTimerEvent::operator==( 
  SIPTimerEvent::Type type 
)
{
  return type == m_Type;
}

void SIPTimerEvent::PrintOn(
  ostream & strm
)const
{
  switch( m_Type )
  {
    case A:
      strm << "Event( Timer-A )";
      break;
    case B:
      strm << "Event( Timer-B )";
      break;
    case C:
      strm << "Event( Timer-C )";
      break;
    case D:
      strm << "Event( Timer-D )";
      break;
    case E:
      strm << "Event( Timer-E )";
      break;
    case F:
      strm << "Event( Timer-F )";
      break;
    case G:
      strm << "Event( Timer-G )";
      break;
    case H:
      strm << "Event( Timer-H )";
      break;
    case I:
      strm << "Event( Timer-I )";
      break;
    case J:
      strm << "Event( Timer-J )";
      break;
    case K:
      strm << "Event( Timer-K )";
      break;
    case T1:
      strm << "Event( Timer-T1 )";
      break;
    case T2:
      strm << "Event( Timer-T2 )";
      break;
    case T4:
      strm << "Event( Timer-T4 )";
      break;
    default:
      strm << "Event( Timer-Unknown )";
  }

  strm << " Interval: " << m_Interval;
}


////////////////////////////////////////////////////////




