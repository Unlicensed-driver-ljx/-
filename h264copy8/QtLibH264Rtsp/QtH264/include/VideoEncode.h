#ifndef ENCODING_H
#define ENCODING_H

#include <rk_mpi.h>
#include "mpp_err.h"
#include "RtspParameter.h"

#define MPP_ALIGN(x, a)         (((x)+(a)-1)&~((a)-1))


struct MppEncData {
	// global flow control flag
	uint32_t frm_eos;
	uint32_t pkt_eos;
	uint32_t frame_count;
	uint64_t stream_size;

	// base flow context
	MppCtx ctx;
	MppApi *mpi;
	MppEncPrepCfg prep_cfg;
	MppEncRcCfg rc_cfg;
	MppEncCodecCfg codec_cfg;

	// input / output
	MppBuffer frm_buf;
	MppEncSeiMode sei_mode;

	uint32_t width;
	uint32_t height;
	uint32_t hor_stride;
	uint32_t ver_stride;
    MppFrameFormat fmt;
    MppCodingType type;
    float bpp;
	uint32_t num_frames;

	// resources
	size_t frame_size;
    int32_t gop;
    int32_t fps;
	int32_t bps;
};


class VideoEncode: public QThread
{
    Q_OBJECT
public:
    VideoEncode(QObject *parent = nullptr);
    ~VideoEncode();
    void init();
    void run() override;

    void startThread();

    void stopThread();

    static unsigned char* getSpsNaluAddr();
    static int getSpsNaluSize();

    static unsigned char* getPpsNaluAddr();
    static int getPpsNaluSize();

    RtspParameter *g_param;

    bool m_threadActive;

private:
    bool processImage(uint8_t *p);
    bool processImageTest(uint8_t *p);
    void paddingFrame(uint8_t *buf, uint8_t *image, uint32_t width, uint32_t height,
                       uint32_t hor_stride, uint32_t ver_stride, MppFrameFormat fmt);
    void destroyMpp();
    void parseExtraInfo(unsigned char* info, int len);


    static unsigned char *spsNaluAddr_, *ppsNaluAddr_;
    static int spsNaluSize_, ppsNaluSize_;

    bool repeatSpsPps_;
    MppEncData mppEncData_;

    cvBuffer m_yuvBuffer;
};

#endif // ENCODING_H


