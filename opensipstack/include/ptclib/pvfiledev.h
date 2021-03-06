/*
 * pvfiledev.cxx
 *
 * Video file declaration
 *
 * Portable Windows Library
 *
 * Copyright (C) 2004 Post Increment
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
 * The Initial Developer of the Original Code is
 * Craig Southeren <craigs@postincrement.com>
 *
 * All Rights Reserved.
 *
 * Contributor(s): ______________________________________.
 *
 * $Log: pvfiledev.h,v $
 * Revision 1.1  2006/08/04 03:33:23  joegenbaclor
 * Moved pwlib and OPAL headers to root include directory
 *
 * Revision 1.1  2006/06/29 04:17:14  joegenbaclor
 * *** empty log message ***
 *
 * Revision 1.6  2006/04/19 04:09:04  csoutheren
 * Allow frame size conversions
 *
 * Revision 1.5  2006/03/17 06:54:49  csoutheren
 * Removed unused member variable
 *
 * Revision 1.4  2006/03/13 07:49:09  csoutheren
 * Added newline at end of file
 *
 * Revision 1.3  2006/03/06 06:04:13  csoutheren
 * Added YUVFile video output device
 *
 * Revision 1.2  2006/02/20 06:49:45  csoutheren
 * Added video file and video file input device code
 *
 * Revision 1.1  2006/02/20 06:17:28  csoutheren
 * Added ability to read video from a file
 *
 */

#ifndef _PVFILEDEV
#define _PVFILEDEV

#ifdef P_USE_PRAGMA
#pragma interface
#endif

#include <ptlib.h>

#if P_VIDFILE


#include <ptlib.h>

#include <ptlib/vconvert.h>
#include <ptclib/pvidfile.h>

///////////////////////////////////////////////////////////////////////////////////////////
//
// This class defines a video capture (input) device that reads video from a raw YUV file
//

class PVideoInputDevice_YUVFile : public PVideoInputDevice
{
 PCLASSINFO(PVideoInputDevice_YUVFile, PVideoInputDevice);
 public:
   enum {
     Channel_PlayAndClose     = 0,
     Channel_PlayAndRepeat    = 1,
     Channel_PlayAndKeepLast  = 2,
     Channel_PlayAndShowBlack = 3,
     ChannelCount             = 4
   };

  /** Create a new (fake) video input device.
   */
    PVideoInputDevice_YUVFile();


    /**Open the device given the device name.
      */
    BOOL Open(
      const PString & deviceName,   /// Device name to open
      BOOL startImmediate = TRUE    /// Immediately start device
    );

    /**Determine of the device is currently open.
      */
    BOOL IsOpen() ;

    /**Close the device.
      */
    BOOL Close();

    /**Start the video device I/O.
      */
    BOOL Start();

    /**Stop the video device I/O capture.
      */
    BOOL Stop();

    /**Determine if the video device I/O capture is in progress.
      */
    BOOL IsCapturing();

    /**Get a list of all of the drivers available.
      */
    static PStringList GetInputDeviceNames();

    virtual PStringList GetDeviceNames() const
      { return GetInputDeviceNames(); }

    /**Get the maximum frame size in bytes.

       Note a particular device may be able to provide variable length
       frames (eg motion JPEG) so will be the maximum size of all frames.
      */
    virtual PINDEX GetMaxFrameBytes();

    /**Grab a frame. 

       There will be a delay in returning, as specified by frame rate.
      */
    virtual BOOL GetFrameData(
      BYTE * buffer,                 /// Buffer to receive frame
      PINDEX * bytesReturned = NULL  /// Optional bytes returned.
    );

    /**Grab a frame.

       Do not delay according to the current frame rate.
      */
    virtual BOOL GetFrameDataNoDelay(
      BYTE * buffer,                 /// Buffer to receive frame
      PINDEX * bytesReturned = NULL  /// OPtional bytes returned.
    );


    /**Generate a static image, containing a constant field of grey.
     */
    void GrabBlankImage(BYTE *resFrame);

    /**Set the video format to be used.

       Default behaviour sets the value of the videoFormat variable and then
       returns the IsOpen() status.
    */
    virtual BOOL SetVideoFormat(
      VideoFormat videoFormat   /// New video format
    );

