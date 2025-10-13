

#ifndef _BaseMaterial_
#define _BaseMaterial_

#include "MaterialGL.h"

class BaseMaterial : public MaterialGL {
public:
    BaseMaterial(std::string name = "");

    ~BaseMaterial();

    virtual void render(Node *o);

    virtual void animate(Node *o, const float elapsedTime);

    virtual void displayInterface(){};

protected:
    GLProgram *vp;
    GLProgram *fp;

    GLuint l_View, l_Proj, l_Model; // location of uniforms

    GLint l_Time = -1, l_Amp = -1, l_Freq = -1;

    float m_time = 0.0f;
    float m_amp = 0.01f;
    float m_freq = 0.5f;
};

#endif