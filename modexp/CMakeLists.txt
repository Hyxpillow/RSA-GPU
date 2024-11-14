# modexp/CMakeLists.txt

# Gather all .cpp and .cu files in the modexp directory
set(SOURCES
    modexp_cpu_mont.cpp
    modexp_cpu.cpp
)

add_library(modexp_module ${SOURCES})

# Include directory for modexp
target_include_directories(modexp_module
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}  # 访问自身的头文件
        ${CMAKE_SOURCE_DIR}          # 访问项目根目录
)

# Link libraries if needed (none specified here)
target_link_libraries(modexp_module
    PUBLIC utils_module
)