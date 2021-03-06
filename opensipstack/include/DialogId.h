/*
 *
 * DialogId.h
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
 * $Log: DialogId.h,v $
 * Revision 1.5  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef DIALOGID_H
#define DIALOGID_H

#include <ptlib.h>
#include "SIPUserAgent.h"

using namespace SIP;

namespace UACORE
{
  class DialogId : public PObject
  {
    PCLASSINFO( DialogId, PObject );
  public:
    DialogId();

    DialogId(
      const PString & dialogId
    );

    DialogId(
      const DialogId & dialogId
    );

    DialogId(
      const SIPMessage & sipMessage
    );

    DialogId( 
      const PString & callId,
      const PString & localTag,
      const PString & remoteTag
    );

    virtual ~DialogId();

    DialogId & operator=(
      const DialogId & dialogId
    );

    DialogId & operator=(
      const PString & dialogId
    );

    BOOL Parse( 
      const PString & dialogId
    );

    DialogId & operator=(
      const SIPMessage & message
    );

    BOOL operator==(
      const PString & dialogId
    );

    BOOL operator==(
      const DialogId & dialogId
    );

    PString AsString()const;

    virtual void PrintOn( 
      ostream & strm 
    )const;

    PINLINE const PString & GetLocalTag()const{ return m_LocalTag; };
    PINLINE const PString & GetRemoteTag()const{ return m_RemoteTag; };
    PINLINE const PString & GetCallId()const{ return m_CallId; };

    PINLINE void SetLocalTag( const PString & localTag ){ m_LocalTag = localTag; };
    PINLINE void SetRemoteTag( const PString & remoteTag ){ m_RemoteTag = remoteTag; };
    PINLINE void SetCallId( const PString & callId ){ m_CallId = callId; };

  protected:
    PString m_LocalTag;
    PString m_RemoteTag;
    PString m_CallId;
  };
};
#endif


