/*
 * silencedetect.cxx
 *
 * Open Phone Abstraction Library (OPAL)
 * Formally known as the Open H323 project.
 *
 * Copyright (c) 2001 Post Increment
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
 * $Log: silencedetect.cxx,v $
 * Revision 1.2  2006/08/07 07:13:44  joegenbaclor
 * Added XMPP/Jabber support via Google's libJingle
 *
 * Revision 1.1  2006/06/26 03:02:35  joegenbaclor
 * I have decided to include the latest development realease  of OPAL tagged Deimos Devel 1 (June 8 2006) as inegrated classes to opensipstack to avoid future version conflicts due to the fast pace in OPAL development.   This move is also aimed to reduce the size of projects using OPAL componets such as the soon to be relased OpenSIPPhone.
 *
 * Revision 1.4  2005/07/09 06:52:39  rjongbloed
 * Added print (operator<<) of silence detect mode enum.
 *
 * Revision 1.3  2005/03/06 15:00:26  dsandras
 * Fixed silence detection, we are now working with time frames, not the number of frames.
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

#include <ptlib.h>

#ifdef __GNUC__
#pragma implementation "silencedetect.h"
#endif

#include <codec/silencedetect.h>
#include <opal/patch.h>

#define new PNEW


extern "C" {
  unsigned char linear2ulaw(int pcm_val);
  int ulaw2linear(unsigned char u_val);
};


ostream & operator<<(ostream & strm, OpalSilenceDetector::Mode mode)
{
  static const char * const names[OpalSilenceDetector::NumModes] = {
      "NoSilenceDetection",
      "FixedSilenceDetection",
      "AdaptiveSilenceDetection"
  };

  if (mode >= 0 && mode < OpalSilenceDetector::NumModes && names[mode] != NULL)
    strm << names[mode];
  else
    strm << "OpalSilenceDetector::Modes<" << mode << '>';
  return strm;
}


///////////////////////////////////////////////////////////////////////////////

OpalSilenceDetector::OpalSilenceDetector()
#ifdef _MSC_VER
#pragma warning(disable:4355)
#endif
  : receiveHandler(PCREATE_NOTIFIER(ReceivedPacket))
#ifdef _MSC_VER
#pragma warning(default:4355)
#endif
{
  // Start off in silent mode
  inTalkBurst = FALSE;

  // Initialise the adaptive threshold variables.
  SetParameters(param);

  PTRACE(3, "Silence\tHandler created");
}


void OpalSilenceDetector::SetParameters(const Params & newParam)
{
  param = newParam;

  if (param.m_mode != AdaptiveSilenceDetection) {
    levelThreshold = param.m_threshold;
    return;
  }

  // Initials threshold levels
  levelThreshold = 0;

  // Initialise the adaptive threshold variables.
  signalMinimum = UINT_MAX;
  silenceMaximum = 0;
  signalReceivedTime = 0;
  silenceReceivedTime = 0;

  // Restart in silent mode
  inTalkBurst = FALSE;
  lastTimestamp = 0;
  receivedTime = 0;
}


OpalSilenceDetector::Mode OpalSilenceDetector::GetStatus(BOOL * isInTalkBurst,
                                                       unsigned * currentThreshold) const
{
  if (isInTalkBurst != NULL)
    *isInTalkBurst = inTalkBurst;

  if (currentThreshold != NULL)
    *currentThreshold = ulaw2linear((BYTE)(levelThreshold ^ 0xff));

  return param.m_mode;
}


void OpalSilenceDetector::ReceivedPacket(RTP_DataFrame & frame, INT)
{
  // Already silent
  if (frame.GetPayloadSize() == 0)
    return;

  // Can never have silence if NoSilenceDetection
  if (param.m_mode == NoSilenceDetection)
    return;

  unsigned thisTimestamp = frame.GetTimestamp();
  if (lastTimestamp == 0) {
    lastTimestamp = thisTimestamp;
    return;
  }

  unsigned timeSinceLastFrame = thisTimestamp - lastTimestamp;
  lastTimestamp = thisTimestamp;

  // Can never have average signal level that high, this indicates that the
  // hardware cannot do silence detection.
  unsigned level = GetAverageSignalLevel(frame.GetPayloadPtr(), frame.GetPayloadSize());
  if (level == UINT_MAX)
    return;

  // Convert to a logarithmic scale - use uLaw which is complemented
  level = linear2ulaw(level) ^ 0xff;

  // Now if signal level above threshold we are "talking"
  BOOL haveSignal = level > levelThreshold;

  // If no change ie still talking or still silent, resent frame counter
  if (inTalkBurst == haveSignal)
    receivedTime = 0;
  else {
    receivedTime += timeSinceLastFrame;
    // If have had enough consecutive frames talking/silent, swap modes.
    if (receivedTime >= (inTalkBurst ? param.m_silenceDeadband : param.m_signalDeadband)) {
      inTalkBurst = !inTalkBurst;
      PTRACE(4, "Silence\tDetector transition: "
             << (inTalkBurst ? "Talk" : "Silent")
             << " level=" << level << " threshold=" << levelThreshold);

      // If we had silence transition restart adaptive threshold measurements
      signalMinimum = UINT_MAX;
      silenceMaximum = 0;
      signalReceivedTime = 0;
      silenceReceivedTime = 0;

      // If we just have moved to sending a talk burst, set the RTP marker
      if (inTalkBurst)
        frame.SetMarker(TRUE);
    }
  }

  if (param.m_mode == FixedSilenceDetection) {
    if (!inTalkBurst)
      frame.SetPayloadSize(0); // Not in talk burst so silence the frame
    return;
  }

  if (levelThreshold == 0) {
    if (level > 1) {
      // Bootstrap condition, use first frame level as silence level
      levelThreshold = level/2;
      PTRACE(4, "Silence\tThreshold initialised to: " << levelThreshold);
    }
    // inTalkBurst always FALSE here, so return silent
    frame.SetPayloadSize(0);
    return;
  }

  // Count the number of silent and signal frames and calculate min/max
  if (haveSignal) {
    if (level < signalMinimum)
      signalMinimum = level;
    signalReceivedTime=signalReceivedTime+timeSinceLastFrame;
  }
  else {
    if (level > silenceMaximum)
      silenceMaximum = level;
    silenceReceivedTime=silenceReceivedTime+timeSinceLastFrame;
  }

  // See if we have had enough frames to look at proportions of silence/signal
  if ((signalReceivedTime + silenceReceivedTime) > param.m_adaptivePeriod) {

    /* Now we have had a period of time to look at some average values we can
       make some adjustments to the threshold. There are four cases:
     */
    if (signalReceivedTime >= param.m_adaptivePeriod) {
      /* If every frame was noisy, move threshold up. Don't want to move too
         fast so only go a quarter of the way to minimum signal value over the
         period. This avoids oscillations, and time will continue to make the
         level go up if there really is a lot of background noise.
       */
      int delta = (signalMinimum - levelThreshold)/4;
      if (delta != 0) {
        levelThreshold += delta;
        PTRACE(4, "Silence\tThreshold increased to: " << levelThreshold);
      }
    }
    else if (silenceReceivedTime >= param.m_adaptivePeriod) {
      /* If every frame was silent, move threshold down. Again do not want to
         move too quickly, but we do want it to move faster down than up, so
         move to halfway to maximum value of the quiet period. As a rule the
         lower the threshold the better as it would improve response time to
         the start of a talk burst.
       */
      unsigned newThreshold = (levelThreshold + silenceMaximum)/2 + 1;
      if (levelThreshold != newThreshold) {
        levelThreshold = newThreshold;
        PTRACE(4, "Silence\tThreshold decreased to: " << levelThreshold);
      }
    }
    else if (signalReceivedTime > silenceReceivedTime) {
      /* We haven't got a definitive silent or signal period, but if we are
         constantly hovering at the threshold and have more signal than
         silence we should creep up a bit.
       */
      levelThreshold++;
      PTRACE(4, "Silence\tThreshold incremented to: " << levelThreshold
             << " signal=" << signalReceivedTime << ' ' << signalMinimum
             << " silence=" << silenceReceivedTime << ' ' << silenceMaximum);
    }

    signalMinimum = UINT_MAX;
    silenceMaximum = 0;
    signalReceivedTime = 0;
    silenceReceivedTime = 0;
  }

  if (!inTalkBurst)
    frame.SetPayloadSize(0); // Not in talk burst so silence the frame
}


/////////////////////////////////////////////////////////////////////////////

unsigned OpalPCM16SilenceDetector::GetAverageSignalLevel(const BYTE * buffer, PINDEX size)
{
  // Calculate the average signal level of this frame
  int sum = 0;
  PINDEX samples = size/2;
  const short * pcm = (const short *)buffer;
  const short * end = pcm + samples;
  while (pcm != end) {
    if (*pcm < 0)
      sum -= *pcm++;
    else
      sum += *pcm++;
  }

  return sum/samples;
}


/////////////////////////////////////////////////////////////////////////////
