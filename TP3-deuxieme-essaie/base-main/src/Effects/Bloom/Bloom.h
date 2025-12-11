#pragma once

#include "EffectGL.h"
#include <Effects/Flou/Flou.h>

class Bloom : public EffectGL {
public:
    Bloom(std::string name);

    ~Bloom();

    virtual void apply(FrameBufferObject *src, FrameBufferObject *target);

    virtual void displayInterface() {};

protected:
    GLProgram *vp;
    GLProgram *fpBright;
    GLProgram *fpCombine;


    GLuint l_fbo1 = -1, l_fbo2 = -1;

    FrameBufferObject *fbo1 = nullptr;
    FrameBufferObject *fbo2 = nullptr;
    GLuint l_CombineSceneTexture, l_CombineBloomTexture;
    Flou *flou = nullptr;
};
