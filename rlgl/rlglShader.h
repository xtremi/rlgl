#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

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
};

}