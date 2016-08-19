/*
 *
 * VoiceFileChannel.cxx
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
 * $Log: VoiceFileChannel.cxx,v $
 * Revision 1.9  2006/12/18 10:31:35  joegenbaclor
 * Fixed ACM G.723.1 one way audio bug
 *
 * Revision 1.8  2006/12/14 08:28:48  joegenbaclor
 * Added VoiceFile Destructor.
 *
 * Revision 1.7  2006/12/13 10:01:26  joegenbaclor
 * More work on Softphone
 *
 * Revision 1.6  2006/12/12 23:53:34  joegenbaclor
 * More work on DTMF encoding
 *
 * Revision 1.5  2006/12/12 05:26:10  joegenbaclor
 * Modified DTMF player to use PDTMFEncoder
 *
 * Revision 1.4  2006/11/30 08:19:38  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/08/15 15:46:43  joegenbaclor
 * Added ability to stop synchronous playback of voice channels
 *
 * Revision 1.2  2006/08/01 03:55:42  joegenbaclor
 * 1.  Completed Initial classes for IVR Prompt channel
 * 2.  Added support for Bamboo Dynamic Hash Table for Network Overlay support
 *
 * Revision 1.1  2006/07/26 00:44:52  joegenbaclor
 * Added Voice File classes for upcoming IVR support
 *
 *
 */

#include "VoiceFileChannel.h"
#include "ParserTools.h"
#include "ptclib/dtmf.h"
#include "ptlib/sockets.h"

#define new PNEW

using namespace MS;
using namespace SIPParser;
/*
enum Format
{
  Wav_8khz_Mono,
  Raw_G723,
  Raw_G729,
  NumFormat
};*/

#define	G729_SAMPLES_PER_BLOCK	80
#define	G729_BANDWIDTH		      (8000/100) //bits per sec
#define G729_BYTES_PER_FRAME    10 //bytes
#define G729_ALGORITHMIC_DELAY  10 //ms

#define G723_ALGORITHMIC_DELAY 30 //ms

/// Default constructor
VoiceFile::VoiceFile()
{
  m_Format = VoiceFile::NumFormat;
}

/// Constructs a voice file from a chunk of memery
VoiceFile::VoiceFile(
  const PBYTEArray & data,
  VoiceFile::Format format,
  const PString & fileId
  ) : PMemoryFile( data )
{
  m_Format = format;
  m_FileId = fileId;
}

///construct a voice file from another instance of a voicefile
VoiceFile::VoiceFile(
  const VoiceFile & voiceFile
)
{
  data = voiceFile.data;
  m_Format = voiceFile.m_Format;
  m_FileId = voiceFile.m_FileId;
}

VoiceFile::~VoiceFile()
{
  StopPlaying();
}

///construct a voice file from an HTTP URL
BOOL VoiceFile::Open(
  const PURL & url,
  VoiceFile::Format format,
  const PString & fileId
)
{
  m_Format = format;
  m_FileId = fileId;

  // open the resource
  PHTTPClient client;
  PMIMEInfo outMIME, replyMIME;

  if ( !client.GetDocument(url, outMIME, replyMIME, FALSE) || (replyMIME(PHTTP::TransferEncodingTag) *= PHTTP::ChunkedTag))
    return FALSE;


  data.SetSize( 0 );
  BYTE buff;
  for( PINDEX i = 0; client.Read( &buff, 1 ) ; i++ )
    data[i] = buff;
  
  SetPosition( 0 );

  return TRUE;
}

/// Constructs a voice file from local storage
BOOL VoiceFile::Open(
  const PFilePath & path,
  VoiceFile::Format format,
  const PString & fileId
)
{
  m_Format = format;
  m_FileId = fileId;

  // open the resource
  PFile * fileChannel;

  if( format == VoiceFile::Wav_8khz_Mono )
    fileChannel = new PWAVFile();
  else
    fileChannel = new PFile();

  if( !fileChannel->Open( path, PFile::ReadOnly ) )
  {
    delete fileChannel;
    return FALSE;
  }

  data.SetSize( 0 );
  BYTE buff;
  for( PINDEX i = 0; fileChannel->Read( &buff, 1 ) ; i++ )
    data[i] = buff;
  
  SetPosition( 0 );

  delete fileChannel;

  return TRUE;
}

