#pragma once
#include "EffectGL.h"

class Flou : public EffectGL {
public:
    Flou(std::string name, int w, int h);
    ~Flou();

    void apply(FrameBufferObject *src, FrameBufferObject *target) override;

    int m_k = 7;

private:
    GLProgram *vp = nullptr;
    GLProgram *fp = nullptr;
    GLProgram *fph = nullptr;
    GLProgram *fpv = nullptr;

    GLint l_src_h = -1;
    GLint l_k_h = -1;

    GLint l_src_v = -1;
    GLint l_k_v = -1;

    FrameBufferObject *tmp = nullptr;
};
