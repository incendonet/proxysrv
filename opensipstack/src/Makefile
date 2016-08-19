PWLIBDIR	= /home/speechbridge/src/ProxySrv/SBMediaSrv/opensipstack
OPALDIR         = /home/speechbridge/src/ProxySrv/SBMediaSrv/opensipstack
SQLITEDIR       = 
CPPSQLITEDIR    = 
OPENSIPSTACKDIR	= /home/speechbridge/src/ProxySrv/SBMediaSrv/opensipstack

LIBRARY_MAKEFILE:=1

include $(OPENSIPSTACKDIR)/make/opensipstack.mak


OSS_OBJDIR = $(OSS_LIBDIR)/$(PT_OBJBASE)
ifdef NOTRACE
OSS_OBJDIR := $(OSS_OBJDIR)n
endif


OBJDIR	=	$(OSS_OBJDIR)
LIBDIR	=	$(OSS_LIBDIR)
TARGET	=	$(LIBDIR)/$(OSS_FILE)
VERSION_FILE =	$(OPENSIPSTACKDIR)/version.h


COMMON_SOURCES := $(OSS_SRCDIR)/A1Hash.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/A2Hash.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/B2BAuthInterface.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/B2BCallInterface.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/B2BMediaInterface.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/B2BRoutingInterface.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/B2BUA.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/B2BUACall.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/B2BUAConnection.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/B2BUAEndPoint.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/B2BUAForkCalls.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/BambooDHT.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/CSeq.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/CacheManager.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/CacheObject.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/CallSession.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/CallSessionManager.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/Contact.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/ContactURI.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/DialogId.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/ECCIAPI.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/EventQueue.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/GCObject.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/InviteClientTransaction.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/InviteServerTransaction.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/Logger.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/MD5Authorization.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/MD5Hash.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/MessageHeaders.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/MimeHeader.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/MimeParam.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/MWIPackage.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/MWIResource.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/Nonce.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/NoneInviteClientTransaction.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/NoneInviteServerTransaction.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/OpalOSSConnection.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/OpalOSSEndPoint.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/OSSAppConfig.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/OSSApplication.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/ParserTools.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/Profile.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/ProfileAuth.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/ProfileMedia.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/ProfileRegister.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/ProfileTransport.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/ProfileUA.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/ProxySession.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/ProxySessionManager.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RecordRouteBranch.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RedirectSession.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RedirectSessionManager.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RegisterSession.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RegisterSessionManager.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RequestLine.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RFC3265Client.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RFC3265ClientManager.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RFC3265EventPackage.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RFC3265Notification.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RFC3265Resource.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RFC3265StateAgent.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RFC3265Subscription.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RFC3265Client.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RFC3680Client.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RFC3680Package.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RFC3680Resource.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/RouteURI.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPAttribute.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPBandwidth.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPConnectionData.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPHeader.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPLazyParser.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPMediaAnnouncement.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPMediaDescription.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPMediaFormat.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPOffer.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPOrigin.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPPacket.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPProtocolVersion.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPSessionDescription.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPSessionName.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPTime.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SDPURI.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPEvent.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPFiniteStateMachine.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPHeader.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPHeaderB.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPHeaderC.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPHeaderD.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPHeaderE.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPHeaderF.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPMessage.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPNATMethods.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPSession.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPSessionManager.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPSrvRecord.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPStack.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPTimer.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPTimerInterface.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPTimerManager.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPTransaction.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPTransactionManager.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPTransactionStage.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPTransport.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPTransportManager.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPUDPSocketList.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPUDPTransport.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPURI.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SIPUserAgent.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SoftPhone.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SoftPhoneInterface.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/SQLiteDb.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/StartLine.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/StatefulEventQueue.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/StatusLine.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/StringParam.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/TelURI.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/ThreadPool.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/TransactionId.cxx
COMMON_SOURCES += $(OSS_SRCDIR)/Via.cxx

