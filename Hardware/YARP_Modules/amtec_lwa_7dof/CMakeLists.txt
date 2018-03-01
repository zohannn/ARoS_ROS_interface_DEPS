cmake_minimum_required( VERSION 2.8 )
cmake_policy( VERSION ${CMAKE_VERSION} )

project( amtec_lwa_7dof )

set(YARP_DIR $ENV{YARP_ROOT}/cmake CACHE PATH "Where is your ${ARGV0} directory?")
find_package(YARP REQUIRED)
include_directories(${YARP_INCLUDE_DIRS})
link_directories(${YARP_LIBRARIES_DIRS})

#--------------------------------------------------------------------------------------------
#project source and include files
set( SOURCE_FILES
	AmtecLWA7dof.cpp
)

set( HEADER_FILES
	AmtecLWA7dof.h
	Amtec_Definitions.h
	common/m5apiw32.h
) 

include_directories(common)
link_directories(lib)

#--------------------------------------------------------------------------------------------

SET( myApp_SRCS
  ${SOURCE_FILES}
  ${HEADER_FILES}
 )
 
source_group( "Header Files" FILES ${HEADER_FILES})
source_group( "Source Files" FILES ${SOURCE_FILES})

add_library( amtec_lwa_7dof STATIC ${myApp_SRCS} )

target_link_libraries(amtec_lwa_7dof m5apiw32.lib ${YARP_LIBRARIES})

# Expose this's public includes (including eigen, tbb and ipp transitively) to other
# subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
								# ${PROJECT_SOURCE_DIR}/common
								 ${YARP_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
	
set(${PROJECT_NAME}_LIBRARY_DIRS 
								${PROJECT_SOURCE_DIR}/lib
								 ${YARP_LIBRARIES_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Libraries Directories" FORCE)
