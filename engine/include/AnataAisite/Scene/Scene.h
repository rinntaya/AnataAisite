#pragma once
#include "AnataAisite/Timestep.h"
#include "AnataAisite/Utils/UUID.h"
#include "entt/entt.hpp"
#include "glm/fwd.hpp"


class b2World;


namespace Aisite {
    class Entity;


    class AISITE_API Scene
    {
    public:
        Scene();
        ~Scene();

        static Ref<Scene> Copy(const Ref<Scene> &other);

        Entity CreateEntity(const std::string& name = std::string());
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnRuntimeStart();
        void OnRuntimeStop();

        void OnSimulationStart();
        void OnSimulationStop();

        void OnUpdateRuntime(Timestep ts);
        void OnUpdateCamera(Timestep ts);
        void OnUpdateSimulation(Timestep ts, const glm::mat4& viewProj);
        void OnUpdateEditor(Timestep ts, const glm::mat4& viewProj);
        void OnViewportResize(float width, float height);

        void DuplicateEntity(Entity entity);
        Entity GetPrimaryCameraEntity();
        template<typename... Components>
        auto GetAllEntitiesWith()
        {
            return m_Registry.view<Components...>();
        }
    private:
        template<typename T>
        AISITE_API void OnComponentAdded(Entity entity, T& component);


        void OnPhysics2DStart();
        void OnPhysics2DStop();

        void OnUpdate(Timestep ts);
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        b2World* m_PhysicsWorld = nullptr;

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };

}
