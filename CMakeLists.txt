cmake_minimum_required(VERSION 3.28)
project(Aisite)

set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    add_compile_options(/utf-8)
#    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# 在工程顶层 CMakeLists.txt 里写
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake-build-target)  # 静态库 (.lib / .a)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake-build-target)  # 动态库 (.so / .dylib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake-build-target)  # 可执行文件 / DLL

#foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
#    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
#    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib)
#    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib)
#    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib)
#endforeach()

add_subdirectory(engine)
add_subdirectory(sandbox)

if (MSVC)

    # CMake >= 3.15 推荐用法
    # Debug -> /MDd, Release -> /MD
    set_property(TARGET Engine PROPERTY
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set_property(TARGET Sandbox PROPERTY
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# deps