HEADER_FILES	:= $(OSS_INCDIR)/A1Hash.h
HEADER_FILES	+= $(OSS_INCDIR)/A2Hash.h
HEADER_FILES  += $(OSS_INCDIR)/B2BAuthInterface.h
HEADER_FILES  += $(OSS_INCDIR)/B2BCallInterface.h
HEADER_FILES  += $(OSS_INCDIR)/B2BMediaInterface.h
HEADER_FILES  += $(OSS_INCDIR)/B2BRoutingInterface.h
HEADER_FILES  += $(OSS_INCDIR)/B2BInterface.h
HEADER_FILES	+= $(OSS_INCDIR)/B2BUA.h
HEADER_FILES	+= $(OSS_INCDIR)/B2BUACall.h
HEADER_FILES	+= $(OSS_INCDIR)/B2BUAConnection.h
HEADER_FILES	+= $(OSS_INCDIR)/B2BUAEndPoint.h
HEADER_FILES	+= $(OSS_INCDIR)/B2BUAForkCalls.h
HEADER_FILES	+= $(OSS_INCDIR)/BambooDHT.h
HEADER_FILES	+= $(OSS_INCDIR)/CSeq.h
HEADER_FILES	+= $(OSS_INCDIR)/CacheManager.h
HEADER_FILES	+= $(OSS_INCDIR)/CacheObject.h
HEADER_FILES	+= $(OSS_INCDIR)/CallSession.h
HEADER_FILES	+= $(OSS_INCDIR)/CallSessionManager.h
HEADER_FILES	+= $(OSS_INCDIR)/Contact.h
HEADER_FILES	+= $(OSS_INCDIR)/ContactURI.h
HEADER_FILES	+= $(OSS_INCDIR)/DialogId.h
HEADER_FILES    += $(OSS_INCDIR)/ECCIAPI.h
HEADER_FILES	+= $(OSS_INCDIR)/EventQueue.h
HEADER_FILES	+= $(OSS_INCDIR)/GCObject.h
HEADER_FILES	+= $(OSS_INCDIR)/InviteClientTransaction.h
HEADER_FILES	+= $(OSS_INCDIR)/InviteServerTransaction.h
HEADER_FILES	+= $(OSS_INCDIR)/Logger.h
HEADER_FILES	+= $(OSS_INCDIR)/MD5Authorization.h
HEADER_FILES	+= $(OSS_INCDIR)/MD5Hash.h
HEADER_FILES	+= $(OSS_INCDIR)/MessageHeaders.h
HEADER_FILES	+= $(OSS_INCDIR)/MimeHeader.h
HEADER_FILES	+= $(OSS_INCDIR)/MimeParam.h
HEADER_FILES	+= $(OSS_INCDIR)/MWIPackage.h
HEADER_FILES	+= $(OSS_INCDIR)/MWIResource.h
HEADER_FILES	+= $(OSS_INCDIR)/Nonce.h
HEADER_FILES	+= $(OSS_INCDIR)/NoneInviteClientTransaction.h
HEADER_FILES	+= $(OSS_INCDIR)/NoneInviteServerTransaction.h
HEADER_FILES  += $(OSS_INCDIR)/OpalOSSConnection.h
HEADER_FILES  += $(OSS_INCDIR)/OpalOSSEndPoint.h
HEADER_FILES  += $(OSS_INCDIR)/OSSAppConfig.h
HEADER_FILES  += $(OSS_INCDIR)/OSSApplication.h
HEADER_FILES	+= $(OSS_INCDIR)/ParserTools.h
HEADER_FILES	+= $(OSS_INCDIR)/Profile.h
HEADER_FILES	+= $(OSS_INCDIR)/ProfileAuth.h
HEADER_FILES	+= $(OSS_INCDIR)/ProfileMedia.h
HEADER_FILES	+= $(OSS_INCDIR)/ProfileRegister.h
HEADER_FILES	+= $(OSS_INCDIR)/ProfileTransport.h
HEADER_FILES	+= $(OSS_INCDIR)/ProfileUA.h
HEADER_FILES	+= $(OSS_INCDIR)/ProxySession.h
HEADER_FILES	+= $(OSS_INCDIR)/ProxySessionManager.h
HEADER_FILES	+= $(OSS_INCDIR)/RecordRouteBranch.h
HEADER_FILES	+= $(OSS_INCDIR)/RedirectSession.h
HEADER_FILES	+= $(OSS_INCDIR)/RedirectSessionManager.h
HEADER_FILES	+= $(OSS_INCDIR)/RegisterSession.h
HEADER_FILES	+= $(OSS_INCDIR)/RegisterSessionManager.h
HEADER_FILES	+= $(OSS_INCDIR)/RequestLine.h
HEADER_FILES  += $(OSS_INCDIR)/RFC3265Client.h
HEADER_FILES  += $(OSS_INCDIR)/RFC3265ClientManager.h
HEADER_FILES  += $(OSS_INCDIR)/RFC3265EventPackage.h
HEADER_FILES  += $(OSS_INCDIR)/RFC3265Notification.h
HEADER_FILES  += $(OSS_INCDIR)/RFC3265Resource.h
HEADER_FILES  += $(OSS_INCDIR)/RFC3265StateAgent.h
HEADER_FILES  += $(OSS_INCDIR)/RFC3265Subscription.h
HEADER_FILES  += $(OSS_INCDIR)/RFC3680Client.h
HEADER_FILES  += $(OSS_INCDIR)/RFC3680Package.h
HEADER_FILES  += $(OSS_INCDIR)/RFC3680Resource.h
HEADER_FILES	+= $(OSS_INCDIR)/RouteURI.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPAttribute.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPBandwidth.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPConnectionData.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPHeader.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPLazyParser.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPMediaAnnouncement.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPMediaDescription.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPMediaFormat.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPOffer.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPOrigin.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPPacket.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPProtocolVersion.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPRTPMap.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPSessionDescription.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPSessionName.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPTime.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPTransactor.h
HEADER_FILES	+= $(OSS_INCDIR)/SDPURI.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPEvent.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPFiniteStateMachine.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPHeader.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPHeaderB.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPHeaderC.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPHeaderD.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPHeaderE.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPHeaderF.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPMessage.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPNATMethods.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPSession.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPSessionManager.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPSrvRecord.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPStack.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPStackEvent.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPTimer.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPTimerInterface.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPTimerManager.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPTransaction.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPTransactionManager.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPTransactionStage.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPTransport.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPTransportManager.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPUDPSocketList.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPUDPTransport.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPURI.h
HEADER_FILES	+= $(OSS_INCDIR)/SIPUserAgent.h
HEADER_FILES	+= $(OSS_INCDIR)/SoftPhone.h
HEADER_FILES	+= $(OSS_INCDIR)/SoftPhoneInterface.h
HEADER_FILES	+= $(OSS_INCDIR)/SQLiteDb.h
HEADER_FILES	+= $(OSS_INCDIR)/StackVersion.h
HEADER_FILES	+= $(OSS_INCDIR)/StartLine.h
HEADER_FILES	+= $(OSS_INCDIR)/StatefulEventQueue.h
HEADER_FILES	+= $(OSS_INCDIR)/StatusLine.h
HEADER_FILES	+= $(OSS_INCDIR)/StringParam.h
HEADER_FILES	+= $(OSS_INCDIR)/TelURI.h
HEADER_FILES	+= $(OSS_INCDIR)/ThreadPool.h
HEADER_FILES	+= $(OSS_INCDIR)/TransactionId.h
HEADER_FILES	+= $(OSS_INCDIR)/Via.h


