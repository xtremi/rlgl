#pragma once
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>

namespace rlgl {
    class Shader;
    class WorldEnv;
    class Material;
    typedef std::shared_ptr<Material> MaterialPtr;
    class Object;
    class Camera;
    class LightProperties;

    /*!
        Utility functions for setting uniform values

        TODO: store uniform IDS (in uniform classes?) to not keep
              calling glGetUniformLocation for every uniform update.
    */
    class ShaderUniforms {
    protected:
        static void setBool(GLuint sID, const std::string& name, bool value);
        static void setFloat(GLuint sID, const std::string& name, float value);
        static void setMat4x4(GLuint sID, const std::string& name, const glm::mat4x4& value);
        static void setVec3(GLuint sID, const std::string& name, const glm::vec3& value);
        static void setVec4(GLuint sID, const std::string& name, const glm::vec4& value);
        static void setUint(GLuint sID, const std::string& name, GLuint value);
        static void setInt(GLuint sID, const std::string& name, GLint value);
        static void setBool(GLuint sID, const std::string& name, GLboolean value);
    };

    /*!A set of uniforms that should be updated for each time that shader is used*/
    class ShaderUniformSetGlobal : public ShaderUniforms {
        friend rlgl::Shader;
        virtual void setUniformValues(
            GLuint                sID,
            const glm::mat4x4&    pvMat,
            const rlgl::Camera&   cam,
            const rlgl::WorldEnv& worldEnv) const {};
    };
    
    /*!A set of uniforms that should be updated for each Object using that shader*/
    class ShaderUniformSetObject : public ShaderUniforms{
        friend rlgl::Shader;
        virtual void setUniformValues(GLuint sID, rlgl::Object* obj) const {};
    };

    /*!A set of uniforms that should be updated for each material using that shader*/
    class ShaderUniformSetMaterial : public ShaderUniforms {
        friend rlgl::Shader;
        virtual void setUniformValues(GLuint sID, const rlgl::MaterialPtr material) const {};
    };


    /*!
        Global uniforms:
            ProjectView matrix
    */
    class ProjectViewUniform : ShaderUniformSetGlobal {
        void setUniformValues(GLuint sID, const glm::mat4x4& pvMat, const rlgl::Camera& cam, const rlgl::WorldEnv& worldEnv) const;
    };

    /*!
        Object uniforms:
            Model matrix
            Color
            Highlight
    */
    class StandardObjectUniforms : ShaderUniformSetObject {
        void setUniformValues(GLuint sID, rlgl::Object* obj) const;
    };

    /*!
        Material uniforms:
            Texture binding (not really a uniform)
    */
    class TextureUniforms : public ShaderUniformSetMaterial {
        virtual void setUniformValues(GLuint sID, const rlgl::MaterialPtr material) const;
    };


    /*!
        Material uniforms:
            Material light properties
                - Ambient
                - Diffuse
                - Specular
                - Shininess
    */
    class MaterialLightPropertiesUniforms : public ShaderUniformSetMaterial {
        virtual void setUniformValues(GLuint sID, const rlgl::MaterialPtr material) const;
    };

    /*!
        Global uniforms:
            Light uniforms
            - Light position
            - Light ambient intensity
            - Light specular intensity
            - Light color
            - Camera pos
    */
    class LightUniforms : public ShaderUniformSetGlobal {
        virtual void setUniformValues(GLuint sID, const glm::mat4x4& pvMat, const rlgl::Camera& cam, const rlgl::WorldEnv& worldEnv) const;
    };


    /*!
        Global unfirms:
            Cam direction
    */
    class CamDirUniform : public ShaderUniformSetGlobal {
        virtual void setUniformValues(GLuint sID, const glm::mat4x4& pvMat, const rlgl::Camera& cam, const rlgl::WorldEnv& worldEnv) const;
    };


}
