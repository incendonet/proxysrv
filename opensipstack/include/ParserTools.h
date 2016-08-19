/*
 *
 * ParserTools.h
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
 * $Log: ParserTools.h,v $
 * Revision 1.13  2006/11/30 08:19:36  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.12  2006/11/10 08:01:29  rcolobong
 * Add property method to configure XOR Hash keys
 *
 * Revision 1.11  2006/08/18 07:12:23  joegenbaclor
 * 1.  added Function overload for GetContentType
 * 2.  Changed unix make files to not include opensbc in make process
 *
 * Revision 1.10  2006/07/17 11:36:45  joegenbaclor
 * 1.  More routing enhancements to B2BUA
 *
 * Revision 1.9  2006/07/10 06:29:37  joegenbaclor
 * Completed crude Registration support for B2BUA
 *
 * Revision 1.8  2006/07/06 05:38:10  joegenbaclor
 * Final last minute changes to CBCOM hash support
 *
 * Revision 1.7  2006/06/30 07:12:21  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.6  2006/06/09 07:37:30  joegenbaclor
 * 1.  Added Publish, SIPETag & SIPIfMatch  support in SIPMessage
 * 2.  Started work on StatusPage Resource for OSSAppConfig
 *
 * Revision 1.5  2006/05/17 03:43:07  joegenbaclor
 * Bulk commit after sourceforge cvs migration changes
 *
 * Revision 1.4  2006/03/14 03:13:21  joegenbaclor
 * Initial header upload for BETA Release Candidate
 *
 *
 */

#ifndef PARSERTOOLS_H
#define PARSERTOOLS_H

#include <ptlib.h>
#include <ptclib/cypher.h>
#include "ossbuildopts.h"

namespace SIPParser
{
  class ParserTools
  {
  public:

    
    static void SliceMIME(
      const PString & mime,
      PString & name,
      PString & value
    );

    static PString LineFeedSanityCheck( 
      const PString & buff 
    );

    static BOOL IsKnownHeader(
      const PString & header
    );

    static BOOL IsNumeric(
      const PString & value
    );

    static PString EscapeAsRFC2396( 
      const PString & str 
    );

    static PString UnescapeAsRFC2396( 
      const PString & str 
    );

    static PString GenGUID();

    static PString GenBranchParameter();

    static PString GenTagParameter();

    static PString GetRFC1123Date();

    static PString GetReasonPhrase(
      WORD statusCode
    );

    static BOOL MsgTokenize(
      const PString & msg,
      PStringArray & tokens
    );

    static BOOL MsgTokenize(
      const PBYTEArray & msg,
      PStringArray & tokens
    );

    static PString GetExpandedHeader(
      const PString & compactHeader
    );

    static PString GetCompactedHeader(
      const PString & expandedHeader
    );

    static PString AsHex(
      const PMessageDigest5::Code & digest
    );

    static PString Quote( 
      const PString & str 
    );

    static PString UnQuote( 
      PString & str 
    );

    static BOOL WildCardCompare(
      const char *wild, 
      const char *string
    ); 

    static BOOL XORHash(
      const PBYTEArray & in,
      PBYTEArray & out,
      WORD twoByteKey,
      BYTE oneByteKey,
      BOOL endianSwap = TRUE
    );

    static void SetDefaultXORHashKey1(
      WORD twoByteKey
    );

    static WORD GetDefaultXORHashKey1();

    static void SetDefaultXORHashKey2(
      BYTE oneByteKey
    );

    static BYTE GetDefaultXORHashKey2();

    static void EndianSwap(
      WORD& x
    );

    static void EndianSwap(
      DWORD& x
    );

#ifdef P_HAS_INT64
    static void EndianSwap(
      PUInt64& x
    );
#endif

    static PString StripHTMLTags( 
      const PString & html 
    );

  protected:
    
    static PSortedStringList m_RegisteredHeaders;
    static PMutex m_RegisteredHeadersMutex;
    static BOOL m_RegisteredHeaderInitialize;
    static WORD m_DefaultHashKey1;
    static BYTE m_DefaultHashKey2;
    
  };

};

#endif