#########
#PRESENCE (Promoted as separate library)
#COMMON_SOURCES += $(OSS_SRCDIR)/PresencePackage.cxx
#COMMON_SOURCES += $(OSS_SRCDIR)/PresenceResource.cxx
#COMMON_SOURCES += $(OSS_SRCDIR)/ProfilePresence.cxx
#COMMON_SOURCES += $(OSS_SRCDIR)/ProfileRFC3265.cxx
#COMMON_SOURCES += $(OSS_SRCDIR)/PublishSession.cxx
#COMMON_SOURCES += $(OSS_SRCDIR)/PublishSessionManager.cxx

#HEADER_FILES	+= $(OSS_INCDIR)/PresencePackage.h
#HEADER_FILES	+= $(OSS_INCDIR)/PresenceResource.h
#HEADER_FILES	+= $(OSS_INCDIR)/ProfilePresence.h
#HEADER_FILES	+= $(OSS_INCDIR)/ProfileRFC3265.h
#HEADER_FILES	+= $(OSS_INCDIR)/PublishSession.h
#HEADER_FILES	+= $(OSS_INCDIR)/PublishSessionManager.h



##################
# OPAL
HAS_OPAL = 1

ifeq ($(HAS_OPAL), 1)
OPAL_SRCDIR = $(OPALDIR)/src/opal/src
OPAL_INCDIR = $(OPALDIR)/include
OPAL_OBJDIR = $(OSS_LIBDIR)/$(PT_OBJBASE)

