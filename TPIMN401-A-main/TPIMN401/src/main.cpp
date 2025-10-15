#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include "utils.hpp"
#include <glm.hpp>
#include <time.h>
#include <chrono>

namespace IMN401 {

    const float M_PI = 3.14159265359f;
    void printProgramError(GLuint program) {
        GLint isLinked = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (!isLinked) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
            std::cerr << "[Program link error] " << std::string(infoLog.begin(), infoLog.end()) << "\n";
            glDeleteProgram(program);
        }
        else {
            std::cout << "Shader program link: OK\n";
        }
    }

    void printPipelineError(GLuint pipeline) {
        GLint isValid = GL_FALSE;
        glGetProgramPipelineiv(pipeline, GL_VALIDATE_STATUS, &isValid);
        if (!isValid) {
            GLint maxLength = 0;
            glGetProgramPipelineiv(pipeline, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramPipelineInfoLog(pipeline, maxLength, &maxLength, infoLog.data());
            std::cerr << "[Pipeline validate error] " << std::string(infoLog.begin(), infoLog.end()) << "\n";
            glDeleteProgramPipelines(1, &pipeline);
        }
        else {
            std::cout << "Pipeline: OK\n";
        }
    }

    struct MeshData {
        std::vector<glm::vec2> vertices;  
        std::vector<unsigned int> indices;
    };

    MeshData generateCircleFanMesh(int n, float radius = 0.5f)
    {
        MeshData mesh;
        if (n < 3) n = 3;

        mesh.vertices.emplace_back(0.0f, 0.0f);

        const float twoPi = 2.0f * static_cast<float>(M_PI);
        const float step = twoPi / static_cast<float>(n);

        for (int i = 0; i < n; ++i) {
            float a = i * step;
            float x = radius * std::cos(a);
            float y = radius * std::sin(a);
            mesh.vertices.emplace_back(x, y);
        }

        mesh.indices.reserve(3 * n);
        for (int i = 0; i < n; ++i) {
            unsigned int i0 = 0;
            unsigned int i1 = static_cast<unsigned int>(i + 1);
            unsigned int i2 = static_cast<unsigned int>((i + 1) % n + 1);
            mesh.indices.push_back(i0);
            mesh.indices.push_back(i1);
            mesh.indices.push_back(i2);
        }

        return mesh;
    }

    int main() {

        time_t start = time(0);
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            return EXIT_FAILURE;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        const int width = 800, height = 800;
        GLFWwindow* window = glfwCreateWindow(width, height, "TP - From Scratch", nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create window\n";
            glfwTerminate();
            return EXIT_FAILURE;
        }
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);


        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD\n";
            glfwTerminate();
            return EXIT_FAILURE;
        }

        int n = 30;
        MeshData cercle = generateCircleFanMesh(n, 0.5f);

        const float points[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.0f,  0.5f
        };


        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;
        glCreateVertexArrays(1, &vao);
        glCreateBuffers(1, &vbo);
        glCreateBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER,
            cercle.vertices.size() * sizeof(glm::vec2),
            cercle.vertices.data(),
            GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            cercle.indices.size() * sizeof(unsigned int),
            cercle.indices.data(),
            GL_STATIC_DRAW);


        /*glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);*/

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
        glBindVertexArray(0);


        std::string strVS = readFile("shaders/triangle-vs.glsl");
        std::string strFS = readFile("shaders/triangle-fs.glsl");
        const GLchar* vsCode = strVS.c_str();
        const GLchar* fsCode = strFS.c_str();

        GLuint vpId = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vsCode);
        GLuint fpId = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fsCode);
        printProgramError(vpId);
        printProgramError(fpId);

        GLuint pipeline = 0;
        glCreateProgramPipelines(1, &pipeline);
        glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vpId);
        glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fpId);
        glValidateProgramPipeline(pipeline);
        printPipelineError(pipeline);

        auto t0 = std::chrono::steady_clock::now();

        GLint l_elapsedTime = glGetUniformLocation(vpId, "u_elapsedTime");
        

        glBindProgramPipeline(pipeline);

        glViewport(0, 0, width, height);
        glClearColor(0.08f, 0.09f, 0.10f, 1.0f);

        //Render
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            auto tNow = std::chrono::steady_clock::now();
            float elapsedTime = std::chrono::duration<float>(tNow - t0).count();

            std::cout << elapsedTime;
            if (l_elapsedTime >= 0) glProgramUniform1f(vpId, l_elapsedTime, elapsedTime);

            glClear(GL_COLOR_BUFFER_BIT);

            glBindProgramPipeline(pipeline);
            glBindVertexArray(vao);

            glDrawElements(GL_TRIANGLES,
                static_cast<GLsizei>(cercle.indices.size()),
                GL_UNSIGNED_INT,
                (void*)0);

            glBindVertexArray(0);
            glfwSwapBuffers(window);
        }

        glDeleteProgramPipelines(1, &pipeline);
        glDeleteProgram(vpId);
        glDeleteProgram(fpId);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);

        glfwTerminate();
        return EXIT_SUCCESS;
    }

}

int main() {
    return IMN401::main();
}
