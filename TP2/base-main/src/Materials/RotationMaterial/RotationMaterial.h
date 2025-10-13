#pragma once
#include "MaterialGL.h"
#include <glm/glm.hpp>

class RotationMaterial final : public MaterialGL {
public:
    explicit RotationMaterial(std::string name = "")
        : MaterialGL(std::move(name)) {}

    void render(Node * /*o*/) override {}

    void animate(Node *o, const float elapsedTime) override {
        const float dt = elapsedTime * 0.001;

        const float deltaAngle = m_speed * dt;
        o->frame()->rotate(glm::vec3(0, 1, 0), deltaAngle);
    }

    float m_speed = glm::radians(90.0f);
};
