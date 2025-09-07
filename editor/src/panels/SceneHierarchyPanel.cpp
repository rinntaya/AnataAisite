#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include "AnataAisite/Scene/Components.h"
#include "glm/gtc/type_ptr.hpp"


namespace Aisite
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
    {
        m_Context = context;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        // 开始前
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 4)); // 窗口无 padding
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, 4)); // 节点按钮内部 padding 大
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // 节点之间无缝隙

        // 可选的视觉优化
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

        // 可选：配合颜色（Blender 的 node 区域比较灰）
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.40f, 0.40f, 0.40f, 1.0f));
        ImGui::Begin("Scene Hierarchy");

        // ImDrawList* drawList = ImGui::GetWindowDrawList();
        // ImVec2 winPos = ImGui::GetCursorScreenPos();
        // ImVec2 winSize = ImGui::GetContentRegionAvail();
        //
        // float lineHeight = ImGui::GetTextLineHeightWithSpacing() + 8;
        // int rowCount = (int)(winSize.y / lineHeight) + 1;
        //
        // for (int row = 0; row < rowCount; row++)
        // {
        //     ImVec2 rectMin = ImVec2(winPos.x, winPos.y + row * lineHeight);
        //     ImVec2 rectMax = ImVec2(winPos.x + winSize.x, winPos.y + (row+1) * lineHeight);
        //     ImU32 bgColor = ImGui::GetColorU32(row % 2 == 0 ? ImVec4(0.15f,0.15f,0.15f,1.0f)
        //                                                    : ImVec4(0.20f,0.20f,0.20f,1.0f));
        //     drawList->AddRectFilled(rectMin, rectMax, bgColor);
        // }

        for (const auto entity: m_Context->m_Registry.view<entt::entity>()) {
            const Entity e{entity, m_Context.get()};
            DrawEntityNode(e);
        }
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};
        ImGui::End();
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(5);


        ImGui::Begin("Properties");
        if (m_SelectionContext)
            DrawComponents(m_SelectionContext);
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        const auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) |
                                   ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding;

        bool opened = ImGui::TreeNodeEx((void *) (uint64_t) (uint32_t) entity, flags, tag.c_str());
        if (ImGui::IsItemClicked()) {
            m_SelectionContext = entity;
        }

        if (opened) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding;
            bool opened = ImGui::TreeNodeEx((void *) 9817239, flags, tag.c_str());
            if (opened)
                ImGui::TreePop();
            ImGui::TreePop();
        }
    }


    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>()) {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256] = {};
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
                tag = std::string(buffer);
            }
        }

        if (entity.HasComponent<TransformComponent>()) {
            if (ImGui::TreeNodeEx((void *) typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
                                  "Transform")) {
                auto& transform = entity.GetComponent<TransformComponent>().Transform;
                ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
                ImGui::TreePop();
            }
        }

        if (entity.HasComponent<CameraComponent>()) {
            if (ImGui::TreeNodeEx((void *) typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
                                  "Camera")) {
                auto& cameraComponent = entity.GetComponent<CameraComponent>();
                auto& camera = cameraComponent.Camera;

                ImGui::Checkbox("Primary", &cameraComponent.Primary);

                const char* projectionTypeStrings[] = {"Perspective", "Orthographic"};
                const char* currentProjectionTypeString = projectionTypeStrings[(int) camera.GetProjectionType()];
                if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
                    for (int i = 0; i < 2; i++) {
                        bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                        if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
                            currentProjectionTypeString = projectionTypeStrings[i];
                            camera.SetProjectionType((SceneCamera::ProjectionType) i);
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
                    float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
                    if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
                        camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

                    float perspectiveNear = camera.GetPerspectiveNearClip();
                    if (ImGui::DragFloat("Near", &perspectiveNear ))
                        camera.SetPerspectiveNearClip(perspectiveNear );

                    float perspectiveFar = camera.GetPerspectiveFarClip();
                    if (ImGui::DragFloat("Far", &perspectiveFar))
                        camera.SetPerspectiveFarClip(perspectiveFar);
                }

                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
                    float orthoSize = camera.GetOrthographicSize();
                    if (ImGui::DragFloat("Size", &orthoSize))
                        camera.SetOrthographicSize(orthoSize);

                    float orthoNear = camera.GetOrthographicNearClip();
                    if (ImGui::DragFloat("Near", &orthoNear))
                        camera.SetOrthographicNearClip(orthoNear);

                    float orthoFar = camera.GetOrthographicFarClip();
                    if (ImGui::DragFloat("Far", &orthoFar))
                        camera.SetOrthographicFarClip(orthoFar);

                    ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);
                }

                ImGui::TreePop();
            }
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
            {
                auto& src = entity.GetComponent<SpriteRendererComponent>();
                ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
                ImGui::TreePop();
            }
        }


    }
} // Aisite
