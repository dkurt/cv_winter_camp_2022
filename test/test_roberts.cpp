#include <opencv2/ts.hpp>

#include "algo.hpp"

typedef testing::TestWithParam<Size> Roberts;
TEST_P(Roberts, parallel)
{
    cv::Mat src(GetParam(), CV_8UC1), ref, dst;
    randu(src, 0, 255);

    roberts_reference(src, ref);
    roberts_parallel(src, dst);

    EXPECT_EQ(countNonZero(ref != dst), 0);
}

// TODO: should be failed?
// TEST_P(Prewitt, parallel_vec)
// {
//     cv::Mat src(GetParam(), CV_8UC1), ref, dst;
//     randu(src, 0, 255);
//
//     prewitt_x(src, ref);
//     prewitt_x_parallel_vec(src, dst);
//
//     EXPECT_EQ(countNonZero(ref != dst), 0);
// }
/*
TEST_P(Prewitt, parallel_vec_wrap)
{
    cv::Mat src(GetParam(), CV_8UC1), ref, dst;
    randu(src, 0, 255);

    prewitt_x(src, ref);
    prewitt_x_parallel_vec_wrap(src, dst);

    EXPECT_EQ(countNonZero(ref != dst), 0);
}

TEST_P(Prewitt, parallel_vec_wrap2)
{
    cv::Mat src(GetParam(), CV_8UC1), ref, dst;
    randu(src, 0, 255);

    prewitt_x(src, ref);
    prewitt_x_parallel_vec_wrap2(src, dst);

    EXPECT_EQ(countNonZero(ref != dst), 0);
}*/
INSTANTIATE_TEST_CASE_P(/**/, Roberts, testing::Values(TYPICAL_MAT_SIZES));
