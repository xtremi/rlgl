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
    if (!_standardTexturedShader) {
        _standardTexturedShader = std::make_shared<rlgl::TextureShader>(
            GlobalConfig::assetDirectory + "/shaders/object_tex.vs",
            GlobalConfig::assetDirectory + "/shaders/object_tex.fs");
    }
    return _standardTexturedShader;
}

rlgl::ShaderPtr ShaderBank::standardUIcolShader() {
    if (!_standardUIcolShader) {
        _standardUIcolShader = std::make_shared<rlgl::StandardShader>(
            GlobalConfig::assetDirectory + "/shaders/ui_element.vs",
            GlobalConfig::assetDirectory + "/shaders/ui_element.fs");
    }
    return _standardUIcolShader;
}


rlgl::ShaderPtr ShaderBank::_standardColorShader = nullptr;
rlgl::ShaderPtr ShaderBank::_standardTexturedShader = nullptr;
rlgl::ShaderPtr ShaderBank::_standardUIcolShader = nullptr;