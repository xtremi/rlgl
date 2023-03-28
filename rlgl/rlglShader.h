#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "rlglWorldEnv.h"
#include "rlglObject.h"
#include <string>


namespace rlgl{

std::string readFile(const std::string& filePath);

class Shader
{
public:
    GLuint glID;

    Shader();
    bool initialize(const std::string& vertexPath, const std::string& fragmentPath);
    GLuint compileShader(const std::string& filePath, GLenum shaderType, std::string& err);

    void use() const;
    void setBool(const std::string& name, bool value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4x4(const std::string& name, const glm::mat4x4& value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setUint(const std::string& name, GLuint value) const;
    void setInt(const std::string& name, GLint value) const;
    void setBool(const std::string& name, GLboolean value) const;


    virtual void setWorldUniforms(
        const glm::mat4x4& pvMat,
        const glm::vec3& camPos,
        const rlgl::WorldEnv& worldEnv) const {}

    virtual void setObjectUniforms(rlgl::Object* obj) const {}
};


class StandardShader : public Shader {
public:
    void setWorldUniforms(
        const glm::mat4x4& pvMat,
        const glm::vec3& camPos,
        const rlgl::WorldEnv& worldEnv) const;

    void setObjectUniforms(rlgl::Object* obj) const;

protected:
    void setProjectViewMatrix(const glm::mat4x4& mat) const;
    void setColor(const glm::vec4& color) const;
    void setHighlight(bool highlight) const;
    void setModelMatrix(const glm::mat4x4& mat) const;


};


class StandardLightShader : public StandardShader {
public:  
    void setWorldUniforms(
        const glm::mat4x4& pvMat,
        const glm::vec3& camPos,
        const rlgl::WorldEnv& worldEnv) const;

    void setObjectUniforms(rlgl::Object* obj) const;

protected:
    void setLightPos(const glm::vec3& position) const;
    void setLightAmbientIntensity(float intensity) const;
    void setLightSpecularIntensity(float intensity) const;
    void setLightColor(const glm::vec3& color) const;
    void setCameraPos(const glm::vec3& position) const;
};

}