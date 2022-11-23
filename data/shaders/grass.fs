#version 330 core

#define M_PI 3.1415926

out vec4 FragColor;
in vec2 o_uvCoords;

uniform vec4 color;

void main()
{	
	vec2 uv = o_uvCoords;

	FragColor = color;

	//if(uv.x < 0.2 || uv.x > 0.8){
	//	FragColor.a = 0.;
	//}

	float x = abs(uv.x);
	
	float alpha = 4.25*uv.y;

	float grassEdgeX = (0.1 + 0.5 * 0.5*(1. + sin(alpha)));

	if(alpha > M_PI){
		FragColor.a = 0.;
	}
	else if(x > grassEdgeX){
		FragColor.a = 0.;
	}

	FragColor.x = o_uvCoords.y;
} 