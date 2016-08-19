/*
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
 * $Log: TransactionId.h,v $
 * Revision 1.4  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/03/14 03:13:22  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef TRANSACTIONID_H
#define TRANSACTIONID_H

#include <ptlib.h>



namespace SIPParser
{
  class SIPMessage;

  class TransactionId : public PObject
  {
    PCLASSINFO( TransactionId, PObject );
  public:

    TransactionId();

    TransactionId( 
      const TransactionId & transactionId
    );

    TransactionId( 
      const PString & transactionId
    );

    TransactionId(
      const SIPMessage & msg
    );

    TransactionId & operator=( 
      const TransactionId & transactionId
    );

    TransactionId & operator=( 
      const PString & transactionId
    );

    TransactionId & operator=(
      const SIPMessage & msg
    );

    PString AsString()const;

    void PrintOn( ostream & strm )const;

    PINLINE const PString & GetCallId()const{ return m_CallId; };
    PINLINE void SetCallId( const PString & callId ){ m_CallId = callId; };

    PINLINE const PString & GetBranch()const{ return m_Branch; }; 
    PINLINE void SetBranch( const PString & branch ){ m_Branch = branch; };

    PINLINE const PString & GetMethod()const{ return m_Method; }; 
    PINLINE void SetMethod( const PString & method ){ m_Method = method; };

    PINLINE const PString & GetStateMachine()const{ return m_StateMachine; };
    PINLINE void SetStateMachine( const PString & stateMachine ){ m_StateMachine = stateMachine; };


    /// RFC 2543 support
    PINLINE void SetFromTag( const PString & tag ){ m_FromTag = tag; };
    PINLINE const PString & GetFromTag()const{ return m_FromTag; };
    PINLINE BOOL IsRFC3261Compliant()const{ return m_IsRFC3261Compliant; }; 

  protected:

    PString m_CallId;
    PString m_Branch;
    PString m_Method;
    PString m_StateMachine;

    /// RFC 2543 support
    PString m_FromTag;
    PString m_CSeqNumber;
    BOOL m_IsRFC3261Compliant;
  };
};
#endif


