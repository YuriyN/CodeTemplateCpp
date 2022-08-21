#pragma once

#include <cstdint>
#include <string>
#include <algorithm>

#ifdef _MSC_VER
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#ifdef _MSC_VER
#include <direct.h>
#else
#include <sys/stat.h>
#endif

// Set to 1 by CMake if CUDA is found
#if USE_CUDA
#include <cuda_runtime.h>
#include <npp.h>

#define CUDA_ECS( status ) { \
        cudaError_t status_ = (status); \
        if(status_ != cudaSuccess) { \
            printf("CUDA ERROR: failed with status = (%d) at " "%s" "#%d\n", \
                status_, __FILENAME__, __LINE__); \
        } \
    }

#define NPP_ECS( status ) { \
        NppStatus status_ = (status); \
        if(status_ != NPP_SUCCESS) { \
            printf("NPP ERROR: failed with status = (%d) at " "%s" "#%d\n", \
                status_, __FILENAME__, __LINE__); \
        } \
    }
#endif

// Set to 1 by CMake if OpenCV is found
#if USE_OPENCV
#include <opencv2/opencv.hpp>
#endif

void make_dir(char* path);
std::string modify_path(char* src);