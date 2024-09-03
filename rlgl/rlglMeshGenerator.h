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

		class Rectangle : public Shape {
		public:
		    Rectangle(float _width, float _height)
				: width{_width}, height{_height}{}
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
			const glm::vec3&				center = glm::vec3(0.f));


		void generatePlane(
			std::shared_ptr<MeshVertexData> data,
			const rl::geom::Rectangle&      shape,
			int 							nElementsSide,
			bool							indexed,
			const glm::vec3& 				center = glm::vec3(0.f));

		bool generateNormalsON = true;
		bool generateTexCoordsON = true;

	private:

	};

	class MeshGenerator {
		friend class MeshFactory;
	
	protected:
		MeshGenerator(bool _includeNormals, bool _includeTexCoords);

		void makeNonIndexed(std::shared_ptr<MeshVertexData> data);
		void generateTriangleFanRow(
			std::shared_ptr<MeshVertexData> data,
			int nElements,
			int indexCenter,
			int firstIndex);

		bool includeNormals = false;
		bool includeTexCoords = false;
	};


	class SphereMeshGenerator : public MeshGenerator{
		friend class MeshFactory;

		SphereMeshGenerator(int nElementsAround, bool _includeNormals, bool _includeTexCoords);

		void generateVertices(
			std::shared_ptr<MeshVertexData> data,
			const rl::geom::Sphere&			sphere,
			const glm::vec3&				center = glm::vec3(0.0f));

		void generateIndices(std::shared_ptr<MeshVertexData> data);

		int nElementsPhi = 0;
		int nElementsTheta = 0;
	};

	class GridMeshGenerator : public MeshGenerator{
		friend class MeshFactory;

		GridMeshGenerator(int _nElementsX, int _nElementsY,  bool _includeNormals, bool _includeTexCoords);

		void generateVertices(
			std::shared_ptr<MeshVertexData> data,
			const rl::geom::Rectangle&		shape,
			const glm::vec3&				center = glm::vec3(0.0f));

		void generateIndices(std::shared_ptr<MeshVertexData> data);

		int nElementsX = 0;
		int nElementsY = 0;
	};

}