/// Constructs a voice file from a chunk of memory
BOOL VoiceFile::Open(
  const PBYTEArray & dataToCopy,
  VoiceFile::Format format,
  const PString & fileId
)
{
  m_Format = format;
  m_FileId = fileId;
  data.SetSize( dataToCopy.GetSize() );
  ::memcpy( data.GetPointer(),  const_cast<PBYTEArray &>(dataToCopy).GetPointer(), dataToCopy.GetSize() ); 
  SetPosition( 0 );

  return TRUE;
}

/// Saves the content of voice file to local storage
/// in accordance to the Format type.  Example.  Wav_8khz_Mono format
/// would be saved using the .wav extension and Raw_G723 would be saved
/// with .g723 extension.  The id of the file will be used
/// as the filename if the filename is not specified.
/// Returns TRUE if saving is sucessful.
BOOL VoiceFile::SaveToFile(
  const PDirectory & dir,
  const PString & fn
)
{
  
  if( m_Format == VoiceFile::NumFormat )
    return FALSE;

  PString fileName = fn;

  if( fileName.IsEmpty() )
    fileName = m_FileId;

  
  PFile * destFile = NULL;
  PString ext;

  if( m_Format == VoiceFile::Wav_8khz_Mono )
  {
    ext = ".wav";
    destFile = new PWAVFile();
  }else
  {
    if( m_Format == VoiceFile::Raw_G723 )
      ext = ".g723";
    else
      ext = ".g729";

    destFile = new PFile();
  }

  PFilePath path;
  
  if( !dir.IsEmpty() )
    path = dir + fileName;
  else
    path = fileName;

  if( !destFile->Open( path, PFile::WriteOnly ) )
  {
    delete destFile;
    return FALSE;
  }


  SetPosition( 0 );

  for( PINDEX i = 0; i < data.GetSize(); i++ )
    destFile->WriteChar( ReadChar() );

  SetPosition( 0 );

  delete destFile;

  return TRUE;
}

/// Plays the content of this file using the default output device
BOOL VoiceFile::PlaySound()
{
  if( m_Format != VoiceFile::Wav_8khz_Mono )
    return FALSE;

  PString device = PSoundChannel::GetDefaultDevice( PSoundChannel::Player );
  PSoundChannel soundChannel( device, PSoundChannel::Player );
  return PlaySound( soundChannel );
}

/// Plays the content of the voicefile using the specified sound channel
BOOL VoiceFile::PlaySound(
  PSoundChannel & soundChannel
)
{
  if( m_Format != VoiceFile::Wav_8khz_Mono )
    return FALSE;

  
  PINDEX offSet = 0;

  m_IsPlaying = TRUE;
  while( m_IsPlaying )
  {
    soundChannel.Write( &(data[offSet]), 320 );
      offSet += 320;
    if(  offSet >= data.GetSize() )
      break;
    else
      m_Delay.Delay( 20 );
  }

  return TRUE;
}

/*
#include "inlines/dtmf_0.h"
#include "inlines/dtmf_1.h"
#include "inlines/dtmf_2.h"
#include "inlines/dtmf_3.h"
#include "inlines/dtmf_4.h"
#include "inlines/dtmf_5.h"
#include "inlines/dtmf_6.h"
#include "inlines/dtmf_7.h"
#include "inlines/dtmf_8.h"
#include "inlines/dtmf_9.h"
#include "inlines/dtmf_10.h"
#include "inlines/dtmf_11.h"
#include "inlines/ring_0.h"
*/



