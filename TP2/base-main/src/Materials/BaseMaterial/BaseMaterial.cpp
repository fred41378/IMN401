
#include "BaseMaterial.h"
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>

BaseMaterial::BaseMaterial(std::string name) : MaterialGL(name) {

    vp = new GLProgram(MaterialPath + "BaseMaterial/Main-VS.glsl", GL_VERTEX_SHADER);
    fp = new GLProgram(MaterialPath + "BaseMaterial/Main-FS.glsl", GL_FRAGMENT_SHADER);

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fp, GL_FRAGMENT_SHADER_BIT);

    l_View = glGetUniformLocation(vp->getId(), "View");
    l_Proj = glGetUniformLocation(vp->getId(), "Proj");
    l_Model = glGetUniformLocation(vp->getId(), "Model");

    l_Time = glGetUniformLocation(vp->getId(), "u_time");
    l_Amp = glGetUniformLocation(vp->getId(), "u_amp");
    l_Freq = glGetUniformLocation(vp->getId(), "u_freq");

}

BaseMaterial::~BaseMaterial() {}

void BaseMaterial::render(Node *o) {

    m_ProgramPipeline->bind();

    o->drawGeometry(GL_TRIANGLES);
    m_ProgramPipeline->release();
}

void BaseMaterial::animate(Node *o, const float elapsedTime) {

    /**********************************************
    TP 2 - A completer
    Calculer et Transmettre les matrices Model View et Proj au shaders
    - Utilisez glm::value_ptr(mat) pour trouver le pointeur de la matrice mat a transmettre au GPU via la fonction glProgramUniform*()
    - Une matrice 4X4 se transmet grace a glProgramUniformMatrix4fv
    ***********************************************/

    m_time += elapsedTime * 0.001f;

    glm::mat4 Model = o->frame()->getModelMatrix();
    glm::mat4 View = scene->camera()->getViewMatrix();
    glm::mat4 Proj = scene->camera()->getProjectionMatrix();

    const GLuint vpId = vp->getId();
    if (l_Model >= 0) glProgramUniformMatrix4fv(vpId, l_Model, 1, GL_FALSE, glm::value_ptr(Model));
    if (l_View >= 0) glProgramUniformMatrix4fv(vpId, l_View, 1, GL_FALSE, glm::value_ptr(View));
    if (l_Proj >= 0) glProgramUniformMatrix4fv(vpId, l_Proj, 1, GL_FALSE, glm::value_ptr(Proj));

    if (l_Time >= 0) glProgramUniform1f(vpId, l_Time, m_time);
    if (l_Amp >= 0) glProgramUniform1f(vpId, l_Amp, m_amp);
    if (l_Freq >= 0) glProgramUniform1f(vpId, l_Freq, m_freq);
}

