#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>

#include "rlglShaderUniforms.h"
#include "rlglWorldEnv.h"
#include "rlglObject.h"
#include "rlglMaterial.h"

#include <string>


namespace rlgl{

std::string readFile(const std::string& filePath);

/*!
    Defines a GLSL shader
    Currently has to be composed of vertex and fragment shader
*/
class Shader
{
public:
    GLuint glID;

    Shader();
    bool initialize(const std::string& vertexPath, const std::string& fragmentPath);
    GLuint compileShader(const std::string& filePath, GLenum shaderType, std::string& err);

    void use() const;

    virtual void setWorldUniforms(
        const glm::mat4x4& pvMat,
        const glm::vec3& camPos,
        const rlgl::WorldEnv& worldEnv) const {};
    virtual void setObjectUniforms(rlgl::Object* obj) const {};
    virtual void setMaterialUniforms(const rlgl::Material* material) const {};
};

class StandardShader : public Shader, public StandardUniforms {
public:
    virtual void setWorldUniforms(
        const glm::mat4x4& pvMat,
        const glm::vec3& camPos,
        const rlgl::WorldEnv& worldEnv) const 
    {
        StandardUniforms::setProjectViewMatrix(glID, pvMat);
    }

    virtual void setObjectUniforms(rlgl::Object* obj) const {
        StandardUniforms::setModelMatrix(glID, obj->modelMatrix());
        StandardUniforms::setHighlight(glID, obj->hasHighlight());
        if (obj->hasColor()) {
            StandardUniforms::setColor(glID, obj->getColor());
        }
    }
};

class TextureShader : public StandardShader, public TextureUniforms {
public:
    virtual void setMaterialUniforms(const rlgl::Material* material) const {
        TextureUniforms::setTexture(glID, material);
    }
};

class LightShader : public StandardShader, public LightUniforms {
public:
    virtual void setWorldUniforms(
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

};

class TextureLightShader : public LightShader, public virtual TextureUniforms {
public:
    virtual void setMaterialUniforms(const rlgl::Material* material) const {
        TextureUniforms::setTexture(glID, material);
    }
    
};

}