#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projView;
uniform mat4 model;

void main()
{
    texCoords = aPos;
    gl_Position = projView * vec4(aPos, 1.0);
}