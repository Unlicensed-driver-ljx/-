#include "VideoEncode.h"
#include <iostream>
#include <QDateTime>

unsigned char* VideoEncode::spsNaluAddr_ = nullptr;
unsigned char* VideoEncode::ppsNaluAddr_ = nullptr;
int VideoEncode::spsNaluSize_ = 0;
int VideoEncode::ppsNaluSize_ = 0;


VideoEncode::VideoEncode(QObject *parent) : QThread(parent) {
    g_param = RtspParameter::getInstance();

    m_threadActive = false;
//    sink_ = &g_param_->mailbox->queueI420;
//    source_ = &g_param_->mailbox->queueH264Packet;

    memset(&mppEncData_, 0, sizeof(mppEncData_));
    mppEncData_.width = g_param->sceneCameraConfig.imgWidth;
    mppEncData_.height = g_param->sceneCameraConfig.imgHeight;
    mppEncData_.hor_stride = MPP_ALIGN(mppEncData_.width, 16);
    mppEncData_.ver_stride = MPP_ALIGN(mppEncData_.height, 16);

//    mppEncData_.fmt = MPP_FMT_YUV422SP;     // MIPI camera
    mppEncData_.fmt = MPP_FMT_YUV420P;        // USB camera
    mppEncData_.bpp = 1.5;
    mppEncData_.type = MPP_VIDEO_CodingAVC;
    mppEncData_.fps = 50;
    mppEncData_.gop = 20;
    mppEncData_.frame_size = mppEncData_.hor_stride * mppEncData_.ver_stride * mppEncData_.bpp;

    repeatSpsPps_ = true;  // send sps and pps repeatedly

    init();
}


VideoEncode::~VideoEncode() {
    delete[] spsNaluAddr_;
    delete[] ppsNaluAddr_;
}


void VideoEncode::parseExtraInfo(unsigned char* info, int len) {
    int i = 0;
    std::vector<int> sep;

    while (i < len) {
        if (info[i] == 0 && info[i + 1] == 0 &&
            info[i + 2] == 0 && info[i + 3] == 1) {
            sep.push_back(i);
            i += 4;
        } else {
            i++;
        }
    }

    spsNaluAddr_ = info + sep[0];
    spsNaluSize_ = sep[1] - sep[0];
    ppsNaluAddr_ = info + sep[1];
    ppsNaluSize_ = len - sep[1];

    unsigned char* spsNalu = new unsigned char[spsNaluSize_];
    unsigned char* ppsNalu = new unsigned char[ppsNaluSize_];
    memcpy(spsNalu, spsNaluAddr_, spsNaluSize_);
    memcpy(ppsNalu, ppsNaluAddr_, ppsNaluSize_);

    spsNaluAddr_ = spsNalu;
    ppsNaluAddr_ = ppsNalu;

//    LOG(INFO) << "sps size: " << spsNaluSize_ << ", pps size: " << ppsNaluSize_;
//    std::cout << std::showbase << std::hex << "spsNalu" << std::endl;
//    for (int i = 0; i < spsNaluSize_; i++) {
////        LOG(INFO) << std::showbase << std::hex << spsNaluAddr_[i];
//        printf("%x, ", spsNaluAddr_[i]);
//    }

//    std::cout << "\nppsNalu" << std::endl;
//    for (int i = 0; i < ppsNaluSize_; i++) {
////        std::cout << std::showbase << std::hex << ppsNaluAddr_[i];
//        printf("%x, ", ppsNaluAddr_[i]);
//    }
}


unsigned char* VideoEncode::getSpsNaluAddr() {
    return spsNaluAddr_;
}


unsigned char* VideoEncode::getPpsNaluAddr() {
    return ppsNaluAddr_;
}


int VideoEncode::getSpsNaluSize() {
    return spsNaluSize_;
}


int VideoEncode::getPpsNaluSize() {
    return ppsNaluSize_;
}


