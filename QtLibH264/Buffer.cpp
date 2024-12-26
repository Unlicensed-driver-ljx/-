#include "Buffer.h"
#include <QDebug>

/**
 * @brief Buffer definition
 */

cvBuffer::cvBuffer(): data_(nullptr),
                  size_(0),
                  format_(BufferFormat::UNKNOWN),
                  width_(0),
                  height_(0),
                  refPtr_(nullptr){};


cvBuffer::cvBuffer(int size, BufferFormat format, int imgWidth, int imgHeight):
               data_(nullptr),
               size_(size),
               format_(format),
               width_(imgWidth),
               height_(imgHeight),
               refPtr_(nullptr) {

    create(size);
}


cvBuffer::cvBuffer(unsigned char* data, int size,
               BufferFormat format,
               int imgWidth, int imgHeight):
               cvBuffer(size, format, imgWidth, imgHeight) {

    memcpy(data_, data, size);
}


cvBuffer::cvBuffer(const cvBuffer& rhs) :
    data_(rhs.data_),
    size_(rhs.size_),
    format_(rhs.format_),
    width_(rhs.width_),
    height_(rhs.height_),
    refPtr_(rhs.refPtr_) {

    addRef();
}


cvBuffer::cvBuffer(cvBuffer&& rhs)  noexcept :
    data_(rhs.data_),
    size_(rhs.size_),
    format_(rhs.format_),
    width_(rhs.width_),
    height_(rhs.height_),
    refPtr_(rhs.refPtr_) {

    rhs.data_ = nullptr;
    rhs.refPtr_ = nullptr;
}


cvBuffer::cvBuffer(cv::Mat& mat, BufferFormat fmt) : cvBuffer() {
    extractFromMat(mat, fmt);
}


void cvBuffer::create(int size) {
    data_ = new unsigned char[size];
    refPtr_ = new int(1);
}


void cvBuffer::addRef() {
    if (refPtr_) ++(*refPtr_);
}



cvBuffer::~cvBuffer() {
    release();
}


cvBuffer& cvBuffer::operator=(cvBuffer&& rhs) noexcept {
    if (this != &rhs) {
        release();
        data_ = rhs.data_;
        size_ = rhs.size_;
        format_ = rhs.format_;
        width_ = rhs.width_;
        height_ = rhs.height_;
        refPtr_ = rhs.refPtr_;

        rhs.data_ = nullptr;
        rhs.refPtr_ = nullptr;
    }
    return *this;
}



cvBuffer& cvBuffer::operator=(const cvBuffer& rhs) {
    if (this != &rhs) {
        release();

        refPtr_ = rhs.refPtr_;
        addRef();

        data_ = rhs.data_;
        size_ = rhs.size_;
        format_ = rhs.format_;
        width_ = rhs.width_;
        height_ = rhs.height_;
    }
    return *this;
}


cvBuffer cvBuffer::copy() const {
    return cvBuffer(data_, size_, format_, width_, height_);
}


cv::Mat cvBuffer::toMat() const {
    return cv::Mat(height_, width_, CV_8UC3, data_);
}


void cvBuffer::extractFromMat(cv::Mat& mat, BufferFormat fmt) {
    release();

    // data transfer
    data_ = mat.data;
    format_ = fmt;

    switch (fmt) {
    case BufferFormat::BGR :
        width_ = mat.cols;
        height_ = mat.rows;
        size_ = width_ * height_ * 3;
        //qDebug() <<"BGR"<< width_ << height_ << size_;
        break;

    case BufferFormat::NV16 :
        width_ = mat.cols;
        height_ = mat.rows / 2;
        size_ = width_ * height_ * 2;
        break;

    case BufferFormat::I420 :
        width_ = mat.cols;
        height_ = mat.rows * 2 / 3;
        size_ = width_ * height_ * 1.5;
        //qDebug() <<"I420"<< width_ << height_ << size_;
        break;

    default:
        qDebug() << "Unsupported color type from cv::Mat";
        exit(0);
    }

    //refPtr_ = new int(1);

//    // reset cv::Mat data
//    mat.data = nullptr;
//    mat.u->origdata = nullptr;
}


unsigned char* cvBuffer::data() const {
    return data_;
}

int cvBuffer::size() const {
    return size_;
}

BufferFormat cvBuffer::format() const {
    return format_;
}

int cvBuffer::width() const {
    return width_;
}

int cvBuffer::height() const {
    return height_;
}

void cvBuffer::release() {
    if (refPtr_ && --(*refPtr_) == 0) {
        delete[] data_;
        delete refPtr_;
        data_ = nullptr;
    }

    refPtr_ = nullptr;
}


void cvBuffer::setData(unsigned char* addr) {
    data_ = addr;
}


/**
 * @brief LinkedcvBuffer definition
 */

LinkedBuffer::LinkedBuffer() {};

LinkedBuffer::~LinkedBuffer() {}

LinkedBuffer::LinkedBuffer(cvBuffer&& yuv, cvBuffer&& bgr) :
    yuv_(std::move(yuv)), bgr_(std::move(bgr)) {};

LinkedBuffer::LinkedBuffer(const LinkedBuffer& rhs) {
    yuv_ = rhs.yuv_;
    bgr_ = rhs.bgr_;
//    yoloResults_ = rhs.yoloResults_;
}

LinkedBuffer::LinkedBuffer(LinkedBuffer&& rhs) {
    yuv_ = std::move(rhs.yuv_);
    bgr_ = std::move(rhs.bgr_);
//    yoloResults_ = std::move(rhs.yoloResults_);
}

LinkedBuffer& LinkedBuffer::operator=(const LinkedBuffer& rhs) {
    yuv_ = rhs.yuv_;
    bgr_ = rhs.bgr_;
//    yoloResults_ = rhs.yoloResults_;
    return *this;
};

LinkedBuffer& LinkedBuffer::operator=(LinkedBuffer&& rhs) {
    yuv_ = std::move(rhs.yuv_);
    bgr_ = std::move(rhs.bgr_);
//    yoloResults_ = std::move(rhs.yoloResults_);
    return *this;
}


cvBuffer& LinkedBuffer::bgr() {
    return bgr_;
}

cvBuffer& LinkedBuffer::yuv() {
    return yuv_;
}

//std::vector<YoloResult>& LinkedBuffer::yoloResults() {
//    return yoloResults_;
//}


//BoxEventOutput& LinkedBuffer::yoloResults() {
//    return yoloResults_;
//}

