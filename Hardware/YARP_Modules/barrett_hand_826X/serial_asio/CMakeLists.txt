cmake_minimum_required( VERSION 2.8 )
cmake_policy( VERSION ${CMAKE_VERSION} )

project( serial_asio )

set(Boost_USE_STATIC_LIBS ON)
find_package( Boost REQUIRED)
include_directories( ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )

#--------------------------------------------------------------------------------------------
#project source and include files
set( SOURCE_FILES
	serial_asio.cpp
)

set( HEADER_FILES
	serial_asio.h
)

#--------------------------------------------------------------------------------------------

SET( myApp_SRCS
  ${SOURCE_FILES}
  ${HEADER_FILES}
 )
 
source_group( "Header Files" FILES ${HEADER_FILES})
source_group( "Source Files" FILES ${SOURCE_FILES})

add_library( serial_asio STATIC ${myApp_SRCS} )

target_link_libraries(serial_asio )

# Expose this's public includes (including eigen, tbb and ipp transitively) to other
# subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}
								# ${PROJECT_SOURCE_DIR}/common
								 ${Boost_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
	
set(${PROJECT_NAME}_LIBRARY_DIRS 
							#	${PROJECT_SOURCE_DIR}/lib
								 ${Boost_LIBRARY_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Libraries Directories" FORCE)

