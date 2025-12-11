
#include "Flou.h"
#include <glm/gtc/type_ptr.hpp>

Flou::Flou(std::string name) : EffectGL(name) {
    vp = new GLProgram(EffectPath + "common/Effect-VS.glsl", GL_VERTEX_SHADER);
    fpV = new GLProgram(EffectPath + "Flou/FlouV-FS.glsl", GL_FRAGMENT_SHADER);
    fpH = new GLProgram(EffectPath + "Flou/FlouH-FS.glsl", GL_FRAGMENT_SHADER); 

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fpH, GL_FRAGMENT_SHADER_BIT);
    m_ProgramPipeline->link();
    m_ProgramPipeline->printInfoLog();

    l_flou_h = glGetUniformLocation(fpH->getId(), "myFBO");
    l_flou_v = glGetUniformLocation(fpV->getId(), "myFBO");

    tmp = new FrameBufferObject("tmpFBO");
}

Flou::~Flou() {}

void Flou::apply(FrameBufferObject *src, FrameBufferObject *target) {

    glDisable(GL_DEPTH_TEST);
    tmp->enable();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ProgramPipeline->useProgramStage(fpH, GL_FRAGMENT_SHADER_BIT);

    if (l_flou_h >= 0) {
        glBindTextureUnit(0, src->getColorTexture()->getId());
        glProgramUniform1i(fpH->getId(), l_flou_h, 0);
    }
    m_ProgramPipeline->bind();
    drawQuad();
    m_ProgramPipeline->release();

    tmp->disable();
    if (target)
        target->enable();

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ProgramPipeline->useProgramStage(fpV, GL_FRAGMENT_SHADER_BIT);

    if (l_flou_v >= 0) {
        glBindTextureUnit(0, tmp->getColorTexture()->getId());
        glProgramUniform1i(fpV->getId(), l_flou_v, 0);
    }

    m_ProgramPipeline->bind();
    drawQuad();
    m_ProgramPipeline->release();
    if (target)
        target->disable();
}

void Flou::animate(const float elapsedTime) {}
