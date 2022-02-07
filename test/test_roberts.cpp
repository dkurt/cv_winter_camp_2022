#include <opencv2/ts.hpp>

#include "algo.hpp"

void test_roberts(Size const &size, std::function<void(const cv::Mat&, cv::Mat&)> const &roberts)
{
    cv::Mat src(size, CV_8UC1), ref, dst;
    randu(src, 0, 255);

    roberts_reference(src, ref);
    roberts(src, dst);

    EXPECT_EQ(countNonZero(ref != dst), 0);
}

typedef testing::TestWithParam<Size> Roberts;

TEST_P(Roberts, parallel)
{
    test_roberts(GetParam(), roberts_parallel);
}

TEST_P(Roberts, no_copy_parallel)
{
    test_roberts(GetParam(), roberts_no_copy_parallel);
}

TEST_P(Roberts, no_copy_parallel_vec)
{
    test_roberts(GetParam(), roberts_no_copy_parallel_vec);
}

INSTANTIATE_TEST_CASE_P(/**/, Roberts, testing::Values(TYPICAL_MAT_SIZES));