void VideoEncode::init() {
	MPP_RET ret = MPP_OK;

    // allocate memory for encoding
    ret = mpp_buffer_get(NULL, &mppEncData_.frm_buf, mppEncData_.frame_size);
    if (ret) {
		printf("failed to get buffer for input frame ret %d\n", ret);
		goto MPP_INIT_OUT;
	}

    // create MPP context and MPP api interface
    ret = mpp_create(&mppEncData_.ctx, &mppEncData_.mpi);
    if (ret) {
        qDebug() << "mpp_create failed!";
		goto MPP_INIT_OUT;
	}

    // initialize encoding format
    ret = mpp_init(mppEncData_.ctx, MPP_CTX_ENC, mppEncData_.type);
    if (ret) {
        qDebug() << "mpp_create failed!";
		goto MPP_INIT_OUT;
	}

    // configure encoding parameters
    //mppEncData_.bps = mppEncData_.width * mppEncData_.height / 8 * mppEncData_.fps;
    mppEncData_.bps = g_param->sceneCameraConfig.bps;
    //mppEncData_.bps = 1024*6;
    mppEncData_.prep_cfg.change        = MPP_ENC_PREP_CFG_CHANGE_INPUT |
                                         MPP_ENC_PREP_CFG_CHANGE_ROTATION |
                                         MPP_ENC_PREP_CFG_CHANGE_FORMAT;
    mppEncData_.prep_cfg.width         = mppEncData_.width;
    mppEncData_.prep_cfg.height        = mppEncData_.height;
    mppEncData_.prep_cfg.hor_stride    = mppEncData_.hor_stride;
    mppEncData_.prep_cfg.ver_stride    = mppEncData_.ver_stride;
    mppEncData_.prep_cfg.format        = mppEncData_.fmt;
    mppEncData_.prep_cfg.rotation      = MPP_ENC_ROT_0;
    ret = mppEncData_.mpi->control(mppEncData_.ctx, MPP_ENC_SET_PREP_CFG, &mppEncData_.prep_cfg);
    if (ret) {
        qDebug() << "mpi control enc set prep cfg failed";
		goto MPP_INIT_OUT;
	}

    // configure coding rate, quality, coding mode
    mppEncData_.rc_cfg.change  = MPP_ENC_RC_CFG_CHANGE_ALL;
    mppEncData_.rc_cfg.rc_mode = MPP_ENC_RC_MODE_AVBR;
    //mppEncData_.rc_cfg.rc_mode = MPP_ENC_RC_MODE_CBR;
    mppEncData_.rc_cfg.quality = MPP_ENC_RC_QUALITY_MEDIUM;

    mppEncData_.rc_cfg.qp_init = 21;
    mppEncData_.rc_cfg.qp_max = 40;
    mppEncData_.rc_cfg.qp_min = 10;
    mppEncData_.rc_cfg.qp_max_i = 40;
    mppEncData_.rc_cfg.qp_min_i = 10;
    mppEncData_.rc_cfg.qp_delta_ip = 2;


    if (mppEncData_.rc_cfg.rc_mode == MPP_ENC_RC_MODE_CBR) {
		/* constant bitrate has very small bps range of 1/16 bps */
        mppEncData_.rc_cfg.bps_target   = mppEncData_.bps;
        mppEncData_.rc_cfg.bps_max      = mppEncData_.bps * 17 / 16;
        mppEncData_.rc_cfg.bps_min      = mppEncData_.bps * 15 / 16;
	}
    else if (mppEncData_.rc_cfg.rc_mode ==  MPP_ENC_RC_MODE_VBR) {
        if (mppEncData_.rc_cfg.quality == MPP_ENC_RC_QUALITY_CQP) {
			/* constant QP does not have bps */
            mppEncData_.rc_cfg.bps_target   = -1;
            mppEncData_.rc_cfg.bps_max      = -1;
            mppEncData_.rc_cfg.bps_min      = -1;
		}
        else {
			/* variable bitrate has large bps range */
            mppEncData_.rc_cfg.bps_target   = mppEncData_.bps;
            mppEncData_.rc_cfg.bps_max      = mppEncData_.bps * 17 / 16;
            mppEncData_.rc_cfg.bps_min      = mppEncData_.bps * 1 / 16;
		}
	}
    if (mppEncData_.rc_cfg.rc_mode == MPP_ENC_RC_MODE_AVBR) {
        /* constant bitrate has very small bps range of 1/16 bps */
        mppEncData_.rc_cfg.bps_target   = mppEncData_.bps;
        mppEncData_.rc_cfg.bps_max      = mppEncData_.bps * 17 / 16;
        mppEncData_.rc_cfg.bps_min      = mppEncData_.bps * 1 / 16;
    }

	/* fix input / output frame rate */
    mppEncData_.rc_cfg.fps_in_flex      = 0;
    mppEncData_.rc_cfg.fps_in_num       = mppEncData_.fps;
    mppEncData_.rc_cfg.fps_in_denorm    = 1;
    mppEncData_.rc_cfg.fps_out_flex     = 0;
    mppEncData_.rc_cfg.fps_out_num      = mppEncData_.fps;
    mppEncData_.rc_cfg.fps_out_denorm   = 1;
    mppEncData_.rc_cfg.gop              = mppEncData_.gop;
    mppEncData_.rc_cfg.skip_cnt         = 0;

    ret = mppEncData_.mpi->control(mppEncData_.ctx, MPP_ENC_SET_RC_CFG, &mppEncData_.rc_cfg);
    if (ret) {
		printf("mpi control enc set rc cfg failed ret %d\n", ret);
		goto MPP_INIT_OUT;
	}


    // configure some parameters which has to do with H264
    mppEncData_.codec_cfg.coding = mppEncData_.type;
    mppEncData_.codec_cfg.h264.change = MPP_ENC_H264_CFG_CHANGE_PROFILE |
            MPP_ENC_H264_CFG_CHANGE_ENTROPY |
            MPP_ENC_H264_CFG_CHANGE_TRANS_8x8;
    /*
     * H.264 profile_idc parameter
     * 66  - Baseline profile
     * 77  - Main profile
     * 100 - High profile
     */
    mppEncData_.codec_cfg.h264.profile  = 100;
    /*
     * H.264 level_idc parameter
     * 10 / 11 / 12 / 13    - qcif@15fps / cif@7.5fps / cif@15fps / cif@30fps
     * 20 / 21 / 22         - cif@30fps / half-D1@@25fps / D1@12.5fps
     * 30 / 31 / 32         - D1@25fps / 720p@30fps / 720p@60fps
     * 40 / 41 / 42         - 1080p@30fps / 1080p@30fps / 1080p@60fps
     * 50 / 51 / 52         - 4K@30fps
     */
    mppEncData_.codec_cfg.h264.level    = 40;
    mppEncData_.codec_cfg.h264.entropy_coding_mode  = 1;
    mppEncData_.codec_cfg.h264.cabac_init_idc  = 0;
    mppEncData_.codec_cfg.h264.transform8x8_mode = 1;
    
    ret = mppEncData_.mpi->control(mppEncData_.ctx, MPP_ENC_SET_CODEC_CFG, &mppEncData_.codec_cfg);
    if (ret) {
		printf("mpi control enc set codec cfg failed ret %d\n", ret);
		goto MPP_INIT_OUT;
	}

	/* optional */
    mppEncData_.sei_mode = MPP_ENC_SEI_MODE_DISABLE;
    ret = mppEncData_.mpi->control(mppEncData_.ctx, MPP_ENC_SET_SEI_CFG, &mppEncData_.sei_mode);
    if (ret) {
		printf("mpi control enc set sei cfg failed ret %d\n", ret);
		goto MPP_INIT_OUT;
	}


    // get extra info of H264 encoding
    if (mppEncData_.type == MPP_VIDEO_CodingAVC) {
        MppPacket packet = NULL;
        ret = mppEncData_.mpi->control(mppEncData_.ctx, MPP_ENC_GET_EXTRA_INFO, &packet);
        if (ret) {
            printf("mpi control enc get extra info failed\n");
            goto MPP_INIT_OUT;
        }

        /* get sps/pps for H.264 */
        if (packet) {
            unsigned char *ptr = static_cast<unsigned char*>(mpp_packet_get_pos(packet));
            size_t len = mpp_packet_get_length(packet);

            parseExtraInfo(ptr, len);
            packet = NULL;
        }
    }

	return;

MPP_INIT_OUT:

    if (mppEncData_.ctx) {
        mpp_destroy(mppEncData_.ctx);
        mppEncData_.ctx = NULL;
    }

    if (mppEncData_.frm_buf) {
        mpp_buffer_put(mppEncData_.frm_buf);
        mppEncData_.frm_buf = NULL;
    }

    printf("init mpp failed!\n");
}


