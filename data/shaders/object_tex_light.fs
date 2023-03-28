#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D textureID;
uniform bool highlight;
uniform vec4 color;
uniform vec3 camPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightAmbientIntensity;
uniform float lightSpecularIntensity;

void main()
{
	FragColor = texture(textureID, texCoords);

	if(highlight){
		FragColor *= color;
	}


	//Light corrections (Phong: ambient + diffuse + specular):
	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(camPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	

	float ambient = lightAmbientIntensity;	
	float diffuse = max(dot(normal, lightDir), 0.0);
	float specular = lightSpecularIntensity * pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec4 lightContribution = vec4((ambient + diffuse + specular) * lightColor, 1.0);

	FragColor = FragColor * lightContribution;
} 