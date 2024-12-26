#include "VideoConvert.h"

#include "rga/im2d.hpp"
#include "rga/RockchipRga.h"
#include "rga/RgaUtils.h"
#include "rga/rga.h"

#include <QDateTime>


VideoConvert::VideoConvert(QObject *parent) : QThread(parent) {
    g_param = RtspParameter::getInstance();
    init();
}


void VideoConvert::init() {

    imgWidth_ = g_param->sceneCameraConfig.imgWidth;
    imgHeight_ = g_param->sceneCameraConfig.imgHeight;
    infWidth_ = infHeight_ = 640;

    m_threadActive = false;
//    scaledYuvBuffer_ = new unsigned char[infWidth_ * infHeight_ * 2];
//    bgrBuffer_ = new unsigned char[infWidth_ * infWidth_ * 3];
}


VideoConvert::~VideoConvert() {
//    delete[] scaledYuvBuffer_;
//    delete[] bgrBuffer_;
}


void VideoConvert::cvtYuv2Bgr(unsigned char *input, unsigned char *output) {
//    querystring(RGA_VERSION);

    rga_buffer_t src;
    rga_buffer_t dst;
    rga_buffer_handle_t src_handle;
    rga_buffer_handle_t dst_handle;


    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    src.format = RK_FORMAT_YCbCr_422_SP;
    dst.format = RK_FORMAT_BGR_888;

    src_handle = importbuffer_virtualaddr(input, infWidth_, infHeight_, src.format);
    if (src_handle <= 0) {
        qDebug() << "Failed to import virtualaddr for dst channel!";
    }

    dst_handle = importbuffer_virtualaddr(output, infWidth_, infHeight_, dst.format);
    if (dst_handle <= 0) {
        qDebug() << "Failed to import virtualaddr for dst channel!";
    }


    src = wrapbuffer_handle(src_handle, infWidth_, infHeight_, src.format);
    dst = wrapbuffer_handle(dst_handle, infWidth_, infHeight_, dst.format);

    if (src.width == 0 || dst.width == 0) {
        qDebug() << __FUNCTION__ << ", " << imStrError();
    }

    int ret = imcvtcolor(src, dst, src.format, dst.format);

    if (ret != IM_STATUS_SUCCESS) {
        releasebuffer_handle(src_handle);
        releasebuffer_handle(dst_handle);
    }
}


void VideoConvert::cvtColor(unsigned char *srcData, int srcFmt, int srcW, int srcH,
                            unsigned char *dstData, int dstFmt, int dstW, int dstH) {
//    querystring(RGA_VERSION);

    rga_buffer_t src;
    rga_buffer_t dst;
    rga_buffer_handle_t src_handle;
    rga_buffer_handle_t dst_handle;


    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    src.format = srcFmt;
    dst.format = dstFmt;


    src_handle = importbuffer_virtualaddr(srcData, srcW, srcH, src.format);
    if (src_handle <= 0) {
        qDebug() << "Failed to import virtualaddr for dst channel!";
    }


    dst_handle = importbuffer_virtualaddr(dstData, dstW, dstH, dst.format);
    if (dst_handle <= 0) {
        qDebug() << "Failed to import virtualaddr for dst channel!";
    }

    qDebug() << "in rga, srcW: " << srcW << ", srcH: " << srcH;
    qDebug() << "in rga, dstW: " << dstW << ", dstH: " << dstH;
    src = wrapbuffer_handle(src_handle, srcW, srcH, src.format);
    dst = wrapbuffer_handle(dst_handle, dstW, dstH, dst.format);

    if (src.width == 0 || dst.width == 0) {
        qDebug() << __FUNCTION__ << ", " << imStrError();
    }

    int ret = imcvtcolor(src, dst, src.format, dst.format, IM_RGB_TO_YUV_BT601_LIMIT);

    if (ret != IM_STATUS_SUCCESS) {
        releasebuffer_handle(src_handle);
        releasebuffer_handle(dst_handle);
    }
}


cvBuffer VideoConvert::opencvColorTrans(const cvBuffer& bgr) {

    cv::Mat bgrImg = bgr.toMat();
    cv::Mat yuvImg;
    cv::cvtColor(bgrImg, yuvImg, cv::COLOR_BGR2YUV_I420);

    return cvBuffer(yuvImg,BufferFormat::I420);
}


void VideoConvert::resize(unsigned char* input, unsigned char* output) {
    rga_buffer_t src;
    rga_buffer_t dst;
    rga_buffer_handle_t src_handle;
    rga_buffer_handle_t dst_handle;

    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    src.format = RK_FORMAT_YCbCr_422_SP;
    dst.format = RK_FORMAT_YCbCr_422_SP;

    // resize 4k -> 640 * 640
    src_handle = importbuffer_virtualaddr(input, imgWidth_, imgHeight_, src.format);
    if (src_handle <= 0) {
        qDebug() << "Failed to import virtualaddr for src channel!";
    }

    dst_handle = importbuffer_virtualaddr(output, infWidth_, infHeight_, dst.format);
    if (dst_handle <= 0) {
        qDebug() << "Failed to import virtualaddr for dst channel!";
    }


    src = wrapbuffer_handle(src_handle, imgWidth_, imgHeight_, src.format);
    dst = wrapbuffer_handle(dst_handle, infWidth_, infHeight_, dst.format);

    if (src.width == 0 || dst.width == 0) {
        qDebug() << __FUNCTION__ << ", " << imStrError();
    }

    int ret = imresize(src, dst);

    if (ret != IM_STATUS_SUCCESS) {
        releasebuffer_handle(src_handle);
        releasebuffer_handle(dst_handle);
    }
}