bool VideoEncode::processImage(uint8_t *p)
{
	MPP_RET ret = MPP_OK;
    MppFrame frame = NULL;
    MppPacket packet = NULL;

    void *buf = mpp_buffer_get_ptr(mppEncData_.frm_buf);

    paddingFrame((uint8_t *)buf, p, mppEncData_.width, mppEncData_.height,
                  mppEncData_.hor_stride, mppEncData_.ver_stride, mppEncData_.fmt);

    ret = mpp_frame_init(&frame);
    if (ret) {
    	printf("mpp_frame_init failed\n");
    	return true;
    }

    //set encoding format
    mpp_frame_set_width(frame, mppEncData_.width);
    mpp_frame_set_height(frame, mppEncData_.height);
    mpp_frame_set_hor_stride(frame, mppEncData_.hor_stride);
    mpp_frame_set_ver_stride(frame, mppEncData_.ver_stride);
    mpp_frame_set_fmt(frame, mppEncData_.fmt);
    mpp_frame_set_buffer(frame, mppEncData_.frm_buf);
    mpp_frame_set_eos(frame, mppEncData_.frm_eos);

    long tic = QDateTime::currentMSecsSinceEpoch();
    ret = mppEncData_.mpi->encode_put_frame(mppEncData_.ctx, frame);
    if (ret) {
        qDebug() << "mpp encode put frame failed.";
    	return true;
    }
    long toc = QDateTime::currentMSecsSinceEpoch();
    qDebug() << "actual encoding cost: " << toc - tic;


    // get packet after encoding
    ret = mppEncData_.mpi->encode_get_packet(mppEncData_.ctx, &packet);
    if (ret) {
        qDebug() << "mpp encode get packet failed\n";
    	return true;
    }

    if (packet) {
        unsigned char *pNalu   = static_cast<unsigned char*>(mpp_packet_get_pos(packet));
    	size_t len  = mpp_packet_get_length(packet);

        mppEncData_.pkt_eos = mpp_packet_get_eos(packet);


        // judge if the NALU is IDR frame

        while(g_param->queueH264Packets.size() > 20){
            g_param->queueH264Packets.pop();
        }

        if (repeatSpsPps_ && (pNalu[4] & 0x1F) == 5) {
            g_param->queueH264Packets.push(
                        cvBuffer(spsNaluAddr_ + 4, spsNaluSize_ - 4,
                               BufferFormat::H264,
                               mppEncData_.width, mppEncData_.height));

            qDebug() << "\n\n"<< "IDR sent SPS!!!" << spsNaluAddr_ << spsNaluSize_ << "\n\n";

            g_param->queueH264Packets.push(
                        cvBuffer(ppsNaluAddr_ + 4, ppsNaluSize_ - 4,
                               BufferFormat::H264,
                               mppEncData_.width, mppEncData_.height));

            qDebug() << "\n\n"<< "IDR sent PPS!!!" << ppsNaluAddr_ << ppsNaluSize_ << "\n\n";
        }

        // remove start code as the request of live555
//        auto h264Packet = cvBuffer(pNalu + 4, len - 4,
//                                 BufferFormat::H264,
//                                 mppEncData_.width, mppEncData_.height);
//        g_param->queueH264Packets.push(std::move(h264Packet));
        // do not remove start code because send to TCP //

        qDebug() <<" pps" << ppsNaluAddr_ << ppsNaluSize_ << ppsNaluAddr_+4 << ppsNaluAddr_+5 << ppsNaluAddr_+6;
        qDebug() << "pnalu" << pNalu[4] << pNalu[5] << pNalu[6] << pNalu[7];
        auto h264Packet = cvBuffer(pNalu, len,
                                 BufferFormat::H264,
                                 mppEncData_.width, mppEncData_.height);
        g_param->queueH264Packets.push(std::move(h264Packet));

        qDebug() << "encoded data length: " << len <<g_param->queueH264Packets.size();

    	mpp_packet_deinit(&packet);
        mpp_frame_deinit(&frame);

        mppEncData_.stream_size += len;
        mppEncData_.frame_count++;

        if (mppEncData_.pkt_eos)
    	{
            qDebug() << "found last packet.";
    	}
    }

    if (mppEncData_.num_frames && mppEncData_.frame_count >= mppEncData_.num_frames)
    {
        qDebug() << "Encode max " << mppEncData_.frame_count << " frames";
    	return false;
    }
    if (mppEncData_.frm_eos && mppEncData_.pkt_eos)
    	return false;
    return true;
}

