QT -= gui

TEMPLATE = lib
DEFINES += QTLIBH264_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Buffer.cpp \
    H264LiveServerMediaSubsession.cpp \
    LiveFramedSource.cpp \
    MainSlaver.cpp \
    RtspParameter.cpp \
    RtspStream.cpp \
    VideoConvert.cpp \
    VideoEncode.cpp \
    qtlibh264.cpp

HEADERS += \
    Buffer.h \
    H264LiveServerMediaSubsession.h \
    LiveFramedSource.h \
    MainSlaver.h \
    QtLibH264_global.h \
    RtspParameter.h \
    RtspStream.h \
    VideoConvert.h \
    VideoEncode.h \
    qtlibh264.h

INCLUDEPATH +=  ../include  \
                /usr/include  \
                /usr/include/opencv4 \
                /usr/include/opencv4/opencv2   \
                /usr/include/opencv4/opencv2/tracking   \
                /usr/local/cuda-11/include \
                /home/orangepi/debug/xdma_app_1.0.3-1.0 \
                /usr/include/c++/11/ \
                /usr/include/aarch64-linux-gnu/c++/11/ \
                /usr/include/aarch64-linux-gnu/ \
                /usr/local/include/liveMedia \
                /usr/local/include/BasicUsageEnvironment \
                /usr/local/include/groupsock \
                /usr/local/include/UsageEnvironment

LIBS += /usr/lib/aarch64-linux-gnu/libopencv_video.so  \
        /usr/lib/aarch64-linux-gnu/libopencv_objdetect.so \
        /usr/lib/aarch64-linux-gnu/libopencv_ml.so  \
        /usr/lib/aarch64-linux-gnu/libopencv_core.so \
        /usr/lib/aarch64-linux-gnu/libopencv_features2d.so  \
        /usr/lib/aarch64-linux-gnu/libopencv_imgproc.so \
        /usr/lib/aarch64-linux-gnu/libopencv_highgui.so \
        /usr/lib/aarch64-linux-gnu/libopencv_flann.so   \
        /usr/lib/aarch64-linux-gnu/libopencv_calib3d.so   \
        /usr/lib/aarch64-linux-gnu/libopencv_videoio.so   \
        /usr/lib/aarch64-linux-gnu/libopencv_video.so   \
        /usr/lib/aarch64-linux-gnu/libopencv_videostab.so   \
        /usr/lib/aarch64-linux-gnu/libopencv_dnn.so   \
        /usr/lib/aarch64-linux-gnu/libopencv_imgcodecs.so   \
        /usr/lib/aarch64-linux-gnu/libopencv_* \
        -L/usr/lib/aarch64-linux-gnu/ \
        -L/usr/lib/gcc/aarch64-linux-gnu/11/ \
        /usr/lib/aarch64-linux-gnu/librga.so \
        /usr/local/lib/libliveMedia.so \
        /usr/local/lib/libBasicUsageEnvironment.so \
        /usr/local/lib/libgroupsock.so \
        /usr/local/lib/libUsageEnvironment.so

unix:!macx: LIBS += -L$$PWD/../mpp/ -lrockchip_mpp

INCLUDEPATH += $$PWD/../mpp/rockchip
DEPENDPATH += $$PWD/../mpp/rockchip


# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

