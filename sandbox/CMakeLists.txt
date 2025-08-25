cmake_minimum_required(VERSION 3.28)
project(Sandbox)

add_executable(Sandbox src/SandboxApp.cpp)
target_link_libraries(Sandbox PRIVATE Aisite::Engine)