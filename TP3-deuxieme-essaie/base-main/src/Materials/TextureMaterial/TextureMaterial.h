#include "MaterialGL.h"
#include <Texture2D.h>

class TextureMaterial : public MaterialGL {
public:
    TextureMaterial(std::string name = "");

    ~TextureMaterial();

    virtual void render(Node *o);

    virtual void animate(Node *o, const float elapsedTime);

    virtual void displayInterface() {};

    void setColor(const glm::vec3 &rgb);
    void setTexture1(Texture2D *texture);

    glm::vec3 m_objColor = glm::vec3(0.0f, 0.0f, 0.0f);
    float m_ka = 0.2f;
    float m_kd = 0.5f;
    float m_ks = 1.0f;
    float m_shiny = 64.0f;
    float m_time = 0.0f;

    Texture2D *m_texture1 = nullptr;

protected:
    GLProgram *vp;
    GLProgram *fp;

    GLuint l_View, l_Proj, l_Model; // location of uniforms
    GLuint l_time = -1;

    GLuint l_ka = -1, l_kd = -1, l_ks = -1, l_shiny = -1,
           l_objColor = -1;

    GLuint l_posLum = -1, l_posCam = -1;

    GLuint l_t1 = -1;

};
