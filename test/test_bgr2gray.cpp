#include <opencv2/ts.hpp>

#include "algo.hpp"

using namespace testing;

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

typedef TestWithParam<tuple<int, int> > bgr2gray_p;
TEST_P(bgr2gray_p, parallel)
{
    Mat src(/*rows*/ get<0>(GetParam()), /*cols*/ get<1>(GetParam()), CV_8UC3), ref, dst;
    randu(src, 0, 255);

    bgr2gray_u8(src, ref);
    bgr2gray_u8_parallel(src, dst);

    EXPECT_EQ(countNonZero(ref != dst), 0);
}
INSTANTIATE_TEST_CASE_P(/**/, bgr2gray_p, Combine(
    Values(3, 4),
    Values(2, 5)
));
