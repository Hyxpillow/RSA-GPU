cmake_minimum_required(VERSION 3.10)
project(RSA_PROJECT)

set(CMAKE_CXX_STANDARD 17)
include_directories(cpu utils)
aux_source_directory(cpu CPU_SRC)
aux_source_directory(utils UTILS_SRC)

add_executable(RSA_PROJECT main.cpp ${CPU_SRC} ${UTILS_SRC})

# 链接 Google Test 和 pthread 库
# target_link_libraries(GTestExample GTest::gtest GTest::gtest_main pthread)
