# -------------------------------------------------------------------------------------------------
# CMake script for configuring Videre Design's Small Vision System on Windows
# Author: Rui Silva

# -------------------------------------------------------------------------------------------------

# How to use:

# Copy this file to where SVS is installed, tipically $ENV{SVS_ROOT}

# then on the project's CMakeLists there are two configuration options:

# --------------------
# 1:

# set( SVS_DIR	$ENV{SVS_ROOT} )
# find_package( SVS REQUIRED )

# --------------------
# 2:

# include( $ENV{MAINPROJECT}/Libraries/CMakeScripts/CommonScripts.cmake )
# Configure_Package( SVS )

# -------------------------------------------------------------------------------------------------

if( NOT SVS_FOUND )

	message( STATUS "Using Videre Design's Small Vision System" )

	#---------------------------------------------------------------------------------
	# Set main variable that controls configuration
	set( SVS_DIR $ENV{SVS_ROOT} )

	#---------------------------------------------------------------------------------
	# Configure include directories
	include_directories( ${SVS_DIR}/src )

	#---------------------------------------------------------------------------------
	# Configure Linker libraries

	# Folder where standard libraries are
	set( SVS_STANDARD_LIBS_DIR ${SVS_DIR}/bin )
	
	# Create a list of standard libraries
	file( GLOB SVS_LIBS_STANDARD ${SVS_STANDARD_LIBS_DIR}/*.lib )

	link_directories( ${SVS_STANDARD_LIBS_DIR} )
	set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES ${SVS_LIBS_STANDARD} )
	
	# Is it necessary to link calibration libraries? Probably not.
	option( LINK_SVS_CALIBRATION_LIBS "Add Videre Design's SVS calibration libraries to the linker? (Usually not necessary)" OFF )
	
	if( ${LINK_CALIBRATION_LIBS} )

		set( SVS_CALIBRATION_LIBS_DIR ${SVS_DIR}/bincal )

		# Create a list of calibration libraries
		file( GLOB SVS_LIBS_CALIBRATION ${SVS_CALIBRATION_LIBS_DIR}/*.lib )

		link_directories( ${SVS_CALIBRATION_LIBS_DIR} )
		set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES ${SVS_LIBS_CALIBRATION} )

	endif()
	
	#---------------------------------------------------------------------------------
	# The pacakge was found
	set( SVS_FOUND TRUE )

endif()
