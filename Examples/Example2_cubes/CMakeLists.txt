set(EXE_NAME Example2_cubes)

set(SOURCE_FILES
    main.cpp
    Example2_App.cpp
)

set(HEADER_FILES
    Example2_App.h
)

add_executable(${EXE_NAME} 
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(${EXE_NAME}  
    PUBLIC ${PROJECT_SOURCE_DIR}/rlgl
)

target_link_libraries(${EXE_NAME}  
    PUBLIC rlgl
)

set_target_properties(${EXE_NAME} PROPERTIES FOLDER "Examples")
set_property(TARGET ${EXE_NAME} PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "../../../data")



#install(TARGETS rlgl DESTINATION lib)
#install(FILES rlgl DESTINATION include)