#--------------------------------------------------------------------------------------------
# Define CMake policy to be used on the project
cmake_minimum_required( VERSION 2.8 )
cmake_policy( VERSION ${CMAKE_VERSION} )

#--------------------------------------------------------------------------------------------
# Define Variables to store directories that will be used on the project
set( HOME $ENV{MAINPROJECT} )
set( LIBRARIES_DIR ${HOME}/Libraries )
set( HARDWARE_DIR ${HOME}/Hardware )

include( ${LIBRARIES_DIR}/CMakeScripts/CommonScripts.cmake )

#--------------------------------------------------------------------------------------------
# The project name
project( PHRI)

#--------------------------------------------------------------------------------------------
# Configuration of necessary packages	
	
	Configure_Package_List(
		YARP
		DataShare
		Matrix
		Utilities
		YarpCommunication
		)
	
#--------------------------------------------------------------------------------------------
# Definition of include directories

	# Directories with code to be compiled
	set( SRC_DIRS 
		${CMAKE_SOURCE_DIR} 
	)

	include_directories( 
		${SRC_DIRS}
		${CMAKE_SOURCE_DIR}/res
	)
		
#--------------------------------------------------------------------------------------------
# Create groups of files: Header, Source and Resource files
	
	set( SOURCE_EXTENSIONS *.cpp *.hpp )
	set( HEADER_EXTENSIONS *.h )

	# Look through all external source directories for source and header files
	foreach( source_dir IN ITEMS ${SRC_DIRS} )

		# Find source files in directories
		foreach( source_ext IN ITEMS ${SOURCE_EXTENSIONS} )
			file( GLOB_RECURSE FOUND_FILES ${source_dir}/${source_ext} )
			set( SOURCE_FILES ${SOURCE_FILES} ${FOUND_FILES} )
		endforeach( source_ext IN ITEMS ${SOURCE_EXTENSIONS} )
		
		# Find header files in directories
		foreach( header_ext IN ITEMS ${HEADER_EXTENSIONS} )
			file( GLOB_RECURSE FOUND_FILES ${source_dir}/${header_ext} )
			set( HEADER_FILES ${HEADER_FILES} ${FOUND_FILES} )
		endforeach( header_ext IN ITEMS ${HEADER_EXTENSIONS} )

	endforeach( source_dir IN ITEMS ${SRC_DIRS} )
	

	# Create a list with resource files
	file( GLOB RESOURCE_FILES "res/*.rc*" "res/*.ico" )
	
	# Exclude all source and header files in build folder
	file( GLOB_RECURSE FILES_TO_REMOVE ${CMAKE_CURRENT_BINARY_DIR}/*.* )
	list( REMOVE_ITEM HEADER_FILES ${FILES_TO_REMOVE} )
	list( REMOVE_ITEM SOURCE_FILES ${FILES_TO_REMOVE} )

	Create_File_Groups(  "${HEADER_FILES}" "${SOURCE_FILES}" "${RESOURCE_FILES}" )

#--------------------------------------------------------------------------------------------
# Define what files to be compiled for the application
	set( Project_App_SRCS
		${SOURCE_FILES}
		${HEADER_FILES}
		${RESOURCE_FILES}
	)

#--------------------------------------------------------------------------------------------
# Configure the compiler with the appropriate definitions to the used version
	Configure_WINVER()
	
	# disable function deprecation warning
	add_definitions( -D_CRT_NONSTDC_NO_DEPRECATE )

	Display_Config_Message()
	
#--------------------------------------------------------------------------------------------
# Preprocessor definitions
	
	foreach( ext_pkg IN ITEMS ${EXTERNAL_PACKAGES} )
		# General dependency from project to each external package
		# Create a preprocessor definition that specifies if that external package is used
		if( ${ext_pkg}_FOUND )
			add_definitions( -DUSE_${ext_pkg} )
		endif( ${ext_pkg}_FOUND )
	endforeach( ext_pkg IN ITEMS ${EXTERNAL_PACKAGES} )

	Configure_MFC() # comment this line for console program
#--------------------------------------------------------------------------------------------
# Create an executable for the application
	add_executable( ${PROJECT_NAME} WIN32 ${Project_App_SRCS} )
	# add_executable( ${PROJECT_NAME} ${Project_App_SRCS} ) #Console program
	
	Link_libraries_to_project()

#--------------------------------------------------------------------------------------------
# Configure project dependencies of other projects and respective libraries
	
	#add_definitions( -DUSE_YarpCommunication )
	#add_dependencies( ${PROJECT_NAME} YarpCommunication )
	
#--------------------------------------------------------------------------------------------
# Required Libraries to be linked with the executable
	
	#target_link_libraries( ${PROJECT_NAME}
	#	YarpCommunication
	#)
