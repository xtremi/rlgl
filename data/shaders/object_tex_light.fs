#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

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


	//Light corrections (Phong: ambient + diffuse + specular):
	vec3 lightDir = normalize(lightPos - fragPos);

	float ambient = lightAmbientIntensity;	
	float diffuse = max(dot(normal, lightDir), 0.0);

	vec4 lightContribution = vec4((ambient + diffuse) * lightColor, 1.0);
	
	FragColor = FragColor * lightContribution;
} 