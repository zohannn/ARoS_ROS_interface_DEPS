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

Configure_Dependent_Library( ImageUtils ${MAIN_PROJECT}/ImageLibs/ImageUtils )

include_directories( ${LIBRARIES_DIR}/TypesDefs )

Configure_Package_List(
	OpenCV
	FilesDirs
	MathLib
	MutexWrapper
	Utilities
	)

set( ImageUtils_FOUND TRUE )