VPATH_CXX := $(OPAL_SRCDIR)/opal \
	     $(OPAL_SRCDIR)/rtp \
	     $(OPAL_SRCDIR)/lids \
	     $(OPAL_SRCDIR)/codec \
	     $(OPAL_SRCDIR)/sip

COMMON_SOURCES += $(OPAL_SRCDIR)/opal/manager.cxx \
           $(OPAL_SRCDIR)/opal/endpoint.cxx \
           $(OPAL_SRCDIR)/opal/connection.cxx \
           $(OPAL_SRCDIR)/opal/call.cxx \
           $(OPAL_SRCDIR)/opal/mediafmt.cxx \
           $(OPAL_SRCDIR)/opal/mediastrm.cxx \
           $(OPAL_SRCDIR)/opal/patch.cxx \
           $(OPAL_SRCDIR)/opal/transcoders.cxx \
           $(OPAL_SRCDIR)/opal/transports.cxx \
           $(OPAL_SRCDIR)/opal/guid.cxx \
           $(OPAL_SRCDIR)/opal/pcss.cxx \
           $(OPAL_SRCDIR)/opal/ivr.cxx \
           $(OPAL_SRCDIR)/opal/opalvxml.cxx \
           $(OPAL_SRCDIR)/rtp/rtp.cxx \
           $(OPAL_SRCDIR)/rtp/jitter.cxx \
           $(OPAL_SRCDIR)/sip/sdp.cxx \

##################
# LIDS

COMMON_SOURCES += $(OPAL_SRCDIR)/lids/lid.cxx \
           $(OPAL_SRCDIR)/lids/lidep.cxx \

ifdef HAS_IXJ
COMMON_SOURCES += $(OPAL_SRCDIR)/lids/ixjunix.cxx
endif

ifdef HAS_VPB
COMMON_SOURCES += $(OPAL_SRCDIR)/lids/vpblid.cxx
endif

ifdef HAS_SANGOMA
COMMON_SOURCES += $(OPAL_SRCDIR)/lids/sangomalid.cxx
endif

##################
# Software codecs
VPATH_C := $(OPAL_SRCDIR)/codec

COMMON_SOURCES += $(OPAL_SRCDIR)/codec/g711codec.cxx \
           $(OPAL_SRCDIR)/codec/g711.c \
           $(OPAL_SRCDIR)/codec/rfc2833.cxx \
           $(OPAL_SRCDIR)/codec/vidcodec.cxx \
           $(OPAL_SRCDIR)/codec/opalwavfile.cxx \
	   $(OPAL_SRCDIR)/codec/silencedetect.cxx \
	   $(OPAL_SRCDIR)/codec/echocancel.cxx

# G.726

G726_DIR = g726


COMMON_SOURCES  += $(OPAL_SRCDIR)/codec/g726codec.cxx \
            $(G726_DIR)/g72x.c \
            $(G726_DIR)/g726_16.c \
            $(G726_DIR)/g726_24.c \
            $(G726_DIR)/g726_32.c \
            $(G726_DIR)/g726_40.c

# GSM06.10

GSM_DIR 	= $(OPAL_SRCDIR)/codec/gsm
GSM_SRCDIR	= $(GSM_DIR)/src
GSM_INCDIR	= $(GSM_DIR)/inc

