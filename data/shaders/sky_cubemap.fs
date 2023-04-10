#version 330 core
out vec4 FragColor;

in vec3 texCoords;
uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, texCoords);
		//vec3(texCoords.y, texCoords.x, texCoords.z));
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
} 