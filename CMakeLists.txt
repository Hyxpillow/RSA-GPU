cmake_minimum_required(VERSION 3.10)
project(RSA_PROJECT)

set(CMAKE_CXX_STANDARD 17)

# 添加子目录
add_subdirectory(cpu)
add_subdirectory(utils)

# 编译主程序并链接子模块
add_executable(RSA_PROJECT main.cpp)
target_link_libraries(RSA_PROJECT PRIVATE cpu_module utils_module)