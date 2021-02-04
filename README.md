# Intel Computer Vision Camp - Optimization Track (Winter 2021 :snowflake: :snowflake: :snowflake:)

Algorithms optimizations using OpenCV

[![GitPitch](https://gitpitch.com/assets/badge.svg)](https://gitpitch.com/dkurt/cv_winter_camp_2021)
[![CI](https://github.com/dkurt/cv_winter_camp_2021/workflows/CI/badge.svg?branch=master)](https://github.com/dkurt/cv_winter_camp_2021/actions?query=branch%3Amaster)

## Getting started

* Clone this repository to your computer
  ```sh
  git clone https://github.com/dkurt/cv_winter_camp_2021
  cd cv_winter_camp_2021
  ```

* Initialize submodules
  ```sh
  git submodule init
  git submodule update --depth 1
  ```

* Build a project (make sure if `cmake` installed)
  ```sh
  mkdir build && cd build
  ```

  * Linux
    ```sh
    cmake -DCMAKE_BUILD_TYPE=Release .. && make -j4
    ```

  * Microsoft Windows
    ```sh
    "C:\Program Files\CMake\bin\cmake.exe" -G "Visual Studio 16 2019" -A x64 ..
    "C:\Program Files\CMake\bin\cmake.exe" --build . --config Release -j 4
    ```

## C++ project structure
* **src** - source files with algorithms implementation
* **include** - header files with algorithms definitions
* **test** - regression tests
* **perf** - performance tests
* **3rdparty** - folder with OpenCV submodule
