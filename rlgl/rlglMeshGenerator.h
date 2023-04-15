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
		float* positions = nullptr;
		float* normals = nullptr;
		float* texCoords = nullptr;
		float* indices = nullptr;
		size_t nPositions = 0;
		size_t nNormals = 0;
		size_t nTexCoords = 0;
		size_t nIndices = 0;
	};

	class MeshGenerator {

	public:
		void generateSphere(
			std::shared_ptr<MeshVertexData> data,
			const rl::geom::Sphere&	sphere,
			const glm::vec3& center,
			int nElementsEdge,
			bool indexed);


		bool generateNormals = true;
		bool generateTexCoords = true;

	};


}