COMMON_SOURCES += $(OPAL_SRCDIR)/codec/gsmcodec.cxx \
           $(GSM_SRCDIR)/gsm_create.c \
           $(GSM_SRCDIR)/gsm_destroy.c \
           $(GSM_SRCDIR)/gsm_decode.c \
           $(GSM_SRCDIR)/gsm_encode.c \
           $(GSM_SRCDIR)/gsm_option.c \
           $(GSM_SRCDIR)/code.c \
           $(GSM_SRCDIR)/decode.c \
           $(GSM_SRCDIR)/add.c \
           $(GSM_SRCDIR)/gsm_lpc.c \
           $(GSM_SRCDIR)/rpe.c \
           $(GSM_SRCDIR)/preprocess.c \
           $(GSM_SRCDIR)/long_term.c \
           $(GSM_SRCDIR)/short_term.c \
           $(GSM_SRCDIR)/table.c

# LPC-10

LPC10_DIR 	= $(OPAL_SRCDIR)/codec/lpc10
LPC10_INCDIR	= $(LPC10_DIR)
LPC10_SRCDIR	= $(LPC10_DIR)/src

COMMON_SOURCES += $(OPAL_SRCDIR)/codec/lpc10codec.cxx \
           $(LPC10_SRCDIR)/f2clib.c \
           $(LPC10_SRCDIR)/analys.c \
           $(LPC10_SRCDIR)/bsynz.c \
           $(LPC10_SRCDIR)/chanwr.c \
           $(LPC10_SRCDIR)/dcbias.c \
           $(LPC10_SRCDIR)/decode_.c \
           $(LPC10_SRCDIR)/deemp.c \
           $(LPC10_SRCDIR)/difmag.c \
           $(LPC10_SRCDIR)/dyptrk.c \
           $(LPC10_SRCDIR)/encode_.c \
           $(LPC10_SRCDIR)/energy.c \
           $(LPC10_SRCDIR)/ham84.c \
           $(LPC10_SRCDIR)/hp100.c \
           $(LPC10_SRCDIR)/invert.c \
           $(LPC10_SRCDIR)/irc2pc.c \
           $(LPC10_SRCDIR)/ivfilt.c \
           $(LPC10_SRCDIR)/lpcdec.c \
           $(LPC10_SRCDIR)/lpcenc.c \
           $(LPC10_SRCDIR)/lpcini.c \
           $(LPC10_SRCDIR)/lpfilt.c \
           $(LPC10_SRCDIR)/median.c \
           $(LPC10_SRCDIR)/mload.c \
           $(LPC10_SRCDIR)/onset.c \
           $(LPC10_SRCDIR)/pitsyn.c \
           $(LPC10_SRCDIR)/placea.c \
           $(LPC10_SRCDIR)/placev.c \
           $(LPC10_SRCDIR)/preemp.c \
           $(LPC10_SRCDIR)/prepro.c \
           $(LPC10_SRCDIR)/random_.c \
           $(LPC10_SRCDIR)/rcchk.c \
           $(LPC10_SRCDIR)/synths.c \
           $(LPC10_SRCDIR)/tbdm.c \
           $(LPC10_SRCDIR)/voicin.c \
           $(LPC10_SRCDIR)/vparms.c \

# Speex

SPEEX_DIR       = $(OPAL_SRCDIR)/codec/speex

HEADER_FILES    += $(OPAL_INCDIR)/speexcodec.h 

COMMON_SOURCES 	+= $(OPAL_SRCDIR)/codec/speexcodec.cxx 

SPEEX_INCDIR    = $(SPEEX_DIR)/libspeex
SPEEX_SRCDIR    = $(SPEEX_DIR)/libspeex

VPATH_C		+= $(SPEEX_SRCDIR)
VPATH_CXX	+= $(SPEEX_SRCDIR)

HEADER_FILES    += $(SPEEX_INCDIR)/speex.h \
                   $(SPEEX_INCDIR)/speex_bits.h \
                   $(SPEEX_INCDIR)/speex_header.h \
                   $(SPEEX_INCDIR)/speex_callbacks.h \
                   $(SPEEX_INCDIR)/speex_config_types.h \
                   $(SPEEX_INCDIR)/speex_preprocess.h \
                   $(SPEEX_INCDIR)/speex_stereo.h \
                   $(SPEEX_INCDIR)/speex_types.h \
                   $(SPEEX_INCDIR)/speex_jitter.h \
                   $(SPEEX_INCDIR)/speex_echo.h 

