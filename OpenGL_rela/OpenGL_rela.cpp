#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "rlglShader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

struct rlglContext {
    GLuint VBO, VAO, EBO;
    rlglShader shader;

    GLint uniformColor;
};

bool triangleTest01_init(rlglContext& ctxt);
void triangleTest01_renderRoutine(const rlglContext& ctxt);



int main(int argc, char* argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    rlglContext rlglCtxt;
    if (!triangleTest01_init(rlglCtxt)) {
        return 1;
    }

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);

        triangleTest01_renderRoutine(rlglCtxt);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}


static const float vertices[] = {
     0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // top right
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom left
    -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f  // top left 
};

static const GLuint indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};




bool triangleTest01_init(rlglContext& ctxt) {
    
    glGenVertexArrays(1, &ctxt.VAO);
    glBindVertexArray(ctxt.VAO);
    
    glGenBuffers(1, &ctxt.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, ctxt.VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ctxt.EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctxt.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ctxt.shader.init("vertexShaderSimple.vs", "fragmentShaderSimple.fs");

    //ctxt.uniformColor = glGetUniformLocation(ctxt.shaderProgram, "ourColor");


    return true;
}

void triangleTest01_renderRoutine(const rlglContext& ctxt) {

    float curTime = glfwGetTime();
    float offset = (std::sinf(curTime) / 2.f);
    
    ctxt.shader.use();
    ctxt.shader.setFloat("offsetX", offset);
    //glUniform4f(ctxt.uniformColor, 0.f, greenVal, 0.f, 1.f);

    glBindVertexArray(ctxt.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}




