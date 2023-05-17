#pragma once
#include <vector>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <memory>

namespace rlgl {

template <class T>
class GLBuffer {
public:
    GLBuffer(const std::vector<T>& _data = {});
    std::vector<T> data;
    GLuint id;
	size_t size() { return data.size(); }

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
	/*!Mesh without indicies*/
	Mesh(const GLBuffer<float>& vertices,
		 bool hasNormals,
		 bool hasColours,
		 bool hasTexCoords);

	/*!Mesh with indices*/
	Mesh(const GLBuffer<float>& vertices,
	     const GLBuffer<GLuint>& indices,
		 bool hasNormals,
		 bool hasColours,
		 bool hasTexCoords);

	void initialize();
	bool isInitialized() { return VAO != 0; }
	void bind() const;
	void draw(int32_t nInstances = 0) const;

public:
    GLBuffer<float>  vertices;
    GLBuffer<GLuint> indices;
    GLBuffer<float>  instances;
	bool hasInstances = false;

protected:
	GLuint VAO = 0;

	bool hasIndices = false;
	bool hasNormals = false;	
	bool hasColours = false;
	bool hasTexCoords = false;

	GLsizei nVertices = 0;
	GLsizei nIndices  = 0;

	int strideSize();

public:
    static std::vector<float> createHexagonal(
        const std::vector<glm::vec3>& top,
        const std::vector<glm::vec3>& bot);

};
typedef std::shared_ptr<rlgl::Mesh> MeshPtr;


}

