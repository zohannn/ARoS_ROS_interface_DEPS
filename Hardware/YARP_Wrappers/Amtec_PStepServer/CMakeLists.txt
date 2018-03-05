#-------------------------------------------------------------------------------
# CMake minimum required version and used policy
cmake_minimum_required( VERSION 2.8 )
cmake_policy( VERSION ${CMAKE_VERSION} )

project( Amtec_PStepServer )

#--------------------------------------------------------------------------------------------
#project source and include files
set( SOURCE_FILES
	src/AmtecPStepServer.cpp
)

set( HEADER_FILES
	include/AmtecPStepServer.h
) 

include_directories(include)

#--------------------------------------------------------------------------------------------

SET( myApp_SRCS
  ${SOURCE_FILES}
  ${HEADER_FILES}
 )
 
source_group( "Header Files" FILES ${HEADER_FILES})
source_group( "Source Files" FILES ${SOURCE_FILES})

add_library( ${PROJECT_NAME} STATIC ${myApp_SRCS} )

#target_link_libraries(´${PROJECT_NAME})

# Expose this's public includes (including eigen, tbb and ipp transitively) to other
# subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
								# ${PROJECT_SOURCE_DIR}/common
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
	
set(${PROJECT_NAME}_LIBRARY_DIRS 
								${PROJECT_SOURCE_DIR}/src
    CACHE INTERNAL "${PROJECT_NAME}: Libraries Directories" FORCE)
