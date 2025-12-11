
#include "Bloom.h"
#include <glm/gtc/type_ptr.hpp>

Bloom::Bloom(std::string name) : EffectGL(name) {
    vp = new GLProgram(EffectPath + "common/Effect-VS.glsl", GL_VERTEX_SHADER);
    fpBright = new GLProgram(EffectPath + "Bloom/BloomBright-FS.glsl", GL_FRAGMENT_SHADER);
    fpCombine = new GLProgram(EffectPath + "Bloom/BloomCombine-FS.glsl", GL_FRAGMENT_SHADER);

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fpBright, GL_FRAGMENT_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fpCombine, GL_FRAGMENT_SHADER_BIT);
    m_ProgramPipeline->link();
    m_ProgramPipeline->printInfoLog();

    flou = new Flou("bloomFlou");

    fbo1 = new FrameBufferObject("myFBO1");
    fbo2 = new FrameBufferObject("myFBO2");

    l_CombineSceneTexture = glGetUniformLocation(fpCombine->getId(), "myFBO1");
    l_CombineBloomTexture = glGetUniformLocation(fpCombine->getId(), "myFBO2");


    glProgramUniform1i(fpCombine->getId(), l_CombineSceneTexture, 0);
    glProgramUniform1i(fpCombine->getId(), l_CombineBloomTexture, 1);
}

Bloom::~Bloom() {
    delete fbo1;
    delete fbo2;
    delete flou;
}

void Bloom::apply(FrameBufferObject *src, FrameBufferObject *target) {

    fbo1->enable();
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTextureUnit(0, src->getColorTexture()->getId());
    m_ProgramPipeline->useProgramStage(fpBright, GL_FRAGMENT_SHADER_BIT);

    m_ProgramPipeline->bind();
    drawQuad();
    m_ProgramPipeline->release();

    fbo1->disable();

    flou->apply(fbo1, fbo2);

    glBindTextureUnit(0, src->getColorTexture()->getId());
    glBindTextureUnit(1, fbo2->getColorTexture()->getId());

    m_ProgramPipeline->useProgramStage(fpCombine, GL_FRAGMENT_SHADER_BIT);

    if (target)
        target->enable();

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ProgramPipeline->bind();
    drawQuad();
    m_ProgramPipeline->release();
    if (target)
        target->disable();
}

