#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 textureCoord;
layout (location = 4) in mat4 instModelMatrix;

out vec3 vertexColor;
out vec2 texCoords;

uniform mat4 projView;
uniform mat4 model;

void main()
{
    gl_Position = projView * instModelMatrix * vec4(aPos, 1.0);
    vertexColor = aColor;
    texCoords = textureCoord;
}