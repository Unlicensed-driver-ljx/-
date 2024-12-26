#include "H264LiveServerMediaSubsession.h"
#include "RtspStream.h"
#include "VideoEncode.h"


H264LiveServerMediaSubsession::H264LiveServerMediaSubsession
(UsageEnvironment& env, RtspStream *rtspStream,
 const char* fileName, bool reuseFirstSource) :
    OnDemandServerMediaSubsession(env, reuseFirstSource),
    fAuxSDPLine_(nullptr),
    camera_(nullptr),
    rtspStream_(rtspStream) {

    g_param = RtspParameter::getInstance();
}



H264LiveServerMediaSubsession::~H264LiveServerMediaSubsession() {
    delete[] fAuxSDPLine_;
}



//const char* H264LiveServerMediaSubsession::getAuxSDPLine(RTPSink* rtpSink, FramedSource* inputSource)
//{
//    char const* auxSDPLine = rtpSink->auxSDPLine();
//    if (auxSDPLine != NULL) {
//        return auxSDPLine;
//    } else {

//        char const* fmtpFmt = "a=fmtp:%d packetization-mode=1"
//                    ";profile-level-id=000000"
//                    ";sprop-parameter-sets=H264\r\n";

//        unsigned fmtpFmtSize = strlen(fmtpFmt)+3/* max char len */;

//        char* fmtp = new char[fmtpFmtSize];
//        delete[] fAuxSDPLine_;
//        memcpy(fmtp,fmtpFmt,fmtpFmtSize*sizeof(char));
//        fAuxSDPLine_ = fmtp;
//        return fAuxSDPLine_;
//    }
//}


//const char * H264LiveServerMediaSubsession::sdpLines(int addressFamily) {

//    qDebug() << "gonna get sdp lines";

//    return fSDPLines = (char*)"m=video 0 RTP/AVP 96\r\n"
//                              "c=IN IP4 0.0.0.0\r\n"
//                              "b=AS:96\r\n"
//                              "a=rtpmap:96 H264/90000\r\n"
//                              "a=fmtp:96 packetization-mode=1;profile-level-id=000000;sprop-parameter-sets=H264\r\n"
//                              "a=control:track1\r\n";
//}



H264LiveServerMediaSubsession*
H264LiveServerMediaSubsession::createNew(
        UsageEnvironment& env, RtspStream *rtspStream,
        const char* fileName, bool reuseFirstSource) {
    return new H264LiveServerMediaSubsession(
                env, rtspStream, fileName, reuseFirstSource);
}


FramedSource* H264LiveServerMediaSubsession::createNewStreamSource
(unsigned int clientSessionId, unsigned int &estBitrate) {

    estBitrate = 400;   // kbps, estimate

    LiveFramedSource *frameSource = new LiveFramedSource(envir());

    camera_ = frameSource;

    FramedSource *tmp = H264VideoStreamDiscreteFramer::createNew(envir(), frameSource);   // attention, use discrete framer here
    return tmp;
}



RTPSink* H264LiveServerMediaSubsession::createNewRTPSink
(Groupsock *rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource *inputSource) {

//    return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);

    return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic,
                                       VideoEncode::getSpsNaluAddr(), VideoEncode::getSpsNaluSize(),
                                       VideoEncode::getPpsNaluAddr(), VideoEncode::getPpsNaluSize());
}


void H264LiveServerMediaSubsession::startStream(unsigned int clientSessionId,
                                                void *streamToken,
                                                TaskFunc *rtcpRRHandler,
                                                void *rtcpRRHandlerClientData,
                                                unsigned short &rtpSeqNum,
                                                unsigned int &rtpTimestamp,
                                                ServerRequestAlternativeByteHandler *serverRequestAlternativeByteHandler,
                                                void *serverRequestAlternativeByteHandlerClientData) {

    qDebug() << "Start stream!";

    rtspStream_->startAttachedTask();

    StreamState* streamState = (StreamState*)streamToken;


    OnDemandServerMediaSubsession::startStream(clientSessionId, streamToken, rtcpRRHandler,
                                               rtcpRRHandlerClientData, rtpSeqNum, rtpTimestamp,
                                               serverRequestAlternativeByteHandler,
                                               serverRequestAlternativeByteHandlerClientData);
}


void H264LiveServerMediaSubsession::pauseStream(unsigned int clientSessionId, void *streamToken) {
    StreamState* streamState = (StreamState*)streamToken;

    qDebug() << "Pause stream!";

    OnDemandServerMediaSubsession::pauseStream(clientSessionId, streamToken);
}


void H264LiveServerMediaSubsession::deleteStream(unsigned int clientSessionId, void *&streamToken) {

    qDebug() << "Delete stream!";

    rtspStream_->stopAttachedTask();

    OnDemandServerMediaSubsession::deleteStream(clientSessionId, streamToken);
}
