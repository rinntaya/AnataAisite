cmake_minimum_required(VERSION 3.28)
project(Editor)




add_executable(Editor src/AisiteEditor.cpp
        src/EditorLayer.cpp
        src/EditorLayer.h
        src/panels/SceneHierarchyPanel.cpp
        src/panels/SceneHierarchyPanel.h)
target_link_libraries(Editor PRIVATE Aisite::Engine)