#pragma once
#include <string>
#include <memory>
#include <glad/gl.h>
#include <glm/glm.hpp>

namespace rlgl {

	class LightProperties {

	public:
		void setProperties(
			glm::vec3 _ambientStrength,
			glm::vec3 _diffuseStrength,
			glm::vec3 _specularStrength,
			float	  _shininess);

		glm::vec3 ambientStrength = glm::vec3(0.2f);
		glm::vec3 diffuseStrength = glm::vec3(0.5f);
		glm::vec3 specularStrength = glm::vec3(5.5f);
		float shininess = 32.0f;
	};


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
	class LightPropMaterial : public Material, public LightProperties {
	public:
		LightPropMaterial(
			glm::vec3		   ambientStrength,
			glm::vec3		   diffuseStrength,
			glm::vec3		   specularStrength,
			float			   shininess);

	};

	/*!Material with texture and light properties*/
	class TextureLightPropMaterial : public TexturedMaterial, public LightProperties {
	public:
		TextureLightPropMaterial(
			const std::string& texturePath,
			bool			   repeatTexture,
			glm::vec3		   ambientStrength,
			glm::vec3		   diffuseStrength,
			glm::vec3		   specularStrength,
			float			   shininess);
	};

}