    /**Get the number of video channels available on the device.

       Default behaviour returns 1.
    */
    virtual int GetNumChannels() ;

    /**Set the video channel to be used on the device. Channels have the following meanings:
        0 (default) = play file and close device
        1           = play file and repeat
        2           = play file and replay last frame
        3           = play file and display black frame

       Default behaviour sets the value of the channelNumber variable and then
       returns the IsOpen() status.
    */
    virtual BOOL SetChannel(
         int channelNumber  /// New channel number for device.
    );
    
    /**Set the colour format to be used.

       Default behaviour sets the value of the colourFormat variable and then
       returns the IsOpen() status.
    */
    virtual BOOL SetColourFormat(
      const PString & colourFormat   // New colour format for device.
    );
    
    /**Set the video frame rate to be used on the device.

       Default behaviour sets the value of the frameRate variable and then
       return the IsOpen() status.
    */
    virtual BOOL SetFrameRate(
      unsigned rate  /// Frames per second
    );
         
    /**Get the minimum & maximum size of a frame on the device.

       Default behaviour returns the value 1 to UINT_MAX for both and returns
       FALSE.
    */
    virtual BOOL GetFrameSizeLimits(
      unsigned & minWidth,   /// Variable to receive minimum width
      unsigned & minHeight,  /// Variable to receive minimum height
      unsigned & maxWidth,   /// Variable to receive maximum width
      unsigned & maxHeight   /// Variable to receive maximum height
    ) ;

    /**Set the frame size to be used.

       Default behaviour sets the frameWidth and frameHeight variables and
       returns the IsOpen() status.
    */
    virtual BOOL SetFrameSize(
      unsigned width,   /// New width of frame
      unsigned height   /// New height of frame
    );

    BOOL SetFrameSizeConverter(
      unsigned width,        ///< New width of frame
      unsigned height,       ///< New height of frame
      BOOL     bScaleNotCrop ///< Scale or crop/pad preference
    );
         
    void ClearMapping() { return ; }

    /**Try all known video formats & see which ones are accepted by the video driver
     */
    virtual BOOL TestAllFormats()
      { return TRUE; }

    void FillRect(BYTE * frame,int xPos, int initialYPos,int rectWidth, int rectHeight,int r, int g,  int b);
   
 protected:
   unsigned grabCount;
   PINDEX   videoFrameSize;
   PINDEX   scanLineWidth;
   PYUVFile file;
   PBYTEArray frameStore;
};

PLOAD_FACTORY_DECLARE(PVideoInputDevice,YUVFile)

///////////////////////////////////////////////////////////////////////////////////////////
//
// This class defines a video display (output) device that writes video to a raw YUV file
//

class PVideoOutputDevice_YUVFile : public PVideoOutputDevice
{
  PCLASSINFO(PVideoOutputDevice_YUVFile, PVideoOutputDevice);

  public:
    /** Create a new video output device.
     */
    PVideoOutputDevice_YUVFile();

    /**Get a list of all of the drivers available.
      */
    static PStringList GetOutputDeviceNames();

    virtual PStringList GetDeviceNames() const
      { return GetOutputDeviceNames(); }

    /**Open the device given the device name.
      */
    virtual BOOL Open(
      const PString & deviceName,   /// Device name to open
      BOOL startImmediate = TRUE    /// Immediately start device
    );

    /**Start the video device I/O.
      */
    BOOL Start();

    /**Stop the video device I/O capture.
      */
    BOOL Stop();

    /**Close the device.
      */
    virtual BOOL Close();

    /**Determine if the device is currently open.
      */
    virtual BOOL IsOpen();

    /**Get the maximum frame size in bytes.

       Note a particular device may be able to provide variable length
       frames (eg motion JPEG) so will be the maximum size of all frames.
      */
    virtual PINDEX GetMaxFrameBytes();

    /**Set a section of the output frame buffer.
      */
    virtual BOOL SetFrameData(
      unsigned x,
      unsigned y,
      unsigned width,
      unsigned height,
      const BYTE * data,
      BOOL endFrame = TRUE
    );

    /**Indicate frame may be displayed.
      */
    virtual BOOL EndFrame();

  protected:  
    PYUVFile file;
};

PLOAD_FACTORY_DECLARE(PVideoOutputDevice,YUVFile)

#endif // P_VIDFILE

#endif // _PVFILEDEV

