
#include "EngineGL.h"
#include "Scene.h"

#include "Materials/BaseMaterial/BaseMaterial.h"
#include "Materials/RotationMaterial/RotationMaterial.h"
#include "Materials/PhongMaterial/PhongMaterial.h"
#include "Materials/TextureMaterial/TextureMaterial.h"

#include "Texture2D.h"

bool EngineGL::init() {
    LOG_INFO << "Initializing Scene" << std::endl;

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, m_Width, m_Height);
    setClearColor(glm::vec4(0.5, 0.5, 0.5, 1.0));

    // TP 2 *******************************************

    // Création d'un materiau de Base
    BaseMaterial *material = new BaseMaterial("IMN401-TP2");
    TextureMaterial *textureMatBunny = new TextureMaterial("TextureMaterial");
    PhongMaterial *phongMatSol = new PhongMaterial("phongMatSol");

    //Textures
    Texture2D *textureBunny1 = new Texture2D(ObjPath + "Textures/Bunny1.png");
    Texture2D *textureBunny2 = new Texture2D(ObjPath + "Textures/Bunny2.png");

    // d'un objet, méthode détaillée
    textureMatBunny->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
    textureMatBunny->setTexture1(textureBunny1);
    textureMatBunny->setTexture2(textureBunny2);
    Node *bunny = scene->getNode("Bunny");
    bunny->setModel(scene->m_Models.get<ModelGL>(ObjPath + "Bunny.obj"));
    bunny->frame()->scale(glm::vec3(30.0));
    bunny->setMaterial(textureMatBunny);
    scene->getSceneNode()->adopt(bunny);

    Node *L1 = scene->getNode("Light1");
    L1->setModel(scene->m_Models.get<ModelGL>(ObjPath + "Sphere.obj"));
    L1->setMaterial(new PhongMaterial("Lumiere1"));
    L1->frame()->translate(glm::vec3(10.0f, 2.0f, 0.0f));
    L1->frame()->scale(glm::vec3(2.0f));

    /*Node *L2 = scene->getNode("Light2");
    L2->setModel(scene->m_Models.get<ModelGL>(ObjPath + "Sphere.obj"));
    L2->setMaterial(new PhongMaterial("Lumiere2"));
    L2->frame()->translate(glm::vec3(-10.0f, 2.0f, 0.0f));
    L2->frame()->scale(glm::vec3(2.0f));

    Node *L3 = scene->getNode("Light3");
    L3->setModel(scene->m_Models.get<ModelGL>(ObjPath + "Sphere.obj"));
    L3->setMaterial(new PhongMaterial("Lumiere3"));
    L3->frame()->translate(glm::vec3(0.0f, 2.0f, 10.0f));
    L3->frame()->scale(glm::vec3(2.0f));*/


    Node *A = new Node("A");
    A->setMaterial(new RotationMaterial("RotA"));
    A->frame()->scale(glm::vec3(1.0f / 30.0f));
    bunny->adopt(A);
    A->adopt(L1);
    /*A->adopt(L2);
    A->adopt(L3);*/

    // sol
    Node *sol = scene->getNode("Sol");
    sol->setModel(scene->m_Models.get<ModelGL>(ObjPath + "wall.obj"));
    sol->setMaterial(phongMatSol);
    phongMatSol->setColor(glm::vec3(0.0f, 1.0, 1.0f));
    sol->frame()->scale(glm::vec3(6.0f, 2.0f, 6.0f));
    sol->frame()->translate(glm::vec3(0.0f, -1.15f, 0.0f));

    scene->getSceneNode()->adopt(sol);

    /*Node *A = new Node("A");
    A->setMaterial(new RotationMaterial("RotA"));
    A->frame()->scale(glm::vec3(1.0f / 30.0f));
    bunny->adopt(A);

    Node *L = new Node("SphereL");
    L->setModel(scene->m_Models.get<ModelGL>(ObjPath + "Sphere.obj"));
    L->setMaterial(new BaseMaterial("BaseMatSphere"));
    A->adopt(L);
    L->frame()->translate(glm::vec3(5.5f, 0.0f, 0.0f));

    L->frame()->scale(glm::vec3(2.0f));*/

    //Texture 2D
    

    setupEngine();
    return (true);
}

void EngineGL::render() {
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (unsigned int i = 0; i < allNodes->nodes.size(); i++)
        allNodes->nodes[i]->render();
}

void EngineGL::animate(const float elapsedTime) {
    // Animate each node
    for (unsigned int i = 0; i < allNodes->nodes.size(); i++) {
        allNodes->nodes[i]->animate(elapsedTime);
    }

    // Update Camera Buffer
    scene->camera()->updateBuffer();
}

void EngineGL::onWindowResize(int w, int h) {
    m_Width = w;
    m_Height = h;
    glViewport(0, 0, w, h);
    float ratio = (float)w / (float)h;

    scene->resizeViewport(w, h);
    scene->camera()->setPerspectiveProjection(glm::radians(45.0f), ratio, 1.0f, 2000.0f);
}

void EngineGL::setClearColor(glm::vec4 color) {
    glClearColor(color.x, color.y, color.z, color.w);
}

void EngineGL::displayInterface() {
    if (myFBO) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("FBOs")) {
                ImGui::MenuItem(myFBO->getName().c_str(), NULL, &(myFBO->show_interface));
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        myFBO->displayInterface();
    }
}

// Message callbck error for getting OpenGL problems
// All credits to https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions/blob/master/README.md#gltexture
void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const *message, void const *user_param) {
    auto const src_str = [source]() {
        switch (source) {
        case GL_DEBUG_SOURCE_API: return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        default: return "";
        }
    }();

    auto const type_str = [type]() {
        switch (type) {
        case GL_DEBUG_TYPE_ERROR: return "ERROR";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
        case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
        case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
        case GL_DEBUG_TYPE_MARKER: return "MARKER";
        case GL_DEBUG_TYPE_OTHER: return "OTHER";
        default: return "";
        }
    }();

    auto const severity_str = [severity]() {
        switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
        case GL_DEBUG_SEVERITY_LOW: return "LOW";
        case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
        case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        default: return "";
        }
    }();
    LOG_INFO << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

EngineGL::EngineGL(int width, int height) {
    m_Width = width;
    m_Height = height;

    myFBO = NULL;

    scene = Scene::getInstance();
    scene->resizeViewport(m_Width, m_Height);
}

EngineGL::~EngineGL() {}

void EngineGL::setupEngine() {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback(message_callback, nullptr);

    this->allNodes = new NodeCollector();
    allNodes->collect(scene->getRoot());

    LOG_INFO << "initialisation complete" << std::endl;
}