#include "algo.hpp"

void roberts_reference(const cv::Mat& src, cv::Mat& dst) {
    CV_Assert(src.type() == CV_8UC1);
    Mat bsrc;
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_32SC1);
    for (int y = 0; y < dst.rows; ++y)
        for (int x = 0; x < dst.cols; ++x) {
            int dx = bsrc.at<uchar>(y, x) - bsrc.at<uchar>(y + 1, x + 1);
            int dy = bsrc.at<uchar>(y, x + 1) - bsrc.at<uchar>(y + 1, x);
            dst.at<int32_t>(y, x) = dx * dx + dy * dy;
        }
}

void roberts_reference_parallel(const cv::Mat& src, cv::Mat& dst) {
    CV_Assert(src.type() == CV_8UC1);
    Mat bsrc;
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_32SC1);
    parallel_for_(Range(0, src.rows), [&](const Range& range) {
        for (int y = range.start; y < range.end; ++y) {
            for (int x = 0; x < dst.cols; ++x) {
                int dx = bsrc.at<uchar>(y, x) - bsrc.at<uchar>(y + 1, x + 1);
                int dy = bsrc.at<uchar>(y, x + 1) - bsrc.at<uchar>(y + 1, x);
                dst.at<int32_t>(y, x) = dx * dx + dy * dy;
            }
        }
    });
}

#include <opencv2/core/hal/intrin.hpp>

void roberts_reference_parallel_vec(const cv::Mat& src, cv::Mat& dst) {
    CV_Assert(src.type() == CV_8UC1);
    Mat bsrc;
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_32SC1);
    parallel_for_(Range(0, src.rows), [&](const Range& range) {
        for (int y = range.start; y < range.end; ++y) {
            const int32_t* psrc0 = bsrc.ptr<int32_t>(y);
            const int32_t* psrc1 = bsrc.ptr<int32_t>(y+1);
            int32_t* pdst = dst.ptr<int32_t>(y);
            int x = 0;
            for (; x < dst.cols - v_int32::nlanes; x+= v_int32::nlanes) {
                v_int32 vdx = v_reinterpret_as_s32(vx_load(psrc0 + x)) - v_reinterpret_as_s32(vx_load(psrc1 + x + 1));
                v_int32 vdy = v_reinterpret_as_s32(vx_load(psrc0 + x + 1)) - v_reinterpret_as_s32(vx_load(psrc1 + x));
                v_int32 res = vdx*vdx+vdy*vdy;

                v_store(pdst + x, res);
            }
            for (; x < dst.cols; ++x) {
                pdst[x] = (psrc0[x] - psrc1[ x + 1])* (psrc0[x] - psrc1[x + 1]) +
                        (psrc0[x + 1] - psrc1[x])* (psrc0[x + 1] - psrc1[x]);

            }
        }
    });
}