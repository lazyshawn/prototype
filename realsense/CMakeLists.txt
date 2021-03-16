############################################################################
# ==>> Basic Setting
############################################################################
cmake_minimum_required(VERSION 3.5)
# Project name | ${PROJECT_NAME}
project(demo)


############################################################################
# ==>> Create an executable
############################################################################
# Source directories
aux_source_directory(${CMAKE_SOURCE_DIR}/src DIR_SRCS)
# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
# Add an executable with the above sources
add_executable(demo_bin
  ${DIR_SRCS}
)

## Realsense
find_package(OpenCV REQUIRED)
target_link_libraries(demo_bin ${OpenCV_LIBS})

## OpenCV
# 前两条命令等同于第三条命令
set(DEPENDENCIES realsense2)
target_link_libraries(demo_bin ${DEPENDENCIES})
# target_link_libraries(demo -lrealsense2)    # Link lib of Realsense




