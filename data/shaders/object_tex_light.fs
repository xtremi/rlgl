#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoords;

uniform sampler2D textureID;
uniform bool highlight;
uniform vec4 color;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightAmbientIntensity;

void main()
{
    //FragColor = vec4(vertexColor, 1.0);
	if(!highlight){
		FragColor = texture(textureID, texCoords);// * vec4(vertexColor, 1.0);
	}
	else{
		FragColor = texture(textureID, texCoords) * color;
	}

	vec4 ambient = vec4(lightAmbientIntensity * lightColor, 1.0);

	FragColor = FragColor * ambient;
} 