#include "algo.hpp"

void roberts_reference(const cv::Mat& src, cv::Mat& dst) {
    CV_Assert(src.type() == CV_8UC1);
    Mat bsrc;
    copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
    dst.create(src.size(), CV_16SC1);
    for (int y = 0; y < dst.rows; ++y)
        for (int x = 0; x < dst.cols; ++x) {
            int dx = bsrc.at<uchar>(y, x) - bsrc.at<uchar>(y + 1, x + 1);
            int dy = bsrc.at<uchar>(y, x + 1) - bsrc.at<uchar>(y + 1, x);
            dst.at<uint16_t>(y, x) = dx * dx + dy * dy;
        }
}

void roberts_parallel(const cv::Mat& src, cv::Mat& dst) {
  CV_Assert(src.type() == CV_8UC1);
  Mat bsrc;
  copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
  dst.create(src.size(), CV_16SC1);
  parallel_for_(Range(0, src.rows), [&](const Range& range) {
    for (int y = range.start; y < range.end; ++y)
      for (int x = 0; x < dst.cols; ++x) {
        int dx = bsrc.at<uchar>(y, x) - bsrc.at<uchar>(y + 1, x + 1);
        int dy = bsrc.at<uchar>(y, x + 1) - bsrc.at<uchar>(y + 1, x);
        dst.at<uint16_t>(y, x) = dx * dx + dy * dy;
      }
    });
}

#include <opencv2/core/hal/intrin.hpp>

void roberts_parallel_vec(const cv::Mat& src, cv::Mat& dst) {
  CV_Assert(src.type() == CV_8UC1);
  Mat bsrc;
  copyMakeBorder(src, bsrc, 1, 1, 1, 1, BORDER_REPLICATE);
  dst.create(src.size(), CV_16SC1);
  parallel_for_(Range(0, src.rows), [&](const Range& range) {
    for (int y = range.start; y < range.end; ++y) {
      const uint8_t* psrc0 = bsrc.ptr(y);
      const uint8_t* psrc1 = bsrc.ptr(y + 1);
      int16_t* pdst = dst.ptr<int16_t>(y);
      int x = 0;
      for (; x <= dst.cols - v_uint16::nlanes; x += v_uint16::nlanes) {
        
        v_int16 dx = v_reinterpret_as_s16(vx_load_expand(psrc0 + x)) - v_reinterpret_as_s16(vx_load_expand(psrc1 + x + 1));
        v_int16 dy = v_reinterpret_as_s16(vx_load_expand(psrc0 + x + 1)) - v_reinterpret_as_s16(vx_load_expand(psrc1 + x));

        v_int16 res = (dx * dx + dy * dy);
        //v_uint16 res = ((vx_load_expand(psrc0 + x) - vx_load_expand(psrc1 + x + 1)) * (vx_load_expand(psrc0 + x) - vx_load_expand(psrc1 + x + 1)) + (vx_load_expand(psrc0 + x + 1) - vx_load_expand(psrc1 + x)) * (vx_load_expand(psrc0 + x + 1) - vx_load_expand(psrc1 + x)));
        v_store(pdst + x, res);
      }
      
      for (; x < dst.cols; ++x) {
        int16_t dx = (int16_t)psrc0[x] - (int16_t)psrc1[x + 1];
        int16_t dy = (int16_t)psrc0[x + 1] - (int16_t)psrc1[x];
        pdst[x] = (dx * dx + dy * dy);
      }
    }
    });
}
