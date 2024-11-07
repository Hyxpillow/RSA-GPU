cmake_minimum_required(VERSION 3.10)
project(RSA_PROJECT)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(cpu)
add_subdirectory(utils)

find_package(OpenSSL REQUIRED)

add_executable(RSA_PROJECT main.cpp)
target_link_libraries(RSA_PROJECT PRIVATE cpu_module utils_module OpenSSL::SSL OpenSSL::Crypto)