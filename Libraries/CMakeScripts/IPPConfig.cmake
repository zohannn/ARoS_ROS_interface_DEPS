# -------------------------------------------------------------------------------------------------
# CMake script for configuring Intel Integrated Performance Primitives v6.1 on Windows
# Author: Rui Silva

# -------------------------------------------------------------------------------------------------

# How to use:

# Copy this file to where IPP is installed, tipically $ENV{IPP_ROOT}

# then on the project's CMakeLists there are two configuration options:

# --------------------
# 1:

# set( IPP_DIR	$ENV{IPP_ROOT} )
# find_package( IPP REQUIRED )

# --------------------
# 2:

# include( $ENV{MAINPROJECT}/Libraries/CMakeScripts/CommonScripts.cmake )
# Configure_Package( IPP )

# -------------------------------------------------------------------------------------------------

if( NOT IPP_FOUND )

	message( STATUS "Using Intel Integrated Performance Primitives v6.1" )

	# Assume the process goes right
	set( IPP_FOUND TRUE )
	
	if( DEFINED ENV{IPP_ROOT} )

		if( EXISTS $ENV{IPP_ROOT} )
			set( IPP_DIR $ENV{IPP_ROOT} )
		else()
			message( SEND_ERROR "IPP_ROOT Environment variable points to a folder that does not exist." )
			set( IPP_FOUND FALSE )
		endif()

	else()
		message( SEND_ERROR "IPP_ROOT Environment variable not defined." )
		set( IPP_FOUND FALSE )
	endif()
	
	set( IPP_INCLUDE_DIR ${IPP_DIR}/include )

	if( EXISTS ${IPP_INCLUDE_DIR} )
		include_directories( ${IPP_INCLUDE_DIR} )
	else()
		message( SEND_ERROR "IPP include directory does not exist." )
		set( IPP_FOUND FALSE )
	endif()

	# ----------------------------------------------------------------------------------------------------------------
	# IPP regular libs
	
	set( IPP_STUB_LIBS_DIR ${IPP_DIR}/stublib )

	if( EXISTS ${IPP_STUB_LIBS_DIR} )
		link_directories( ${IPP_STUB_LIBS_DIR} )
	else()
		message( SEND_ERROR "IPP main library directory does not exist." )
		set( IPP_FOUND FALSE )
	endif()

	file( GLOB IPP_STUB_LIBS ${IPP_STUB_LIBS_DIR}/*.lib )
	
	# Count lib files
	list( LENGTH IPP_STUB_LIBS NUMBER_LIBS )

	if( ${NUMBER_LIBS} GREATER 0 )
		set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES ${IPP_STUB_LIBS} )
	else()
		# Issue an error complaining that no lib files were found
		message( SEND_ERROR "IPP library directory '" ${IPP_STUB_LIBS_DIR} "' does not contain any library files." )
		set( IPP_FOUND FALSE )
	endif()

	# ----------------------------------------------------------------------------------------------------------------
	# Is it necessary to link extra libraries? Probably not.
	option( LINK_IPP_EXTRA_LIBS "Add extra libs from IPP 6.1 to the linker (in folder lib, usually only libs in folder stublib are necessary)" OFF )
	
	if( ${LINK_IPP_EXTRA_LIBS} )
		
		set( IPP_LIBS_DIR ${IPP_DIR}/lib )

		if( EXISTS ${IPP_LIBS_DIR} )
			link_directories( ${IPP_LIBS_DIR} )
		else()
			message( SEND_ERROR "IPP extra library directory does not exist." )
			set( IPP_FOUND FALSE )
		endif()

		file( GLOB IPP_LIBS ${IPP_LIBS_DIR}/*.lib )

		# Count lib files
		list( LENGTH IPP_LIBS NUMBER_LIBS )

		if( ${NUMBER_LIBS} GREATER 0 )
			set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES ${IPP_LIBS} )
		else()
			# Issue an error complaining that no lib files were found
			message( SEND_ERROR "IPP library directory '" ${IPP_LIBS_DIR} "' does not contain any library files." )
			set( IPP_FOUND FALSE )
		endif()

	endif()
	
	# ----------------------------------------------------------------------------------------------------------------
	# Necessary to identify the used package in the code, so that multiple versions of IPP can coexist peacefully
	if( IPP_FOUND )
		add_definitions( -DIPP_VERSION_MAJOR=6 )
		add_definitions( -DIPP_VERSION_MINOR=1 )
		add_definitions( -DIPP_VERSION_UPDATE=2 )
	endif()
		
endif( NOT IPP_FOUND )
