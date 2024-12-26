#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>
#include <string>
#include "opencv2/opencv.hpp"

enum class BufferFormat {
    NV16,
    I420,
    BGR,
    H264,
    UNKNOWN
};


class cvBuffer {
public:
    cvBuffer();

    ~cvBuffer();

    cvBuffer(int size,
           BufferFormat format,
           int width, int height);

    cvBuffer(unsigned char* data, int size,
           BufferFormat format,
           int width, int height);

    cvBuffer(const cvBuffer& rhs);

    cvBuffer(cvBuffer&& rhs) noexcept;

    cvBuffer(cv::Mat& mat, BufferFormat fmt=BufferFormat::BGR);

    cvBuffer& operator=(cvBuffer&& rhs) noexcept;

    cvBuffer& operator=(const cvBuffer& rhs);

    cvBuffer copy() const;

    cv::Mat toMat() const;

    void extractFromMat(cv::Mat& mat, BufferFormat fmt=BufferFormat::BGR);

    unsigned char* data() const;

    int size() const;

    BufferFormat format() const;

    int width() const;

    int height() const;

private:
    void setData(unsigned char* addr);

    void create(int size);

    void addRef();

    void release();

private:
    unsigned char* data_;
    int size_;
    BufferFormat format_;
    int width_;
    int height_;
    int *refPtr_;
};


class LinkedBuffer {
public:
    LinkedBuffer();

    ~LinkedBuffer();

    LinkedBuffer(cvBuffer&& yuv, cvBuffer&& bgr);

    LinkedBuffer(const LinkedBuffer& rhs);

    LinkedBuffer(LinkedBuffer&& rhs);

    LinkedBuffer& operator=(const LinkedBuffer& rhs);

    LinkedBuffer& operator=(LinkedBuffer&& rhs);

    cvBuffer& yuv();

    cvBuffer& bgr();

//    std::vector<YoloResult>& yoloResults();
//    BoxEventOutput& yoloResults();


private:
    cvBuffer yuv_;
    cvBuffer bgr_;
//    std::vector<YoloResult> yoloResults_;
//    BoxEventOutput yoloResults_;
};


#endif // BUFFER_H
