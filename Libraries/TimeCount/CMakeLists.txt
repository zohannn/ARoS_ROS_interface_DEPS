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

project( TimeCount )

set( EXTERNAL_PACKAGES
	OpenCV
	#YARP
	STL
	)

# Test Visual Studio version, if it's inferior to Visual Studio 12, disable the use of STL
if( ${MSVC_VERSION} VERSION_LESS 1700 )
	set( ${PROJECT_NAME}_PACKAGE_STL OFF )
else()
	set( ${PROJECT_NAME}_PACKAGE_STL ON CACHE BOOL "Use ${ext_pkg} in project ${PROJECT_NAME}?" )
endif()

foreach( ext_pkg ${EXTERNAL_PACKAGES} )
	
	if( NOT ${ext_pkg} STREQUAL STL )
	
		set( ${PROJECT_NAME}_PACKAGE_${ext_pkg} ON CACHE BOOL "Use ${ext_pkg} in project ${PROJECT_NAME}?" )
		
		if( ${PROJECT_NAME}_PACKAGE_${ext_pkg} )
			Configure_Package( ${ext_pkg} )
		endif()
	
	endif()
	
endforeach()

if( ${PROJECT_NAME}_PACKAGE_STL )
	add_definitions( -DUSE_STL )
endif()

Setup_Library_Standard_Config( MutexWrapper )
