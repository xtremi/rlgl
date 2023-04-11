#version 330 core
out vec4 FragColor;

in vec3 texCoords;
uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, texCoords);

	FragColor.r *= (2.0f - smoothstep(0.0, 0.6, texCoords.y));
	FragColor.g *= (1.0f + smoothstep(0.5, 1.0, texCoords.y));
	//FragColor.rgb *= (1.0f - 0.7f* smoothstep(0.8, 1.0, texCoords.y));

		//vec3(texCoords.y, texCoords.x, texCoords.z));
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
} 