COMMON_SOURCES 	+= $(SPEEX_SRCDIR)/nb_celp.c \
                   $(SPEEX_SRCDIR)/sb_celp.c \
                   $(SPEEX_SRCDIR)/lpc.c \
                   $(SPEEX_SRCDIR)/ltp.c \
                   $(SPEEX_SRCDIR)/lsp.c \
                   $(SPEEX_SRCDIR)/quant_lsp.c \
                   $(SPEEX_SRCDIR)/lsp_tables_nb.c \
                   $(SPEEX_SRCDIR)/gain_table.c \
                   $(SPEEX_SRCDIR)/gain_table_lbr.c \
                   $(SPEEX_SRCDIR)/cb_search.c \
                   $(SPEEX_SRCDIR)/filters.c \
                   $(SPEEX_SRCDIR)/bits.c \
                   $(SPEEX_SRCDIR)/modes.c \
                   $(SPEEX_SRCDIR)/vq.c \
                   $(SPEEX_SRCDIR)/high_lsp_tables.c \
                   $(SPEEX_SRCDIR)/vbr.c \
                   $(SPEEX_SRCDIR)/hexc_table.c \
                   $(SPEEX_SRCDIR)/exc_5_256_table.c \
                   $(SPEEX_SRCDIR)/exc_5_64_table.c \
                   $(SPEEX_SRCDIR)/exc_8_128_table.c \
                   $(SPEEX_SRCDIR)/exc_10_32_table.c \
                   $(SPEEX_SRCDIR)/exc_10_16_table.c \
                   $(SPEEX_SRCDIR)/exc_20_32_table.c \
                   $(SPEEX_SRCDIR)/math_approx.c \
                   $(SPEEX_SRCDIR)/kiss_fft.c \
                   $(SPEEX_SRCDIR)/kiss_fftr.c \
                   $(SPEEX_SRCDIR)/fftwrap.c \
                   $(SPEEX_SRCDIR)/hexc_10_32_table.c \
                   $(SPEEX_SRCDIR)/mdf.c \
                   $(SPEEX_SRCDIR)/misc.c \
                   $(SPEEX_SRCDIR)/smallft.c \
                   $(SPEEX_SRCDIR)/speex.c \
                   $(SPEEX_SRCDIR)/speex_header.c \
                   $(SPEEX_SRCDIR)/speex_callbacks.c \
                   $(SPEEX_SRCDIR)/speex_preprocess.c 

# iLBC

ILBC_DIR 	= $(OPAL_SRCDIR)/codec/iLBC

VPATH_C		+= $(ILBC_DIR)
VPATH_CXX	+= $(ILBC_DIR)

HEADER_FILES	+= $(OPAL_INCDIR)/codec/ilbccodec.h
COMMON_SOURCES	+= $(OPAL_SRCDIR)/codec/ilbccodec.cxx

