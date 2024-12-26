#pragma once

#include <QThread>
#include <QDebug>

// Live555 includes
#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>

#include "H264LiveServerMediaSubsession.h"

class RtspStream : public QThread
{
    Q_OBJECT
public:
    RtspStream(QObject *parent = nullptr);
    ~RtspStream();

    void init();
//    void start() override;
//    void stop() override;
//    void quit() override;
    void run() override;

    void startThread();

    void stopThread();

    void release();

    void startAttachedTask();
    void stopAttachedTask();

signals:
    void signalStartAttachedTask();
    void signalStopAttachedTask();

private:
    void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
                        const char* streamName, const char* inputFileName);

private:
    char eventLoopWatchVariable_;

    TaskScheduler *scheduler_;
    UsageEnvironment *env_;
    RTSPServer *rtspServer_;
    ServerMediaSession *sms_;
    H264VideoStreamFramer *videoSource_;

    int imgWidth_;
    int imgHeight_;

    bool m_threadActive;
};


#endif // LIVERTSP_H
