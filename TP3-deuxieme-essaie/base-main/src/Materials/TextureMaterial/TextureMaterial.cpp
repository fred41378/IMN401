
#include "Node.h"
#include "TextureMaterial.h"
#include <glm/gtc/type_ptr.hpp>

TextureMaterial::TextureMaterial(std::string name) : MaterialGL(name) {

    vp = new GLProgram(MaterialPath + "TextureMaterial/TextureMaterial-VS.glsl", GL_VERTEX_SHADER);
    fp = new GLProgram(MaterialPath + "TextureMaterial/TextureMaterial-FS.glsl", GL_FRAGMENT_SHADER);
    const GLuint vpId = vp->getId();
    const GLuint fpId = fp->getId();

    m_ProgramPipeline->useProgramStage(vp, GL_VERTEX_SHADER_BIT);
    m_ProgramPipeline->useProgramStage(fp, GL_FRAGMENT_SHADER_BIT);

    l_View = glGetUniformLocation(vpId, "View");
    l_Proj = glGetUniformLocation(vpId, "Proj");
    l_Model = glGetUniformLocation(vpId, "Model");

    l_time = glGetUniformLocation(vpId, "u_time");

    l_ka = glGetUniformLocation(fpId, "u_ka");
    l_kd = glGetUniformLocation(fpId, "u_kd");
    l_ks = glGetUniformLocation(fpId, "u_ks");
    l_shiny = glGetUniformLocation(fpId, "u_shiny");

    l_objColor = glGetUniformLocation(fpId, "u_objColor");
    l_posLum = glGetUniformLocation(vpId, "u_posLum");
    l_posCam = glGetUniformLocation(vpId, "u_posCam");

    l_t1 = glGetUniformLocation(fpId, "T1");
    l_t2 = glGetUniformLocation(fpId, "T2");
    l_hasSecondTexture = glGetUniformLocation(fpId, "u_hasSecondTexture");
}

TextureMaterial::~TextureMaterial() {}

void TextureMaterial::render(Node *o) {

    m_ProgramPipeline->bind();
    const GLuint vpId = vp->getId();
    const GLuint fpId = fp->getId();

    if (m_texture1 && l_t1 >= 0) {
        glBindTextureUnit(0, m_texture1->getId());
        glProgramUniform1i(fpId, l_t1, 0);
    }
    bool hasTex2 = (m_texture2 != nullptr);
    if (l_hasSecondTexture >= 0) glProgramUniform1f(fpId, l_hasSecondTexture, hasTex2 ? 1 : 0);
    if (m_texture2 && l_t2 >= 0) {
        glBindTextureUnit(1, m_texture2->getId());
        glProgramUniform1i(fpId, l_t2, 1);
    }
    o->drawGeometry(GL_TRIANGLES);
    m_ProgramPipeline->release();
}

void TextureMaterial::setColor(const glm::vec3 &rgb) {
    m_objColor = rgb;
}

void TextureMaterial::setTexture1(Texture2D *texture1) {
    m_texture1 = texture1;
} 

void TextureMaterial::setTexture2(Texture2D *texture2) {
    m_texture2 = texture2;
}

void TextureMaterial::animate(Node *o, const float elapsedTime) {

    /**********************************************
    TP 2 - A completer
    Calculer et Transmettre les matrices Model View et Proj au shaders
    - Utilisez glm::value_ptr(mat) pour trouver le pointeur de la matrice mat a transmettre au GPU via la fonction glProgramUniform*()
    - Une matrice 4X4 se transmet grace a glProgramUniformMatrix4fv
    ***********************************************/

    if (elapsedTime < 10) m_time += elapsedTime;

    glm::mat4 Projection = scene->camera()->getProjectionMatrix();
    glm::mat4 View = scene->camera()->getViewMatrix();
    glm::mat4 Model = o->frame()->getModelMatrix();

    const GLuint vpId = vp->getId();
    const GLuint fpId = fp->getId();
    if (l_Proj >= 0) glProgramUniformMatrix4fv(vpId, l_Proj, 1, GL_FALSE, glm::value_ptr(Projection));
    if (l_View >= 0) glProgramUniformMatrix4fv(vpId, l_View, 1, GL_FALSE, glm::value_ptr(View));
    if (l_Model >= 0) glProgramUniformMatrix4fv(vpId, l_Model, 1, GL_FALSE, glm::value_ptr(Model));

    if (l_time >= 0) glProgramUniform1f(vpId, l_time, m_time);

    if (l_ka >= 0) glProgramUniform1f(fpId, l_ka, m_ka);
    if (l_kd >= 0) glProgramUniform1f(fpId, l_kd, m_kd);
    if (l_ks >= 0) glProgramUniform1f(fpId, l_ks, m_ks);
    if (l_shiny >= 0) glProgramUniform1f(fpId, l_shiny, m_shiny);
    if (l_objColor >= 0) glProgramUniform3fv(fpId, l_objColor, 1, glm::value_ptr(m_objColor));

    glm::vec3 P(0.0f);
    glm::vec3 lights_obj;

    Node *L1 = scene->getNode("Light1");

    lights_obj = o->frame()->convertPtFrom(P, L1->frame());

    if (l_posLum >= 0) glProgramUniform3fv(vpId, l_posLum, 1, glm::value_ptr(lights_obj));

    glm::vec3 p0 = glm::vec3(0.0f);
    glm::vec3 pCam = o->frame()->convertPtFrom(p0, scene->camera()->frame());
    if (l_posCam >= 0) glProgramUniform3fv(vpId, l_posCam, 1, glm::value_ptr(pCam));

}
