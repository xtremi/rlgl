#version 330 core

#define M_PI 3.1415926

out vec4 FragColor;
in vec2 o_uvCoords;

uniform vec4 color;

const vec4 COL_LEAF = vec4(0.6, 0.8, 0.3, 1.0);
const vec4 COL_BACKGROUND = vec4(0.9, 0.9, 1., 0.0);
const float GRASS_MAX_WIDTH = 0.25;
const float GRASS_WAVE_LENGTH_FAC = 0.25;
const float SPEED_FAC = 1.;


void main()
{
    float iTime = 0.;

    float time = SPEED_FAC * iTime;

    vec2 uv = o_uvCoords;
    //rescaleUV(uv);

    //Swaying leaf by moving Y-axis along X
    uv.x += sin(time)*0.5*uv.y*uv.y;
    uv.y += abs(sin(time)*sin(time)*0.3*uv.y*uv.y*uv.y);

	float x = abs(uv.x);	
	float alpha = (1./GRASS_WAVE_LENGTH_FAC)*pow(uv.y, 1.);
	//float grassEdgeX = (0.0 + 0.5 * 0.5*(1. + sin(alpha)));
    float grassEdgeX = GRASS_MAX_WIDTH * sin(alpha + 0.1);
    grassEdgeX *= (0.1 + 0.9*uv.y*uv.y);
	if(alpha > M_PI){
		FragColor = COL_BACKGROUND;
	}
	else if(x > grassEdgeX){
		FragColor = COL_BACKGROUND;
	}
    else{
        FragColor = COL_LEAF;
        
        FragColor.r += 0.2*uv.y;
        FragColor.g += 0.2*uv.y;
        FragColor.b -= 0.4*uv.y;
        
        FragColor.rgb *= pow(x/GRASS_MAX_WIDTH, 0.2);
    }

	//FragColor.x = uv.y;
    //FragColor = vec4(1., 0., 0., 1.);
} 