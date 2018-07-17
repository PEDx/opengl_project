//
//  main.cpp
//  opengl-project
//
//  Created by ped on 2018/6/6.
//  Copyright © 2018年 ped. All rights reserved.
//

#include "includes/glad.h"
#include "includes/glfw3.h"
#include <iostream>
#include <string>
#include "shader.hpp"
#include "camera.hpp"
#include "model.hpp"

#include "includes/glm/glm.hpp"
#include "includes/glm/gtc/matrix_transform.hpp"
#include "includes/glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"

const float SCR_WIDTH = 800;
const float SCR_HEIGHT = 600;

using namespace std;

void reportGlInfo();
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
unsigned int loadTexture(char const *path);
GLFWwindow *createWindow();

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;
bool firstMouse = true;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
float cubeVertices[] = {
    // Back face
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom-left
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // bottom-right
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // top-left
    // Front face
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // bottom-right
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-right
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // top-right
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // top-left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
    // Left face
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top-right
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-left
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // top-right
                                     // Right face
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-left
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // top-right
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  // bottom-right
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,    // top-left
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,   // bottom-left
    // Bottom face
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // top-left
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,   // bottom-left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // bottom-right
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
    // Top face
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // top-right
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // bottom-right
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f   // bottom-left
};
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f),
    glm::vec3(0.0f, 0.0f, -3.0f)};

int main()
{
    GLFWwindow *window = createWindow();

    reportGlInfo();

    Shader lamp_shader("./shader/lamp.vs", "./shader/lamp.fs");
    Shader model_shader("./shader/model.vs", "./shader/model.fs");
    Shader single_color_shader("./shader/singleColor.vs", "./shader/singleColor.fs");

    Model m_Model("./object/nanosuit.obj");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    unsigned int lightVAO, VBO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

//         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    model_shader.userShader();

    model_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    model_shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    model_shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    model_shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    for (int i = 0; i < 4; i++)
    {
        string t_str = "pointLights[";
        t_str += to_string(i);
        model_shader.setVec3(t_str + "].position", pointLightPositions[i]);
        model_shader.setVec3(t_str + "].ambient", 0.05f, 0.05f, 0.05f);
        model_shader.setVec3(t_str + "].diffuse", 0.8f, 0.8f, 0.8f);
        model_shader.setVec3(t_str + "].specular", 1.0f, 1.0f, 1.0f);
        model_shader.setFloat(t_str + "].constant", 1.0f);
        model_shader.setFloat(t_str + "].linear", 0.09);
        model_shader.setFloat(t_str + "].quadratic", 0.032);
    }

    glClearColor(0.07f, 0.149f, 0.227f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // also clear the depth buffer now!

        glm::mat4 view = camera.GetViewMatrix();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f);

        model_shader.userShader();
        model_shader.setMat4("view", view);
        model_shader.setMat4("projection", projection);
        model_shader.setVec3("viewPos", camera.Position);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene // it's a bit too big for our scene, so scale it down
        model = glm::scale(model, glm::vec3(0.2f));
        model_shader.setMat4("model", model);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        m_Model.Draw(model_shader);

        single_color_shader.userShader();
        single_color_shader.setMat4("view", view);
        single_color_shader.setMat4("projection", projection);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene // it's a bit too big for our scene, so scale it down
        model = glm::scale(model, glm::vec3(0.202f));
        single_color_shader.setMat4("model", model);

        m_Model.Draw(single_color_shader);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);

        lamp_shader.userShader();
        lamp_shader.setMat4("view", view);
        lamp_shader.setMat4("projection", projection);

        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lamp_shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}

GLFWwindow *createWindow()
{
    if (!glfwInit())
    {
        throw "glfwInit failed!";
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Mac OS X required
#endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello OpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!window)
    {
        glfwTerminate();
        throw "create window failed!";
    }
    // Must be under the glfwMakeContextCurrent function call
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw "Failed to initialize GLAD";
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

void reportGlInfo()
{
    cout << "\n\n>>>>>>>>>>>>>>>>>>>>>> OpenGL Info <<<<<<<<<<<<<<<<<<<<<<<\n"
         << endl;
    cout << "OpenGL Vendor:" << glGetString(GL_VENDOR) << endl;
    cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL Version:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;
    cout << "\n>>>>>>>>>>>>>>>>>>>>>> OpenGL Info <<<<<<<<<<<<<<<<<<<<<<<\n\n"
         << endl;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
