#pragma once
#include "AnataAisite/Timestep.h"
#include "entt/entt.hpp"
#include "glm/fwd.hpp"


namespace Aisite {
    class Entity;


    class AISITE_API Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);


        void OnUpdate(Timestep ts);
        void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, const glm::mat4& viewProj);
        void OnViewportResize(float width, float height);

        Entity GetPrimaryCameraEntity();
    private:
        template<typename T>
        AISITE_API void OnComponentAdded(Entity entity, T& component);
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };

}
