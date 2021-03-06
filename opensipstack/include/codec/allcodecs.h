/*
 * allcodecs.h
 *
 * Open Phone Abstraction Library (OPAL)
 * Formally known as the Open H323 project.
 *
 * Copyright (c) 2001 Equivalence Pty. Ltd.
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
 * The Original Code is Open Phone Abstraction Library.
 *
 * The Initial Developer of the Original Code is Equivalence Pty. Ltd.
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: allcodecs.h,v $
 * Revision 1.3  2006/09/06 04:14:34  joegenbaclor
 * Added check on packet size for SIP UDP Transport Reads
 *
 * Revision 1.2  2006/08/28 07:18:35  joegenbaclor
 * Bulk Commit: Logging overhaul and Call Session callback changes to reflect SIPMessage eg:  OnAlerting(), OnProgress(), etc
 *
 * Revision 1.1  2006/08/04 03:33:18  joegenbaclor
 * Moved pwlib and OPAL headers to root include directory
 *
 * Revision 1.1  2006/06/26 03:02:25  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 2.10  2006/04/17 00:06:27  csoutheren
 * Allow ILBC codec to be disabled and detect if not present
 *
 * Revision 2.9  2006/01/01 19:19:32  dsandras
 * Added RFC2190 H.263 codec thanks to Salyens. Many thanks!
 *
 * Revision 2.8  2004/05/15 12:53:40  rjongbloed
 * Fixed incorrect laoding of H.323 capability for G.726
 *
 * Revision 2.7  2004/02/19 10:46:43  rjongbloed
 * Merged OpenH323 version 1.13.1 changes.
 *
 * Revision 2.6  2004/02/17 08:48:57  csoutheren
 * Disabled VoiceAge G.729 codec on Linux
 *
 * Revision 2.5  2003/06/02 04:04:54  rjongbloed
 * Changed to use new autoconf system
 *
 * Revision 2.4  2003/03/17 10:26:59  robertj
 * Added video support.
 *
 * Revision 2.3  2002/11/10 11:33:16  robertj
 * Updated to OpenH323 v1.10.3
 *
 * Revision 2.2  2002/07/01 04:56:29  robertj
 * Updated to OpenH323 v1.9.1
 *
 * Revision 2.1  2001/08/01 05:03:26  robertj
 * Changes to allow control of linking software transcoders, use macros
 *   to force linking.
 *
 */

#ifndef __CODEC_ALLCODECS_H
#define __CODEC_ALLCODECS_H

#include <opal/buildopts.h>

#include <codec/g711codec.h>
OPAL_REGISTER_G711();

#if ENABLE_MYACM
#include <codec/g7231codec.h>
OPAL_REGISTER_G7231();
#endif

#if VOICE_AGE_G729A
#include <codec/g729codec.h>
OPAL_REGISTER_G729();
#endif

#include <codec/gsmcodec.h>
OPAL_REGISTER_GSM0610();

#include <codec/g726codec.h>
OPAL_REGISTER_G726();

//#include <codec/mscodecs.h>
//OPAL_REGISTER_MSCODECS();

#include <codec/lpc10codec.h>
OPAL_REGISTER_LPC10();

#include <codec/speexcodec.h>
OPAL_REGISTER_SPEEX();

#ifdef OPAL_ILBC
#include <codec/ilbccodec.h>
OPAL_REGISTER_iLBC();
#endif

#include <codec/vidcodec.h>
OPAL_REGISTER_UNCOMPRESSED_VIDEO();

#include <codec/h261codec.h>
OPAL_REGISTER_H261();

#ifdef RFC2190_AVCODEC
#include <codec/h263codec.h>
OPAL_REGISTER_H263();
#endif // RFC2190_AVCODEC

#endif // __CODEC_ALLCODECS_H


/////////////////////////////////////////////////////////////////////////////
