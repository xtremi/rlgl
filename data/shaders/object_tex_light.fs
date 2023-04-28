#version 330 core
out vec4 FragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform vec3 camPos;

//Object:
uniform sampler2D textureID;
uniform bool highlight;
uniform vec4 color;

//Light:
struct PointLight{
	vec3 pos;
	vec3 col;
	float ambientIntensity;
	float specularIntensity;
};
#define MAX_NUMBER_OF_LIGHTS 32
uniform int nLights;
uniform PointLight pointLight[MAX_NUMBER_OF_LIGHTS];

vec3 pointLightContribution(
	vec3 	   		   camPos,
	PointLight 		   light,
	vec3 			   normal,
	vec3 			   fragPos)
{
	vec3 lightDir = normalize(light.pos - fragPos);
	vec3 viewDir = normalize(camPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	//Light contribution (Phong: ambient + diffuse + specular):
	float ambient = light.ambientIntensity;	
	float diffuse = max(dot(normal, lightDir), 0.0);
	float specular = light.specularIntensity 
		* pow(max(dot(viewDir, reflectDir), 0.0), 32);
		
	vec4 lightContribution = vec4(
		(ambient + diffuse + specular) * light.col, 1.0);
	return lightContribution;
}


void main()
{
	FragColor = texture(textureID, texCoords);

	if(highlight){
		FragColor *= color;
	}

	vec3 lightContribution = vec3(0.0);
	for(int i = 0; i < nLights; i++){
		lightContribution += pointLightContribution(
			camPos,
			pointLight[i],
			normal,
			fragPos);
	}

	FragColor = FragColor * vec4(lightContribution, 1.0);
} 