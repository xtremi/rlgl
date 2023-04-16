#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 projView;
uniform mat4 model;

void main()
{
	gl_PointSize = 10.0;
    gl_Position = projView * model * vec4(aPos.xyz, 1.0);
}