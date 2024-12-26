#include "RtspStream.h"


RtspStream::RtspStream(QObject *parent) : QThread(parent) {
    init();
}


RtspStream::~RtspStream() {
    release();
}


void RtspStream::startAttachedTask() {
    emit signalStartAttachedTask();
}


void RtspStream::stopAttachedTask() {
    emit signalStopAttachedTask();
}


void RtspStream::init() {

    m_threadActive = false;
    OutPacketBuffer::maxSize = 4000000;

    scheduler_ = BasicTaskScheduler::createNew();
    env_ = BasicUsageEnvironment::createNew(*scheduler_);
    rtspServer_ = RTSPServer::createNew(*env_, 8554);


    const char* streamName = "live";
    const char* inputFileNam = "MIPI";
    const char* descriptionString = "Session streamed by live555";

    sms_ = ServerMediaSession::createNew(*env_, streamName, streamName, descriptionString);

    sms_->addSubsession(H264LiveServerMediaSubsession::createNew(
                            *env_, this, inputFileNam, true));

    rtspServer_->addServerMediaSession(sms_);

    announceStream(rtspServer_, sms_, streamName, inputFileNam);
}


void RtspStream::announceStream
(RTSPServer *rtspServer, ServerMediaSession *sms, const char *streamName, const char *inputFileName) {
    char *url = rtspServer->rtspURL(sms_);
    UsageEnvironment& env = rtspServer->envir();

//    LOG(INFO) << "\"" << streamName << "\" stream, from the file \""
//        << inputFileName << "\"\n";

    qDebug() << "Play rtsp stream using the URL \"" << url << "\"\n";

    delete[] url;
}


void RtspStream::release() {

    qDebug() << "rtsp released";

    Medium::close(rtspServer_);

    env_->reclaim();
    delete scheduler_;
}


void RtspStream::startThread() {
    if (!m_threadActive) {
        m_threadActive = true;
        eventLoopWatchVariable_ = 0;
        qDebug() << "Start RTSP Streaming";
        start();
    }
}

void RtspStream::stopThread()
{
    if (m_threadActive) {
        m_threadActive = false;
        qDebug() << "Stop RTSP server!";
        terminate();
    }
}


//void RtspStream::stop() {
//    threadActive_ = false;
//    eventLoopWatchVariable_ = 1;

//    g_param_->mailbox->queueH264Packet.wake();
//}


//void RtspStream::quit() {
//    if (threadActive_) {
//        terminate_ = true;
//        stop();
//    } else {
//        emitQuitSignal();
//    }
//}


void RtspStream::run() {
    while(m_threadActive){
        env_->taskScheduler().doEventLoop(&eventLoopWatchVariable_);
        QThread::msleep(5);
    }


//    if (terminate_) {
//        release();
//        emitQuitSignal();
//    }
}




