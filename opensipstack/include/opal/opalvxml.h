/*
 * opalvxml.h
 *
 * Header file for IVR code
 *
 * A H.323 IVR application.
 *
 * Copyright (C) 2002 Equivalence Pty. Ltd.
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.0 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is Portable Windows Library.
 *
 * The Initial Developer of the Original Code is Equivalence Pty. Ltd.
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: opalvxml.h,v $
 * Revision 1.1  2006/08/04 03:33:19  joegenbaclor
 * Moved pwlib and OPAL headers to root include directory
 *
 * Revision 1.1  2006/06/26 03:02:30  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 2.3  2004/03/11 06:54:27  csoutheren
 * Added ability to disable SIP or H.323 stacks
 *
 * Revision 2.2  2003/03/07 05:48:12  robertj
 * Added OnEndSession() to automatically close connection.
 *
 * Revision 2.1  2002/11/10 11:33:17  robertj
 * Updated to OpenH323 v1.10.3
 *
 * Revision 1.15  2002/09/16 01:14:15  robertj
 * Added #define so can select if #pragma interface/implementation is used on
 *   platform basis (eg MacOS) rather than compiler, thanks Robert Monaghan.
 *
 * Revision 1.14  2002/08/27 02:21:07  craigs
 * Added silence detection capability to fake G.723.1codec
 *
 * Revision 1.13  2002/08/15 08:22:25  craigs
 * Added P_EXPAT test back in
 *
 * Revision 1.12  2002/08/15 04:55:26  robertj
 * Fixed shutdown problems with closing vxml session, leaks a thread.
 * Fixed potential problems with indirect channel Close() function.
 *
 * Revision 1.11  2002/08/06 06:00:38  craigs
 * Removed ifdef test for EXPLATFLAG
 *
 * Revision 1.10  2002/08/06 05:11:24  craigs
 * Moved most of stuff to ptclib
 *
 * Revision 1.9  2002/08/05 09:43:30  robertj
 * Added pragma interface/implementation
 * Moved virtual into .cxx file
 *
 * Revision 1.8  2002/07/29 15:13:51  craigs
 * Added autodelete option to PlayFile
 *
 * Revision 1.7  2002/07/18 04:17:12  robertj
 * Moved virtuals to source and changed name of G.723.1 file capability
 *
 * Revision 1.6  2002/07/10 13:16:19  craigs
 * Moved some VXML classes from Opal back into PTCLib
 * Added ability to repeat outputted data
 *
 * Revision 1.5  2002/07/09 08:27:23  craigs
 * Added GetMediaFormat to channels
 *
 * Revision 1.4  2002/07/03 01:53:58  craigs
 * Added newline to the end of the file for Linux
 *
 * Revision 1.3  2002/07/02 06:32:04  craigs
 * Added recording functions
 *
 * Revision 1.2  2002/06/28 02:42:54  craigs
 * Fixed problem with G.723.1 file codec not identified as native format
 *
 * Revision 1.1  2002/06/27 05:44:39  craigs
 * Initial version
 *
 * Revision 1.2  2002/06/26 09:05:28  csoutheren
 * Added ability to utter various "sayas" types within prompts
 *
 * Revision 1.1  2002/06/26 01:13:53  csoutheren
 * Disassociated VXML and Opal/OpenH323 specific elements
 *
 *
 */

#ifndef _OPAL_OPALVXML_H
#define _OPAL_OPALVXML_H

#ifdef P_USE_PRAGMA
#pragma interface
#endif

#include <opal/buildopts.h>

#include <ptclib/vxml.h>


class OpalConnection;


//////////////////////////////////////////////////////////////////


#if P_EXPAT

class PTextToSpeech;

class OpalVXMLSession : public PVXMLSession 
{
  PCLASSINFO(OpalVXMLSession, PVXMLSession);
  public:
    OpalVXMLSession(
      OpalConnection * _conn,
      PTextToSpeech * tts = NULL,
      BOOL autoDelete = FALSE
    );

    virtual BOOL Close();
    virtual void OnEndSession();

    virtual PWAVFile * CreateWAVFile(
      const PFilePath & fn,
      PFile::OpenMode mode,
      int opts,
      unsigned fmt
    );

  protected:
    OpalConnection * conn;
};

#endif

#endif // _OPAL_OPALVXML_H


// End of File ///////////////////////////////////////////////////////////////
