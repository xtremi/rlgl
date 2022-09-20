#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoords;

uniform sampler2D textureID;

void main()
{
    //FragColor = vec4(vertexColor, 1.0);
    FragColor = texture(textureID, texCoords);// * vec4(vertexColor, 1.0);
} 