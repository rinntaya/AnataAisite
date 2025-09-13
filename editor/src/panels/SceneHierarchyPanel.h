#pragma once
#include "AnataAisite/Scene/Entity.h"
#include "AnataAisite/Scene/Scene.h"

namespace Aisite {

    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);

        void OnImGuiRender();

        Entity GetSelectedEntity() const { return m_SelectionContext; }
        void SetSelectedEntity(const Entity entity) { m_SelectionContext = entity; }
    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);
        template<class T>
        void DisplayAddComponentEntry(const char *id);

    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };

}
