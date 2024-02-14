#include "rlglShaderBank.h"
#include "rlglConfig.h"

using namespace rlgl;

rlgl::ShaderPtr ShaderBank::standardColorShader() {
    if (!_standardColorShader) {
        _standardColorShader = std::make_shared<rlgl::StandardShader>(
            GlobalConfig::assetDirectory + "/shaders/object_col.vs",
            GlobalConfig::assetDirectory + "/shaders/object_col.fs");
    }
    return _standardColorShader;
}

rlgl::ShaderPtr ShaderBank::standardTextureShader() {
    if (!_standardTexturedShader) {
        _standardTexturedShader = std::make_shared<rlgl::TextureShader>(
            GlobalConfig::assetDirectory + "/shaders/object_tex.vs",
            GlobalConfig::assetDirectory + "/shaders/object_tex.fs");
    }
    return _standardTexturedShader;
}

rlgl::ShaderPtr ShaderBank::standardUIshader() {
    if (!_standardUIshader) {
        _standardUIshader = std::make_shared<rlgl::StandardShader>(
            GlobalConfig::assetDirectory + "/shaders/ui_element.vs",
            GlobalConfig::assetDirectory + "/shaders/ui_element.fs");
    }
    return _standardUIshader;
}

rlgl::ShaderPtr ShaderBank::_standardColorShader = nullptr;
rlgl::ShaderPtr ShaderBank::_standardTexturedShader = nullptr;
rlgl::ShaderPtr ShaderBank::_standardUIshader = nullptr;