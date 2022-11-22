#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace rlgl {

    template <class T>
    class GLBuffer {
    public:
        GLBuffer(const std::vector<T>& _data = {});
        std::vector<T> data;
        GLuint id;

        void genBuffer() const;
        void bindBuffer(GLenum type) const;
        void bufferData(GLenum target, GLenum usage) const;
    };

    template <class T>
    GLBuffer<T>::GLBuffer(const std::vector<T>& _data) {
        data = _data;
    }
    template <class T>
    void GLBuffer<T>::genBuffer() const {
        glGenBuffers(1, (GLuint*)&id);
    }
    template <class T>
    void GLBuffer<T>::bindBuffer(GLenum target) const {
        glBindBuffer(target, id);
    }
    template <class T>
    void GLBuffer<T>::bufferData(GLenum target, GLenum usage) const {
        glBufferData(target, sizeof(T) * data.size(), (void*)&data[0], usage);
    }


class Mesh
{
public:
	void initialize();
	void bind() const;
	void draw(int32_t nInstances = 0) const;

    GLBuffer<float>  vertices;
    GLBuffer<GLuint> indices;
    GLBuffer<float>  instances;
	GLuint VAO;

	bool hasIndices = false;
	bool hasNormals = false;	
	bool hasColours = false;
	bool hasTexCoords = false;
	bool hasInstances = false;

	GLsizei nVertices;
	GLsizei nElements;

public:
    static std::vector<float> createHexagonal(
        const std::vector<glm::vec3>& top,
        const std::vector<glm::vec3>& bot);

};

namespace primitive_mesh{
	extern Mesh plane;
	extern Mesh square;
	extern Mesh plane_textureX10;
	extern Mesh cube_tex;
	extern Mesh cube;
	extern Mesh grass;

    extern Mesh terrainDummy;
}

}

