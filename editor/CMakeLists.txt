cmake_minimum_required(VERSION 3.28)
project(Editor)




add_executable(Editor src/AisiteEditor.cpp
        src/EditorLayer.cpp
        src/EditorLayer.h
        src/panels/SceneHierarchyPanel.cpp
        src/panels/SceneHierarchyPanel.h
        src/renderer/EditorCamera.cpp
        src/renderer/EditorCamera.h
        src/panels/ContentBrowserPanel.cpp
        src/panels/ContentBrowserPanel.h)
target_link_libraries(Editor PRIVATE Aisite::Engine)


set_target_properties(Editor PROPERTIES OUTPUT_NAME "AnataEditor")