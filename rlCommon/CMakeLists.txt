set(LIB_NAME rlCommon)

set(SOURCE_FILES
    rlGeometry.cpp
    rlMath.cpp
    rlRayIntersection.cpp    
)

set(HEADER_FILES
    rlGeometry.h
    rlMath.h
    rlRayIntersection.h    
)

add_library(${LIB_NAME} 
    ${SOURCE_FILES}
    ${HEADER_FILES}
    )

target_link_libraries(${LIB_NAME}  PUBLIC glm::glm)

#install(TARGETS ${LIB_NAME}  DESTINATION lib)
#install(FILES ${LIB_NAME}  DESTINATION include)