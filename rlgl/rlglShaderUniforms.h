#pragma once
#include <glad/gl.h>
#include <string>
#include <glm/glm.hpp>
#include "rlglWorldEnv.h"
#include "rlglObject.h"
#include "rlglMaterial.h"

class ShaderUniforms {
public:
    /*virtual void setWorldUniforms(
        GLuint sID,
        const glm::mat4x4& pvMat,
        const glm::vec3& camPos,
        const rlgl::WorldEnv& worldEnv) const { }

    virtual void setObjectUniforms(GLuint sID, rlgl::Object* obj) const { };
    virtual void setMaterialUniforms(GLuint sID, rlgl::Object* obj) const { };*/

protected:
    static void setBool(GLuint sID, const std::string& name, bool value);
    static void setFloat(GLuint sID, const std::string& name, float value);
    static void setMat4x4(GLuint sID, const std::string& name, const glm::mat4x4& value);
    static void setVec3(GLuint sID, const std::string& name, const glm::vec3& value);
    static void setVec4(GLuint sID, const std::string& name, const glm::vec4& value);
    static void setUint(GLuint sID, const std::string& name, GLuint value);
    static void setInt(GLuint sID, const std::string& name, GLint value);
    static void setBool(GLuint sID, const std::string& name, GLboolean value);
};

class StandardUniforms : public ShaderUniforms {
protected:
    void setProjectViewMatrix(GLuint sID, const glm::mat4x4& mat) const;
    void setColor(GLuint sID, const glm::vec4& color) const;
    void setHighlight(GLuint sID, bool highlight) const;
    void setModelMatrix(GLuint sID, const glm::mat4x4& mat) const;
};

class TextureUniforms : public ShaderUniforms {
protected:
    void setTexture(GLuint sID, const rlgl::Material* material) const;
};

class LightUniforms : public ShaderUniforms {
protected:
    void setLightPos(GLuint sID, const glm::vec3& position) const;
    void setLightAmbientIntensity(GLuint sID, float intensity) const;
    void setLightSpecularIntensity(GLuint sID, float intensity) const;
    void setLightColor(GLuint sID, const glm::vec3& color) const;
    void setCameraPos(GLuint sID, const glm::vec3& position) const;
};

