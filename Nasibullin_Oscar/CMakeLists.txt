cmake_minimum_required(VERSION 2.8)
project(perspectiveTool)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Mnist.cpp)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(perspectiveTool ${SOURCE_FILES})
target_link_libraries( perspectiveTool ${OpenCV_LIBS} )