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

Configure_Dependent_Library( ImageIO ${MAIN_PROJECT}/ImageLibs/ImageIO )

set( DEPENDENT_PROJECTS
	OpenCV
	#YARP
	SVS
	CMU1394
	CLEye
	)
	
foreach( dep ${DEPENDENT_PROJECTS} )
	if( ${ImageIO_PACKAGE_${dep}} )
		set( PACKAGE_LIST ${PACKAGE_LIST} ${dep} )
	endif()
endforeach()

Configure_Package_List(
	${PACKAGE_LIST}
	FilesDirs
	MutexWrapper
	Utilities
	#YarpCommunication
	)
	
Add_Project_USE_Flags( ImageIO ${DEPENDENT_PROJECTS} )
	
add_definitions( -DIMAGE_IO_DIR="${ImageIO_DIR}" )

set( ImageIO_FOUND TRUE )
