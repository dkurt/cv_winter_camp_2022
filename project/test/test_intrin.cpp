#include <opencv2/ts.hpp>
#include <opencv2/core/hal/intrin.hpp>

TEST(intrin, add)
{
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8};

    cv::v_int32x4 twos = cv::v_setall_s32(2);

    cv::v_int32x4 b0 = cv::v_load(&data[0]);
    b0 *= twos;
    v_store(&data[0], b0);

    b0 = cv::v_load(&data[4]);
    b0 -= twos;
    v_store(&data[4], b0);

    for (int i = 0; i < 4; ++i)
        EXPECT_EQ(data[i], (i + 1) * 2);
    for (int i = 4; i < 8; ++i)
        EXPECT_EQ(data[i], i - 1);
}
