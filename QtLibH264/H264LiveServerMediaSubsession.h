#ifndef H264LIVESERVERMEDIASUBSESSION_H
#define H264LIVESERVERMEDIASUBSESSION_H

#include "LiveFramedSource.h"
#include "groupsock/Groupsock.hh"
#include "RtspParameter.h"

class RtspStream;

class H264LiveServerMediaSubsession : public OnDemandServerMediaSubsession {

public:

    static H264LiveServerMediaSubsession*
    createNew(UsageEnvironment& env, RtspStream *rtspStream,
              const char* fileName, bool reuseFirstSource);


protected:
    H264LiveServerMediaSubsession(UsageEnvironment& env, RtspStream *rtspStream,
                                  const char* fileName, bool reuseFirstSource);

    virtual ~H264LiveServerMediaSubsession();


protected:

//    virtual const char* getAuxSDPLine(RTPSink* rtpSink, FramedSource* inputSource);

//    virtual const char * sdpLines(int addressFamily) override;

    virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned &estBitrate);

    virtual RTPSink* createNewRTPSink(Groupsock *rtpGroupsock,
                                      unsigned char rtpPayloadTypeIfDynamic,
                                      FramedSource* inputSource);


    virtual void startStream(unsigned clientSessionId, void* streamToken,
                             TaskFunc* rtcpRRHandler,
                             void* rtcpRRHandlerClientData,
                             unsigned short& rtpSeqNum,
                             unsigned& rtpTimestamp,
                             ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler,
                             void* serverRequestAlternativeByteHandlerClientData);


    virtual void pauseStream(unsigned clientSessionId, void* streamToken);

    virtual void deleteStream(unsigned clientSessionId, void*& streamToken);


private:
    char* fAuxSDPLine_;
    LiveFramedSource *camera_;
    RtspParameter *g_param;
    RtspStream *rtspStream_;
};



#endif // H264LiveSERVERMEDIASUBSESSION_H
