#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec3 camPos;

//Object:
uniform bool highlight;
uniform vec4 color;

//Light:
struct PointLight{
	vec3 pos;
	vec3 col;
	float ambientIntensity;
	float specularIntensity;
};
uniform PointLight pointLight;

//Material:
struct MaterialProperties{
	vec3 ambientFactor;
	vec3 diffuseFactor;
	vec3 specularFactor;
	float shininessFactor;
};
uniform MaterialProperties matProp;

vec3 pointLightContribution(
	vec3 	   		   camPos,
	PointLight 		   light,
	MaterialProperties mat,
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
		* pow(max(dot(viewDir, reflectDir), 0.0), mat.shininessFactor);
		
	//+ Light material properties contribution:
	vec3 lightContribution = 
		ambient  * mat.ambientFactor + 
		diffuse  * mat.diffuseFactor + 
		specular * mat.specularFactor;

	lightContribution *= light.col;
	return lightContribution;
}

void main()
{
	FragColor = color;

	vec3 lightContribution = pointLightContribution(
		camPos,
		pointLight,
		matProp,
		normal,
		fragPos);

	FragColor = FragColor * vec4(lightContribution, 1.0);
} 