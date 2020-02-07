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

* `cv::Mat` types: `[depth | (i.e. CV_8U, CV_16F, CV_32F, CV_64F)] + number of channels`

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

### Example: edge detector
1. Sobel operator

```cpp
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

@[3-20]
@[22-37]
@[45-64]
@[71-93]
@[101-125]

<!----------------------------------------------------------------------------->
