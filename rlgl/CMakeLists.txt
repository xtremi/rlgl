set(LIB_NAME rlgl)

set(SOURCE_FILES
    rlglBaseApp.cpp
    rlglCamera.cpp
    rlglLodController.cpp
    rlglMaterial.cpp
    rlglMesh.cpp
    rlglObject.cpp
    rlglRenderer.cpp
    rlglScene.cpp
    rlglShader.cpp
    rlglShaderUniforms.cpp
    rlglLightProperties.cpp
    rlglBaseModel.cpp
    rlglColors.cpp
    rlglConfig.cpp
    rlglMeshBank.cpp
    rlglShaderBank.cpp
    rlglDefaultMeshes.cpp
    rlglMeshGenerator.cpp
    )
    
    set(HEADER_FILES
    rlglBaseApp.h
    rlglCamera.h
    rlglErrors.h
    rlglLodController.h
    rlglMaterial.h
    rlglMesh.h
    rlglObject.h
    rlglRenderer.h
    rlglScene.h
    rlglShader.h
    rlglWorldEnv.h
    rlglShaderUniforms.h
    rlglLightProperties.h
    rlglBaseModel.h
    rlglColors.h
    rlglConfig.h
    rlglMeshBank.h
    rlglShaderBank.h
    rlglDefaultMeshes.h
    rlglMeshGenerator.h
)

add_library(${LIB_NAME} 
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(${LIB_NAME}  
    PUBLIC ${PROJECT_SOURCE_DIR}/rlCommon
    PUBLIC ${PROJECT_SOURCE_DIR}/rlOctree
    PUBLIC ${PROJECT_SOURCE_DIR}/submodules/stb
)

message("Calling glad_add_library to generate GLAD opengl interface:")
glad_add_library(
    glad_gl_core_40
    SHARED
	LOCATION ${PROJECT_BINARY_DIR}/glad-gen
	API gl:core=4.0)

target_link_libraries(${LIB_NAME}  
    PUBLIC glfw             #submodule    
    PUBLIC glad_gl_core_40  #submodule
    PUBLIC glm::glm         #submodule
    PUBLIC rlCommon
    PUBLIC rlOctree
)

#${OPENGL_LIBRARIES}     #opengl found by find_package

#install(TARGETS rlgl DESTINATION lib)
#install(FILES rlgl DESTINATION include)
