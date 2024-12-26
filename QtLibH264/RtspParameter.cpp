#include "RtspParameter.h"
#include <QDebug>
/// V1.5.0-beta2 zhangheng: you hua xin tiao bao
/// V1.9.2-test  zhangheng: back QAccessManager delete code

RtspParameter *RtspParameter::getInstance()
{
    static RtspParameter instance;
    return &instance;
}

RtspParameter::RtspParameter()
{
    init();
}

RtspParameter::~RtspParameter()
{

}

void RtspParameter::init()
{

    sceneCameraConfig.id = "sceneCameraConfig";
    sceneCameraConfig.enabled = true;
    sceneCameraConfig.imgWidth = 1920;
    sceneCameraConfig.imgHeight = 1080;
    sceneCameraConfig.fps = 50;
    sceneCameraConfig.bps = 6000000;
    sceneCameraConfig.port = 0;
    isImgCaptured = false;

}
