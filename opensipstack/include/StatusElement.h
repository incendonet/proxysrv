/*
 *
 * StatusElement.h
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
 * $Log: StatusElement.h,v $
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

#ifndef STATUSELEMENT_H_
#define STATUSELEMENT_H_

#include "ExtendedElement.h"
#include "SIPXMLElement.h"

#if P_EXPAT

using namespace SIPParser;

namespace Presence
{
  namespace PIDFParser
  {
    class StatusElement : public SIPXMLElement
    {

      PCLASSINFO( StatusElement, SIPXMLElement );

    public:
      /*
      (b) STATUS has at least the mutually-exclusive values OPEN and
         CLOSED, which have meaning for the acceptance of INSTANT
         MESSAGES, and may have meaning for other COMMUNICATION MEANS.
         There may be other values of STATUS that do not imply anything
         about INSTANT MESSAGE acceptance.  These other values of STATUS
         may be combined with OPEN and CLOSED or they may be mutually-
         exclusive with those values (RFC2778:Sec.3, RFC2779:Sec.4.4.1-
         4.4.3).
       */
      enum BasicState
      {
        closed,      //The <basic> element contains one of the
        open,       //following strings: "open" or "closed".
        unsupported
      };

      StatusElement( 
        const BasicState & state = open
      );

      StatusElement( 
        const StatusElement & status 
      );

      StatusElement( 
        const SIPXML & status 
      );

      virtual ~StatusElement();

      StatusElement & operator = ( 
        const StatusElement & status 
      );

      StatusElement & operator = ( 
        const SIPXML & status 
      );

      virtual void PrintOn( ostream & strm )const;

      virtual PString AsString();

      virtual PObject * Clone() const;


      PINLINE BasicState GetBasicState(){ return m_BasicState; };

      PINLINE void SetBasicState( const BasicState & state ){ m_BasicState = state; };

    private:
      PString TranslateState( BasicState state );
      void ConstructStatus( ostream & strm );

      BasicState m_BasicState;
      PString m_BasicTag;

    }; //StatusElement
  }; // PIDFParser
}; // Presence

#endif //P_EXPAT
#endif //_STATUSELEMENT_H_


