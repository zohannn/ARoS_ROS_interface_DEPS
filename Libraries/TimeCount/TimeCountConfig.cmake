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

Configure_Package( MutexWrapper )

Configure_Dependent_Library( TimeCount ${LIBRARIES_DIR}/TimeCount )
	
Configure_Project_Dependencies( 
	TimeCount 
	OpenCV
	)

Add_Project_USE_Flags( TimeCount STL )

set( TimeCount_FOUND TRUE )
