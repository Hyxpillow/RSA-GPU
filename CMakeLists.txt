cmake_minimum_required(VERSION 3.10)
project(RSA_PROJECT)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(utils)
add_subdirectory(modexp)

find_package(OpenSSL REQUIRED)

add_executable(RSA_PROJECT main.cpp rsa.cpp)
target_link_libraries(RSA_PROJECT PRIVATE modexp_module utils_module OpenSSL::SSL OpenSSL::Crypto)