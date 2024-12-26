#ifndef MAINSLAVER_H
#define MAINSLAVER_H


#include <QObject>
#include <QProcess>
#include <QTimer>
#include <QJsonDocument>

//#include "CVVideoCapture.h"
//#include "VideoDetect.h"
#include <QEventLoop>
#include <QMutex>

#include "VideoConvert.h"
#include "VideoEncode.h"
#include "RtspStream.h"

class MainSlaver : public QObject
{

    friend class StateCheck;
    Q_OBJECT
public:
    explicit MainSlaver(QObject *parent = nullptr);
    /// 析构
    ~MainSlaver();
    void startSlaver();
    void stopSlaver();

    /// 启动任务
    void startTask();
    /// 关闭任务
    void stopTask();

    void startDetection();

    void stopDetection();

    void terminate();

    void pushImageToEncodeQueue(cv::Mat inputImg);

private:
    RtspParameter *g_param = nullptr;

    /// scene camera video capture
    // CVVideoCapture *m_captureVideo = nullptr;


    /// rtsp streaming
    VideoConvert *m_videoConverter = nullptr;
    VideoEncode *m_videoEncoder = nullptr;
    RtspStream *m_rtspServer = nullptr;

    /// video detect
    //VideoDetect *m_videoDetect = nullptr;

    /// mutex
    QMutex m_mtx;

    int timeFlag;
};

#endif // MAINSLAVER_H
