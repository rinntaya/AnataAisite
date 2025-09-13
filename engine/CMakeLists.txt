cmake_minimum_required(VERSION 3.28)
project(Engine)

set(HEADERS
        include/_atpch.h
        include/AnataAisite.h
        include/AnataAisite/EntryPoint.h
        include/AnataAisite/Application.h
        include/AnataAisite/Debug/Log.h
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
        src/renderer/GraphicsContext.h
        include/AnataAisite/Timestep.h
        include/AnataAisite/Debug/Instrumentor.h

)

set(SOURCES
        vendor/stb/stb_image.cpp
        vendor/entt/entt.hpp

        src/Application.cpp
        src/Log.cpp

        src/Layer.cpp
        src/LayerStack.cpp
        src/imgui/ImGuiLayer.cpp
        src/platform/opengl/OpenGLContext.cpp
        src/platform/opengl/OpenGLContext.h
        src/renderer/Shader.cpp
        include/AnataAisite/Renderer/Shader.h
        src/renderer/Buffer.cpp
        include/AnataAisite/Renderer/Buffer.h
        src/renderer/VertexArray.cpp
        include/AnataAisite/Renderer/VertexArray.h
        src/renderer/RendererAPI.h
        src/renderer/RenderCommand.cpp
        include/AnataAisite/Renderer/RenderCommand.h
        src/platform/opengl/OpenGLRendererAPI.cpp
        src/platform/opengl/OpenGLRendererAPI.h


        src/renderer/Camera.cpp
        include/AnataAisite/Renderer/Camera.h
        src/platform/opengl/OpenGLShader.cpp
        src/platform/opengl/OpenGLShader.h
        src/renderer/Texture.cpp
        include/AnataAisite/Renderer/Texture.h
        src/platform/opengl/OpenGLTexture.cpp
        src/platform/opengl/OpenGLTexture.h
        src/platform/opengl/OpenGLBuffer.cpp
        src/platform/opengl/OpenGLBuffer.h
        src/platform/opengl/OpenGLVertexArray.cpp
        src/platform/opengl/OpenGLVertexArray.h
        include/AnataAisite/Renderer/CameraController.h
        src/renderer/CameraController.cpp
        src/renderer/Renderer.cpp
        include/AnataAisite/Renderer/Renderer.h
        src/renderer/Renderer2D.cpp
        include/AnataAisite/Renderer/Renderer2D.h
        include/AnataAisite/Renderer/Framebuffer.h
        src/renderer/Framebuffer.cpp
        src/platform/opengl/OpenGLFramebuffer.cpp
        src/platform/opengl/OpenGLFramebuffer.h
        include/AnataAisite/Scene/Scene.cpp
        include/AnataAisite/Scene/Scene.h
        include/AnataAisite/Scene/Components.h
        include/AnataAisite/Scene/Components.cpp
        include/AnataAisite/Scene/Entity.cpp
        include/AnataAisite/Scene/Entity.h
        include/AnataAisite/Scene/SceneCamera.cpp
        include/AnataAisite/Scene/SceneCamera.h
        include/AnataAisite/Scene/ScriptableEntity.h
        include/AnataAisite/PlatformDetection.h
        include/AnataAisite/Debug/Assert.h
        include/AnataAisite/Scene/SceneSerializer.cpp
        include/AnataAisite/Scene/SceneSerializer.h
        include/AnataAisite/Utils/PlatformUtils.h
        include/AnataAisite/Math/Math.cpp
        include/AnataAisite/Math/Math.h
        include/AnataAisite/Debug/Timer.h
        include/AnataAisite/Renderer/UniformBuffer.cpp
        include/AnataAisite/Renderer/UniformBuffer.h
        src/platform/opengl/OpenGLUniformBuffer.cpp
        src/platform/opengl/OpenGLUniformBuffer.h
        include/AnataAisite/Utils/UUID.cpp
        include/AnataAisite/Utils/UUID.h
)

