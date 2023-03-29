#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 projView;
uniform mat4 model;

out vec3 normal;
out vec3 fragPos;

void main()
{
    vec4 vertPos = model * vec4(aPos.xyz, 1.0);

    gl_Position = projView * vertPos;
    
    normal      = aNormal;
    fragPos = vec3(vertPos);
}