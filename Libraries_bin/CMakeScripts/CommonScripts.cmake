#---------------------------------------------------------------------------------------------------------------------------
# Configure the compiler with the appropriate WINVER value to the used version of Visual Studio
# Call: Configure_WINVER()
macro( Configure_WINVER )

if( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 6" )
	#use definitions for Visual Studio 6
	add_definitions( -DWINVER=0x0400 )
	
else( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 6" )
	#choose appropriate settings depending on OS version for Visual Studio other than VS6

	if( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.1 )
		#Windows 7
		add_definitions( -DWINVER=0x0601 )

	elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.0 )
		#Windows Vista or Server 2008
		add_definitions( -DWINVER=0x0600 )

	elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 5.2 )
		#Windows Server 2003 with SP1, Windows XP with SP2
		add_definitions( -DWINVER=0x0502 )
	
	elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 5.1 )
		#Windows Server 2003, Windows XP
		add_definitions( -DWINVER=0x0501 )

	else( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.1 )
		# VERSION_LESS 5.1 
		# Windows 2000
		add_definitions( -DWINVER=0x0500 )
	endif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.1 )

	add_definitions( -D_CRT_SECURE_NO_WARNINGS ) # disable function deprecation warning
	
endif( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 6" )

endmacro( Configure_WINVER )

#---------------------------------------------------------------------------------------------------------------------------
# Create file groups in Visual Studio. The first two parameters are mandatory, the third is optional
# Call: Create_File_Groups( "<header_file_list>" "<source_file_list>" ["<resource_file_list>"] )
macro( Create_File_Groups )

if( NOT ( ${ARGC} LESS 2 ) )

		if( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10" )
			set( GROUP_NAME "Header Files" )
		else( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10" )
			set( GROUP_NAME "Header" )
		endif( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10" )

		source_group( ${GROUP_NAME} FILES ${ARGV0} )
		
		if( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10" )
			set( GROUP_NAME "Source Files" )
		else( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10" )
			set( GROUP_NAME "Source" )
		endif( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10" )

		source_group( ${GROUP_NAME} FILES ${ARGV1} )
		
		if( ${ARGC} GREATER 2 )

			if( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10" )
				set( GROUP_NAME "Resource Files" )
			else( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10" )
				set( GROUP_NAME "Resource" )
			endif( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 10" )

			source_group( ${GROUP_NAME} FILES ${ARGV2} )

		endif( ${ARGC} GREATER 2 )

else( NOT ( ${ARGC} LESS 2 ) )

		message( SEND_ERROR "Macro Create_File_Groups called with insufficient parameters" )

endif( NOT ( ${ARGC} LESS 2 ) )

endmacro( Create_File_Groups )


#---------------------------------------------------------------------------------------------------------------------------
# Configure a package based in an environment variable with the same name with the suffix _ROOT
# or passing as a second parameter the path where the config script is
macro( Configure_Package )

	if( ${ARGC} GREATER 1 )
		set( ${ARGV0}_DIR ${ARGV1} CACHE LOCATION "Where is your ${ARGV0} directory?" )
	else( ${ARGC} GREATER 1 )
		set( ${ARGV0}_DIR $ENV{${ARGV0}_ROOT} CACHE LOCATION "Where is your ${ARGV0} directory?" )
	endif( ${ARGC} GREATER 1 )
	
	find_package( ${ARGV0} REQUIRED )
	
	# YARP needs additional configuration steps
	if( ${ARGV0} STREQUAL YARP )
		include_directories( 	
			$ENV{ACE_ROOT}
			${YARP_INCLUDE_DIRS}
		)
	
		link_libraries( ${YARP_LIBRARIES} )
	endif( ${ARGV0} STREQUAL YARP )

endmacro( Configure_Package )

#---------------------------------------------------------------------------------------------------------------------------
macro( Configure_Dependent_Library )

if( NOT ( ${ARGC} LESS 2 ) )

	if( NOT TARGET ${ARGV0} )
		set( ${ARGV0}_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/${ARGV0} )
		add_subdirectory( ${ARGV1} ${${ARGV0}_BUILD_DIR} )
	endif( NOT TARGET ${ARGV0} )

	include_directories(
		${ARGV1}
		${ARGV1}/include
	) 

	link_directories(
		${${ARGV0}_BUILD_DIR}/${CMAKE_BUILD_TYPE}
	)

	link_libraries( ${ARGV0} )

else( NOT ( ${ARGC} LESS 2 ) )

		message( SEND_ERROR "Macro Configure_Dependent_Library called with insufficient parameters" )

endif( NOT ( ${ARGC} LESS 2 ) )

endmacro( Configure_Dependent_Library )

#---------------------------------------------------------------------------------------------------------------------------
# configure include directory and library to link based in the assumption that the folder where header files
# is <LIB_..._BINARIES_DIR>/include and binaries are in <LIB_..._BINARIES_DIR>/lib/<OS>-<ARCHITECTURE>/
# THe library name should end in d.lib for debug and .lib for Release

macro( Configure_Library_Binaries )

if( NOT ( ${ARGC} LESS 2 ) )

	set( LIB_${ARGV0}_BINARIES_DIR ${ARGV1} CACHE LOCATION "Where is your library binaries directory?" )
	
	include_directories( ${LIB_${ARGV0}_BINARIES_DIR}/include )

	#---------------------------------------------------------------------------
	# Test Operating System version
	if( ${CMAKE_SYSTEM_VERSION} VERSION_LESS 6.0 )
		set( LIB_OS WinXP )
	endif( ${CMAKE_SYSTEM_VERSION} VERSION_LESS 6.0 )

	if( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.0 )
		set( LIB_OS WinVista )
	endif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.0 )

	if( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.1 )
		set( LIB_OS Win7 )
	endif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.1 )
	
	#---------------------------------------------------------------------------
	# Test compiler version
	
	if( ${MSVC_VERSION} VERSION_EQUAL 1200 )
		set( COMPILER_VERSION vc6 )
	endif( ${MSVC_VERSION} VERSION_EQUAL 1200 )

	if( ${MSVC_VERSION} VERSION_EQUAL 1300 )
		set( COMPILER_VERSION vc7 )
	endif( ${MSVC_VERSION} VERSION_EQUAL 1300 )

	if( ${MSVC_VERSION} VERSION_EQUAL 1310 )
		set( COMPILER_VERSION vc71 )
	endif( ${MSVC_VERSION} VERSION_EQUAL 1310 )

	if( ${MSVC_VERSION} VERSION_EQUAL 1400 )
		set( COMPILER_VERSION vc8 )
	endif( ${MSVC_VERSION} VERSION_EQUAL 1400 )

	if( ${MSVC_VERSION} VERSION_EQUAL 1500 )
		set( COMPILER_VERSION vc9 )
	endif( ${MSVC_VERSION} VERSION_EQUAL 1500 )

	if( ${MSVC_VERSION} VERSION_EQUAL 1600 )
		set( COMPILER_VERSION vc10 )
	endif( ${MSVC_VERSION} VERSION_EQUAL 1600 )

	#---------------------------------------------------------------------------
	
	link_libraries( 
		debug;${LIB_${ARGV0}_BINARIES_DIR}/lib/${LIB_OS}-${CMAKE_SYSTEM_PROCESSOR}-${COMPILER_VERSION}/${ARGV0}d.lib;
		optimized;${LIB_${ARGV0}_BINARIES_DIR}/lib/${LIB_OS}-${CMAKE_SYSTEM_PROCESSOR}-${COMPILER_VERSION}/${ARGV0}.lib;
		)

else( NOT ( ${ARGC} LESS 2 ) )
	message( SEND_ERROR "Macro Configure_Library_Binaries called with insufficient parameters" )
endif( NOT ( ${ARGC} LESS 2 ) )

endmacro( Configure_Library_Binaries )
	
#---------------------------------------------------------------------------------------------------------------------------
# Add proper definitions to compile code with MFC's
macro( Configure_MFC )
	add_definitions( -D_AFXDLL )	# use MFC's
	set( CMAKE_MFC_FLAG 2 )		# use MFC's as a shared DLL
endmacro( Configure_MFC )

#---------------------------------------------------------------------------------------------------------------------------

macro( Display_Config_Message )
	message( STATUS "Configuring project " ${PROJECT_NAME} " using " ${CMAKE_GENERATOR} " in " ${CMAKE_SYSTEM_NAME} " " ${CMAKE_SYSTEM_VERSION} )
endmacro( Display_Config_Message )

#---------------------------------------------------------------------------------------------------------------------------
# Make sure that versions of Visual Studio before VS2010 recognize nullptr as NULL
macro( Handle_nullptr_compatibility )
	if( ${MSVC_VERSION} LESS 1600 )
		add_definitions( -Dnullptr=NULL )
	endif( ${MSVC_VERSION} LESS 1600 )
endmacro( Handle_nullptr_compatibility )
