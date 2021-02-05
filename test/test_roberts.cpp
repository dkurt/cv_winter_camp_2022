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

TEST_P(Roberts, parallel_vector)
{
  cv::Mat src(GetParam(), CV_8UC1), ref, dst;
  randu(src, 0, 127);

  roberts_reference(src, ref);
  roberts_parallel_vector(src, dst);

  EXPECT_EQ(countNonZero(ref != dst), 0);
}

INSTANTIATE_TEST_CASE_P(/**/, Roberts, testing::Values(TYPICAL_MAT_SIZES));