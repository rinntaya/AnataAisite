cmake_minimum_required(VERSION 3.28)
project(Engine)

set(HEADERS
        include/_atpch.h
        include/AnataAisite.h
        include/AnataAisite/EntryPoint.h
        include/AnataAisite/Application.h
        include/AnataAisite/Log.h
        include/AnataAisite/Events/Event.h
        include/AnataAisite/Events/KeyEvent.h
        include/AnataAisite/Events/MouseEvent.h
        include/AnataAisite/Events/ApplicationEvent.h
        include/AnataAisite/Window.h
        include/AnataAisite/Layer.h
        include/AnataAisite/LayerStack.h
        include/AnataAisite/ImGuiLayer.h
        include/AnataAisite/Input.h
        include/AnataAisite/KeyCodes.h
)

set(SOURCES
        vendor/stb_image.cpp

        src/Application.cpp
        src/Log.cpp

        src/Layer.cpp
        src/LayerStack.cpp
        src/imgui/ImGuiLayer.cpp
)

set(WINDOWS_FILES
        src/platform/Windows/WindowsWindow.cpp
        src/platform/Windows/WindowsWindow.h
        src/platform/Windows/WindowsInput.cpp
        src/platform/Windows/WindowsInput.h
)
set(UNIX_FILES
#        src/platform/Unix/UnixWindow.cpp
#        src/platform/Unix/UnixWindow.h
)

add_library(Engine SHARED ${SOURCES} ${HEADERS} ${WINDOWS_FILES} ${UNIX_FILES})
add_library(Aisite::Engine ALIAS Engine)
target_sources(Engine
        PRIVATE
        $<$<PLATFORM_ID:Windows>:${WINDOWS_FILES}>
        $<$<PLATFORM_ID:Linux>:${UNIX_FILES}>
)



target_compile_definitions(Engine PRIVATE AISITE_EXPORTS)
target_compile_definitions(Engine
        PUBLIC
        $<$<CONFIG:Debug>:_AT_DEBUG>
)
target_include_directories(Engine PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_include_directories(Engine PRIVATE src)
target_include_directories(Engine PRIVATE vendor)
target_include_directories(Engine PRIVATE res)
target_precompile_headers(Engine PUBLIC include/_atpch.h)


add_subdirectory(deps/spdlog EXCLUDE_FROM_ALL)
target_link_libraries(Engine PUBLIC spdlog::spdlog)
add_subdirectory(deps/glfw EXCLUDE_FROM_ALL)
target_link_libraries(Engine PRIVATE glfw)
add_subdirectory(deps/glad EXCLUDE_FROM_ALL)
target_link_libraries(Engine PRIVATE glad)
add_subdirectory(deps/glm EXCLUDE_FROM_ALL)
target_link_libraries(Engine PRIVATE glm)

add_library(imgui
        deps/imgui/imgui.cpp
        deps/imgui/imgui_draw.cpp
        deps/imgui/imgui_tables.cpp
        deps/imgui/imgui_widgets.cpp
        deps/imgui/imgui_demo.cpp
        deps/imgui/backends/imgui_impl_opengl3.cpp
        deps/imgui/backends/imgui_impl_glfw.cpp
)
target_include_directories(imgui
        PUBLIC deps/imgui
        PUBLIC deps/imgui/backends
        PUBLIC deps/glfw/include
)
target_link_libraries(Engine PRIVATE imgui)


if (MSVC)
    set_target_properties(Engine PROPERTIES
            OUTPUT_NAME "libanataAisite"
#            WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
else ()
    set_target_properties(Engine PROPERTIES OUTPUT_NAME "anataAisite")
endif()
