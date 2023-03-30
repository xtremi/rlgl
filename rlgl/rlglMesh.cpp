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

/*!
	Quad mesh composed two triangles.
	Has Normals, colours and texture coordinates
	Has indices.
	Texture coordinates are going from 0.0,0.0 to 10.0, 10.0

	nVertices = 4, nElements = 6
*/
MeshPtr rlgl::primitive_mesh::plane_textureX10 = std::make_shared<Mesh>(

    rlgl::GLBuffer<float>({
		//Positions,			 Normals,		 Colours,		    Texcoords
         0.5f,  0.5f, 0.0f,  0.f, 0.f, 1.f,  1.0f, 0.0f, 0.0f,  10.0f, 10.0f, // top right
         0.5f, -0.5f, 0.0f,  0.f, 0.f, 1.f,  0.0f, 1.0f, 0.0f,  10.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.f, 0.f, 1.f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,	 // bottom left
        -0.5f,  0.5f, 0.0f,  0.f, 0.f, 1.f,  1.0f, 1.0f, 0.0f,  0.0f, 10.0f   // top left 
    }),

    rlgl::GLBuffer<GLuint>({
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }),
	true, true, true);


/*!
	Quad mesh composed two triangles.
	Has Normals
	Has indices.

	nVertices = 4, nElements = 6
*/
MeshPtr rlgl::primitive_mesh::terrainDummy = std::make_shared<Mesh>(

	rlgl::GLBuffer<float>({
		//Positions,        Normals
		0.5f,  0.5f, 0.0f,  0.f, 0.f, 1.f, // top right
		0.5f, -0.5f, 0.0f,  0.f, 0.f, 1.f, // bottom right
	   -0.5f, -0.5f, 0.0f,  0.f, 0.f, 1.f, // bottom left
	   -0.5f,  0.5f, 0.0f,  0.f, 0.f, 1.f  // top left 
		}),
	rlgl::GLBuffer<GLuint>({
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
		}),
	true, false, false 

);

/*!
	Quad mesh composed two triangles.
	Has colours and texture coordinates
	Has indices.

	nVertices = 4, nElements = 6
*/
MeshPtr rlgl::primitive_mesh::plane = std::make_shared<Mesh>(

    rlgl::GLBuffer<float>({
		//Positions,		 Colours,          Texture
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left 
    }),
    rlgl::GLBuffer<GLuint>({
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }),
    false, true, true
);

/*!
	Quad mesh composed two triangles.
	Has only positions
	Has indices.

	nVertices = 4, nElements = 6
*/
MeshPtr rlgl::primitive_mesh::square = std::make_shared<Mesh>(

	rlgl::GLBuffer<float>({
		 //Positions
		 0.5f,  0.5f, 0.0f, // top right
		 0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f  // top left 
	}),
	rlgl::GLBuffer<GLuint>({
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	}),
	false, false, false
);

/*!
	Cube mesh (W=H=L = 1.0, center in 0,0,0)
	Has normals, colours and texture coordinates

	nVertices = 36, nElements = 12
*/
MeshPtr rlgl::primitive_mesh::cube_tex = std::make_shared<Mesh>(
     rlgl::GLBuffer<float>({
		//Positions,		   Normals,		    Colours,		Texcoords
		//Bottom plane (Z-):				    
        -0.5f, -0.5f, -0.5f,   0.f, 0.f, -1.f,  1.f, 0.f, 0.f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.f, 0.f, -1.f,  1.f, 0.f, 0.f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   0.f, 0.f, -1.f,  1.f, 0.f, 0.f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.f, 0.f, -1.f,  1.f, 0.f, 0.f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.f, 0.f, -1.f,  1.f, 0.f, 0.f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.f, 0.f, -1.f,  1.f, 0.f, 0.f,   0.0f, 0.0f,
		//Top plane (Z+):					    
        -0.5f, -0.5f,  0.5f,   0.f, 0.f, 1.f,   1.f, 0.f, 0.f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.f, 0.f, 1.f,   1.f, 0.f, 0.f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.f, 0.f, 1.f,   1.f, 0.f, 0.f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.f, 0.f, 1.f,   1.f, 0.f, 0.f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.f, 0.f, 1.f,   1.f, 0.f, 0.f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.f, 0.f, 1.f,   1.f, 0.f, 0.f,   0.0f, 0.0f,
		//X- plane:							    
        -0.5f,  0.5f,  0.5f,   -1.f, 0.f, 0.f,  1.f, 0.f, 0.f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   -1.f, 0.f, 0.f,  1.f, 0.f, 0.f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   -1.f, 0.f, 0.f,  1.f, 0.f, 0.f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   -1.f, 0.f, 0.f,  1.f, 0.f, 0.f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   -1.f, 0.f, 0.f,  1.f, 0.f, 0.f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   -1.f, 0.f, 0.f,  1.f, 0.f, 0.f,   1.0f, 0.0f,
        //X+ plane:							    
         0.5f,  0.5f,  0.5f,   1.f, 0.f, 0.f,   1.f, 0.f, 0.f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.f, 0.f, 0.f,   1.f, 0.f, 0.f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.f, 0.f, 0.f,   1.f, 0.f, 0.f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.f, 0.f, 0.f,   1.f, 0.f, 0.f,   0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.f, 0.f, 0.f,   1.f, 0.f, 0.f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.f, 0.f, 0.f,   1.f, 0.f, 0.f,   1.0f, 0.0f,
        //Y- plane:							    				 
        -0.5f, -0.5f, -0.5f,   0.f, -1.f, 0.f,  1.f, 0.f, 0.f,   0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.f, -1.f, 0.f,  1.f, 0.f, 0.f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.f, -1.f, 0.f,  1.f, 0.f, 0.f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.f, -1.f, 0.f,  1.f, 0.f, 0.f,   1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0.f, -1.f, 0.f,  1.f, 0.f, 0.f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.f, -1.f, 0.f,  1.f, 0.f, 0.f,   0.0f, 1.0f,
        //Y+ plane:							    
        -0.5f,  0.5f, -0.5f,   0.f, 1.f, 0.f,   1.f, 0.f, 0.f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   0.f, 1.f, 0.f,   1.f, 0.f, 0.f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.f, 1.f, 0.f,   1.f, 0.f, 0.f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.f, 1.f, 0.f,   1.f, 0.f, 0.f,   1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,   0.f, 1.f, 0.f,   1.f, 0.f, 0.f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0.f, 1.f, 0.f,   1.f, 0.f, 0.f,   0.0f, 1.0f
     }),
	true, true, true
);

