#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec2 textureCoord;

out vec2 texCoords;

uniform mat4 projView;
uniform mat4 model;

void main()
{
    gl_Position = /*projView */ model * vec4(aPos.xyz, 1.0);
    gl_Position.z = 0.999;

    texCoords = textureCoord;
}