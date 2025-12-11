#pragma once

#include "EffectGL.h"

class Flou : public EffectGL {
public:
    Flou(std::string name);

    ~Flou();

    virtual void apply(FrameBufferObject *src, FrameBufferObject *target);

    virtual void animate(const float elapsedTime);

    virtual void displayInterface() {};

protected:
    GLProgram *vp;
    GLProgram *fp;
    GLProgram *fpV;
    GLProgram *fpH;
    GLuint l_Texture = -1, l_Texture_h = -1, l_Texture_v = -1;

    GLuint l_flou_h = -1, l_flou_v = -1;

    FrameBufferObject *tmp = nullptr;
};