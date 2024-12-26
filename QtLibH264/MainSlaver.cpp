#include "MainSlaver.h"
#include <QTime>
#include <QThread>
#include <QString>
#include <QDateTime>
#include <iostream>
#include <QtNetwork/QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

MainSlaver::MainSlaver(QObject *parent) : QObject(parent)
{


    m_videoConverter = new VideoConvert();
    m_videoEncoder = new VideoEncode();
    //m_rtspServer = new RtspStream();

    timeFlag = 0;

    g_param = RtspParameter::getInstance();

    qDebug() << "Rtsp Image Size:" << g_param->sceneCameraConfig.imgWidth << g_param->sceneCameraConfig.imgHeight;
    qDebug() << "Rtsp Image Fps:" << g_param->sceneCameraConfig.fps;
    qDebug() << "H264 Convert Bitrate:" << g_param->sceneCameraConfig.bps;
}

MainSlaver::MainSlaver(cv::Size imsize, int fps, int32_t bps, QObject *parent)
{
    g_param = RtspParameter::getInstance();
    g_param->sceneCameraConfig.imgWidth = imsize.width;
    g_param->sceneCameraConfig.imgHeight = imsize.height;
    g_param->sceneCameraConfig.fps = fps;
    g_param->sceneCameraConfig.bps = bps;

    m_videoConverter = new VideoConvert();
    m_videoEncoder = new VideoEncode();
    //m_rtspServer = new RtspStream();

    timeFlag = 0;

    qDebug() << "Rtsp Image Size:" << g_param->sceneCameraConfig.imgWidth << g_param->sceneCameraConfig.imgHeight;
    qDebug() << "Rtsp Image Fps:" << g_param->sceneCameraConfig.fps;
    qDebug() << "H264 Convert Bitrate:" << g_param->sceneCameraConfig.bps;
}


MainSlaver::~MainSlaver()
{
    stopTask();
    terminate();
}


void MainSlaver::startSlaver(){

    startTask();
}


void MainSlaver::stopSlaver(){

    stopTask();
}


void MainSlaver::startTask()
{
    //m_captureVideo->startCapture();
    //m_videoDetect->startDetect();

    m_videoConverter->startThread();
    m_videoEncoder->startThread();
    //m_rtspServer->startThread();
}


void MainSlaver::stopTask()
{
    m_videoConverter->stopThread();
    m_videoEncoder->stopThread();
    //m_rtspServer->stopThread();

}


void MainSlaver::startDetection() {
    //m_videoDetect->startDetect();
}


void MainSlaver::stopDetection() {
    //m_videoDetect->stopDetect();
}


void MainSlaver::terminate(){
    //m_captureVideo->stopCapture();
    // m_videoDetect->quitThread();
}

void MainSlaver::pushImageToEncodeQueue(cv::Mat inputImg)
{
    while(g_param->queueOsdVideo.size()>MAX_QUEUE_SIZE){
        g_param->queueOsdVideo.pop();
    }
    g_param->queueOsdVideo.push(inputImg);
}

cvBuffer MainSlaver::getH264Frame()
{
    cvBuffer getBuffer;
    if(g_param->queueH264Packets.size()){
        getBuffer = g_param->queueH264Packets.front();
        g_param->queueH264Packets.pop();
    }
    else{
        //getBuffer = cvBuffer();
    }

    qDebug() << "H264 Buffer Size ->" << getBuffer.size();
    return getBuffer;
}



