#pragma once
#include "rlglShader.h"

namespace rlgl{

    class ShaderBank {
    public:
        static rlgl::ShaderPtr standardColorShader();
        static rlgl::ShaderPtr standardTextureShader();

    private:
        static rlgl::ShaderPtr _standardColorShader;
        static rlgl::ShaderPtr _standardTexturedShader;
    };

}