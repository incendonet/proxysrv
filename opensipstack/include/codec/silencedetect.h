/*
 * silencedetect.h
 *
 * Open Phone Abstraction Library (OPAL)
 * Formally known as the Open H323 project.
 *
 * Copyright (c) 2004 Post Increment
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
 * $Log: silencedetect.h,v $
 * Revision 1.1  2006/08/04 03:33:18  joegenbaclor
 * Moved pwlib and OPAL headers to root include directory
 *
 * Revision 1.1  2006/06/26 03:02:27  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 1.4  2005/11/30 13:35:26  csoutheren
 * Changed tags for Doxygen
 *
 * Revision 1.3  2005/07/09 06:52:38  rjongbloed
 * Added print (operator<<) of silence detect mode enum.
 *
 * Revision 1.2  2004/05/24 13:39:26  rjongbloed
 * Fixed setting marker bit when silence suppression transitions from
 *   silence to signal, thanks Ted Szoczei
 * Added a separate structure for the silence suppression paramters to make
 *   it easier to set from global defaults in the manager class.
 *
 * Revision 1.1  2004/05/17 13:24:26  rjongbloed
 * Added silence suppression.
 *
 */

#ifndef __OPAL_SILENCEDETECT_H
#define __OPAL_SILENCEDETECT_H

#ifdef P_USE_PRAGMA
#pragma interface
#endif


#include <rtp/rtp.h>


///////////////////////////////////////////////////////////////////////////////

class OpalSilenceDetector : public PObject
{
    PCLASSINFO(OpalSilenceDetector, PObject);
  public:
    enum Mode {
      NoSilenceDetection,
      FixedSilenceDetection,
      AdaptiveSilenceDetection,
      NumModes
    };

    struct Params {
      Params(
        Mode mode = AdaptiveSilenceDetection, ///<  New silence detection mode
        unsigned threshold = 0,               ///<  Threshold value if FixedSilenceDetection
        unsigned signalDeadband = 80,         ///<  10 milliseconds of signal needed
        unsigned silenceDeadband = 3200,      ///<  400 milliseconds of silence needed
        unsigned adaptivePeriod = 4800        ///<  600 millisecond window for adaptive threshold
      )
        : m_mode(mode),
          m_threshold(threshold),
          m_signalDeadband(signalDeadband),
          m_silenceDeadband(silenceDeadband),
          m_adaptivePeriod(adaptivePeriod)
        { }

      Mode     m_mode;             /// Silence detection mode
      unsigned m_threshold;        /// Threshold value if FixedSilenceDetection
      unsigned m_signalDeadband;   /// 10 milliseconds of signal needed
      unsigned m_silenceDeadband;  /// 400 milliseconds of silence needed
      unsigned m_adaptivePeriod;   /// 600 millisecond window for adaptive threshold
    };

  /**@name Construction */
  //@{
    /**Create a new connection.
     */
    OpalSilenceDetector();
  //@}

  /**@name Basic operations */
  //@{
    const PNotifier & GetReceiveHandler() const { return receiveHandler; }

    /**Set the silence detector parameters.
       This enables, disables the silence detector as well as adjusting its
       "agression". The deadband periods are in audio samples of 8kHz.
      */
    void SetParameters(
      const Params & newParam ///<  New parameters for silence detector
    );

    /**Get silence detection status

       The inTalkBurst value is TRUE if packet transmission is enabled and
       FALSE if it is being suppressed due to silence.

       The currentThreshold value is the value from 0 to 32767 which is used
       as the threshold value for 16 bit PCM data.
      */
    Mode GetStatus(
      BOOL * isInTalkBurst,
      unsigned * currentThreshold
    ) const;

    /**Get the average signal level in the stream.
       This is called from within the silence detection algorithm to
       calculate the average signal level of the last data frame read from
       the stream.

       The default behaviour returns UINT_MAX which indicates that the
       average signal has no meaning for the stream.
      */
    virtual unsigned GetAverageSignalLevel(
      const BYTE * buffer,  ///<  RTP payload being detected
      PINDEX size           ///<  Size of payload buffer
    ) = 0;

  protected:
    PDECLARE_NOTIFIER(RTP_DataFrame, OpalSilenceDetector, ReceivedPacket);

    PNotifier receiveHandler;

    Params param;

    BOOL     inTalkBurst;           // Currently sending RTP data
    unsigned lastTimestamp;         // Last timestamp received
    unsigned receivedTime;          // Signal/Silence duration received so far.
    unsigned levelThreshold;        // Threshold level for silence/signal
    unsigned signalMinimum;         // Minimum of frames above threshold
    unsigned silenceMaximum;        // Maximum of frames below threshold
    unsigned signalReceivedTime;    // Duration of signal received
    unsigned silenceReceivedTime;   // Duration of silence received
};


class OpalPCM16SilenceDetector : public OpalSilenceDetector
{
    PCLASSINFO(OpalPCM16SilenceDetector, OpalSilenceDetector);
  public:
  /**@name Overrides from OpalSilenceDetector */
  //@{
    /**Get the average signal level in the stream.
       This is called from within the silence detection algorithm to
       calculate the average signal level of the last data frame read from
       the stream.

       The default behaviour returns UINT_MAX which indicates that the
       average signal has no meaning for the stream.
      */
    virtual unsigned GetAverageSignalLevel(
      const BYTE * buffer,  ///<  RTP payload being detected
      PINDEX size           ///<  Size of payload buffer
    );
  //@}
};


extern ostream & operator<<(ostream & strm, OpalSilenceDetector::Mode mode);


#endif // __OPAL_SILENCEDETECT_H


/////////////////////////////////////////////////////////////////////////////