/*!
	Cube mesh (W=H=L = 1.0, center in 0,0,0)
	Has normals

	nVertices = 36, nElements = 12
*/
MeshPtr rlgl::primitive_mesh::cube = std::make_shared<Mesh>(
	 rlgl::GLBuffer<float>({
		//Positions,		   Normals
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
	true, false, false
);


//TODO: this mesh probably doesn't belong here, move to application/example
/*!
	Two planes mesh (used for grass leaf)
		- One in face XZ, one in face YZ		
	Has texture coordinates

	nVertices = 8, nElements = 12
*/

MeshPtr rlgl::primitive_mesh::grass = std::make_shared<Mesh>(
	rlgl::GLBuffer<float>({
	   -0.5f,  0.0f,  0.0f,    -1.f, 0.f,
		0.5f,  0.0f,  0.0f,     1.f,  0.f,
		0.5f,  0.0f,  1.0f,     1.f,  1.f,
	   -0.5f,  0.0f,  1.0f,    -1.f, 1.f,

		0.0f, -0.5f,  0.0f,    -1.f, 0.f,
		0.0f,  0.5f,  0.0f,     1.f,  0.f,
		0.0f,  0.5f,  1.0f,     1.f,  1.f,
		0.0f, -0.5f,  1.0f,    -1.f, 1.f,
		}),
	rlgl::GLBuffer<GLuint>({
		0,1,2, 0,2,3,
		4,5,6, 4,6,7
		}),
	false, false, true
);

/*
  
    back

   3x---x2
	| / |
	x---x
	0   1
left       right
   3x---x2
	| / |
	x---x
    0   1
	front

*/
std::vector<float> Mesh::createHexagonal(
	const std::vector<glm::vec3>& top, 
	const std::vector<glm::vec3>& bot) 
{

	std::vector<glm::vec3> bufferdataV({
		//bottom face
		bot[0], bot[1], bot[2], //glm::normalize(glm::cross(bot[1] - bot[0], bot[2] - bot[0])),
		bot[0], bot[2], bot[3], //glm::normalize(glm::cross(bot[2] - bot[0], bot[3] - bot[0])),
		//front face			//
		bot[0], bot[1], top[1], //glm::normalize(glm::cross(bot[1] - bot[0], top[1] - bot[0])),
		bot[0], top[1], top[0],	//glm::normalize(glm::cross(top[1] - bot[0], top[0] - bot[0])),
		//right face			//
		bot[1], bot[2], top[2], //glm::normalize(glm::cross(bot[2] - bot[1], top[2] - bot[1])),
		bot[1], top[2], top[1],	//glm::normalize(glm::cross(top[2] - bot[1], top[1] - bot[1])),
		//back face				//
		bot[2], bot[3], top[3], //glm::normalize(glm::cross(bot[3] - bot[2], top[3] - bot[2])),
		bot[2], top[3], top[2],	//glm::normalize(glm::cross(top[3] - bot[2], top[2] - bot[2])),
		//left face				//
		bot[3], bot[0], top[0], //glm::normalize(glm::cross(bot[0] - bot[3], top[0] - bot[3])),
		bot[3], top[0], top[3],	//glm::normalize(glm::cross(top[0] - bot[3], top[3] - bot[3])),
		//top face				//
		top[0], top[1], top[2],	//glm::normalize(glm::cross(top[1] - top[0], top[2] - top[0])),
		top[0], top[2], top[3],	//glm::normalize(glm::cross(top[2] - top[0], top[3] - top[0]))

	});

	std::vector<float> bufferDataF;
	for (const glm::vec3& v : bufferdataV) {
		bufferDataF.push_back(v.x);
		bufferDataF.push_back(v.y);
		bufferDataF.push_back(v.z);
		bufferDataF.push_back(1.f);
		bufferDataF.push_back(0.f);
		bufferDataF.push_back(0.f);
	}

	return bufferDataF;
}

