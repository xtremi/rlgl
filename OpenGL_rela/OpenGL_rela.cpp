#include <iostream>
#include "rlglBaseApp.h"


int main(int argc, char* argv[]) {

    glm::ivec2 windowSize(800, 600);

	rlgl::BaseApp app;
    if (int err = app.init(windowSize.x, windowSize.y)) {
        return err;
    }

    while (!app.windowClosed()){
        if (int err = app.loopIteration()) {
            return err;
        }
    }

	app.cleanUp();

    glfwTerminate();
    return 0;
}


/*
* 
* struct rlglContext {
    GLuint VBO, VAO, EBO;
    rlgl::Shader shader;

    GLuint textures[10];
};

bool triangleTest01_init(rlglContext& ctxt);
bool triangleTest02_init(rlglContext& ctxt);
void triangleTest01_renderRoutine(const rlglContext& ctxt);
void triangleTest02_renderRoutine(const rlglContext& ctxt);
bool triangleTest02_init(rlglContext& ctxt) {
    ctxt.shader.initialize("vertexShaderSimple.vs", "fragmentShaderSimple.fs");

    glGenVertexArrays(1, &ctxt.VAO);
    glBindVertexArray(ctxt.VAO);

    rlgl::cube_tex.vertices.genBuffer();
    rlgl::cube_tex.vertices.bindBuffer(GL_ARRAY_BUFFER);
    rlgl::cube_tex.vertices.bufferData(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    //position:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture:
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    //texture:
    int width, height, nrChannels;
    unsigned char* imgData = stbi_load(
        "..\\data\\checker.jpg", &width, &height, &nrChannels, 0);

    glGenTextures(1, &ctxt.textures[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ctxt.textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
    ctxt.shader.setInt("textureID", 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imgData);



    return true;
}


bool triangleTest01_init(rlglContext& ctxt) {
    
    ctxt.shader.initialize("vertexShaderSimple.vs", "fragmentShaderSimple.fs");

    glGenVertexArrays(1, &ctxt.VAO);
    glBindVertexArray(ctxt.VAO);
    
    rlgl::plane.vertices.genBuffer();
    rlgl::plane.vertices.bindBuffer(GL_ARRAY_BUFFER);
    rlgl::plane.vertices.bufferData(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    //position:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture:
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //indices:
    rlgl::plane.indices.genBuffer();
    rlgl::plane.indices.bindBuffer(GL_ELEMENT_ARRAY_BUFFER);
    rlgl::plane.indices.bufferData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

    //texture:
    int width, height, nrChannels;
    unsigned char* imgData = stbi_load(
        "C:\\coding\\my_projects\\LearnOpenGL\\OpenGL_rela\\data\\checker.jpg", &width, &height, &nrChannels, 0);
    
    glGenTextures(1, &ctxt.textures[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ctxt.textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
    ctxt.shader.setInt("textureID", 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imgData);
    return true;
}


void triangleTest02_renderRoutine(const rlglContext& ctxt) {
    float width = 800.f, height = 600.f;

    float curTime = glfwGetTime();

    ctxt.shader.use();

    glm::mat4 projM = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 viewM = glm::lookAt(glm::vec3(-2.f, -2.f, 1.5f), glm::vec3(0.f), glm::vec3(0.f, 0.f, 1.f));
    ctxt.shader.setMat4x4("projection", projM);
    ctxt.shader.setMat4x4("view", viewM);

    glm::mat4 translM = glm::translate(glm::mat4(1.5f), glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 rotM = glm::rotate(glm::mat4(1.f), curTime, glm::vec3(0.f, 1.f, 1.f));
    glm::mat4 scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.6f));
    glm::mat4 modelM = translM * scaleM * rotM;
    ctxt.shader.setMat4x4("model", modelM);

    //glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    //glBindTexture(GL_TEXTURE_2D, ctxt.textures[0]);
    glBindVertexArray(ctxt.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    translM = glm::translate(glm::mat4(1.f), glm::vec3(2.f, 0.f, 1.f));
    modelM = translM * scaleM * rotM;
    ctxt.shader.setMat4x4("model", modelM);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void triangleTest01_renderRoutine(const rlglContext& ctxt) {
    float width = 800.f, height = 600.f;

    float curTime = glfwGetTime();

    ctxt.shader.use();

    //glm::mat4 ortho = glm::ortho(0.0f, width, 0.0f, height, 0.1f, 100.0f);
    glm::mat4 projM = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    //glm::mat4 viewM = glm::translate(glm::mat4(1.f), glm::vec3(-0.f, -0.2f, -1.f));
    glm::mat4 viewM = glm::lookAt(glm::vec3(-2.f, -2.f, 1.5f), glm::vec3(0.f), glm::vec3(0.f, 0.f, 1.f));
    ctxt.shader.setMat4x4("projection", projM);
    ctxt.shader.setMat4x4("view", viewM);

    glm::mat4 rotM = glm::rotate(glm::mat4(1.f), curTime, glm::vec3(0.f, 0.f, 1.f));
    glm::mat4 scaleM = glm::scale(glm::mat4(2.0f), glm::vec3(0.6f));
    glm::mat4 modelM = scaleM * rotM;
    ctxt.shader.setMat4x4("model", modelM);

    //glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    //glBindTexture(GL_TEXTURE_2D, ctxt.textures[0]);
    glBindVertexArray(ctxt.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    modelM = glm::translate(scaleM, glm::vec3(1.5f, 0.f, 0.f));
    modelM = modelM * rotM;
    ctxt.shader.setMat4x4("model", modelM);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}*/




