#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

//Object:
uniform sampler2D textureID;
uniform bool highlight;
uniform vec4 color;

//Environment:
uniform vec3 camPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightAmbientIntensity;
uniform float lightSpecularIntensity;

//Material:
uniform vec3 materialAmbientFactor;
uniform vec3 materialDiffuseFactor;
uniform vec3 materialSpecularFactor;
uniform float materialShininessFactor;

void main()
{
	FragColor = texture(textureID, texCoords);

	if(highlight){
		FragColor *= color;
	}

	vec3 lightDir = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(camPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	//Light contribution (Phong: ambient + diffuse + specular):
	float ambient = lightAmbientIntensity;	
	float diffuse = max(dot(normal, lightDir), 0.0);
	float specular = lightSpecularIntensity 
		* pow(max(dot(viewDir, reflectDir), 0.0), materialShininessFactor);
		
	//+ Light material properties contribution:
	vec3 lightContribution = 
		ambient  * materialAmbientFactor + 
		diffuse  * materialDiffuseFactor + 
		specular * materialSpecularFactor;

	lightContribution *= lightColor;

	FragColor = FragColor * vec4(lightContribution, 1.0);
} 