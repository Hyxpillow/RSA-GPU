file(GLOB UTILS_SRC "*.cpp")

add_library(utils_module ${UTILS_SRC})

target_include_directories(utils_module
    PUBLIC
        ${CMAKE_SOURCE_DIR}/cpu    # 访问 cpu 目录中的头文件
        ${CMAKE_CURRENT_SOURCE_DIR}  # 访问 utils 自身的头文件
)

target_link_libraries(utils_module PUBLIC cpu_module)
