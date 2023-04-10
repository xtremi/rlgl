#version 330 core
out vec4 FragColor;

in vec2 texCoords;
uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, texCoords);
} 