#include "PhongMaterial.h"
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>

PhongMaterial::PhongMaterial(std::string name) : MaterialGL(std::move(name)) {
    vp = new GLProgram(MaterialPath + "BaseMaterial/Main-VS.glsl", GL_VERTEX_SHADER);
    fp = new GLProgram(MaterialPath + "BaseMaterial/Main-FS.glsl", GL_FRAGMENT_SHADER);

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fp, GL_FRAGMENT_SHADER_BIT);

    const GLuint vpId = vp->getId();
    const GLuint fpId = fp->getId();

    //VS
    l_Model = glGetUniformLocation(vpId, "Model");
    l_View = glGetUniformLocation(vpId, "View");
    l_Proj = glGetUniformLocation(vpId, "Proj");
    l_posLum = glGetUniformLocation(vpId, "u_posLum");
    l_posCam = glGetUniformLocation(vpId, "u_posCam");

    //FS
    l_ObjCol = glGetUniformLocation(fpId, "u_objectColor");
    l_ka = glGetUniformLocation(fpId, "u_ka");
    l_kd = glGetUniformLocation(fpId, "u_kd");
    l_ks = glGetUniformLocation(fpId, "u_ks");
    l_shiny = glGetUniformLocation(fpId, "u_shiny");
}

void PhongMaterial::render(Node *o) {
    m_ProgramPipeline->bind();
    o->drawGeometry(GL_TRIANGLES);
    m_ProgramPipeline->release();
}

void PhongMaterial::setColor(const glm::vec3 &rgb) {
    m_objectColor = glm::clamp(rgb, glm::vec3(0.0f), glm::vec3(1.0f));
}

void PhongMaterial::animate(Node *o, const float elapsedTime) {
    const GLuint vpId = vp->getId();
    const GLuint fpId = fp->getId();

    glm::mat4 Model = o->frame()->getModelMatrix();
    glm::mat4 View = scene->camera()->getViewMatrix();
    glm::mat4 Proj = scene->camera()->getProjectionMatrix();

    if (l_Model >= 0) glProgramUniformMatrix4fv(vpId, l_Model, 1, GL_FALSE, glm::value_ptr(Model));
    if (l_View >= 0) glProgramUniformMatrix4fv(vpId, l_View, 1, GL_FALSE, glm::value_ptr(View));
    if (l_Proj >= 0) glProgramUniformMatrix4fv(vpId, l_Proj, 1, GL_FALSE, glm::value_ptr(Proj));

    Node *Lumiere = scene->getNode("Lumiere");
    glm::vec3 P(0.0f);
    glm::vec3 p_objet = o->frame()->convertPtFrom(P, Lumiere->frame());
    if (l_posLum >= 0) glProgramUniform3fv(vpId, l_posLum, 1, glm::value_ptr(p_objet));

    glm::mat4 V_inv = glm::inverse(View);
    glm::vec3 cam_world = glm::vec3(V_inv * glm::vec4(0, 0, 0, 1));
    glm::vec3 cam_object = glm::vec3(glm::inverse(Model) * glm::vec4(cam_world, 1));
    if (l_posCam >= 0) glProgramUniform3fv(vpId, l_posCam, 1, glm::value_ptr(cam_object));

    if (l_ObjCol >= 0) glProgramUniform3fv(fpId, l_ObjCol, 1, glm::value_ptr(m_objectColor));
    if (l_ka >= 0) glProgramUniform1f(fpId, l_ka, m_ka);
    if (l_kd >= 0) glProgramUniform1f(fpId, l_kd, m_kd);
    if (l_ks >= 0) glProgramUniform1f(fpId, l_ks, m_ks);
    if (l_shiny >= 0) glProgramUniform1f(fpId, l_shiny, m_shiny);
}
