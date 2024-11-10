# 查找 cpu 目录中的所有 .cpp 文件，并存储在 CPU_SRC 变量中
file(GLOB_RECURSE CPU_SRC
    "*.cpp"  # 匹配当前目录下所有的 .cpp 文件
)

# 将所有 cpp 文件编译成一个静态或共享库，命名为 cpu_module
add_library(cpu_module ${CPU_SRC})

# 将 cpu 目录添加到库的头文件搜索路径中
target_include_directories(cpu_module PUBLIC ${CMAKE_SOURCE_DIR}/cpu)