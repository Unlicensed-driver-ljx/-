#ifndef RTSPPARAMTER_H
#define RTSPPARAMTER_H
#include <QObject>
//#include <QQueue>
#include <queue>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>
#include <opencv2/opencv.hpp>
//#include "GeneralType.h"
#include "Buffer.h"

const int MAX_QUEUE_SIZE = 10;
const int OFFLINE_MODE = 0;
const int ONLINE_MODE = 1;

//using namespace cv;

/// 相机参数
typedef struct SceneCameraConfig
{
    QString id;
    bool enabled;
    int imgWidth;
    int imgHeight;
    int fps;
    int port;
}SceneCameraConfig;



/// 单例模式全局类
class RtspParameter
{
public:
    static RtspParameter *getInstance();
private:
    RtspParameter();
    ~RtspParameter();

public:
    /// 参数初始化
    void init();

public:

    /// 相机参数
    SceneCameraConfig sceneCameraConfig;



public:
    std::queue<cv::Mat> queueVideoCapture;
    std::queue<cv::Mat> queueStreaming;
    // Rtsp
    std::queue<cv::Mat> queueOsdVideo;
    std::queue<cvBuffer> queueYUV420Images;
    std::queue<cvBuffer> queueH264Packets;

    bool isImgCaptured;

};

#endif // GLOBALPARAMTER_H
