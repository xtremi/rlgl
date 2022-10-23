#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 textureCoord;
layout (location = 4) in vec3 aOffset;

out vec3 vertexColor;
out vec2 texCoords;

uniform mat4 projView;
uniform mat4 model;

void main()
{
    gl_Position = projView * model * vec4(aPos + aOffset, 1.0);
    vertexColor = aColor;
    texCoords = textureCoord;
}