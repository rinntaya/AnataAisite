#include "Components.h"

#include "box2d/b2_body.h"


namespace Aisite {

    void Rigidbody2DComponent::SetTransform(glm::vec2 pos, const float ang) const
    {
        if (RuntimeBody == nullptr) return;
        b2Body* body = (b2Body*)RuntimeBody;
        body->SetTransform({pos.x, pos.y}, ang);
    }
}
