/*
 *
 * RouteRecord.cxx
 *
 * Open SIP Stack ( OSS )
 *
 * Copyright (c) opensipstack.org.
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either expressed or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is OpenSIPStack.
 *
 * The Initial Developer of the Original Code is opensipstack.org.
 *
 * The author of this code is Joegen E. Baclor
 *
 *
 * $Log: RouteRecord.cxx,v $
 * Revision 1.4  2007/01/22 10:00:58  joegenbaclor
 * Fixed ProxyRouteRequest
 *
 * Revision 1.3  2007/01/02 01:54:59  joegenbaclor
 * Corrected parser bug for Route Records
 *
 * Revision 1.2  2006/11/22 11:33:25  rcolobong
 * 1. Change method FindRoute to HasScheme
 * 2. Fix problem where it "Rounds Robin" in HasScheme method
 *
 * Revision 1.1  2006/08/14 10:04:59  rcolobong
 * Convert B2BUA to SBC
 * Support MP logging
 *
 * Revision 1.2  2006/07/11 13:59:34  joegenbaclor
 * Completed Proxy functionalities for B2BUA
 *
 * Revision 1.1  2006/06/20 09:58:11  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.2  2006/05/19 06:30:37  joegenbaclor
 * 1.  Fixed bug in SIPHeaderB where tag may not be parsed properly if no < > enclosure is present
 * 2.  Various enhancements to OpenSBC and proxy session
 *
 * Revision 1.1  2006/05/17 04:04:48  joegenbaclor
 * Initial upload of OpenSBC files
 *
 *
 */



//Routing:

/// routes calls prefixed by 1212 for example.domain.com to sip:xxx.xxx.xxx.xxx
///<1212*@example.domain.com> sip:xxx.xxx.xxx.xxx

/// routes all calls to example.domain.com to the two destinations using a round robin scheme
///<*@example.domain.com> sip:xxx.xxx.xxx.xxx:5060, sip:xxx.xxx.xxx.xxx:5060

/// relay everything
///<*> *

///Upper Registration:

/// routes REGISTERS for example.domain.com to sip:xxx.xxx.xxx.xxx
///<*@example.domain.com> sip:xxx.xxx.xxx.xxx

/// routes all REGISTERS to example.domain.com to the two destinations using a round robin scheme
/// will hijack contacts if upper-registrar is true
/// will challenge REGISTER if authenticate is true

///<*@example.domain.com;upper-registrar=true;authenticate=false> sip:xxx.xxx.xxx.xxx:5060, sip:xxx.xxx.xxx.xxx:5060

/// relay everything
///<*> *


#include "RouteRecord.h"
#include "ParserTools.h"

using namespace SIPParser;

#define new PNEW

RouteRecord::RouteRecord()
{
  m_CurrentTargetIndex = 0;
}

RouteRecord::RouteRecord( 
  const PString & routeRec
)
{
  m_CurrentTargetIndex = 0;
  operator=( routeRec );
}

RouteRecord::RouteRecord( 
  const RouteRecord & routeRec
)
{
  m_CurrentTargetIndex = 0;
  operator=( routeRec );
}

RouteRecord & RouteRecord::operator=(
  const PString & routeRec
)
{
  Parse( routeRec );
  return *this;
}

RouteRecord & RouteRecord::operator=(
  const RouteRecord & routeRec
)
{
  m_RouteURI = routeRec.m_RouteURI;
  m_Targets = routeRec.m_Targets;
  m_CurrentTargetIndex = routeRec.m_CurrentTargetIndex;
  return *this;
}


BOOL RouteRecord::Parse( 
  const PString & routeRec
)
{
  PINDEX less, greater;

  less = routeRec.Find( '[' );
  if( less == P_MAX_INDEX )
    return FALSE;

  greater = routeRec.Find( ']' );
  if( greater == P_MAX_INDEX )
    return FALSE;

  if( greater <= less )
    return FALSE;

  PString buff = routeRec.Mid( less+1 , greater-less-1 );
  m_RouteURI = buff;

  SIPURI routeURI = buff;
  MimeParam::SortedCollection paramCollection = routeURI.GetParameters();

  PStringStream params;

  for( PINDEX i = 0; i < paramCollection.GetSize(); i++ )
  {
    params << paramCollection.GetDataAt( i );

    if( i < paramCollection.GetSize() -1 )
      params << "; ";
  }

  PString target = routeRec.Mid( greater + 1 );

  if( target.IsEmpty() )
    return FALSE;

  PStringArray targets = target.Tokenise( ',' );

  if( targets.GetSize() == 0 )
    m_Targets.AppendString( target.Trim() );
  else
  {
    for( PINDEX i = 0; i < targets.GetSize(); i++ )
    {
      if( !params.IsEmpty() )
      {
        PStringStream targetString;
        targetString << targets[i].Trim() << "; " << params;
        m_Targets.AppendString( targetString );
      }else
      {
        m_Targets.AppendString( targets[i].Trim() );
      }
    }
  }

  m_TargetsSubIndex = PIntArray( m_Targets.GetSize() );
  for( PINDEX j = 0; j < m_Targets.GetSize(); j++ )
    m_TargetsSubIndex[j] = 0;

  return TRUE;
}

