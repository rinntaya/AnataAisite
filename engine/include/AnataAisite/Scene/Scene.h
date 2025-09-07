#pragma once
#include "AnataAisite/Timestep.h"
#include "entt/entt.hpp"


namespace Aisite {
    class Entity;


    class AISITE_API Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());


        void OnUpdate(Timestep ts);
        void OnViewportResize(float width, float height);
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity;
        friend class SceneHierarchyPanel;
    };

}
