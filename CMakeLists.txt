cmake_minimum_required(VERSION 3.4.1)

project(rknn_yolov5_demo_linux)

set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -s -O3")

if (CMAKE_C_COMPILER MATCHES "aarch64")
  set(LIB_ARCH lib64)
else()
  set(LIB_ARCH lib)
endif()

# rga
set(RGA_DIR ${CMAKE_SOURCE_DIR}/3rdparty/rga)
include_directories(${RGA_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# rknn api
set(RKNN_API_PATH ${CMAKE_SOURCE_DIR}/rknpu/rknn/rknn_api/librknn_api)
include_directories(${RKNN_API_PATH}/include)
set(RKNN_API_LIB ${RKNN_API_PATH}/${LIB_ARCH}/librknn_api.so)

#stb
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/)
include_directories(${CMAKE_SOURCE_DIR}/include/)

set(CMAKE_INSTALL_RPATH "lib")

# Add the toolchain library directory to the linker search paths
# link_directories(
#   /tools/toolchain/gcc-sigmastar-9.1.0-2020.07-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/
#   # libc/usr/lib/
#   # /tools/toolchain/gcc-sigmastar-9.1.0-2020.07-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib/
# /tools/toolchain/gcc-sigmastar-9.1.0-2020.07-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/lib/
# # /tools/toolchains/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/lib/


# /tools/toolchains/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/
# # /tools/toolchains/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib/
# )

add_executable(rknn_yolov5_demo
  src/rga_func.c
  src/postprocess.cc
  src/main.cc
  src/utils.cc
  src/inference_handler.cc
  src/camera_handler.cc
)

target_link_libraries(rknn_yolov5_demo
	${RKNN_API_LIB}
  dl
  m
  pthread
  stdc++
  # ${CMAKE_SOURCE_DIR}/lib/libm.so
  # ${CMAKE_SOURCE_DIR}/lib/libpthread.so
  # ${CMAKE_SOURCE_DIR}/lib/libstdc++.so
  ${CMAKE_SOURCE_DIR}/lib/libopencv_core.so
  ${CMAKE_SOURCE_DIR}/lib/libopencv_highgui.so
  ${CMAKE_SOURCE_DIR}/lib/libopencv_imgcodecs.so
  ${CMAKE_SOURCE_DIR}/lib/libopencv_imgproc.so
  ${CMAKE_SOURCE_DIR}/lib/libopencv_videoio.so


  
  # ${CMAKE_SOURCE_DIR}/lib/libopencv_video.so

  # ${CMAKE_SOURCE_DIR}/lib/libavdevice.a
  # ${CMAKE_SOURCE_DIR}/lib/libavfilter.a
  # ${CMAKE_SOURCE_DIR}/lib/libavformat.a
  # ${CMAKE_SOURCE_DIR}/lib/libavutil.a
  # ${CMAKE_SOURCE_DIR}/lib/libcurl.so
  # ${CMAKE_SOURCE_DIR}/lib/libcurl.so.4
  # ${CMAKE_SOURCE_DIR}/lib/libcurl.so.4.8.0
  # ${CMAKE_SOURCE_DIR}/lib/libavcodec.a
)

# install target and libraries
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/rknn_yolov5_demo)
install(TARGETS rknn_yolov5_demo DESTINATION ./)
install(DIRECTORY model DESTINATION ./)
install(PROGRAMS ${RKNN_API_LIB} DESTINATION lib)
