#include "rlglShaderBank.h"
#include "rlglConfig.h"

using namespace rlgl;




rlgl::ShaderPtr ShaderBank::standardColorShader() {
    return retreiveShader<rlgl::StandardShader>(_standardColorShader, "/shaders/object_col.vs", "/shaders/object_col.fs");
}

rlgl::ShaderPtr ShaderBank::standardTextureShader() {
    return retreiveShader<standardTextureShader>(_standardTexturedShader, "/shaders/object_tex.vs", "/shaders/object_tex.fs");
}

rlgl::ShaderPtr ShaderBank::standardUIcolShader() {
    return retreiveShader(_standardUIcolShader, "/shaders/ui_element.vs", "/shaders/ui_element.fs");
}


rlgl::ShaderPtr ShaderBank::_standardColorShader = nullptr;
rlgl::ShaderPtr ShaderBank::_standardTexturedShader = nullptr;
rlgl::ShaderPtr ShaderBank::_standardUIcolShader = nullptr;