BOOL RouteRecord::SetRouteURI(
  const PString & uri
)
{
  m_RouteURI = uri;
  return TRUE;
}

const PString & RouteRecord::GetRouteURI()const
{
  return m_RouteURI;
}

BOOL RouteRecord::GetTargetURI( 
  SIPURI & target
)
{
  if( m_Targets.GetSize() == 1 )
    return GetTargetURI( target, 0 );

  PTRACE( 1, "Performing routeRobin" << m_CurrentTargetIndex );

  if(  GetTargetURI( target, m_CurrentTargetIndex ) )
  {
    if( ++m_CurrentTargetIndex >= m_Targets.GetSize() )
      m_CurrentTargetIndex = 0;

    return TRUE;
  }

  return FALSE;
}

PStringArray RouteRecord::GetTargetURIList()const
{
  PStringArray list;
  for( PINDEX i = 0; i < m_Targets.GetSize(); i++ )
  {
    SIPURI target;
    if( const_cast<RouteRecord *>(this)->GetTargetURI( target, i ) )
      list.AppendString( target.AsString() );
  }

  return list;
}

BOOL RouteRecord::GetTargetURI( 
  SIPURI & target,
  PINDEX index
)
{
  PINDEX subIndex = m_TargetsSubIndex[index];

  if( index >= m_Targets.GetSize() )
    return FALSE;

  PStringArray targets = m_Targets[index].Tokenise( '|' ); 
 
  if( subIndex >= targets.GetSize() )
    subIndex = 0;

  if( targets.GetSize() > 1 )
  {
    target = targets[subIndex];
    if( ++subIndex >= targets.GetSize() )
      subIndex = 0;
  }else
  {
    target = m_Targets[index];
  }
  
  m_TargetsSubIndex[index] = subIndex;

  return TRUE;
}

BOOL RouteRecord::AppendTargetURI(
  const SIPURI & target
)
{
  m_Targets.AppendString( target.AsString() );
  return TRUE;
}

BOOL RouteRecord::RemoveTargetURI(
  PINDEX index
)
{
  m_Targets.RemoveAt( index );
  return TRUE;
}

BOOL RouteRecord::HasScheme(
  const PString& scheme
)
{
  SIPURI uri;
  for( PINDEX index = 0; index < m_Targets.GetSize(); ++index )
  {
    uri = m_Targets[index];
    if( uri.GetScheme() *= scheme )
      return TRUE;
  }

  return FALSE;
}

PINDEX RouteRecord::GetTargetURICount()const
{
  return m_Targets.GetSize();
}

void RouteRecord::PrintOn( ostream & strm )const
{
  strm << "<" << m_RouteURI << "> ";
  for( PINDEX i = 0; i < m_Targets.GetSize(); i++ )
  {
    strm << m_Targets[i];

    if( i < m_Targets.GetSize() - 1 )
      strm << ", ";
  }
}


BOOL RouteRecord::operator*=(
  const SIPURI & uri
)
{
  PString buff = uri.AsString();
  return operator*=( buff );
}

BOOL RouteRecord::operator*=(
  const PString & uri
)
{
  PString wild = m_RouteURI;

  PStringArray tokens = m_RouteURI.Tokenise( ';' );
  if( tokens.GetSize() > 0 )
    wild = tokens[0];

  BOOL match = ParserTools::WildCardCompare( wild, uri );

  if( match )
    PTRACE( 3, "*** MATCH *** " << wild << " --> " << uri );
  else
    PTRACE( 3, "*** NO MATCH *** " << wild << " --> " << uri );

  return match;
}


