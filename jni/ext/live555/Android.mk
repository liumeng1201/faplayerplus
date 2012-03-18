
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := liblive555

LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -DLOCALE_NOT_USED -DSOCKLEN_T=socklen_t -DNO_SSTREAM=1 -D_LARGEFILE_SOURCE=1 -D_FILE_OFFSET_BITS=64

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/BasicUsageEnvironment/include \
    $(LOCAL_PATH)/UsageEnvironment/include \
    $(LOCAL_PATH)/groupsock/include \
    $(LOCAL_PATH)/liveMedia/include

LOCAL_SRC_FILES := \
    BasicUsageEnvironment/BasicHashTable.cpp \
    BasicUsageEnvironment/BasicTaskScheduler.cpp \
    BasicUsageEnvironment/BasicTaskScheduler0.cpp \
    BasicUsageEnvironment/BasicUsageEnvironment.cpp \
    BasicUsageEnvironment/BasicUsageEnvironment0.cpp \
    BasicUsageEnvironment/DelayQueue.cpp \
    UsageEnvironment/HashTable.cpp \
    UsageEnvironment/UsageEnvironment.cpp \
    UsageEnvironment/strDup.cpp \
    groupsock/GroupEId.cpp \
    groupsock/Groupsock.cpp \
    groupsock/GroupsockHelper.cpp \
    groupsock/IOHandlers.cpp \
    groupsock/NetAddress.cpp \
    groupsock/NetInterface.cpp \
    groupsock/inet.c \
    liveMedia/AC3AudioFileServerMediaSubsession.cpp \
    liveMedia/AC3AudioRTPSink.cpp \
    liveMedia/AC3AudioRTPSource.cpp \
    liveMedia/AC3AudioStreamFramer.cpp \
    liveMedia/ADTSAudioFileServerMediaSubsession.cpp \
    liveMedia/ADTSAudioFileSource.cpp \
    liveMedia/AMRAudioFileServerMediaSubsession.cpp \
    liveMedia/AMRAudioFileSink.cpp \
    liveMedia/AMRAudioFileSource.cpp \
    liveMedia/AMRAudioRTPSink.cpp \
    liveMedia/AMRAudioRTPSource.cpp \
    liveMedia/AMRAudioSource.cpp \
    liveMedia/AVIFileSink.cpp \
    liveMedia/AudioInputDevice.cpp \
    liveMedia/AudioRTPSink.cpp \
    liveMedia/Base64.cpp \
    liveMedia/BasicUDPSink.cpp \
    liveMedia/BasicUDPSource.cpp \
    liveMedia/BitVector.cpp \
    liveMedia/ByteStreamFileSource.cpp \
    liveMedia/ByteStreamMultiFileSource.cpp \
    liveMedia/DVVideoFileServerMediaSubsession.cpp \
    liveMedia/DVVideoRTPSink.cpp \
    liveMedia/DVVideoRTPSource.cpp \
    liveMedia/DVVideoStreamFramer.cpp \
    liveMedia/DarwinInjector.cpp \
    liveMedia/DeviceSource.cpp \
    liveMedia/DigestAuthentication.cpp \
    liveMedia/FileServerMediaSubsession.cpp \
    liveMedia/FileSink.cpp \
    liveMedia/FramedFileSource.cpp \
    liveMedia/FramedFilter.cpp \
    liveMedia/FramedSource.cpp \
    liveMedia/GSMAudioRTPSink.cpp \
    liveMedia/H261VideoRTPSource.cpp \
    liveMedia/H263plusVideoFileServerMediaSubsession.cpp \
    liveMedia/H263plusVideoRTPSink.cpp \
    liveMedia/H263plusVideoRTPSource.cpp \
    liveMedia/H263plusVideoStreamFramer.cpp \
    liveMedia/H263plusVideoStreamParser.cpp \
    liveMedia/H264VideoFileServerMediaSubsession.cpp \
    liveMedia/H264VideoFileSink.cpp \
    liveMedia/H264VideoRTPSink.cpp \
    liveMedia/H264VideoRTPSource.cpp \
    liveMedia/H264VideoStreamDiscreteFramer.cpp \
    liveMedia/H264VideoStreamFramer.cpp \
    liveMedia/HTTPSink.cpp \
    liveMedia/InputFile.cpp \
    liveMedia/JPEGVideoRTPSink.cpp \
    liveMedia/JPEGVideoRTPSource.cpp \
    liveMedia/JPEGVideoSource.cpp \
    liveMedia/Locale.cpp \
    liveMedia/MP3ADU.cpp \
    liveMedia/MP3ADURTPSink.cpp \
    liveMedia/MP3ADURTPSource.cpp \
    liveMedia/MP3ADUTranscoder.cpp \
    liveMedia/MP3ADUdescriptor.cpp \
    liveMedia/MP3ADUinterleaving.cpp \
    liveMedia/MP3AudioFileServerMediaSubsession.cpp \
    liveMedia/MP3FileSource.cpp \
    liveMedia/MP3HTTPSource.cpp \
    liveMedia/MP3Internals.cpp \
    liveMedia/MP3InternalsHuffman.cpp \
    liveMedia/MP3InternalsHuffmanTable.cpp \
    liveMedia/MP3StreamState.cpp \
    liveMedia/MP3Transcoder.cpp \
    liveMedia/MPEG1or2AudioRTPSink.cpp \
    liveMedia/MPEG1or2AudioRTPSource.cpp \
    liveMedia/MPEG1or2AudioStreamFramer.cpp \
    liveMedia/MPEG1or2Demux.cpp \
    liveMedia/MPEG1or2DemuxedElementaryStream.cpp \
    liveMedia/MPEG1or2DemuxedServerMediaSubsession.cpp \
    liveMedia/MPEG1or2FileServerDemux.cpp \
    liveMedia/MPEG1or2VideoFileServerMediaSubsession.cpp \
    liveMedia/MPEG1or2VideoHTTPSink.cpp \
    liveMedia/MPEG1or2VideoRTPSink.cpp \
    liveMedia/MPEG1or2VideoRTPSource.cpp \
    liveMedia/MPEG1or2VideoStreamDiscreteFramer.cpp \
    liveMedia/MPEG1or2VideoStreamFramer.cpp \
    liveMedia/MPEG2IndexFromTransportStream.cpp \
    liveMedia/MPEG2TransportFileServerMediaSubsession.cpp \
    liveMedia/MPEG2TransportStreamFramer.cpp \
    liveMedia/MPEG2TransportStreamFromESSource.cpp \
    liveMedia/MPEG2TransportStreamFromPESSource.cpp \
    liveMedia/MPEG2TransportStreamIndexFile.cpp \
    liveMedia/MPEG2TransportStreamMultiplexor.cpp \
    liveMedia/MPEG2TransportStreamTrickModeFilter.cpp \
    liveMedia/MPEG4ESVideoRTPSink.cpp \
    liveMedia/MPEG4ESVideoRTPSource.cpp \
    liveMedia/MPEG4GenericRTPSink.cpp \
    liveMedia/MPEG4GenericRTPSource.cpp \
    liveMedia/MPEG4LATMAudioRTPSink.cpp \
    liveMedia/MPEG4LATMAudioRTPSource.cpp \
    liveMedia/MPEG4VideoFileServerMediaSubsession.cpp \
    liveMedia/MPEG4VideoStreamDiscreteFramer.cpp \
    liveMedia/MPEG4VideoStreamFramer.cpp \
    liveMedia/MPEGVideoStreamFramer.cpp \
    liveMedia/MPEGVideoStreamParser.cpp \
    liveMedia/Media.cpp \
    liveMedia/MediaSession.cpp \
    liveMedia/MediaSink.cpp \
    liveMedia/MediaSource.cpp \
    liveMedia/MultiFramedRTPSink.cpp \
    liveMedia/MultiFramedRTPSource.cpp \
    liveMedia/OnDemandServerMediaSubsession.cpp \
    liveMedia/OutputFile.cpp \
    liveMedia/PassiveServerMediaSubsession.cpp \
    liveMedia/QCELPAudioRTPSource.cpp \
    liveMedia/QuickTimeFileSink.cpp \
    liveMedia/QuickTimeGenericRTPSource.cpp \
    liveMedia/RTCP.cpp \
    liveMedia/RTPInterface.cpp \
    liveMedia/RTPSink.cpp \
    liveMedia/RTPSource.cpp \
    liveMedia/RTSPClient.cpp \
    liveMedia/RTSPCommon.cpp \
    liveMedia/RTSPServer.cpp \
    liveMedia/SIPClient.cpp \
    liveMedia/ServerMediaSession.cpp \
    liveMedia/SimpleRTPSink.cpp \
    liveMedia/SimpleRTPSource.cpp \
    liveMedia/StreamParser.cpp \
    liveMedia/VideoRTPSink.cpp \
    liveMedia/WAVAudioFileServerMediaSubsession.cpp \
    liveMedia/WAVAudioFileSource.cpp \
    liveMedia/uLawAudioFilter.cpp \
    liveMedia/our_md5.c \
    liveMedia/our_md5hl.c \
    liveMedia/rtcp_from_spec.c

include $(BUILD_STATIC_LIBRARY)