namespace MS
{
VoiceFile DTMFTone_0( PDTMFEncoder( "0", 100 ), VoiceFile::Wav_8khz_Mono, "DTMF_0" );
VoiceFile DTMFTone_1( PDTMFEncoder( "1", 100 ), VoiceFile::Wav_8khz_Mono, "DTMF_1" );
VoiceFile DTMFTone_2( PDTMFEncoder( "2", 100 ), VoiceFile::Wav_8khz_Mono, "DTMF_2" );
VoiceFile DTMFTone_3( PDTMFEncoder( "3", 100 ), VoiceFile::Wav_8khz_Mono, "DTMF_3" );
VoiceFile DTMFTone_4( PDTMFEncoder( "4", 100 ), VoiceFile::Wav_8khz_Mono, "DTMF_4" );
VoiceFile DTMFTone_5( PDTMFEncoder( "5", 100 ), VoiceFile::Wav_8khz_Mono, "DTMF_5" );
VoiceFile DTMFTone_6( PDTMFEncoder( "6", 100 ), VoiceFile::Wav_8khz_Mono, "DTMF_6" );
VoiceFile DTMFTone_7( PDTMFEncoder( "7", 100 ), VoiceFile::Wav_8khz_Mono, "DTMF_7" );
VoiceFile DTMFTone_8( PDTMFEncoder( "8", 100 ), VoiceFile::Wav_8khz_Mono, "DTMF_8" );
VoiceFile DTMFTone_9( PDTMFEncoder( "9", 100 ), VoiceFile::Wav_8khz_Mono, "DTMF_9" );
VoiceFile DTMFTone_10( PDTMFEncoder( "*", 100 ), VoiceFile::Wav_8khz_Mono, "DTMF_10" );
VoiceFile DTMFTone_11(PDTMFEncoder( "#", 100 ), VoiceFile::Wav_8khz_Mono, "DTMF_11" );
}

///////////////////////////////////////////////////////////

VoiceFileChannel::VoiceFileChannel(
  VoiceFile::Format format 
)
{
  m_SilenceCount = 20;         // wait 20 frames before playing the OGM
  m_Closed        = FALSE;
  m_BytesPlayed = m_FrameLen = m_FrameOffs = 0;
  m_Format = format;
}

BOOL VoiceFileChannel::Read( 
  void * buffer, PINDEX amount 
)
{
  PWaitAndSignal lock(m_ChanMutex);

  // if the channel is closed, then return error
  if (m_Closed )
    return FALSE;

  // Create the frame buffer using the amount of bytes the codec wants to
  // read. Different codecs use different read sizes.
  m_FrameBuffer.SetMinSize(1024);//amount);

  // assume we are returning silence
  BOOL doSilence = TRUE;
  BOOL frameBoundary = FALSE;

  // if still outputting a frame from last time, then keep doing it
  if (m_FrameOffs < m_FrameLen) 
  {
    frameBoundary = AdjustFrame(buffer, amount);
    doSilence = FALSE;
  }else 
  {

    // if we are returning silence frames, then 
    if( m_SilenceCount > 0 ) 
      m_SilenceCount--;
    // if a channel is already open, don't do silence
    else if (GetBaseReadChannel() != NULL)
      doSilence = FALSE;

    // If not in silence and no existing channel, open a new file.
    else 
    {
      PFile  * chan = m_PlayQueue.Dequeue();
      if(chan != NULL) 
      {
        m_CurrentPlayFile = chan; 
        SetReadChannel(chan, TRUE);
        doSilence = FALSE;
      }
	  }

    // if not doing silence, try and read from the file
    if (!doSilence) 
    {
  
      if (ReadFrame(amount)) 
      {  
        frameBoundary = AdjustFrame(buffer, amount);
        m_TotalData += amount;
      } else 
      {
        OnFinishedPlayFile( *m_CurrentPlayFile );
        m_BytesPlayed = m_TotalData;
        PIndirectChannel::Close();
        m_SilenceCount = 5;   // always do 5 frames of silence after every file
        // no silence
        doSilence = TRUE;
      }
    }
  }
  
  // start silence frame if required
  if (doSilence) {
    CreateSilenceFrame(amount);
    frameBoundary = AdjustFrame(buffer, amount);
  }

  // delay to synchronise to frame boundary
  if (frameBoundary)
    Synchronize(amount);

  return TRUE;
}

