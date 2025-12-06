#include "Bloom.h"

static inline void bindTex(GLuint programId, GLint loc, Texture2D *tex, int unit) {
    if (loc >= 0 && tex) {
        glBindTextureUnit(unit, tex->getId());
        glProgramUniform1i(programId, loc, unit);
    }
}

Bloom::Bloom(std::string name, int w, int h) : EffectGL(name) {
    vp = new GLProgram(EffectPath + "common/Effect-VS.glsl", GL_VERTEX_SHADER);

    fpExtract = new GLProgram(EffectPath + "Bloom/BloomExtract-FS.glsl", GL_FRAGMENT_SHADER);
    fpCombine = new GLProgram(EffectPath + "Bloom/BloomCombine-FS.glsl", GL_FRAGMENT_SHADER);

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fpExtract, GL_FRAGMENT_SHADER_BIT);
    m_ProgramPipeline->link();
    m_ProgramPipeline->printInfoLog();

    l_src = glGetUniformLocation(fpExtract->getId(), "u_src");
    l_limite = glGetUniformLocation(fpExtract->getId(), "u_limite");
    l_softness = glGetUniformLocation(fpExtract->getId(), "u_softness");

    l_src_combine = glGetUniformLocation(fpCombine->getId(), "u_src");
    l_bloom_combine = glGetUniformLocation(fpCombine->getId(), "u_bloom");
    l_intensite = glGetUniformLocation(fpCombine->getId(), "u_intensite");

    FBO1 = new FrameBufferObject("Bloom_FBO1", w, h);
    FBO2 = new FrameBufferObject("Bloom_FBO2", w, h);

    flou = new Flou("BloomFlou", w, h);
    flou->m_k = 4;
}

Bloom::~Bloom() {}

void Bloom::apply(FrameBufferObject *src, FrameBufferObject *target) {
    glDisable(GL_DEPTH_TEST);

    FBO1->enable();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ProgramPipeline->useProgramStage(fpExtract, GL_FRAGMENT_SHADER_BIT);
    bindTex(fpExtract->getId(), l_src, src->getColorTexture(), 0);
    if (l_limite >= 0) glProgramUniform1f(fpExtract->getId(), l_limite, m_limite);
    if (l_softness >= 0) glProgramUniform1f(fpExtract->getId(), l_softness, m_softness);

    m_ProgramPipeline->bind();
    drawQuad();
    m_ProgramPipeline->release();

    FBO1->disable();

    flou->apply(FBO1, FBO2);

    if (target) target->enable();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ProgramPipeline->useProgramStage(fpCombine, GL_FRAGMENT_SHADER_BIT);

    bindTex(fpCombine->getId(), l_src_combine, src->getColorTexture(), 0);
    bindTex(fpCombine->getId(), l_bloom_combine, FBO2->getColorTexture(), 1);
    if (l_intensite >= 0) glProgramUniform1f(fpCombine->getId(), l_intensite, m_intensite);

    m_ProgramPipeline->bind();
    drawQuad();
    m_ProgramPipeline->release();

    if (target) target->disable();
}
