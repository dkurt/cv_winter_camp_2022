#include "algo.hpp"

void roberts_reference(const cv::Mat& src, cv::Mat& dst) {
    CV_Assert(src.type() == CV_8UC1);
    Mat bsrc;
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_16SC1);
    for (int y = 0; y < dst.rows; ++y)
        for (int x = 0; x < dst.cols; ++x) {
            int dx = bsrc.at<uchar>(y, x) - bsrc.at<uchar>(y + 1, x + 1);
            int dy = bsrc.at<uchar>(y, x + 1) - bsrc.at<uchar>(y + 1, x);
            dst.at<uint16_t>(y, x) = dx * dx + dy * dy;
        }
}

void roberts_parallel(const cv::Mat& src, cv::Mat& dst) {
    CV_Assert(src.type() == CV_8UC1);
    Mat bsrc;
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_16SC1);
    parallel_for_(Range(0, src.rows), [&](const Range& range) {
        for (int y = range.start; y < range.end; ++y)
            for (int x = 0; x < dst.cols; ++x) {
                int dx = bsrc.at<uchar>(y, x) - bsrc.at<uchar>(y + 1, x + 1);
                int dy = bsrc.at<uchar>(y, x + 1) - bsrc.at<uchar>(y + 1, x);
                dst.at<uint16_t>(y, x) = dx * dx + dy * dy;
            }
    });
}

#include <opencv2/core/hal/intrin.hpp>

void roberts_parallel_vec(const Mat& src, Mat& dst) {
    Mat bsrc(src.size(), CV_8U);
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_16SC1);
    parallel_for_(Range(0, src.rows), [&](const Range& range) {
        for (int y = range.start; y < range.end; ++y) {
            const uint8_t* psrc0 = bsrc.ptr(y);
            const uint8_t* psrc1 = bsrc.ptr(y + 1);
            uint16_t* pdst = dst.ptr<uint16_t>(y);
            int x = 0;
            for (; x <= dst.cols - v_int16::nlanes; x += v_int16::nlanes) {
                v_int16 dx = v_reinterpret_as_s16(vx_load_expand(psrc0 + x)) - v_reinterpret_as_s16(vx_load_expand(psrc1 + x + 1));
                v_int16 dy = v_reinterpret_as_s16(vx_load_expand(psrc0 + x + 1)) - v_reinterpret_as_s16(vx_load_expand(psrc1 + x));
                v_uint16 dx2 = v_reinterpret_as_u16(v_mul_wrap(dx, dx));
                v_uint16 dy2 = v_reinterpret_as_u16(v_mul_wrap(dy, dy));
                v_uint16 res = v_add_wrap(dx2, dy2);
                v_store(pdst + x, res);
            }
            for (; x < dst.cols; ++x) {
                int dx = psrc0[x] - psrc1[x + 1];
                int dy = psrc0[x + 1] - psrc1[x];
                pdst[x] = dx * dx + dy * dy;
            }
        }
    });
}

void roberts_parallel_vec_inplace(const Mat& src, Mat& dst) {
    dst.create(src.size(), CV_16SC1);
    parallel_for_(Range(1, src.rows), [&](const Range& range) {
        for (int y = range.start; y < range.end; ++y) {
            const uint8_t* psrc0 = src.ptr(y - 1);
            const uint8_t* psrc1 = src.ptr(y);
            uint16_t* pdst = dst.ptr<uint16_t>(y);
            int x = 1;
            for (; x <= dst.cols - v_int16::nlanes; x += v_int16::nlanes) {
                v_int16 dx = v_reinterpret_as_s16(vx_load_expand(psrc0 + x - 1)) - v_reinterpret_as_s16(vx_load_expand(psrc1 + x));
                v_int16 dy = v_reinterpret_as_s16(vx_load_expand(psrc0 + x)) - v_reinterpret_as_s16(vx_load_expand(psrc1 + x - 1));
                v_uint16 dx2 = v_reinterpret_as_u16(v_mul_wrap(dx, dx));
                v_uint16 dy2 = v_reinterpret_as_u16(v_mul_wrap(dy, dy));
                v_uint16 res = v_add_wrap(dx2, dy2);
                v_store(pdst + x, res);
            }
            for (; x < dst.cols; ++x) {
                int dx = psrc0[x - 1] - psrc1[x];
                int dy = psrc0[x] - psrc1[x - 1];
                pdst[x] = dx * dx + dy * dy;
            }
        }
    });

    // First row
    dst.at<uint16_t>(0, 0) = 0;
    for (int x = 1; x < dst.cols; ++x) {
        int dx = src.at<uchar>(0, x - 1) - src.at<uchar>(0, x);
        int dy = src.at<uchar>(0, x) - src.at<uchar>(0, x - 1);
        dst.at<uint16_t>(0, x) = dx * dx + dy * dy;
    }

    // First column
    for (int y = 1; y < dst.rows; ++y) {
        int dx = src.at<uchar>(y - 1, 0) - src.at<uchar>(y, 0);
        int dy = src.at<uchar>(y, 0) - src.at<uchar>(y - 1, 0);
        dst.at<uint16_t>(y, 0) = dx * dx + dy * dy;
    }
}
