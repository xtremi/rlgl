#pragma once
#include "rlglShader.h"

namespace rlgl{

    class ShaderBank {
    public:
        static rlgl::ShaderPtr standardColorShader();
        static rlgl::ShaderPtr standardTextureShader();
        static rlgl::ShaderPtr standardUIcolShader();
        
    private:        
        static rlgl::ShaderPtr _standardColorShader;
        static rlgl::ShaderPtr _standardTexturedShader;
        static rlgl::ShaderPtr _standardUIcolShader;
    };


    template<typename T>
    rlgl::ShaderPtr retreiveShader(
        rlgl::ShaderPtr shader,
        const std::string& vertexShaderPath,
        const std::string& fragShaderPath)
    {
        if (!shader) {
            shader = std::make_shared<T>(
                GlobalConfig::assetDirectory + vertexShaderPath,
                GlobalConfig::assetDirectory + fragShaderPath);
        }
        return shader;
    }

}