set(WINDOWS_FILES
        src/platform/Windows/WindowsWindow.cpp
        src/platform/Windows/WindowsWindow.h
        src/platform/Windows/WindowsInput.cpp
        src/platform/windows/WindowsPlatformUtils.cpp
)
set(UNIX_FILES

        #        src/platform/Unix/UnixWindow.cpp
#        src/platform/Unix/UnixWindow.h
)

list(APPEND SOURCES
        deps/imgui/imgui.cpp
        deps/imgui/imgui_draw.cpp
        deps/imgui/imgui_tables.cpp
        deps/imgui/imgui_widgets.cpp
        deps/imgui/imgui_demo.cpp
        deps/imgui/backends/imgui_impl_opengl3.cpp
        deps/imgui/backends/imgui_impl_glfw.cpp

        deps/ImGuizmo/ImGuizmo.cpp
)

add_library(Engine SHARED ${SOURCES} ${HEADERS})
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
target_include_directories(Engine
        PUBLIC src
        PRIVATE src
        PRIVATE res
        PUBLIC vendor
        PUBLIC deps/ImGuizmo
        PUBLIC deps/imgui
        PRIVATE deps/imgui/backends
)
target_precompile_headers(Engine PUBLIC include/_atpch.h)

find_package(Vulkan REQUIRED)
target_link_libraries(Engine PRIVATE Vulkan::Vulkan)


# Vulkan SDK
set(VULKAN_SDK $ENV{VULKAN_SDK})
set(LibraryDir_VulkanSDK "${VULKAN_SDK}/Lib")
set(Libs_Debug
        "${LibraryDir_VulkanSDK}/shaderc_sharedd.lib"
        "${LibraryDir_VulkanSDK}/spirv-cross-cored.lib"
        "${LibraryDir_VulkanSDK}/spirv-cross-glsld.lib"
        "${LibraryDir_VulkanSDK}/SPIRV-Toolsd.lib"
)
set(Libs_Release
        "${LibraryDir_VulkanSDK}/shaderc_shared.lib"
        "${LibraryDir_VulkanSDK}/spirv-cross-core.lib"
        "${LibraryDir_VulkanSDK}/spirv-cross-glsl.lib"
)
target_link_libraries(Engine PRIVATE
        $<$<CONFIG:Debug>:${Libs_Debug}>
        $<$<CONFIG:Release>:${Libs_Release}>
)

add_subdirectory(deps/spdlog)
target_link_libraries(Engine PUBLIC spdlog::spdlog)
add_subdirectory(deps/glfw)
target_link_libraries(Engine PRIVATE glfw)
add_subdirectory(deps/glad)
target_link_libraries(Engine PRIVATE glad)
add_subdirectory(deps/yaml-cpp)
target_link_libraries(Engine PRIVATE yaml-cpp)
add_subdirectory(deps/glm)
target_link_libraries(Engine PUBLIC  glm)
add_subdirectory(deps/box2d)
target_link_libraries(Engine PRIVATE box2d)

#add_library(imgui
#        deps/imgui/imgui.cpp
#        deps/imgui/imgui_draw.cpp
#        deps/imgui/imgui_tables.cpp
#        deps/imgui/imgui_widgets.cpp
#        deps/imgui/imgui_demo.cpp
#        deps/imgui/backends/imgui_impl_opengl3.cpp
#        deps/imgui/backends/imgui_impl_glfw.cpp
#)
#target_include_directories(imgui
#        PUBLIC deps/imgui
#        PUBLIC deps/imgui/backends
#        PRIVATE deps/glfw/include
#)
#target_link_libraries(Engine PUBLIC imgui)

if (MSVC)
    set_target_properties(Engine PROPERTIES
            OUTPUT_NAME "libanataAisite"
#            WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
else ()
    set_target_properties(Engine PROPERTIES OUTPUT_NAME "anataAisite")
endif()
