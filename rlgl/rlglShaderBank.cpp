#include "rlglShaderBank.h"
#include "rlglConfig.h"
using namespace rlgl;

rlgl::ShaderPtr ShaderBank::standardColorShader() {
    return retreiveShader<rlgl::StandardShader>(_standardColorShader, "/shaders/object_col.vs", "/shaders/object_col.fs");
}

rlgl::ShaderPtr ShaderBank::standardTextureShader() {
    return retreiveShader<rlgl::TextureShader>(_standardTexturedShader, "/shaders/object_tex.vs", "/shaders/object_tex.fs");
}

rlgl::ShaderPtr ShaderBank::textureLightShader() {
    return retreiveShader<rlgl::TextureLightShader>(_textureLightShader, "/shaders/object_tex_light.vs", "/shaders/object_tex_light.fs");
}

rlgl::ShaderPtr ShaderBank::textureLightMaterialShader() {
    return retreiveShader<rlgl::TextureLightMaterialShader>(_textureLightMaterialShader, "/shaders/object_tex_light_mat.vs", "/shaders/object_tex_light_mat.fs");
}

rlgl::ShaderPtr ShaderBank::lightMaterialShader() {
    return retreiveShader<rlgl::LightMaterialShader>(_lightMaterialShader, "/shaders/object_col_light_mat.vs", "/shaders/object_tex_object_col_light_mat.fs");
}

rlgl::ShaderPtr ShaderBank::cubeMapShader() {
    return retreiveShader<rlgl::CubeMapShader>(_cubeMapShader, "/shaders/sky_cubemap.vs", "/shaders/sky_cubemap.fs");
}

rlgl::ShaderPtr ShaderBank::standardUIcolShader() {
    return retreiveShader<rlgl::StandardShader>(_standardUIcolShader, "/shaders/ui_element.vs", "/shaders/ui_element.fs");
}

rlgl::ShaderPtr ShaderBank::_standardColorShader = nullptr;
rlgl::ShaderPtr ShaderBank::_standardTexturedShader = nullptr;
rlgl::ShaderPtr ShaderBank::_standardUIcolShader = nullptr;
rlgl::ShaderPtr ShaderBank::_textureLightShader = nullptr;
rlgl::ShaderPtr ShaderBank::_textureLightMaterialShader = nullptr;
rlgl::ShaderPtr ShaderBank::_lightMaterialShader = nullptr;
rlgl::ShaderPtr ShaderBank::_textureLightShader = nullptr;
rlgl::ShaderPtr ShaderBank::_cubeMapShader = nullptr;
