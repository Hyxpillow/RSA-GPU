file(GLOB_RECURSE UTILS_SRC "*.cpp")

add_library(utils_module ${UTILS_SRC})

target_include_directories(utils_module
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}  # 访问 utils 自身的头文件
        ${CMAKE_SOURCE_DIR}
)

# target_link_libraries(utils_module PUBLIC)
find_package(OpenSSL REQUIRED)
target_link_libraries(utils_module
    PUBLIC OpenSSL::SSL OpenSSL::Crypto
)