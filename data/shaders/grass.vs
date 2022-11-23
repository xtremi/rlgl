#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec2 uvCoords;

uniform mat4 projView;
uniform mat4 model;

out vec2 o_uvCoords;

void main()
{
    gl_Position = projView * model * vec4(aPos.xyz, 1.0);
	o_uvCoords = uvCoords;
}