void VoiceFileChannel::PlayFile( 
  PFile * chan 
)
{
  PWaitAndSignal mutex(m_ChanMutex);
  m_TotalData = 0;
  SetReadChannel(chan, TRUE);
}

BOOL VoiceFileChannel::Close()
{
  PWaitAndSignal mutex(m_ChanMutex);
  m_Closed = TRUE;
  PIndirectChannel::Close();
  return TRUE;
}

void VoiceFileChannel::QueueFile(
  const PFilePath & loc
)
{
  VoiceFile * voiceFile = NULL;
  if( ParserTools::WildCardCompare( "*.wav", loc.GetFileName() ) )
  {
    if( voiceFile->Open( loc, VoiceFile::Wav_8khz_Mono, loc.GetFileName() ) )
      m_PlayQueue.Enqueue( voiceFile );
    else
      delete  voiceFile;
  }else if( ParserTools::WildCardCompare( "*.g7231", loc.GetFileName() ) )
  {
    if( voiceFile->Open( loc, VoiceFile::Raw_G723, loc.GetFileName() ) )
      m_PlayQueue.Enqueue( voiceFile );
    else
      delete voiceFile;
  }else if( ParserTools::WildCardCompare( "*.g729", loc.GetFileName() ) )
  {
    if( voiceFile->Open( loc, VoiceFile::Raw_G729, loc.GetFileName() ) )
      m_PlayQueue.Enqueue( voiceFile );
    else
      delete voiceFile;
  }else
  {
    delete voiceFile;
  }
}

void VoiceFileChannel::QueueFile(
  const PURL & loc
)
{
  VoiceFile * voiceFile = NULL;
  if( ParserTools::WildCardCompare( "*.wav", loc.AsString() ) )
  {
    if( voiceFile->Open( loc, VoiceFile::Wav_8khz_Mono, loc.AsString() ) )
      m_PlayQueue.Enqueue( voiceFile );
    else
      delete  voiceFile;
  }else if( ParserTools::WildCardCompare( "*.g7231", loc.AsString() ) )
  {
    if( voiceFile->Open( loc, VoiceFile::Raw_G723, loc.AsString() ) )
      m_PlayQueue.Enqueue( voiceFile );
    else
      delete voiceFile;
  }else if( ParserTools::WildCardCompare( "*.g729", loc.AsString() ) )
  {
    if( voiceFile->Open( loc, VoiceFile::Raw_G729, loc.AsString() ) )
      m_PlayQueue.Enqueue( voiceFile );
    else
      delete voiceFile;
  }else
  {
    delete voiceFile;
  }
}

void VoiceFileChannel::QueueFile( 
  const PFile & chan 
)
{
  PWaitAndSignal mutex( m_ChanMutex);
  ///m_PlayQueue.Enqueue(new PString( *fn ));
  if( PIsDescendant( &chan, VoiceFile ) )
  {
    m_PlayQueue.Enqueue( new VoiceFile( (const VoiceFile&)chan ) );
  }else
  {
    VoiceFile * voiceFile = new VoiceFile();
    if( ParserTools::WildCardCompare( "*.wav", chan.GetFilePath().GetFileName() ) )
    {
      if( voiceFile->Open( chan.GetFilePath(), VoiceFile::Wav_8khz_Mono, chan.GetFilePath() ) )
        m_PlayQueue.Enqueue( voiceFile );
      else
        delete  voiceFile;
    }else if( ParserTools::WildCardCompare( "*.g7231", chan.GetFilePath().GetFileName() ) )
    {
      if( voiceFile->Open( chan.GetFilePath(), VoiceFile::Raw_G723, chan.GetFilePath() ) )
        m_PlayQueue.Enqueue( voiceFile );
      else
        delete voiceFile;
    }else if( ParserTools::WildCardCompare( "*.g729", chan.GetFilePath().GetFileName() ) )
    {
      if( voiceFile->Open( chan.GetFilePath(), VoiceFile::Raw_G729, chan.GetFilePath() ) )
        m_PlayQueue.Enqueue( voiceFile );

      else
        delete voiceFile;
    }else
    {
      delete voiceFile;
    }
  }
}

