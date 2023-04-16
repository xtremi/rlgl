#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace rl {
	namespace geom {
		
		class Shape{};
		
		class Circle : public Shape {
		public:
			float radius = 1.0f;
		};

		class Sphere : public Shape {
		public:
			Sphere(float _radius) : radius{_radius}{}
			float radius = 1.0f;
		};

		class Rectangle : public Sphere {
		public:
			float width = 1.0f;
			float height = 1.0f;
		};

		class Cube : public Rectangle {
		public:
			float length = 1.0f;
		};

	}
}

namespace rlgl {

	class MeshVertexData {
	public:
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
	};

	class MeshGenerator {

	public:
		void generateSphere(
			std::shared_ptr<MeshVertexData> data,
			const rl::geom::Sphere&	sphere,
			int nElementsAround,
			bool indexed,
			const glm::vec3& center = glm::vec3(0.0f));


		bool generateNormals = true;
		bool generateTexCoords = true;

	private:
			
		void generateSphereIndices(std::vector<unsigned int>& indices, int nElementsAround);

	};


}