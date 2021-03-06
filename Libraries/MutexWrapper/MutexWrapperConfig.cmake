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

Configure_Dependent_Library( MutexWrapper ${LIBRARIES_DIR}/MutexWrapper )

# if( MutexWrapper_PACKAGE_YARP )
	# Configure_Package( YARP )
# endif()

Add_Project_USE_Flags( 
	MutexWrapper 
	#YARP 
	STL )

set( MutexWrapper_FOUND TRUE )
