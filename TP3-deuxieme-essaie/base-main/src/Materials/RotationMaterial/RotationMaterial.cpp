#include "RotationMaterial.h"
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>

RotationMaterial::RotationMaterial(std::string name) : MaterialGL(name) {

}

RotationMaterial::~RotationMaterial() {}

void RotationMaterial::render(Node *o) {

    m_ProgramPipeline->bind();

    o->drawGeometry(GL_TRIANGLES);
    m_ProgramPipeline->release();
}

void RotationMaterial::animate(Node *o, const float elapsedTime) {
    float deltaAngle = m_speed * elapsedTime;
    o->frame()->rotate(glm::vec3(0, 1, 0), deltaAngle);
}
