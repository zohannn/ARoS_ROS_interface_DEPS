if( COMPILE_DEVICE_LIBRARY )
  YARP_PREPARE_DEVICE( pantilt_ptud46 TYPE PanTilt_PTUD46 INCLUDE pantilt_ptud46.h WRAPPER controlboard )
endif()

if( NOT SKIP_pantilt_ptud46 )

	set( MODULES_DIR $ENV{MAINPROJECT}/Hardware/YARP_Modules )

	#-------------------------------------------------------------------------------
	# Configure YARP
	find_package( YARP REQUIRED )

	include_directories( ${YARP_INCLUDE_DIRS} )

	#-------------------------------------------------------------------------------
	# Configure Serial Port implementation

	set( YARPMOD_PANTILT_SERIAL_PORT_TYPE "boost::serial_asio" CACHE STRING "Select the desired Serial Port implementation to use with BarrettHand" )
	set_property( CACHE YARPMOD_PANTILT_SERIAL_PORT_TYPE PROPERTY STRINGS "boost::serial_asio" "ace::serial_io" )

	if( YARPMOD_PANTILT_SERIAL_PORT_TYPE STREQUAL "ace::serial_io" )
		include_directories( 
			${MODULES_DIR}/serial_io
			$ENV{ACE_ROOT} )
			
		add_definitions( -DUSE_ACE_SERIAL_IO )

	else()
		find_package( Boost REQUIRED )

		include_directories( 
			${Boost_INCLUDE_DIRS}
			${MODULES_DIR}/serial_asio )
		
		link_directories( ${Boost_LIBRARY_DIRS} )

		add_definitions( -DUSE_BOOST_SERIAL_ASIO )

	endif()

	#-------------------------------------------------------------------------------
	# Configure preprocessor definition _WIN32_WINNT

	if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
		# if the target machine is Windows, configure _WIN32_WINNT
		if( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.2 )
			set( TARGET_OS 602 ) # Windows 8
		elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.1 )
			set( TARGET_OS 601 ) # Windows 7
		elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.0 )
			set( TARGET_OS 600 ) # Windows Vista or Server 2008
		elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 5.2 )
			set( TARGET_OS 502 ) # Windows Server 2003 with SP1, Windows XP with SP2
		elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 5.1 )
			set( TARGET_OS 501 ) # Windows Server 2003, Windows XP
		else()
			set( TARGET_OS 500 ) # Windows 2000 or less
		endif()
			
		add_definitions( -D_WIN32_WINNT=0x0${TARGET_OS} )
	endif()

	#-------------------------------------------------------------------------------
	YARP_ADD_PLUGIN( pantilt_ptud46 pantilt_ptud46.cpp pantilt_ptud46.h pantilt_definitions.h )

	target_link_libraries( pantilt_ptud46 PRIVATE ${YARP_LIBRARIES} )

endif()
