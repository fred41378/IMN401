
#include "TextureMaterial.h"
#include "Node.h"
#include <glm/gtc/type_ptr.hpp>

TextureMaterial::TextureMaterial(std::string name) : MaterialGL(name) {

    vp = new GLProgram(MaterialPath + "TextureMaterial/Main-VS.glsl", GL_VERTEX_SHADER);
    fp = new GLProgram(MaterialPath + "TextureMaterial/Main-FS.glsl", GL_FRAGMENT_SHADER);
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
    l_nbLumiere = glGetUniformLocation(fpId, "u_nbLumiere");
    l_posCam = glGetUniformLocation(vpId, "u_posCam");
    l_id = glGetUniformLocation(vpId, "u_id");

    l_sampler2d_1 = glGetUniformLocation(fpId, "T1");
    l_sampler2d_2 = glGetUniformLocation(fpId, "T2");
    l_sampler2d_N = glGetUniformLocation(fpId, "NormalMap");
}

TextureMaterial::~TextureMaterial() {}

void TextureMaterial::render(Node *o) {

    m_ProgramPipeline->bind();

    const GLuint vpId = vp->getId();
    const GLuint fpId = fp->getId();

    // Texture
    glBindTextureUnit(0, m_texture1->getId());
    if (l_sampler2d_1 >= 0) glProgramUniform1i(fpId, l_sampler2d_1, 0);
    glBindTextureUnit(1, m_texture2->getId());
    if (l_sampler2d_2 >= 0) glProgramUniform1i(fpId, l_sampler2d_2, 1);
    glBindTextureUnit(2, m_normal->getId());
    if (l_sampler2d_N >= 0) glProgramUniform1i(fpId, l_sampler2d_N, 2);

    o->drawGeometry(GL_TRIANGLES);
    m_ProgramPipeline->release();
}

void TextureMaterial::setColor(const glm::vec3 &rgb) {
    m_objColor = rgb;
}

void TextureMaterial::setTexture1( Texture2D *texture) {
    m_texture1 = texture;
}
void TextureMaterial::setTexture2(Texture2D *texture) {
    m_texture2 = texture;
}
void TextureMaterial::setNormal(Texture2D *normal) {
    m_normal = normal;
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

    Node *L1 = scene->getNode("Light1");
   /* Node *L2 = scene->getNode("Light2");
    Node *L3 = scene->getNode("Light3");*/

    glm::vec3 lights_obj = o->frame()->convertPtFrom(P, L1->frame());
    //lights_obj[1] = o->frame()->convertPtFrom(P, L2->frame());
    //lights_obj[2] = o->frame()->convertPtFrom(P, L3->frame());

    if (l_posLum >= 0) glProgramUniform3fv(vpId, l_posLum, 1, glm::value_ptr(lights_obj));

    if (l_nbLumiere >= 0) glProgramUniform1i(fpId, l_nbLumiere, 1);

    glm::vec3 p0 = glm::vec3(0.0f);
    glm::vec3 pCam = o->frame()->convertPtFrom(p0, scene->camera()->frame());
    if (l_posCam >= 0) glProgramUniform3fv(vpId, l_posCam, 1, glm::value_ptr(pCam));
    
    float m_id = 0.0f;

    /*if (o->getName() == "Sol") {
        m_id = 1.0f;
    }*/

    if (l_id >= 0) glProgramUniform1f(vpId, l_id, m_id);

    // anais

    glm::vec3 originLapin = glm::vec3(0, 0, 0);
    glm::vec3 posLapin = o->frame()->convertPtFrom(originLapin, scene->getNode("Bunny")->frame());
    GLuint l_posLapin = glGetUniformLocation(vpId, "u_posLapin");
    glProgramUniform3fv(vpId, l_posLapin, 1, glm::value_ptr(posLapin));

    
}
