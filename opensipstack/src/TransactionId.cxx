/*
 *
 * TransactionId.cxx
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
 * $Log: TransactionId.cxx,v $
 * Revision 1.5  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/03/14 03:53:54  joegenbaclor
 * Initial source upload for BETA Release Candidate
 *
 *
 *
 */

#include "TransactionId.h"
#include "SIPMessage.h"

#define new PNEW

using namespace SIPParser;

#define MAGIC_COOKIE "z9hG4bK"

TransactionId::TransactionId()
  : m_IsRFC3261Compliant( FALSE )
{
}

TransactionId::TransactionId(
  const TransactionId & transactionId
) : m_IsRFC3261Compliant( FALSE )
{
  operator=( transactionId );
}

TransactionId::TransactionId(
  const PString & transactionId
) : m_IsRFC3261Compliant( FALSE )
{
  operator=( transactionId );
}

TransactionId::TransactionId(
  const SIPMessage & msg
) : m_IsRFC3261Compliant( FALSE )
{
  operator=( msg );
}

TransactionId & TransactionId::operator=(
  const TransactionId & transactionId
)
{
  m_StateMachine = transactionId.m_StateMachine;
  m_CallId = transactionId.m_CallId;
  m_Branch = transactionId.m_Branch;
  m_Method = transactionId.m_Method;

  m_IsRFC3261Compliant = transactionId.m_IsRFC3261Compliant;
  m_FromTag = transactionId.m_FromTag;
  m_CSeqNumber = transactionId.m_CSeqNumber;

  return *this;
}

TransactionId & TransactionId::operator=(
  const PString & transactionId
)
{

  PStringArray tokens = transactionId.Tokenise( "|" );

  if( tokens.GetSize() == 3 )
  {
    m_CallId = tokens[0];
    m_Branch = tokens[1];
    m_Method = tokens[2];
  }else if( tokens.GetSize() == 4 )
  {
    m_StateMachine = tokens[0];
    m_CallId = tokens[1];
    m_Branch = tokens[2];
    m_Method = tokens[3];
  }else if( tokens.GetSize() == 5 )
  {
    m_CallId = tokens[0];
    m_Branch = tokens[1];
    m_Method = tokens[2];
    m_FromTag = tokens[3];
    m_CSeqNumber = tokens[4];
  }else if( tokens.GetSize() == 6 )
  {
    m_StateMachine = tokens[0];
    m_CallId = tokens[1];
    m_Branch = tokens[2];
    m_Method = tokens[3];
    m_FromTag = tokens[4];
    m_CSeqNumber = tokens[5];
  }

  if( m_Branch.Find( MAGIC_COOKIE ) == P_MAX_INDEX )
      m_IsRFC3261Compliant = FALSE;

  return *this;
}

PString TransactionId::AsString()const
{
  PStringStream strm;
  PrintOn( strm );
  return strm;
}

void TransactionId::PrintOn( ostream & strm )const
{
  if( !m_StateMachine.IsEmpty() )
    strm << m_StateMachine << "|";

  strm << m_CallId << "|";
  strm << m_Branch << "|";
  strm << m_Method;

  if( !m_IsRFC3261Compliant )
  {
    if( !m_FromTag.IsEmpty() && !m_CSeqNumber.IsEmpty() )
      strm << "|" << m_FromTag << "|" << m_CSeqNumber;
  }
}

TransactionId & TransactionId::operator=(
  const SIPMessage & msg
)
{
  /*
  17.1.3 Matching Responses to Client Transactions

  1.  If the response has the same value of the branch parameter in
      the top Via header field as the branch parameter in the top
      Via header field of the request that created the transaction.

  2.  If the method parameter in the CSeq header field matches the
      method of the request that created the transaction.  The
      method is needed since a CANCEL request constitutes a
      different transaction, but shares the same value of the branch
      parameter.
  */

  CallId cid;
  if( !msg.GetCallId( cid ) )
  {
    PTRACE( 10, "Parser\tTransactionId::operator=() - No Call-ID present in message" );
  }else
  {
    m_CallId = cid.GetHeaderBody();
  }

  if( m_CallId.IsEmpty() )
  {
    PTRACE( 10, "Parser\tTransactionId::operator=() - No Call-ID present in message" );
  }

  Via via;
  if( msg.GetViaSize() == 0 )
  {
    PTRACE( 10, "Parser\tTransactionId::operator=() - No Via present in message" );
  }else
  {
    msg.GetViaAt( 0, via );
    m_Branch = via.GetBranch();
    if( m_Branch.IsEmpty() )
    {
      PTRACE( 10, "Parser\tTransactionId::operator=() - No Branch Paramemter in Via present in message" );
    }
  }

  if( m_Branch.Find( MAGIC_COOKIE ) != P_MAX_INDEX )
      m_IsRFC3261Compliant = TRUE;

  CSeq cseq;

  if( !msg.GetCSeq( cseq ) )
  {
    PTRACE( 10, "Parser\tTransactionId::operator=() - No CSeq present in message" );
  }else
  {
    m_Method = cseq.GetMethod();

  }

  if( m_Method.IsEmpty() )
  {
    PTRACE( 10, "Parser\tTransactionId::operator=() - No CSeq Method present in message" );
  }

  if( !m_IsRFC3261Compliant )
  {
    m_CSeqNumber = PString( cseq.GetSequence() );

    From from;
    if( msg.GetFrom( from ) )
    {
      from.GetParameter( "tag", m_FromTag );

      if( m_FromTag.IsEmpty() )
      {
        PTRACE( 10, "Parser\tTransactionId::operator=() - No FROM Tag present in message" );
      }
    }
  }

  return *this;
}
