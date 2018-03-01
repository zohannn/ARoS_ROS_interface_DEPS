#---------------------------------------------------------------------------------------------------------------------------
macro( Determine_WINVER )

	if( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.3 )
		# Windows 8.1
		set( WINVER_VAL 603 )
		set( NTDDI_VERSION_VAL "NTDDI_WINBLUE" ) # (0x06030000)

	elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.2 )
		# Windows 8
		set( WINVER_VAL 602 )
		set( NTDDI_VERSION_VAL "NTDDI_WIN8" ) # (0x06020000)
		
	elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.1 )
		#Windows 7
		set( WINVER_VAL 601 )
		set( NTDDI_VERSION_VAL "NTDDI_WIN7" ) # (0x06010000)

	elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.0 )
		#Windows Vista SP1 or Server 2008
		set( WINVER_VAL 600 )
		set( NTDDI_VERSION_VAL "NTDDI_VISTASP1" ) # (0x06010100)

	elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 5.2 )
		#Windows Server 2003 with SP1, Windows XP with SP3
		set( WINVER_VAL 502 )
		set( NTDDI_VERSION_VAL "NTDDI_WINXPSP3" ) # (0x05010300)
	
	else()
		#Windows Server 2003, Windows XP
		set( WINVER_VAL 501 )
		set( NTDDI_VERSION_VAL "NTDDI_WINXP" ) # (0x05010000)
		
	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Configure the compiler with the appropriate values to some specific Windows flags
# WINVER, NTDDI_VERSION and _WIN32_WINNT
# Call: Configure_Windows_OS_Specific_Flags()
macro( Configure_Windows_OS_Specific_Flags )

	if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )

		if( NOT DEFINED WINVER_VAL )
			Determine_WINVER()
		endif()
		
		add_definitions( -D_CRT_SECURE_NO_WARNINGS ) # disable function deprecation warning
		
		add_definitions( -DWINVER=0x0${WINVER_VAL} )

		if( DEFINED NTDDI_VERSION_VAL )
			add_definitions( -DNTDDI_VERSION=${NTDDI_VERSION_VAL} )
			add_definitions( -D_WIN32_WINNT=0x0${WINVER_VAL} )
		endif()

	endif()
	
endmacro()

# Maintain backward compatibility with programs that used this macro
macro( Configure_WINVER )
	Configure_Windows_OS_Specific_Flags()
endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Create file groups in Visual Studio. The first two parameters are mandatory, the third is optional
# Call: Create_File_Groups( "<header_file_list>" "<source_file_list>" ["<resource_file_list>"] )
macro( Create_File_Groups )

	if( NOT ( ${ARGC} LESS 2 ) )

		source_group( "Header Files" FILES ${ARGV0} )
		source_group( "Source Files" FILES ${ARGV1} )
		
		if( ${ARGC} GREATER 2 )
			
			# if ARGV2 is not passed into one variable, counting doesn't work.
			set( RESOURCES_LIST ${ARGV2} )
			
			# Count resource files
			list( LENGTH RESOURCES_LIST NUMBER_RES )
	
			# Create file group "Resource Files" only when files are present
			if( ${NUMBER_RES} GREATER 0 )
				source_group( "Resource Files" FILES ${RESOURCES_LIST} )
			endif()

		endif()

	else()
		message( SEND_ERROR "Macro Create_File_Groups called with insufficient parameters" )
	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Add libraries to property LINK_LIBRARIES to be added later in a target
macro( Add_to_link_libraries )
	if( ${ARGC} GREATER 0 )
		set_property( GLOBAL APPEND PROPERTY LINK_LIBRARIES ${ARGV} )
	else()
		message( SEND_ERROR "No arguments passed to macro 'Add_to_link_libraries'" )
	endif()
endmacro()

# Get link libraries to be passed to a target
macro( Get_link_libraries )
	if( ${ARGC} GREATER 0 )
		get_property( ${ARGV0} GLOBAL PROPERTY LINK_LIBRARIES )
	else()
		message( SEND_ERROR "No arguments passed to macro 'Get_link_libraries'" )
	endif()