bool VideoEncode::processImageTest(uint8_t *p)
{
    MPP_RET ret = MPP_OK;
    MppFrame frame = NULL;
    MppPacket packet = NULL;

    void *buf = mpp_buffer_get_ptr(mppEncData_.frm_buf);

    paddingFrame((uint8_t *)buf, p, mppEncData_.width, mppEncData_.height,
                  mppEncData_.hor_stride, mppEncData_.ver_stride, mppEncData_.fmt);

    ret = mpp_frame_init(&frame);
    if (ret) {
        printf("mpp_frame_init failed\n");
        return true;
    }

    //set encoding format
    mpp_frame_set_width(frame, mppEncData_.width);
    mpp_frame_set_height(frame, mppEncData_.height);
    mpp_frame_set_hor_stride(frame, mppEncData_.hor_stride);
    mpp_frame_set_ver_stride(frame, mppEncData_.ver_stride);
    mpp_frame_set_fmt(frame, mppEncData_.fmt);
    mpp_frame_set_buffer(frame, mppEncData_.frm_buf);
    mpp_frame_set_eos(frame, mppEncData_.frm_eos);

    mpp_packet_init_with_buffer(&packet, mppEncData_.frm_buf);
    //MppMeta meta = mpp_frame_get_meta(frame);
    mpp_packet_set_length(packet, 0);
    //mpp_meta_set_packet(meta, KEY_OUTPUT_PACKET, packet);
    //mpp_meta_set_buffer(meta, KEY_MOTION_INFO, p->md_info);
    long tic = QDateTime::currentMSecsSinceEpoch();
    ret = mppEncData_.mpi->encode_put_frame(mppEncData_.ctx, frame);
    if (ret) {
        qDebug() << "mpp encode put frame failed.";
        return true;
    }
    mpp_frame_deinit(&frame);
    long toc = QDateTime::currentMSecsSinceEpoch();
//    qDebug() << "actual encoding cost: " << toc - tic;


    // get packet after encoding
    ret = mppEncData_.mpi->encode_get_packet(mppEncData_.ctx, &packet);
    if (ret) {
        qDebug() << "mpp encode get packet failed\n";
        return true;
    }

    if (packet) {
        unsigned char *pNalu   = static_cast<unsigned char*>(mpp_packet_get_pos(packet));
        size_t len  = mpp_packet_get_length(packet);

        mppEncData_.pkt_eos = mpp_packet_get_eos(packet);


        // judge if the NALU is IDR frame

        while(g_param->queueH264Packets.size() > 20){
            g_param->queueH264Packets.pop();
        }

        //qDebug() << "Judge IDR " << repeatSpsPps_ << (pNalu[4] & 0x1F);
        if (repeatSpsPps_ && (pNalu[4] & 0x1F) == 5) {
            g_param->queueH264Packets.push(
                        cvBuffer(spsNaluAddr_, spsNaluSize_,
                               BufferFormat::H264,
                               mppEncData_.width, mppEncData_.height));

            qDebug() << "\n"<< "IDR sent SPS" << spsNaluAddr_ << spsNaluSize_ << "\n";

            g_param->queueH264Packets.push(
                        cvBuffer(ppsNaluAddr_, ppsNaluSize_,
                               BufferFormat::H264,
                               mppEncData_.width, mppEncData_.height));

            qDebug() << "IDR sent PPS" << ppsNaluAddr_ << ppsNaluSize_ << "\n";
        }

        // remove start code as the request of live555
//        auto h264Packet = cvBuffer(pNalu + 4, len - 4,
//                                 BufferFormat::H264,
//                                 mppEncData_.width, mppEncData_.height);
        // do not remove start code because send to TCP //
        //qDebug() <<" sps " << spsNaluAddr_ << spsNaluSize_ << spsNaluAddr_[0] << spsNaluAddr_[1] << spsNaluAddr_[2] << spsNaluAddr_[3]
                //<< spsNaluAddr_[4] << spsNaluAddr_[5] << spsNaluAddr_[6] << spsNaluAddr_[7];
        //qDebug() <<" pps " << ppsNaluAddr_ << ppsNaluSize_ << ppsNaluAddr_[0] << ppsNaluAddr_[1] << ppsNaluAddr_[2] << ppsNaluAddr_[3]
                //<< ppsNaluAddr_[4] << ppsNaluAddr_[5] << ppsNaluAddr_[6] << ppsNaluAddr_[7];
        //qDebug() << "pnalu" << pNalu << pNalu[0] << pNalu[1] << pNalu[2] << pNalu[3] << pNalu[4] << pNalu[5] << pNalu[6] << pNalu[7];
//        for(uint i=0;i <50;i++){
//            qDebug() << pNalu[i];
//        }
        //qDebug() << "\nPnalu4 and 1f "<<(pNalu[4] & 0x1F);
        auto h264Packet = cvBuffer(pNalu, len,
                                 BufferFormat::H264,
                                 mppEncData_.width, mppEncData_.height);
        g_param->queueH264Packets.push(std::move(h264Packet));

        qDebug() << "encoded H264 data length: " << len ;

        mpp_packet_deinit(&packet);
        mpp_frame_deinit(&frame);

        mppEncData_.stream_size += len;
        mppEncData_.frame_count++;

        if (mppEncData_.pkt_eos)
        {
            qDebug() << "found last packet.";
        }
    }

    if (mppEncData_.num_frames && mppEncData_.frame_count >= mppEncData_.num_frames)
    {
        qDebug() << "Encode max " << mppEncData_.frame_count << " frames";
        return false;
    }
    if (mppEncData_.frm_eos && mppEncData_.pkt_eos)
        return false;
    return true;
}



