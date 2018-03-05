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
# Define global MSVC heap allocation parameters
Setup_Global_MSVC_Heap_Allocation( ON 0 )

#-------------------------------------------------------------------------------

project( BarrettHand )

set( MODULES_DIR ${MAIN_PROJECT}/Hardware/YARP_Modules )

SubDirList( SUB_DIRECTORIES_LIST ${CMAKE_CURRENT_SOURCE_DIR} )

include_directories(
	${SUB_DIRECTORIES_LIST}
	${MODULES_DIR}/barrett_hand_826X
	${MODULES_DIR}/serial_asio
	${MODULES_DIR}/serial_io
	) 

#-------------------------------------------------------------------------------
# Configure Serial Port implementation

set( BARRETTHAND_SERIAL_PORT_TYPE "boost::serial_asio" CACHE STRING "Select the desired Serial Port implementation to use with BarrettHand, must match the one selected in Yarp" )
set_property( CACHE BARRETTHAND_SERIAL_PORT_TYPE PROPERTY STRINGS "boost::serial_asio" "ace::serial_io" )

if( BARRETTHAND_SERIAL_PORT_TYPE STREQUAL "ace::serial_io" )
	include_directories( ${MODULES_DIR}/serial_io )
	add_definitions( -DUSE_ACE_SERIAL_IO )

else()
	Configure_Package( Boost )
	include_directories( 
		${Boost_INCLUDE_DIRS}
		${MODULES_DIR}/serial_asio )
	
	link_directories( ${Boost_LIBRARY_DIRS} )

	add_definitions( -DUSE_BOOST_SERIAL_ASIO )

endif()


#-------------------------------------------------------------------------------
Setup_Application_Standard_Config(
	YARP
	TimeCount
	YarpCommunication
	)