endmacro()

macro( Link_libraries_to_target )
	if( ${ARGC} GREATER 0 )
		Get_link_libraries( LIST_LINK_LIBRARIES )
		target_link_libraries( ${ARGV0} ${LIST_LINK_LIBRARIES} )
	else()
		message( SEND_ERROR "No arguments passed to macro 'Link_libraries_to_target'" )
	endif()
endmacro()

macro( Link_libraries_to_project )
		Link_libraries_to_target( ${PROJECT_NAME} )
endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Configure a package based in an environment variable with the same name with the suffix _ROOT
# or passing as a second parameter the path where the config script is
macro( Configure_Package )

	if( ${ARGC} GREATER 1 )
		set( ${ARGV0}_DIR ${ARGV1} CACHE PATH "Where is your ${ARGV0} directory?" )
	else()
	
		if( NOT DEFINED ${ARGV0}_DIR )
			
			if( DEFINED ENV{${ARGV0}_ROOT} )
				set( ${ARGV0}_DIR $ENV{${ARGV0}_ROOT} CACHE PATH "Where is your ${ARGV0} directory?" )
			else()
				Find_Package_Directory( ${ARGV0} )
			endif()
		
		endif()
				
	endif()
	
	find_package( ${ARGV0} REQUIRED )
	
	string( REPLACE "-" "_" PACKAGE_NAME ${ARGV0} )
	
	add_definitions( -DUSE_${PACKAGE_NAME} )
	
	#--------------------------------------------------------------------------
	#Additional configuration steps for some packages
	string( TOUPPER ${ARGV0} PKG_NAME )
	
	#--------------------------------------------------------------------------
	# OpenCV
	if( ( ${PKG_NAME} STREQUAL OPENCV ) AND OpenCV_FOUND )
		include_directories( ${OpenCV_INCLUDE_DIRS} )
		Add_to_link_libraries( ${OpenCV_LIBS} )
	#--------------------------------------------------------------------------
	# YARP
	elseif( ( ${PKG_NAME} STREQUAL YARP ) AND YARP_FOUND )
		include_directories( 	
			$ENV{ACE_ROOT}
			${YARP_INCLUDE_DIRS}
		)
	
		Add_to_link_libraries( ${YARP_LIBRARIES} )

	elseif( ( ${PKG_NAME} STREQUAL Boost ) AND Boost_FOUND )
		include_directories( ${Boost_INCLUDE_DIRS} )
		link_directories( ${Boost_LIBRARY_DIRS} )
	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Configure a list of packages
macro( Configure_Package_List )
	
	if( ${ARGC} GREATER 0 )
		
		foreach( pkg ${ARGV} )
			Configure_Package( ${pkg} )
		endforeach()
		
	endif()
	
endmacro()
#---------------------------------------------------------------------------------------------------------------------------
# Searches for directory where a package is located and sets the variable <PackageName>_DIR with the found location
# or <PackageName>_DIR-NOTFOUND if the search fails
macro( Find_Package_Directory PackageName )

if( DEFINED ENV{MAINPROJECT} )

	set( MAINPROJECT_DIR $ENV{MAINPROJECT} )
	
	string( TOLOWER PKG_ALT_NAME ${PackageName} )

	SubDirList( MAINPROJECT_SUBDIRS ${MAINPROJECT_DIR} )
	
	foreach( mp_dir ${MAINPROJECT_SUBDIRS} )
		set( SEARCH_DIRS ${SEARCH_DIRS} ${MAINPROJECT_DIR}/${mp_dir} )
	endforeach()
	
	set( PKG_SUFFIXES
		CMakeScripts
		CMake
		cpp
		${PackageName}
		${PackageName}/CMakeScripts
		${PackageName}/CMake
		)
		
	find_path(
		${PackageName}_DIR
		${PackageName}Config.cmake ${PKG_ALT_NAME}-config.cmake
		#HINTS ENV ${PackageName}_ROOT
		PATHS ${SEARCH_DIRS}
		PATH_SUFFIXES ${PKG_SUFFIXES} 
		#[DOC "cache documentation string"]
		NO_DEFAULT_PATH
		NO_CMAKE_ENVIRONMENT_PATH
		NO_CMAKE_PATH
		NO_SYSTEM_ENVIRONMENT_PATH
		NO_CMAKE_FIND_ROOT_PATH
	)
	
