#include "rlglMesh.h"
using namespace rlgl;


void Mesh::bind() const {
	glBindVertexArray(VAO);
}

void Mesh::draw() const {
	if(!hasIndices){
		glDrawArrays(GL_TRIANGLES, 0, nVertices);
	}
	else{
		glDrawElements(GL_TRIANGLES, nElements, GL_UNSIGNED_INT, 0);
	}
}

void Mesh::initialize() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	vertices.genBuffer();
	vertices.bindBuffer(GL_ARRAY_BUFFER);
	vertices.bufferData(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

	int stride = 3;
	if (hasNormals) stride += 3;
	if (hasColours) stride += 3;
	if (hasTexCoords) stride += 2;

	int startIndex = 0;

	//position:
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)startIndex);
	glEnableVertexAttribArray(0);
	startIndex += 3;

	//normals:
	if (hasNormals) {
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(startIndex * sizeof(float)));
		glEnableVertexAttribArray(1);
		startIndex += 3;
	}

	//color:
	if(hasColours){
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(startIndex * sizeof(float)));
		glEnableVertexAttribArray(2);
		startIndex += 3;
	}

	//texture:
	if(hasTexCoords){
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(startIndex * sizeof(float)));
		glEnableVertexAttribArray(3);
		startIndex += 2;
	}

	//indices:
	if(hasIndices){
		indices.genBuffer();
		indices.bindBuffer(GL_ELEMENT_ARRAY_BUFFER);
		indices.bufferData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
	}

}


Mesh rlgl::primitive_mesh::plane = {

    rlgl::GLBuffer<float>({
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left 
    }),
    rlgl::GLBuffer<GLuint>({
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }),
	0, true, false, true, true, 4, 6
	
};

extern Mesh rlgl::primitive_mesh::cube{
     rlgl::GLBuffer<float>({
        -0.5f, -0.5f, -0.5f,   1.0f, 0.f, 0.f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.f, 0.f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.f, 0.f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.f, 0.f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 0.f, 0.f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.f, 0.f,   0.0f, 0.0f,
                               
        -0.5f, -0.5f,  0.5f,   1.0f, 0.f, 0.f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.f, 0.f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.f, 0.f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.f, 0.f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.f, 0.f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.f, 0.f,   0.0f, 0.0f,
                               
        -0.5f,  0.5f,  0.5f,   1.0f, 0.f, 0.f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 0.f, 0.f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.f, 0.f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.f, 0.f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.f, 0.f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.f, 0.f,   1.0f, 0.0f,
                               
         0.5f,  0.5f,  0.5f,   1.0f, 0.f, 0.f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.f, 0.f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.f, 0.f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.f, 0.f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.f, 0.f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.f, 0.f,   1.0f, 0.0f,
                               
        -0.5f, -0.5f, -0.5f,   1.0f, 0.f, 0.f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.f, 0.f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.f, 0.f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.f, 0.f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.f, 0.f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.f, 0.f,   0.0f, 1.0f,
                               
        -0.5f,  0.5f, -0.5f,   1.0f, 0.f, 0.f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.f, 0.f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.f, 0.f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.f, 0.f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 0.f, 0.f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 0.f, 0.f,   0.0f, 1.0f
     }),
    rlgl::GLBuffer<GLuint>({}),
	0, false, false, true, true, 36, 12
};