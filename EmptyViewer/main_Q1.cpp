#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

#define M_PI 3.14159265358979323846

GLuint LoadShader(const char* vertexPath, const char* fragmentPath) {
    std::ifstream vFile(vertexPath), fFile(fragmentPath);
    std::stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();
    std::string vCode = vStream.str(), fCode = fStream.str();
    const char* vShaderCode = vCode.c_str(), * fShaderCode = fCode.c_str();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return shaderProgram;
}

void createSphere(std::vector<float>& vertices, std::vector<float>& normals, std::vector<unsigned int>& indices) {
    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;

    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
            float xSeg = (float)x / X_SEGMENTS;
            float ySeg = (float)y / Y_SEGMENTS;
            float xPos = std::cos(xSeg * 2.0f * M_PI) * std::sin(ySeg * M_PI);
            float yPos = std::cos(ySeg * M_PI);
            float zPos = std::sin(xSeg * 2.0f * M_PI) * std::sin(ySeg * M_PI);

            vertices.push_back(xPos);
            vertices.push_back(yPos);
            vertices.push_back(zPos);

            normals.push_back(xPos);
            normals.push_back(yPos);
            normals.push_back(zPos);
        }
    }

    for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
        for (unsigned int x = 0; x < X_SEGMENTS; ++x) {
            unsigned int i1 = y * (X_SEGMENTS + 1) + x;
            unsigned int i2 = i1 + X_SEGMENTS + 1;
            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i1 + 1);
            indices.push_back(i1 + 1);
            indices.push_back(i2);
            indices.push_back(i2 + 1);
        }
    }
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(512, 512, "Phong Shader", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW 초기화 실패" << std::endl;
        return -1;
    }

    glViewport(0, 0, 512, 512);
    glEnable(GL_DEPTH_TEST);

    GLuint shaderProgram = LoadShader("Phong.vert", "Phong.frag");

    std::vector<float> vertices, normals;
    std::vector<unsigned int> indices;
    createSphere(vertices, normals, indices);

    std::vector<float> interleaved;
    for (size_t i = 0; i < vertices.size() / 3; ++i) {
        interleaved.push_back(vertices[3 * i + 0]);
        interleaved.push_back(vertices[3 * i + 1]);
        interleaved.push_back(vertices[3 * i + 2]);
        interleaved.push_back(normals[3 * i + 0]);
        interleaved.push_back(normals[3 * i + 1]);
        interleaved.push_back(normals[3 * i + 2]);
    }

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, interleaved.size() * sizeof(float), interleaved.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -7.0f));
        model = glm::scale(model, glm::vec3(2.0f));

        glm::mat4 view = glm::lookAt(
            glm::vec3(0, 0, 0),     // eye
            glm::vec3(0, 0, -1),    // center
            glm::vec3(0, 1, 0)      // up
        );

        glm::mat4 projection = glm::frustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1000.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glUniform3f(glGetUniformLocation(shaderProgram, "ka"), 0.0f, 1.0f, 0.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "kd"), 0.0f, 0.5f, 0.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "ks"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(shaderProgram, "shininess"), 32.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "ambientIntensity"), 0.2f);
        glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), -4.0f, 4.0f, -3.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), 0.0f, 0.0f, 0.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
