#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoords;

uniform sampler2D textureID;
uniform bool highlight;

void main()
{
    //FragColor = vec4(vertexColor, 1.0);
	if(!highlight){
		FragColor = texture(textureID, texCoords);// * vec4(vertexColor, 1.0);
	}
	else{
		FragColor = texture(textureID, texCoords) * vec4(1.0f, 0.1f, 0.f, 0.9f);
	}
} 