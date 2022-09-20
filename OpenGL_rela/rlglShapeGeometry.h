#pragma once
#include <vector>
#include <glad/glad.h>

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

struct Mesh
{
    GLBuffer<float> vertices;
    GLBuffer<GLuint> indices;
};


extern const Mesh plane;
extern const Mesh cube;

}

