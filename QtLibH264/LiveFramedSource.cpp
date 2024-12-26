#include "LiveFramedSource.h"


LiveFramedSource::LiveFramedSource(UsageEnvironment& env):
    FramedSource(env)
//    fOutputBuffer_(nullptr),
//    fMaxOfFrameToSend_(1),
//    fTotOfFrameToSend_(0)
{
   sps_sent_ = false;
   g_param = RtspParameter::getInstance();

}


unsigned int LiveFramedSource::maxFrameSize() const {
//    return 50000;
    return 0;
}


void LiveFramedSource::doGetNextFrame() {
    //qDebug() << "do next frame!";
//    if (!sps_sent_) {

////        memcpy(fTo, g_param_->sps_pps_buffer + 4, g_param_->sps_pps_len - 4);
//        memcpy(fTo, g_param_->sps_pps_buffer, g_param_->sps_pps_len);
//        fFrameSize = g_param_->sps_pps_len;

//        qDebug() << "has sent sps ======================================="
//                    "===================================================="
//                    "=====================================================";

//        sps_sent_ = true;
//        return;
//    }



    while(!g_param->queueH264Packets.size()){
        QThread::msleep(30);
        //qDebug() << "H264 queue empty";
    }
    if(g_param->queueH264Packets.size()){
        m_h264Packet = g_param->queueH264Packets.front();
        g_param->queueH264Packets.pop();
    }
        //Buffer packet = m_h264Packet.copy();
        if (m_h264Packet.size() == 0) {
            //qDebug() << "Empty Packet!";
            //return;
        }
        else{
            memcpy(fTo, m_h264Packet.data(), m_h264Packet.size());
            qDebug() << "Send H264!";

            if (m_h264Packet.size() > fMaxSize) {
                fNumTruncatedBytes = m_h264Packet.size() - fMaxSize;
                fFrameSize = fMaxSize;
            } else {
                fNumTruncatedBytes = 0;
                fFrameSize = m_h264Packet.size();
            }
        }


    //    fFrameSize = packet.size();

    //    static auto f = fopen("test.h264", "wb+");


    //    std::cout << "left fmaxSize: " << fMaxSize << std::endl;



    //    fNumTruncatedBytes = 0;


    gettimeofday(&fPresentationTime, NULL);
    afterGetting(this);

}

