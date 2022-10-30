#include "rlglMesh.h"
using namespace rlgl;


void Mesh::bind() const {
	glBindVertexArray(VAO);
}

void Mesh::draw(int32_t nInst) const {
	if(!hasIndices){
		if(!hasInstances){
			glDrawArrays(GL_TRIANGLES, 0, nVertices);
		}
		else {
			glDrawArraysInstanced(GL_TRIANGLES, 0, nVertices, (GLsizei)nInst);
		}
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
	//if (hasInstances) stride += 3;

	int startIndex = 0;

	//position:
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)startIndex);
	
	startIndex += 3;

	//normals:
	if (hasNormals) {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(startIndex * sizeof(float)));
		
		startIndex += 3;
	}

	//color:
	if(hasColours){
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(startIndex * sizeof(float)));
		
		startIndex += 3;
	}

	//texture:
	if(hasTexCoords){
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(startIndex * sizeof(float)));
		
		startIndex += 2;
	}

	//indices:
	if(hasIndices){
		indices.genBuffer();
		indices.bindBuffer(GL_ELEMENT_ARRAY_BUFFER);
		indices.bufferData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
	}

	//instances:
	if (hasInstances) {
		instances.genBuffer();
		instances.bindBuffer(GL_ARRAY_BUFFER);
		instances.bufferData(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(1 * 4 * sizeof(float)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(2 * 4 * sizeof(float)));
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(3 * 4 * sizeof(float)));
		
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
	}

}


Mesh rlgl::primitive_mesh::plane_textureX10 = {

    rlgl::GLBuffer<float>({
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 10.0f, 10.0f, // top right
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 10.0f  // top left 
    }),
    rlgl::GLBuffer<GLuint>({
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }),
	rlgl::GLBuffer<float>({}),
	0, true, false, true, true, false, 4, 6
	
};

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
	rlgl::GLBuffer<float>({}),
    0, true, false, true, true, false, 4, 6

};

Mesh rlgl::primitive_mesh::square = {

	rlgl::GLBuffer<float>({
		 0.5f,  0.5f, 0.0f, // top right
		 0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f  // top left 
	}),
	rlgl::GLBuffer<GLuint>({
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	}),
	rlgl::GLBuffer<float>({}),
	0, true, false, false, false, false, 4, 6

};

Mesh rlgl::primitive_mesh::cube_tex{
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
	rlgl::GLBuffer<float>({}),
	0, false, false, true, true, false, 36, 12
};


Mesh rlgl::primitive_mesh::cube{
	 rlgl::GLBuffer<float>({
		-0.5f, -0.5f, -0.5f,   0.f, 0.f, -1.f,
		 0.5f, -0.5f, -0.5f,   0.f, 0.f, -1.f,
		 0.5f,  0.5f, -0.5f,   0.f, 0.f, -1.f,
		 0.5f,  0.5f, -0.5f,   0.f, 0.f, -1.f,
		-0.5f,  0.5f, -0.5f,   0.f, 0.f, -1.f,
		-0.5f, -0.5f, -0.5f,   0.f, 0.f, -1.f,

		-0.5f, -0.5f,  0.5f,   0.f, 0.f, 1.f,
		 0.5f, -0.5f,  0.5f,   0.f, 0.f, 1.f,
		 0.5f,  0.5f,  0.5f,   0.f, 0.f, 1.f,
		 0.5f,  0.5f,  0.5f,   0.f, 0.f, 1.f,
		-0.5f,  0.5f,  0.5f,   0.f, 0.f, 1.f,
		-0.5f, -0.5f,  0.5f,   0.f, 0.f, 1.f,

		-0.5f,  0.5f,  0.5f,   -1.f, 0.f, 0.f,
		-0.5f,  0.5f, -0.5f,   -1.f, 0.f, 0.f,
		-0.5f, -0.5f, -0.5f,   -1.f, 0.f, 0.f,
		-0.5f, -0.5f, -0.5f,   -1.f, 0.f, 0.f,
		-0.5f, -0.5f,  0.5f,   -1.f, 0.f, 0.f,
		-0.5f,  0.5f,  0.5f,   -1.f, 0.f, 0.f,

		 0.5f,  0.5f,  0.5f,   1.f, 0.f, 0.f,
		 0.5f,  0.5f, -0.5f,   1.f, 0.f, 0.f,
		 0.5f, -0.5f, -0.5f,   1.f, 0.f, 0.f,
		 0.5f, -0.5f, -0.5f,   1.f, 0.f, 0.f,
		 0.5f, -0.5f,  0.5f,   1.f, 0.f, 0.f,
		 0.5f,  0.5f,  0.5f,   1.f, 0.f, 0.f,

		-0.5f, -0.5f, -0.5f,   0.f, -1.f, 0.f,
		 0.5f, -0.5f, -0.5f,   0.f, -1.f, 0.f,
		 0.5f, -0.5f,  0.5f,   0.f, -1.f, 0.f,
		 0.5f, -0.5f,  0.5f,   0.f, -1.f, 0.f,
		-0.5f, -0.5f,  0.5f,   0.f, -1.f, 0.f,
		-0.5f, -0.5f, -0.5f,   0.f, -1.f, 0.f,

		-0.5f,  0.5f, -0.5f,   0.f, 1.f, 0.f,
		 0.5f,  0.5f, -0.5f,   0.f, 1.f, 0.f,
		 0.5f,  0.5f,  0.5f,   0.f, 1.f, 0.f,
		 0.5f,  0.5f,  0.5f,   0.f, 1.f, 0.f,
		-0.5f,  0.5f,  0.5f,   0.f, 1.f, 0.f,
		-0.5f,  0.5f, -0.5f,   0.f, 1.f, 0.f,
	 }),
	rlgl::GLBuffer<GLuint>({}),
	rlgl::GLBuffer<float>({}),
	0, false, true, false, false, false, 36, 12
};