# -------------------------------------------------------------------------------------------------
# CMake script for configuring C= Parallel C/C++ Programming Language Extension
# Author: Rui Silva

# -------------------------------------------------------------------------------------------------

# How to use:

# Copy this file to where C= is installed, tipically $ENV{CStripes_ROOT}

# then on the project's CMakeLists there are two configuration options:

# --------------------
# 1:

# set( CStripes_DIR	$ENV{CStripes_ROOT} )
# find_package( CStripes REQUIRED )

# --------------------
# 2:

# include( $ENV{MAINPROJECT}/Libraries/CMakeScripts/CommonScripts.cmake )
# Configure_Package( CStripes )

# -------------------------------------------------------------------------------------------------

if( NOT CStripes_FOUND )

	#---------------------------------------------------------------------------------
	# Set main variable that controls configuration
	set( CStripes_DIR $ENV{CStripes_ROOT} )
	
	message( STATUS "Configuring C= Parallel C/C++ Programming Language Extension" )
	
	if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
		set( OS_Name Windows )
	else()
		set( OS_Name Linux )
	endif()

	#---------------------------------------------------------------------------------
	# Configure include directories
	set( CStripes_INCLUDE_DIRS ${CStripes_DIR}/${OS_Name} )
	include_directories( ${CStripes_INCLUDE_DIRS} )

	#---------------------------------------------------------------------------------
	# Configure Linker libraries

	if( ${PROCESSOR_ARCHITECTURE} STREQUAL "x86" )
		set( SYSTEM_ARCH x86 )
	else()
		set( SYSTEM_ARCH x64 )
	endif()

	# Folder where standard libraries are
	set( CStripes_LIBS_DIR ${CStripes_DIR}/${OS_Name}/${SYSTEM_ARCH} )
	
	# Create a list of standard libraries
	file( GLOB CStripes_LIBS ${CStripes_LIBS_DIR}/*.lib )

	link_directories( ${CStripes_LIBS_DIR} )
	set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES ${CStripes_LIBS} )

	#---------------------------------------------------------------------------------
	# The package was found
	set( CStripes_FOUND TRUE )

endif()
