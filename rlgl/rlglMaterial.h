#pragma once
#include "rlglLightProperties.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <vector>

namespace rlgl {

	/*!Material based class*/
	class Material {};
	typedef std::shared_ptr<Material> MaterialPtr;


	class TexturedMaterial : public Material {
	public:
		TexturedMaterial() = default;
		GLuint glID = 0;

		virtual void bind() = 0;
	};

	/*!Material with a texture*/
	class Textured2dMaterial : public TexturedMaterial {
	public:
		Textured2dMaterial() = default;
		Textured2dMaterial(const std::string& texturePath, bool repeatTexture);

		virtual void defineTexture(const std::string& texturePath, bool repeatTexture);
		virtual void bind();
	};

	/*!Material with cubemap texture*/
	class TexturedCubeMapMaterial : public TexturedMaterial {
	public:
		TexturedCubeMapMaterial() = default;
		TexturedCubeMapMaterial(const std::vector<std::string>& texturePaths);

		void defineTextures(const std::vector<std::string>& texturePaths);
		virtual void bind();
	};

	/*!Material with light properties*/
	class LightPropMaterial : public Material{
	public:
		LightPropMaterial(const LightProperties& lightProps) 
			: lightProperties{lightProps}{}
	
	public:
		LightProperties lightProperties;
	};

	/*!Material with texture and light properties*/
	class TextureLightPropMaterial : public Textured2dMaterial {
	public:
		TextureLightPropMaterial(
			const std::string& texturePath,
			bool			   repeatTexture,
			const LightProperties& lightProps) 
			: Textured2dMaterial(texturePath, repeatTexture), lightProperties{lightProps}{}

	public:
		LightProperties lightProperties;
	};

}