void VoiceFileChannel::FlushQueue()
{
  PWaitAndSignal mutex( m_ChanMutex );

  VoiceFile * voiceFile;
  while ((voiceFile = m_PlayQueue.Dequeue()) != NULL)
    delete voiceFile;
}

void VoiceFileChannel::OnFinishedPlayFile( 
  PFile & chan  
)
{
}


static int GetG7231FrameLen(int val)
{
  static const int frameLen[] = { 24, 20, 4, 1 };
  return frameLen[val & 3];
}

BOOL VoiceFileChannel::ReadFrame( 
  PINDEX amount 
)
{
  BOOL result = FALSE;
  if( m_Format == VoiceFile::Raw_G723 )
  {
    result = PIndirectChannel::Read( m_FrameBuffer.GetPointer() + 1, m_FrameLen - 1 );
  }else{
    result = PIndirectChannel::Read( m_FrameBuffer.GetPointer(), m_FrameLen );
  }

  m_FrameOffs = 0;
  if( m_Format == VoiceFile::Wav_8khz_Mono )
    m_FrameLen  = amount;
  else if( m_Format == VoiceFile::Raw_G729 )
    m_FrameLen = G729_BYTES_PER_FRAME;
  else
    m_FrameLen = GetG7231FrameLen( m_FrameBuffer[0] );

  // if we did not read a full frame of audio, fill the end of the
  // frame with zeros.
  if( m_Format == VoiceFile::Wav_8khz_Mono )
  {
    PINDEX count = GetLastReadCount();
    if( count < m_FrameLen )
      memset( m_FrameBuffer.GetPointer() + count, 0, m_FrameLen-count );
  }

  return result;
}

void VoiceFileChannel::CreateSilenceFrame( 
  PINDEX amount 
)
{
  if( m_Format == VoiceFile::Wav_8khz_Mono )
  {
    m_FrameOffs = 0;
    m_FrameLen  = amount;
    memset( m_FrameBuffer.GetPointer(), 0, m_FrameLen );
  }else if( m_Format == VoiceFile::Raw_G729 )
  {
    memset( m_FrameBuffer.GetPointer(), 0, G729_BYTES_PER_FRAME);
  }else if( m_Format == VoiceFile::Raw_G723 )
  {
    m_FrameOffs = 0;
    m_FrameLen  = 4;
    m_FrameBuffer[0] = 2;
    memset( m_FrameBuffer.GetPointer()+1, 0, 3);
  }
}

void VoiceFileChannel::Synchronize( 
  PINDEX amount 
)
{
  if( m_Format == VoiceFile::Raw_G729 )
    m_Delay.Delay(G729_ALGORITHMIC_DELAY);
  else if( m_Format == VoiceFile::Raw_G723 )
    m_Delay.Delay(G723_ALGORITHMIC_DELAY);
  else if( m_Format == VoiceFile::Wav_8khz_Mono )
    m_Delay.Delay(amount / 16);
}



BOOL VoiceFileChannel::AdjustFrame( 
  void * buffer, PINDEX amount 
)
{
  if( m_Format == VoiceFile::Wav_8khz_Mono && ( m_FrameOffs + amount) > m_FrameLen )
    return TRUE;

  memcpy(buffer, m_FrameBuffer.GetPointer() + m_FrameOffs, amount);
  
  if( m_Format == VoiceFile::Wav_8khz_Mono )
    m_FrameOffs += amount;
  else
    m_FrameOffs = amount;
  
  lastReadCount = amount;
  
  if( m_Format == VoiceFile::Wav_8khz_Mono )
    return m_FrameOffs == m_FrameLen;

  return TRUE;
}


