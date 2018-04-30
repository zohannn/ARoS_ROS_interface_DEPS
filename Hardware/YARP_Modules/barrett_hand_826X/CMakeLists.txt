cmake_minimum_required( VERSION 2.8 )
cmake_policy( VERSION ${CMAKE_VERSION} )

project( barrett_hand_826X )

set( MODULES_DIR $ENV{MAINPROJECT}/Hardware/YARP_Modules )

#--------------------------------------------------------------------------------------------
# YARP
set(YARP_DIR $ENV{YARP_ROOT}/cmake CACHE PATH "Where is your ${ARGV0} directory?")
find_package(YARP REQUIRED)
include_directories(${YARP_INCLUDE_DIRS})
link_directories(${YARP_LIBRARIES_DIRS})


#--------------------------------------------------------------------------------------------
# Boost and Serial ASIO

# add_definitions( -DUSE_BOOST_SERIAL_ASIO )
# add_subdirectory(serial_asio)
# include_directories(${serial_asio_INCLUDE_DIRS})
# link_directories(${serial_asio_LIBRARIES_DIRS})

find_package( Boost REQUIRED )

include_directories( 
	${Boost_INCLUDE_DIRS}
	${MODULES_DIR}/serial_asio )

link_directories( ${Boost_LIBRARY_DIRS} )

add_definitions( -DUSE_BOOST_SERIAL_ASIO )

#--------------------------------------------------------------------------------------------
#project source and include files
set( SOURCE_FILES
	barrett_hand_826X.cpp
)

set( HEADER_FILES
	barrett_hand_826X.h
	barrett_hand_definitions.h
) 

#--------------------------------------------------------------------------------------------

SET( myApp_SRCS
  ${SOURCE_FILES}
  ${HEADER_FILES}
 )
 
source_group( "Header Files" FILES ${HEADER_FILES})
source_group( "Source Files" FILES ${SOURCE_FILES})

add_library( barrett_hand_826X STATIC ${myApp_SRCS} )

target_link_libraries(barrett_hand_826X  serial_asio ${YARP_LIBRARIES})

target_compile_definitions(barrett_hand_826X PUBLIC BOOST_ASIO_ENABLE_CANCELIO)

# Expose this's public includes (including eigen, tbb and ipp transitively) to other
# subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
								# ${PROJECT_SOURCE_DIR}/common
								${serial_asio_INCLUDE_DIRS}
								 ${YARP_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
	
set(${PROJECT_NAME}_LIBRARY_DIRS 
							  	${PROJECT_SOURCE_DIR}
								${YARP_LIBRARIES_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Libraries Directories" FORCE)