void VideoEncode::paddingFrame(uint8_t *buf, uint8_t *image, uint32_t width, uint32_t height,
                       uint32_t hor_stride, uint32_t ver_stride, MppFrameFormat fmt) {

    // if already aligned, just copy
    if (width == hor_stride && height == ver_stride) {
        //qDebug() << "already aligned";
        memcpy(buf, image, width * height * mppEncData_.bpp);
    }


    MPP_RET ret = MPP_OK;
    uint32_t row = 0;
    uint8_t *buf_y = buf;
    uint8_t *buf_u = buf_y + hor_stride * ver_stride; // NOTE: diff from gen_yuv_image
    uint8_t *buf_v = buf_u + hor_stride * ver_stride / 4; // NOTE: diff from gen_yuv_image

    switch (fmt) {
    case MPP_FMT_YUV420SP : {
        for (row = 0; row < height; row++) {
			memcpy(buf_y + row * hor_stride, image,width);
			image += width;
        }
        for (row = 0; row < height / 2; row++) {
			memcpy(buf_u + row * hor_stride, image, width);
			image += width;
        }
    } break;

    case MPP_FMT_YUV420P : {
        for (row = 0; row < height; row++) {
			memcpy(buf_y + row * hor_stride, image, width);
			image += width;
        }
        for (row = 0; row < height / 2; row++) {
            memcpy(buf_u + row * hor_stride/2, image, width/2);
			image += width/2;
        }
        for (row = 0; row < height / 2; row++) {
			memcpy(buf_v + row * hor_stride/2, image, width/2);
			image += width/2;
        }
    } break;

    case MPP_FMT_ARGB8888 : {
        for (row = 0; row < height; row++) {
			memcpy(buf_y + row * hor_stride*4, image, width*4);
			image += width*4;
        }
    } break;

    case MPP_FMT_RGB888 :
    case MPP_FMT_BGR888 : {
//        cv::Mat mat = cv::Mat(1080, 1920, CV_8UC3, image, cv::Mat::AUTO_STEP);
//        cv::imwrite("mppImg.jpg", mat);

        for (row = 0; row < height; row++) {
            memcpy(buf_y + row * hor_stride * 3, image, width * 3);
            image += width * 3;
        }

//        cv::Mat mat = cv::Mat(1088, 1920, CV_8UC3, buf, cv::Mat::AUTO_STEP);
//        cv::imwrite("mppImg.jpg", mat);
    } break;

    case MPP_FMT_YUV422_YUYV :
    case MPP_FMT_YUV422_UYVY : {
        for (row = 0; row < height; row++) {
			memcpy(buf_y + row * hor_stride, image, width*2);
			image += width*2;
        }
    } break;

    default : {
        qDebug() << "Image format not supported.";
        ret = MPP_ERR_VALUE;
    } break;
    }
}



