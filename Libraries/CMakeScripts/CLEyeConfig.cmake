# -------------------------------------------------------------------------------------------------
# CMake script for configuring Code Laboratories SDK on Windows to interface with PS3 Eye Camera
# Author: Rui Silva

# -------------------------------------------------------------------------------------------------

# How to use:

# Copy this file to where CLEye is installed, tipically $ENV{CLEye_ROOT}

# then on the project's CMakeLists there are two configuration options:

# --------------------
# 1:

# set( CLEye_DIR	$ENV{CLEye_ROOT} )
# find_package( CLEye REQUIRED )

# --------------------
# 2:

# include( $ENV{MAINPROJECT}/Libraries/CMakeScripts/CommonScripts.cmake )
# Configure_Package( CLEye )

# -------------------------------------------------------------------------------------------------

if( NOT CLEye_FOUND )

	message( STATUS "Using CL-Eye Platform SDK" )

	#---------------------------------------------------------------------------------
	# Set main variable that controls configuration
	set( CLEye_DIR $ENV{CLEye_ROOT} )

	#---------------------------------------------------------------------------------
	# Configure include directories

	include_directories( ${CLEye_DIR}/Include )

	#---------------------------------------------------------------------------------
	# Configure Linker libraries

	# Folder where standard libraries are
	set( CLEye_LIBS_DIR ${CLEye_DIR}/Lib )
	
	# Create a list of standard libraries
	file( GLOB CLEye_LIBS ${CLEye_LIBS_DIR}/*.lib )

	link_directories( ${CLEye_LIBS_DIR} )

	set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES ${CLEye_LIBS} )

	#---------------------------------------------------------------------------------
	# The pacakge was found
	set( CLEye_FOUND TRUE )

endif()
