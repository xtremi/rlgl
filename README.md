# rlgl

## Introduction
My openGL learning playground


## Dependencies

1. GLFW

    - Main page: https://www.glfw.org/download.html
    - Used version: 3.3.8, [git](https://github.com/glfw/glfw) or [source](https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip)
    - Build with CMake
    - Set `%GLFW%` env. var. to install diretory (parent of `lib` and `include` folder)
    
2. Glad

    - [Git](https://github.com/Dav1dde/glad)
    - Best to use webservice to generate: [link](https://glad.dav1d.de/)
        - API gl: Opengl 4.0
        - Profile: core
        - Untick generate loader
    - Set `%GLAD%` env. var. to parent of `include\glad\glad.h`
    
3. GLM
    - [Git](https://github.com/g-truc/glm)
    - [Git release 0.9.9.8](https://github.com/g-truc/glm/releases/tag/0.9.9.8) 
    - I used 0.9.9.8 (Can be built with CMake I think, but is header only library) 
    - Set `%GLM_DIR%` env. var to parent of `glm\glm.hpp`
    
4. STB_image
    - [Git](https://github.com/nothings/stb)
    - Only need this [file](https://github.com/nothings/stb/blob/master/stb_image.h)
    - Set `%STB_IMAGE%` env. var to folder containing `stb_image.h`  
