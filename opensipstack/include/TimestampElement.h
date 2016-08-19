/*
 *
 * TimeStampElement.h
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
 * The author of this code is Solegy Systems c/o Hartley Mendoza
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: TimestampElement.h,v $
 * Revision 1.4  2006/08/23 10:35:53  hartley2481
 * Support for RPID
 *
 * Revision 1.2  2006/06/06 14:21:07  joegenbaclor
 * Corrected #endif with trailing comments and changed #ifdef P_EXPAT to #if P_EXPAT
 *
 * Revision 1.1  2006/06/06 13:30:25  joegenbaclor
 * Initial upload of PIDF files - thanks Solegy Systems!
 *
 *
 */

#ifndef TIMESTAMPELEMENT_H
#define TIMESTAMPELEMENT_H

#include "SIPXMLElement.h"

#if P_EXPAT

using namespace SIPParser;

namespace Presence
{
  namespace PIDFParser
  {
    /*
    The <timestamp> element contains a string indicating the date and
    time of the status change of this tuple.  The value of this element
    MUST follow the IMPP datetime format [RFC3339].  Timestamps that
    contain 'T' or 'Z' MUST use the capitalized forms.
    */
    class TimestampElement : public SIPXMLElement
    {

      PCLASSINFO( TimestampElement, SIPXMLElement );

    public:

      TimestampElement();

      TimestampElement( 
        const TimestampElement & timestamp 
      );

      TimestampElement( 
        const SIPXMLElement & timestamp
      );

      TimestampElement( 
        const PString & timestamp 
      );

      virtual ~TimestampElement();

      TimestampElement & operator = ( 
        const TimestampElement & timestamp 
      );

      TimestampElement & operator = ( 
        const SIPXMLElement & timestamp 
      );

      virtual void PrintOn( ostream & strm )const;

      virtual PString AsString();

      virtual PObject * Clone() const;

      /*
      As a security measure, the <timestamp> element SHOULD be included in
      all tuples unless the exact time of the status change cannot be
      determined.  For security guidelines for watchers receiving presence
      information with timestamps, see the Security Considerations.
      */
      PINLINE PString GetTimestamp(){ return m_Data; };

      PINLINE void SetTimestamp( const PString & timestamp ){ m_Data = timestamp; };

      PString GenerateTimestamp(); 

    private:

    }; //TimestampElement
  }; // PIDFParser
}; // Presence

#endif // P_EXPAT
#endif // TIMESTAMPELEMENT_H

