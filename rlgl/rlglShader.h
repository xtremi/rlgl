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

    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    void use() const;
    bool create(const std::string& vertexPath, const std::string& fragmentPath);

    virtual void setGlobalUniforms(
        const glm::mat4x4& pvMat,
        const rlgl::Camera& cam,
        const rlgl::WorldEnv& worldEnv) const;
    virtual void setObjectUniforms(rlgl::Object* obj) const;
    virtual void setMaterialUniforms(const rlgl::MaterialPtr material) const;

    //Used for enabling/diabling gl settings before and after the shader is used.
    virtual void preRender() const {}
    virtual void postRender() const {}

    void addGlobalUniforms(std::shared_ptr<uniforms::ShaderUniformSetGlobal> u) {
        uniformsGlobal.push_back(u);
    }
    void addMaterialUniforms(std::shared_ptr<uniforms::ShaderUniformSetMaterial> u) {
        uniformsMaterial.push_back(u);
    }
    void addObjectUniforms(std::shared_ptr<uniforms::ShaderUniformSetObject> u) {
        uniformsObj.push_back(u);
    }

protected:
    GLuint compileShader(const std::string& filePath, GLenum shaderType, std::string& err);
    std::vector<std::shared_ptr<uniforms::ShaderUniformSetObject>>   uniformsObj;
    std::vector<std::shared_ptr<uniforms::ShaderUniformSetGlobal>>   uniformsGlobal;
    std::vector<std::shared_ptr<uniforms::ShaderUniformSetMaterial>> uniformsMaterial;
};

typedef std::shared_ptr<rlgl::Shader> ShaderPtr;


/*!
    Standard shader
*/
class StandardShader : public Shader {
public:
    StandardShader(const std::string& v, const std::string& f) : Shader(v, f) {
        addGlobalUniforms(std::make_shared<uniforms::ProjectViewUniform>());
        addObjectUniforms(std::make_shared<uniforms::StandardObjectUniforms>());
    }

};

/*!
    Background shader (not used - deprecated)
*/
class BackgroundShader : public StandardShader{
public:
    BackgroundShader(const std::string& v, const std::string& f) : StandardShader(v, f) {
        addGlobalUniforms(std::make_shared<uniforms::CamDirUniform>());
    }
};

/*!
    Standard shader
        + Texture
*/
class TextureShader : public StandardShader{
public:
    TextureShader(const std::string& v, const std::string& f) : StandardShader(v, f) {
        addMaterialUniforms(std::make_shared<uniforms::TextureUniforms>());
    }
};

/*!
    Standard shader
        + Texture
        + Remove translation from view matrix
*/
class CubeMapShader : public TextureShader {
public:
    using TextureShader::TextureShader;

    virtual void setGlobalUniforms(
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
class LightShader : public StandardShader {
public:
    LightShader(const std::string& v, const std::string& f) : StandardShader(v, f) {
        addGlobalUniforms(std::make_shared<uniforms::LightUniforms>());
    }
};

/*!
    Standard shader
        + Light uniforms
        + Material light properties uniforms
*/
class LightMaterialShader : public LightShader{
public:
    LightMaterialShader(const std::string& v, const std::string& f) : LightShader(v, f) {
        addMaterialUniforms(std::make_shared<uniforms::MaterialLightPropertiesUniforms>());
    }
};

/*!
    Standard shader
        + Light uniforms
        + Texture
*/
class TextureLightShader : public LightShader{
public:
    TextureLightShader(const std::string& v, const std::string& f) : LightShader(v, f) {
        addMaterialUniforms(std::make_shared<uniforms::TextureUniforms>());
    }
};

/*!
    Standard shader
        + Light uniforms
        + Texture
        + Material light properties uniforms
*/
class TextureLightMaterialShader : public TextureLightShader {
public:
    TextureLightMaterialShader(const std::string& v, const std::string& f) : TextureLightShader(v, f) {
        addMaterialUniforms(std::make_shared<uniforms::MaterialLightPropertiesUniforms>());
    }
};

}