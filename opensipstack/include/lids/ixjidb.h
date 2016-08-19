/*
	ixjIdb.h

	Copyright (c) 1996-2002, Quicknet Technologies, Inc.
	All Rights Reserved.

	Internet PhoneJACK, Internet LineJACK, etc. definitions.

    -----------------------------------------------------------------

	$Header: /cvsroot/opensipstack/opensipstack/include/lids/ixjidb.h,v 1.1 2006/08/04 03:33:18 joegenbaclor Exp $

    $Log: ixjidb.h,v $
    Revision 1.1  2006/08/04 03:33:18  joegenbaclor
    Moved pwlib and OPAL headers to root include directory

    Revision 1.1  2006/06/26 03:02:28  joegenbaclor
    I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.

    Revision 2.2  2002/03/27 02:21:51  robertj
    Updated to OpenH323 v1.8.4

    Revision 2.1  2002/03/22 06:57:48  robertj
    Updated to OpenH323 version 1.8.2

    Revision 1.3  2002/03/27 00:18:13  robertj
    Added new line to end of file

    Revision 1.2  2002/03/21 21:21:27  craigs
    Added information from ixjDefs.h

	
*/

#ifndef _IXJIDB_H
#define _IXJIDB_H

typedef enum {
  QTH_DETECT_TONE_TYPE_ADD,
  QTH_DETECT_TONE_TYPE_MOD_BEAT
} qthDetectToneType;

typedef enum {
  QTH_DETECT_TONE_REPEAT_LAST,
  QTH_DETECT_TONE_REPEAT_ALL
} qthDetectToneCadenceTerm;

typedef struct {
  UINT32 ulOnTime; // In ms
  UINT32 ulOffTime; // In ms
} qthDetectToneCadenceElement;

typedef struct {
  UINT32 ulFilter;
  UINT32 ulNumElements;
  qthDetectToneType type;
  qthDetectToneCadenceTerm term;
  UINT32 ulTolerance;
  UINT32 ulMinDetectLoops;
  qthDetectToneCadenceElement element[4]; // Array
} qthDetectToneCadence;


#endif
