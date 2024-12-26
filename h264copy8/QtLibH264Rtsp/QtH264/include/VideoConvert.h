#ifndef VIDEOCONVERT_H
#define VIDEOCONVERT_H

#include "RtspParameter.h"

class VideoConvert : public QThread
{
    Q_OBJECT
public:
    VideoConvert(QObject *parent = nullptr);
    ~VideoConvert();

    void init();
    
    void run() override;

    void startThread();

    void stopThread();

private:
    void cvtYuv2Bgr(unsigned char* input, unsigned char* output);

    void cvtColor(unsigned char *srcData, int srcFmt, int srcW, int srcH,
                  unsigned char *dstData, int dstFmt, int dstW, int dstH);

    void resize(unsigned char* input, unsigned char* output);

    void yuv2Bgr();

    void bgr2Yuv();

    cvBuffer opencvColorTrans(const cvBuffer& bgr);


private:    
    int imgWidth_;
    int imgHeight_;

    int infWidth_;
    int infHeight_;

    RtspParameter *g_param;

    bool m_threadActive;

    cv::Mat m_bgrImage;

    unsigned char* scaledYuvBuffer_;
//    unsigned char* bgrBuffer_;
};

#endif // VIDEOCONVERT_H
