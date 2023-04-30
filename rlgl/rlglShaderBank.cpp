#include "rlglShaderBank.h"
#include "rlglConfig.h"

using namespace rlgl;

rlgl::ShaderPtr ShaderBank::retreiveShader(
    rlgl::ShaderPtr shader,
    const std::string& vertexShaderPath,
    const std::string& fragShaderPath)
{
    if (!shader) {
        shader = std::make_shared<rlgl::StandardShader>(
            GlobalConfig::assetDirectory + vertexShaderPath,
            GlobalConfig::assetDirectory + fragShaderPath);
    }
    return shader;
}

rlgl::ShaderPtr ShaderBank::standardColorShader() {
    return retreiveShader(_standardColorShader, "/shaders/object_col.vs", "/shaders/object_col.fs");
}

rlgl::ShaderPtr ShaderBank::standardTextureShader() {
    return retreiveShader(_standardTexturedShader, "/shaders/object_tex.vs", "/shaders/object_tex.fs");
}

rlgl::ShaderPtr ShaderBank::standardUIcolShader() {
    return retreiveShader(_standardUIcolShader, "/shaders/ui_element.vs", "/shaders/ui_element.fs");
}


rlgl::ShaderPtr ShaderBank::_standardColorShader = nullptr;
rlgl::ShaderPtr ShaderBank::_standardTexturedShader = nullptr;
rlgl::ShaderPtr ShaderBank::_standardUIcolShader = nullptr;