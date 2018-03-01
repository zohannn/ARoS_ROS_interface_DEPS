# CMake script for configuring Seeing Machines' faceAPI on Windows
# Author: Rui Silva

if( NOT faceAPI_FOUND )

	set( USE_FACEAPI_CPP_WRAPPERS ON CACHE BOOL "Use C++ wrappers for faceAPI?" )

	set( ENV_VARS
		SM_API_PATH
	)
	
	if( ${USE_FACEAPI_CPP_WRAPPERS} )
		
		set( ENV_VARS 
			${ENV_VARS}
			SM_API_CPP_WRAPPERS )
			
		set( USE_FACEAPI_QT_WIDGETS OFF CACHE BOOL "Use Qt widgets for faceAPI?" )
			
		if( ${USE_FACEAPI_QT_WIDGETS} )
			
			set( ENV_VARS 
				${ENV_VARS}
				SM_API_WIDGETS	)
				
		endif()
		
	endif()
	

	foreach( env_var ${ENV_VARS} )

		if( NOT IS_DIRECTORY $ENV{${env_var}} )
			message( SEND_ERROR "Environment variable " ${env_var} " not set or points to an invalid directory" )
		endif()
	
	endforeach()

	message( STATUS "Using Seeing Machines' faceAPI" )

	set( API_PATH $ENV{SM_API_PATH} )
	
	#--------------------------------------------------------------------------
	# Configure include directories
	include_directories( ${API_PATH}/include )

	#--------------------------------------------------------------------------
	# Configure Linker libraries
	
	link_directories( ${API_PATH}/bin )
	
	set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES $ENV{SM_API_LIB_NAME} )

	#--------------------------------------------------------------------------

	if( ${USE_FACEAPI_CPP_WRAPPERS} )

		set( CPP_WRAPPERS_PATH $ENV{SM_API_CPP_WRAPPERS} )
		
		include_directories( ${CPP_WRAPPERS_PATH}/include )
		
		set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES 
			debug ${CPP_WRAPPERS_PATH}/lib_dbg/smftcpp.lib
			optimized ${CPP_WRAPPERS_PATH}/lib/smftcpp.lib
			)

			if( ${USE_FACEAPI_QT_WIDGETS} )

			set( QT_WIDGETS_PATH $ENV{SM_API_WIDGETS} )

				include_directories( ${QT_WIDGETS_PATH}/include )

				set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES 
					debug ${QT_WIDGETS_PATH}/lib_dbg/smftqt.lib
					optimized ${QT_WIDGETS_PATH}/lib/smftqt.lib
					)
					
			endif()

	endif()
	
	#--------------------------------------------------------------------------
	# The package was found
	set( faceAPI_FOUND TRUE )

endif()
