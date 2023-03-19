add_library(rlCommon 
    rlGeometry.cpp
    rlMath.cpp
    rlRayIntersection.cpp
    )

install(TARGETS rlCommon DESTINATION lib)
install(FILES rlCommon DESTINATION include)