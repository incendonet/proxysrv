/*
 *
 * VoiceFileChannel.h
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
 * $Log: VoiceFileChannel.h,v $
 * Revision 1.8  2006/12/18 10:31:35  joegenbaclor
 * Fixed ACM G.723.1 one way audio bug
 *
 * Revision 1.7  2006/12/14 08:28:46  joegenbaclor
 * Added VoiceFile Destructor.
 *
 * Revision 1.6  2006/12/13 10:01:22  joegenbaclor
 * More work on Softphone
 *
 * Revision 1.5  2006/12/12 23:53:32  joegenbaclor
 * More work on DTMF encoding
 *
 * Revision 1.4  2006/11/30 08:19:37  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.3  2006/08/15 15:46:43  joegenbaclor
 * Added ability to stop synchronous playback of voice channels
 *
 * Revision 1.2  2006/08/01 03:55:41  joegenbaclor
 * 1.  Completed Initial classes for IVR Prompt channel
 * 2.  Added support for Bamboo Dynamic Hash Table for Network Overlay support
 *
 * Revision 1.1  2006/07/26 00:44:52  joegenbaclor
 * Added Voice File classes for upcoming IVR support
 *
 *
 */

#ifndef VOICEFILECHANNEL_H
#define VOICEFILECHANNEL_H


#include <ptlib.h>
#include <ptlib/sound.h>
#include <ptclib/memfile.h>
#include <ptclib/pwavfile.h>
#include <ptclib/delaychan.h>
#include <ptclib/url.h>
#include <ptclib/http.h>
#include <ptclib/delaychan.h>


namespace MS
{
  //////////////////////////////////////////////////////
  /// Implements a voice file cached in memory and masquerades
  /// as a normal file with Read and Write operations.
  /// Only three formats are supported by this clas, wav 8Khz Mono, G.7231 
  /// and G.729 raw or preencoded file.
  class VoiceFile : public PMemoryFile
  {
    PCLASSINFO( VoiceFile, PMemoryFile );
  public:

    enum Format
    {
      Wav_8khz_Mono,
      Raw_G723,
      Raw_G729,
      NumFormat
    };

    PQUEUE( Queue, VoiceFile );

    /// Default constructor
    VoiceFile();

    /// Constructs a voice file from a chunk of memery
    VoiceFile(
      const PBYTEArray & data,
      VoiceFile::Format format,
      const PString & fileId
    );

    ///construct a voice file from another instance of a voicefile
    VoiceFile(
      const VoiceFile & voiceFile
    );

    virtual ~VoiceFile();

    ///construct a voice file from an HTTP URL
    BOOL Open(
      const PURL & path,
      VoiceFile::Format format,
      const PString & fileId
    );

    /// Constructs a voice file from local storage
    BOOL Open(
      const PFilePath & path,
      VoiceFile::Format format,
      const PString & fileId
    );

    /// Constructs a voice file from a chunk of memory
    BOOL Open(
      const PBYTEArray & data,
      VoiceFile::Format format,
      const PString & fileId
    );

    /// Saves the content of voice file to local storage
    /// in accordance to the Format type.  Example.  Wav_8khz_Mono format
    /// would be saved using the .wav extension and Raw_G723 would be saved
    /// with .g723 extension.  The id of the file will be used
    /// as the filename if the filename is not specified.
    /// Returns TRUE if saving is sucessful.
    BOOL SaveToFile(
      const PDirectory & path = PString::Empty(),
      const PString & fileName = PString::Empty()
    );

    /// Plays the content of this file using the default output device
    BOOL PlaySound();

    /// Plays the content of the voicefile using the specified sound channel
    BOOL PlaySound(
      PSoundChannel & soundChannel
    );

    PINLINE VoiceFile::Format GetFormat()const{ return m_Format; };
    PINLINE const PString & GetFileId()const{ return m_FileId; };
    PINLINE void StopPlaying(){ m_IsPlaying = FALSE; };
  private:

    Format m_Format;
    PString m_FileId;
    BOOL m_IsPlaying;
    PAdaptiveDelay m_Delay;
  };


  extern VoiceFile DTMFTone_0;
  extern VoiceFile DTMFTone_1;
  extern VoiceFile DTMFTone_2;
  extern VoiceFile DTMFTone_3;
  extern VoiceFile DTMFTone_4;
  extern VoiceFile DTMFTone_5;
  extern VoiceFile DTMFTone_6;
  extern VoiceFile DTMFTone_7;
  extern VoiceFile DTMFTone_8;
  extern VoiceFile DTMFTone_9;
  extern VoiceFile DTMFTone_10;
  extern VoiceFile DTMFTone_11;

  class  VoiceFileChannel : public PIndirectChannel
  {
    PCLASSINFO(VoiceFileChannel, PIndirectChannel);

  public:
    VoiceFileChannel( 
      VoiceFile::Format
    );

    BOOL Read( 
      void * buffer, PINDEX amount 
    );

    void PlayFile( 
      PFile * chan 
    );

    BOOL Close();

    void QueueFile(
      const PFilePath & loc
    );

    void QueueFile(
      const PURL & loc
    );

    void QueueFile( 
      const PFile & chan 
    );

    void FlushQueue();

    void OnFinishedPlayFile( 
      PFile & chan  
    );

  protected:

    virtual BOOL ReadFrame( 
      PINDEX amount 
    );

    virtual void CreateSilenceFrame( 
      PINDEX amount 
    );

    virtual void Synchronize( 
      PINDEX amount 
    );

    virtual BOOL AdjustFrame( 
      void * buffer, PINDEX amount 
    );

    VoiceFile::Queue m_PlayQueue;
    PMutex m_ChanMutex;
    int m_SilenceCount;
    int m_TotalData;
    BOOL m_Closed;
    PAdaptiveDelay m_Delay;
    PBYTEArray m_FrameBuffer;
    PINDEX m_FrameLen, m_FrameOffs;
    int m_BytesPlayed;
    PFile * m_CurrentPlayFile;
    VoiceFile::Format m_Format;
  };

    /*
    class VoiceFileChannelManager : public PObject
    {
    };

    //////////////////////////////////////////////////////
    
    */


}



#endif // VOICEFILECHANNEL_H



