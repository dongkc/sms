cmake_minimum_required(VERSION 2.8)

INCLUDE_DIRECTORIES(/usr/include/gammu)

SET(CMAKE_CXX_FLAGS "-Wall -g -fmessage-length=0 -fno-strict-aliasing --std=c++11 ")
SET(CMAKE_C_FLAGS "-g -Wall -fmessage-length=0 -fno-strict-aliasing ")

PROJECT(smsd)

ADD_EXECUTABLE(smsd
  sms_send.cpp
  api.cpp
  http.cpp
  main.cpp)

TARGET_LINK_LIBRARIES(smsd
  Gammu
  glog
  gflags
  PocoNet
  PocoFoundation
  PocoUtil
  )
