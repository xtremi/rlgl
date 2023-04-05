#pragma once
#include <string>
#include <memory>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include "rlglLightProperties.h"

namespace rlgl {

	/*!Material based class*/
	class Material {};
	typedef std::shared_ptr<Material> MaterialPtr;


	/*!Material with a texture*/
	class TexturedMaterial : public Material {
	public:
		TexturedMaterial(const std::string& texturePath, bool repeatTexture);

		void defineTexture(const std::string& texturePath, bool repeatTexture);
		GLuint glID = 0;
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
	class TextureLightPropMaterial : public TexturedMaterial {
	public:
		TextureLightPropMaterial(
			const std::string& texturePath,
			bool			   repeatTexture,
			const LightProperties& lightProps) 
			: TexturedMaterial(texturePath, repeatTexture), lightProperties{lightProps}{}

	public:
		LightProperties lightProperties;
	};

}