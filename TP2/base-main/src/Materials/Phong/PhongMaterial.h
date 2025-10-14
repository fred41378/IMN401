#include "MaterialGL.h"
#include <glm/glm.hpp>

class PhongMaterial : public MaterialGL {
public:
    explicit PhongMaterial(std::string name = "");

    void setColor(const glm::vec3 &rgb);
    void render(Node *o) override;
    void animate(Node *o, const float elapsedTime) override;

    glm::vec3 m_objectColor = glm::vec3(1.0f, 0.0f, 0.0f);
    float m_ka = 0.2f; //ambiante
    float m_kd = 0.8f; //difuse
    float m_ks = 0.8f; //speculaire
    float m_shiny = 32.0f; //its a rare pokemon !
    glm::vec3 m_posLum = glm::vec3(0.0f, 5.0f, 0.0f);

protected:
    GLProgram *vp = nullptr;
    GLProgram *fp = nullptr;
    GLint l_Model = -1, l_View = -1, l_Proj = -1; 
    GLint l_posLum = -1, l_posCam = -1;           

    GLint l_ObjCol = -1, l_ka = -1, l_kd = -1;
    GLint l_ks = -1, l_shiny = -1;
};
