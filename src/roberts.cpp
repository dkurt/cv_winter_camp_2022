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
            dst.at<uint32_t>(y, x) = dx * dx + dy * dy;
        }
}

void roberts_parralel(const cv::Mat& src, cv::Mat& dst) {
    CV_Assert(src.type() == CV_8UC1);
    Mat bsrc;
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_32SC1);
    parallel_for_(Range(0, dst.rows), [&](const Range& range) {
        for (int y = range.start; y < range.end; ++y) {
            for (int x = 0; x < dst.cols; ++x) {
                int dx = bsrc.at<uchar>(y, x) - bsrc.at<uchar>(y + 1, x + 1);
                int dy = bsrc.at<uchar>(y, x + 1) - bsrc.at<uchar>(y + 1, x);
                dst.at<uint32_t>(y, x) = dx * dx + dy * dy;
            }
        }
    });
}
