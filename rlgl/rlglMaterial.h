#pragma once
#include "rlglLightProperties.h"
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <vector>

namespace rlgl {

	/*!Material based class*/
	class Material {
	public:
		virtual ~Material() {}
		Material() = default;
	};
	typedef std::shared_ptr<Material> MaterialPtr;

	/*!Base class for material with any type of texture*/
	class TexturedMaterial : public virtual Material {
	public:
		~TexturedMaterial(){}
		TexturedMaterial() = default;
		GLuint glID = 0;

		virtual void bind() = 0;
	};

	/*!Material with light properties [virtual]*/
	class LightPropMaterial : public virtual Material {
	public:
		~LightPropMaterial(){}
		LightPropMaterial() = default;
		LightPropMaterial(const LightProperties& lightProps)
			: lightProperties{ lightProps } {}

	public:
		LightProperties lightProperties;
	};

	/*!Material with a normal 2D texture*/
	class Textured2dMaterial : public virtual TexturedMaterial {
	public:
		~Textured2dMaterial() {}
		Textured2dMaterial() = default;
		Textured2dMaterial(const std::string& texturePath, bool repeatTexture);

		virtual void defineTexture(const std::string& texturePath, bool repeatTexture);
		virtual void bind();
	};

	/*!Material with a cubemap texture*/
	class TexturedCubeMapMaterial : public TexturedMaterial {
	public:
		~TexturedCubeMapMaterial() {}
		TexturedCubeMapMaterial() = default;
		TexturedCubeMapMaterial(const std::vector<std::string>& texturePaths);

		void defineTextures(const std::vector<std::string>& texturePaths);
		virtual void bind();
	};



	/*!Material with texture and light properties*/
	class TextureLightPropMaterial : public Textured2dMaterial, public LightPropMaterial {
	public:
		~TextureLightPropMaterial(){}
		TextureLightPropMaterial() = default;
		TextureLightPropMaterial(
			const std::string& texturePath,
			bool			   repeatTexture,
			const LightProperties& lightProps) 
			: Textured2dMaterial(texturePath, repeatTexture), LightPropMaterial(lightProps ) {}
	};

}