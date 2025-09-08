#pragma once


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace Aisite {


    struct AISITE_API TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag)
            : Tag(tag) {}
    };

    struct AISITE_API TransformComponent
    {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation)
                    : Translation(translation) {}

        glm::mat4 GetTransform() const
        {
            const auto q = glm::quat(Rotation);

            return glm::translate(glm::mat4(1.0f), Translation)
                * glm::toMat4(q)
                * glm::scale(glm::mat4(1.0f), Scale);
        }

        operator glm::mat4 () const { return GetTransform(); }
        operator const glm::mat4 () const { return GetTransform(); }
    };

    struct AISITE_API SpriteRendererComponent
    {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            : Color(color) {}
    };


    struct CameraComponent
    {
		SceneCamera Camera;
        bool Primary = true; // TODO: think about moving to Scene
        bool FixedAspectRatio = false;


        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };



    struct NativeScriptComponent
    {
        ScriptableEntity* Instance = nullptr;

        ScriptableEntity*(*InstantiateScript)();
        void (*DestroyScript)(NativeScriptComponent*);


        template<typename T>
        void Bind()
        {
            InstantiateScript = [] { return static_cast<ScriptableEntity*>(new T()); };
            DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
        }
    };

}
