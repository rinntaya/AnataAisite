cmake_minimum_required(VERSION 3.12)
project(glad LANGUAGES C)

# 添加头文件路径
add_library(glad STATIC
        src/glad.c
)

target_include_directories(glad
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# 设置C标准
target_compile_features(glad PUBLIC c_std_99)
