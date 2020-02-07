#include "algo.hpp"

//      | -1  0  +1 |
// Gx = | -1  0  +1 | * A
//      | -1  0  +1 |
void prewitt_x(const Mat& src, Mat& dst) {
    CV_Assert(src.type() == CV_8UC1);
    Mat bsrc;
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_8UC1);
    for (int y = 0; y < dst.rows; ++y)
        for (int x = 0; x < dst.cols; ++x) {
            dst.at<uchar>(y, x) = bsrc.at<uchar>(y    , x + 2) - bsrc.at<uchar>(y    , x) +
                                  bsrc.at<uchar>(y + 1, x + 2) - bsrc.at<uchar>(y + 1, x) +
                                  bsrc.at<uchar>(y + 2, x + 2) - bsrc.at<uchar>(y + 2, x);
        }
}

void prewitt_x_parallel(const Mat& src, Mat& dst) {
    Mat bsrc;
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_8UC1);
    parallel_for_(Range(0, src.rows), [&](const Range& range) {
      for (int y = range.start; y < range.end; ++y)
          for (int x = 0; x < dst.cols; ++x) {
              dst.at<uchar>(y, x) = bsrc.at<uchar>(y    , x + 2) - bsrc.at<uchar>(y    , x) +
                                    bsrc.at<uchar>(y + 1, x + 2) - bsrc.at<uchar>(y + 1, x) +
                                    bsrc.at<uchar>(y + 2, x + 2) - bsrc.at<uchar>(y + 2, x);
          }
    });
}

#include <opencv2/core/hal/intrin.hpp>

void prewitt_x_parallel_vec(const Mat& src, Mat& dst) {
    Mat bsrc;
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_8UC1);
    parallel_for_(Range(0, src.rows), [&](const Range& range) {
        for (int y = range.start; y < range.end; ++y) {
            const uint8_t* psrc0 = bsrc.ptr(y);
            const uint8_t* psrc1 = bsrc.ptr(y + 1);
            const uint8_t* psrc2 = bsrc.ptr(y + 2);
            uint8_t* pdst = dst.ptr(y);
            int x = 0;
            for (; x <= dst.cols - v_uint8::nlanes; x += v_uint8::nlanes) {
                v_uint8 res = vx_load(psrc0 + x + 2) - vx_load(psrc0 + x) +
                              vx_load(psrc1 + x + 2) - vx_load(psrc1 + x) +
                              vx_load(psrc2 + x + 2) - vx_load(psrc2 + x);
                v_store(pdst + x, res);
            }
            for (; x < dst.cols; ++x) {
                pdst[x] = psrc0[x + 2] - psrc0[x] +
                          psrc1[x + 2] - psrc1[x] +
                          psrc2[x + 2] - psrc2[x];
            }
        }
    });
}

void prewitt_x_parallel_vec_wrap(const Mat& src, Mat& dst) {
    Mat bsrc;
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_8UC1);
    parallel_for_(Range(0, src.rows), [&](const Range& range) {
        for (int y = range.start; y < range.end; ++y) {
            const uint8_t* psrc0 = bsrc.ptr(y);
            const uint8_t* psrc1 = bsrc.ptr(y + 1);
            const uint8_t* psrc2 = bsrc.ptr(y + 2);
            uint8_t* pdst = dst.ptr(y);
            int x = 0;
            for (; x <= dst.cols - v_uint8::nlanes; x += v_uint8::nlanes) {
                v_uint8 res = v_add_wrap(v_sub_wrap(vx_load(psrc0 + x + 2), vx_load(psrc0 + x)),
                              v_add_wrap(v_sub_wrap(vx_load(psrc1 + x + 2), vx_load(psrc1 + x)),
                                         v_sub_wrap(vx_load(psrc2 + x + 2), vx_load(psrc2 + x))));
                v_store(pdst + x, res);
            }
            for (; x < dst.cols; ++x) {
                pdst[x] = psrc0[x + 2] - psrc0[x] +
                          psrc1[x + 2] - psrc1[x] +
                          psrc2[x + 2] - psrc2[x];
            }
        }
    });
}

void prewitt_x_parallel_vec_wrap2(const Mat& src, Mat& dst) {
    Mat bsrc;
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_8UC1);
    parallel_for_(Range(0, src.rows), [&](const Range& range) {
        int y = range.start;
        for (; y <= range.end - 2; ++y) {
            const uint8_t* psrc0 = bsrc.ptr(y);
            const uint8_t* psrc1 = bsrc.ptr(y + 1);
            const uint8_t* psrc2 = bsrc.ptr(y + 2);
            const uint8_t* psrc3 = bsrc.ptr(y + 3);
            uint8_t* pdst0 = dst.ptr(y);
            uint8_t* pdst1 = dst.ptr(y+1);
            int x = 0;
            for (; x <= dst.cols - v_uint8::nlanes; x += v_uint8::nlanes) {
                v_uint8 res = v_add_wrap(v_sub_wrap(vx_load(psrc1 + x + 2), vx_load(psrc1 + x)),
                                         v_sub_wrap(vx_load(psrc2 + x + 2), vx_load(psrc2 + x)));
                v_store(pdst0 + x, v_add_wrap(res, v_sub_wrap(vx_load(psrc0 + x + 2),
                                                              vx_load(psrc0 + x))));
                v_store(pdst1 + x, v_add_wrap(res, v_sub_wrap(vx_load(psrc3 + x + 2),
                                                              vx_load(psrc3 + x))));
            }
            for (; x < dst.cols; ++x) {
                uint8_t res = psrc1[x + 2] - psrc1[x] + psrc2[x + 2] - psrc2[x];
                pdst0[x] = res + psrc0[x + 2] - psrc0[x];
                pdst1[x] = res + psrc3[x + 2] - psrc3[x];
            }
        }
        const uint8_t* psrc0 = bsrc.ptr(y);
        const uint8_t* psrc1 = bsrc.ptr(y + 1);
        const uint8_t* psrc2 = bsrc.ptr(y + 2);
        uint8_t* pdst = dst.ptr(y);
        int x = 0;
        for (; x <= dst.cols - v_uint8::nlanes; x += v_uint8::nlanes) {
            v_uint8 res = v_add_wrap(v_sub_wrap(vx_load(psrc0 + x + 2), vx_load(psrc0 + x)),
                          v_add_wrap(v_sub_wrap(vx_load(psrc1 + x + 2), vx_load(psrc1 + x)),
                                     v_sub_wrap(vx_load(psrc2 + x + 2), vx_load(psrc2 + x))));
            v_store(pdst + x, res);
        }
        for (; x < dst.cols; ++x) {
            pdst[x] = psrc0[x + 2] - psrc0[x] +
                      psrc1[x + 2] - psrc1[x] +
                      psrc2[x + 2] - psrc2[x];
        }
    });
}
