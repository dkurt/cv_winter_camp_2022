<!-- .slide: class="center" -->
# Optimizations with OpenCV

---

### Agenda
* OpenCV
  * Brief overview
  * Basic structures
  * Example functions and methods
* Let’s write an algorithm
* Testing module
  * Accuracy test
  * Performance test
* Let’s optimize
* Practice / Homework

---

### OpenCV

* Open source project on GitHub with >3M downloads per year
* The most popular computer vision library with 20 years of development history
* 4 major releases: `1.0`, `2.4.x`, `3.4.x`, `4.x`
* Modular structure: `core`, `imgproc`, `ts`, `dnn`, `stitching`, ...
* Written in C++ but has automatic wrappers in Python, Java, JavaScript, Matlab, GO, PHP, C#, etc.
* Cross-platform and well optimized for research and development

---

### OpenCV basic structures

* `cv::Mat` - for images, masks, vector fields, complex values and custom data

```cpp
cv::Mat mat(480, 640, CV_8UC3);
int rows     = mat.rows;        // 480
int cols     = mat.cols;        // 640
int channels = mat.channels();  // 3
uint8_t* data = mat.ptr<uint8_t>();
```

* `cv::Mat` types: `[depth | (i.e. CV_8U, CV_16F, CV_32F, CV_64F)] + channels`

*  `std::cout << mat << std::endl` - To print `cv::Mat` in console and watch values

---
### OpenCV basic structures

```cpp
cv::Rect rect;           cv::Point point;       cv::Size size;
int x = rect.x;          int x = point.x;       int w = size.width;
int y = rect.y;          int y = point.y;       int h = size.height;
int w = rect.width;
int h = rect.height;
```

---
### OpenCV methods
Most of the methods work with basic OpenCV data structures as input and output

```cpp
cv::Mat src, dst, mask;

cv::cvtColor(src, dst, COLOR_BGR2GRAY);

cv::resize(src, dst, cv::Size(1280, 960));

cv::Canny(src, dst, /*threshold1*/ 100, /*threshold2*/ 200);

cv::inpaint(src, mask, dst, /*inpaintRadius*/ 3, cv::INPAINT_TELEA);

std::vector<cv::Mat> images;
cv::Ptr<Stitcher> stitcher = cv::Stitcher::create(cv::Sticher::PANORAMA);
stitcher->stitch(images, dst);
```

---
### BGR2Gray, parallel_for

---
### Regression tests

One of OpenCV modules is named `ts`. It sonsists of
* Google Test based testing infrastructure
* OpenCV related extensions for regression and performance tests
* Python scripts for tests analysis

---
### Regression test example

* Use `TEST` macro to define non-parametrized test
* Use `EXPECT_*` checks for numerical and logical tests

```cpp
#include <opencv2/ts.hpp>

TEST(bgr2gray, parallel)
{
    cv::Mat src(10, 11, CV_8UC3), ref, dst;
    randu(src, 0, 255);

    bgr2gray_u8(src, ref);
    bgr2gray_u8_parallel(src, dst);

    EXPECT_EQ(countNonZero(ref != dst), 0);
}
```

```bash
$ ./bin/test_algo --gtest_filter=bgr2gray*

[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from bgr2gray
[ RUN      ] bgr2gray.u8
[       OK ] bgr2gray.u8 (0 ms)
[ RUN      ] bgr2gray.parallel
[       OK ] bgr2gray.parallel (0 ms)
[----------] 2 tests from bgr2gray (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (0 ms total)
[  PASSED  ] 2 tests.
```

---
### Parametrized regression test example

---
### Performance tests

* Use `PERF_TEST` to define performance test
* Wrap target code to a block `PERF_SAMPLE_BEGIN()` - `PERF_SAMPLE_END()`
* OpenCV does as much iterations as it's needed to have stable metrics

```cpp
PERF_TEST(bgr2gray, u8_parallel)
{
    cv::Mat src(480, 640, CV_8UC3), dst;

    PERF_SAMPLE_BEGIN()
        bgr2gray_u8_parallel(src, dst);
    PERF_SAMPLE_END()

    SANITY_CHECK_NOTHING();
}
```

```bash
$ ./bin/perf_algo --gtest_filter=bgr2gray*

[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from bgr2gray
[ RUN      ] bgr2gray.u8_parallel
[ PERFSTAT ]    (samples=100   mean=0.32   median=0.28   min=0.22   stddev=0.11 (33.9%))
[       OK ] bgr2gray.u8_parallel (40 ms)
[----------] 1 test from bgr2gray (58 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (91 ms total)
[  PASSED  ] 1 test.
```

---

### Example: edge detector
1. Sobel operator

```cpp
```
     | -1  0  +1 |             | -1  -2  -1 |
Gx = | -2  0  +2 | * A,   Gy = |  0   0   0 | * A
     | -1  0  +1 |             | +1  +2  +1 |
```

cv::Sobel(src, dst, CV_8U, 1, 0);  // d/dx
cv::Sobel(src, dst, CV_8U, 0, 1);  // d/dy
```

2. Prewitt operator

```
     | -1  0  +1 |             | -1  -1  -1 |
Gx = | -1  0  +1 | * A,   Gy = |  0   0   0 | * A
     | -1  0  +1 |             | +1  +1  +1 |
```

3. Roberts cross

```
      | +1   0  |             |  0  +1 |
Gx =  |  0  -1  | * A,   Gy = | -1   0 | * A
```

<!----------------------------------------------------------------------------->

---?code=project/src/prewitt.cpp&lang=cpp&title=Prewitt operator implementation

@[3-17](Reference implementation: 11.07ms)
@[19-31](Parallel implementation: 9.41ms (x1.17))
@[39-58](Parallel vectorized implementation: 2.56ms (x4.32))
@[65-84](Parallel vectorized implementation: 2.51ms (x4.41))
@[91-115](Parallel vectorized implementation: 2.11ms (x5.24))

<!----------------------------------------------------------------------------->