else()
	set( ${PackageName}_DIR-NOTFOUND )
endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
macro( Configure_Project_Dependencies )

	#if( ${ARGC} GREATER 2 )
		
		set( PROJ_NAME ${ARGV0} )
		
		foreach( pkg ${ARGV} )
			
			if( NOT ${pkg} STREQUAL ${PROJ_NAME} )
		
				if( ${PROJ_NAME}_PACKAGE_${pkg} )
					Configure_Package( ${pkg} )
					set( PROJ_DEP ${PROJ_DEP} ${pkg} )
				endif()

			endif()
		
		endforeach()

		Add_Project_USE_Flags( ${PROJ_NAME} ${PROJ_DEP} )

	#else()
	#	message( SEND_ERROR "Macro 'Configure_Project_Dependencies' called with insufficient number of parameters" )
	#endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
macro( Configure_Dependent_Library )

	if( NOT ( ${ARGC} LESS 2 ) )

		if( NOT TARGET ${ARGV0} )
			set( ${ARGV0}_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/${ARGV0} )
			add_subdirectory( ${ARGV1} ${${ARGV0}_BUILD_DIR} )
		endif()

		include_directories(
			${ARGV1}
			${ARGV1}/include
		) 

		link_directories(
			${${ARGV0}_BUILD_DIR}/${CMAKE_BUILD_TYPE}
		)

		Add_to_link_libraries( ${ARGV0} )
		
		Configure_Character_Encoding()

	else()
		message( SEND_ERROR "Macro Configure_Dependent_Library called with insufficient parameters" )
	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Test compiler version
macro( Configure_MSVC_Compiler_Version )

# Format generator name 'Visual Studio <version> ...' into 'vc<version>'
# Saves the result in the variable COMPILER_VERSION

if( MSVC )
	# Replace "Visual Studio" with "vc"
	string( REPLACE "Visual Studio " "vc" COMPILER_VERSION ${CMAKE_GENERATOR} )
	
	# Remove eventual trailing words in generator name
	string( REPLACE " 2010" "" COMPILER_VERSION ${COMPILER_VERSION} )
	string( REPLACE " 2012" "" COMPILER_VERSION ${COMPILER_VERSION} )
	string( REPLACE " 2013" "" COMPILER_VERSION ${COMPILER_VERSION} )
	string( REPLACE " ARM" "" COMPILER_VERSION ${COMPILER_VERSION} )
	string( REPLACE " Win64" "" COMPILER_VERSION ${COMPILER_VERSION} )
	
else()
	message( SEND_ERROR "This macro is meant to be used only with Microsoft Visual Studio compiler" )
endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Test Windows name
macro( Configure_WinOS_Name )

	if( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.2 )
		# Windows 8
		set( WIN_OS_NAME Win8 )
	
	elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.1 )
		#Windows 7
		set( WIN_OS_NAME Win7 )

	elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 6.0 )
		#Windows Vista or Server 2008
		set( WIN_OS_NAME WinVista )

	elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 5.2 )
		#Windows Server 2003 with SP1, Windows XP with SP2
		set( WIN_OS_NAME Win2003 )
	
	elseif( ${CMAKE_SYSTEM_VERSION} VERSION_EQUAL 5.1 )
		#Windows Server 2003, Windows XP
		set( WIN_OS_NAME WinXP )

	else()
		# VERSION_LESS 5.1 
		# Windows 2000
		set( WIN_OS_NAME Win2000 )
		
	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# configure include directory and library to link based in the assumption that the folder where header files
# is <LIB_..._BINARIES_DIR>/include and binaries are in <LIB_..._BINARIES_DIR>/lib/<OS>-<ARCHITECTURE>/
# THe library name should end in d.lib for debug and .lib for Release

