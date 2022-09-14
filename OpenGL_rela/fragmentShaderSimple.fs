#version 330 core
out vec4 FragColor;
in vec3 vertexColor;

//uniform vec4 ourColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
} 