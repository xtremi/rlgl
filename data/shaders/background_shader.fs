#version 330 core
out vec4 FragColor;

in vec2 texCoords;
uniform vec3 camDir;

void main()
{
    const float fov = 45.0;
    const float fovR = radians(fov);

    float fragAng = 0.5 * fovR * texCoords.x;

    FragColor = vec4(1.0, 0.0, 0.0, 1.0);

    if(camDir.x > 0.25 && camDir.x < 0.5){
        FragColor.g = 0.25;
    }

    //if(distance(texCoords, vec2(0.0, 0.0)) < 0.2){
    //    FragColor.g = 0.8;
    //}

    /*if(abs(texCoords.x) < 0.1){
        FragColor.g = 0.8;
    }
    if(abs(texCoords.y) < 0.1){
        FragColor.b = 0.8;
    }*/
} 