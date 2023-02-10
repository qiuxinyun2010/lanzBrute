cmake_minimum_required (VERSION 3.0)
project (lanzBrute)
#add_library(xlog STATIC xlog.cpp)
include_directories(include include/hv)
link_directories(lib)
add_executable(lanzBrute main.cpp)
# 指定加载的库
target_link_libraries(lanzBrute libhv_static.a pthread ssl)