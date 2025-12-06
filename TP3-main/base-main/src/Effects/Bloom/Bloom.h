#pragma once
#include "EffectGL.h"
#include "../Flou/Flou.h"
#include "FrameBufferObject.h"

class Bloom : public EffectGL {
public:
    Bloom(std::string name, int w, int h);
    ~Bloom();

    void apply(FrameBufferObject *src, FrameBufferObject *target) override;

    float m_threshold = 0.4f;
    float m_softness = 0.5f;
    float m_intensity = 1.5f;

private:
    GLProgram *vp = nullptr;
    GLProgram *fpExtract = nullptr; 
    GLProgram *fpCombine = nullptr; 

    GLint l_src_extract = -1;
    GLint l_threshold = -1;
    GLint l_softness = -1;

    GLint l_src_combine = -1;
    GLint l_bloom_combine = -1;
    GLint l_intensity = -1;

    FrameBufferObject *FBO1 = nullptr; 
    FrameBufferObject *FBO2 = nullptr; 
    Flou *flou = nullptr;              
};
