cmake_minimum_required( VERSION 2.8 )
cmake_policy( VERSION ${CMAKE_VERSION} )

include( $ENV{MAINPROJECT}/Libraries/CMakeScripts/CommonScripts.cmake )

if( MSVC )

	include_directories( ${cvBlob_DIR}/include )

	Configure_System_Architecture()

	Configure_MSVC_Compiler_Version()
	
	link_directories( ${cvBlob_DIR}/lib/${SYSTEM_ARCH}/${COMPILER_VERSION}/${CMAKE_BUILD_TYPE} )
	
	set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES cvblob )
					
	set( cvBlob_FOUND TRUE )
	
else()

	message( SEND_ERROR "Existing cvBlob precompiled binaries only support Visual Studio, if you are not using Visual Studio you must compile the lib yourself" )

endif()
