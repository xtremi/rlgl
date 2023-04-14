#include "rlglShaderUniforms.h"
#include "rlglShader.h"
#include "rlglObject.h"
#include "rlglMaterial.h"
#include "rlglCamera.h"
#include "rlglLightProperties.h"
#include <glm/gtc/type_ptr.hpp>
using namespace rlgl;
using namespace uniforms;

void ShaderUniforms::setBool(GLuint sID, const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(sID, name.c_str()), (int)value);
}
void ShaderUniforms::setFloat(GLuint sID, const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(sID, name.c_str()), value);
}
void ShaderUniforms::setMat4x4(GLuint sID, const std::string& name, const glm::mat4x4& value) {
    glUniformMatrix4fv(glGetUniformLocation(sID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void ShaderUniforms::setVec3(GLuint sID, const std::string& name, const glm::vec3& value) {
    glUniform3f(glGetUniformLocation(sID, name.c_str()), value.x, value.y, value.z);
}
void ShaderUniforms::setVec4(GLuint sID, const std::string& name, const glm::vec4& value) {
    glUniform4f(glGetUniformLocation(sID, name.c_str()), value.x, value.y, value.z, value.w);
}
void ShaderUniforms::setUint(GLuint sID, const std::string& name, GLuint value) {
    glUniform1ui(glGetUniformLocation(sID, name.c_str()), value);
}
void ShaderUniforms::setInt(GLuint sID, const std::string& name, GLint value) {
    glUniform1i(glGetUniformLocation(sID, name.c_str()), value);
}
void ShaderUniforms::setBool(GLuint sID, const std::string& name, GLboolean value) {
    glUniform1i(glGetUniformLocation(sID, name.c_str()), value);
}


void ProjectViewUniform::setUniformValues(
    GLuint sID,
    const glm::mat4x4& pvMat,
    const rlgl::Camera& cam,
    const rlgl::WorldEnv& worldEnv) const
{
    setMat4x4(sID, "projView", pvMat);
};

void StandardObjectUniforms::setUniformValues(GLuint sID, rlgl::Object* obj) const {
    setMat4x4(sID, "model", obj->modelMatrix());
    setVec4(sID, "color", obj->getColor());
    setBool(sID, "highlight", obj->hasHighlight());
};

void TextureUniforms::setUniformValues(GLuint sID, const rlgl::MaterialPtr material) const {
    std::dynamic_pointer_cast<rlgl::TexturedMaterial>(material)->bind();
};


void MaterialLightPropertiesUniforms::setUniformValues(GLuint sID, const rlgl::MaterialPtr material) const {
    rlgl::LightProperties* lp = &std::dynamic_pointer_cast<LightPropMaterial>(material)->lightProperties;
    setVec3(sID, "materialAmbientFactor", lp->ambientStrength);
    setVec3(sID, "materialDiffuseFactor", lp->diffuseStrength);
    setVec3(sID, "materialSpecularFactor", lp->specularStrength);
    setFloat(sID, "materialShininessFactor", lp->shininess);
};
void LightUniforms::setUniformValues(
    GLuint sID,
    const glm::mat4x4& pvMat,
    const rlgl::Camera& cam,
    const rlgl::WorldEnv& worldEnv) const
{
    if (worldEnv.lights.size() == 0) {
        throw("LightUniforms::setUniformValues - attempted to set light uniforms, but no lights exist in WorldEnv");
    }
    setVec3(sID, "lightPos", worldEnv.lights[0].pos);
    setFloat(sID, "lightAmbientIntensity", worldEnv.lights[0].ambientIntensity);
    setFloat(sID, "lightSpecularIntensity", worldEnv.lights[0].specularIntensity);
    setVec3(sID, "lightColor", worldEnv.lights[0].color);
    setVec3(sID, "camPos", cam.position);

};
    
void CamDirUniform::setUniformValues(
    GLuint sID,
    const glm::mat4x4& pvMat,
    const rlgl::Camera& cam,
    const rlgl::WorldEnv& worldEnv) const
{
    setVec3(sID, "camDir", cam.front);
};


