#include <opencv2/ts.hpp>

#include "algo.hpp"

PERF_TEST(bgr2gray, reference)
{
    cv::Mat src(480, 640, CV_8UC3), dst;

    PERF_SAMPLE_BEGIN()
        bgr2gray_reference(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}

PERF_TEST(bgr2gray, u8)
{
    cv::Mat src(480, 640, CV_8UC3), dst;

    PERF_SAMPLE_BEGIN()
        bgr2gray_u8(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}

PERF_TEST(bgr2gray, u8_parallel)
{
    cv::Mat src(480, 640, CV_8UC3), dst;

    PERF_SAMPLE_BEGIN()
        bgr2gray_u8_parallel(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}
