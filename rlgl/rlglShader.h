#pragma once
#include "rlglShaderUniforms.h"
#include "rlglWorldEnv.h"
#include "rlglCamera.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace rlgl{

    class Object;
    class Material;
    typedef std::shared_ptr<Material> MaterialPtr;

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
        const rlgl::Camera& cam,
        const rlgl::WorldEnv& worldEnv) const {};
    virtual void setObjectUniforms(rlgl::Object* obj) const {};
    virtual void setMaterialUniforms(const rlgl::MaterialPtr material) const {};

    //Used for enabling/diabling gl settings before and after the shader is used.
    virtual void preRender() const {}
    virtual void postRender() const {}

private:
    GLuint compileShader(const std::string& filePath, GLenum shaderType, std::string& err);
};

typedef std::shared_ptr<rlgl::Shader> ShaderPtr;


/*!
    Standard shader
*/
class StandardShader : public Shader, public StandardUniforms {
public:
    using Shader::Shader;

    virtual void setWorldUniforms(
        const glm::mat4x4&    pvMat,
        const rlgl::Camera&   cam,
        const rlgl::WorldEnv& worldEnv) const;

    virtual void setObjectUniforms(rlgl::Object* obj) const;
};

/*!
    Background shader (not used - deprecated)
*/
class BackgroundShader : public StandardShader, public CamDirUniforms {
public:
    using StandardShader::StandardShader;

    virtual void setWorldUniforms(
        const glm::mat4x4& pvMat,
        const rlgl::Camera& cam,
        const rlgl::WorldEnv& worldEnv) const;
};

/*!
    Standard shader
        + Texture
*/
class TextureShader : public StandardShader, public TextureUniforms {
public:
    using StandardShader::StandardShader;

    virtual void setMaterialUniforms(const rlgl::MaterialPtr material) const;
};

/*!
    Standard shader
        + Texture
        + Remove translation from view matrix
*/
class CubeMapShader : public TextureShader {
public:
    using TextureShader::TextureShader;

    virtual void setWorldUniforms(
        const glm::mat4x4& pvMat,
        const rlgl::Camera& cam,
        const rlgl::WorldEnv& worldEnv) const;

    virtual void preRender() const;
    virtual void postRender() const;
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
        const rlgl::Camera& cam,
        const rlgl::WorldEnv& worldEnv) const;
};

/*!
    Standard shader
        + Light uniforms
        + Material light properties uniforms
*/
class LightMaterialShader : public LightShader, public MaterialLightPropertiesUniforms {
public:
    using LightShader::LightShader;

    virtual void setMaterialUniforms(const rlgl::MaterialPtr material) const;
};

/*!
    Standard shader
        + Light uniforms
        + Texture
*/
class TextureLightShader : public LightShader, public TextureUniforms {
public:
    using LightShader::LightShader;

    virtual void setMaterialUniforms(const rlgl::MaterialPtr material) const;
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

    virtual void setMaterialUniforms(const rlgl::MaterialPtr material) const;
};

}