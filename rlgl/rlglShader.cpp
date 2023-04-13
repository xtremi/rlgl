#include "rlglShader.h"
#include "rlglObject.h"
#include "rlglMaterial.h"
#include "rlglConfig.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace rlgl;

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


void Shader::setGlobalUniforms(
    const glm::mat4x4& pvMat,
    const rlgl::Camera& cam,
    const rlgl::WorldEnv& worldEnv) const
{
    for (const rlgl::ShaderUniformSetGlobal& uniformSet : uniformsGlobal) {
        uniformSet.setUniformValues(pvMat, cam, worldEnv);
    }
}

void Shader::setObjectUniforms(rlgl::Object* obj) const
{
    for (const rlgl::ShaderUniformSetObject& uniformSet : uniformsObj) {
        uniformSet.setUniformValues(obj);
    }
}

void Shader::setMaterialUniforms(const rlgl::MaterialPtr material) const
{
    for (const rlgl::ShaderUniformSetMaterial& uniformSet : uniformsMaterial) {
        uniformSet.setUniformValues(material);
    }
}




void StandardShader::setGlobalUniforms(
    const glm::mat4x4& pvMat,
    const rlgl::Camera& cam,
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

void BackgroundShader::setGlobalUniforms(
    const glm::mat4x4& pvMat,
    const rlgl::Camera& cam,
    const rlgl::WorldEnv& worldEnv) const
{
    StandardShader::setGlobalUniforms(pvMat, cam, worldEnv);
    CamDirUniforms::setCameraDirection(glID, cam.front);
}

void TextureShader::setMaterialUniforms(const rlgl::MaterialPtr material) const {
    TextureUniforms::setTexture(glID, material);
}

void CubeMapShader::setGlobalUniforms(
    const glm::mat4x4& pvMat,
    const rlgl::Camera& cam,
    const rlgl::WorldEnv& worldEnv) const
{
    glm::mat4 pvMatNoTransl = cam.projectionMatrix() * cam.viewMatrix_noTranslation();
    StandardUniforms::setProjectViewMatrix(glID, pvMatNoTransl);
}

void CubeMapShader::preRender() const {
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
}

void CubeMapShader::postRender() const {
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void LightShader::setGlobalUniforms(
    const glm::mat4x4& pvMat,
    const rlgl::Camera& cam,
    const rlgl::WorldEnv& worldEnv) const
{
    StandardShader::setGlobalUniforms(pvMat, cam, worldEnv);
    setLightPos(glID, worldEnv.lights[0].pos);
    setLightAmbientIntensity(glID, worldEnv.lights[0].ambientIntensity);
    setLightSpecularIntensity(glID, worldEnv.lights[0].specularIntensity);
    setLightColor(glID, worldEnv.lights[0].color);
    setCameraPos(glID, cam.position);
}


void LightMaterialShader::setMaterialUniforms(const rlgl::MaterialPtr material) const {
    MaterialLightPropertiesUniforms::setMaterialLightProperties(glID,
        std::static_pointer_cast<LightPropMaterial>(material)->lightProperties);
}

void TextureLightShader::setMaterialUniforms(const rlgl::MaterialPtr material) const {
    TextureUniforms::setTexture(glID, material);
}

void TextureLightMaterialShader::setMaterialUniforms(const rlgl::MaterialPtr material) const {
    TextureLightShader::setMaterialUniforms(material);

    MaterialLightPropertiesUniforms::setMaterialLightProperties(glID, 
        std::static_pointer_cast<TextureLightPropMaterial>(material)->lightProperties);
}
