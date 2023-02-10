cmake_minimum_required (VERSION 3.0)
project (lanzBrute)
#add_library(xlog STATIC xlog.cpp)
link_directories(lib)
add_executable(lanzBrute main.cpp)
# 指定加载的库
target_link_libraries(lanzBrute libuv_a.a pthread dl)