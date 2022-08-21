set (PROJECT_NAME template)

cmake_minimum_required(VERSION 3.1)

project(${PROJECT_NAME})

# Disable security warnings for Visual Studio
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Application files
set (app_files
    app.cpp
    app.hpp
)

# Library files
set (lib_files
    lib.cpp
    lib.hpp
)

# 3rdparty libraries
find_package(OpenCV)

find_package(CUDA)
if (CUDA_FOUND)
set(
    CUDA_NVCC_FLAGS
    ${CUDA_NVCC_FLAGS}
    --default-stream per-thread
    -gencode=arch=compute_30,code=sm_30 
    -gencode=arch=compute_35,code=sm_35 
    -gencode=arch=compute_53,code=sm_53 
    -gencode=arch=compute_61,code=sm_61
    -gencode=arch=compute_62,code=sm_62 
    -gencode=arch=compute_72,code=sm_72 
    -use_fast_math
    #-Xptxas="-v"
    #-maxregcount 16
    )
    
set(
    CUDA_NVCC_FLAGS_MINSIZEREL
    ${CUDA_NVCC_FLAGS_MINSIZEREL}
    -O1
    )
    
set(
    CUDA_NVCC_FLAGS_RELWITHDEBINFO
    ${CUDA_NVCC_FLAGS_RELWITHDEBINFO}
    -O2
    )    
    
set(
    CUDA_NVCC_FLAGS_RELEASE
    ${CUDA_NVCC_FLAGS_RELEASE}
    -O3
    )    
    
endif()

add_executable(${PROJECT_NAME}_app ${app_files})
add_library(${PROJECT_NAME}_lib STATIC ${lib_files})

if (OpenCV_FOUND)
MESSAGE("OpenCV Found")
add_compile_definitions(USE_OPENCV)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_lib ${OpenCV_LIBS})
endif()

if (CUDA_FOUND)
MESSAGE("CUDA Found")
add_compile_definitions(USE_CUDA)
include_directories(${CUDA_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_lib ${CUDA_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_lib ${CUDA_npp_LIBRARY})
target_link_libraries(${PROJECT_NAME}_lib ${CUDA_nppi_LIBRARY})
endif()

target_link_libraries(${PROJECT_NAME}_app ${PROJECT_NAME}_lib)

# Enable C++17 support in Windows
set_property(TARGET ${PROJECT_NAME}_app PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME}_lib PROPERTY CXX_STANDARD 17)

# Enable additional warnings 
if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -lm -std=c++17")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -pedantic -O3 -fopenmp -pthread -std=c++17")
endif()