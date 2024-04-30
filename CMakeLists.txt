cmake_minimum_required(VERSION 3.2)
project(Codec LANGUAGES CXX C)

# Set C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)

# FFMPEG
find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)
find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)
find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)

include_directories(
  include
  ${AVCODEC_INCLUDE_DIR}
  ${AVFORMAT_INCLUDE_DIR}
  ${AVUTIL_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
)

# Lib
add_library(${PROJECT_NAME}_lib SHARED
  src/H264Encoder.cpp
  src/H264Decoder.cpp
)

target_link_libraries(${PROJECT_NAME}_lib
  ${AVCODEC_LIBRARY}
  ${AVFORMAT_LIBRARY}
  ${AVUTIL_LIBRARY}
  ${Boost_LIBRARIES}
  -lboost_system
  -lpthread
)

# Example
add_executable(${PROJECT_NAME}_example
  example/Camera.cpp
)

target_link_libraries(${PROJECT_NAME}_example
  ${PROJECT_NAME}_lib
  ${OpenCV_LIBRARIES}
)

# Install.
install(TARGETS ${PROJECT_NAME}_lib
  ARCHIVE DESTINATION lib # ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION lib # ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION bin # ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include
  DESTINATION /usr/local/include
  FILES_MATCHING PATTERN "*.hpp*"
)
