cmake_minimum_required(VERSION 2.13.0)
project(robot_base)

## Set compiler to use c++ 11 features
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Add additional path for CMake to find wrp_sdk (IMPORTANT)
list(APPEND CMAKE_PREFIX_PATH "/opt/weston_robot/lib/cmake")

## Find wra_sdk
find_package(wra_sdk REQUIRED)

add_executable(sample_camera_client src/sample_camera_client.cpp)
target_link_libraries(sample_camera_client PRIVATE westonrobot::wra_sdk)

add_executable(sample_mobile_base_client src/sample_mobile_base_client.cpp)
target_link_libraries(sample_mobile_base_client PRIVATE westonrobot::wra_sdk)
