# -------------------------------------------------------------------------------------------------
# CMake script for configuring MFC Chart Control library
# Author: Rui Silva

# -------------------------------------------------------------------------------------------------

# How to use:

# Copy this file to where MFC Chart Control is located

# then on the project's CMakeLists there are two configuration options:

# --------------------
# 1:

# set( MFCChartControl_DIR	$ENV{MFCChartControl_ROOT} )
# find_package( MFCChartControl REQUIRED )

# --------------------
# 2:

# include( $ENV{MAINPROJECT}/Libraries/CMakeScripts/CommonScripts.cmake )
# Configure_Package( MFCChartControl )

# -------------------------------------------------------------------------------------------------

if( NOT MFCChartControl_FOUND )

	message( STATUS "Using MFC Chart Control" )

	#---------------------------------------------------------------------------------
	# Set main variable that controls configuration
	set( MFCChartControl_DIR $ENV{MFCChartControl_ROOT} )

	#---------------------------------------------------------------------------------
	# Configure include directories

	include_directories( ${MFCChartControl_DIR}/include )

	#---------------------------------------------------------------------------------
	# Configure Linker libraries

	# Folder where standard libraries are
	link_directories( ${MFCChartControl_DIR}/lib )

	Configure_System_Architecture()

	Add_to_link_libraries( 
		debug ChartCtrlLib-${SYSTEM_ARCH}d.lib
		optimized ChartCtrlLib-${SYSTEM_ARCH}.lib )
	
	#---------------------------------------------------------------------------------
	# The pacakge was found
	set( MFCChartControl_FOUND TRUE )

endif()
