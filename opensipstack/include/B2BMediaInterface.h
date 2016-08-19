/*
 *
 * B2BMediaInterface.h
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
 * $Log: B2BMediaInterface.h,v $
 * Revision 1.13  2006/11/30 08:19:35  joegenbaclor
 * Relicensed opensipstack to MPL/GPL/LGPL triple license scheme
 *
 * Revision 1.12  2006/11/08 06:01:03  joegenbaclor
 * added class keyword in GCObject friend declaration
 *
 * Revision 1.11  2006/10/30 03:42:42  joegenbaclor
 * Protected the destructor from getting deleted outside of the garbage collector
 *
 * Revision 1.10  2006/09/26 03:26:04  rcolobong
 * RTP will be automatically encrypted if SIP Messages is encrypted
 *
 * Revision 1.9  2006/09/21 08:12:21  joegenbaclor
 * We now set socket information for inbound calls if media address is public
 *
 * Revision 1.8  2006/09/18 09:35:15  joegenbaclor
 * Completed initial implementation of RTP media aggregation
 *
 * Revision 1.7  2006/09/18 03:28:54  joegenbaclor
 * 1. More work on media aggregation
 * 2. Check sock->GetPort() for NULL in SIPTransportManager::GetListenerAddress() (thanks to
 *  Vamsi Pottangi for reporting the bug)
 * 3.  Added reference size check to GCObject::ReleaseRef()
 *
 * Revision 1.6  2006/09/15 15:31:39  joegenbaclor
 * Changed Log levels to decrease log size.
 * Changed default session keep alaive interval from 10 seconds to two minutes
 *
 * Revision 1.5  2006/09/15 09:55:51  joegenbaclor
 * Uploaded initial classes for ThreadPool implementation.
 *
 * Revision 1.4  2006/06/28 14:14:55  joegenbaclor
 * Bulk commit for Media Proxy functionalities for OpenB2BUA
 * Marks a minor version upgrade from 1.0.2 to 1.0.3
 *
 * Revision 1.3  2006/06/21 14:29:11  joegenbaclor
 * This upload marks the first successful call by OpenB2BUA
 *
 * Revision 1.2  2006/06/20 09:52:55  joegenbaclor
 * Added Routing and SDP Handlers to B2BUA
 *
 * Revision 1.1  2006/06/15 09:47:28  joegenbaclor
 * More preliminary design work on B2BUA
 *
 *
 */

#ifndef B2BMEDIAINTERFACE_H
#define B2BMEDIAINTERFACE_H

#include "B2BInterface.h"
#include "ThreadPool.h"

#if HAS_OPAL
#include <rtp/rtp.h>
#include <opal/manager.h>
#include <opal/transports.h>
#include <opal/mediafmt.h>
#endif

namespace B2BUA
{
  class B2BMediaInterface : public B2BInterface
  {
    PCLASSINFO( B2BMediaInterface, B2BInterface );
  public:
    B2BMediaInterface(  
      B2BUserAgent & b2bua 
    );

    virtual ~B2BMediaInterface();

    virtual BOOL OnIncomingSDPOffer(
      B2BUAConnection & connection,
      B2BUACall & call,
      const SIPMessage & sdp
    );

    virtual BOOL OnRequireSDPOffer(
      B2BUAConnection & connection,
      B2BUACall & call,
      SIPMessage & offer
    );

    virtual BOOL OnRequireSDPAnswer(
      B2BUAConnection & connection,
      B2BUACall & call,
      const SIPMessage & offer,
      SIPMessage & answer
    );

#if HAS_OPAL
  protected:
    OpalManager m_OpalManager;

    BOOL CreateRTPSession(
      B2BUAConnection & connection,
      B2BUACall & call,
      const SIPMessage & sdp,
      BOOL createOnlyIfPrivate = TRUE
    );

    BOOL CreateRTPSession(
      unsigned sessionId,
      const PIPSocket::Address & localAddress,
      const PIPSocket::Address & targetAddress,
      WORD targetPort,
      const PString & sdp,
      RTP_SessionManager & manager,
      BOOL isXORHashed = FALSE
    );

    BOOL TranslateSDPOffer(
      B2BUAConnection & connection,
      B2BUACall & call,
      SIPMessage & offer
    );

    BOOL TranslateSDPAnswer(
      B2BUAConnection & connection,
      B2BUACall & call,
      const SIPMessage & offer,
      SIPMessage & answer
    );

