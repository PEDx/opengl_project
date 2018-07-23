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
GLboolean BLINN_PHONG = false;

using namespace std;

void reportGlInfo();
void Do_Movement();
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
unsigned int loadCubemap(vector<std::string> faces);
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

float skyboxVertices[] = {
    // positions
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f};

float vertices[] = {
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

float quadVertices[] = {
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f};

int main()
{
    GLFWwindow *window = createWindow();

    reportGlInfo();

    Shader lamp_shader("./shader/lamp.vs", "./shader/lamp.fs");
    Shader model_shader("./shader/model.vs", "./shader/model.fs");
    Shader quad_shader("./shader/quad.vs", "./shader/quad.fs");
    Shader skybox_shader("./shader/skybox.vs", "./shader/skybox.fs");
    Shader instance_shader("./shader/model_ins.vs", "./shader/model_ins.fs");
    Shader single_color_shader("./shader/singleColor.vs", "./shader/singleColor.fs");

    Model m_Model("./object/nanosuit.obj");
    Model m_RockModel("./object/rock.obj");

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDepthFunc(GL_LESS);

    unsigned int lightVAO, lightVBO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    vector<std::string> faces{
        "./object/darkskies/darkskies_ft.tga",
        "./object/darkskies/darkskies_bk.tga",
        "./object/darkskies/darkskies_up.tga",
        "./object/darkskies/darkskies_dn.tga",
        "./object/darkskies/darkskies_rt.tga",
        "./object/darkskies/darkskies_lf.tga",
    };

    unsigned int cubemapTexture = loadCubemap(faces);

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

    instance_shader.userShader();
    instance_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    instance_shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    instance_shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    instance_shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    for (int i = 0; i < 4; i++)
    {
        string t_str = "pointLights[";
        t_str += to_string(i);
        instance_shader.setVec3(t_str + "].position", pointLightPositions[i]);
        instance_shader.setVec3(t_str + "].ambient", 0.05f, 0.05f, 0.05f);
        instance_shader.setVec3(t_str + "].diffuse", 0.8f, 0.8f, 0.8f);
        instance_shader.setVec3(t_str + "].specular", 1.0f, 1.0f, 1.0f);
        instance_shader.setFloat(t_str + "].constant", 1.0f);
        instance_shader.setFloat(t_str + "].linear", 0.09);
        instance_shader.setFloat(t_str + "].quadratic", 0.032);
    }

    quad_shader.userShader();
    quad_shader.setInt("screenTexture", 0);

    // framebuffer configuration
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);           // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int amount = 10000;
    glm::mat4 *modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // 初始化随机种子
    float radius = 20.0;
    float offset = 3.0f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. 缩放：在 0.05 和 0.25f 之间缩放
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale / 2.5));

        // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. 添加到矩阵的数组中
        modelMatrices[i] = model;
    }

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < m_RockModel.meshes.size(); i++)
    {
        unsigned int VAO = m_RockModel.meshes[i].VAO;
        glBindVertexArray(VAO);
        // 顶点属性
        GLsizei vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 view = camera.GetViewMatrix();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f);

        Do_Movement();
        // glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.07f, 0.149f, 0.227f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        model_shader.userShader();
        model_shader.setMat4("view", view);
        model_shader.setMat4("projection", projection);
        model_shader.setVec3("viewPos", camera.Position);
        model_shader.setBool("blinn", BLINN_PHONG);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene // it's a bit too big for our scene, so scale it down
        model = glm::scale(model, glm::vec3(0.3f));
        model_shader.setMat4("model", model);
        m_Model.Draw(model_shader);

        instance_shader.userShader();
        instance_shader.setMat4("view", view);
        instance_shader.setMat4("projection", projection);
        instance_shader.setVec3("viewPos", camera.Position);

        instance_shader.setInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_RockModel.textures_loaded[0].id);

        for (unsigned int i = 0; i < m_RockModel.meshes.size(); i++)
        {
            glBindVertexArray(m_RockModel.meshes[i].VAO);
            glDrawElementsInstanced(
                GL_TRIANGLES, m_RockModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }

        lamp_shader.userShader();
        lamp_shader.setMat4("view", view);
        lamp_shader.setMat4("projection", projection);

        glBindVertexArray(lightVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.5f));
            lamp_shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glDepthFunc(GL_LEQUAL);
        skybox_shader.userShader();
        glm::mat4 view_x = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skybox_shader.setMat4("view", view_x);
        skybox_shader.setMat4("projection", projection);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(10.0f));
        skybox_shader.setMat4("model", model);
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        // glBindVertexArray(0);
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // // clear all relevant buffers
        // glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        // glClear(GL_COLOR_BUFFER_BIT);
        // quad_shader.userShader();
        // glBindVertexArray(quadVAO);
        // glBindTexture(GL_TEXTURE_2D, textureColorbuffer); // use the color attachment texture as the texture of the quad plane
        // glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &lightVBO);
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
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello OpenGL", NULL, NULL);
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
    glfwSetKeyCallback(window, key_callback);
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


bool keys[1024];
bool keysPressed[1024];
// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (keys[GLFW_KEY_B] && !keysPressed[GLFW_KEY_B])
    {
        BLINN_PHONG = !BLINN_PHONG;
        keysPressed[GLFW_KEY_B] = true;
    }
}
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key <= 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
            keysPressed[key] = false;
        }
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}