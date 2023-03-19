add_library(rlOctree 
    rlOctree.cpp
    rlOctreeStruct.cpp
    )

install(TARGETS rlOctree DESTINATION lib)
install(FILES rlOctree DESTINATION include)