#include "algo.hpp"

void bgr2gray_reference(const cv::Mat& src, cv::Mat& dst) {
    const int w = src.cols;
    const int h = src.rows;
    dst.create(h, w, CV_8UC1);

    const uint8_t* src_data = src.ptr<uint8_t>();
    uint8_t* dst_data = dst.ptr<uint8_t>();
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            uint8_t b = src_data[x * 3];
            uint8_t g = src_data[x * 3 + 1];
            uint8_t r = src_data[x * 3 + 2];
            dst_data[x] = static_cast<uint8_t>(0.114f * b +
                                               0.587f * g +
                                               0.299f * r);
        }
        dst_data += w;
        src_data += w * 3;
    }
}

void bgr2gray_u8(const cv::Mat& src, cv::Mat& dst) {
    const int w = src.cols;
    const int h = src.rows;
    dst.create(h, w, CV_8UC1);

    const uint8_t* src_data = src.ptr<uint8_t>();
    uint8_t* dst_data = dst.ptr<uint8_t>();
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            uint8_t b = src_data[x * 3];
            uint8_t g = src_data[x * 3 + 1];
            uint8_t r = src_data[x * 3 + 2];
            dst_data[x] = (29 * b + 150 * g + 77 * r) >> 8;
        }
        dst_data += w;
        src_data += w * 3;
    }
}

// https://docs.opencv.org/master/d7/dff/tutorial_how_to_use_OpenCV_parallel_for_.html
void bgr2gray_u8_parallel(const cv::Mat& src, cv::Mat& dst) {
    const int w = src.cols;
    const int h = src.rows;
    dst.create(h, w, CV_8UC1);

    parallel_for_(cv::Range(0, h), [&](const cv::Range& range) {
        const uint8_t* src_data = src.ptr<uint8_t>(range.start);
        uint8_t* dst_data = dst.ptr<uint8_t>(range.start);
        for (int y = range.start; y < range.end; ++y) {
            for (int x = 0; x < w; ++x) {
                uint8_t b = src_data[x * 3];
                uint8_t g = src_data[x * 3 + 1];
                uint8_t r = src_data[x * 3 + 2];
                dst_data[x] = (29 * b + 150 * g + 77 * r) >> 8;
            }
            dst_data += w;
            src_data += w * 3;
        }
    });
}
