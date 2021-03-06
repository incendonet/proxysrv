/*
 * OpalWavFile.cxx
 *
 * WAV file class with auto-PCM conversion
 *
 * OpenH323 Library
 *
 * Copyright (c) 2002 Equivalence Pty. Ltd.
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
 * The Original Code is Open H323 Library.
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: opalwavfile.cxx,v $
 * Revision 1.1  2006/06/26 03:02:35  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 2.4  2006/02/13 03:46:17  csoutheren
 * Added initialisation stuff to make sure that everything works OK
 *
 * Revision 2.3  2004/07/15 12:19:23  rjongbloed
 * Various enhancements to the VXML code
 *
 * Revision 2.2  2004/02/19 10:53:04  rjongbloed
 * Merged OpenH323 version 1.13.1 changes.
 *
 * Revision 2.1  2002/09/06 07:19:21  robertj
 * OPAL port.
 *
 * Revision 1.5  2004/07/15 11:14:25  rjongbloed
 * Added missing purity to G.711 decoder function
 *
 * Revision 1.4  2004/07/15 03:18:00  csoutheren
 * Migrated changes from crs_vxnml_devel branch into main trunk
 *
 * Revision 1.3.6.1  2004/07/07 07:10:11  csoutheren
 * Changed to use new factory based PWAVFile
 * Removed redundant blocking/unblocking when using G.723.1
 *
 * Revision 1.3  2003/12/28 00:07:56  csoutheren
 * Added support for 8-bit PCM WAV files
 *
 * Revision 1.2  2002/08/05 10:03:48  robertj
 * Cosmetic changes to normalise the usage of pragma interface/implementation.
 *
 * Revision 1.1  2002/06/20 01:21:32  craigs
 * Initial version
 *
 */

#include <ptlib.h>

#ifdef __GNUC__
#pragma implementation "opalwavfile.h"
#endif

#include <codec/opalwavfile.h>

#include <codec/g711codec.h>



#define new PNEW


OpalWAVFile::OpalWAVFile(unsigned fmt)
  : PWAVFile(fmt)
{
  SetAutoconvert();
}


OpalWAVFile::OpalWAVFile(OpenMode mode, int opts, unsigned fmt)
  : PWAVFile(mode, opts, fmt)
{
  SetAutoconvert();
}


OpalWAVFile::OpalWAVFile(const PFilePath & name, 
                                  OpenMode mode,  /// Mode in which to open the file.
                                       int opts,  /// #OpenOptions enum# for open operation.
                                   unsigned fmt)  /// Type of WAV File to create
  : PWAVFile(name, mode, opts, fmt)
{
  SetAutoconvert();
}


/////////////////////////////////////////////////////////////////////////////////

class PWAVFileConverterXLaw : public PWAVFileConverter
{
  public:
    off_t GetPosition     (const PWAVFile & file) const;
    BOOL SetPosition      (PWAVFile & file, off_t pos, PFile::FilePositionOrigin origin);
    unsigned GetSampleSize(const PWAVFile & file) const;
    off_t GetDataLength   (PWAVFile & file);
    BOOL Read             (PWAVFile & file, void * buf, PINDEX len);
    BOOL Write            (PWAVFile & file, const void * buf, PINDEX len);

    virtual short DecodeSample(int sample) = 0;
};

off_t PWAVFileConverterXLaw::GetPosition(const PWAVFile & file) const
{
  off_t pos = file.RawGetPosition();
  return pos * 2;
}

BOOL PWAVFileConverterXLaw::SetPosition(PWAVFile & file, off_t pos, PFile::FilePositionOrigin origin)
{
  pos /= 2;
  return file.SetPosition(pos, origin);
}

unsigned PWAVFileConverterXLaw::GetSampleSize(const PWAVFile &) const
{
  return 16;
}

off_t PWAVFileConverterXLaw::GetDataLength(PWAVFile & file)
{
  return file.RawGetDataLength() * 2;
}

BOOL PWAVFileConverterXLaw::Read(PWAVFile & file, void * buf, PINDEX len)
{
  // read the xLaw data
  PINDEX samples = (len / 2);
  PBYTEArray xlaw;
  if (!file.PFile::Read(xlaw.GetPointer(samples), samples))
    return FALSE;

  // convert to PCM
  PINDEX i;
  short * pcmPtr = (short *)buf;
  for (i = 0; i < samples; i++)
    *pcmPtr++ = DecodeSample(xlaw[i]);

  // fake the lastReadCount
  file.SetLastReadCount(len);

  return TRUE;
}


BOOL PWAVFileConverterXLaw::Write(PWAVFile & /*file*/, const void * /*buf*/, PINDEX /*len*/)
{
  return FALSE;
}

//////////////////////////////////////////////////////////////////////

class PWAVFileConverterULaw : public PWAVFileConverterXLaw
{
  public:
    unsigned GetFormat(const PWAVFile & /*file*/) const
    { return PWAVFile::fmt_uLaw; }

    short DecodeSample(int sample)
    { return (short)Opal_G711_uLaw_PCM::ConvertSample(sample);}
};

class PWAVFileConverterALaw : public PWAVFileConverterXLaw
{
  public:
    unsigned GetFormat(const PWAVFile & /*file*/) const
    { return PWAVFile::fmt_ALaw; }

    short DecodeSample(int sample)
    { return (short)Opal_G711_ALaw_PCM::ConvertSample(sample);}
};

PWAVFileConverterFactory::Worker<PWAVFileConverterULaw> uLawConverter(PWAVFile::fmt_uLaw, true);
PWAVFileConverterFactory::Worker<PWAVFileConverterALaw> ALawConverter(PWAVFile::fmt_ALaw, true);

namespace PWLibStupidLinkerHacks {
  int opalwavfileLoader;
};

///////////////////////////////////////////////////////////////////////

