cmake_minimum_required(VERSION 3.10)
project(GTestExample)

set(CMAKE_CXX_STANDARD 17)

# 添加 Google Test
find_package(GTest REQUIRED)

# 编译 big_num.cpp
add_executable(GTestExample test_bignum_mod.cpp big_num.cpp encrypt.cpp)

# 链接 Google Test 和 pthread 库
target_link_libraries(GTestExample GTest::gtest GTest::gtest_main pthread)
