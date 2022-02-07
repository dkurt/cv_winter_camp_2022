#include <opencv2/ts.hpp>

#include "algo.hpp"

PERF_TEST(Roberts, ref)
{
    cv::Mat src(1080, 1920, CV_8UC1), dst;

    PERF_SAMPLE_BEGIN()
        roberts_reference(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}

PERF_TEST(Roberts, parallel)
{
    cv::Mat src(1080, 1920, CV_8UC1), dst;

    PERF_SAMPLE_BEGIN()
        roberts_parallel(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}

PERF_TEST(Roberts, no_copy_parallel)
{
    cv::Mat src(1080, 1920, CV_8UC1), dst;

    PERF_SAMPLE_BEGIN()
        roberts_no_copy_parallel(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}

PERF_TEST(Roberts, no_copy_parallel_vec)
{
    cv::Mat src(1080, 1920, CV_8UC1), dst;

    PERF_SAMPLE_BEGIN()
        roberts_no_copy_parallel_vec(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}
