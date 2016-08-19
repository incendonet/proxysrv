/*
 *
 * SDPTime.h
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
 * $Log: SDPTime.h,v $
 * Revision 1.5  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.4  2006/03/16 14:44:18  joegenbaclor
 * 1.  Implemented Clone function in all SDPHeader decendant.
 * 2.  Fixed bug in mime param where "=" characters in the param value may cause the parser to barf.
 * 3.  Implemented Proxy Authentication in CallSession
 * 4.  More work on Opal OSS Classes
 * 5.  Changed UDP Transport write behavior to send outbound message in all interfaces if GetSocket() returns NULL.
 *      Old behavior was to send the packet using the default interface.
 *
 * Revision 1.3  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef SDPTIME_H
#define SDPTIME_H

#include "SDPHeader.h"

namespace SDP
{
  class SDPTime : public SDPHeader
  {
    PCLASSINFO( SDPTime, SDPHeader );
  public:

    SDPTime();

    SDPTime( 
      const SDPHeader & header
    );

    SDPTime(
      const PString & header
    );

    SDPTime(
      const PString & name,
      const PString & value
    );

    SDPTime(
      const SDPTime & header
    );

    SDPTime(
      PUInt64 startTime,
      PUInt64 stopTime
    );

    SDPTime & operator=( const PString & header );

    SDPTime & operator=( const SDPTime & header );

    void SetStartTime(
      PUInt64 startTime
    );

    void SetStopTime(
      PUInt64 stopTime
    );

    PObject * Clone()const;
    
    PINLINE PUInt64 GetStartTime(){ return m_StartTime; };
    PINLINE PUInt64 GetStopTime(){ return m_StopTime; };

  protected:

    PUInt64 m_StartTime;
    PUInt64 m_StopTime;

  };
};

#endif

