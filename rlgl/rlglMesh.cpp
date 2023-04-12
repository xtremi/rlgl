#include "rlglMesh.h"
using namespace rlgl;


Mesh::Mesh(
	const GLBuffer<float>& _vertices,
	bool _hasNormals,
	bool _hasColours,
	bool _hasTexCoords)
	: vertices{ _vertices }, hasNormals{ _hasNormals }, hasColours{ _hasColours }, hasTexCoords {
	_hasTexCoords
}
{
	hasIndices = false;
	nVertices = vertices.size() / strideSize();
}

Mesh::Mesh(
	const GLBuffer<float>& _vertices,
	const GLBuffer<GLuint>& _indices,
	bool _hasNormals,
	bool _hasColours,
	bool _hasTexCoords)
	: vertices{ _vertices }, indices{ _indices }, hasNormals{ _hasNormals }, hasColours{ _hasColours }, hasTexCoords{ _hasTexCoords }
{
	hasIndices = true;
	nVertices = vertices.size() / strideSize();
	nIndices = indices.size();
}


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
		glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
	}
}

/*!
	Stride = number of values for each vertices in a buffer
*/
int Mesh::strideSize() {
	int stride = 3;
	if (hasNormals) stride += 3;
	if (hasColours) stride += 3;
	if (hasTexCoords) stride += 2;
	return stride;
}

void Mesh::initialize() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	vertices.genBuffer();
	vertices.bindBuffer(GL_ARRAY_BUFFER);
	vertices.bufferData(GL_ARRAY_BUFFER, GL_STATIC_DRAW);

	int stride = strideSize();
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
