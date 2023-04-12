#pragma once
#include <glad/gl.h>
#include <string>
#include <glm/glm.hpp>
#include "rlglWorldEnv.h"
#include "rlglMaterial.h"

namespace rlgl {
    class Object;
    class LightProperties;
    class Shader;


class ShaderUniforms {
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


class ShaderUniformSetObject {
    friend rlgl::Shader;
    virtual void setUniformValues(rlgl::Object* obj) const {};
};

class ShaderUniformSetGlobal {
    friend rlgl::Shader;
    virtual void setUniformValues(
        const glm::mat4x4& pvMat,
        const rlgl::Camera& cam,
        const rlgl::WorldEnv& worldEnv) const {};
};

class ShaderUniformSetMaterial {
    friend rlgl::Shader;
    virtual void setUniformValues(const rlgl::MaterialPtr material) const {};
};

class ObjectColorUniform {
    void setColor(GLuint sID, const glm::vec4& color) const;
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
    void setTexture(GLuint sID, const rlgl::MaterialPtr material) const;
};


class MaterialLightPropertiesUniforms : public ShaderUniforms {
protected:
    void setMaterialLightProperties(GLuint sID, const rlgl::LightProperties& properties) const;

    void setMaterialAmbientProp(GLuint sID, const glm::vec3& factor) const;
    void setMaterialDiffuseProp(GLuint sID, const glm::vec3& factor) const;
    void setMaterialSpecularProp(GLuint sID, const glm::vec3& factor) const;
    void setMaterialShininessProp(GLuint sID, float factor) const;
};

class LightUniforms : public ShaderUniforms {
protected:
    void setLightProperties(
        GLuint sID,
        const rlgl::StandardLight& light,
        const glm::vec3& cameraPosition) const;

    void setLightPos(GLuint sID, const glm::vec3& position) const;
    void setLightAmbientIntensity(GLuint sID, float intensity) const;
    void setLightSpecularIntensity(GLuint sID, float intensity) const;
    void setLightColor(GLuint sID, const glm::vec3& color) const;
    void setCameraPos(GLuint sID, const glm::vec3& position) const;
};

class CamDirUniforms : public ShaderUniforms {
protected:
    void setCameraDirection(GLuint sID, const glm::vec3& direction) const;
};

}
