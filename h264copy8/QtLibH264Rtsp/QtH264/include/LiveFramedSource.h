#ifndef LIVEFRAMEDSOURCE_H
#define LIVEFRAMEDSOURCE_H

#include "BasicUsageEnvironment.hh"
#include "UsageEnvironment.hh"
#include "liveMedia.hh"

#include "RtspParameter.h"

class LiveFramedSource : public FramedSource {
public:

    LiveFramedSource(UsageEnvironment& env);

protected:
    virtual void doGetNextFrame();

    virtual unsigned int maxFrameSize() const;


private:
//    inline void reset(void);
//    inline void registerOutputInterest(void);
//    inline void convert_to_OutputBuffer(void);

//    unsigned char* fOutputBuffer_;
//    unsigned fMaxOfFrameToSend_;
//    unsigned fTotOfFrameToSend_;

    RtspParameter *g_param;

    bool sps_sent_;

    cvBuffer m_h264Packet;

};



#endif // LIVEFRAMEDSOURCE_H
