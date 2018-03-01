#-------------------------------------------------------------------------------
# CMake minimum required version and used policy
cmake_minimum_required( VERSION 2.8 )
cmake_policy( VERSION ${CMAKE_VERSION} )

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

project( ImageIO )

#-------------------------------------------------------------------------------
# Configuration of necessary packages	

set( EXTERNAL_PACKAGES
	CLEye
	CMU1394
	OpenCV
	SVS
	#YARP
	)

foreach( ext_pkg ${EXTERNAL_PACKAGES} )

	if( ( ${ext_pkg} STREQUAL OpenCV ) OR ( ${ext_pkg} STREQUAL YARP ) )
		set( ${PROJECT_NAME}_PACKAGE_${ext_pkg} ON )	# Required
	else()
		set( ${PROJECT_NAME}_PACKAGE_${ext_pkg} OFF CACHE BOOL "Use ${ext_pkg} in the build?" )	# Optional
	endif()
	
	if( ${PROJECT_NAME}_PACKAGE_${ext_pkg} )
		set( PACKAGE_LIST ${PACKAGE_LIST} ${ext_pkg} )
	endif()

endforeach()

#-------------------------------------------------------------------------------
Setup_Library_Standard_Config(
	${PACKAGE_LIST}
	FilesDirs
	MutexWrapper
	Utilities
	#YarpCommunication
	)

#-------------------------------------------------------------------------------
# Add a definition to compiler contatining current directory
add_definitions( -DIMAGE_IO_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

#-------------------------------------------------------------------------------
# Generate an include file that includes all headers at once
Generate_Header_File_Aggregator( ${CMAKE_CURRENT_SOURCE_DIR}/include AllGrabbers.h ) 
