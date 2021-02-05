#include <opencv2/opencv.hpp>

using namespace cv;

void bgr2gray_reference(const Mat& src, Mat& dst);

void bgr2gray_u8(const Mat& src, Mat& dst);

void bgr2gray_u8_parallel(const Mat& src, Mat& dst);

void prewitt_x(const Mat& src, Mat& dst);

void prewitt_x_parallel(const Mat& src, Mat& dst);

void prewitt_x_parallel_vec(const Mat& src, Mat& dst);

void prewitt_x_parallel_vec_wrap(const Mat& src, Mat& dst);

void prewitt_x_parallel_vec_wrap2(const Mat& src, Mat& dst);

void roberts_reference(const cv::Mat& src, cv::Mat& dst);

void roberts_parallel(const cv::Mat& src, cv::Mat& dst);

void roberts_parallel_vector(const cv::Mat& src, cv::Mat& dst);