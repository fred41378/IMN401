#include "Flou.h"

Flou::Flou(std::string name, int w, int h) : EffectGL(name) {
    vp = new GLProgram(EffectPath + "common/Effect-VS.glsl", GL_VERTEX_SHADER);
    fph = new GLProgram(EffectPath + "Flou/FlouH-FS.glsl", GL_FRAGMENT_SHADER);
    fpv = new GLProgram(EffectPath + "Flou/FlouV-FS.glsl", GL_FRAGMENT_SHADER);

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fph, GL_FRAGMENT_SHADER_BIT);
    m_ProgramPipeline->link();
    m_ProgramPipeline->printInfoLog();

    l_src_h = glGetUniformLocation(fph->getId(), "u_src");
    l_k_h = glGetUniformLocation(fph->getId(), "u_k");

    l_src_v = glGetUniformLocation(fpv->getId(), "u_src");
    l_k_v = glGetUniformLocation(fpv->getId(), "u_k");

    tmp = new FrameBufferObject("tmpFlou", w, h);
}

Flou::~Flou() {
    delete vp;
    delete fph;
    delete fpv;
    delete tmp;
}

static inline void bindSrc(GLuint programId, GLint locSampler, FrameBufferObject *fbo, int unit) {
    if (locSampler >= 0) {
        glBindTextureUnit(unit, fbo->getColorTexture()->getId());
        glProgramUniform1i(programId, locSampler, unit);
    }
}

void Flou::apply(FrameBufferObject *src, FrameBufferObject *target) {
    glDisable(GL_DEPTH_TEST);

    tmp->enable();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ProgramPipeline->useProgramStage(fph, GL_FRAGMENT_SHADER_BIT);

    bindSrc(fph->getId(), l_src_h, src, 0);
    if (l_k_h >= 0) glProgramUniform1i(fph->getId(), l_k_h, m_k);

    m_ProgramPipeline->bind();
    drawQuad();
    m_ProgramPipeline->release();

    tmp->disable();

    if (target) target->enable();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ProgramPipeline->useProgramStage(fpv, GL_FRAGMENT_SHADER_BIT);

    bindSrc(fpv->getId(), l_src_v, tmp, 0);
    if (l_k_v >= 0) glProgramUniform1i(fpv->getId(), l_k_v, m_k);

    m_ProgramPipeline->bind();
    drawQuad();
    m_ProgramPipeline->release();

    if (target) target->disable();
}
