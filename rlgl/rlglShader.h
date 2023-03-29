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

    Shader(const std::string& vertexPath = "", const std::string& fragmentPath = "");
    void use() const;
    bool create(const std::string& vertexPath, const std::string& fragmentPath);


    virtual void setWorldUniforms(
        const glm::mat4x4& pvMat,
        const glm::vec3& camPos,
        const rlgl::WorldEnv& worldEnv) const {};
    virtual void setObjectUniforms(rlgl::Object* obj) const {};
    virtual void setMaterialUniforms(const rlgl::Material* material) const {};

private:
    GLuint compileShader(const std::string& filePath, GLenum shaderType, std::string& err);
};

class StandardShader : public Shader, public StandardUniforms {
public:
    using Shader::Shader;

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

/*!
    Standard shader
        + Texture
*/
class TextureShader : public StandardShader, public TextureUniforms {
public:
    using StandardShader::StandardShader;

    virtual void setMaterialUniforms(const rlgl::Material* material) const {
        TextureUniforms::setTexture(glID, material);
    }
};



/*!
    Standard shader
        + Light uniforms
*/
class LightShader : public StandardShader, public LightUniforms {
public:
    using StandardShader::StandardShader;

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

/*!
    Standard shader
        + Light uniforms
        + Material light properties uniforms
*/
class LightMaterialShader : public LightShader, public MaterialLightPropertiesUniforms {
public:
    using LightShader::LightShader;

    virtual void setMaterialUniforms(const rlgl::Material* material) const {
        MaterialLightPropertiesUniforms::setMaterialLightProperties(glID, *((LightProperties*)material));
    }
};

/*!
    Standard shader
        + Light uniforms
        + Texture
*/
class TextureLightShader : public LightShader, public TextureUniforms {
public:
    using LightShader::LightShader;

    virtual void setMaterialUniforms(const rlgl::Material* material) const {
        TextureUniforms::setTexture(glID, material);
    }    
};

/*!
    Standard shader
        + Light uniforms
        + Material light properties uniforms
        + Texture
*/
class TextureLightMaterialShader : public TextureLightShader, public MaterialLightPropertiesUniforms {
public:
    using TextureLightShader::TextureLightShader;

    virtual void setMaterialUniforms(const rlgl::Material* material) const {
        TextureLightShader::setMaterialUniforms(material);

        MaterialLightPropertiesUniforms::setMaterialLightProperties(glID, *((LightProperties*)material));
    }


};


}