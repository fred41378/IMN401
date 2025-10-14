#include "PhongMaterial.h"
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>

PhongMaterial::PhongMaterial(std::string name) : MaterialGL(std::move(name)) {
    // Point to your actual shader paths
    vp = new GLProgram(MaterialPath + "BaseMaterial/Main-VS.glsl", GL_VERTEX_SHADER);
    fp = new GLProgram(MaterialPath + "BaseMaterial/Main-FS.glsl", GL_FRAGMENT_SHADER);

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fp, GL_FRAGMENT_SHADER_BIT);

    l_Model = glGetUniformLocation(vp->getId(), "Model");
    l_View = glGetUniformLocation(vp->getId(), "View");
    l_Proj = glGetUniformLocation(vp->getId(), "Proj");

    l_ObjCol = glGetUniformLocation(vp->getId(), "u_objectColor");
    l_ka = glGetUniformLocation(vp->getId(), "u_ka");
    l_kd = glGetUniformLocation(vp->getId(), "u_kd");
    l_posLum = glGetUniformLocation(vp->getId(), "u_posLum");
}

void PhongMaterial::render(Node *o) {
    m_ProgramPipeline->bind();
    o->drawGeometry(GL_TRIANGLES);
    m_ProgramPipeline->release();
}

void PhongMaterial::animate(Node *o, const float elapsedTime) {
    const GLuint vpId = vp->getId();
    glm::mat4 Model = o->frame()->getModelMatrix();
    glm::mat4 View = scene->camera()->getViewMatrix();
    glm::mat4 Proj = scene->camera()->getProjectionMatrix();

    if (l_Model >= 0) glProgramUniformMatrix4fv(vpId, l_Model, 1, GL_FALSE, glm::value_ptr(Model));
    if (l_View >= 0) glProgramUniformMatrix4fv(vpId, l_View, 1, GL_FALSE, glm::value_ptr(View));
    if (l_Proj >= 0) glProgramUniformMatrix4fv(vpId, l_Proj, 1, GL_FALSE, glm::value_ptr(Proj));

    if (l_ObjCol >= 0) glProgramUniform3fv(vpId, l_ObjCol, 1, glm::value_ptr(m_objectColor));
    if (l_ka >= 0) glProgramUniform1f(vpId, l_ka, m_ka);
    if (l_kd >= 0) glProgramUniform1f(vpId, l_kd, m_kd);

    Node *Lumiere = scene->getNode("Lumiere");
    glm::vec3 P = glm::vec3(0.0f);
    glm::vec3 P_objet = o->frame()->convertPtFrom(P, Lumiere->frame());

    if (l_posLum >= 0) glProgramUniform3fv(vpId, l_posLum, 1, glm::value_ptr(P_objet));
}
