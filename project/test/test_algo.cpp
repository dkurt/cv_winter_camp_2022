#include <opencv2/ts.hpp>

#include "algo.hpp"

TEST(bgr2gray, reference)
{
    cv::Mat src(10, 11, CV_8UC3), dst;
    bgr2gray_reference(src, dst);
    EXPECT_EQ(1, 2);
}
