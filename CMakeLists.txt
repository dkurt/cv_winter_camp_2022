cmake_minimum_required(VERSION 2.8)

project(cv_winter_camp)

# OpenCV's flags
set(BUILD_LIST "ts,core" CACHE STRING "")
set(WITH_WEBP OFF CACHE BOOL "")
set(WITH_ZLIB OFF CACHE BOOL "")
set(WITH_OPENEXR OFF CACHE BOOL "")
set(WITH_PROTOBUF OFF CACHE BOOL "")
set(WITH_FFMPEG OFF CACHE BOOL "")
set(WITH_IPP OFF CACHE BOOL "")
set(WITH_JASPER OFF CACHE BOOL "")
set(WITH_QUIRC OFF CACHE BOOL "")
set(WITH_ITT OFF CACHE BOOL "")
set(WITH_ADE OFF CACHE BOOL "")
set(WITH_OPENCL OFF CACHE BOOL "")
set(WITH_TIFF OFF CACHE BOOL "")
set(BUILD_ZLIB OFF CACHE BOOL "")
set(WITH_JASPER OFF CACHE BOOL "")
set(BUILD_opencv_ts ON CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")
set(BUILD_PERF_TESTS OFF CACHE BOOL "")
set(BUILD_opencv_apps OFF CACHE BOOL "")
add_subdirectory(3rdparty/opencv)

include_directories(
  "3rdparty/opencv/include"
  "3rdparty/opencv/modules/core/include"
  "3rdparty/opencv/modules/ts/include"
  "3rdparty/opencv/modules/highgui/include"
  "3rdparty/opencv/modules/imgcodecs/include"
  "3rdparty/opencv/modules/videoio/include"
  "3rdparty/opencv/modules/imgproc/include"
  "${CMAKE_BINARY_DIR}"
  "include"
)

file(GLOB LIB_SOURCES src/*.cpp)
add_library(algo STATIC ${LIB_SOURCES})

# Regression test
file(GLOB TEST_SOURCES test/*.cpp)
add_executable(test_algo ${TEST_SOURCES})
target_link_libraries(test_algo
  opencv_ts
  algo
)

# Performance test
file(GLOB PERF_SOURCES perf/*.cpp)
add_executable(perf_algo ${PERF_SOURCES})
target_link_libraries(perf_algo
  opencv_ts
  algo
)
