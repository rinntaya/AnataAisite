cmake_minimum_required(VERSION 3.28)
project(Sandbox)




add_executable(Sandbox src/SandboxApp.cpp
        src/Sandbox2D.cpp
        src/Sandbox2D.h)
target_link_libraries(Sandbox PRIVATE Aisite::Engine)