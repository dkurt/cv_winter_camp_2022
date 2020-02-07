#include <opencv2/ts.hpp>

#include "algo.hpp"

PERF_TEST(bgr2gray, reference)
{
    cv::Mat src(480, 640, CV_8UC3), dst;

    // Warmup
    bgr2gray_reference(src, dst);

    PERF_SAMPLE_BEGIN()
        bgr2gray_reference(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}

PERF_TEST(bgr2gray, u8)
{
    cv::Mat src(480, 640, CV_8UC3), dst;

    // Warmup
    bgr2gray_u8(src, dst);

    PERF_SAMPLE_BEGIN()
        bgr2gray_u8(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}

PERF_TEST(bgr2gray, u8_parallel)
{
    cv::Mat src(480, 640, CV_8UC3), dst;

    // Warmup
    bgr2gray_u8_parallel(src, dst);

    PERF_SAMPLE_BEGIN()
        bgr2gray_u8_parallel(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}

PERF_TEST(Prewitt, ref)
{
    cv::Mat src(1080, 1920, CV_8UC1), dst;

    PERF_SAMPLE_BEGIN()
        prewitt_x(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}

PERF_TEST(Prewitt, parallel)
{
    cv::Mat src(1080, 1920, CV_8UC1), dst;

    PERF_SAMPLE_BEGIN()
        prewitt_x_parallel(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}

PERF_TEST(Prewitt, parallel_vec)
{
    cv::Mat src(1080, 1920, CV_8UC1), dst;

    PERF_SAMPLE_BEGIN()
        prewitt_x_parallel_vec(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}

PERF_TEST(Prewitt, parallel_vec_wrap)
{
    cv::Mat src(1080, 1920, CV_8UC1), dst;

    PERF_SAMPLE_BEGIN()
        prewitt_x_parallel_vec_wrap(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}

PERF_TEST(Prewitt, parallel_vec_wrap2)
{
    cv::Mat src(1080, 1920, CV_8UC1), dst;

    PERF_SAMPLE_BEGIN()
        prewitt_x_parallel_vec_wrap2(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}
