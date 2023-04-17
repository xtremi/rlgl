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
		void addVertexVec3(const glm::vec3& vec);
		void addVertexVec2(const glm::vec2& vec);
		void addIndices(int elIndices[4]);
		void addIndices(int triIndex1, int triIndex2, int triIndex3);
	};

	class MeshFactory {

	public:
		void generateSphere(
			std::shared_ptr<MeshVertexData> data,
			const rl::geom::Sphere&			sphere,
			int								nElementsAround,
			bool							indexed,
			const glm::vec3&				center = glm::vec3(0.0f));


		bool generateNormalsON = true;
		bool generateTexCoordsON = true;

	private:

	};


	class MeshGenerator {
		friend class MeshFactory;
	
	protected:
		MeshGenerator(bool _includeNormals, bool _includeTexCoords) : 
			includeNormals{_includeNormals}, includeTexCoords{_includeTexCoords}{}


		bool includeNormals = false, includeTexCoords = false;

		void makeNonIndexed(std::shared_ptr<MeshVertexData> data);
		void generateTriangleFanRow(
			std::shared_ptr<MeshVertexData> data,
			int nElements,
			int indexCenter,
			int firstIndex);
	};


	class SphereMeshGenerator : public MeshGenerator{
		friend class MeshFactory;

		SphereMeshGenerator(int nElementsAround, bool _includeNormals, bool _includeTexCoords) 
			: MeshGenerator(_includeNormals, _includeTexCoords) 
		{
			nElementsPhi = 2 * (nElementsAround / 2);	//make even
			nElementsTheta = nElementsPhi / 2;
			includeNormals = _includeNormals;
			includeTexCoords = _includeTexCoords;
		}

		void generateVertices(
			std::shared_ptr<MeshVertexData> data,
			const rl::geom::Sphere&			sphere,
			const glm::vec3&				center = glm::vec3(0.0f));

		void generateIndices(std::shared_ptr<MeshVertexData> data);

		int nElementsPhi = 0;
		int nElementsTheta = 0;
	};

}