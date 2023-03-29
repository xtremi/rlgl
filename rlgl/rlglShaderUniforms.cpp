#include "rlglShaderUniforms.h"
#include <glm/gtc/type_ptr.hpp>


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



/*****STANDARD UNIFORMS ****************************************************************/
/*void StandardUniforms::setWorldUniforms(
    GLuint sID,
    const glm::mat4x4& pvMat,
    const glm::vec3& camPos,
    const rlgl::WorldEnv& worldEnv) const
{
    setProjectViewMatrix(sID, pvMat);
}

void StandardUniforms::setObjectUniforms(GLuint sID, rlgl::Object* obj) const
{
    setModelMatrix(sID, obj->modelMatrix());
    if (obj->hasColor()) {
        setColor(sID, obj->getColor());
    }
    setHighlight(sID, obj->hasHighlight());
}*/


void StandardUniforms::setModelMatrix(GLuint sID, const glm::mat4x4& mat) const {
    setMat4x4(sID, "model", mat);
}
void StandardUniforms::setProjectViewMatrix(GLuint sID, const glm::mat4x4& mat) const {
    setMat4x4(sID, "projView", mat);
}
void StandardUniforms::setColor(GLuint sID, const glm::vec4& color) const {
    setVec4(sID, "color", color);
}
void StandardUniforms::setHighlight(GLuint sID, bool highlight) const {
    setBool(sID, "highlight", highlight);
}



/*****TEXTURE UNIFORMS ****************************************************************/
void TextureUniforms::setTexture(GLuint sID, const rlgl::Material* material) const {
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, ((rlgl::TexturedMaterial*)material)->glID);
}



/*****LIGHT UNIFORMS ****************************************************************/
/*void LightUniforms::setWorldUniforms(
    GLuint sID,
    const glm::mat4x4& pvMat,
    const glm::vec3& camPos,
    const rlgl::WorldEnv& worldEnv) const
{
    setLightPos(sID, worldEnv.lights[0].pos);
    setLightAmbientIntensity(sID, worldEnv.lights[0].ambientIntensity);
    setLightSpecularIntensity(sID, worldEnv.lights[0].specularIntensity);
    setLightColor(sID, worldEnv.lights[0].color);
    setCameraPos(sID, camPos);
}*/


void LightUniforms::setLightPos(GLuint sID, const glm::vec3& position) const {
    setVec3(sID, "lightPos", position);
}
void LightUniforms::setLightAmbientIntensity(GLuint sID, float intensity) const {
    setFloat(sID, "lightAmbientIntensity", intensity);
}
void LightUniforms::setLightSpecularIntensity(GLuint sID, float intensity) const {
    setFloat(sID, "lightSpecularIntensity", intensity);
}
void LightUniforms::setLightColor(GLuint sID, const glm::vec3& color) const {
    setVec3(sID, "lightColor", color);
}
void LightUniforms::setCameraPos(GLuint sID, const glm::vec3& position) const {
    setVec3(sID, "camPos", position);
}

