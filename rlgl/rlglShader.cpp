#include "rlglShader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace rlgl;

Shader::Shader() {}

bool Shader::initialize(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string errmsg;
    GLuint vShader = compileShader(vertexPath, GL_VERTEX_SHADER, errmsg);
    if (!vShader) {
        std::cout << errmsg << std::endl;
        return false;
    }
    GLuint fShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER, errmsg);
    if (!fShader) {
        std::cout << errmsg << std::endl;
        return false;
    }


    glID = glCreateProgram();
    glAttachShader(glID, vShader);
    glAttachShader(glID, fShader);
    glLinkProgram(glID);

    GLint success;
    errmsg.resize(512);
    glGetProgramiv(glID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(glID, 512, NULL, &errmsg[0]);
        std::cout << errmsg;
        return false;
    }
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return true;
}

GLuint Shader::compileShader(const std::string& filePath, GLenum shaderType, std::string& err) {

    const std::string shaderContent = readFile(filePath);
    if (shaderContent.empty()) {
        err = "Could not read shader file " + filePath;
        return 0;
    }
    GLchar const* shaderContentPtr = &shaderContent[0];

    GLuint shaderID;
    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderContentPtr, NULL);
    glCompileShader(shaderID);

    int success;
    static const int MAX_ERROR_MESSAGE_SIZE = 512;
    err.resize(MAX_ERROR_MESSAGE_SIZE);
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderID, MAX_ERROR_MESSAGE_SIZE, NULL, &err[0]);
        return 0;
    }
    return shaderID;
}

void Shader::use() const {
    glUseProgram(glID);
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(glID, name.c_str()), (int)value);
}
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(glID, name.c_str()), value);
}
void Shader::setMat4x4(const std::string& name, const glm::mat4x4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(glID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3f(glGetUniformLocation(glID, name.c_str()), value.x, value.y, value.z);
}
void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4f(glGetUniformLocation(glID, name.c_str()), value.x, value.y, value.z, value.w);
}
void Shader::setUint(const std::string& name, GLuint value) const {
    glUniform1ui(glGetUniformLocation(glID, name.c_str()), value);
}
void Shader::setInt(const std::string& name, GLint value) const {
    glUniform1i(glGetUniformLocation(glID, name.c_str()), value);
}



std::string rlgl::readFile(const std::string& filePath) {
    std::ifstream file;
    file.open(filePath);
    if (!file.is_open()) {
        return "";
    }
    std::string fileContent;
    fileContent.assign(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>());
    return fileContent;
}