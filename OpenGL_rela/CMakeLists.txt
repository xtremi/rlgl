set(EXE_NAME rlglPlayground)

set(SOURCE_FILES
    MyApp.cpp
    OpenGL_rela.cpp
)

set(HEADER_FILES
    MyApp.h
)

add_executable(${EXE_NAME} 
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(${EXE_NAME}  
    PUBLIC ${PROJECT_SOURCE_DIR}/rlgl
)
#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

target_link_libraries(${EXE_NAME}  
    PUBLIC rlgl
)


#install(TARGETS rlgl DESTINATION lib)
#install(FILES rlgl DESTINATION include)