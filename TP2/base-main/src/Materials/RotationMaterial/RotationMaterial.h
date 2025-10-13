#pragma once
#include "MaterialGL.h"
#include <glm/glm.hpp>
#include <iostream>

class RotationMaterial final : public MaterialGL {
public:
    explicit RotationMaterial(std::string name = "")
        : MaterialGL(std::move(name)) {}

    void render(Node * /*o*/) override {}

    void animate(Node *o, const float elapsedTime) override {
        const float dt = elapsedTime;

        const float deltaAngle = m_speed * dt;
        o->frame()->rotate(glm::vec3(0, 1, 0), deltaAngle);
    }

    float m_speed = 0.003f;

};
