#pragma once
#include "MaterialGL.h"
#include <glm/glm.hpp>

class PhongMaterial : public MaterialGL {
public:
    explicit PhongMaterial(std::string name = "");


    void render(Node *o) override;
    void animate(Node *o, const float elapsedTime) override;

    glm::vec3 m_objectColor = glm::vec3(1.0f, 0.0f, 0.0f);
    float m_ka = 0.2f;
    float m_kd = 0.8f;
    glm::vec3 m_posLum = glm::vec3(0.0f, 5.0f, 0.0f);

protected:
    GLProgram *vp = nullptr;
    GLProgram *fp = nullptr;
    GLint l_Model = -1, l_View = -1, l_Proj = -1;
    GLint l_ObjCol = -1, l_ka = -1, l_kd = -1, l_posLum = -1;
};
