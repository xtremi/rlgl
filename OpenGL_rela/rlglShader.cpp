#include "rlglShader.h"
#include <glm/gtc/type_ptr.hpp>


rlglShader::rlglShader() {}

bool rlglShader::init(const std::string& vertexPath, const std::string& fragmentPath) {
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


    ID = glCreateProgram();
    glAttachShader(ID, vShader);
    glAttachShader(ID, fShader);
    glLinkProgram(ID);

    GLint success;
    errmsg.resize(512);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, &errmsg[0]);
        std::cout << errmsg;
        return false;
    }
    glDeleteShader(vShader);
    glDeleteShader(fShader);
    return true;
}

GLuint rlglShader::compileShader(const std::string& filePath, GLenum shaderType, std::string& err) {

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

void rlglShader::use() const {
    glUseProgram(ID);
}

void rlglShader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void rlglShader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void rlglShader::setMat4x4(const std::string& name, const glm::mat4x4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void rlglShader::setUint(const std::string& name, GLuint value) const {
    glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}
void rlglShader::setInt(const std::string& name, GLint value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}



std::string readFile(const std::string& filePath) {
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