void VideoConvert::startThread(){
    if (!m_threadActive) {
        m_threadActive = true;
        qDebug() << "Start Video Convert!";
        start();
    }
}

void VideoConvert::stopThread()
{
        if (m_threadActive) {
            m_threadActive = false;
            qDebug() << "Stop Video Convert!";
            terminate();
        }
}



//void VideoConvert::stop() {
//    if (m_threadActive_) {
//        threadActive_ = false;
//        sink_->wake();
//    }
//}


//void VideoConvert::quit() {
//    if (m_threadActive_) {
//        terminate_ = true;
//        stop();
//    } else {
//        emitQuitSignal();
//    }
//}


//void VideoConvert::yuv2Bgr() {
//    long t1, t2, t3;
//    Buffer yuvBuffer;
//    Buffer bgrBuffer;
//    LinkedBuffer linkedBuffer;

//    Buffer yuvTest;
//    Buffer bgrTest;
//    while(threadActive_) {

//        while (g_param_->mailbox->queueImageSource.tryPop(yuvBuffer)) {
//            t1 = QDateTime::currentMSecsSinceEpoch();
//            resize(yuvBuffer.data(), scaledYuvBuffer_);
//            t2 = QDateTime::currentMSecsSinceEpoch();
//            cvtColor(scaledYuvBuffer_, bgrBuffer_);
//            t3 = QDateTime::currentMSecsSinceEpoch();

////            qDebug() << "rga resize cost: " << t2 - t1;
////            qDebug() << "rga cvtcolor cost: " << t3 - t2;


//            bgrBuffer = Buffer(bgrBuffer_, infWidth_ * infHeight_ * 3,
//                               BufferFormat::BGR,
//                               infWidth_, infHeight_);

//            linkedBuffer = LinkedBuffer(std::move(yuvBuffer), std::move(bgrBuffer));

////            g_param_->mailbox->queueLinkedBufferInput.push(std::move(linkedBuffer));

//            g_param_->videoCaptured = false;
//        }

//        g_param_->mailbox->queueImageSource.wait();
//    }

//    if (terminate_) {
//        emitQuitSignal();
//    }
//}


void VideoConvert::bgr2Yuv() {
//    long t1, t2;
    cvBuffer yuvBuffer;
    cvBuffer bgrBuffer;

    while(m_threadActive) {

        if(g_param->queueOsdVideo.size()) {

            m_bgrImage = g_param->queueOsdVideo.front().clone();

            g_param->queueOsdVideo.pop();
            cv::Mat toBuffer = m_bgrImage.clone();
            cv::resize(toBuffer,toBuffer,cv::Size(g_param->sceneCameraConfig.imgWidth,g_param->sceneCameraConfig.imgHeight));
            while(g_param->queueYUV420Images.size() > MAX_QUEUE_SIZE){
                g_param->queueYUV420Images.pop();
            }


            if(toBuffer.empty()){
                //qDebug() <<"empty images..";
                continue;
            }

//            qDebug() << "test Bgr: " << bgrBuffer.width() << bgrBuffer.height();
//            cv::Mat test = toBuffer.clone();
//            cv::resize(test,test,cv::Size(3840,2160));
//            double tt1 = QDateTime::currentMSecsSinceEpoch();
//            cvBuffer testBuffer = cvBuffer(test);
//            resize(testBuffer.data(), scaledYuvBuffer_);
//            double tt2 = QDateTime::currentMSecsSinceEpoch();
//            qDebug() << "rga resize 4K -> 640*640 cost: " << tt2 - tt1;

            bgrBuffer = cvBuffer(toBuffer);

            //qDebug() << "BgrBuffer " << bgrBuffer.width() << bgrBuffer.height();

            double t1 = QDateTime::currentMSecsSinceEpoch();
////            cvtColor(bgrBuffer.data(), RK_FORMAT_BGR_888, bgrBuffer.width(), bgrBuffer.height(),
////                     yuvBuffer.data(), RK_FORMAT_YCbCr_420_P, yuvBuffer.width(), yuvBuffer.height());
            yuvBuffer = opencvColorTrans(bgrBuffer);

                //qDebug() << " push YUV420!";
            g_param->queueYUV420Images.push(yuvBuffer);
            double t2 = QDateTime::currentMSecsSinceEpoch();

            //qDebug() << "rga cvtcolor cost: " << t2 - t1;

        }
        QThread::msleep(5);
    }
}

void VideoConvert::run() {
    bgr2Yuv();
}