macro( Configure_Library_Binaries )

	if( NOT ( ${ARGC} LESS 2 ) )

		set( LIB_${ARGV0}_BINARIES_DIR ${ARGV1} CACHE PATH "Where is your library binaries directory?" )
		
		include_directories( ${LIB_${ARGV0}_BINARIES_DIR}/include )

		#---------------------------------------------------------------------------
		# Test Operating System name
		Configure_WinOS_Name()

		#---------------------------------------------------------------------------
		# Test compiler version
		Configure_MSVC_Compiler_Version()

		#---------------------------------------------------------------------------
		
		Add_to_link_libraries(
			debug;${LIB_${ARGV0}_BINARIES_DIR}/lib/${WIN_OS_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${COMPILER_VERSION}/${ARGV0}d.lib;
			optimized;${LIB_${ARGV0}_BINARIES_DIR}/lib/${WIN_OS_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${COMPILER_VERSION}/${ARGV0}.lib;
			)

	else()

		message( SEND_ERROR "Macro Configure_Library_Binaries called with insufficient parameters" )

	endif()

endmacro()
	
#---------------------------------------------------------------------------------------------------------------------------
# Add proper definitions to compile code with MFC's
macro( Configure_MFC )
	add_definitions( -D_AFXDLL )	# use MFC's
	
	if( ${ARGC} GREATER 0 )

		if( ${ARGV0} STREQUAL "Static" )
			set( CMAKE_MFC_FLAG 1 )		# use MFC's as a static library
		else()
			set( CMAKE_MFC_FLAG 2 )		# use MFC's as a shared DLL
		endif()

	else()

		set( CMAKE_MFC_FLAG 2 )		# use MFC's as a shared DLL
	
	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Display a message containing the project, the generator and some system information
macro( Display_Config_Message )
	message( STATUS "Configuring project " ${PROJECT_NAME} " using " ${CMAKE_GENERATOR} " in " ${CMAKE_SYSTEM_NAME} " " ${CMAKE_SYSTEM_VERSION} )
endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Make sure that versions of Visual Studio before VS2010 recognize nullptr as NULL
macro( Handle_nullptr_compatibility )

	if( ${MSVC_VERSION} VERSION_LESS 1600 )
		add_definitions( -Dnullptr=NULL )
	endif( )
	
endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Configure the compiler to use unicode or ANSI
macro( Configure_Character_Encoding )

	if( NOT EXISTS CHARACTER_ENCODING )
		set( CHARACTER_ENCODING "Unicode" CACHE STRING "Select the desired character encoding" )
		set_property( CACHE CHARACTER_ENCODING PROPERTY STRINGS "Unicode" "ANSI" )
	endif()

	if( CHARACTER_ENCODING STREQUAL "Unicode" )
		add_definitions( 
			-DUNICODE
			-D_UNICODE	)
	endif()
	
endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Create a variable that informs what is the architecture being used x86 or x64.
# When using Visual Studio, it is created automatically, if not, a combo box is created with the options
macro( Configure_System_Architecture )

	# if using visual studio, configuration is automatic
	if( MSVC )
		if( CMAKE_CL_64 )
			set( SYSTEM_ARCH x64 )
		else()
			set( SYSTEM_ARCH x86 )
		endif()
		
	# if not, a combo box selection is presented
	else()

		set( PROCESSOR_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "Select the desired architecture" )
		set_property( CACHE PROCESSOR_ARCHITECTURE PROPERTY STRINGS "x86" "x64" )

		if( ${PROCESSOR_ARCHITECTURE} STREQUAL "x86" )
			set( SYSTEM_ARCH x86 )
		else()
			set( SYSTEM_ARCH x64 )
		endif()

	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Control heap allocation option in Visual Studio (Accepts one argument)
# < 0 or > 2000 - Uses default configuration set by CMake
# 0 - Removes the option
# [ 1 - 2000 ] - Sets the desired value
macro( Set_MSVC_Heap_Allocation )

	if( ${MSVC} AND ( ${ARGC} GREATER 0 ) )
	
		if( ( ${ARGV0} GREATER -1 ) AND ( ${ARGV0} LESS 2001 ) )
		
			set( CMAKE_DEFAULT_HEAP_ALLOCATION "/Zm1000" )
			
			if( ${ARGV0} EQUAL 0 )
				set( HEAP_VALUE_STR " " )
			else()
				set( HEAP_VALUE_STR "/Zm${ARGV0}" )
			endif()
		
			string( REPLACE ${CMAKE_DEFAULT_HEAP_ALLOCATION} ${HEAP_VALUE_STR} CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
			string( REPLACE ${CMAKE_DEFAULT_HEAP_ALLOCATION} ${HEAP_VALUE_STR} CMAKE_C_FLAGS ${CMAKE_C_FLAGS} )

		endif()

	endif()
	
endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Configure in the GUI the heap allocation option
macro( Configure_MSVC_Heap_Allocation )

	if( ${DEFAULT_OVERRIDE_MSVC_HEAP_ALLOCATION_SET} )
		set( MSVC_HEAP_BOOL ${DEFAULT_OVERRIDE_MSVC_HEAP_ALLOCATION_SET} )
	else()
		set( MSVC_HEAP_BOOL OFF )
	endif()
	
	set( ${PROJECT_NAME}_OVERRIDE_MSVC_HEAP_ALLOCATION ${MSVC_HEAP_BOOL} CACHE BOOL "Override Visual Studio compiler heap allocation? If disabled, the default value of '1000' defined by CMake will be used" )

	if( ${PROJECT_NAME}_OVERRIDE_MSVC_HEAP_ALLOCATION )

		if( ${DEFAULT_MSVC_HEAP_VALUE} )
			set( MSVC_HEAP_VALUE ${DEFAULT_MSVC_HEAP_VALUE} )
		else()
			set( MSVC_HEAP_VALUE "0" )
		endif()

		set( ${PROJECT_NAME}_HEAP_VALUE ${MSVC_HEAP_VALUE} CACHE STRING "Set to '0' to disable or a value between 1 and 2000 to enforce this value in the compiler" )
		
		Set_MSVC_Heap_Allocation( ${${PROJECT_NAME}_HEAP_VALUE} )
	
	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Setup the global parameters to apply to all projects that use MSVC Heap allocation override
# Parameter 1 - ON or OFF, controls the application of not of heap allocation override
# Parameter 2 - Numeric value, the desired value to apply according to macro 'Configure_MSVC_Heap_Allocation'
macro( Setup_Global_MSVC_Heap_Allocation )

	if( ${ARGC} EQUAL 2 )
		set( DEFAULT_OVERRIDE_MSVC_HEAP_ALLOCATION_SET ${ARGV0} )
		set( DEFAULT_MSVC_HEAP_VALUE "${ARGV1}" )
	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Add proper definitions based on a list of projects
# Based on a prefix indicated on the first argument, it tests if the package is being used and adds proper compile definitions
macro( Add_Project_USE_Flags )

	if( ${ARGC} GREATER 1 )

		set( pkg_prefix ${ARGV0} )
		
		foreach( arg ${ARGV} )
			
			if( NOT ${arg} STREQUAL ${pkg_prefix} )
			
				if( ${pkg_prefix}_PACKAGE_${arg} )
					add_definitions( -DUSE_${arg} )
				endif()
				
			endif()
		
		endforeach()

	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# This macro performs common configurations regarding package configuration, include directories and
# preprocessor definitions
# All subdirectories will be added as include directories
# If any extra configuration is needed, it has to be performed outside this macro
# Extra packages that exist in the system, can be passed as parameters if they have an environment variable
# defined or a <package_name>_DIR variable pointing to a valid configuration script
macro( Setup_Standard_Config )

	if( ${ARGC} GREATER 0 )
		Configure_Package_List( ${ARGV} )
	endif()

	#---------------------------------------------------------------------------
	# Create a list of all the subdirectories and add them as include directories, except 'build'
	set( APP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR} )
	
	SubDirList( SUB_DIRECTORIES_LIST ${CMAKE_CURRENT_SOURCE_DIR} )
	
	list( LENGTH SUB_DIRECTORIES_LIST NUMBER_SUBDIRS )
	
	if( ${NUMBER_SUBDIRS} GREATER 0 )
		list( REMOVE_ITEM SUB_DIRECTORIES_LIST build )
		set( APP_INCLUDE_DIRS ${APP_INCLUDE_DIRS} ${SUB_DIRECTORIES_LIST} )
	endif()
	
	include_directories( ${APP_INCLUDE_DIRS} )

	#---------------------------------------------------------------------------
	# Look for files of desired extensions in all directories
	set( APP_HEADER_EXTENSIONS
		h
		hpp
		inl
		)
		
	set( APP_SOURCE_EXTENSIONS
		c
		cpp
		cxx
		)
	

	set( APP_SOURCE_DIRECTORIES
		${CMAKE_CURRENT_SOURCE_DIR}
		${SUB_DIRECTORIES_LIST}
		)
	
	foreach( dir ${APP_SOURCE_DIRECTORIES} )
	
		foreach( hdr_ext ${APP_HEADER_EXTENSIONS} )
			file( GLOB HDR ${dir}/*.${hdr_ext} )
			set( HEADER_FILES ${HEADER_FILES} ${HDR} )
		endforeach()

		foreach( src_ext ${APP_SOURCE_EXTENSIONS} )
			file( GLOB SRC ${dir}/*.${src_ext} )
			set( SOURCE_FILES ${SOURCE_FILES} ${SRC} )
		endforeach()

	endforeach()
	
	#---------------------------------------------------------------------------
	# Resource files
	file( GLOB RESOURCE_FILES
		${CMAKE_CURRENT_SOURCE_DIR}/res/*.rc*
		${CMAKE_CURRENT_SOURCE_DIR}/res/*.ico )
	
	#---------------------------------------------------------------------------
	# File groups
	Create_File_Groups( "${HEADER_FILES}" "${SOURCE_FILES}" "${RESOURCE_FILES}" )

	set( PROJECT_FILES
		${SOURCE_FILES}
		${HEADER_FILES}
		${RESOURCE_FILES}
	)

	#---------------------------------------------------------------------------
	# Configure preprocessor definitions
	
	Handle_nullptr_compatibility()

	Configure_Windows_OS_Specific_Flags()
		
	Display_Config_Message()

	Configure_MSVC_Heap_Allocation()
	
endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# This macro configures a library with standard options regarding include directories and 
# files that are part of the project.
# All subdirectories will be added as include directories
# If any extra configuration is needed, it has to be performed outside this macro
# Extra packages that exist in the system, can be passed as parameters if they have an environment variable
# defined or a <package_name>_DIR variable pointing to a valid configuration script
macro( Setup_Library_Standard_Config )

	Setup_Standard_Config( ${ARGV} )

	if( NOT TARGET ${PROJECT_NAME} )
		add_library( ${PROJECT_NAME} ${PROJECT_FILES} )
		Get_link_libraries( LIST_LINK_LIBRARIES )
		target_link_libraries( ${PROJECT_NAME} ${LIST_LINK_LIBRARIES} )
	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# This macro configures an application with standard options regarding include directories and 
# files that are part of the project.
# All subdirectories will be added as include directories
# If any extra configuration is needed, it has to be performed outside this macro
# Extra packages that exist in the system, can be passed as parameters if they have an environment variable
# defined or a <package_name>_DIR variable pointing to a valid configuration script
macro( Setup_Application_Standard_Config )

	Setup_Standard_Config( ${ARGV} )

	if( NOT TARGET ${PROJECT_NAME} )
		add_executable( ${PROJECT_NAME} ${PROJECT_FILES} )
		Get_link_libraries( LIST_LINK_LIBRARIES )
		target_link_libraries( ${PROJECT_NAME} ${LIST_LINK_LIBRARIES} )
	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Creates a list of all the subdirectories that exist in 'current_dir'
macro( SubDirList directory_list current_dir )
	file( GLOB children RELATIVE ${current_dir} ${current_dir}/* )
	set( dir_list "" )

	foreach( child ${children} )
		if( IS_DIRECTORY ${current_dir}/${child} )
			set( dir_list ${dir_list} ${child} )
		endif()
	endforeach()

	set( ${directory_list} ${dir_list} )
endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Generates a header file that includes all headers in a specified directory
# Parameter 1 (mandatory) : directory to look for header files (.h*)
# Parameter 2 (optional)  : name of the file that will be generated (if not specified, the file will be called 'All.h')
macro( Generate_Header_File_Aggregator )
	
	if( ${ARGC} GREATER 0 )
		
		# Test if file name is specified
		if( ${ARGC} GREATER 1 )
			set( HEADER_FILENAME ${ARGV1} )
		else()
			set( HEADER_FILENAME All.h )
		endif()
	
		# Get specified directory
		set( HEADER_DIRECTORY ${ARGV0} )
		
		# Compose full file path
		set( FILE_PATH ${HEADER_DIRECTORY}/${HEADER_FILENAME} )
		
		# Create header definition to be used
		string( TOUPPER ${HEADER_FILENAME} HEADER_DEFINE ) 
		string( REPLACE "." "_" HEADER_DEFINE ${HEADER_DEFINE} )
		 
		# Compose file header with automatic message and preprocessor definitions to avoid multiple inclusions
		set( FILE_HEADER "// Header file automatically generated by CMake that aggregates all the headers.\n// Do not edit!\n\n" )
		set( FILE_HEADER ${FILE_HEADER} "#ifndef _${HEADER_DEFINE}_\n" )
		set( FILE_HEADER ${FILE_HEADER} "#define _${HEADER_DEFINE}_\n\n" )
		set( FILE_HEADER ${FILE_HEADER} "#pragma once\n\n" )
		
		# Write file header
		file( WRITE ${FILE_PATH} ${FILE_HEADER} )
		
		# Create list of all header files
		file( GLOB HEADERS_LIST RELATIVE ${HEADER_DIRECTORY} ${HEADER_DIRECTORY}/*.h* )

		# Insert include of each of the header files, except the file created by this script
		foreach( hdr ${HEADERS_LIST} )
			if( NOT ${hdr} MATCHES ${HEADER_FILENAME} )
				file( APPEND ${FILE_PATH} "#include \"${hdr}\"\n" )
			endif()
		endforeach()

		# Write the file ending
		file( APPEND ${FILE_PATH} "\n#endif // _${HEADER_DEFINE}_\n" )

	else()
		message( SEND_ERROR "Macro 'Generate_Header_File_Aggregator' called with incorrect number of arguments" )
	endif()

endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# macro that tests current OS and issues an error when the not the desired one
macro( Enforce_Supported_OS )

	if( ${ARGC} EQUAL 1 )

		# Test if target machine is running the desired OS
		if( NOT( ${CMAKE_SYSTEM_NAME} MATCHES ${ARGV0} ) )
			# if the target machine isn't the desired, issue an error
			message( SEND_ERROR "Current CMakeLists only supports ${ARGV0}!" )
		endif()

	else()
		message( SEND_ERROR "Macro 'Enforce_Supported_OS' requires one parameter" )
	endif()
	
endmacro()

#---------------------------------------------------------------------------------------------------------------------------
# Delay loading of some OpenCV modules to avoid getting wrong memory leak information
macro( Delay_Load_OpenCV_Modules )

if( NOT ${ARGC} GREATER 0 )
	message( "macro 'Delay_Load_OpenCV_DLLs' called without parameters. Doing nothing..." )

else()

	# Get a string with the OpenCV version
	set( OpenCV_VERSION_STR ${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH} )

	set( DELAY_OPENCV_MODULES_DEBUG "/DELAYLOAD:" )

	# Create the linker flags for debug
	foreach( module ${ARGV} )
		set( DELAY_OPENCV_MODULES_DEBUG 
			${DELAY_OPENCV_MODULES_DEBUG}
			"opencv_${module}${OpenCV_VERSION_STR}d.dll;"
			)
	endforeach()

	# Define the linker flags for debug only
	set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "${DELAY_OPENCV_MODULES_DEBUG}" )

endif()

endmacro()
