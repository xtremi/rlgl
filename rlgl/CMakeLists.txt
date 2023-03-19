add_library(rlgl 
    rlglBaseApp.cpp
    rlglCamera.cpp
    rlglLodController.cpp
    rlglMaterial.cpp
    rlglMesh.cpp
    rlglObject.cpp
    rlglRenderer.cpp
    rlglScene.cpp
    rlglShader.cpp
    )

install(TARGETS rlgl DESTINATION lib)
install(FILES rlgl DESTINATION include)