    BOOL OnOpenMediaStreams(
      B2BUAConnection & connection
    );

  public:

    class OpalMediaThread : public PThread
    {
      PCLASSINFO( OpalMediaThread, PThread );
    public:
      OpalMediaThread(
        RTP_UDP * leg1,
        RTP_UDP * leg2
      );
      virtual void Main();

      RTP_UDP * m_Leg1;
      RTP_UDP * m_Leg2;
    };

    class AggregatedMedia : public PObject
    {
      GCREFINFO( AggregatedMedia, "AggregatedMedia" )
      PCLASSINFO( AggregatedMedia, PObject );
    public:
      AggregatedMedia(
        B2BMediaInterface & mediaInterface,
        RTP_UDP * leg1,
        RTP_UDP * leg2
      );

    /// protect the destructor from 
    /// getting deleted outside of the garbage collector
    protected:
      ~AggregatedMedia();
      friend class GCObject<AggregatedMedia>;
    public:

      BOOL ProcessRead();

      void Close();

      BOOL WaitForTermination( 
        const PTimeInterval & wait = INT_MAX 
      );

      PINLINE int GetDataSocketHandle()const{ return m_DataSocketHandle; };
      PINLINE int GetControlSocketHandle()const{ return m_ControlSocketHandle; };
      PINLINE PUDPSocket & GetDataSocket(){ return (m_Leg1->GetDataSocket()); };
      PINLINE PUDPSocket & GetControlSocket(){ return (m_Leg1->GetControlSocket()); };
      B2BMediaInterface & GetMediaInterface(){ return m_MediaInterface; };
    protected:

      B2BMediaInterface & m_MediaInterface;
      PMutex m_ReadMutex;
      RTP_UDP * m_Leg1;
      RTP_UDP * m_Leg2;
      int m_DataSocketHandle;
      int m_ControlSocketHandle;
      PSemaphore m_ExitSync;
      BOOL m_ExitFlag;
    };

    class MediaAggregationThread : public PThread
    {
      PCLASSINFO( MediaAggregationThread, PThread );
    public:
      MediaAggregationThread( B2BMediaInterface & mediaInterface )
        : PThread( 1024, PThread::NoAutoDeleteThread ), 
          m_MediaInterface( mediaInterface )
      {
        m_ExitFlag = FALSE;
        Resume(); 
      }

      virtual ~MediaAggregationThread()
      {
        m_ExitFlag = TRUE;
        WaitForTermination();
      }

      void Main()
      {
        for(;;)
        {
          m_MediaInterface.ProcessMediaAggregation();

          if( m_ExitFlag )
            break;
        }
      }
    
      B2BMediaInterface & m_MediaInterface;
      BOOL m_ExitFlag;
    };

  
    virtual BOOL OnClosedAggregatedMedia(
      AggregatedMedia::GCRef & gcRef 
    );

    BOOL CreateMediaAggregation(
      RTP_UDP * leg1,
      RTP_UDP * leg2
    );

    BOOL ProcessMediaAggregation();

    PDECLARE_NOTIFIER( PThread, B2BMediaInterface, OnProcessAggregation );

    PINLINE void UseAggregatedMedia( BOOL aggregate ){ m_UseAggregatedMedia = aggregate; };
    PINLINE BOOL IsAggregatedMedia()const{ return m_UseAggregatedMedia; };
    PINLINE PMutex & GetMediaAggregationMutex(){ return m_MediaAggregationMutex; };
  private:
    PMutex m_MediaStreamsMutex;
    typedef  B2BMediaInterface::AggregatedMedia MediaAggregator;
    PDICTIONARY( AggregatedMediaList, POrdinalKey, MediaAggregator );
    BOOL m_UseAggregatedMedia;
    PMutex m_MediaAggregationMutex;
    AggregatedMediaList m_MediaAggregationList;

    PDICTIONARY( RTPSocketList, POrdinalKey, PUDPSocket );
    PLIST( RTPSelectList, PUDPSocket );
    PMutex m_RTPSocketListMutex;
    RTPSocketList m_RTPSocketList;
    RTPSelectList m_RTPSelectList;

    MediaAggregationThread * m_MediaAggregationThread;
    Tools::ThreadPool * m_MediaAggragationPool;

    PNotifier m_AggregationCallBack;
    PSyncPoint m_MediaAggregationSync;
    
#endif // HAS_OPAL
  };
}

#endif

