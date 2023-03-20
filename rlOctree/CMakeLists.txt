set(LIB_NAME rlOctree)

set(SOURCE_FILES
    rlOctree.cpp
    rlOctreeStruct.cpp 
)

set(HEADER_FILES
    rlOctree.h
    rlOctreeStruct.h
)

add_library(${LIB_NAME} 
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

target_include_directories(${LIB_NAME}  
    PUBLIC ${PROJECT_SOURCE_DIR}/rlCommon
)

target_link_libraries(${LIB_NAME} 
    PUBLIC glm::glm
    PUBLIC rlCommon)

#install(TARGETS ${LIB_NAME}  DESTINATION lib)
#install(FILES ${LIB_NAME}  DESTINATION include)