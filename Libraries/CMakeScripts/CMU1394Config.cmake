# CMake script for configuring Carnegie Mellon API to capture from firewire cameras on Windows
# Author: Toni Machado

IF( NOT CMU1394_FOUND )

	message( STATUS "Using Carnegie Mellon API to capture from firewire cameras." )

	#---------------------------------------------------------------------------------
	# Set main variable that controls configuration
	set( CMU1394_DIR $ENV{CMU1394_ROOT} )

	#---------------------------------------------------------------------------------
	# Configure include directories
	set( CMU1394_INCLUDE_DIRS "${CMU1394_DIR}/include" )
	include_directories( ${CMU1394_INCLUDE_DIRS} )

	#---------------------------------------------------------------------------------
	# Configure Linker libraries
	link_directories( ${CMU1394_DIR}/lib )
	
	set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES 
		debug 1394Camerad.lib
		optimized 1394Camera.lib
	)
	
	#---------------------------------------------------------------------------------
	# The pacakge was found
	set( CMU1394_FOUND TRUE )

endif( NOT CMU1394_FOUND )