HEADER_FILES	+= $(ILBC_DIR)/iLBC_define.h
HEADER_FILES	+= $(ILBC_DIR)/iLBC_decode.h
COMMON_SOURCES		+= $(ILBC_DIR)/iLBC_decode.c
HEADER_FILES	+= $(ILBC_DIR)/iLBC_encode.h
COMMON_SOURCES		+= $(ILBC_DIR)/iLBC_encode.c
HEADER_FILES	+= $(ILBC_DIR)/FrameClassify.h
COMMON_SOURCES		+= $(ILBC_DIR)/FrameClassify.c
HEADER_FILES	+= $(ILBC_DIR)/LPCdecode.h
COMMON_SOURCES		+= $(ILBC_DIR)/LPCdecode.c
HEADER_FILES	+= $(ILBC_DIR)/LPCencode.h
COMMON_SOURCES		+= $(ILBC_DIR)/LPCencode.c
HEADER_FILES	+= $(ILBC_DIR)/StateConstructW.h
COMMON_SOURCES		+= $(ILBC_DIR)/StateConstructW.c
HEADER_FILES	+= $(ILBC_DIR)/StateSearchW.h
COMMON_SOURCES		+= $(ILBC_DIR)/StateSearchW.c
HEADER_FILES	+= $(ILBC_DIR)/anaFilter.h
COMMON_SOURCES		+= $(ILBC_DIR)/anaFilter.c
HEADER_FILES	+= $(ILBC_DIR)/constants.h
COMMON_SOURCES		+= $(ILBC_DIR)/constants.c
HEADER_FILES	+= $(ILBC_DIR)/createCB.h
COMMON_SOURCES		+= $(ILBC_DIR)/createCB.c
HEADER_FILES	+= $(ILBC_DIR)/doCPLC.h
COMMON_SOURCES		+= $(ILBC_DIR)/doCPLC.c
HEADER_FILES	+= $(ILBC_DIR)/enhancer.h
COMMON_SOURCES		+= $(ILBC_DIR)/enhancer.c
HEADER_FILES	+= $(ILBC_DIR)/filter.h
COMMON_SOURCES		+= $(ILBC_DIR)/filter.c
HEADER_FILES	+= $(ILBC_DIR)/gainquant.h
COMMON_SOURCES		+= $(ILBC_DIR)/gainquant.c
HEADER_FILES	+= $(ILBC_DIR)/getCBvec.h
COMMON_SOURCES		+= $(ILBC_DIR)/getCBvec.c
HEADER_FILES	+= $(ILBC_DIR)/helpfun.h
COMMON_SOURCES		+= $(ILBC_DIR)/helpfun.c
HEADER_FILES	+= $(ILBC_DIR)/hpInput.h
COMMON_SOURCES		+= $(ILBC_DIR)/hpInput.c
HEADER_FILES	+= $(ILBC_DIR)/hpOutput.h
COMMON_SOURCES		+= $(ILBC_DIR)/hpOutput.c
HEADER_FILES	+= $(ILBC_DIR)/iCBConstruct.h
COMMON_SOURCES		+= $(ILBC_DIR)/iCBConstruct.c
HEADER_FILES	+= $(ILBC_DIR)/iCBSearch.h
COMMON_SOURCES		+= $(ILBC_DIR)/iCBSearch.c
HEADER_FILES	+= $(ILBC_DIR)/lsf.h
COMMON_SOURCES		+= $(ILBC_DIR)/lsf.c
HEADER_FILES	+= $(ILBC_DIR)/packing.h
COMMON_SOURCES		+= $(ILBC_DIR)/packing.c
HEADER_FILES	+= $(ILBC_DIR)/syntFilter.h
COMMON_SOURCES		+= $(ILBC_DIR)/syntFilter.c


VIC_DIR = $(OPAL_SRCDIR)/codec/vic

COMMON_SOURCES += $(OPAL_SRCDIR)/codec/h261codec.c \
           $(VIC_DIR)/dct.cxx \
           $(VIC_DIR)/p64.cxx \
           $(VIC_DIR)/huffcode.c \
           $(VIC_DIR)/bv.c \
           $(VIC_DIR)/encoder-h261.cxx \
           $(VIC_DIR)/p64encoder.cxx \
           $(VIC_DIR)/transmitter.cxx \
           $(VIC_DIR)/vid_coder.cxx \
	   $(OPAL_SRCDIR)/codec/h263codec.c 

endif

SOURCES=$(COMMON_SOURCES)

ifndef NOTRACE
STDCCFLAGS += -DPTRACING
endif




#######################################################

#SRC Makefile
include $(PWLIBDIR)/make/common.mak



#######################################################
# make sure pwlib is in the list of external libraries
# this ensures pwlib is not statically linked
LDFLAGS        += -L$(PW_LIBDIR)
SYSLIBS        += -l$(PTLIB_BASE)$(LIB_TYPE)

LIB_BASENAME=$(OSS_BASE)
LIB_FILENAME=$(OSS_FILE)

include $(PWLIBDIR)/make/lib.mak
#######################################################


notrace::
	$(MAKE) NOTRACE=1 opt

ifeq ($(HAS_OPAL), 1)
# Build rules for VIC codecs

$(OPAL_OBJDIR)/%.o : $(VIC_DIR)/%.cxx
	@if [ ! -d $(OPAL_OBJDIR) ] ; then mkdir -p $(OPAL_OBJDIR) ; fi
	$(CPLUS) -I$(VIC_DIR) $(STDCCFLAGS) $(CFLAGS) -c $< -o $@

