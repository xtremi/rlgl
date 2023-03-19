cmake_minimum_required(VERSION 3.10)
project(rlglPlayground VERSION 1.0)

message("##################################################################")
message("PROJECT_BINARY_DIR       = ${PROJECT_BINARY_DIR}")
message("CMAKE_SOURCE_DIR         = ${CMAKE_SOURCE_DIR}")
message("PROJECT_SOURCE_DIR       = ${PROJECT_SOURCE_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_SYSTEM_PROCESSOR   = " ${CMAKE_SYSTEM_PROCESSOR})
message("CMAKE_GENERATOR          = " ${CMAKE_GENERATOR})
message("CMAKE_VS_PLATFORM_NAME   = " ${CMAKE_VS_PLATFORM_NAME})
message("CMAKE_BUILD_TYPE         = " ${CMAKE_BUILD_TYPE})
message("CMAKE_SYSTEM             = " ${CMAKE_SYSTEM})
message("CMAKE_SYSTEM_NAME        = " ${CMAKE_SYSTEM_NAME})
message("UNIX                     = " ${UNIX})
message("WIN32                    = " ${WIN32})
message("MSVC                     = " ${MSVC})
message("##################################################################")


if(WIN32)
	message("Setting policy CMP0074 to NEW to allow find_package to locate GLEW based on enviroment variable GLEW_ROOT")
	cmake_policy(SET CMP0074 NEW)
	
	#So that includes do not end up as "system include" (In VS as /external:I <dir> as command line arguments)
	set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)
endif()


#configure_file(rlLearnCMakeConfig.h.in rlLearnCMakeConfig.h)

add_executable(${PROJECT_NAME} OpenGL_rela/MyApp.cpp OpenGL_rela/OpenGL_rela.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

message(STATUS "Locating OpenGL using find_package")
find_package(OpenGL REQUIRED)
message("OPENGL_INCLUDE_DIR = " ${OPENGL_INCLUDE_DIR})
message("OPENGL_glu_LIBRARY = " ${OPENGL_glu_LIBRARY})
message("OPENGL_gl_LIBRARY  = " ${OPENGL_gl_LIBRARY})
message("OPENGL_LIBRARIES   = " ${OPENGL_LIBRARIES})
message("##################################################")
message(STATUS "OpenGL found")


find_package(Git QUIET)

#DOWNLOAD GIT SUBMODULES
message("Retreiving submodules (GLFW, GLAD, GLM, STB)")
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}")
        endif()
    endif()

endif()


add_subdirectory(rlgl)
add_subdirectory(rlCommon)
add_subdirectory(rlOctree)
#add_subdirectory(OpenGL_rela)
list(APPEND EXTRA_LIBS rlgl rlCommon rlOctree)
list(APPEND EXTRA_LIB_DIRS useless rlgl rlCommon rlOctree)
list(APPEND EXTRA_INCLUDE_DIRS rlgl rlCommon rlOctree)

add_subdirectory(submodules/glfw)
add_subdirectory(submodules/glm)
add_subdirectory(submodules/glad/cmake)


message("Calling glad_add_library to generate GLAD opengl interface:")
glad_add_library(
	glad_gl_core_40
	SHARED
	LOCATION ${PROJECT_BINARY_DIR}/glad-gen
	API gl:core=4.0)
	

target_include_directories(${PROJECT_NAME} 
    PUBLIC submodule/glfw/include
    PUBLIC submodule/stb
    PUBLIC ${EXTRA_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PUBLIC 
	glad_gl_core_40         #submodule
    glfw                    #submodule
    glm::glm                #submodule
    ${OPENGL_LIBRARIES}     #opengl found by find_package
    ${EXTRA_LIBS}           #integrated lib - optional	
    )

#install(TARGETS ${PROJECT_NAME} DESTINATION bin)
#install(FILES "${PROJECT_BINARY_DIR}/rlLearnCMakeConfig.h" DESTINATION include)
#install(FILES "${PROJECT_BINARY_DIR}/glad-gen/include/glad" DESTINATION include)
#
#install(FILES "${GLEW_INCLUDE_DIR}/GL/glew.h" DESTINATION include/GL)
#
#if(WIN32)
#    install(FILES "$ENV{GLEW_ROOT}/bin/Release/${CMAKE_VS_PLATFORM_NAME}/glew32.dll" DESTINATION bin)
#elseif(UNIX)
#    #this does not work well - getting dangling symlink on .so file
#    #there is .so.1 file that has something to do with that in GLEW_SHARED_LIBRARY directory
#    install(FILES ${GLEW_SHARED_LIBRARY} NAMELINK_COMPONENT DESTINATION bin)
#endif()
