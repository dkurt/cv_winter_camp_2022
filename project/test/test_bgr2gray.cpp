#include <opencv2/ts.hpp>

#include "algo.hpp"

TEST(bgr2gray, u8)
{
    cv::Mat src(10, 11, CV_8UC3), ref, dst;
    randu(src, 0, 255);

    bgr2gray_reference(src, ref);
    bgr2gray_u8(src, dst);

    double maxV;
    minMaxLoc(abs(ref - dst), 0, &maxV);

    EXPECT_LE(maxV, 1);
}

TEST(bgr2gray, parallel)
{
    cv::Mat src(10, 11, CV_8UC3), ref, dst;
    randu(src, 0, 255);

    bgr2gray_u8(src, ref);
    bgr2gray_u8_parallel(src, dst);

    EXPECT_EQ(countNonZero(ref != dst), 0);
}