$(OPAL_OBJDIR)/%.o : $(VIC_DIR)/%.c
	@if [ ! -d $(OPAL_OBJDIR) ] ; then mkdir -p $(OPAL_OBJDIR) ; fi
	$(CC) -I$(VIC_DIR) $(STDCCFLAGS) $(CFLAGS) -c $< -o $@

$(DEPDIR)/%.dep : $(VIC_DIR)/%.c
	@if [ ! -d $(DEPDIR) ] ; then mkdir -p $(DEPDIR) ; fi
	@printf %s $(OPAL_OBJDIR)/ > $@
	$(CPLUS) -I$(VIC_DIR) $(STDCCFLAGS:-g=) $(CFLAGS) -M $< >> $@

$(DEPDIR)/%.dep : $(VIC_DIR)/%.cxx
	@if [ ! -d $(DEPDIR) ] ; then mkdir -p $(DEPDIR) ; fi
	@printf %s $(OPAL_OBJDIR)/ > $@
	$(CPLUS) -I$(VIC_DIR) $(STDCCFLAGS:-g=) $(CFLAGS) -M $< >> $@


# Build rules for the GSM codec

$(OPAL_OBJDIR)/%.o : $(GSM_SRCDIR)/%.c
	@if [ ! -d $(OPAL_OBJDIR) ] ; then mkdir -p $(OPAL_OBJDIR) ; fi
	$(CC) $(STDCCFLAGS) $(ANSI_FLAG) -I$(GSM_INCDIR) -DWAV49 -DNeedFunctionPrototypes=1 $(OPTCCFLAGS) $(CFLAGS) -c $< -o $@

$(DEPDIR)/%.dep : $(GSM_SRCDIR)/%.c
	@if [ ! -d $(DEPDIR) ] ; then mkdir -p $(DEPDIR) ; fi
	@printf %s $(OPAL_OBJDIR)/ > $@
	$(CC) $(ANSI_FLAG) -I$(GSM_INCDIR) -DWAV49 -DNeedFunctionPrototypes=1 $(CFLAGS) -M $< >> $@


# Build rules for the G.726 codec

$(OPAL_OBJDIR)/%.o : $(G726_DIR)/%.c
	@if [ ! -d $(OPAL_OBJDIR) ] ; then mkdir -p $(OPAL_OBJDIR) ; fi
	$(CC) $(STDCCFLAGS) $(CFLAGS) -c $< -o $@

$(DEPDIR)/%.dep : $(G726_DIR)/%.c
	@if [ ! -d $(DEPDIR) ] ; then mkdir -p $(DEPDIR) ; fi
	@printf %s $(OPAL_OBJDIR)/ > $@
	$(CC) $(CFLAGS) -M $< >> $@


# Build rules for the LPC10 codec

$(OPAL_OBJDIR)/%.o : $(LPC10_SRCDIR)/%.c
	@if [ ! -d $(OPAL_OBJDIR) ] ; then mkdir -p $(OPAL_OBJDIR) ; fi
	$(CC) $(STDCCFLAGS) -I$(LPC10_INCDIR) $(OPTCCFLAGS) $(CFLAGS) -c $< -o $@

$(DEPDIR)/%.dep : $(LPC10_SRCDIR)/%.c
	@if [ ! -d $(DEPDIR) ] ; then mkdir -p $(DEPDIR) ; fi
	@printf %s $(OPAL_OBJDIR)/ > $@
	$(CC) -I$(LPC10_INCDIR) $(CFLAGS) -M $< >> $@


# Build rules for the Speex codec

$(OPAL_OBJDIR)/%.o : $(SPEEX_SRCDIR)/%.c
	@if [ ! -d $(OPAL_OBJDIR) ] ; then mkdir -p $(OPAL_OBJDIR) ; fi
	$(CC) $(STDCCFLAGS) -I$(SPEEX_INCDIR) $(OPTCCFLAGS) $(CFLAGS) -c $< -o $@

$(DEPDIR)/%.dep : $(SPEEX_SRCDIR)/%.c
	@if [ ! -d $(DEPDIR) ] ; then mkdir -p $(DEPDIR) ; fi
	@printf %s $(OPAL_OBJDIR)/ > $@
	$(CC) -I$(SPEEX_INCDIR) $(CFLAGS) -M $< >> $@

endif


