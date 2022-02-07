#include "algo.hpp"

#include <opencv2/core/hal/intrin.hpp>


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
        for (int y = range.start; y < range.end; ++y) {
            for (int x = 0; x < dst.cols; ++x) {
                int dx = bsrc.at<uchar>(y, x) - bsrc.at<uchar>(y + 1, x + 1);
                int dy = bsrc.at<uchar>(y, x + 1) - bsrc.at<uchar>(y + 1, x);
                dst.at<uint16_t>(y, x) = dx * dx + dy * dy;
            }
        }
    });
}


void roberts_no_copy_parallel(const cv::Mat& src, cv::Mat& dst) {
    CV_Assert(src.type() == CV_8UC1);
    dst.create(src.size(), CV_16SC1);

    parallel_for_(Range(0, src.rows - 1), [&](const Range& range) {
        for (int y = range.start; y < range.end; ++y) {
            uchar const * const r0 = src.ptr(y);
            uchar const * const r1 = src.ptr(y + 1);
            uint16_t * const rd = dst.ptr<uint16_t>(y + 1);
            for (int x = 0; x + 1 < dst.cols; ++x) {
                int dx = r0[x] - r1[x + 1];
                int dy = r0[x + 1] - r1[x];
                rd[x + 1] = dx * dx + dy * dy;
            }
        }
    });

    dst.at<uint16_t>(0, 0) = 0;

    for (int x = 0; x + 1 < dst.cols; ++x) {
        int d = src.at<uchar>(0, x) - src.at<uchar>(0, x + 1);
        dst.at<uint16_t>(0, x + 1) = 2 * d * d;
    }

    for (int y = 0; y + 1 < dst.rows; ++y) {
        int d = src.at<uchar>(y, 0) - src.at<uchar>(y + 1, 0);
        dst.at<uint16_t>(y + 1, 0) = 2 * d * d;
    }
}


void process_row_vec(uchar const * const r0, uchar const * const r1,
                     uint16_t * const rdst, int const cols)
{
    int x = 0;
    int const step = v_uint16::nlanes;
    for (; x + step + 1 < cols; x += step) {
        v_int16 const r0c0 = v_reinterpret_as_s16(vx_load_expand(r0 + x));
        v_int16 const r0c1 = v_reinterpret_as_s16(vx_load_expand(r0 + x + 1));
        v_int16 const r1c0 = v_reinterpret_as_s16(vx_load_expand(r1 + x));
        v_int16 const r1c1 = v_reinterpret_as_s16(vx_load_expand(r1 + x + 1));
        v_int16 const dx = r0c0 - r1c1;
        v_int16 const dy = r0c1 - r1c0;
        v_int16 const res = v_add_wrap(v_mul_wrap(dx, dx), v_mul_wrap(dy, dy));
        v_store(rdst + x + 1, v_reinterpret_as_u16(res));
    }
    for (; x + 1 < cols; ++x) {
        int dx = r0[x] - r1[x + 1];
        int dy = r0[x + 1] - r1[x];
        rdst[x + 1] = dx * dx + dy * dy;
    }
}


void roberts_no_copy_parallel_vec(const cv::Mat& src, cv::Mat& dst) {
    CV_Assert(src.type() == CV_8UC1);
    dst.create(src.size(), CV_16SC1);

    parallel_for_(Range(0, src.rows - 1), [&](const Range& range) {
        for (int y = range.start; y < range.end; ++y) {
            uchar const * const r0 = src.ptr(y);
            uchar const * const r1 = src.ptr(y + 1);
            uint16_t * const rd = dst.ptr<uint16_t>(y + 1);
            process_row_vec(r0, r1, rd, dst.cols);
        }
    });

    dst.at<uint16_t>(0, 0) = 0;

    process_row_vec(src.ptr(0), src.ptr(0), dst.ptr<uint16_t>(0), dst.cols);

    for (int y = 0; y + 1 < dst.rows; ++y) {
        int d = src.at<uchar>(y, 0) - src.at<uchar>(y + 1, 0);
        dst.at<uint16_t>(y + 1, 0) = 2 * d * d;
    }
}