void VideoEncode::startThread() {
    if (!m_threadActive) {
        m_threadActive = true;
        qDebug() << "Start Video Encode!";
        start();
    }
}

void VideoEncode::stopThread()
{
    if (m_threadActive) {
        m_threadActive = false;
        qDebug() << "Stop Video Encode!";
        terminate();
    }
}


//void VideoEncode::stop() {

//    if (threadActive_) {
//        threadActive_ = false;
//        sink_->wake();
//    }
//}


//void VideoEncode::quit() {
//    if (threadActive_) {
//        terminate_ = true;
//        stop();
//    } else {
//        emitQuitSignal();
//    }
//}



void VideoEncode::run() {
    long tic = QDateTime::currentMSecsSinceEpoch();
    long toc = tic;

    cvBuffer frame;
    while (m_threadActive) {
        if(g_param->queueYUV420Images.size()) {

            m_yuvBuffer = g_param->queueYUV420Images.front();
            g_param->queueYUV420Images.pop();
            //frame = m_yuvBuffer.copy();

            //qDebug() << "Encoding Image Size" << m_yuvBuffer.size();
            tic = QDateTime::currentMSecsSinceEpoch();
            processImageTest(m_yuvBuffer.data());
            toc = QDateTime::currentMSecsSinceEpoch();

            //qDebug() << "encoding cost: " << toc - tic;
        }

        // wait until queue is not empty or task is finished
        //sink_->wait();
        QThread::msleep(5);
    }

//    if (terminate_) {
//        emitQuitSignal();
//    }
}

