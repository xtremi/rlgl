#pragma once
#include "rlglShader.h"

namespace rlgl{

    class ShaderBank {
    public:
        static rlgl::ShaderPtr standardColorShader();
        static rlgl::ShaderPtr standardTextureShader();
        static rlgl::ShaderPtr standardUIshader();

    private:
        static rlgl::ShaderPtr _standardColorShader;
        static rlgl::ShaderPtr _standardTexturedShader;
        static rlgl::ShaderPtr _standardUIshader;
    };

}