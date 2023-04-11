#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 projView;

void main()
{
    texCoords = aPos;

    float ang = -90.0;
    float c = cos(radians(ang));
    float s = sin(radians(ang));
    mat3 rotMat90x = mat3(
        1.0, 0.0, 0.0,
        0, c, -s,
        0, -s, c);
    
    vec3 pos = rotMat90x * aPos;

    gl_Position = projView * vec4(pos, 1.0);
}