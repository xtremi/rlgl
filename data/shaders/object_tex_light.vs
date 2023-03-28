#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoords;

uniform mat4 projView;
uniform mat4 model;

out vec3 vertexColor;
out vec2 texCoords;
out vec3 normal;
out vec3 fragPos; //in world space

void main()
{
    gl_Position = projView * model * vec4(aPos.xyz, 1.0);
    vertexColor = aColor;
    texCoords = aTexCoords;
    normal = aNormal;

    fragPos = vec3(model * vec4(aPos.xyz, 1.0));
}