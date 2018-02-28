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

project( MutexWrapper )

set( EXTERNAL_PACKAGES
	# YARP
	STL
	)

foreach( ext_pkg ${EXTERNAL_PACKAGES} )
	
	# Test Visual Studio version, if it's inferior to Visual Studio 12, disable the use of STL
	if( ( ${ext_pkg} STREQUAL STL ) AND ( ${MSVC_VERSION} VERSION_LESS 1700 ) )
			set( ${PROJECT_NAME}_PACKAGE_STL OFF )
	else()
		set( ${PROJECT_NAME}_PACKAGE_${ext_pkg} ON CACHE BOOL "Use ${ext_pkg} in project ${PROJECT_NAME}?" )
	endif()

	if( ( ${PROJECT_NAME}_PACKAGE_${ext_pkg} ) AND ( NOT ${ext_pkg} STREQUAL STL ) )
		Configure_Package( ${ext_pkg} )
	endif()
	
endforeach()

if( ${PROJECT_NAME}_PACKAGE_STL )
	add_definitions( -DUSE_STL )
endif()

Setup_Library_Standard_Config()
