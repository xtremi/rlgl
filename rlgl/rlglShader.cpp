#include "rlglShader.h"
#include "rlglObject.h"
#include "rlglMaterial.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace rlgl;

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

    if (!vertexPath.empty() && !fragmentPath.empty()) {
        create(vertexPath, fragmentPath);
    }

}

bool Shader::create(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string errmsg;
    GLuint vShader = compileShader(vertexPath, GL_VERTEX_SHADER, errmsg);
    if (!vShader) {
        std::cout << "Error while compiling vertex shader from \"" + vertexPath + "\"" << std::endl;
        std::cout << errmsg << std::endl;
        return false;
    }
    GLuint fShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER, errmsg);
    if (!fShader) {
        std::cout << "Error while compiling fragment shader from \"" + fragmentPath + "\"" << std::endl;
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



void StandardShader::setWorldUniforms(
    const glm::mat4x4& pvMat,
    const glm::vec3& camPos,
    const rlgl::WorldEnv& worldEnv) const
{
    StandardUniforms::setProjectViewMatrix(glID, pvMat);
}

void StandardShader::setObjectUniforms(rlgl::Object* obj) const {
    StandardUniforms::setModelMatrix(glID, obj->modelMatrix());
    StandardUniforms::setHighlight(glID, obj->hasHighlight());
    if (obj->hasColor()) {
        StandardUniforms::setColor(glID, obj->getColor());
    }
}

void TextureShader::setMaterialUniforms(const rlgl::MaterialPtr material) const {
    TextureUniforms::setTexture(glID, material);
}

void LightShader::setWorldUniforms(
    const glm::mat4x4& pvMat,
    const glm::vec3& camPos,
    const rlgl::WorldEnv& worldEnv) const
{
    StandardShader::setWorldUniforms(pvMat, camPos, worldEnv);
    setLightPos(glID, worldEnv.lights[0].pos);
    setLightAmbientIntensity(glID, worldEnv.lights[0].ambientIntensity);
    setLightSpecularIntensity(glID, worldEnv.lights[0].specularIntensity);
    setLightColor(glID, worldEnv.lights[0].color);
    setCameraPos(glID, camPos);
}


void LightMaterialShader::setMaterialUniforms(const rlgl::MaterialPtr material) const {
    MaterialLightPropertiesUniforms::setMaterialLightProperties(glID,
        *std::static_pointer_cast<LightProperties>(material).get());
}

void TextureLightShader::setMaterialUniforms(const rlgl::MaterialPtr material) const {
    TextureUniforms::setTexture(glID, material);
}

void TextureLightMaterialShader::setMaterialUniforms(const rlgl::MaterialPtr material) const {
    TextureLightShader::setMaterialUniforms(material);

    MaterialLightPropertiesUniforms::setMaterialLightProperties(glID, 
        *std::static_pointer_cast<LightProperties>(material).get());
}
