#-------------------------------------------------------------------------------
# CMake minimum required version and used policy
cmake_minimum_required( VERSION 2.8 )
cmake_policy( VERSION ${CMAKE_VERSION} )
#-------------------------------------------------------------------------------

project( PanTilt )

#-------------------------------------------------------------------------------
# Test environment variable MAINPROJECT
if( DEFINED ENV{MAINPROJECT} )

	set( MAIN_PROJECT $ENV{MAINPROJECT} )
	set( LIBRARIES_DIR ${MAIN_PROJECT}/Libraries )

	include( ${LIBRARIES_DIR}/CMakeScripts/CommonScripts.cmake )

else()
	message( SEND_ERROR "Environment variable 'MAINPROJECT' not defined!" )
endif()

#-------------------------------------------------------------------------------
# Define global MSVC heap allocation parameters
Setup_Global_MSVC_Heap_Allocation( ON 0 )

#-------------------------------------------------------------------------------

set( MODULES_DIR ${MAIN_PROJECT}/Hardware/YARP_Modules )


#-------------------------------------------------------------------------------
# Directories with source code to be compiled
set( SRC_DIRS 
	${CMAKE_CURRENT_SOURCE_DIR} 
	${MODULES_DIR}/pantilt_ptud46
	${MODULES_DIR}/serial_asio
)

include_directories(
	${SRC_DIRS}
	$ENV{ACE_ROOT}
)

link_directories(
	$ENV{ACE_ROOT}/lib
)

#-------------------------------------------------------------------------------
# Configure Serial Port implementation

find_package( Boost REQUIRED system )
include_directories( ${Boost_INCLUDE_DIRS} )
link_directories( ${Boost_LIBRARY_DIRS} )

add_definitions( -DUSE_BOOST_SERIAL_ASIO )

# set( PANTILT_SERIAL_PORT_TYPE "boost::serial_asio" CACHE STRING "Select the desired Serial Port implementation to use with BarrettHand, must match the one selected in Yarp" )
# set_property( CACHE PANTILT_SERIAL_PORT_TYPE PROPERTY STRINGS "boost::serial_asio" "ace::serial_io" )

# if( PANTILT_SERIAL_PORT_TYPE STREQUAL "ace::serial_io" )
	# include_directories( ${MODULES_DIR}/serial_io )
	# add_definitions( -DUSE_ACE_SERIAL_IO )

# else()
	# Configure_Package( Boost )
	# include_directories( 
		# ${MODULES_DIR}/serial_asio
		# ${Boost_INCLUDE_DIRS} )

	# link_directories( ${Boost_LIBRARY_DIRS} )

	# add_definitions( -DUSE_BOOST_SERIAL_ASIO )

# endif()

# if( DEFINED ENV{Boost_ROOT} )
# endif()

#-------------------------------------------------------------------------------
	
#set( external_packages
#	yarp
#	filesdirs
#	mathlib
#	utilities
#	yarpcommunication
#)
#configure_package_list( ${external_packages} )

#Project dependencies include directories


Configure_Package(FilesDirs)
Configure_Package(MathLib)
Configure_Package(Utilities)
Configure_Package(YARP)
Configure_Package(YarpCommunication)


#-------------------------------------------------------------------------------
# Create groups of files: Header and Source files

set( HEADER_EXTENSIONS *.h *.hpp )
set( SOURCE_EXTENSIONS *.cpp )

# Look through all external source directories for source and header files
foreach( source_dir ${SRC_DIRS} )

	# Find source files in directories
	foreach( source_ext ${SOURCE_EXTENSIONS} )
		file( GLOB_RECURSE FOUND_FILES ${source_dir}/${source_ext} )
		set( SOURCE_FILES ${SOURCE_FILES} ${FOUND_FILES} )
	endforeach()
	
	# Find header files in directories
	foreach( header_ext ${HEADER_EXTENSIONS} )
		file( GLOB_RECURSE FOUND_FILES ${source_dir}/${header_ext} )
		set( HEADER_FILES ${HEADER_FILES} ${FOUND_FILES} )
	endforeach()

endforeach()

# Create a list with resource files
#file( GLOB RESOURCE_FILES res/*.rc* res/*.ico )

# Exclude all source and header files in build folder
file( GLOB_RECURSE FILES_TO_REMOVE ${CMAKE_CURRENT_BINARY_DIR}/*.* )
list( REMOVE_ITEM HEADER_FILES ${FILES_TO_REMOVE} )
list( REMOVE_ITEM SOURCE_FILES ${FILES_TO_REMOVE} )

Create_File_Groups(  "${HEADER_FILES}" "${SOURCE_FILES}" )

#-------------------------------------------------------------------------------
# Define what files to be compiled for the application
set( myApp_SRCS
	${SOURCE_FILES}
	${HEADER_FILES}
)

#--------------------------------------------------------------------------------------------

#create the executable
add_executable( ${PROJECT_NAME} ${myApp_SRCS} )
target_link_libraries( ${PROJECT_NAME} $ENV{ACE_ROOT}/lib/ACEd.lib )
Link_libraries_to_project()
